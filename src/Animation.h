#ifndef ANIMATION_H
#define ANIMATION_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Animation {
public:
	Animation(std::vector<std::string> v);
	void LoadAnimation(std::vector<std::string>);
	std::vector<std::string> keyFrameMeshesPath;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> texCoords;
	unsigned int VAO, VBO, EBO, texture;
	const aiScene* scene_open;
	const aiScene* scene_closed;
	const aiMesh* mesh_open;
	const aiMesh* mesh_closed;
	unsigned int numElements;

};
#endif
