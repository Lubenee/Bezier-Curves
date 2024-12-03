
#include "ApplicationManager.hpp"

// Function to draw grid
void drawGrid(float gridSize, int divisions)
{
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.8f, 0.8f); // Light grey color for grid lines

    // Draw vertical lines
    for (int i = -divisions; i <= divisions; ++i)
    {
        glVertex2f(i * gridSize, -divisions * gridSize);
        glVertex2f(i * gridSize, divisions * gridSize);
    }

    // Draw horizontal lines
    for (int j = -divisions; j <= divisions; ++j)
    {
        glVertex2f(-divisions * gridSize, j * gridSize);
        glVertex2f(divisions * gridSize, j * gridSize);
    }

    glEnd();
}

// Function to draw coordinate axes
void drawAxes()
{
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for axes

    // X-axis
    glVertex2f(-1200, 0);
    glVertex2f(1200, 0);

    // Y-axis
    glVertex2f(0, -1200);
    glVertex2f(0, 1200);

    glEnd();
}

// Callback for mouse scroll to handle zooming
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    if (yOffset > 0)
    {
        // zoomLevel *= 1.1f; // Zoom in
    }
    else if (yOffset < 0)
    {
        // zoomLevel /= 1.1f; // Zoom out
    }

    // Update the projection matrix with new zoom level
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-400 * zoomLevel, 400 * zoomLevel, -400 * zoomLevel, 400 * zoomLevel, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

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
