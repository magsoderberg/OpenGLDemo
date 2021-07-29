#ifndef MODEL_H
#define MODEL_H

#include <iostream>

#include "GameObject.h"
#include "Light.h"
#include "allLights.h"

class Model : public GameObject {
public:
	inline bool GUIGetReceiveLightFrom(int index) { return guiReceiveLightFrom[index]; }
	inline void GUISetReceiveLightFrom(int index, bool b) { guiReceiveLightFrom[index] = b; }
	void AddLight(int index);
	void RemoveLight(int index);
	void UpdateShaderUniforms();
	void SetAnimation(int index, bool(*funcPtr)(Model&), const char* name);
	bool SetShaderUniforms() override;
	std::vector<const char*> GetAddedLightNames();
	json Serialize() override;
	void DeSerialize(json& j) override;
	void UpdateLightColor();
	int gui_light_selected = 0;
	glm::vec3 rotateAround = glm::vec3(1.0f, 1.0f, 1.0f);
	float rotationAngle = 0.0f;
	bool(*setShaderUniforms)(Model&) = nullptr;
	std::string setShaderUniformsName;
	int modelAnimationChoice = 0;
	
private:
	std::vector<int> guiReceiveLightFrom;
	std::vector<std::shared_ptr<Light>> lights;


};

#endif
