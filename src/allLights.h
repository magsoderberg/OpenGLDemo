#ifndef ALLLIGHTS_H
#define ALLLIGHTS_H

#include <memory>
#include <vector>

#include "GameObject.h"
#include "Mesh.h"
#include "Light.h"


namespace allLights {
	static std::vector<std::shared_ptr<Light>> allLightsList;
	void CreateLight();
	void DeleteObject(int index);
	inline const char* GetName(int i) { return allLightsList[i]->GetName(); }
	inline int GetID(int i) { return allLightsList[i]->GetID(); }
	std::shared_ptr<Light> GetObject(int index);
	std::vector<const char*> GetAllLightNames();
	inline int GetSize() { return allLightsList.size(); }
	void Serialize();
	void DeSerialize();
}

#endif


