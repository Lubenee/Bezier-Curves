#include "VertexManager.hpp"
#include "Utils.hpp"
#include "MathUtils.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

VertexManager::VertexManager()
    : draggedPointIndex(-1)
{
    controlPointShaderProgram = Utils::createShaderProgram("./Shaders/ControlPoints/vert_shader.vert", "./Shaders/ControlPoints/frag_shader.frag");
    bezierCurveShaderProgram = Utils::createShaderProgram("./Shaders/BezierCurve/vert_shader.vert", "./Shaders/BezierCurve/frag_shader.frag");
    transitionPointsShaderProgram = Utils::createShaderProgram("./Shaders/TransitionPoints/transition_points.vert", "./Shaders/TransitionPoints/transition_points.frag");
    hodographControlPointsShaderProgram = Utils::createShaderProgram("./Shaders/HodographControlPoints/vert_shader.vert", "./Shaders/HodographControlPoints/frag_shader.frag");

    glGenVertexArrays(vaoSets::Count, vao);
    glGenBuffers(vaoSets::Count, vbo);

    setupVAOs(vao[vaoSets::ControlPoints], vbo[0]);
    setupVAOs(vao[vaoSets::TransitionPoints], vbo[1]);
    setupVAOs(vao[vaoSets::BezierCurve], vbo[2]);
    setupVAOs(vao[vaoSets::Hodograph], vbo[3]);
    setupVAOs(vao[vaoSets::HodographControlPoints], vbo[4]);

    waveAmplitudeLocation = glGetUniformLocation(bezierCurveShaderProgram, "waveAmplitude");
    waveFrequencyLocation = glGetUniformLocation(bezierCurveShaderProgram, "waveFrequency");
    timeLocation = glGetUniformLocation(bezierCurveShaderProgram, "time");
    pulseSpeedLocation = glGetUniformLocation(bezierCurveShaderProgram, "pulseSpeed");
}

void VertexManager::setupVAOs(GLuint vao, GLuint vbo)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0); // x, y (2 floats)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

VertexManager::~VertexManager()
{
    // cleanup
    glDeleteBuffers(vaoSets::Count, vbo);
    glDeleteVertexArrays(vaoSets::Count, vao);
}

void VertexManager::addImguiTabs()
{
    if (ImGui::BeginTabItem("Bezier curve"))
    {
        if (ImGui::Button("Clear curve"))
        {
            controlPoints.clear();
            transitionPoints.clear();
            curvePoints.clear();
            hodographControlPoints.clear();
            hodographPoints.clear();
            for (int i = 0; i < vaoSets::Count; ++i)
                updateVBO(vaoSets(i));
        }

        ImGui::Separator();

        if (ImGui::SliderFloat("t", &this->t, 0.f, 1.f))
        {
            this->transitionPoints = MathUtils::getBezierTransitionPoints(controlPoints, t);
            this->updateVBO(vaoSets::TransitionPoints);
        }

        ImGui::SliderFloat("Control Points Size", &this->controlPointsSize, 1.f, 20.f);
        ImGui::SliderFloat("Bezier Curve Line Width", &this->bezierCurveLineWidth, 1.f, 20.f);
        ImGui::SliderFloat("Transition Points Line Width", &this->transitionPointsLineWidth, 1.f, 20.f);

        if (ImGui::SliderInt("Smoothness", &this->highResNumSegments, 10, 350))
        {
            calculateBezierCurve(highResNumSegments);
            this->updateVBO(vaoSets::BezierCurve);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("WARNING! WITH TOO MANY CONTROL POINTS, APPLICATION MIGHT CRASH IF YOU SET THE VALUE TOO HIGH.\nAdjust the smoothness of the Bezier curve. Higher values result in a smoother curve.");

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Hodograph"))
    {
        if (ImGui::SliderFloat("Scale Factor", &hodographScaleFactor, 0.f, 1.0f))
        {
            calculateHodograph();
            updateVBO(vaoSets::HodographControlPoints);
        }
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Animate"))
    {
        ImGui::Text("Oscilations");
        ImGui::Separator();
        ImGui::SliderFloat("Wave Amplitude", &waveAmplitude, 0.f, 1.0f);
        ImGui::SliderFloat("Wave Frequency", &waveFrequency, 0.f, 10.0f);
        ImGui::SliderFloat("Pulse Speed", &pulseSpeed, 0.f, 5.0f);

        ImGui::EndTabItem();
    }
}

void VertexManager::calculateBezierCurve(int segments)
{
    if (!controlPoints.size())
        return;
    const float step = 1.0f / segments;
    curvePoints.clear();
    for (float k = 0.0f; k <= 1.0f; k += step)
        curvePoints.push_back(MathUtils::evaluateBezier(controlPoints, k));
    curvePoints.push_back(MathUtils::evaluateBezier(controlPoints, 1.f));
}

void VertexManager::calculateHodograph()
{
    if (controlPoints.size() < 2)
        return;

    hodographPoints.clear();
    hodographControlPoints.clear();

    int n = controlPoints.size() - 1;
    for (size_t i = 0; i < controlPoints.size() - 1; ++i)
        hodographControlPoints.push_back(static_cast<float>(n) * (controlPoints[i + 1] - controlPoints[i]));

    // Downscale or upscale the hodograph to fit it inside the screen
    for (auto &point : hodographControlPoints)
        point *= this->hodographScaleFactor;

    // Compute hodograph curve points
    int segments = highResNumSegments;
    float step = 1.0f / segments;
    for (float t = 0.0f; t <= 1.0f; t += step)
        hodographPoints.push_back(MathUtils::evaluateBezier(hodographControlPoints, t));
    hodographPoints.push_back(MathUtils::evaluateBezier(hodographControlPoints, 1.f));

    // Update VBO
    updateVBO(vaoSets::Hodograph);
}

void VertexManager::recalculateAllVertices()
{
    this->transitionPoints = MathUtils::getBezierTransitionPoints(controlPoints, t);
    calculateHodograph();
    if (draggedPointIndex != NO_DRAG)
        calculateBezierCurve(lowResNumSegments);
    else
        calculateBezierCurve(highResNumSegments);
    lowResNumSegments = Utils::calculateLowResNumSegments(controlPoints.size());
    for (int i = 0; i < vaoSets::Count; ++i)
        updateVBO(vaoSets(i));
}

void VertexManager::addPoint(float x, float y)
{
    controlPoints.emplace_back(x, y);
    recalculateAllVertices();
}

void VertexManager::removePoint(std::size_t index)
{
    if (index >= controlPoints.size())
        return;
    controlPoints.erase(controlPoints.begin() + index);
    recalculateAllVertices();
}

void VertexManager::translatePoint(float x, float y, std::size_t pointIndex)
{
    if (pointIndex >= controlPoints.size())
        return;
    controlPoints[pointIndex] = glm::vec2(x, y);
    recalculateAllVertices();
}

void VertexManager::updateVBO(vaoSets loc)
{
    glBindVertexArray(vao[loc]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[loc]);

    if (loc == VertexManager::vaoSets::ControlPoints)
        glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec2), controlPoints.data(), GL_DYNAMIC_DRAW);
    else if (loc == VertexManager::vaoSets::TransitionPoints)
        glBufferData(GL_ARRAY_BUFFER, transitionPoints.size() * sizeof(glm::vec2), transitionPoints.data(), GL_DYNAMIC_DRAW);
    else if (loc == VertexManager::vaoSets::BezierCurve)
        glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(glm::vec2), curvePoints.data(), GL_DYNAMIC_DRAW);
    else if (loc == VertexManager::vaoSets::Hodograph)
        glBufferData(GL_ARRAY_BUFFER, hodographPoints.size() * sizeof(glm::vec2), hodographPoints.data(), GL_DYNAMIC_DRAW);
    else if (loc == VertexManager::vaoSets::HodographControlPoints)
        glBufferData(GL_ARRAY_BUFFER, hodographControlPoints.size() * sizeof(glm::vec2), hodographControlPoints.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexManager::renderPoints() const
{
    glUseProgram(controlPointShaderProgram);
    glPointSize(controlPointsSize);
    glBindVertexArray(vao[VertexManager::vaoSets::ControlPoints]);
    glDrawArrays(GL_POINTS, 0, controlPoints.size());
    glBindVertexArray(0);
}

void VertexManager::renderTransitionPoints() const
{
    glUseProgram(transitionPointsShaderProgram);
    glLineWidth(transitionPointsLineWidth);
    glBindVertexArray(vao[VertexManager::vaoSets::TransitionPoints]);

    // Draw line strips by levels so that they connect properly
    // Otherwise its' a mess because points from different levels of the De Casteljau algorithm connect, and they shouldn't most of the time
    std::size_t controlPointCount = controlPoints.size();
    int idx = 0, count = 0;
    for (size_t i = 0; i < transitionPoints.size(); ++i)
    {
        count++;
        if (count == controlPointCount)
        {
            glDrawArrays(GL_LINE_STRIP, idx, controlPointCount);
            idx += controlPointCount;
            controlPointCount--;
            count = 0;
        }
    }
    glBindVertexArray(0);
}

void VertexManager::renderCurve() const
{
    glUseProgram(bezierCurveShaderProgram);
    glLineWidth(bezierCurveLineWidth);

    float time = glfwGetTime();
    glUniform1f(timeLocation, time);
    glUniform1f(waveAmplitudeLocation, waveAmplitude);
    glUniform1f(waveFrequencyLocation, waveFrequency);
    glUniform1f(pulseSpeedLocation, pulseSpeed);

    glBindVertexArray(vao[VertexManager::vaoSets::BezierCurve]);
    glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
    glBindVertexArray(0);
}

void VertexManager::renderHodograph() const
{
    glUseProgram(transitionPointsShaderProgram);
    glLineWidth(transitionPointsLineWidth);
    glBindVertexArray(vao[VertexManager::vaoSets::Hodograph]);
    glDrawArrays(GL_LINE_STRIP, 0, hodographPoints.size());
    glBindVertexArray(0);
}

void VertexManager::renderHodographControlPoints() const
{
    glUseProgram(hodographControlPointsShaderProgram);
    glPointSize(6.f);
    glLineWidth(2.f);
    glBindVertexArray(vao[VertexManager::vaoSets::HodographControlPoints]);
    glDrawArrays(GL_POINTS, 0, hodographControlPoints.size());
    glDrawArrays(GL_LINE_STRIP, 0, hodographControlPoints.size());

    glBindVertexArray(0);
}
