#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Model.h"
#include "Light.h"
#include "Camera3D.h"
#include "Interaction.h"


//Global Variable
const int WIDTH = 1200;
const int HEIGHT = 800;
const GLfloat BACKGROUND_COLOR_R = 1.0f;
const GLfloat BACKGROUND_COLOR_G = 1.0f;
const GLfloat BACKGROUND_COLOR_B = 1.0f;
const GLfloat BACKGROUND_COLOR_A = 1.0f;
const float NEAR_CLIP = 0.01f;
const float FAR_CLIP = 10.0f;
bool ARCACTIVE = false;
bool PANACTIVE = false;
float LAST_X = 0;
float LAST_Y = 0;
Camera3D camera;
Light light;
//Global Variable End

//Interactive Function
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void cursor_callback(GLFWwindow *window, double xpos, double ypos);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
//Interactive Function End

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // OSX should add the statement as flows
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // End

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Rendering", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 150, 60);

    //Get Related Information
    const GLubyte *byteGLVersion = glGetString(GL_VERSION);
    const GLubyte *byteGLVendor = glGetString(GL_VENDOR);
    const GLubyte *byteGLRenderer = glGetString(GL_RENDERER);
    const GLubyte *byteSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "GLVersion: " << byteGLVersion << std::endl;
    std::cout << "GLVerdor: " << byteGLVendor << std::endl;
    std::cout << "GLRenderer: " << byteGLRenderer << std::endl;
    std::cout << "GLSLVersion: " << byteSLVersion << std::endl;
    //End
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    //OSX WIDTH and HEIGHT in glViewport should multiply by 2
    glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
    //Enable GL_DEPTH_TEST
    glEnable(GL_DEPTH_TEST);

    // Setup ImGui binding
    if (!MyLayout::Init(window)) {
        std::cout << "Failed to create IMGUI window" << std::endl;
        return -1;
    }

    //Interactive Function
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //End

    //Shader
    Shader shader("../GLSL/render.vs", "../GLSL/render.fs");
    shader.Compile();
    //Model
    Model model;

    //Camera
    camera.UpdateViewMatrix();
    camera.UpdateProjMatrix(WIDTH, HEIGHT, NEAR_CLIP, FAR_CLIP);
    //Main Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        MyLayout::SetLayout(model, camera);
        if (model.need_update) {
            Model _model;
            _model.SetModelPath(model.GetModelPath());
            _model.Load();
            model = _model;
            model.need_update = false;
            model.load_status = true;
        }
        glClearColor(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, BACKGROUND_COLOR_A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //
        if (model.load_status) {
            shader.Use();
            //
            if (camera.need_update) {
                camera.UpdateViewMatrix();
                camera.need_update = false;
            }
            //
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE,
                               glm::value_ptr(camera.GetProjMatrix()));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE,
                               glm::value_ptr(camera.GetViewMatirx()));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE,
                               glm::value_ptr(model.GetModelMatrix()));
            //For convenience, set the light position equal to camera position
            //light.SetPosition(camera.GetCartesianPos() + glm::vec3(-1, 1, camera.GetCartesianPos().z * 2));
            light.SetPosition(camera.GetCartesianPos());
            glUniform3f(glGetUniformLocation(shader.Program, "light1.position"), light.GetPosition().x,
                        light.GetPosition().y, light.GetPosition().z);
            glUniform3f(glGetUniformLocation(shader.Program, "light1.ambient"), light.GetAmbient().r,
                        light.GetAmbient().g,
                        light.GetAmbient().b);
            glUniform3f(glGetUniformLocation(shader.Program, "light1.diffuse"), light.GetDiffuse().r,
                        light.GetDiffuse().g,
                        light.GetDiffuse().b);
            glUniform3f(glGetUniformLocation(shader.Program, "light1.specular"), light.GetSpecular().r,
                        light.GetSpecular().g, light.GetSpecular().b);
            //
            glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), model.GetMatAmbient().r,
                        model.GetMatAmbient().g, model.GetMatAmbient().b);
            glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), model.GetMatDiffuse().r,
                        model.GetMatDiffuse().g, model.GetMatDiffuse().b);
            glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), model.GetMatSpecular().r,
                        model.GetMatSpecular().g, model.GetMatSpecular().b);
            glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), model.GetMatShininess());
            //render mode
            glUniform1i(glGetUniformLocation(shader.Program,"mode"),model.render_mode);
            //
            model.Draw(shader);

        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        MyLayout::Render();

        switch (model.draw_pattern) {
            case 0:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                //std::cout<<model.draw_pattern<<std::endl;
                break;
            case 1:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                //std::cout<<model.draw_pattern<<std::endl;
                break;
            case 2:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                //std::cout<<model.draw_pattern<<std::endl;
                break;
        }
        //glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(window);
    }
    MyLayout::ShutDown();
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        camera.Reset();
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    if (ARCACTIVE && xpos > 180) {
        float dTheta = ((float) (LAST_X - xpos) * camera.rotate_speed);
        float dPhi = ((float) (LAST_Y - ypos) * camera.rotate_speed);
        camera.Rotate(-dTheta, dPhi);
    }

    if (PANACTIVE) {
        float dx = ((float) (LAST_X - xpos));
        float dy = ((float) (LAST_Y - ypos));
        camera.Pan(dx * camera.pan_speed, -dy * camera.pan_speed);
    }
    LAST_X = xpos;
    LAST_Y = ypos;

}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        ARCACTIVE = true;
    else
        ARCACTIVE = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        PANACTIVE = true;
    else
        PANACTIVE = false;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.Zoom(yoffset * camera.zoom_speed);
}

