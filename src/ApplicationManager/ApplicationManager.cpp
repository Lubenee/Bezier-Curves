#include "ApplicationManager.hpp"
#include "ImGuiContext.hpp"

void glfwErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

ApplicationManager::ApplicationManager()
    : windowWidth(1280), windowHeight(1024), windowName("Bezier Curves | Hodograph")
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW.\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(glfwErrorCallback);

    window = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.\n");
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);

    imgui.init(window);
    grid = std::make_unique<Grid>(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

ApplicationManager::~ApplicationManager()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void ApplicationManager::run()
{
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime;
        lastTime = currentTime;

        update(dt);
        render();

        glfwPollEvents();
    }
}

void ApplicationManager::update(double dt)
{
    // grid.update(dt);
}

void ApplicationManager::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    imgui.newFrame();
    grid->render();
    imgui.render();

    glfwSwapBuffers(window);
}
