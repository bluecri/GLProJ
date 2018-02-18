#pragma once
#include <Plane.h>
#include <src\glMain\Control.h>

class PlayerPlane : public Plane {
public:
	Control * m_control;

	PlayerPlane(Control* control, int hp, int speed, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam) :  Plane(hp, speed, textManagerPtr, ddoWithCollision, planeDeltaParam), m_control(control){

	}
	virtual void update(float deltaTime) override {
		//input progress
		
		if (m_control->LEFT) {
			//m_planeDeltaParam. += yaw * deltaTime * yawDelta
			//m_planeDeltaParam.
		}

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

	
};