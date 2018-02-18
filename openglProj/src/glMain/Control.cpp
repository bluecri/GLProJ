

#include "src/glMain/Control.h"

float Control::s_curTime = glfwGetTime();
float Control::s_curTimeForFrame = s_curTime;

void Control::m_changeCameraIndex(int idx) {
	if (-1 < idx && idx < m_cameraObjectVec.size()) {
		m_curCameraObjextIndex = idx;
	}
}
void Control::m_modifyCurCameraWithInput() {

	CameraObject* curCamera = m_cameraObjectVec[m_curCameraObjextIndex];

	//position
	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);	//get
	glfwSetCursorPos(m_window, 1024 / 2, 768 / 2);	//reset to center position

	curCamera->modifyAngleWithXYPos(xPos, yPos);

	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		curCamera->modelVec += curCamera->direction * m_deltaTime * curCamera->speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		curCamera->modelVec -= curCamera->direction * m_deltaTime * curCamera->speed;
	}
	// Strafe right
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		curCamera->modelVec += curCamera->right * m_deltaTime * curCamera->speed;
	}
	// Strafe left
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		curCamera->modelVec -= curCamera->right * m_deltaTime * curCamera->speed;
	}
	curCamera->updateViewMatrix();
	curCamera->updateProjectionMatrix();
}

void Control::m_inputProgress() {
	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);	//get
	glfwSetCursorPos(m_window, 1024 / 2, 768 / 2);	//reset to center position
	
	m_mouseDeltaXPos= float(1024 / 2 - xPos);
	m_mouseDeltaYPos = float(768 / 2 - yPos);
	
	iskeyUp[EnumKey::UP] = (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS);
	iskeyUp[EnumKey::DOWN] = (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS);
	iskeyUp[EnumKey::LEFT] = (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS);
	iskeyUp[EnumKey::RIGHT] = (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS);
	iskeyUp[EnumKey::SPACE] = (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS);
	
}

float Control::m_calcDeltaTime() {
	m_deltaTime = glfwGetTime() - s_curTime;
	s_curTime = glfwGetTime();
	//frame check
	float timeInterval = glfwGetTime() - s_curTimeForFrame;
	m_p_innerFps += 1;
	if (timeInterval >= 1.0) {
		m_fps = m_p_innerFps;
		m_p_innerFps = 0;
		printf("%d fps\n", m_fps);

		CameraObject* curCamera = m_cameraObjectVec[m_curCameraObjextIndex];
		printf("%f %f %f\n", curCamera->modelVec.x, curCamera->modelVec.y, curCamera->modelVec.z);

		s_curTimeForFrame += 1.0;
	}
	return m_deltaTime;
}
glm::mat4 Control::m_getCurCameraProjectionMatrix() {
	return m_cameraObjectVec[m_curCameraObjextIndex]->getProjectionMatrix();
}
glm::mat4 Control::m_getCurCameraModelMatrix() {
	return m_cameraObjectVec[m_curCameraObjextIndex]->getModelMatrix();
}
glm::mat4 Control::m_getCurCameraViewMatrix() {
	return m_cameraObjectVec[m_curCameraObjextIndex]->getViewMatrix();
}
glm::mat4 Control::m_getCurCameraMVPMatrix() {
	CameraObject* curCamera = m_cameraObjectVec[m_curCameraObjextIndex];

	return curCamera->getProjectionMatrix() * curCamera->getViewMatrix() * curCamera->getModelMatrix();
}

void Control::m_controlProgress() {
	m_calcDeltaTime();
	//m_modifyCurCameraWithInput();
	m_inputProgress();
}