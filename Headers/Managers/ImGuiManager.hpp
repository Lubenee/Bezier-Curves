#if !defined(IM_GUI_MANAGER)
#define IM_GUI_MANAGER

#include <GLFW/glfw3.h>

/**
 * @class ImGuiManager
 * @brief A class that manages the lifecycle of ImGui, handling its initialization,
 *        rendering, and cleanup.
 *
 * The ImGuiManager class is responsible for setting up ImGui, starting a new frame,
 * rendering ImGui components, and cleaning up resources when the application closes.
 */
class ImGuiManager
{
public:
    ImGuiManager();
    ~ImGuiManager();

    /**
     * @brief Initializes ImGui with the given GLFW window.
     *
     * Sets up the ImGui context, links it with GLFW for input handling, and initializes
     * the OpenGL backend for rendering ImGui.
     *
     * @param window A pointer to the GLFW window used for rendering.
     */
    void init(GLFWwindow *window);

    /**
     * @brief Prepares a new ImGui frame.
     *
     * This function should be called at the beginning of each render loop iteration.
     * It sets up a new ImGui frame, ready to accept input and rendering commands.
     */
    void newFrame();

    /**
     * @brief Renders ImGui components to the screen.
     *
     * This function finalizes the current ImGui frame and issues OpenGL commands
     * to draw ImGui components. It should be called at the end of each render loop
     * iteration after updating ImGui UI elements.
     */
    void render();

private:
    /**
     * @brief Cleans up ImGui resources.
     *
     * Shuts down ImGui's GLFW and OpenGL backends and destroys the ImGui context.
     * This should be called before closing the application to prevent resource leaks.
     */
    void cleanup();

private:
    GLFWwindow *window;
    bool initialized;
};

#endif // IM_GUI_MANAGER
