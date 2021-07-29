/******************************************************
*   OpenGL demo
*
*   Light class is for all lights in demo.
*	Subclass of GameObject. 
*	
*
*	Author: Magnus Söderberg
*
********************************************************/

#include <iostream>

#include "Light.h"
#include "allAnimations.h"


Light::Light() {
	UseShader("shaders/vertex.shader", "shaders/fs_lightsource.shader");
}

glm::vec4* Light::GetColor() {
	return &color;
}

void Light::SetAnimation(bool(*funcPtr)(Light&), const char* name) {
	setShaderUniforms = funcPtr;
	setShaderUniformsName = name;
}

bool Light::SetShaderUniforms() {

	if (setShaderUniforms == nullptr)
		return false;

	if (setShaderUniforms(*this)) {
			return true;
	}
	else
		return false;
	
}

json Light::Serialize()
{
	json j;
	j["Light"]["name"] = name;
	j["Light"]["render"] = render;
	j["Light"]["setShaderUniformsName"] = setShaderUniformsName;
	j["Light"]["lightUniforms"]["redIncrementor"] = lightUniforms.redIncrementor;
	j["Light"]["lightUniforms"]["greenIncrementor"] = lightUniforms.greenIncrementor;
	j["Light"]["lightUniforms"]["blueIncrementor"] = lightUniforms.blueIncrementor;
	j["Light"]["position"] = transform.position;
	j["Light"]["direction"] = direction;
	j["Light"]["meshPath"] = meshPath;
	j["Light"]["color"]["R"] = color.r;
	j["Light"]["color"]["G"] = color.g;
	j["Light"]["color"]["B"] = color.b;
	j["Light"]["color"]["A"] = color.a;
	j["Light"]["guiLightType"] = guiLightType;
	j["Light"]["ambient"] = ambient;
	j["Light"]["diffuse"] = diffuse;
	j["Light"]["specular"] = specular;
	j["Light"]["linear"] = linear;
	j["Light"]["constant"] = constant;
	j["Light"]["quadratic"] = quadratic;
	j["Light"]["cutoff"] = cutoff;
	j["Light"]["outerCutoff"] = outerCutoff;

	return j;
}

void Light::DeSerialize(json& j) {

	for (auto& el : j.items())
	{
		if (el.key() == "Light")
		{

			for (auto& el2 : j["Light"].items())
			{
				if (el2.key() == "name")
				{

					name = el2.value();
				}

				if (el2.key() == "render")
				{

					render = el2.value();
				}

				if (el2.key() == "setShaderUniformsName")
				{
					if (el2.value() == "allAnimations::changeLightColor") {
						SetAnimation(allAnimations::ChangeLightColor, "allAnimations::changeLightColor");
					}
				}

				if (el2.key() == "meshPath") {
					meshPath = el2.value();
					UseMesh(meshPath.c_str());
				}

				if (el2.key() == "cutoff")
				{

					cutoff = el2.value();
				}

				if (el2.key() == "outerCutoff")
				{

					outerCutoff = el2.value();
				}

				if (el2.key() == "ambient")
				{

					for (auto& el3 : j["Light"]["ambient"].items())
					{
						if (el3.key() == "x")
						{
							ambient.x = el3.value();
						}
						if (el3.key() == "y")
						{
							ambient.y = el3.value();
						}
						if (el3.key() == "z")
						{
							ambient.z = el3.value();
						}

					}
				}
				if (el2.key() == "diffuse")
				{

					for (auto& el3 : j["Light"]["diffuse"].items())
					{
						if (el3.key() == "x")
						{
							diffuse.x = el3.value();
						}
						if (el3.key() == "y")
						{
							diffuse.y = el3.value();
						}
						if (el3.key() == "z")
						{
							diffuse.z = el3.value();
						}

					}
				}
				if (el2.key() == "specular")
				{

					for (auto& el3 : j["Light"]["specular"].items())
					{
						if (el3.key() == "x")
						{
							specular.x = el3.value();
						}
						if (el3.key() == "y")
						{
							specular.y = el3.value();
						}
						if (el3.key() == "z")
						{
							specular.z = el3.value();
						}

					}
				}

				if (el2.key() == "linear")
				{
					linear = el2.value();
				}

				if (el2.key() == "constant")
				{
					constant = el2.value();
				}

				if (el2.key() == "quadratic")
				{
					quadratic = el2.value();
				}
				

				if (el2.key() == "position")
				{
					for (auto& el3 : j["Light"]["position"].items())
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
				if (el2.key() == "direction")
				{
					for (auto& el3 : j["Light"]["direction"].items())
					{
						if (el3.key() == "x")
						{
							direction.x = el3.value();
						}
						if (el3.key() == "y")
						{
							direction.y = el3.value();
						}
						if (el3.key() == "z")
						{
							direction.z = el3.value();
						}

					}
				}
				if (el2.key() == "color") {

					for (auto& el3 : j["Light"]["color"].items())
					{
						if (el3.key() == "R")
						{
							color.r = el3.value();
						}
						if (el3.key() == "G")
						{
							color.g = el3.value();
						}
						if (el3.key() == "B")
						{
							color.b = el3.value();
						}
						if (el3.key() == "A")
						{
							color.a = el3.value();
						}
						shader.use();
						shader.setVec3("color", color.r, color.g, color.b);
						lightUniforms.redRadian = color.r;
						lightUniforms.greenRadian = color.g;
						lightUniforms.blueRadian = color.b;

					}
				}
				if (el2.key() == "lightUniforms") {

					for (auto& el3 : j["Light"]["lightUniforms"].items())
					{
						if (el3.key() == "redIncrementor")
						{
							lightUniforms.redIncrementor = el3.value();
						}
						if (el3.key() == "greenIncrementor")
						{
							lightUniforms.greenIncrementor = el3.value();
						}
						if (el3.key() == "blueIncrementor")
						{
							lightUniforms.blueIncrementor = el3.value();
						}
					}
				}
				if (el2.key() == "guiLightType") {
					guiLightType = el2.value();
				}
				
			}

		}
	}

}