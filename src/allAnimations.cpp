/******************************************************
*   OpenGL demo
* 
*   allAnimations is a namespace that contains all functions
*   that animate anything in the demo.
*   
    Author: Magnus Söderberg
*   
********************************************************/

#include "allAnimations.h"
#include "GameObject.h"
#include "Light.h"
#include "Model.h"

//Interpolates between the vertices of the open and closed model of the chest
//I.e, changes the vertex positions and normals over time
bool allAnimations::ChestAnimation(Model& m) {

    static float tween = 0.0f;
    static int sign = 1;
    tween += 0.01f * sign;
    if (tween >= 1.0f) {
        tween = 1.0f;
        sign = -1;
        return false;
    }
    if (tween <= 0.0f) {
        tween = 0.0f;
        sign = 1;
        return false;
    }

    m.shader.setFloat("tweenFactor", tween);
    return true;
}

//Function to move texture coordinates over time
bool allAnimations::StreamAnimation(Model& m) {
    static float t_x = 0.0f;
    static float t_x_rad = 0.0f;
    static float t_y = 1.0f;

    t_x_rad += 0.0025f;

    t_x = glm::sin(t_x_rad);
    t_x /= 12;

    t_y -= 0.001f;


    m.shader.setInt("shift_texture", 1);
    m.shader.setFloat("tex_x_shift", t_x);
    m.shader.setFloat("tex_y_shift", t_y);
    return true;
}

//Moves the start screen back and forth
bool allAnimations::StartScreenAnimation(Model& m) {
    static glm::vec3 endPos(12.0f, -5.0, 0.0);
    static glm::vec3 startPos(27.0, 0.0, 0.0);
    static float startAngle = 0.0f;
    static float endAngle = 22.0f;
    static int sign = 1;

    static float increment = 0.0f;
    static glm::vec3 currentPos;
    static float currentAngle;

    currentPos = endPos * increment + startPos*(1 - increment);
    currentAngle = endAngle * increment + startAngle * (1 - increment);
    increment += 0.01f * sign;

    if (increment >= 1.0f) {
        sign = -1;
        increment = 1.0f;
        return false;
    }
    if (increment < 0.0f) {
        sign = 1;
        increment = 0.0f;
        return false;
    }

    m.transform.position = currentPos;
    m.rotationAngle = currentAngle;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(currentAngle), glm::vec3(0,0,1));
    model = glm::translate(model, m.transform.position);
    m.shader.setMat4("model", model);

    return true;
}

//Changes the light color of lights
bool allAnimations::ChangeLightColor(Light& l) {

    l.lightUniforms.redRadian += l.lightUniforms.redIncrementor;
    l.lightUniforms.greenRadian += l.lightUniforms.greenIncrementor;
    l.lightUniforms.blueRadian += l.lightUniforms.blueIncrementor;
    
    l.color.r = glm::sin(l.lightUniforms.redRadian);
    l.color.g = glm::sin(l.lightUniforms.greenRadian);
    l.color.b = glm::sin(l.lightUniforms.blueRadian);


    if (l.color.r < 0) {
        l.color.r *= -1;
    }
    if (l.color.g < 0) {
        l.color.g *= -1;
    }
    if (l.color.b < 0) {
        l.color.b *= -1;
    }

    l.shader.setVec3("color", l.color.r, l.color.g, l.color.b);

    return true;
}
