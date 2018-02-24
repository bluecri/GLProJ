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
		alHitSource->sourcePlay();
		m_hp -= anotherCpi->m_dmg;
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

		move(deltaTime);

		alLaserSource->modifyPos(m_ddoWithCollision->modelVec + glm::vec3(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 0.3f, 0.0f)));

		if (m_hp <= 0) {
			isCollisionObjDelete = true;
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