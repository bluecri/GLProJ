#pragma once
#include <Plane.h>
#include <src/glMain/TextManager.h>
#include <src\glMain\Control.h>
#include <PlaneDeltaParam.h>
#include <ALListener.h>
#include <ALSource.h>
#include <CollisionProcessInfo.h>
#include <ManagerOfManagerObservee.h>
#include <ParticleInfo.h>

class PlayerPlane : public Plane {
public:
	Control * m_control;
	CameraObject * m_mainCamera;
	float shotDelayTime = 0.3f;
	float lastShotTime = 0.0f;

	ALListener * alListener;

	ALSource * alLaserSource;
	ALSource * alHitSource;
	ALSource * alExplosionSource;

	float collisionParticleTime = 0.0f;
	glm::vec3 boostPosition = glm::vec3(-0.05f, 0.0f, -0.1f);

	bool isWillBeDeleted = false;
	
	enum EnumCameraMode
	{
		FPS = 0, TPS, SMALL, BACK, CAMERA_MODE_LEN
	};
	int cameraMode = SMALL;

	float cameraEyeYDelta[CAMERA_MODE_LEN] = { 0.0f, 0.5f, 1.5f, 1.0f };
	float cameraEyeZDelta[CAMERA_MODE_LEN] = { 4.5f,-0.8f, -4.5f, 4.0f };
	float cameraLookDelta[CAMERA_MODE_LEN] = { 10.0f, 0.8f, 1.5f, -2.0f };


	PlayerPlane(Control* control, CameraObject * mainCamera, int hp, CollisionProcessInfo * cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		:  Plane(hp, cpi, speed, maxSpeed, acc, textManagerPtr, ddoWithCollision, planeDeltaParam), m_control(control), m_mainCamera(mainCamera){
		alListener = new ALListener();
		alListener->modifyPos(3.0f, 0.0f, 0.0f);
		alLaserSource = new ALSource();
		alHitSource = new ALSource(1.0f, 0.3f);
		alExplosionSource = new ALSource();
		
	}


	void bindLaserSourceToALSound(ALSound *alSound) {
		alLaserSource->bindSourceToALSound(alSound);
	}
	void bindHitSourceToALSound(ALSound *alSound) {
		alHitSource->bindSourceToALSound(alSound);
	}
	void bindExplosionSourceToALSound(ALSound *alSound) {
		alExplosionSource->bindSourceToALSound(alSound);
	}

	virtual void collisionOccur(CollisionProcessInfo * anotherCpi) override{
		//collision process with anotherCpi
		printf("collision occur player\n");
		alHitSource->sourcePlay();
		m_hp -= anotherCpi->m_dmg;
	}

	virtual void update(float deltaTime) override {
		if (!isWillBeDeleted) {
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

			//camera update
			cameraUpdate();

			m_planeDeltaParam.yaw = 0;
			m_planeDeltaParam.pitch = 0;
			m_planeDeltaParam.roll = 0;

			//listener position/orientation update
			alListener->modifyPos(m_ddoWithCollision->modelVec);
			alListener->modifyOrientationWithRotationMatrix(m_ddoWithCollision->getRotationMatrix());
			alLaserSource->modifyPos(m_ddoWithCollision->modelVec + glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 0.3f, 0.0f)));

			//collisionParticle progress end
			if (collisionParticleTime >= 0.0f) {
				damagedParticleSource->startPos = m_ddoWithCollision->modelVec;
				collisionParticleTime -= deltaTime;
				if (collisionParticleTime < 0.0f) {
					collisionParticleTime = 0.0f;
					damagedParticleSource->frameVsParticle = 0;
				}
			}

			//boost Particle Progress	
			backParticleSource->frameVsParticle = 22 - (int)(m_speed / m_maxSpeed * 100 * 20);	//21 - (0~20)
			backParticleSource->startPos = m_ddoWithCollision->modelVec + boostPosition;

			if (m_hp <= 0) {
				isCollisionCheck = false;
				m_ddoWithCollision->isDrawableObjDraw = false;
				alExplosionSource->sourcePlay();
				isWillBeDeleted = true;
			}
		}
		else {
			//collisionParticle progress end
			if (collisionParticleTime >= 0.0f) {
				damagedParticleSource->startPos = m_ddoWithCollision->modelVec;
				collisionParticleTime -= deltaTime;
				if (collisionParticleTime < 0.0f) {
					collisionParticleTime = 0.0f;
					damagedParticleSource->frameVsParticle = 0;
				}
			}

			if (damagedParticleSource->frameVsParticle == 0) {
				backParticleSource->frameVsParticle = 0;
				ALint sourceState;
				alGetSourcei(alExplosionSource->m_sourceID, AL_SOURCE_STATE, &sourceState);
				if (sourceState != AL_PLAYING) {
					isCollisionObjDelete = true;
				}
			}
		}
		
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
			alLaserSource->sourcePlay();
			observee->notifyBulletFire(m_ddoWithCollision->modelVec + glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.5f, 0.0f)), m_ddoWithCollision->getRotationMatrix());
			lastShotTime = m_control->s_curTime;
		}
		
		Plane::shot();
	}

	
	

	virtual void destoryPlane() override {
		Plane::destoryPlane();
	}

	virtual ~PlayerPlane() {
		delete alLaserSource;
	}

	
};