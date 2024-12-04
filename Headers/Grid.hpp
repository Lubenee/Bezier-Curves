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
                               renderBezierCurve(true)
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
                ImGui::Checkbox("Control Points", &renderControlPoints);
                ImGui::Checkbox("Transition Points", &renderTransitionPoints);
                ImGui::Checkbox("Bezier Curve", &renderBezierCurve);

                ImGui::EndTabItem();
            }
        }
    }

    void render()
    {
        addImguiTabs();
        vertices.addImguiTabs();
        ImGui::EndTabBar();

        if (renderControlPoints)
            vertices.renderPoints();
        if (renderTransitionPoints)
            vertices.renderTransitionPoints();
        if (renderBezierCurve)
            vertices.renderCurve();
    }

private:
    GLFWwindow *window;

    std::unique_ptr<InputManager> input;
    VertexManager vertices;

    bool renderControlPoints;
    bool renderTransitionPoints;
    bool renderBezierCurve;
};

#endif // GRID_H
