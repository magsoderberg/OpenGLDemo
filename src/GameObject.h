#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"
#include "Animation.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <deque>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;


class GameObject {
public:
	GameObject();
	void UseTexture(const char* texturePath);
	void UseMesh(const char* meshPath);
	void UseShader(const char*, const char*);
	const char* GetName();
	unsigned int GetID();
	void ChangeName(const char* newName);
	virtual json Serialize() = 0;
	virtual void DeSerialize(json& j) = 0;
	unsigned int GetVAO() { return VAO; }
	unsigned int GetNumOfElements() { return numElements; }
	void SetID(int i) { ID = i; }
	void ChangeReceivingLightFrom(int index, bool b);
	bool * GetReceiveLightFrom(int index);
	Shader shader;
	unsigned int texture;
	unsigned int numElements;
	unsigned int numElements_nonAnim;
	unsigned int VAO;
	unsigned int VAO_nonAnim;
	unsigned int ID;
	static unsigned int ID_Counter;
	glm::mat4 model, projection, view;
	std::string name;
	glm::Transform transform;
	std::vector<unsigned int> aniVAO;
	std::vector<unsigned int> aniEBO;
	std::vector<std::shared_ptr<Animation>> animations;
	bool virtual SetShaderUniforms() = 0;
	void PlayAnimation();
	void StopAnimation();
	bool playAnimationOnMousePicked = true;
	bool playAnimation = false;
	int guiVertexShader = 0;
	int guiFragmentShader = 0;
	int guiTexture = 0;
	void MousePicked();
	bool render = true;

protected:
	std::string texturePath;
	std::string meshPath;
	std::string vshaderPath;
	std::string fshaderPath;

private:
	std::vector<glm::vec3> positions;
	std::deque<bool> guiReceiveLightFrom;

};



#endif