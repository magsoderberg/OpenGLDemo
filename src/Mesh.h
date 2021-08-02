#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <sstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;


class Mesh
{
	public:
		Mesh() {};
		Mesh(const char* meshPath);
		void Init(const char* meshPath);
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<float> texCoords;
		std::vector<float> boundingBox;
		std::vector<glm::vec4> _vertPos;
		unsigned int GetVAO() { return VAO; }
		unsigned int GetNumElements() { return numElements; }
		unsigned int VAO, VBO, EBO, texture;
		const char* GetName() { return meshPath.c_str(); }
		json Serialize();
		const char* GetType();
		std::string name;
		std::string texturePath;
		
	private:
		const aiScene* scene;
		const aiMesh* mesh;
		unsigned int numElements;
		std::string meshPath;
};

#endif