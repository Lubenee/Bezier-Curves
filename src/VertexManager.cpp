#include "VertexManager.hpp"
#include "Utils.hpp"
#include "MathUtils.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

VertexManager::VertexManager()
    : points(), draggedPointIndex(-1)
{
    controlPointShaderProgram = Utils::createShaderProgram("./Shaders/ControlPoints/vert_shader.vert", "./Shaders/ControlPoints/frag_shader.frag");
    bezierCurveShaderProgram = Utils::createShaderProgram("./Shaders/BezierCurve/vert_shader.vert", "./Shaders/BezierCurve/frag_shader.frag");

    glGenVertexArrays(vaoSets::Count, vao);
    glGenBuffers(vaoSets::Count, vbo);

    // Bind the first VAO
    glBindVertexArray(vao[vaoSets::ControlPoints]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0); // x, y (2 floats)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Bind the second VAO
    glBindVertexArray(vao[vaoSets::TransitionPoints]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Bind the third VAO
    glBindVertexArray(vao[vaoSets::BezierCurve]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);
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

void VertexManager::calculateBezierCurve(int segments)
{
    const float step = 1.0f / segments;
    curvePoints.clear();
    for (float k = 0.0f; k <= 1.0f; k += step)
        curvePoints.push_back(MathUtils::evaluateBezier(controlPoints, k));
    curvePoints.push_back(MathUtils::evaluateBezier(controlPoints, 1.f));
}

void VertexManager::addPoint(float x, float y)
{
    points.emplace_back(x, y);
    controlPoints.emplace_back(x, y);
    this->transitionPoints = MathUtils::getBezierTransitionPoints(points, t);
    calculateBezierCurve(highResNumSegments);
    lowResNumSegments = Utils::calculateLowResNumSegments(points.size());
    for (int i = 0; i < vaoSets::Count; ++i)
        updateVBO(vaoSets(i));
}

void VertexManager::translatePoint(float x, float y, std::size_t pointIndex)
{
    points[pointIndex].x = x;
    points[pointIndex].y = y;
    controlPoints[pointIndex].x = x;
    controlPoints[pointIndex].y = y;
    this->transitionPoints = MathUtils::getBezierTransitionPoints(points, t);

    if (draggedPointIndex != NO_DRAG)
        calculateBezierCurve(lowResNumSegments);

    for (int i = 0; i < vaoSets::Count; ++i)
        updateVBO(vaoSets(i));
}

void VertexManager::updateVBO(vaoSets loc)
{
    glBindVertexArray(vao[loc]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[loc]);

    if (loc == VertexManager::vaoSets::ControlPoints)
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), points.data(), GL_DYNAMIC_DRAW);
    else if (loc == VertexManager::vaoSets::TransitionPoints)
        glBufferData(GL_ARRAY_BUFFER, transitionPoints.size() * sizeof(glm::vec2), transitionPoints.data(), GL_DYNAMIC_DRAW);
    else if (loc == VertexManager::vaoSets::BezierCurve)
        glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(glm::vec2), curvePoints.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexManager::renderPoints() const
{
    glUseProgram(controlPointShaderProgram);
    glPointSize(12.f);
    glBindVertexArray(vao[VertexManager::vaoSets::ControlPoints]);
    glDrawArrays(GL_POINTS, 0, points.size());
    glBindVertexArray(0);
}

void VertexManager::renderTransitionPoints() const
{
    glUseProgram(bezierCurveShaderProgram);
    glLineWidth(3.5f);
    glBindVertexArray(vao[VertexManager::vaoSets::TransitionPoints]);

    std::size_t controlPointCount = points.size();
    int idx = 0, count = 0;
    for (size_t i = 0; i < transitionPoints.size(); ++i)
    {
        count++;
        if (count == controlPointCount)
        { // Draw line stripes by levels so that they connect properly
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
    glUseProgram(controlPointShaderProgram);
    glBindVertexArray(vao[VertexManager::vaoSets::BezierCurve]);
    glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
    glBindVertexArray(0);
}