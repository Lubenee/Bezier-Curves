#include "InputManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include <imgui_impl_glfw.h>
#include <imgui.h>

InputManager::InputManager(GLFWwindow *window, VertexManager &vertexManager)
    : window(window), vertexManager(vertexManager)
{
    // Set this instance as the user pointer for the window
    glfwSetWindowUserPointer(window, this);
}

void InputManager::setCallbacks()
{
    // Set the mouse callback
    glfwSetMouseButtonCallback(window, InputManager::mouseCallback);
    glfwSetCursorPosCallback(window, InputManager::cursorPositionCallback);
    glfwSetKeyCallback(window, InputManager::keyboardCallback);
}

void InputManager::mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    auto &io = ImGui::GetIO();
    // Forward the action to ImGui first to see if the user is clicking within the GUI itself
    io.AddMouseButtonEvent(button, action);

    // If the click is inside the GUI, discard mouse input
    if (io.WantCaptureMouse)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Retrieve the InputManager instance from the user pointer
        InputManager *inputHandler = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        float worldX, worldY;
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        convertScreenToWorld(xpos, ypos, width, height, worldX, worldY);

        if (inputHandler->vertexManager.draggedPointIndex == NO_DRAG)
        {
            // not dragging, add new point
            inputHandler->vertexManager.addPoint(worldX, worldY);
        }
        else
        {
            // stop dragging
            inputHandler->vertexManager.draggedPointIndex = NO_DRAG;
            inputHandler->vertexManager.calculateBezierCurve(inputHandler->vertexManager.highResNumSegments);
            inputHandler->vertexManager.updateVBO(VertexManager::vaoSets::BezierCurve);
        }
    }

    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    { // TODO: remove a point
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        InputManager *inputHandler = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float worldX, worldY;
        convertScreenToWorld(xpos, ypos, width, height, worldX, worldY);

        // Check if the mouse is inside any existing point
        for (std::size_t i = 0; i < inputHandler->vertexManager.controlPoints.size(); ++i)
        {
            const auto &point = inputHandler->vertexManager.controlPoints[i];
            if (Utils::isPointBeingHovered(worldX, worldY, point.x, point.y))
            {
                inputHandler->vertexManager.draggedPointIndex = i;
                break;
            }
        }
    }

    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        InputManager *inputHandler = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float worldX, worldY;
        convertScreenToWorld(xpos, ypos, width, height, worldX, worldY);

        // Check if the mouse is inside any existing point
        for (std::size_t i = 0; i < inputHandler->vertexManager.controlPoints.size(); ++i)
        {
            const auto &point = inputHandler->vertexManager.controlPoints[i];
            if (Utils::isPointBeingHovered(worldX, worldY, point.x, point.y))
            {
                inputHandler->vertexManager.removePoint(i);
                break;
            }
        }
    }
}

void InputManager::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    // used to forward the mouse movement (cursor pos) to ImGui from GLFW's native cursor position callback.
    // allows ImGui to correctly track the cursor's position in relation to any ImGui windows.
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    InputManager *inputHandler = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
    // if a point is being dragged, update its position
    if (inputHandler->vertexManager.draggedPointIndex != NO_DRAG)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float worldX, worldY;
        convertScreenToWorld(xpos, ypos, width, height, worldX, worldY);

        inputHandler->vertexManager.translatePoint(worldX, worldY, inputHandler->vertexManager.draggedPointIndex);
    }
}

void InputManager::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Z && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE)
    {
        // TODO: Implement a command stack.
        std::cout << "Ctrl + Z" << std::endl;
    }
}

// convert screen to world coordinates
void InputManager::convertScreenToWorld(double xpos, double ypos, GLint windowWidth, GLint windowHeight, float &worldX, float &worldY)
{
    worldX = (2.0f * xpos / windowWidth) - 1.0f;
    worldY = 1.0f - (2.0f * ypos / windowHeight);
}
