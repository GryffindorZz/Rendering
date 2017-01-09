//
// Created by Gryffindor on 2017/1/9.
//


#include "Interaction.h"

bool MyLayout::Init(GLFWwindow *window) {
    if(!ImGui_ImplGlfwGL3_Init(window, false))
        return false;
    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../extra_fonts/DroidSans.ttf", 16.0f);
    return true;
}

bool MyLayout::SetLayout() {

    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetNextWindowSize(ImVec2(100, 300), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Control");
    ImGui::Text("Hello World!");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    return false;
}
void ::MyLayout::Render() {
    ImGui::Render();
}

void ::MyLayout::ShutDown() {
    ImGui_ImplGlfwGL3_Shutdown();
}
