/******************************************************
*   OpenGL demo
*
*   Mesh loads any external model using ASSIMP
*   
*	Author: Magnus Söderberg
*
********************************************************/

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"
#include "allMeshes.h"


Mesh::Mesh(const char* path)
{
    Init(path);
}

void Mesh::Init(const char* path)
{
    vertices.clear();
    indices.clear();
    texCoords.clear();
       
    this->texturePath = texturePath;
    meshPath = path;

    Assimp::Importer import;
    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {

        mesh = scene->mMeshes[i];
        
        int numFaces = 0;
        int numVerts = 0;
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                indices.push_back(face.mIndices[k]);
            }
            numFaces++;
        }
       

        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {

            vertices.push_back(mesh->mVertices[j].x);
            vertices.push_back(mesh->mVertices[j].y);
            vertices.push_back(mesh->mVertices[j].z);

            vertices.push_back(mesh->mNormals[j].x);
            vertices.push_back(mesh->mNormals[j].y);
            vertices.push_back(mesh->mNormals[j].z);

            vertices.push_back(mesh->mTextureCoords[0][j].x);
            vertices.push_back(mesh->mTextureCoords[0][j].y);


            numVerts++;
        }


        //Assign the loaded vertices as buffers for OpenGL
        numElements = mesh->mNumFaces * 3;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

       
        //Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //Vertex texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
    }
}

json Mesh::Serialize()
{
    json j;
    std::stringstream stream;
    
    j["meshPath"] = meshPath;

    return j;
}


const char* Mesh::GetType()
{
    return "Mesh";
}

