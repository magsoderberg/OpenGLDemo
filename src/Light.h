#ifndef LIGHT_H
#define LIGHT_H

#include "GameObject.h"
#include <string>

class Model;

struct LightUniforms {
	float redIncrementor;
	float greenIncrementor;
	float blueIncrementor;

	float redRadian;
	float greenRadian;
	float blueRadian;

};

class Light : public GameObject {
public:
	Light();
	glm::vec4* GetColor();
	json Serialize() override;
	void DeSerialize(json& j) override;
	void SetAnimation(bool(*funcPtr)(Light&), const char*);
	bool SetShaderUniforms() override;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 spotDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	float cutoff = 0.0f;
	float outerCutoff = 0.0f;
	glm::vec3 ambient = glm::vec3(0);
	glm::vec3 diffuse = glm::vec3(0);
	glm::vec3 specular = glm::vec3(0);
	std::vector<const char*> lightTypes{ "Direction", "Point", "Spot", "SpotColored" };
	int guiLightType = 0;
	float linear = 0.09f;
	float constant = 1.0f;
	float quadratic = 0.032f;
	std::vector<Model*> models;

	int lightAnimationChoice = 0;
	float changeColorIncrementor = 0.0f;
	int changeColorSign = 1;

	bool(*setShaderUniforms)(Light&) = nullptr;
	std::string setShaderUniformsName;

	LightUniforms lightUniforms;

};


#endif

