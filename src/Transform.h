#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace glm
{
    class Transform
    {
    public:
        glm::vec3 position;
    };

    void to_json(json& j, const vec3& P);
    void from_json(const json& j, vec3& P);

}


#endif
