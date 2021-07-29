#ifndef ALLANIMATIONS_H
#define ALLANIMATIONS_H
#include "Shader.h"

class GameObject;
class Light;
class Model;

namespace allAnimations {

	bool ChestAnimation(Model&);
	bool StreamAnimation(Model&);
	bool StartScreenAnimation(Model& m);
	bool ChangeLightColor(Light&);
	static std::vector<const char*> lightAnimations = { "ChangeLightColor" };
	static const char * const modelAnimations[] = {"ChestAnimation", "StreamAnimation", "StartScreenAnimation" };
}

#endif



