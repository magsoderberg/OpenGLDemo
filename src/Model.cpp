/******************************************************
*   OpenGL demo
*
*   Model is any object in the scene that is not a light.
*   Subclass of GameObject
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "Model.h"
#include "allModels.h"
#include "allAnimations.h"

void Model::AddLight(int index) {
	if (index < allLights::GetSize() && index >= 0) {
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->GetID() == allLights::GetObject(index)->GetID()) {
				std::cout << "---ERROR: light already added----" << std::endl;
				return;
			}
		}
		lights.push_back(allLights::GetObject(index));
		allLights::GetObject(index)->models.push_back(this);
	}
	else
		throw std::out_of_range("---ERROR: Model::AddLight() out of range----");
}

void Model::RemoveLight(int index) {
	if (index < allLights::GetSize() && index >= 0) {
		for (int j = 0; j < lights.size(); j++) {
			if(allLights::GetObject(index)->GetName() == lights[j]->GetName()) {
				lights.erase(lights.begin() + j);
				return;
			}
		}
	}
	else
		throw std::out_of_range("---ERROR: Model::RemoveLight() out of range----");
}

void Model::SetAnimation(int index, bool(*funcPtr)(Model&), const char* name) {
	
	if (index != -1) {
		VAO_nonAnim = VAO;
		numElements_nonAnim = numElements;
		VAO = animations[index]->VAO;
		numElements = animations[index]->numElements;
	}


	setShaderUniforms = funcPtr;
	setShaderUniformsName = name;
}

bool Model::SetShaderUniforms() {
	if (setShaderUniforms == nullptr)
		return false;

	if (setShaderUniforms(*this)) {
		return true;
	}
	else
		return false;
	
}

void Model::UpdateLightColor() {
	for (int i = 0; i < lights.size(); i++) {
		shader.setVec3("lights[" + std::to_string(i) + "].color", lights[i]->color.r, lights[i]->color.g, lights[i]->color.b);
		shader.setVec3("lights[" + std::to_string(i) + "].position", lights[i]->transform.position);
		shader.setVec3("lights[" + std::to_string(i) + "].spotDirection", lights[i]->spotDirection);
	}
}

void Model::UpdateShaderUniforms() {
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(rotationAngle), rotateAround);
	model = glm::translate(model, transform.position);
	shader.setMat4("model", model);

	shader.setInt("nr_lights", lights.size());

	shader.setInt("material.diffuse", 0);
	shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.setFloat("material.shininess", 32.0f);



	for (int i = 0; i < lights.size(); i++) {

		if (lights[i]->lightTypes[lights[i]->guiLightType] == "Direction") {
			shader.setVec3("lights[" + std::to_string(i) + "].direction", lights[i]->direction);
			shader.setInt("lights[" + std::to_string(i) + "].directional", 1);
			shader.setInt("lights[" + std::to_string(i) + "].spot", 0);
			shader.setInt("lights[" + std::to_string(i) + "].point", 0);
			shader.setInt("lights[" + std::to_string(i) + "].spotColored", 0);

		}
		else if (lights[i]->lightTypes[lights[i]->guiLightType] == "Spot") {
			
			shader.setVec3("lights[" + std::to_string(i) + "].position", lights[i]->transform.position);
			shader.setFloat("lights[" + std::to_string(i) + "].cutoff", glm::cos(glm::radians(lights[i]->cutoff)));
			shader.setVec3("lights[" + std::to_string(i) + "].direction", lights[i]->direction);

			shader.setInt("lights[" + std::to_string(i) + "].directional", 0);
			shader.setInt("lights[" + std::to_string(i) + "].spot", 1);
			shader.setInt("lights[" + std::to_string(i) + "].point", 0);
			shader.setInt("lights[" + std::to_string(i) + "].spotColored", 0);
			shader.setFloat("lights[" + std::to_string(i) + "].outerCutoff", glm::cos(glm::radians(lights[i]->outerCutoff)));

		}
		else if (lights[i]->lightTypes[lights[i]->guiLightType] == "SpotColored") {

			shader.setVec3("lights[" + std::to_string(i) + "].position", lights[i]->transform.position);
			shader.setFloat("lights[" + std::to_string(i) + "].cutoff", glm::cos(glm::radians(lights[i]->cutoff)));
			shader.setFloat("lights[" + std::to_string(i) + "].outerCutoff", glm::cos(glm::radians(lights[i]->outerCutoff)));
			shader.setVec3("lights[" + std::to_string(i) + "].direction", lights[i]->direction);

			shader.setInt("lights[" + std::to_string(i) + "].directional", 0);
			shader.setInt("lights[" + std::to_string(i) + "].spot", 0);
			shader.setInt("lights[" + std::to_string(i) + "].point", 0);
			shader.setInt("lights[" + std::to_string(i) + "].spotColored", 1);

		}
		else if (lights[i]->lightTypes[lights[i]->guiLightType] == "Point") {
			shader.setInt("lights[" + std::to_string(i) + "].directional", 0);
			shader.setInt("lights[" + std::to_string(i) + "].spot", 0);
			shader.setInt("lights[" + std::to_string(i) + "].point", 1);
			shader.setInt("lights[" + std::to_string(i) + "].spotColored", 0);

		}
		

		shader.setVec3("lights[" + std::to_string(i) + "].ambient", lights[i]->ambient);
		shader.setVec3("lights[" + std::to_string(i) + "].diffuse", lights[i]->diffuse);
		shader.setVec3("lights[" + std::to_string(i) + "].specular", lights[i]->specular);

		shader.setFloat("lights[" + std::to_string(i) + "].linear", lights[i]->linear);
		shader.setFloat("lights[" + std::to_string(i) + "].constant", lights[i]->constant);
		shader.setFloat("lights[" + std::to_string(i) + "].quadratic", lights[i]->quadratic);

	}
}


std::vector<const char*> Model::GetAddedLightNames() {
	std::vector<const char*> list;
	for (int i = 0; i < lights.size(); i++) {
		list.push_back(lights[i]->GetName());
	}
	return list;
}

json Model::Serialize()
{
	json j;
	j["Model"]["name"] = name;
	j["Model"]["setShaderUniformsName"] = setShaderUniformsName;
	j["Model"]["position"] = transform.position;
	j["Model"]["rotation"] = rotateAround;
	j["Model"]["rotationAngle"] = rotationAngle;
	j["Model"]["texturePath"] = texturePath;
	j["Model"]["meshPath"] = meshPath;
	j["Model"]["vshader"] = vshaderPath;
	j["Model"]["fshader"] = fshaderPath;
	for (int i = 0; i < animations.size(); i++) {
		for (int k = 0; k < animations[i]->keyFrameMeshesPath.size(); k++) {
			j["Model"]["animation"][std::to_string(k)] = animations[i]->keyFrameMeshesPath[k];
		}
	}
	for (int i = 0; i < lights.size(); i++) {
		j["Model"]["lights"][std::to_string(i)] = lights[i]->GetName();
	}

	return j;
}

void Model::DeSerialize(json& j) {

	for (auto& el : j.items())
	{
		if (el.key() == "Model")
		{

			for (auto& el2 : j["Model"].items())
			{

				if (el2.key() == "name")
				{

					name = el2.value();
				}

				if (el2.key() == "position")
				{
					for (auto& el3 : j["Model"]["position"].items())
					{
						if (el3.key() == "x")
						{
							transform.position.x = el3.value();
						}
						if (el3.key() == "y")
						{
							transform.position.y = el3.value();
						}
						if (el3.key() == "z")
						{
							transform.position.z = el3.value();
						}

					}
					shader.use();
					model = glm::mat4(1.0f);
					model = glm::translate(model, transform.position);
					shader.setMat4("model", model);
				}
				if (el2.key() == "rotation")
				{
					for (auto& el3 : j["Model"]["rotation"].items())
					{
						if (el3.key() == "x")
						{
							rotateAround.x = el3.value();
						}
						if (el3.key() == "y")
						{
							rotateAround.y = el3.value();
						}
						if (el3.key() == "z")
						{
							rotateAround.z = el3.value();
						}

					}
				}
				if (el2.key() == "rotationAngle") {
					rotationAngle = el2.value();
				}
				if (el2.key() == "texturePath") {
					texturePath = el2.value();
					UseTexture(texturePath.c_str());
				}
				if (el2.key() == "meshPath") {
					meshPath = el2.value();
					UseMesh(meshPath.c_str());
				}
				if (el2.key() == "vshader") {
					vshaderPath = el2.value();
				}
				if (el2.key() == "fshader") {
					fshaderPath = el2.value();

				}
				if (el2.key() == "lights") {
					for (auto& el3 : j["Model"]["lights"].items()) {

						if (allLights::GetSize() < 1) {
							std::cout << "----ERROR: MUST DESERIALIZE LIGHTS BEFORE MODELS----" << std::endl;
						}

						for (int i = 0; i < allLights::GetSize(); i++) {
							if (allLights::GetObject(i)->GetName() == el3.value()) {
								AddLight(i);
							}
						}
					}
				}

				if (el2.key() == "animation") {
					std::vector<std::string> v;
					for (auto& el3 : j["Model"]["animation"].items()) {
						v.push_back(el3.value());
					}
					animations.push_back(std::make_shared<Animation>(v));
				}

				if (el2.key() == "setShaderUniformsName")
				{
					std::string a = std::string(el2.value());
					if (a.compare("allAnimations::ChestAnimation") == 0) {
						SetAnimation(0, allAnimations::ChestAnimation, "allAnimations::ChestAnimation");
					}

					if (a.compare("allAnimations::StreamAnimation") == 0) {
						SetAnimation(-1, allAnimations::StreamAnimation, "allAnimations::StreamAnimation");
						playAnimation = true;
					}

					if (a.compare("allAnimations::StartScreenAnimation") == 0) {
						SetAnimation(-1, allAnimations::StartScreenAnimation, "allAnimations::StartScreenAnimation");
						playAnimation = false;
					}
				}

			}

		}

	}
	UseShader(vshaderPath.c_str(), fshaderPath.c_str());
	UpdateShaderUniforms();

}
