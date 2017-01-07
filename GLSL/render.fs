#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light1;
uniform Material material;
uniform vec3 viewpos;

void main() {
    //ambient
    vec3 ambient = light1.ambient * material.ambient;
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightdir = normalize(light1.position - FragPos);
    float diff = max(dot(norm, lightdir), 0.0f);
    vec3 diffuse = light1.diffuse * (diff * material.diffuse);
    //specular
    vec3 viewdir = normalize(viewpos - FragPos);
    vec3 specular;
    vec3 reflectdir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewdir, reflectdir), 0.0f), material.shininess * 32);
    specular = light1.specular * (spec * material.specular);
    color = vec4(ambient + diffuse + specular,1.0f);
}