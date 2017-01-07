//
// Created by Gryffindor on 2017/1/6.
//
#include "Shader.h"
Shader::Shader(const GLchar* _vertex_path, const GLchar* _frag_path)
{
    vertexPath = _vertex_path;
    fragPath = _frag_path;
}
void Shader::Compile()
{
    //Retrieve ther vertex/fragment source code from file path
    std::string vertexCode;
    std::string fragCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //Ensure ifstream objetcs can throw exceptions
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        //Open Files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragPath);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        //Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //Close file handles
        vShaderFile.close();
        fShaderFile.close();

        //Convert steram into string
        vertexCode = vShaderStream.str();
        fragCode = fShaderStream.str();
    }
    catch (std::ifstream::failure failure)
    {
        std::cout << "ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragCode.c_str();

    //Compile Shader
    GLuint vertex,fragment;
    GLint success;
    GLchar infoLog[512];

    //Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode,NULL);
    glCompileShader(vertex);
    //Print compile error
    glGetShaderiv(vertex, GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertex,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&fShaderCode,NULL);
    glCompileShader(fragment);
    //Print compile error
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragment,512,NULL,infoLog);
        std::cout<<"ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Shader Program
    Program = glCreateProgram();
    glAttachShader(Program,vertex);
    glAttachShader(Program,fragment);
    glLinkProgram(Program);
    //Print linking errors
    glGetProgramiv(Program,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(Program,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog<<std::endl;
    }
    //Delete the shaders as they have been linked program and won't be used
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}
void Shader::Use()
{
    glUseProgram(Program);
}