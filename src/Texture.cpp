/******************************************************
*   OpenGL demo
*
*   Texture loads a image file using stb_image
    and holds the reference to that data.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "Texture.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <sstream>


Texture::Texture(const char* texturePath) {
    textureName = texturePath;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //stbi_set_flip_vertically_on_load(true);
    stbi_image_free(data);
}

json Texture::Serialize()
{
    json j;
    std::stringstream stream;

    j["texturePath"] = textureName.c_str();

    return j;
}
