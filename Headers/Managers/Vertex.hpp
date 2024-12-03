#if !defined(VERTEX_H)
#define VERTEX_H

#include "GLFW/glfw3.h"

struct Vertex
{
public:
    Vertex() : x(0.f), y(0.f) {}
    Vertex(float x, float y) : x(x), y(y) {}

    GLboolean isMouseOver(float mouseX, float mouseY) const
    {
        float threshhold = 0.02;
        return (mouseX >= x - threshhold && mouseX <= x + threshhold &&
                mouseY >= y - threshhold && mouseY <= y + threshhold);
    }

    operator glm::vec2() const
    {
        return glm::vec2(x, y);
    }

public:
    float x, y;

private:
    bool isDragged = false;
};

#endif // VERTEX_H
