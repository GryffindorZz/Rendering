//
// Created by Gryffindor on 2017/1/6.
//

#ifndef RENDERING_SHADER_H
#define RENDERING_SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader{
public:
    Shader(const GLchar* _vertex_path, const GLchar* _frag_path);
    void Compile();
    void Use();
    GLuint Program;
private:
    const GLchar* vertexPath;
    const GLchar* fragPath;
};
#endif //RENDERING_SHADER_H
