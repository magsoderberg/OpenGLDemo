/******************************************************
*   OpenGL demo
*
*   Shader handles both the vertex and fragment shader.
*   From here you can compile shaders and change
*   uniforms.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"
#include "allShaders.h"
#include "Application.h"


void Shader::ChangeShader(int vertex_selection, int fragment_selection, glm::Transform& transform)
{

    init(allShaders::GetVertexShaderPath(vertex_selection), allShaders::GetFragmentShaderPath(fragment_selection));

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    use();
    projection = glm::perspective(glm::radians(45.0f), Application::GetWindowSize()[0] / Application::GetWindowSize()[1], 0.1f, 100.0f);
    model = glm::translate(model, transform.position);
    setMat4("projection", projection);
    setMat4("model", model);
    setMat4("view", view);

}

Shader::Shader()
{
    Shader::Shader("shaders/vertex_simple.shader", "shaders/fragment.shader");
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    init(vertexPath, fragmentPath);
}

void Shader::init(const char* vertexPath, const char* fragmentPath)
{
    
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete when linked
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}


void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) 
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) 
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) 
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setInt(const std::string& name, int value) 
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) 
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) 
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

 void Shader::checkCompileErrors(unsigned int shader, std::string type)
 {
     int success;
     char infoLog[1024];
     if (type != "PROGRAM")
     {
         glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
         if (!success)
         {
             glGetShaderInfoLog(shader, 1024, NULL, infoLog);
             std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
         }
     }
     else
     {
         glGetProgramiv(shader, GL_LINK_STATUS, &success);
         if (!success)
         {
             glGetProgramInfoLog(shader, 1024, NULL, infoLog);
             std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
         }
     }
 }


