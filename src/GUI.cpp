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
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "GUI.h"
#include "allMeshes.h"
#include "allTextures.h"
#include "allLights.h"
#include "allModels.h"
#include "allShaders.h"
#include "allAnimations.h"
#include "Mesh.h"
#include "OpenFiles.h"
#include "Application.h"


int GUI::selected_model = -1;
int GUI::selected_light = -1;

GUI::GUI(GLFWwindow* window)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.5f;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

}


void GUI::NewFrame(Camera & camera) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void GUI::UpdateObjectList() {
	allObjects.clear();

	for (int i = 0; i < allLights::GetSize(); i++) {
		allObjects.push_back(allLights::GetObject(i));
	}
	for (int i = 0; i < allModels::GetSize(); i++) {
		allObjects.push_back(allModels::GetObject(i));
	}
}

//Shorten name from full path to relative path name
std::string GUI::makePathNameRelative(const char* fullPathName) {
	std::string pathFullPathName = std::string(fullPathName);
	std::string pathRelativeName;
	const char* projectName = "OpenGLDemo";

	std::string word;
	std::vector<std::string> allWords;
	for (std::string::iterator it = pathFullPathName.begin(); it != pathFullPathName.end(); it++)
	{
		if (*it != '\\' && it != pathFullPathName.end() - 1) {
			word += *it;
		}
		else {
			if (it == pathFullPathName.end() - 1)
				word += pathFullPathName.back();

			allWords.push_back(word);
			word.clear();
		}
	}

	std::vector<std::string> relativePathWords;
	for (int i = allWords.size() - 1; i > 1; i--)
	{
		if (allWords[i].compare(projectName) != 0) {
			if (i == allWords.size() - 1)
				relativePathWords.push_back(allWords[i]);
			else
				relativePathWords.push_back(allWords[i] + "/");
		}
		else {
			break;
		}
	}

	std::reverse(relativePathWords.begin(), relativePathWords.end());
	for (int i = 0; i < relativePathWords.size(); i++)
	{
		pathRelativeName += relativePathWords[i];
	}
	return pathRelativeName;

}

void GUI::Menu(Camera& camera) {
	ImGui::Begin("--MENU--");

	if (ImGui::Button("Save")) {
		allModels::Serialize();
		allLights::Serialize();
		allMeshes::Serialize();
		allTextures::Serialize();
		camera.Serialize();
		allShaders::Serialize();

	}
	ImGui::SameLine();
	if (ImGui::Button("Add mesh"))
	{
		std::string newMeshPath = openFiles.OpenDialog();
		if (newMeshPath == "")
		{
			std::cout << "Pressed cancel" << std::endl;
		}
		else
		{
			allMeshes::AddMesh(makePathNameRelative(newMeshPath.c_str()).c_str());
		}

	}
	ImGui::SameLine();
	if (ImGui::Button("Add texture"))
	{
		std::string newTexturePath = openFiles.OpenDialog();
		if (newTexturePath == "")
		{
			std::cout << "Pressed cancel" << std::endl;
		}
		else
		{
			allTextures::AddTexture(makePathNameRelative(newTexturePath.c_str()).c_str());
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Vertex Shader"))
	{
		std::string newShaderPath = openFiles.OpenDialog();
		if (newShaderPath == "")
		{
			std::cout << "Pressed cancel" << std::endl;
		}
		else
		{
			allShaders::AddVertexShader(makePathNameRelative(newShaderPath.c_str()));
		}

	}
	ImGui::SameLine();
	if (ImGui::Button("Add Fragment Shader"))
	{
		std::string newShaderPath = openFiles.OpenDialog();
		if (newShaderPath == "")
		{
			std::cout << "Pressed cancel" << std::endl;
		}
		else
		{
			allShaders::AddFragmentShader(makePathNameRelative(newShaderPath.c_str()));
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Quit")) {
		Application::Quit();
	}

	ImGui::End();
}



void GUI::Meshes() {
	ImGui::Begin("--Meshes--");
	for (int i = 0; i < allMeshes::GetSize(); i++) {
		ImGui::Text(allMeshes::GetMesh(i)->GetName());
	}
	ImGui::End();
}

void GUI::Textures() {
	ImGui::Begin("--Textures--");
	for (int i = 0; i < allTextures::GetSize(); i++) {
		ImGui::Text(allTextures::GetTexture(i)->GetName());
	}
	ImGui::End();
}

void GUI::OutLiner()
{

    ImGui::Begin("--Scene Hierarchy--");

	if (ImGui::Button("Create Model")) {

        allModels::CreateModel();
    }

	if (ImGui::Button("Create Light")) {

		allLights::CreateLight();
	}
    
    ImGui::Separator();

	ImGui::Text("LIGHTS");
	ImGui::Separator();
	for (int i = 0; i < allLights::GetSize(); i++) {
		if (ImGui::Selectable(allLights::GetObject(i)->GetName(), allLights::GetObject(i)->GetID(), selected_light == i))
		{
			selected_light = i;
			selected_model = -1;
			currentObjectSelected = i;
		}

	}

	ImGui::Separator();
	ImGui::Text("MODELS");
	ImGui::Separator();
	for (int i = 0; i < allModels::GetSize(); i++) {
		if (ImGui::Selectable(allModels::GetObject(i)->GetName(), allModels::GetObject(i)->GetID(), selected_model == i))
		{
			selected_light = -1;
			selected_model = i;
			currentObjectSelected = i;
		}
	}

    ImGui::End();
}


void GUI::GUIComponentsForLights(std::shared_ptr<Light> l) {

	ImGui::Text("COLOR");

	ImGui::ColorEdit4("Color of light", (float*)l->GetColor(), ImGuiColorEditFlags_NoInputs);

	ImGui::Text("Direction");

	ImGui::SliderFloat3("direction", &l->direction[0], -1.0f, 1.0f);

	ImGui::Combo("Light type", &l->guiLightType, &l->lightTypes[0], l->lightTypes.size());

	ImGui::Text("All lights");

	ImGui::SliderFloat3("ambient", &l->ambient[0], 0.0, 1.0f);
	ImGui::SliderFloat3("diffuse", &l->diffuse[0], 0.0, 1.0f);
	ImGui::SliderFloat3("specular", &l->specular[0], 0.0, 1.0f);

	ImGui::SliderFloat("linear", &l->linear, 0.0, 1.0f);
	ImGui::SliderFloat("constant", &l->constant, 0.0, 1.0f);
	ImGui::SliderFloat("quadratic", &l->quadratic, 0.0, 1.0f);

	ImGui::Text("Spot light");

	ImGui::SliderFloat("Cut off", &l->cutoff, 0.0, 120.0f);
	ImGui::SliderFloat("Outer cut off", &l->outerCutoff, 0.0, 120.0f);

	if (ImGui::Button("Update light")) {
		for(int i = 0; i < l->models.size(); i++)
		{
			l->models[i]->UpdateShaderUniforms();
			l->shader.use();
			l->shader.setVec3("color", l->color.r, l->color.g, l->color.b);


		}
	}
	ImGui::Separator();

	ImGui::Text("Set mouse click action");
	ImGui::Combo("light animations", &l->lightAnimationChoice, &allAnimations::lightAnimations[0], allAnimations::lightAnimations.size());
	
	if (ImGui::Button("Set Animation")) {
		if (allAnimations::lightAnimations[l->lightAnimationChoice] == "ChangeLightColor") {
			l->SetAnimation(allAnimations::ChangeLightColor, "allAnimations::ChangeLightColor");
			l->lightUniforms.redRadian = l->color.r;
			l->lightUniforms.greenRadian = l->color.g;
			l->lightUniforms.blueRadian = l->color.b;
		}
	}

	ImGui::Separator();

	ImGui::SliderFloat("Red incrementor", &l->lightUniforms.redIncrementor, 0.0f, 0.5f);
	ImGui::SliderFloat("Green incrementor", &l->lightUniforms.greenIncrementor, 0.0f, 0.5f);
	ImGui::SliderFloat("Blue incrementor", &l->lightUniforms.blueIncrementor, 0.0f, 0.5f);

	ImGui::Separator();

	//Change mesh
	ImGui::Text("MESH");

	std::vector<const char*> allMeshNames = allMeshes::GetAllMeshesNames();

	static int mesh_selector_light = 0;
	int currentSelection_mesh_light = mesh_selector_light;
	ImGui::Combo("Mesh", &mesh_selector_light, &allMeshNames[0], allMeshNames.size());
	if (currentSelection_mesh_light != mesh_selector_light)
	{
		l->UseMesh(allMeshNames[mesh_selector_light]);
	}

	ImGui::Text("Render?");
	ImGui::Checkbox("Render light?", &l->render);

}

void GUI::GUIComponentsForModels(std::shared_ptr<Model> g) {

	ImGui::Begin("--ANIMATIONS--");
	ImGui::Text("ANIMATE");
	ImGui::Separator();
	
	//keyFrameMeshesPath
	static std::vector<std::string> keyFrameMeshes{ "empty", "empty" };

	if (ImGui::Button("Add keyframe mesh 1"))
	{
		std::string newMeshPath = openFiles.OpenDialog();
		if (newMeshPath == "")
		{
			std::cout << "Pressed cancel" << std::endl;
		}
		else
		{
			keyFrameMeshes[0] = makePathNameRelative(newMeshPath.c_str());
		}

	}
	ImGui::SameLine();
	ImGui::Text(keyFrameMeshes[0].c_str());

	if (ImGui::Button("Add keyframe mesh 2"))
	{
		std::string newMeshPath = openFiles.OpenDialog();
		if (newMeshPath == "")
		{
			std::cout << "Pressed cancel" << std::endl;
		}
		else
		{
			keyFrameMeshes[1] = makePathNameRelative(newMeshPath.c_str());
		}
	}
	ImGui::SameLine();
	ImGui::Text(keyFrameMeshes[1].c_str());
	ImGui::Text("Animation Name");
	ImGui::SameLine();


	if (ImGui::Button("Add animation")) {
		g->animations.push_back(std::make_shared<Animation>(keyFrameMeshes));
	}

	
	if (ImGui::Button("Clear keyframes")) {
		for (int i = 0; i < keyFrameMeshes.size(); i++) {
			keyFrameMeshes[i] = "empty";
		}
	}
	ImGui::Text("ADDED ANIMATIONS");
	for (int i = 0; i < g->animations.size(); i++) {
		ImGui::Text(std::to_string(i).c_str());
	}

	ImGui::Text("Set mouse click action");
	ImGui::Combo("model animations", &g->modelAnimationChoice, allAnimations::modelAnimations, sizeof(allAnimations::modelAnimations)/sizeof(allAnimations::modelAnimations[0]));

	if (ImGui::Button("Set Animation")) {
		if (allAnimations::modelAnimations[g->modelAnimationChoice] == "ChestAnimation") {
			g->SetAnimation(0, allAnimations::ChestAnimation, "allAnimations::ChestAnimation");
		}
		if (allAnimations::modelAnimations[g->modelAnimationChoice] == "StreamAnimation") {
			g->SetAnimation(-1, allAnimations::StreamAnimation, "allAnimations::StreamAnimation");
		}
		if (allAnimations::modelAnimations[g->modelAnimationChoice] == "StartScreenAnimation") {
			g->SetAnimation(-1, allAnimations::StartScreenAnimation, "allAnimations::StartScreenAnimation");
		}
		
	}

	ImGui::End();

	//TEXTURE
	ImGui::Text("TEXTURE");

	std::vector<const char*> allTextureNames = allTextures::GetAllTextureNames();
	
	int currentSelection_texture = g->guiTexture;
	ImGui::Combo("Texture", &g->guiTexture, &allTextureNames[0], allTextureNames.size());
	if (currentSelection_texture != g->guiTexture)
	{
		g->UseTexture(allTextureNames[g->guiTexture]);
	}

	//Change mesh
	ImGui::Text("MESH");

	std::vector<const char*> allMeshNames = allMeshes::GetAllMeshesNames();

	static int mesh_selector = 0;
	int currentSelection_mesh = mesh_selector;
	ImGui::Combo("Mesh", &mesh_selector, &allMeshNames[0], allMeshNames.size());
	if (currentSelection_mesh != mesh_selector)
	{
		g->UseMesh(allMeshNames[mesh_selector]);
	}

	ImGui::Separator();

	//Change shader

	std::vector<const char*> allVertexShaderNames = allShaders::GetAllVertexShaderNames();
	std::vector<const char*> allFragmentShaderNames = allShaders::GetAllFragmentShaderNames();

	static bool changeShader = false;
	int currentSelection_vertex = g->guiVertexShader;
	int currentSelection_fragment = g->guiFragmentShader;

	ImGui::Text("SHADER");
	ImGui::Combo("Vertex Shader", &g->guiVertexShader, &allVertexShaderNames[0], allVertexShaderNames.size());
	if (currentSelection_vertex != g->guiVertexShader)
	{
		changeShader = true;
	}

	ImGui::Combo("Fragment Shader", &g->guiFragmentShader, &allFragmentShaderNames[0], allFragmentShaderNames.size());
	if (currentSelection_fragment != g->guiFragmentShader)
	{
		changeShader = true;
	}

	if (changeShader) {
		g->UseShader(allVertexShaderNames[g->guiVertexShader], allFragmentShaderNames[g->guiFragmentShader]);
		changeShader = false;
	}


	std::vector<const char*> allLightNames = allLights::GetAllLightNames();
	
	ImGui::Text("LIGHTS");
	ImGui::Separator();

	ImGui::Combo("Light", &g->gui_light_selected, &allLightNames[0], allLights::GetSize());

	if (ImGui::Button("Add Light")) {
		try {
			g->AddLight(g->gui_light_selected);
		}
		catch (std::out_of_range e) {
			std::cout << e.what() << std::endl;
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Remove Light")) {
		try {
			g->RemoveLight(g->gui_light_selected);
		}
		catch (std::out_of_range e) {
			std::cout << e.what() << std::endl;
		}
	}

	std::vector<const char*> addedLights = g->GetAddedLightNames();
	for (int i = 0; i < addedLights.size(); i++) {
		ImGui::Text(addedLights[i]);
	}

	ImGui::Separator();
	ImGui::Text("ROTATE");
	ImGui::Separator();

	ImGui::SliderFloat3("rotate around", &g->rotateAround[0], 0.0f, 1.0f);
	ImGui::SliderFloat("rotation angle", &g->rotationAngle, 0.0f, 360.0f);
	
	ImGui::Separator();

	if (ImGui::Button("Update shader")) {
		g->UpdateShaderUniforms();
	}

}

void GUI::GUIComponentsForAll(std::shared_ptr<GameObject> g) {

	// Name
	ImGui::Text("NAME");
	ImGui::Text(g->GetName());
	
	ImGui::Separator();

	//TRANSFORM

	ImGui::Text("Transform");
	glm::vec3 pos = g->transform.position;
	float v[3]{ pos.x, pos.y, pos.z };
	ImGui::InputFloat3("Position", &v[0]);
	g->transform.position = glm::vec3(v[0], v[1], v[2]);

	if (ImGui::Button("Move mesh")) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, g->transform.position);

		g->shader.use();
		g->model = model;
		g->shader.setMat4("model", model);
	}

	//Change name

	ImGui::Text("CHANGE NAME");
	static char nameBuffer[64]; ImGui::InputText("", nameBuffer, 64);
	if (ImGui::Button("Change name"))
	{
		g->ChangeName(nameBuffer);
		nameBuffer[0] = '\0';
	}

}

bool GUI::GUIDelete() {
	ImGui::Separator();
	ImGui::Separator();
	ImGui::Separator();
	if (ImGui::Button("Delete GameObject")) {
		return true;
	}
	return false;
}

void GUI::Inspector()
{
    ImGui::Begin("--INSPECTOR--");
	
	if (selected_light != -1) {
		GUIComponentsForAll(allLights::GetObject(selected_light));
		GUIComponentsForLights(allLights::GetObject(selected_light));
		if (GUIDelete()) {
			try {
				allLights::DeleteObject(selected_light);
				selected_light = -1;
			}
			catch (...) {

			}
		}
	}
	else if (selected_model != -1) {
		GUIComponentsForAll(allModels::GetObject(selected_model));
		GUIComponentsForModels(allModels::GetObject(selected_model));
		if (GUIDelete()) {
			try {
				allModels::DeleteObject(selected_model);
				selected_model = -1;
			}
			catch (...) {

			}
		}
	}
	

    ImGui::End();
}

void GUI::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
