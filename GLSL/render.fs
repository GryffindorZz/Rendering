#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

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
uniform int mode;

vec3 Phong()
{
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
    float spec = pow(max(dot(viewdir, reflectdir), 0.0f), material.shininess * 128);
    specular = light1.specular * (spec * material.specular);
    return (ambient+diffuse+specular);
}

vec3 ClusterColor()
{
    //ambient
    vec3 ambient = light1.ambient * material.ambient;
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightdir = normalize(light1.position - FragPos);
    float diff = max(dot(norm, lightdir), 0.0f);
    vec3 diffuse = light1.diffuse * (diff * Color);
    return (ambient+diffuse);

}

void main() {
    vec3 result;
    switch (mode)
    {
        case 0:
            result = Phong();
            break;
        case 1:
            result = ClusterColor();
            break;
    }
    color = vec4(result,1.0f);

}