#ifndef ALLTEXTURES_H
#define ALLTEXTURES_H

#include "Texture.h"
#include <memory>
#include <vector>

namespace allTextures {
	static std::vector<std::shared_ptr<Texture>> allTextureList;
	void AddTexture(const char* texturePath);
	inline const int GetSize() { return allTextureList.size(); }
	int FindTexture(const char* texturePath);
	std::shared_ptr<Texture> GetTexture(int index);
	std::vector<const char*> GetAllTextureNames();
	void Serialize();
	void DeSerialize();
}


#endif


