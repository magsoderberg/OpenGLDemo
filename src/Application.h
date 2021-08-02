#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "GUI.h"
#include "Camera.h"

class Application {
public:

	int InitWindow();
	void InitGUI();
	void SetupInputs();
	void DeSerialize();

	void RenderLoop();

	void Render(std::shared_ptr<GameObject>);
	void CheckCollisions(glm::vec3 v);
	void ViewPortToWorldPos(int mouse_x, int mouse_y);

	static float const * GetWindowSize();

	inline Camera& GetCamera() { return camera; }

	static void Quit();

	float currentFrame;
	float deltaTime;
	float lastFrame;
	bool showGUI = false;
	bool showLights = false;
	bool renderLights = true;

private:
	GLFWwindow* window = nullptr;
	Camera camera;
	GUI gui;

	static float const WINDOW_SIZE_X;
	static float const WINDOW_SIZE_Y;

	static float const * WINDOW_SIZE;

	static int quit;

};

void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);




#endif
