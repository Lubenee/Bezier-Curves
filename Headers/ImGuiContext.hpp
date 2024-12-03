#if !defined(IMGUI_CONTEXT_H)
#define IMGUI_CONTEXT_H

#include <imgui.h>

namespace imguiContext
{
    void createImguiContext()
    {
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("Frame Time: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }
}

#endif // IMGUI_CONTEXT_H
