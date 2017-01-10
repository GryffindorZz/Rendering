//
// Created by Gryffindor on 2017/1/7.
//
#include "Camera3D.h"
#include <glm/gtc/matrix_transform.hpp>

Camera3D::Camera3D() {
    theta = 0.0f;
    phi = M_PI/2;
    radius = 2.0f;
    up = 1.0f;
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    ViewMatrix = glm::mat4(1.0f);
    ProjMatrix = glm::mat4(1.0f);
    rotate_speed = 0.005f;
    zoom_speed = 0.1f;
    pan_speed = 0.01f;
    need_update = false;
}

Camera3D::Camera3D(float _theta, float _phi, float _radius) {
    theta = _theta;
    phi = _phi;
    radius = _radius;
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    ViewMatrix = glm::mat4(1.0f);
    ProjMatrix = glm::mat4(1.0f);
}

void Camera3D::Rotate(float d_theta, float d_phi) {
    need_update = true;
    if (up > 1.0f)
        theta += d_theta;
    else
        theta -= d_theta;

    phi += d_phi;
    if (phi > 2 * M_PI)
        phi -= 2 * M_PI;
    else if (phi < -2 * M_PI)
        phi += 2 * M_PI;

    if ((phi > 0 && phi < M_PI) || (phi < -M_PI && phi > -2 * M_PI))
        up = 1.0f;
    else
        up = -1.0f;

}

void Camera3D::Zoom(float distance) {
    need_update = true;
    radius += distance;
    if (radius < 0.5f)
        radius = 0.5f;
    if (radius > 8.0f)
        radius = 8.0f;
}


void Camera3D::Pan(float dx, float dy) {
    need_update = true;
    glm::vec3 look = glm::normalize(target - GetCameraPosition());
    glm::vec3 world_up = glm::vec3(0.0f,up,0.0f);
    glm::vec3 right = glm::cross(look,world_up);
    glm::vec3 w_up = glm::cross(look,right);
    target = target + (right * dx + up * dy);
}

glm::mat4 Camera3D::GetViewMatirx() {
    return ViewMatrix;
}

void Camera3D::UpdateViewMatrix() {
    ViewMatrix = glm::lookAt(GetCameraPosition(), target, glm::vec3(0.0f, up, 0.0f));
}

glm::mat4 Camera3D::GetProjMatrix() {
    return ProjMatrix;
}

void Camera3D::UpdateProjMatrix(const int WIDTH, const int HEIGHT, float near, float far) {
    ProjMatrix = glm::perspective(45.0f, 1.0f * WIDTH / HEIGHT, near, far);
}

void Camera3D::Reset() {
    need_update = true;
    theta = 0.0f;
    phi = M_PI/2;
    radius = 2.0f;
    target = glm::vec3(0.0f,0.0f,0.0f);
}

glm::vec3 Camera3D::GetCameraPosition() {
    return target + ToCartesian();
}

glm::vec3 Camera3D::ToCartesian() {
    float x = radius * sinf(phi) * sinf(theta);
    float y = radius * cosf(phi);
    float z = radius * sinf(phi) * cosf(theta);
    return glm::vec3(x, y, z);
}








