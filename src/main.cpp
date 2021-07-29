/******************************************************
*   OpenGL demo
* 
*   A small demo using OpenGL with the purpose of learning
*   rudimentary graphics programming.
* 
*   Includes the following dependencies:
*   ASSIMP - loading models
*   GLEW - OpenGL functions
*   GLFW - rendering context and user inputs
*   GLM - vector/matrix math
*   IMGUI - GUI
*   NLOHMANN/JSON - Serialize/Deserialize data using JSON
*   stb_image.h - loading textures
*   
*   All other code written by me. All models made by me. 
*   Shader help from learnOpenGL.com.
* 
*   Author: Magnus Söderberg
*   Date: 2021-07-28
* 
********************************************************/


#include "Application.h"

int main(void)
{
    Application app;

    app.InitWindow();
    app.SetupInputs();
    app.InitGUI();
    app.DeSerialize();
    app.RenderLoop();
    
    return 0;

}
