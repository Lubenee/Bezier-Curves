#if !defined(VERTEX_MANAGER_H)
#define VERTEX_MANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Vertex.hpp"

#define NO_DRAG -1

class VertexManager
{
public:
    enum vaoSets
    {
        ControlPoints = 0,
        TransitionPoints,
        BezierCurve,
        Hodograph,
        HodographControlPoints,
        Count
    };
    friend class InputManager;

    VertexManager();
    ~VertexManager();

    // Add a point to the list
    void addPoint(float x, float y);
    void removePoint(std::size_t index);
    void translatePoint(float x, float y, std::size_t pointIndex);

    void updateVBO(vaoSets);
    void addImguiTabs();

    void renderPoints() const;
    void renderTransitionPoints() const;
    void renderCurve() const;
    void renderHodograph() const;
    void renderHodographControlPoints() const;

private:
    void setupVAOs(GLuint vao, GLuint vbo);
    void calculateBezierCurve(int segments);
    void calculateHodograph();

    void recalculateAllVertices();

private:
    std::vector<glm::vec2> controlPoints;
    std::vector<glm::vec2> transitionPoints;
    std::vector<glm::vec2> curvePoints;
    std::vector<glm::vec2> hodographControlPoints;
    std::vector<glm::vec2> hodographPoints;

    float t = 0.5;
    int highResNumSegments = 150;
    int lowResNumSegments = 25;

    float controlPointsSize = 12.f;
    float transitionPointsLineWidth = 3.5f;
    float bezierCurveLineWidth = 2.5f;
    float hodographScaleFactor = 0.1f;

    GLint draggedPointIndex;
    GLuint vao[vaoSets::Count];
    GLuint vbo[vaoSets::Count];

    // Oscilations stuff
    GLuint waveAmplitudeLocation;
    GLuint waveFrequencyLocation;
    GLuint timeLocation;
    GLuint pulseSpeedLocation;

    float waveAmplitude = 0.f;
    float waveFrequency = 0.f;
    float pulseSpeed = 0.f;

    GLuint controlPointShaderProgram;
    GLuint bezierCurveShaderProgram;
    GLuint transitionPointsShaderProgram;
    GLuint hodographControlPointsShaderProgram;
};

#endif // VERTEX_MANAGER_H
