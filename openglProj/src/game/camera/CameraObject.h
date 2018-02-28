#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/game/gameObject/parent/DrawableObject.h"

class CameraObject : public DrawableObject {
	public:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		bool bProjection = true;

		glm::vec3 direction;
		glm::vec3 right;
		glm::vec3 up;

		float horizontalAngle; // == 3.14f == pi : -z direction
		float verticalAngle;	//0.0f
		float speed; // 3.0f == 3 units / second
		float mouseSpeed; // default 0.005f

		float fov;	//default 45.0f degree
		float horizontalFovValue;	//4:3 window ratio
		float verticalFovValue;
		float frontFrustrumValue;	//0.1f
		float backFrustrumValue;	//100.0f

		CameraObject();

		CameraObject(glm::vec3 modelVec, float horizontalAngle, float verticalAngle);

		CameraObject(glm::vec3 modelVec, float horizontalAngle, float verticalAngle, float fov, float speed = 0.1f, float mouseSpeed = 0.005f,
			float horizontalFovValue = 4.0f, float verticalFovValue = 3.0f, float frontFrustrumValue = 0.1f, float backFrustrumValue = 100.0f);

		void modifyAngleWithXYPos(float xPos, float yPos);

		void updateViewMatrix();

		void updateViewMatrixWithParam(glm::vec3 eye, glm::vec3 lookCenter, glm::vec3 up);

		void updateProjectionMatrix();

		glm::mat4 getProjectionMatrix() {
			return projectionMatrix;
		}

		glm::mat4 getViewMatrix() {
			return viewMatrix;
		}

		glm::mat4 getModelMatrix() {
			return glm::translate(glm::mat4(), modelVec);
		}

		virtual ~CameraObject() {}
};