/******************************************************
*   OpenGL demo
*
*   Camera makes it possible to move around in the demo. 
*   Only one camera is used in the demo.
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "Camera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iomanip>

Camera::Camera()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
}

void Camera::Serialize() {

    std::ofstream out("saved_data/cameras.json");
    out << "[";
    
    json jHeader;
    jHeader["Number Of Cameras"] = 1;
    out << jHeader;
    out << ",";
    
    json jPos;
    
    jPos["cameraPos"]["x"] = cameraPos.x;
    jPos["cameraPos"]["y"] = cameraPos.y;
    jPos["cameraPos"]["z"] = cameraPos.z;
    out << std::setw(4) << jPos << ",";

    json jUp;

    jUp["cameraUp"]["x"] = cameraUp.x;
    jUp["cameraUp"]["y"] = cameraUp.y;
    jUp["cameraUp"]["z"] = cameraUp.z;
    out << std::setw(4) << jUp << ",";

    json jFront;

    jFront["cameraFront"]["x"] = cameraFront.x;
    jFront["cameraFront"]["y"] = cameraFront.y;
    jFront["cameraFront"]["z"] = cameraFront.z;
    out << std::setw(4) << jFront << ",";

    json jYaw;
    jYaw["yaw"] = yaw;

    json jPitch;
    jPitch["pitch"] = pitch;

    out << std::setw(4) << jYaw << ",";

    out << std::setw(4) << jPitch;

    out << "]";
}

void Camera::DeSerialize() {
    json j;
    {
        std::ifstream in("saved_data/cameras.json");

        in >> j;
        in.close();
    }
    int num = 0;

    for (auto& el : j[0].items())
    {
        if (el.key() == "Number Of Cameras")
        {
            num = el.value();
        }
    }

    json jPos = j[1];

    for (auto& el : jPos.items()) {

        if (el.key() == "cameraPos") {

            for (auto& el2 : jPos["cameraPos"].items()) {
                if (el2.key() == "x")
                    cameraPos.x = el2.value();
                if (el2.key() == "y")
                    cameraPos.y = el2.value();
                if (el2.key() == "z")
                    cameraPos.z = el2.value();
            }

        }
            
    }

    json jUp = j[2];

    for (auto& el : jUp.items()) {

        if (el.key() == "cameraUp") {

            for (auto& el2 : jUp["cameraUp"].items()) {
                if (el2.key() == "x")
                    cameraUp.x = el2.value();
                if (el2.key() == "y")
                    cameraUp.y = el2.value();
                if (el2.key() == "z")
                    cameraUp.z = el2.value();
            }

        }

    }

    json jFront = j[3];

    for (auto& el : jFront.items()) {

        if (el.key() == "cameraFront") {

            for (auto& el2 : jFront["cameraFront"].items()) {
                if (el2.key() == "x")
                    cameraFront.x = el2.value();
                if (el2.key() == "y")
                    cameraFront.y = el2.value();
                if (el2.key() == "z")
                    cameraFront.z = el2.value();
            }

        }

    }

    for (auto& el : j[4].items()) {
        if (el.key() == "yaw") {
            yaw = el.value();
        }
    }

    for (auto& el : j[5].items()) {
        if (el.key() == "pitch") {
            pitch = el.value();
        }
    }


}

glm::vec3 Camera::GetCameraPosition()
{
    return cameraPos;
}

glm::vec3 Camera::GetCameraDirection()
{
    return cameraFront;
}

void Camera::setCameraDirection(float xpos, float ypos, bool firstMouse)
{
 
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }


    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    
}

void Camera::setCameraDirectionDuringTransition(float yaw, float pitch) {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->yaw = yaw;
    this->pitch = pitch;
    cameraFront = glm::normalize(direction);
}

void Camera::setCameraPosition(Camera_Movement CAM_DIR_ENUM, float cameraSpeed)
{
    glm::vec3 movementDirection;
    if (CAM_DIR_ENUM == FORWARD)
    {
        movementDirection = cameraFront;
    }
    else if(CAM_DIR_ENUM == BACKWARD)
    {
        movementDirection = -cameraFront;
    }
    else if (CAM_DIR_ENUM == LEFT)
    {
        movementDirection = -glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    else if (CAM_DIR_ENUM == RIGHT)
    {
        movementDirection = glm::normalize(glm::cross(cameraFront, cameraUp));
    }

    cameraPos += movementDirection * cameraSpeed;

}

void Camera::SetTransition(glm::vec3 start, glm::vec3 direction, int d) {

    static int startPos = 0;
    static int endPos = 0;
    
    endPos += d;
    
    if (endPos > 3)
        endPos = 0;
    if (endPos < 0)
        endPos = 3;

    inTransition = true;
    transitionStartPosition = start;
    transitionEndPosition = playerPositions[endPos];

    glm::normalize(direction);

    startingAngle = playerPositionAngles[startPos];
    endAngle = playerPositionAngles[endPos];

    endPitch = playerPitch[endPos];
    startPitch = pitch;

    endYaw = yaw + 90;
    startYaw = yaw;

    startPos = endPos;

}

void Camera::SmoothTransition() {

    static float increment = 0.0f;
    static float currentAngle = 0;
    static float currentPitch = 0.0f;
    static float currentYaw = 0.0f;

    currentAngle = startingAngle + 90 * increment;

    increment += 0.0005f;
    if (increment > 1.0f) {
        increment = 1.0f;
    }

    //Change camera direction
    currentPitch = endPitch * increment + startPitch * (1 - increment);
    currentYaw = endYaw * increment + startYaw * (1 - increment);
    setCameraDirectionDuringTransition(currentYaw, currentPitch);

    //Change player position
    float x = 40 * glm::cos(glm::radians(currentAngle));
    float y = transitionEndPosition.y * increment + transitionStartPosition.y * (1 - increment);
    float z = 40 * glm::sin(glm::radians(currentAngle));

    static glm::vec3 currentPos = transitionStartPosition;
    glm::vec3 newPos = glm::vec3(x, y, z);
    glm::vec3 move = newPos - currentPos;
    cameraPos = currentPos + move;
    currentPos = newPos;

    if(increment >= 1.0f) {
       // cameraFront = transitionEndDirection;
        cameraPos = transitionEndPosition;
        fixMouseAfterMenu = true;
        inTransition = false;
        currentAngle = endAngle;
        increment = 0.0f;
    }

}

glm::mat4 Camera::getViewMatrix()
{
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}
