#include "ImGuiManager.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
using namespace ImGui;

ImGuiManager::ImGuiManager()
    : window(nullptr), initialized(false) {}

ImGuiManager::~ImGuiManager()
{
    this->cleanup();
}

void ImGuiManager::init(GLFWwindow *window)
{
    if (initialized)
        return;
    this->window = window;

    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Initialize ImGui with GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Set ImGui style
    ImGui::StyleColorsDark();
    initialized = true;
}

void ImGuiManager::cleanup()
{
    if (!initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
}

void ImGuiManager::setNextWindowContext()
{
    SetNextWindowPos(ImVec2(0.f, 0.f));
    SetNextWindowSize(ImVec2(150.f, 300.f));
}

void ImGuiManager::newFrame()
{
    if (!initialized)
        return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::render()
{
    if (!initialized)
        return;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
