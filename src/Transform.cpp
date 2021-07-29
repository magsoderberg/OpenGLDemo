/******************************************************
*   OpenGL demo
*
*   Transform is holds the position of a GameObject.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "Transform.h"


void glm::to_json(json& j, const vec3& P)
{
    j = json{ {"x", P.x}, {"y", P.y}, {"z", P.z} };
}


void glm::from_json(const json& j, vec3& P) {
    j.at("x").get_to(P.x);
    j.at("y").get_to(P.y);
    j.at("z").get_to(P.z);
}