/******************************************************
*   OpenGL demo
*
*   Application is responsible for running the program
*   containing the OpenGL context and render loop etc.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "Application.h"

#include "allMeshes.h"
#include "allTextures.h"
#include "allShaders.h"
#include "allModels.h"
#include "allAnimations.h"

#include <iostream>

float const Application::WINDOW_SIZE_X = 2560.0f;
float const Application::WINDOW_SIZE_Y = 1440.0f;
float const * Application::WINDOW_SIZE = new float[] {WINDOW_SIZE_X, WINDOW_SIZE_Y};


int Application::InitWindow() {

    //Initialize the library
    if (!glfwInit())
        return -1;

    //Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "OpenGL Demo by Magnus Soderberg", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, this);

}

void Application::SetupInputs() {

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewInit();

    glEnable(GL_DEPTH_TEST);
}

void Application::InitGUI() {
    gui = GUI(window);
    gui.UpdateObjectList();
}

void Application::DeSerialize() {

    allMeshes::DeSerialize();
    allTextures::DeSerialize();
    camera.DeSerialize();
    allShaders::DeSerialize();
    allLights::DeSerialize();
    allModels::DeSerialize();
}

void Application::RenderLoop() {

    while (!glfwWindowShouldClose(window))
    {
        gui.NewFrame(camera);

        /* per frame time logic */
        // -------------------------------
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Check user inputs
        processInput(window);

        //Background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*-------------- OBJECTS TO RENDER -------------*/

        for (int i = 0; i < allModels::GetSize(); i++) {
            Render(allModels::GetObject(i));
            allModels::GetObject(i)->UpdateLightColor();
        }

        for (int i = 0; i < allLights::GetSize(); i++) {
            Render(allLights::GetObject(i));
        }

        if (showGUI) {
            gui.Menu(camera);
            gui.Meshes();
            gui.Textures();
            gui.OutLiner();
            gui.Inspector();
        }
            gui.Render();
        


        //Swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

}

void Application::Render(std::shared_ptr<GameObject> g)
{
    if (g->render) {
        if (camera.inTransition)
            camera.SmoothTransition();

        glBindTexture(GL_TEXTURE_2D, g->texture);
        g->shader.use();
        glBindVertexArray(g->GetVAO());

        if (g->playAnimation) {
            if (!g->SetShaderUniforms())
                g->playAnimation = false;
        }

        glm::mat4 view = camera.getViewMatrix();
        g->shader.setMat4("view", view);
        g->shader.setVec3("viewPos", camera.GetCameraPosition());

        glDrawElements(GL_TRIANGLES, g->GetNumOfElements(), GL_UNSIGNED_INT, 0);
    }

}

//Send a vector from origin of screen to world pos provided. 
//Iterates over all Model:s and checks if the vector collides with a circle around Model. 
void Application::CheckCollisions(glm::vec3 v) {
    glm::vec3 origin = camera.GetCameraPosition();

    for (int i = 0; i < allModels::GetSize(); i++) {

        glm::vec3 center = allModels::GetObject(i)->transform.position;
        float b = glm::dot(v, (origin - center));

        if (b < 0) {
            float c = glm::dot((origin - center), (origin - center)) - (1.0f * 1.0f); // Using diameter = 1.0
            if ((b * b - c) > 0) {
                float t1 = -b + glm::sqrt(b * b - c);
                float t2 = -b - glm::sqrt(b * b - c);
                //std::cout << "\nHit " << allModels::GetObject(i)->name << " t1: " << t1 << " t2: " << t2 << std::endl;
                allModels::GetObject(i)->MousePicked();
            }
        }

    }

    for (int i = 0; i < allLights::GetSize(); i++) {

        glm::vec3 center = allLights::GetObject(i)->transform.position;
        float b = glm::dot(v, (origin - center));

        if (b < 0) {
            float c = glm::dot((origin - center), (origin - center)) - (1.0f * 1.0f);
            if ((b * b - c) > 0) {
                float t1 = -b + glm::sqrt(b * b - c);
                float t2 = -b - glm::sqrt(b * b - c);
                std::cout << "\nHit " << allLights::GetObject(i)->name << " t1: " << t1 << " t2: " << t2 << std::endl;
                allLights::GetObject(i)->MousePicked();
            }
        }

    }

}

void Application::ViewPortToWorldPos(int mouse_x, int mouse_y) {

    //ViewPort to Normalized Device Space 0:2560, 1440:0 -> -1:1, -1:1, -1:1
    float x = (2.0f * mouse_x) / Application::GetWindowSize()[0] - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / Application::GetWindowSize()[1];
    float z = 1.0f;
    glm::vec3 ray_nds = glm::vec3(x, y, z);

    //Homogeneous Clip Coordinates
    //Adds the w-component to make it 4D
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

    //Eye (Camera) Coordinates
    glm::mat4 inv_projection = glm::inverse(glm::perspective(glm::radians(45.0f), Application::GetWindowSize()[0] / Application::GetWindowSize()[1], 0.1f, 100.0f));
    glm::vec4 ray_eye = inv_projection * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    
    //World coordinates
    glm::vec3 ray_wor = (glm::inverse(camera.getViewMatrix()) * ray_eye);

    ray_wor = glm::normalize(ray_wor);

    CheckCollisions(ray_wor);

}

//Listens to key inputs
void processInput(GLFWwindow* window)
{
    Application* app = (Application*)glfwGetWindowUserPointer(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !app->GetCamera().inMenu)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        app->GetCamera().inMenu = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && app->GetCamera().inMenu)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        app->GetCamera().inMenu = false;
        app->GetCamera().fixMouseAfterMenu = true;
    }
    //Press H to show GUI
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && app->showGUI) {
        app->showGUI = false;
    }
    //Press H + LEFT CONTROL to hide GUI
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && !app->showGUI) {
        app->showGUI = true;
    }
    
    if (!app->GetCamera().inMenu)
    {
        float cameraSpeed = 20 * app->deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            app->GetCamera().setCameraPosition(app->GetCamera().FORWARD, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            app->GetCamera().setCameraPosition(app->GetCamera().BACKWARD, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            app->GetCamera().setCameraPosition(app->GetCamera().LEFT, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            app->GetCamera().setCameraPosition(app->GetCamera().RIGHT, cameraSpeed);
        
        //Press LEFT ARROW to do a smooth transition of camera
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !app->GetCamera().inTransition) {
            app->GetCamera().SetTransition(app->GetCamera().GetCameraPosition(), app->GetCamera().GetCameraDirection(), 1);
        }
    }

}

//Listens for mouse button inputs
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = (Application*)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        app->ViewPortToWorldPos(xpos, ypos);

    }
}

//Listens for mouse movement 
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = (Application*)glfwGetWindowUserPointer(window);

    if (!app->GetCamera().inTransition) {

        if (app->GetCamera().firstMouse)
        {
            app->GetCamera().setCameraDirection(xpos, ypos, true);
            app->GetCamera().firstMouse = false;
        }
        if (app->GetCamera().fixMouseAfterMenu)
        {
            app->GetCamera().setCameraDirection(xpos, ypos, true);
            app->GetCamera().fixMouseAfterMenu = false;
        }

        if (!app->GetCamera().inMenu)
        {
            app->GetCamera().setCameraDirection(xpos, ypos, false);
        }
    }
   
}

//Listens for window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


float const * Application::GetWindowSize() {
	if (WINDOW_SIZE != 0)
		return WINDOW_SIZE;
	else {
		throw std::runtime_error("----ERROR: Window size is null pointer----");
	}
}