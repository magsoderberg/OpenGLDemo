/******************************************************
*   OpenGL demo
*
*   Animation is a class for loading several models
*   as key frames and putting them into a single VAO.
*   This is used to animate the chest between open and closed.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "Animation.h"
#include "Shader.h"
#include "stb_image.h"

#include <iostream>
#include <GL/glew.h>
#include <string>
#include <vector>


Animation::Animation(std::vector<std::string> v) {
    LoadAnimation(v);
    keyFrameMeshesPath.clear();
    for (int i = 0; i < v.size(); i++) {
        keyFrameMeshesPath.push_back(v[i]);
    }
}

//Key frames set two maximum two now but can easily be extended 
void Animation::LoadAnimation(std::vector<std::string> keyFrames)
{
    vertices.clear();
    indices.clear();
    texCoords.clear();

    const aiScene* scene1;
    const aiScene* scene2;

    const aiMesh* mesh1;
    const aiMesh* mesh2;

    std::vector<const aiScene*> scenes;
    std::vector<const aiMesh*> meshes;

    scenes.push_back(scene1);
    scenes.push_back(scene2);

    meshes.push_back(mesh1);
    meshes.push_back(mesh2);


    Assimp::Importer import;
    Assimp::Importer import2;

   
    //Only two meshes loaded here
    //-------------------------------
    scenes[0] = import.ReadFile(keyFrames[0], aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scenes[0] || scenes[0]->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scenes[0]->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

        meshes[0] = scenes[0]->mMeshes[0];
    

    scenes[1] = import2.ReadFile(keyFrames[1], aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scenes[1] || scenes[1]->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scenes[1]->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    meshes[1] = scenes[1]->mMeshes[0];

    //---------------------------------

    int numFaces = 0;
    int numVerts = 0;

    for (unsigned int j = 0; j < meshes[0]->mNumFaces; j++)
    {
        aiFace face = meshes[0]->mFaces[j];
        for (unsigned int k = 0; k < face.mNumIndices; k++)
        {
            indices.push_back(face.mIndices[k]);
        }
        numFaces++;
    }     


    for (unsigned int j = 0; j < meshes[0]->mNumVertices; j++)
    {

        for (int i = 0; i < keyFrames.size(); i++) {
            vertices.push_back(meshes[i]->mVertices[j].x);
            vertices.push_back(meshes[i]->mVertices[j].y);
            vertices.push_back(meshes[i]->mVertices[j].z);
        }

        for (int i = 0; i < keyFrames.size(); i++) {
            vertices.push_back(meshes[i]->mNormals[j].x);
            vertices.push_back(meshes[i]->mNormals[j].y);
            vertices.push_back(meshes[i]->mNormals[j].z);
        }

        for (int i = 0; i < keyFrames.size(); i++) {
            vertices.push_back(meshes[i]->mTextureCoords[0][j].x);
            vertices.push_back(meshes[i]->mTextureCoords[0][j].y);
        }

        numVerts++;
    }
    

    numElements = meshes[0]->mNumFaces * 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Copy the data from cpu to gpu
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

    int offset = 0;
    for (int i = 0; i < keyFrames.size(); i++) {
        //Vertex Positions
        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 8 * keyFrames.size() * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += 3;
    }

    for (int i = keyFrames.size(); i < keyFrames.size() *2; i++) {
        //Vertex normals
        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 8 * keyFrames.size() * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += 3;

    }

    for (int i = keyFrames.size() * 2; i < keyFrames.size() * 3; i++) {
        //Texture coords
        glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 8 * keyFrames.size() * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += 2;

    }

}


