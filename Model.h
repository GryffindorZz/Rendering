//
// Created by Gryffindor on 2017/1/6.
//

#ifndef RENDERING_MODEL_H
#define RENDERING_MODEL_H

#include <GL/glew.h>
#include <vector>
#include <queue>
#include <unordered_set>
#include <glm/glm.hpp>
#include "MyMesh.h"
#include "Shader.h"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

struct Cluster{
    std::unordered_set<MyMesh::HalfedgeHandle> boundary;
    std::vector<MyMesh::FaceHandle> face;
    std::unordered_set<MyMesh::VertexHandle> vertex;
    OpenMesh::Vec3f repre_normal;
    int cluster_id;
    int face_number;
    bool visit;
    float area;
    float perimeter;
    float scale;
    int small_flag;
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
    void SetMatAmbient(float* color);
    glm::vec3 GetMatDiffuse();
    void SetMatDiffuse(float* color);
    glm::vec3 GetMatSpecular();
    void SetMatSpecular(float* color);
    float GetMatShininess();
    void SetMatShininess(float s);
    //
    void ScaleCluster();
    void UpdateClusterColor();
    //
    MyMesh mesh;
    std::vector<Vertex> vertex;
    std::vector<GLuint> indices;
    bool need_update;
    bool load_status;
    int draw_pattern;
    bool need_cluster;
    std::vector<std::vector<Cluster>> total_clus;
    int render_mode;
    int cluster_layer;
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
