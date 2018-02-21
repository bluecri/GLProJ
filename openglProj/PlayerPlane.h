#pragma once
#include <Plane.h>
#include <src/glMain/TextManager.h>
#include <src\glMain\Control.h>
#include <PlaneDeltaParam.h>
#include <CollisionProcessInfo.h>

class PlayerPlane : public Plane {
public:
	Control * m_control;
	CameraObject * m_mainCamera;
	float shotDelayTime = 0.3f;
	float lastShotTime = 0.0f;


	enum EnumCameraMode
	{
		FPS = 0, TPS, SMALL, BACK, CAMERA_MODE_LEN
	};
	int cameraMode = SMALL;

	float cameraEyeYDelta[CAMERA_MODE_LEN] = { 0.0f, 0.5f, 1.5f, 1.0f };
	float cameraEyeZDelta[CAMERA_MODE_LEN] = { 4.5f,-0.8f, -4.5f, 4.0f };
	float cameraLookDelta[CAMERA_MODE_LEN] = { 10.0f, 0.8f, 1.5f, -2.0f };


	PlayerPlane(Control* control, CameraObject * mainCamera, int hp, CollisionProcessInfo * cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, PortAudioClass* portAudioManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		:  Plane(hp, cpi, speed, maxSpeed, acc, textManagerPtr, portAudioManagerPtr, ddoWithCollision, planeDeltaParam), m_control(control), m_mainCamera(mainCamera){
	}

	virtual void collisionOccur(CollisionProcessInfo * anotherCpi) override{
		//collision process with anotherCpi
		printf("collision!\n");
	}

	virtual void update(float deltaTime) override {
		//input progress
		if (m_control->iskeyUp[Control::UP]) {
			m_speed += (deltaTime * m_acc);
			if (m_speed > m_maxSpeed) {
				m_speed = m_maxSpeed;
			}
		}
		if (m_control->iskeyUp[Control::DOWN]) {
			m_speed -= (deltaTime * m_acc);
			if (m_speed < -0.001) {
				m_speed = 0;
			}
		}

		if (m_control->iskeyUp[Control::LEFT]) {
			//m_planeDeltaParam.addYaw(deltaTime * -m_planeDeltaParam.yawDelta);
			m_planeDeltaParam.addPitch(deltaTime * -m_planeDeltaParam.pitchDelta);
		}
		if (m_control->iskeyUp[Control::RIGHT]) {
			//m_planeDeltaParam.addYaw(deltaTime * m_planeDeltaParam.yawDelta);
			m_planeDeltaParam.addPitch(deltaTime * m_planeDeltaParam.pitchDelta);
		}

		if (m_control->iskeyUp[Control::ONE]) {
			cameraMode = FPS;
		}
		if (m_control->iskeyUp[Control::TWO]) {
			cameraMode = TPS;
		}
		if (m_control->iskeyUp[Control::THREE]) {
			cameraMode = SMALL;
		}
		if (m_control->iskeyUp[Control::FOUR]) {
			cameraMode = BACK;
		}
		if (m_control->iskeyUp[Control::SPACE]) {
			//printf("shot"); fflush(stdout);
			shot();
		}

		m_planeDeltaParam.addRoll(deltaTime * m_planeDeltaParam.rollDelta * m_control->m_mouseDeltaXPos);
		m_planeDeltaParam.addYaw(deltaTime * m_planeDeltaParam.yawDelta *m_control->m_mouseDeltaYPos);

		m_planeDeltaParam.reBackAllParam(deltaTime);
		move(deltaTime);

		cameraUpdate();

		m_planeDeltaParam.yaw = 0;
		m_planeDeltaParam.pitch = 0;
		m_planeDeltaParam.roll = 0;
	}
	void cameraUpdate() {
		//m_ddoWithCollision
		glm::vec3 modelZAxis = m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		glm::vec3 modelYAxis = m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

		switch (cameraMode) {
		case FPS:
			//BUG
			m_mainCamera->modelVec = m_ddoWithCollision->modelVec + modelZAxis * cameraEyeZDelta[cameraMode];
			//m_mainCamera->updateViewMatrixWithParam(m_mainCamera->modelVec, m_ddoWithCollision->modelVec + modelZAxis * cameraLookDelta[cameraMode], glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			m_mainCamera->updateViewMatrixWithParam(m_mainCamera->modelVec, m_ddoWithCollision->modelVec + modelZAxis * cameraLookDelta[cameraMode], glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
			break;
		case TPS:
			m_mainCamera->modelVec = m_ddoWithCollision->modelVec + modelYAxis * cameraEyeYDelta[cameraMode] + modelZAxis * cameraEyeZDelta[cameraMode];
			m_mainCamera->updateViewMatrixWithParam(m_mainCamera->modelVec, m_ddoWithCollision->modelVec + modelZAxis * cameraLookDelta[cameraMode], glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
			//m_mainCamera->updateViewMatrixWithParam(m_mainCamera->modelVec, m_ddoWithCollision->modelVec + modelZAxis * cameraLookDelta[cameraMode], glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
			break;
		case SMALL:
			m_mainCamera->modelVec = m_ddoWithCollision->modelVec + modelYAxis * cameraEyeYDelta[cameraMode] + modelZAxis * cameraEyeZDelta[cameraMode];
			m_mainCamera->updateViewMatrixWithParam(m_mainCamera->modelVec, m_ddoWithCollision->modelVec + modelZAxis * cameraLookDelta[cameraMode], glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
			break;
		case BACK:
			m_mainCamera->modelVec = m_ddoWithCollision->modelVec + modelYAxis * cameraEyeYDelta[cameraMode] + modelZAxis * cameraEyeZDelta[cameraMode];
			m_mainCamera->updateViewMatrixWithParam(m_mainCamera->modelVec, m_ddoWithCollision->modelVec + modelZAxis * cameraLookDelta[cameraMode], glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
			break;
		default:
			break;
		}
	}

	virtual void move(float deltaTime) override {
		Plane::move(deltaTime);
	}

	virtual void shot() override {
		if (m_control->s_curTime - lastShotTime > shotDelayTime) {
			m_portAudioManagerPtr->playSound("laserSound");
			lastShotTime = m_control->s_curTime;
		}
		
		Plane::shot();
	}


	virtual void destoryPlane() override {
		Plane::destoryPlane();
	}

	
};