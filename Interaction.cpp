//
// Created by Gryffindor on 2017/1/9.
//


#include "Interaction.h"
#include <iostream>
#include <nfd.h>

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

void ShowMainMenuBar(Model &model);


bool MyLayout::Init(GLFWwindow *window) {
    if (!ImGui_ImplGlfwGL3_Init(window, false))
        return false;
    // Load Fonts
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../extra_fonts/DroidSans.ttf", 16.0f);
    return true;
}

bool MyLayout::SetLayout(Model &model) {
    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetNextWindowSize(ImVec2(100, 300), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_FirstUseEver);

    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    // Demonstrate the various window flags. Typically you would just use the default.
    ImGuiWindowFlags window_flags = 0;
    if (!no_titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border) window_flags |= ImGuiWindowFlags_ShowBorders;
    if (!no_resize) window_flags |= ImGuiWindowFlags_NoResize;
    if (!no_move) window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_collapse) window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu) window_flags |= ImGuiWindowFlags_MenuBar;

    //   ImGui::Begin("", NULL, window_flags);
    ShowMainMenuBar(model);
//    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//    ImGui::End();
    return true;
}

void ::MyLayout::Render() {
    ImGui::Render();
}

void ::MyLayout::ShutDown() {
    ImGui_ImplGlfwGL3_Shutdown();
}

void ShowMainMenuBar(Model &model) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "CTRL+O")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("ply,obj", NULL, &outPath);
                if (result == NFD_OKAY) {
                    model.SetModelPath(outPath);
                    model.need_update = true;
                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    puts("User pressed cancel.");
                } else {
                    printf("Error: %s\n", NFD_GetError());
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

