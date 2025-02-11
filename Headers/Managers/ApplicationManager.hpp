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
};

#endif // APPLICATION_MANAGER_H
