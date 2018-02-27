#pragma once
#pragma once
#include <Plane.h>
#include <src/glMain/TextManager.h>
#include <src\glMain\Control.h>
#include <PlaneDeltaParam.h>
#include <PlayerPlane.h>
#include <glm/gtx/rotate_vector.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>

class EnemyPlane : public Plane {
public:
	PlayerPlane * m_playerPlanePtr;	//for AI
	Control * m_control;
	float shotDelayTime = 0.7f;
	float lastShotTime = 0.0f;
	ALSource * alLaserSource;
	ALSource * alHitSource;
	ALSource * alExplosionSource;

	float collisionParticleTime = 0.0f;


	glm::vec3 boostPosition = glm::vec3(-0.05f, 0.0f, -0.1f);

	EnemyPlane(Control * control, PlayerPlane * playerPlanePtr, int hp, CollisionProcessInfo* cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		: Plane(hp, cpi, speed, maxSpeed, acc, textManagerPtr, ddoWithCollision, planeDeltaParam), m_playerPlanePtr(playerPlanePtr), m_control(control) {
		alLaserSource = new ALSource();
		alHitSource = new ALSource(1.0f, 0.3f);
		alExplosionSource = new ALSource();
	}

	float chaseTime = 0.0f;
	float chaseTimeMax = 15.0f;
	float chaseTimeMin = 10.0f;
	float idleTimeMax = 1.0f;
	float idleTimeMin = 0.0f;

	float chaseSpeedMax = 3.5f, chaseSpeedMin = 3.2f, idleSpeedMax=0.5f, idleSpeedMin=2.0f;
	bool isChasing = false;

	float rotationSpeed = 2.0f;

	float shotDistance = 30.0f;
	glm::quat targetQuat;

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
		
		m_textManagerPtr->addPrintTextListWithModelVec(0, std::to_string(anotherCpi->m_dmg).c_str(), m_ddoWithCollision->modelVec, 20, 1.0f);
	}


	virtual void update(float deltaTime) override {
		if(!isWillBeDeleted){
			if (isChasing) {
				chaseTime -= deltaTime;
				if (chaseTime < 0.0f) {
					chaseTime = glm::linearRand(idleTimeMin, idleTimeMax);
					isChasing = false;
				}
				//move direction
				m_speed = glm::linearRand(chaseSpeedMin, chaseSpeedMax);

				glm::vec3 distVec = m_playerPlanePtr->m_ddoWithCollision->modelVec + glm::vec3(m_playerPlanePtr->m_ddoWithCollision->m_rotateMatrix * glm::vec4(0.0f, 0.0f, glm::linearRand(0.0f, m_playerPlanePtr->m_speed), 0.0f)) - m_ddoWithCollision->modelVec;
				distVec *= -1.0f;
				distVec = glm::normalize(distVec);
				

				float x, y, z;
				//x = glm::linearRand(-1.0f, 1.0f);
				//y = glm::linearRand(-1.0f, 1.0f);
				x = 0.0f;
				y = -1.0f;
				z = (distVec[0] * x + distVec[1] * y) / (-distVec[2]);
				
				glm::vec3 upVec = glm::vec3(x, y, z);
				upVec = glm::normalize(upVec);

				targetQuat = glm::toQuat(glm::transpose(glm::lookAt(glm::vec3(0.0f), distVec, upVec)));
				//targetQuat = glm::toQuat(glm::lookAt(glm::vec3(0.0f), glm::normalize(distVec), upVec));
				//m_ddoWithCollision->m_rotateMatrix = glm::inverse(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
				
				//targetQuat = glm::toQuat(glm::lookAt(glm::vec3(0.0f), glm::normalize(distVec), upVec));
				//targetQuat = glm::toQuat(glm::lookAt(m_ddoWithCollision->modelVec, m_playerPlanePtr->m_ddoWithCollision->modelVec, upVec));

				glm::quat curQuat = glm::toQuat(m_ddoWithCollision->m_rotateMatrix);

				targetQuat = glm::slerp(curQuat, targetQuat, deltaTime* rotationSpeed);

				m_ddoWithCollision->m_rotateMatrix = glm::toMat4(targetQuat);
				
				if (glm::distance2(m_playerPlanePtr->m_ddoWithCollision->modelVec, m_ddoWithCollision->modelVec) < shotDistance) {
					shot();
				}
				

			}
			else {
				chaseTime -= deltaTime;
				m_speed = glm::linearRand(idleSpeedMin, idleSpeedMax);
				if (chaseTime < 0.0f) {
					chaseTime = glm::linearRand(chaseTimeMin, chaseTimeMax);
					isChasing = true;
				}
			}
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
		glm::vec3 moveDirection(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * m_speed * deltaTime);
		//do move position
		m_ddoWithCollision->accModelMatrix(moveDirection);
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

	virtual ~EnemyPlane() {
		delete alLaserSource;
	}

};