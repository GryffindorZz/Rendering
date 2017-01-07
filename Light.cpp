//
// Created by Gryffindor on 2017/1/7.
//
#include "Light.h"
Light::Light() {
    position = glm::vec3(2,2,2);
    ambient = glm::vec3(0.1f,0.1f,0.1f);
    diffuse = glm::vec3(0.7f,0.7f,0.7f);
    specular = glm::vec3(1.0f,1.0f,1.0f);
}

void Light::SetPosition(glm::vec3 _position) {
    position = _position;
}

glm::vec3 Light::GetPosition() {
    return position;
}

glm::vec3 Light::GetAmbient() {
    return ambient;
}

glm::vec3 Light::GetDiffuse() {
    return diffuse;
}

glm::vec3 Light::GetSpecular() {
    return specular;
}
