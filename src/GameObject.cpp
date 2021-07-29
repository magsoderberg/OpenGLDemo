/******************************************************
*   OpenGL demo
*
*   GameObject is the abstract parent class of Model and Light.
*
*	Author: Magnus Söderberg
*
********************************************************/


#include "GameObject.h"
#include "Mesh.h"
#include "stb_image.h"
#include "allMeshes.h"
#include "allTextures.h"
#include "allShaders.h"
#include "allLights.h"
#include "Application.h"

unsigned int GameObject::ID_Counter = 0;

//Called when the user clicks on a GameObject
void GameObject::MousePicked() {
	if (playAnimation) {
		playAnimation = false;
		return;
	}
	PlayAnimation();
}

void GameObject::PlayAnimation() {
	playAnimation = true;
}

void GameObject::StopAnimation() {
	VAO = VAO_nonAnim;
	numElements = numElements_nonAnim;
	playAnimation = false;
}

bool * GameObject::GetReceiveLightFrom(int index) {
		if (index > guiReceiveLightFrom.size() - 1 || index < 0)
			throw std::out_of_range("Cannot remove that light. Index out of bounds");

		return &guiReceiveLightFrom[index];
}

void GameObject::ChangeReceivingLightFrom(int index, bool b) {
	if (index > guiReceiveLightFrom.size() - 1 || index < 0)
		throw std::out_of_range("Cannot remove that light. Index out of bounds");

	guiReceiveLightFrom[index] = b;
}

GameObject::GameObject() : transform()
{
	ID_Counter++;
	ID = ID_Counter;

	name = "Empty GameObject";
	transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	projection = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	view = glm::mat4(1.0f);

	shader.use();
	projection = glm::perspective(glm::radians(45.0f), Application::GetWindowSize()[0] / Application::GetWindowSize()[1], 0.1f, 100.0f);
	model = glm::translate(model, transform.position);
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);
	shader.setMat4("view", view);

	if (texturePath != "") {
		UseTexture(texturePath.c_str());
	}

	for (int i = 0; i < allLights::GetSize(); i++) {
		guiReceiveLightFrom.push_back(false);
	}

	UseMesh("assets/models/sphere/cube.obj");
	UseShader("shaders/vertex.shader", "shaders/fragment.shader");

}

void GameObject::UseTexture(const char* texturePath) {

	int t = allTextures::FindTexture(texturePath);
	if (t != -1) {
		texture = t;
		this->texturePath = texturePath;
	}
	else
		std::cerr << "----ERROR: COULD NOT FIND TEXTURE ----" << std::endl;

}

void GameObject::UseShader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	int vertexIndex = allShaders::FindVertexShader(vertexShaderPath);
	if (vertexIndex == -1) {
		std::cerr << "----ERROR: Could not find Vertex Shader----  " << vertexShaderPath << std::endl;
	}
	else
		vshaderPath = std::string(vertexShaderPath);

	int fragmentIndex = allShaders::FindFragmentShader(fragmentShaderPath);
	if (fragmentIndex == -1) {
		std::cerr << "----ERROR: Could not find Fragment Shader----  " << fragmentShaderPath << std::endl;
	}
	else
		fshaderPath = std::string(fragmentShaderPath);
	
	shader.ChangeShader(vertexIndex, fragmentIndex, transform);
	
	guiVertexShader = vertexIndex;
	guiFragmentShader = fragmentIndex;

}

void GameObject::UseMesh(const char* meshPath) {

	std::shared_ptr<Mesh> mesh = allMeshes::FindMesh(meshPath);

	VAO = mesh->GetVAO();
	numElements = mesh->GetNumElements();
	this->meshPath = meshPath;
	
	VAO_nonAnim = VAO;
	numElements_nonAnim = numElements;
}


unsigned int GameObject::GetID()
{
	return ID;
}

void GameObject::ChangeName(const char* newName)
{
	name = newName;
}

const char* GameObject::GetName()
{
	return name.c_str();
}


