#if !defined(GRID_H)
#define GRID_H
#include <memory>

#include "InputManager.hpp"
#include "Utils.hpp"

class Grid
{
public:
    Grid(GLFWwindow *window) : window(window)
    {
        input = std::make_unique<InputManager>(window, vertices);
        input->setCallbacks();
    }
    ~Grid() = default;

    void update(double dt) {}
    void render()
    {
        vertices.renderPoints();
        vertices.renderTransitionPoints();
        vertices.renderCurve();
    }

private:
    GLFWwindow *window;

    std::unique_ptr<InputManager> input;
    VertexManager vertices;
};

#endif // GRID_H
