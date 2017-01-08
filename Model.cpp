//
// Created by Gryffindor on 2017/1/6.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"


Model::Model() {
    mat_ambient = glm::vec3(0.1f,0.1f,0.1f);
    mat_diffuse = glm::vec3(0.8f,0.2f,0.3f);
    mat_specular = glm::vec3(0.0f,0.0f,0.0f);
    mat_shininess = 1.0f;
}

void Model::Load(std::string path) {

    mesh.request_face_normals();
    mesh.request_vertex_normals();

    OpenMesh::IO::Options opt;
    if (!OpenMesh::IO::read_mesh(mesh, path, opt))
        std::cerr << "ERROR LOADING MESH FORM FILE" << std::endl;
    if (!opt.check(OpenMesh::IO::Options::FaceNormal))
        mesh.update_face_normals();
    if (!opt.check(OpenMesh::IO::Options::VertexNormal))
        mesh.update_vertex_normals();

    //Initialize to 0
    vertexNum = faceNum = 0;
    //Vertex
    for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        Vertex _vertex;
        //Get the vertex position
        glm::vec3 position;
        position.x = mesh.point(*v_it)[0];
        position.y = mesh.point(*v_it)[1];
        position.z = mesh.point(*v_it)[2];
        _vertex.position = position;
        //Get the vertex normal
        glm::vec3 normal;
        normal.x = mesh.normal(*v_it)[0];
        normal.y = mesh.normal(*v_it)[1];
        normal.z = mesh.normal(*v_it)[2];
        _vertex.normal = normal;

        vertex.push_back(_vertex);
        ++vertexNum;
    }
    std::cout << "Vertex Number: " << vertexNum << std::endl;
    //Indices
    for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
            indices.push_back(fv_it->idx());
        ++faceNum;
    }
    std::cout << "Face Number: " << faceNum << std::endl;

    model_matrix = glm::mat4(1.0f);
    BindData();
    GetAABB_Box();
    Scale();
    TranslateToCenter();

}

void Model::BindData() {
    //Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    //Set vertex attribute pointers
    //Vertex Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
    //Vertex Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
    //Vertex Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, color));

    glBindVertexArray(0);

}

void Model::Draw(Shader shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int Model::GetVertexNum() {
    return vertexNum;
}

unsigned int Model::GetFaceNum() {
    return faceNum;
}

void Model::GetAABB_Box() {
    float maxX, maxY, maxZ;
    float minX, minY, minZ;
    maxX = maxY = maxZ = 1e-5;
    minX = minX = minZ = 1e5;
    for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        maxX = maxX > mesh.point(*v_it)[0] ? maxX : mesh.point(*v_it)[0];
        minX = minX < mesh.point(*v_it)[0] ? minX : mesh.point(*v_it)[0];

        maxY = maxY > mesh.point(*v_it)[1] ? maxY : mesh.point(*v_it)[1];
        minY = minY < mesh.point(*v_it)[1] ? minY : mesh.point(*v_it)[1];

        maxZ = maxZ > mesh.point(*v_it)[2] ? maxZ : mesh.point(*v_it)[2];
        minZ = minZ < mesh.point(*v_it)[2] ? minZ : mesh.point(*v_it)[2];
    }
    center.x = (maxX + minX) / 2;
    center.y = (maxY + minY) / 2;
    center.z = (maxZ + minZ) / 2;
    radius = std::sqrt((maxX - minX) * (maxX - minX) + (maxY - minY) * (maxY - minY) + (maxZ - minZ) * (maxZ - minZ)) /
             2;
}

void Model::TranslateToCenter() {
    model_matrix = glm::translate(model_matrix,glm::vec3(-center.x,-center.y,-center.z));
}

void Model::Scale() {
    float scale = 1.0/radius;
    model_matrix = glm::scale(model_matrix,glm::vec3(scale,scale,scale));
}

glm::mat4 Model::GetModelMatrix() {
    return model_matrix;
}

glm::vec3 Model::GetMatAmbient() {
    return mat_ambient;
}

glm::vec3 Model::GetMatDiffuse() {
    return mat_diffuse;
}

glm::vec3 Model::GetMatSpecular() {
    return mat_specular;
}

float Model::GetMatShininess() {
    return mat_shininess;
}




