#pragma once
#pragma once
#include <Plane.h>
#include <src/glMain/TextManager.h>
#include <src\glMain\Control.h>
#include <PlaneDeltaParam.h>
#include <PlayerPlane.h>

class EnemyPlane : public Plane {
public:
	PlayerPlane * playerPlanePtr;

	ALSource * alLaserSource;

	EnemyPlane(PlayerPlane * playerPlanePtr, int hp, CollisionProcessInfo* cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		: Plane(hp, cpi, speed, maxSpeed, acc, textManagerPtr, ddoWithCollision, planeDeltaParam), playerPlanePtr(playerPlanePtr) {
		alLaserSource = new ALSource();
	}

	void bindLaserSourceToALSound(ALSound *alSound) {
		alLaserSource->bindSourceToALSound(alSound);
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
	}

	virtual void move(float deltaTime) override {
		Plane::move(deltaTime);
	}

	virtual void shot() override {
		Plane::shot();
	}


	virtual void destoryPlane() override {
		Plane::destoryPlane();
	}

	virtual ~EnemyPlane() {
		delete alLaserSource;
	}

};