#pragma once
#pragma once
#pragma once
#include <Plane.h>
#include <src/glMain/TextManager.h>
#include <src\glMain\Control.h>
#include <PlaneDeltaParam.h>
#include <PlayerPlane.h>

class LaserBullet : public Plane {
public:
	PlayerPlane * m_playerPlanePtr;	//for AI
	Control * m_control;
	
	LaserBullet(Control * control, PlayerPlane * playerPlanePtr, int hp, CollisionProcessInfo* cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		: Plane(hp, cpi, speed, maxSpeed, acc, textManagerPtr, ddoWithCollision, planeDeltaParam), m_playerPlanePtr(playerPlanePtr), m_control(control) {

	}

	virtual void collisionOccur(CollisionProcessInfo * anotherCpi) override {
		//collision process with anotherCpi
		isCollisionObjDelete = true;
	}

	virtual void update(float deltaTime) override {
		move(deltaTime);
		checkOutOfBox(50.0f);
	}

	virtual void move(float deltaTime) override {
		Plane::move(deltaTime);
	}

	void checkOutOfBox(float border) {
		glm::vec3 distVec =  m_playerPlanePtr->m_ddoWithCollision->modelVec - m_ddoWithCollision->modelVec;
		if (glm::abs(distVec.x) > border || glm::abs(distVec.y) > border || glm::abs(distVec.z) > border) {
			isCollisionObjDelete = true;
		}
	}

	virtual void destoryPlane() override {
		Plane::destoryPlane();
	}

	virtual ~LaserBullet() {

	}

};