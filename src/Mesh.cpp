/******************************************************
*   OpenGL demo
*
*   GUI instantiates and handles all GUI.
*	All GUI use ImGui.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include <GL/glew.h>
#include "Mesh.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "allMeshes.h"
#include <iostream>

Mesh::Mesh()
{

}


Mesh::Mesh(const char* path)
{

    Init(path);

}

void Mesh::Init(const char* path)
{
    std::cout << "MESH INIT: " << path << std::endl;
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
        //std::cout << "Num faces: " << numFaces << std::endl;

        /*for (unsigned int j = 0; j < indices.size(); j++)
        {
            std::cout << indices[j] << std::endl;
        }*/

        //arr[24 * 3];
       /* float max_x = -2;
        float max_y = -2;
        float max_z = -2;
        float min_x = 1000;
        float min_y = 1000;
        float min_z = 1000;*/

        //std::cout << "------" << path << "-----" << std::endl;

        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            //_vertPos.push_back(glm::vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1));
            
            //Vertex.Position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
            vertices.push_back(mesh->mVertices[j].x);
            vertices.push_back(mesh->mVertices[j].y);
            vertices.push_back(mesh->mVertices[j].z);
           // std::cout << "x: " << mesh->mVertices[j].x << " y: " << mesh->mVertices[j].y << " z: " << mesh->mVertices[j].z << std::endl;

            //std::cout << mesh->mVertices[j].x << " " << mesh->mVertices[j].y << " " << mesh->mVertices[j].z << std::endl;
           /* if (mesh->mVertices[j].x > max_x) {
                max_x = mesh->mVertices[j].x;
            }
            if (mesh->mVertices[j].y > max_y) {
                max_y = mesh->mVertices[j].y;
            }
            if (mesh->mVertices[j].z > max_z) {
                max_z = mesh->mVertices[j].z;
            }
            if (mesh->mVertices[j].x < min_x) {
                min_x = mesh->mVertices[j].x;
            }
            if (mesh->mVertices[j].y < min_y) {
                min_y = mesh->mVertices[j].y;
            }
            if (mesh->mVertices[j].z < min_z) {
                min_z = mesh->mVertices[j].z;
            }*/
            

            vertices.push_back(mesh->mNormals[j].x);
            vertices.push_back(mesh->mNormals[j].y);
            vertices.push_back(mesh->mNormals[j].z);

            vertices.push_back(mesh->mTextureCoords[0][j].x);
            vertices.push_back(mesh->mTextureCoords[0][j].y);



            //std::cout << mesh->mTextureCoords[0][j].x << " " << mesh->mTextureCoords[0][j].y << std::endl;
            /*arr[arrindex] = mesh->mVertices[j].x;
            arrindex++;
            arr[arrindex] = mesh->mVertices[j].y;
            arrindex++;
            arr[arrindex] = mesh->mVertices[j].z;
            arrindex++;*/

            numVerts++;
        }
        /*boundingBox.push_back(max_x);
        boundingBox.push_back(max_y);
        boundingBox.push_back(max_z);
        boundingBox.push_back(min_x);
        boundingBox.push_back(min_y);
        boundingBox.push_back(min_z);*/

       // std::cout << "max " << max_x << " " << max_y << " " << max_z << std::endl;
       // std::cout << "min " << min_x << " " << min_y << " " << min_z << std::endl;
        //std::cout << "Num verts: " << numVerts << std::endl;

        //int counter = 0;
        //for (unsigned int j = 0; j < vertices.size(); j++)
        //{

        //    std::cout << " " << vertices[j];
        //    if (counter == 2)
        //    {
        //        std::cout << "" << std::endl;
        //        counter = -1;
        //    }
        //    counter++;
        //}
        //for (unsigned int i = 0; i < 24 * 3; i++)
        //{
        //    //std::cout << arr[i] << std::endl;
        //}

        numElements = mesh->mNumFaces * 3;
        //std::cout << "NUMBER: " << numElements << std::endl;


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        //std::cout << "VAO: " << VAO << " AT: " << &VAO << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //Copy the data from cpu to gpu
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        //std::cout << "SIZEOF " << vertices.size() << std::endl;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

        //Texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        //Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
    }

}


json Mesh::Serialize()
{
    json j;
    std::stringstream stream;
    
    j["meshPath"] = meshPath;
    //j["texturePath"] = texturePath;


    return j;
}

//void Mesh::DeSerialize(json& j)
//{
//
//    for (auto& el : j.items())
//    {
//        if (el.key() == "meshPath")
//        {
//            meshPath = el.value();
//        }
//        if (el.key() == "texturePath")
//        {
//            texturePath = el.value();
//        }
//    }
//
//    Init(meshPath.c_str());
//}


const char* Mesh::GetType()
{
    return "Mesh";
}

