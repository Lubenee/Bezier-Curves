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
        Count
    };
    friend class InputManager;

    VertexManager();
    ~VertexManager();

    // Add a point to the list
    void addPoint(float x, float y);

    void translatePoint(float x, float y, std::size_t pointIndex);

    void renderPoints() const;
    void renderTransitionPoints() const;
    void renderCurve() const;
    void updateVBO(vaoSets);

private:
    void calculateBezierCurve(int segments);

private:
    std::vector<Vertex> points;

    std::vector<glm::vec2> controlPoints;
    std::vector<glm::vec2> transitionPoints;
    std::vector<glm::vec2> curvePoints;

    float t = 0.5;
    const int highResNumSegments = 150;
    int lowResNumSegments = 25;

    GLint draggedPointIndex;
    GLuint vao[vaoSets::Count];
    GLuint vbo[vaoSets::Count];

    GLuint controlPointShaderProgram;
    GLuint bezierCurveShaderProgram;
};

#endif // VERTEX_MANAGER_H
