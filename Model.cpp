//
// Created by Gryffindor on 2017/1/6.
//

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "Model.h"

////////////Cluster Property
OpenMesh::FPropHandleT<bool> visit;
OpenMesh::FPropHandleT<int> face_id;
OpenMesh::FPropHandleT<float> face_area;
OpenMesh::FPropHandleT<float> edge_length;

////////////Cluster Function
//In first cluster, setting every face visit to false
void SetFaceVisitFalse(MyMesh &mesh);

//In first cluster, initializing every face cluster id to -1
void SetFaceId(MyMesh &mesh);

//For other clusters, setting the current cluster id to -1
void SetClusterId(std::vector<Cluster> &clus);

//For other clusters, setting the current cluster visit to false
void SetClusterVisitFalse(std::vector<Cluster> &clus);
////////////

Model::Model() {
    mat_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    mat_diffuse = glm::vec3(233.0f / 255, 67.0f / 255, 45.0f / 255);
    mat_specular = glm::vec3(0.0f, 0.0f, 0.0f);
    mat_shininess = 1.0f;
    need_update = false;
    load_status = false;
    need_cluster = false;
    render_mode = 0;
    cluster_layer = 0;
}

void Model::Load() {

    mesh.request_face_normals();
    mesh.request_vertex_normals();
    std::string _path = model_path;
    OpenMesh::IO::Options opt;
    if (!OpenMesh::IO::read_mesh(mesh, _path, opt))
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
    model_matrix = glm::translate(model_matrix, glm::vec3(-center.x, -center.y, -center.z));
}

void Model::Scale() {
    float scale = 1.0 / radius;
    model_matrix = glm::scale(model_matrix, glm::vec3(scale, scale, scale));
}

glm::mat4 Model::GetModelMatrix() {
    return model_matrix;
}

glm::vec3 Model::GetMatAmbient() {
    return mat_ambient;
}

void Model::SetMatAmbient(float color[3]) {
    mat_ambient.r = color[0];
    mat_ambient.g = color[1];
    mat_ambient.b = color[2];
}

glm::vec3 Model::GetMatDiffuse() {
    return mat_diffuse;
}

void Model::SetMatDiffuse(float *color) {
    mat_diffuse.r = color[0];
    mat_diffuse.g = color[1];
    mat_diffuse.b = color[2];
}

glm::vec3 Model::GetMatSpecular() {
    return mat_specular;
}

void Model::SetMatSpecular(float *color) {
    mat_specular.r = color[0];
    mat_specular.g = color[1];
    mat_specular.b = color[2];
}

float Model::GetMatShininess() {
    return mat_shininess;
}

void Model::SetMatShininess(float s) {
    mat_shininess = s;
}

void Model::SetModelPath(char *_path) {
    if (!_path)
        std::cout << "path error!" << std::endl;
    model_path = _path;
}

char *Model::GetModelPath() {
    return model_path;
}

void Model::ScaleCluster() {
    //begin cluster
    mesh.add_property(visit);
    mesh.add_property(face_id);
    mesh.add_property(face_area);
    mesh.add_property(edge_length);
    SetFaceVisitFalse(mesh);
    SetFaceId(mesh);
    //first cluster
    std::vector<Cluster> first_clus;
    //for every cluster, set the id to cluster_id, then ++id
    int id = 0;
    std::cout << "first clustering..." << std::endl;
    for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        //if the face hasn't been visited, do something
        if (!mesh.property(visit, *f_it)) {
            Cluster each_clus;
            each_clus.area = 0;
            each_clus.perimeter = 0;
            each_clus.face_number = 0;
            each_clus.cluster_id = id;
            //initialize the represent normal to 0,0,0
            each_clus.repre_normal = OpenMesh::Vec3f(0, 0, 0);
            //BFS Queue
            std::queue<MyMesh::FaceHandle> Q;
            //Set the face visit to true
            mesh.property(visit, *f_it) = true;
            //push
            Q.push(*f_it);
            while (!Q.empty()) {
                MyMesh::FaceHandle f_handle;
                f_handle = Q.front();
                //set the face id
                mesh.property(face_id, f_handle) = id;
                //update cluster info
                each_clus.repre_normal += mesh.normal(f_handle);
                each_clus.area += mesh.property(face_area, f_handle);
                each_clus.face.push_back(f_handle);
                each_clus.face_number++;
                for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(f_handle); fv_it.is_valid(); ++fv_it) {
                    MyMesh::VertexHandle v_handle = *fv_it;
                    each_clus.vertex.insert(v_handle);
                }
                for (MyMesh::FaceHalfedgeIter fh_it = mesh.fh_iter(f_handle); fh_it.is_valid(); ++fh_it) {
                    MyMesh::HalfedgeHandle opposite = mesh.opposite_halfedge_handle(*fh_it);
                    MyMesh::FaceHandle face_handle = mesh.face_handle(opposite);
                    if (mesh.property(face_id, f_handle) != mesh.property(face_id, face_handle)) {
                        MyMesh::HalfedgeHandle tmp_fh_it = *fh_it;
                        each_clus.boundary.insert(tmp_fh_it);
                    } else {
                        each_clus.boundary.erase(opposite);
                    }
                }
                Q.pop();
                for (MyMesh::FaceFaceIter ff_it = mesh.ff_iter(f_handle); ff_it.is_valid(); ++ff_it) {
                    if (!mesh.property(visit, *ff_it)) {
                        float cos_diff = OpenMesh::dot(mesh.normal(*f_it), mesh.normal(*ff_it));
                        if (cos_diff > 0.85) {
                            mesh.property(visit, *ff_it) = true;
                            MyMesh::FaceHandle tmp_face = *ff_it;
                            Q.push(tmp_face);
                        }
                    }
                }

            }
            each_clus.repre_normal.normalize();
//            for(auto k = each_clus.boundary.begin();k!=each_clus.boundary.end();++k) {
//                MyMesh::HalfedgeHandle tmp_handle = *k;
//                mesh.property(edge_length,tmp_handle);
//                //each_clus.perimeter += mesh.property(edge_length, tmp_handle);
//            }
//            each_clus.scale = 4.0f*each_clus.area/each_clus.perimeter;
            ++id;
            first_clus.push_back(each_clus);
        }
    }
    total_clus.push_back(first_clus);
    // rest clusters
    int count = 0;
    while (count < 3) {
        id = 0;
        SetClusterId(total_clus[count]);
        SetClusterVisitFalse(total_clus[count]);
        std::vector<Cluster> each_scale_clus;
        for (int i = 0; i < total_clus[count].size(); ++i) {
            if (!total_clus[count][i].visit) {
                Cluster tmp_clus;
                tmp_clus.area = 0;
                tmp_clus.perimeter = 0;
                tmp_clus.face_number = 0;
                tmp_clus.cluster_id = id;
                total_clus[count][i].visit = true;
                total_clus[count][i].cluster_id = id;
                std::queue<Cluster> Q;
                Q.push(total_clus[count][i]);
                tmp_clus.repre_normal = OpenMesh::Vec3f(0, 0, 0);
                while (!Q.empty()) {
                    Cluster each_clus = Q.front();
                    tmp_clus.repre_normal += each_clus.repre_normal;
                    tmp_clus.area += each_clus.area;
                    for (int j = 0; j < each_clus.face.size(); ++j) {
                        tmp_clus.face.push_back(each_clus.face[j]);
                        mesh.property(face_id, each_clus.face[j]) = id;
                    }
                    for (auto j = each_clus.vertex.begin(); j != each_clus.vertex.end(); ++j) {
                        tmp_clus.vertex.insert(*j);
                    }
                    tmp_clus.face_number += each_clus.face_number;
                    for (auto j = each_clus.boundary.begin(); j != each_clus.boundary.end(); ++j) {
                        MyMesh::HalfedgeHandle hf_it = *j;
                        MyMesh::HalfedgeHandle opposite = mesh.opposite_halfedge_handle(hf_it);
                        MyMesh::FaceHandle face_handle = mesh.face_handle(opposite);
                        if (each_clus.cluster_id != mesh.property(face_id, face_handle))
                            tmp_clus.boundary.insert(hf_it);
                        else
                            tmp_clus.boundary.erase(opposite);
                    }
                    Q.pop();
                    for (auto j = each_clus.boundary.begin(); j != each_clus.boundary.end(); ++j) {
                        MyMesh::HalfedgeHandle he_handle = *j;
                        MyMesh::HalfedgeHandle opposite = mesh.opposite_halfedge_handle(he_handle);
                        MyMesh::FaceHandle face_handle = mesh.face_handle(opposite);
                        if (face_handle.is_valid()) {
                            Cluster &adjacency = total_clus[count][mesh.property(face_id, face_handle)];
                            if (!adjacency.visit) {
                                if (OpenMesh::dot(total_clus[count][i].repre_normal, adjacency.repre_normal) > 0.75) {
                                    adjacency.visit = true;
                                    Q.push(adjacency);
                                }
                            }
                        }
                    }
                }
                tmp_clus.repre_normal.normalized();
                each_scale_clus.push_back(tmp_clus);
                ++id;
            }

        }
        total_clus.push_back(each_scale_clus);
        ++count;
    }
}


void SetFaceVisitFalse(MyMesh &mesh) {
    for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
        mesh.property(visit, *f_it) = false;
}

void SetFaceId(MyMesh &mesh) {
    for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
        mesh.property(face_id, *f_it) = -1;
}

void SetClusterId(std::vector<Cluster> &clus) {
    for (int i = 0; i < clus.size(); ++i)
        clus[i].cluster_id = -1;
}

void SetClusterVisitFalse(std::vector<Cluster> &clus) {
    for (int i = 0; i < clus.size(); ++i)
        clus[i].visit = false;
}

void Model::UpdateClusterColor() {
    glm::vec3 DarkRED(1.0 * 230 / 255, 1.0 * 47 / 255, 1.0 * 47 / 255);
    glm::vec3 LightRED(1.0 * 234 / 255, 1.0 * 153 / 255, 1.0 * 153 / 255);
    glm::vec3 DarkGREEN(1.0 * 46 / 255, 1.0 * 99 / 255, 1.0 * 19 / 255);
    glm::vec3 LightGREEN(1.0 * 124 / 255, 1.0 * 182 / 255, 1.0 * 98 / 255);
    glm::vec3 DarkBLUE(1.0 * 28 / 255, 1.0 * 69 / 255, 1.0 * 135 / 255);
    glm::vec3 LightBLUE(1.0 * 109 / 255, 1.0 * 157 / 255, 1.0 * 235 / 255);
    glm::vec3 PURPLE(1.0 * 103 / 255, 1.0 * 78 / 255, 1.0 * 167 / 255);
    glm::vec3 YELLOW(1.0 * 241 / 255, 1.0 * 193 / 255, 1.0 * 50 / 255);
//     enum COLOR {RED,GREEN,BLUE,PUPLE,YELLOW};
    int k = 0;
    std::cout<<total_clus.size()<<std::endl;
    for (int i = 0; i < total_clus[cluster_layer].size(); ++i, ++k) {
        glm::vec3 col;
        if (k == 8)
            k = 0;
        switch (k) {
            case 0:
                col = DarkRED;
                break;
            case 1:
                col = LightRED;
                break;
            case 2:
                col = DarkGREEN;
                break;
            case 3:
                col = LightGREEN;
                break;
            case 4:
                col = DarkBLUE;
                break;
            case 5:
                col = LightBLUE;
                break;
            case 6:
                col = PURPLE;
                break;
            case 7:
                col = YELLOW;
                break;
            default:
                break;
        }
        //col.r = 1.0*rand() / RAND_MAX;
        //col.g = 1.0*rand() / RAND_MAX;
        //col.b = 1.0*rand() / RAND_MAX;
        for (auto j = total_clus[0][i].vertex.begin(); j != total_clus[0][i].vertex.end(); ++j) {
/*                   for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(totalCluster[scale][i].face[j]); fv_it.is_valid(); ++fv_it)
                           vertex[fv_it->idx()].color = col;        */
            vertex[j->idx()].color = col;
        }
    }
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




