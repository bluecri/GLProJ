/******************************************************
*  Document   : Control.h
*  Description: window�κ��� input�� �ް� �׿� �ش��ϴ� ó���� �����Ѵ�.
main loop���� 1ycle�� 1���� �ݵ�� ȣ��ȴ�.
mouse, key input�� ó���Ѵ�.
camera���� ������ ������ main camera�� �����Ѵ�..
*******************************************************/

#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "src/game/CameraObject.h"

using namespace glm;

/**
*
* @brief Get Input from window and handling process result from input
* @details Get inputs, Move Camera, Frame Check
* @author bluecri
* @date 2018-02-12
* @version 0.0.1
*
*/
class Control {
private:
	int m_p_innerFps = 0;							///>	inner frame counter. changing per frame.
public:
	GLFWwindow * m_window;
	std::vector<CameraObject*> m_cameraObjectVec;	///> camera object storage vector

	int m_curCameraObjextIndex;						///> main camera index
	static float s_curTime;							///> For delta time check
	static float s_curTimeForFrame;					///> For frame check
	float m_deltaTime = 0;							///> delta time
	int m_fps = 0;	//use this for print frame per sec

	int m_width, m_height;	//window size;
	float m_mouseDeltaXPos, m_mouseDeltaYPos;

	enum EnumKey
	{
		UP = 0, DOWN, LEFT, RIGHT, SPACE, MOUSE_LEFT, ENUM_KEY_LEN
	};
	bool iskeyUp[ENUM_KEY_LEN];
	
	

	Control(GLFWwindow* window, int width, int height) : m_width(width), m_height(height){
		//m_cameraObjectVec.push_back(new CameraObject());
		m_window = window;
		glfwSetCursorPos(m_window, m_width / 2, m_height / 2);	//reset to center position
		m_mouseDeltaXPos = m_width / 2;
		m_mouseDeltaYPos = m_width / 2;
	}

	Control(int cameraNum) {
		for (int i = 0; i < cameraNum; i++) {
			m_cameraObjectVec.push_back(new CameraObject());
		}
	}
	void m_cameraAdd() {
		m_cameraObjectVec.push_back(new CameraObject());
	}

	void m_cameraAdd(glm::vec3 modelVec, float horizontalAngle, float verticalAngle) {
		m_cameraObjectVec.push_back(new CameraObject(modelVec, horizontalAngle, verticalAngle));
	}

	void m_cameraAdd(glm::vec3 modelVec, float horizontalAngle, float verticalAngle, float fov, float speed, float mouseSpeed,
		float horizontalFovValue = 4.0f, float verticalFovValue = 3.0f, float frontFrustrumValue = 0.1f, float backFrustrumValue = 100.0f) {
		m_cameraObjectVec.push_back(new CameraObject(modelVec, horizontalAngle, verticalAngle, fov, speed, mouseSpeed,
			horizontalFovValue, verticalFovValue, frontFrustrumValue, backFrustrumValue));
	}

	void m_changeCameraIndex(int idx);
	void m_controlProgress();
	float m_calcDeltaTime();
	void m_modifyCurCameraWithInput();
	void m_inputProgress();
	glm::mat4 m_getCurCameraProjectionMatrix();
	glm::mat4 m_getCurCameraModelMatrix();
	glm::mat4 m_getCurCameraViewMatrix();
	glm::mat4 m_getCurCameraMVPMatrix();

	virtual ~Control() {
		for (size_t i = 0; i < m_cameraObjectVec.size(); i++) {
			delete m_cameraObjectVec[i];
		}
	}

};
