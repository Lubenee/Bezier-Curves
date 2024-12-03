#include "ImGuiManager.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/**
 * @brief Constructor for the ImGuiManager.
 *
 * Initializes the internal GLFW window pointer to nullptr, indicating that ImGui
 * has not yet been initialized.
 */
ImGuiManager::ImGuiManager()
    : window(nullptr), initialized(false) {}

/**
 * @brief Destructor for the ImGuiManager.
 *
 * Ensures that any remaining ImGui resources are properly cleaned up.
 * Calls Cleanup() to shut down ImGui resources if they haven't been cleaned up already.
 */
ImGuiManager::~ImGuiManager()
{
    this->cleanup();
}

/**
 * @brief Initializes ImGui with the given GLFW window.
 *
 * This method initializes the ImGui context, sets up the input handling through GLFW,
 * and configures the rendering backend for OpenGL.
 *
 * @param window A pointer to the GLFW window used for rendering.
 */
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

/**
 * @brief Cleans up ImGui resources.
 *
 * Shuts down the GLFW and OpenGL backends used by ImGui and destroys the ImGui context.
 * This function should be called before the application exits to release resources properly.
 */
void ImGuiManager::cleanup()
{
    if (!initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
}

/**
 * @brief Prepares a new ImGui frame.
 *
 * This method should be called at the start of each render loop. It initializes
 * a new ImGui frame, allowing you to define UI elements for the current frame.
 */
void ImGuiManager::newFrame()
{
    if (!initialized)
        return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/**
 * @brief Renders ImGui components to the screen.
 *
 * This method finalizes the ImGui frame and issues the drawing commands
 * necessary to render ImGui components to the window.
 */
void ImGuiManager::render()
{
    if (!initialized)
        return;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
