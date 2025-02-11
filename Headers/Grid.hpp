#if !defined(GRID_H)
#define GRID_H
#include <memory>
#include <imgui.h>

#include "InputManager.hpp"
#include "Utils.hpp"

class Grid
{
public:
    Grid(GLFWwindow *window) : window(window),
                               input(), vertices(),
                               renderControlPoints(true),
                               renderTransitionPoints(false),
                               renderBezierCurve(true),
                               renderHodograph(true),
                               renderHodographControlPoints(true)
    {
        input = std::make_unique<InputManager>(window, vertices);
        input->setCallbacks();
    }
    ~Grid() = default;

    void update(double dt) {}

    void addImguiTabs()
    {
        if (ImGui::BeginTabBar("Tabs"))
        { // This Tab Bar segment gets closed in the render method.
          // It should be here, and not there, but im lazy
            if (ImGui::BeginTabItem("Renderer"))
            {
                ImGui::Text("What do you want to render?");
                ImGui::Separator();
                ImGui::Checkbox("Bezier Curve", &renderBezierCurve);
                ImGui::Checkbox("Bezier Curve Control Points", &renderControlPoints);
                ImGui::Checkbox("Bezier Transition Points", &renderTransitionPoints);
                ImGui::Checkbox("Hodograph", &renderHodograph);
                ImGui::Checkbox("Hodograph Control Points", &renderHodographControlPoints);
                ImGui::EndTabItem();
            }
        }
    }

    void handleProjectionAndViewMatrices()
    {
    }

    void render()
    {
        addImguiTabs();
        vertices.addImguiTabs();
        ImGui::EndTabBar();

        if (renderHodograph)
            vertices.renderHodograph();
        if (renderHodographControlPoints)
            vertices.renderHodographControlPoints();
        if (renderControlPoints)
            vertices.renderPoints();
        if (renderTransitionPoints)
            vertices.renderTransitionPoints();
        if (renderBezierCurve)
            vertices.renderCurve();
    }

private:
    GLFWwindow *window;

    // CAMERA
    float cameraX = 0.f, cameraY = 0.f, cameraZ = 13.5f;
    float fov = 1.0472f, nearPlane = 0.1f, farPlane = 1000.f, aspect;

    // Projection & View Matrices & locations for shaders
    glm::mat4 projMatrix, viewMatrix;
    GLuint projLoc, viewLoc;

    std::unique_ptr<InputManager> input;
    VertexManager vertices;

    bool renderControlPoints;
    bool renderTransitionPoints;
    bool renderBezierCurve;
    bool renderHodograph;
    bool renderHodographControlPoints;
};

#endif // GRID_H
