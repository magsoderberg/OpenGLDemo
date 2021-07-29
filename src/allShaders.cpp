/******************************************************
*   OpenGL demo
*
*   allShaders is a namespace that contains and handles
*	all Shader:s in the demo.
*	A Shader contains both a vertex and fragment shader
*	program as well as helper functions to communicate
*	between shaders and CPU.
*	
*	Author: Magnus Söderberg
*
********************************************************/

#include "allShaders.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


void allShaders::AddVertexShader(std::string shaderPath) {
	allVertexShaders.push_back(shaderPath);
}

void allShaders::AddFragmentShader(std::string shaderPath) {
	allFragmentShaders.push_back(shaderPath);
}

const char* allShaders::GetVertexShaderPath(int index) {
	if (index < allVertexShaders.size() && index >= 0)
		return allVertexShaders[index].c_str();
	else
		throw std::runtime_error("---ERROR: Wanted Vertex Shader was out of range----");

}

const char* allShaders::GetFragmentShaderPath(int index) {
	if (index < allFragmentShaders.size() && index >= 0)
		return allFragmentShaders[index].c_str();
	else
		throw std::runtime_error("---ERROR: Wanted Fragment Shader was out of range----");
}

int allShaders::FindVertexShader(const char* vertexPath) {

	for (int i = 0; i < allVertexShaders.size(); i++) {
		if (std::string(allVertexShaders[i]).compare(std::string(vertexPath)) == 0) {
			return i;
		}
	}
	return -1;
}

int allShaders::FindFragmentShader(const char* fragmentPath) {

	for (int i = 0; i < allFragmentShaders.size(); i++) {
		if (std::string(allFragmentShaders[i]).compare(std::string(fragmentPath)) == 0) {
			return i;
		}
	}
	return -1;
}

std::vector<const char*> allShaders::GetAllVertexShaderNames() {
	std::vector<const char*> list;
	for (int i = 0; i < allVertexShaders.size(); i++) {
		list.push_back(allVertexShaders[i].c_str());
	}
	return list;
}

std::vector<const char*> allShaders::GetAllFragmentShaderNames() {
	std::vector<const char*> list;
	for (int i = 0; i < allFragmentShaders.size(); i++) {
		list.push_back(allFragmentShaders[i].c_str());
	}
	return list;
}


void allShaders::Serialize() {

	std::ofstream out("saved_data/shaders.json");
	out << "[";

	json numVertShaders;
	numVertShaders["Number Of Vertex Shaders"] = allVertexShaders.size();
	out << numVertShaders;
	out << ",";

	json numFragShaders;
	numFragShaders["Number Of Fragment Shaders"] = allFragmentShaders.size();
	out << numFragShaders;
	out << ",";

	for (int i = 0; i < allVertexShaders.size(); i++)
	{
		json jVertex;
		jVertex["vertexShader"] = allVertexShaders[i];
		out << std::setw(4) << jVertex << ",";
		
	}
	for (int i = 0; i < allFragmentShaders.size(); i++)
	{
		json jFragment;
		
		jFragment["fragmentShader"] = allFragmentShaders[i];
		out << std::setw(4) << jFragment;
		if (i != allFragmentShaders.size() - 1)
		{
			out << ",";
		}
	}

	out << "]";

}

void allShaders::DeSerialize() {

	json j;
	{
		std::ifstream in("saved_data/shaders.json");

		in >> j;
		in.close();
	}

	int numVertShaders;
	int numFragShaders;

	for (auto& el : j[0].items())
	{
		if (el.key() == "Number Of Vertex Shaders")
		{
			numVertShaders = el.value();
			std::cout << "NUM VERT SHADERS " << numVertShaders << std::endl;
		}
	}

	for (auto& el : j[1].items())
	{
		if (el.key() == "Number Of Fragment Shaders")
		{
			numFragShaders = el.value();
			std::cout << "NUM FRAG SHADERS " << numFragShaders << std::endl;
		}
	}

	for (int i = 2; i < numVertShaders + numFragShaders + 2; i++) {
		for (auto& el : j[i].items()) {
			if (el.key() == "vertexShader") {
				allVertexShaders.push_back(std::string(el.value()));
			}
			if (el.key() == "fragmentShader") {
				allFragmentShaders.push_back(std::string(el.value()));
			}
		}
	}
	std::cout << "Deserialized Shader:s " << std::endl;
}

