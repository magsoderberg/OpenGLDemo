#ifndef ALLMESHES_H
#define ALLMESHES_H

#include <vector>

#include "Mesh.h"

namespace allMeshes {
	static std::vector<std::shared_ptr<Mesh>> allMeshesList;
	void AddMesh(const char* meshPath);
	const char* GetName(int i);
	std::shared_ptr<Mesh> FindMesh(const char* texturePath);
	std::vector<const char*> GetAllMeshesNames();
	inline int GetSize() { return allMeshesList.size(); }
	int GetVAO(int index);
	std::shared_ptr<Mesh> GetMesh(int i);
	void Serialize();
	void DeSerialize();

}

#endif
