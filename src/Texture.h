#ifndef TEXTURE
#define TEXTURE

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Texture {
public:
	Texture(const char* texturePath);
	const char* GetName() { return textureName.c_str(); }
	unsigned int GetTexture() { return texture; }
	json Serialize();
private:
	unsigned int texture;
	std::string textureName;
};

#endif


