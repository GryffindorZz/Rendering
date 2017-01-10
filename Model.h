//
// Created by Gryffindor on 2017/1/6.
//

#ifndef RENDERING_MODEL_H
#define RENDERING_MODEL_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "MyMesh.h"
#include "Shader.h"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

class Model{
public:
    Model();
    void SetModelPath(char* _path);
    char* GetModelPath();
    void Load();
    void BindData();
    void Draw(Shader shader);
    unsigned int GetVertexNum();
    unsigned int GetFaceNum();
    glm::mat4 GetModelMatrix();
    void GetAABB_Box();
    void TranslateToCenter();
    void Scale();
    glm::vec3 GetMatAmbient();
    glm::vec3 GetMatDiffuse();
    glm::vec3 GetMatSpecular();
    float GetMatShininess();
    //
    MyMesh mesh;
    std::vector<Vertex> vertex;
    std::vector<GLuint> indices;
    bool need_update;
    bool status;
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    char *model_path;
    glm::vec3 center;
    float radius;
    unsigned int vertexNum;
    unsigned int faceNum;
    glm::mat4 model_matrix;
    glm::vec3 mat_ambient;
    glm::vec3 mat_diffuse;
    glm::vec3 mat_specular;
    float mat_shininess;
};

#endif //RENDERING_MODEL_H
