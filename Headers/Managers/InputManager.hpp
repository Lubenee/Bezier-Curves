#if !defined(INPUT_HANDLER_H)
#define INPUT_HANDLER_H

#include "VertexManager.hpp"
#include <GLFW/glfw3.h>

class InputManager
{
public:
    InputManager(GLFWwindow *window, VertexManager &vertexManager);
    InputManager(const InputManager &) = delete;
    InputManager(const InputManager &&) = delete;
    InputManager &operator=(const InputManager &) = delete;
    ~InputManager() = default;

    // Convert screen coordinates to world coordinates
    static void convertScreenToWorld(double xpos, double ypos, GLint windowWidth, GLint windowHeight, float &worldX, float &worldY);
    void setCallbacks();

private:
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    static void keyboardCallback();

    GLFWwindow *window;
    VertexManager &vertexManager;
};

#endif // INPUT_HANDLER_H
