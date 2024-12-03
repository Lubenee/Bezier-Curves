
#include "ApplicationManager.hpp"

// void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
// {
//     if (yOffset > 0)
//     {
//         // zoomLevel *= 1.1f; // Zoom in
//     }
//     else if (yOffset < 0)
//     {
//         // zoomLevel /= 1.1f; // Zoom out
//     }

//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     // glOrtho(-400 * zoomLevel, 400 * zoomLevel, -400 * zoomLevel, 400 * zoomLevel, -1, 1);
//     glMatrixMode(GL_MODELVIEW);
// }

int main()
{
    try
    {
        ApplicationManager app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::flush;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
