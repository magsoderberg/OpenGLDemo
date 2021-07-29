/******************************************************
*   OpenGL demo
*
*   allTextures is a namespace that contains and handles
*	all Texture:s that has been loaded in the demo.
*	A Shader contains both a vertex and fragment shader
*	program as well as helper functions to communicate
*	between shaders and CPU.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "allTextures.h"
#include "Texture.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <iomanip>

using json = nlohmann::json;

void allTextures::AddTexture(const char* texturePath) {
	allTextureList.push_back(std::make_shared<Texture>(texturePath));
}


int allTextures::FindTexture(const char* texturePath) {

	for (int i = 0; i < allTextureList.size(); i++) {
		if (std::string(allTextureList[i]->GetName()).compare(std::string(texturePath)) == 0) {

			return allTextureList[i]->GetTexture();
		}
	}
	return -1;
}

std::shared_ptr<Texture> allTextures::GetTexture(int index) { 
	if (index < allTextureList.size() && index >= 0)
		return allTextureList[index];
	else
		throw std::runtime_error("---ERROR: Wanted Texture was out of range----");

}


std::vector<const char*> allTextures::GetAllTextureNames() {
	std::vector<const char*> allTextureNames;
	for (int i = 0; i < allTextureList.size(); i++) {
		allTextureNames.push_back(allTextureList[i]->GetName());
	}
	return allTextureNames;
}


void allTextures::Serialize() {
	std::ofstream out("saved_data/textures.json");
	out << "[";
	json j;
	j["Number Of Textures"] = allTextureList.size();
	out << j;
	out << ",";
	for (int i = 0; i < allTextureList.size(); i++)
	{
		out << std::setw(4) << allTextureList[i]->Serialize();
		if (i != allTextureList.size() - 1)
		{
			out << ",";
		}
	}
	out << "]";
}

void allTextures::DeSerialize() {

	json j;
	{
		std::ifstream in("saved_data/textures.json");

		in >> j;
		in.close();
	}
	int num = 0;

	for (auto& el : j[0].items())
	{
		if (el.key() == "Number Of Textures")
		{
			num = el.value();
		}
	}

	for (int i = 1; i < num + 1; i++) {
		for (auto& el : j[i].items()) {
			if (el.key() == "texturePath") {
				std::string path = el.value();
				allTextureList.push_back(std::make_shared<Texture>(path.c_str()));
			}
		}
	}
	std::cout << "Deserialized Texture:s " << std::endl;
}
