#ifndef ALLSHADERS_H
#define ALLSHADERS_H

#include <vector>
#include <string>
#include <iostream>

namespace allShaders {
	static std::vector<std::string> allVertexShaders;
	static std::vector<std::string> allFragmentShaders;
	int FindVertexShader(const char* vertexPath);
	int FindFragmentShader(const char* fragmentPath);
	void AddVertexShader(std::string shaderPath);
	void AddFragmentShader(std::string shaderPath);
	const char* GetVertexShaderPath(int index);
	const char* GetFragmentShaderPath(int index);
	std::vector<const char*> GetAllVertexShaderNames();
	std::vector<const char*> GetAllFragmentShaderNames();
	void Serialize();
	void DeSerialize();

};

#endif


