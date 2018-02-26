#pragma once
#pragma once
#include <Plane.h>
#include <src/glMain/TextManager.h>
#include <src\glMain\Control.h>
#include <PlaneDeltaParam.h>
#include <PlayerPlane.h>

class EnemyPlane : public Plane {
public:
	PlayerPlane * m_playerPlanePtr;	//for AI
	Control * m_control;
	float shotDelayTime = 0.3f;
	float lastShotTime = 0.0f;
	ALSource * alLaserSource;
	ALSource * alHitSource;
	ALSource * alExplosionSource;

	float collisionParticleTime = 0.0f;

	bool isWillBeDeleted = false;

	glm::vec3 boostPosition = glm::vec3(-0.05f, 0.0f, -0.1f);

	EnemyPlane(Control * control, PlayerPlane * playerPlanePtr, int hp, CollisionProcessInfo* cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		: Plane(hp, cpi, speed, maxSpeed, acc, textManagerPtr, ddoWithCollision, planeDeltaParam), m_playerPlanePtr(playerPlanePtr), m_control(control) {
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

	virtual void collisionOccur(CollisionProcessInfo * anotherCpi) override {
		//collision process with anotherCpi
		printf("collision occur enemy\n");
		alHitSource->sourcePlay();
		m_hp -= anotherCpi->m_dmg;
		collisionParticleTime = 1.0f;
		damagedParticleSource->frameVsParticle = 2;
	}


	virtual void update(float deltaTime) override {
		//input progress

		/*
		if (m_control->iskeyUp[Control::LEFT]) {
			m_planeDeltaParam.addYaw(deltaTime * m_planeDeltaParam.yawDelta);
		}
		if (m_control->iskeyUp[Control::RIGHT]) {
			m_planeDeltaParam.addYaw(deltaTime * -m_planeDeltaParam.yawDelta);
		}
		m_planeDeltaParam.addPitch(deltaTime * m_planeDeltaParam.pitchDelta * m_control->m_mouseDeltaYPos);
		m_planeDeltaParam.addRoll(deltaTime * m_planeDeltaParam.rollDelta *m_control->m_mouseDeltaXPos);
		*/
		if(!isWillBeDeleted){
			move(deltaTime);

			alLaserSource->modifyPos(m_ddoWithCollision->modelVec + glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 0.3f, 0.0f)));

			//collisionParticle end
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

	virtual void move(float deltaTime) override {
		Plane::move(deltaTime);
	}

	virtual void shot() override {
		if (m_control->s_curTime - lastShotTime > shotDelayTime) {
			alLaserSource->sourcePlay();
			lastShotTime = m_control->s_curTime;
		}
		Plane::shot();
	}


	virtual void destoryPlane() override {
		Plane::destoryPlane();
	}

	virtual ~EnemyPlane() {
		delete alLaserSource;
	}

};