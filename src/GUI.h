#ifndef GUI_H
#define GUI_H
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>

#include "OpenFiles.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"


class GUI
{
public:
	GUI() {};
	GUI(GLFWwindow* window);
	void NewFrame(Camera & camera);
	void Menu(Camera& camera);
	void Meshes();
	void Textures();
	void OutLiner();
	void Render();
	void Inspector();
	void UpdateObjectList();
private:
	bool GUIDelete();
	void GUIComponentsForAll(std::shared_ptr<GameObject> g);
	void GUIComponentsForLights(std::shared_ptr<Light> l);
	void GUIComponentsForModels(std::shared_ptr<Model> m);
	std::vector<std::shared_ptr<GameObject>> allObjects;
	std::string makePathNameRelative(const char* fullPathName);
	OpenFiles openFiles;
	int currentObjectSelected = -1;
	const char* meshName;
	static int selected_model;
	static int selected_light;
};

#endif