#if !defined(APPLICATION_MANAGER_H)
#define APPLICATION_MANAGER_H

#include "Grid.hpp"
#include "ImGuiManager.hpp"

#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ApplicationManager
{
public:
    ApplicationManager();
    ApplicationManager(const ApplicationManager &) = delete;
    ApplicationManager &operator=(const ApplicationManager &) = delete;
    ~ApplicationManager();

    void run();

private:
    void update(double dt);
    void render();

private:
    GLFWwindow *window;
    const GLint windowWidth, windowHeight;
    const char *windowName;

    // ImGui
    ImGuiManager imgui;

    // Everything is rendered on the grid
    std::unique_ptr<Grid> grid;

    // CAMERA
    float cameraX = 0.f, cameraY = 0.f, cameraZ = 13.5f;
    float fov = 1.0472f, nearPlane = 0.1f, farPlane = 1000.f, aspect;

    // Projection & View Matrices & locations for shaders
    glm::mat4 projMatrix, viewMatrix;
    GLuint projLoc, viewLoc;

    // Rendering Program
    GLuint renderingProgram;
};

#endif // APPLICATION_MANAGER_H
