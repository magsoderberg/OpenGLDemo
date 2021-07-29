#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Transform.h"

class Shader
{
public:
    Shader();
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);

    void ChangeShader(int vertex_selection, int fragment_selection, glm::Transform& transform);
    virtual void use();

    void setBool(const std::string& name, bool value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, const glm::mat4& mat);

    std::string name;
    
private:
    void init(const char* vertexPath, const char* fragmentPath);
    void checkCompileErrors(unsigned int shader, std::string type);
    
};

#endif

