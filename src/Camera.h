#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Camera
{
private:

	float lastX;
	float lastY;
	float yaw;
	float pitch;

	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;

public:
	Camera();
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	bool inTransition = false;
	void SetTransition(glm::vec3 start, glm::vec3 direction, int d);
	void SmoothTransition();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();
	void setCameraDirection(float x, float y, bool firstMouse);
	void setCameraDirectionDuringTransition(float yaw, float pitch);
	void setCameraPosition(Camera_Movement CAM_DIR_ENUM, float x);
	glm::mat4 getViewMatrix();
	void Serialize();
	void DeSerialize();

	bool inMenu = false;
	bool fixMouseAfterMenu = false;
	bool firstMouse = true;

private:
	glm::vec3 transitionEndPosition;
	glm::vec3 transitionStartPosition;
	glm::vec3 transitionEndDirection;
	glm::vec3 transitionStartDirection;
	float startingAngle;
	float endAngle;
	glm::vec3 playerPositions[4]{ glm::vec3(40.0, 0.0, 0.0), glm::vec3(0.0, 10.0, 40.0), glm::vec3(-40.0,0.0,0.0), glm::vec3(0.0,5.0,-40.0) };
	float playerPitch[4]{ 0.0f, -20.0f, 0.0f, -10.0f };
	float endPitch;
	float endYaw;
	float startPitch;
	float startYaw;
	float playerPositionAngles[4]{ 0.0f, 90.0f, 180.0f, 270.0f };
};


#endif