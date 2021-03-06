//
// Created by Gryffindor on 2017/1/9.
//

#ifndef RENDERING_INTERACTION_H
#define RENDERING_INTERACTION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "Model.h"
#include "Camera3D.h"
#include "imgui_impl_glfw_gl3.h"
namespace MyLayout{
    bool Init(GLFWwindow* window);
    bool SetLayout(Model &model,Camera3D &camera);
    void Render();
    void ShutDown();
}




#endif //RENDERING_INTERACTION_H
