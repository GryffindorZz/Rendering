//
// Created by Gryffindor on 2017/1/7.
//

#ifndef RENDERING_LIGHT_H
#define RENDERING_LIGHT_H

#include <glm/glm.hpp>

class Light{
public:
    Light();
    void SetPosition(glm::vec3 _position);
    glm::vec3 GetPosition();
    glm::vec3 GetAmbient();
    glm::vec3 GetDiffuse();
    glm::vec3 GetSpecular();
private:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif //RENDERING_LIGHT_H
