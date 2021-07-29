/******************************************************
*   OpenGL demo
*
*   allMeshes is a namespace that contains all models
*	loaded into the demo.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "allMeshes.h"
#include <iostream>
#include <fstream>
#include <memory>

void allMeshes::AddMesh(const char* meshPath) {

	allMeshesList.push_back(std::make_shared<Mesh>(meshPath));
}


std::shared_ptr<Mesh> allMeshes::GetMesh(int i) { 
	if (i > GetSize() || i < 0) {
		throw std::runtime_error("---ERROR: Mesh index out of bounds---");
	}
	return allMeshesList[i]; 
}

const char* allMeshes::GetName(int i) {
	if (i > GetSize() || i < 0) {
		throw std::runtime_error("---ERROR: Mesh index out of bounds---");
	}
	return allMeshesList[i]->GetName();
}

int allMeshes::GetVAO(int i) { 
	if (i > GetSize() || i < 0) {
		throw std::runtime_error("---ERROR: Mesh index out of bounds---");
	}
	return allMeshesList[i]->VAO; 
}

std::vector<const char*> allMeshes::GetAllMeshesNames() {
	std::vector<const char*> allMeshesNames;
	for (int i = 0; i < allMeshesList.size(); i++) {
		allMeshesNames.push_back(allMeshesList[i]->GetName());
	}
	return allMeshesNames;
}

std::shared_ptr<Mesh> allMeshes::FindMesh(const char* meshPath) {

	for (int i = 0; i < allMeshesList.size(); i++) {
		if (std::string(allMeshesList[i]->GetName()).compare(std::string(meshPath)) == 0) {
			return allMeshesList[i];
		}
	}
	return 0;
}



void allMeshes::Serialize() {

	std::ofstream out("saved_data/meshes.json");
	out << "[";
	json j;
	j["Number Of Meshes"] = allMeshesList.size();
	out << j;
	out << ",";
	for (int i = 0; i < allMeshesList.size(); i++)
	{
		out << std::setw(4) << allMeshesList[i]->Serialize();
		if (i != allMeshesList.size() - 1)
		{
			out << ",";
		}
	}
	out << "]";

}

void allMeshes::DeSerialize() {

	json j;
	{
		std::ifstream in("saved_data/meshes.json");

		in >> j;
		in.close();
	}
	int num = 0;

	for (auto& el : j[0].items())
	{
		if (el.key() == "Number Of Meshes")
		{
			num = el.value();
		}
	}

	for (int i = 1; i < num+1; i++) {
		for (auto& el : j[i].items()) {
			if (el.key() == "meshPath") {
				std::string path = el.value();
				allMeshesList.push_back(std::make_shared<Mesh>(path.c_str()));
			}
		}

	}
	std::cout << "Deserialized all Mesh:es" << std::endl;


}