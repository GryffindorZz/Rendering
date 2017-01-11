//
// Created by Gryffindor on 2017/1/9.
//

#include "Interaction.h"
#include <nfd.h>

void ShowMainMenuBar(Model &model);

void ShowParameterBar(Model &model, Camera3D &camera);

void ShowFPS();


bool MyLayout::Init(GLFWwindow *window) {
    if (!ImGui_ImplGlfwGL3_Init(window, true))
        return false;
    // Load Fonts
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../extra_fonts/DroidSans.ttf", 16.0f);
    return true;
}

bool MyLayout::SetLayout(Model &model, Camera3D &camera) {
    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetNextWindowSize(ImVec2(100, 300), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_FirstUseEver);


    //   ImGui::Begin("", NULL, window_flags);
    ShowMainMenuBar(model);
    ShowParameterBar(model, camera);
    ShowFPS();

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
            //Open file
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
            //
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

void ShowParameterBar(Model &model, Camera3D &camera) {
    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    // Demonstrate the various window flags. Typically you would just use the default.
    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border) window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize) window_flags |= ImGuiWindowFlags_NoResize;
    if (!no_move) window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse) window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_menu) window_flags |= ImGuiWindowFlags_MenuBar;

    ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiSetCond_FirstUseEver);
    ImGui::SetWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Parameter", NULL, window_flags);
    //static ImVec4 ff(0.8f,0.5f,0.2f,1.0f);
    if (ImGui::CollapsingHeader("Material")) {
        static float diff_color[3]{model.GetMatDiffuse().r, model.GetMatDiffuse().g, model.GetMatDiffuse().b};
        ImGui::ColorEdit3("diff_color", diff_color);
        model.SetMatDiffuse(diff_color);

        static float spec_color[3]{model.GetMatSpecular().r, model.GetMatSpecular().g, model.GetMatSpecular().b};
        ImGui::ColorEdit3("spec_color", spec_color);
        model.SetMatSpecular(spec_color);

        static float shininess = model.GetMatShininess();
        ImGui::SliderFloat("shininess", &shininess, 0.0f, 1.0f);
        model.SetMatShininess(shininess);
    }
    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::Text("Cartesian Pos: x:%.2f y:%.2f z:%.2f", camera.GetCartesianPos().x, camera.GetCartesianPos().y,
                    camera.GetCartesianPos().z);
        float *s_pos = camera.GetSpherialPos();
        ImGui::Text("Spherical Pos: t:%.2f p:%.2f r:%.2f", s_pos[0], s_pos[1], s_pos[2]);
    }
    if (ImGui::CollapsingHeader("Advanced")) {
        if (ImGui::TreeNode("Draw Pattern")) {
            static int e = 0;
            ImGui::RadioButton("fill", &e, 0);
            ImGui::SameLine();
            ImGui::RadioButton("point", &e, 1);
            ImGui::SameLine();
            ImGui::RadioButton("line", &e, 2);
            model.draw_pattern = e;
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Cluster")) {
            static int cluster_layer = 1;
            ImGui::SliderInt("layer", &cluster_layer, 0, 3);

            static bool is_cluster = model.need_cluster;
            ImGui::Checkbox("cluster color", &is_cluster);
            if (is_cluster) {
                if (model.total_clus.empty())
                    model.ScaleCluster();
                if (model.cluster_layer != cluster_layer)
                    model.UpdateClusterColor();
                model.render_mode = 1;
            } else
                model.render_mode = 0;
            model.cluster_layer = cluster_layer;
            ImGui::TreePop();
        }
    }


    ImGui::End();

}

void ShowFPS() {
    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    // Demonstrate the various window flags. Typically you would just use the default.
    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border) window_flags |= ImGuiWindowFlags_ShowBorders;
    if (!no_resize) window_flags |= ImGuiWindowFlags_NoResize;
    if (!no_move) window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse) window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_menu) window_flags |= ImGuiWindowFlags_MenuBar;

    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
    ImGui::SetWindowPos(ImVec2(1100, 10), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("FPS", NULL, window_flags);
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

}

