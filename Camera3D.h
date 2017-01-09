//
// Created by Gryffindor on 2017/1/7.
//

#ifndef RENDERING_CAMERA3D_H
#define RENDERING_CAMERA3D_H
#include <glm/glm.hpp>
class Camera3D{
public:
    Camera3D();
    Camera3D(float _theta,float _phi,float _radius);
    //
    void Rotate(float d_theta,float d_phi);
    void Zoom(float distance);
    void Pan(float dx, float dy);
    //
    glm::mat4 GetViewMatirx();
    void UpdateViewMatrix();
    glm::mat4 GetProjMatrix();
    void UpdateProjMatrix(const int WIDTH,const int HEIGHT,float near,float far);
    void Reset();
    //
    glm::vec3 GetCameraPosition();
    inline glm::vec3 ToCartesian();
    float rotate_speed;
    float zoom_speed;
    float pan_speed;
    bool need_update;
private:
    float theta;
    float phi;
    float radius;
    float up;
    glm::vec3 target;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjMatrix;
};
#endif //RENDERING_CAMERA3D_H
