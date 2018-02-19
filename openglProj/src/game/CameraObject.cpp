#include "src/game/CameraObject.h"

CameraObject::CameraObject() : DrawableObject(){
	
	horizontalAngle = 3.14f; // == pi : -z direction
	verticalAngle = 0.0f;
	speed = 0.1f; // 3 units / second
	mouseSpeed = 0.005f;

	fov = 45.0f;
	horizontalFovValue = 4.0f;
	verticalFovValue = 3.0f;
	frontFrustrumValue = 0.1f;
	backFrustrumValue = 100.0f;

	updateViewMatrix();
	updateProjectionMatrix();
}


CameraObject::CameraObject(glm::vec3 modelVec, float horizontalAngle, float verticalAngle) : DrawableObject(modelVec), horizontalAngle(horizontalAngle), verticalAngle(verticalAngle){
	speed = 0.1f; // 3 units / second
	mouseSpeed = 0.005f;

	fov = 45.0f;
	horizontalFovValue = 4.0f;
	verticalFovValue = 3.0f;
	frontFrustrumValue = 0.1f;
	backFrustrumValue = 100.0f;

	updateViewMatrix();
	updateProjectionMatrix();
}

CameraObject::CameraObject(glm::vec3 modelVec, float horizontalAngle, float verticalAngle, float fov, float speed, float mouseSpeed,
	float horizontalFovValue, float verticalFovValue, float frontFrustrumValue, float backFrustrumValue) :
	DrawableObject(modelVec), horizontalAngle(horizontalAngle), verticalAngle(verticalAngle), fov(fov), speed(speed), mouseSpeed(mouseSpeed), horizontalFovValue(horizontalFovValue),
	verticalFovValue(verticalFovValue), frontFrustrumValue(frontFrustrumValue), backFrustrumValue(backFrustrumValue){

	updateViewMatrix();
	updateProjectionMatrix();
}


void CameraObject::modifyAngleWithXYPos(float xPos, float yPos) {
	horizontalAngle += mouseSpeed * float(1024 / 2 - xPos);
	verticalAngle += mouseSpeed * float(768 / 2 - yPos);
}


void CameraObject::updateViewMatrix() {
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);

	viewMatrix = glm::lookAt(modelVec, modelVec + direction, up);
}

void CameraObject::updateViewMatrixWithParam(glm::vec3 eye, glm::vec3 lookCenter, glm::vec3 up) {
	viewMatrix = glm::lookAt(eye, lookCenter, up);
}

void CameraObject::updateProjectionMatrix() {
	projectionMatrix = glm::perspective(glm::radians(fov), horizontalFovValue / verticalFovValue, frontFrustrumValue, backFrustrumValue);
}