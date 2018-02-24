#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OBBStruct.h>
#include <DDOWithCollision.h>
#include <DmgPrintUI.h>
#include <PlaneDeltaParam.h>
#include <HasCollisionObj.h>
#include <string>

#include <ManagerOfManagerObservee.h>

class Plane : public HasCollisionObj{
public:
	int m_hp;
	float m_speed, m_maxSpeed, m_acc;
	TextManager* m_textManagerPtr;

	ManagerOfManagerObservee* observee;

	PlaneDeltaParam m_planeDeltaParam;

	Plane(int hp, CollisionProcessInfo * cpi, float speed, float maxSpeed, float acc, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam)
		: m_hp(hp), m_speed(speed), m_textManagerPtr(textManagerPtr), HasCollisionObj(ddoWithCollision, cpi), m_planeDeltaParam(planeDeltaParam), m_acc(acc), m_maxSpeed(maxSpeed){
		observee = new ManagerOfManagerObservee();
	}

	virtual void hpMinus(int dmg) {
		m_hp -= dmg;
		m_textManagerPtr->addPrintTextListWithRaycast(0, std::to_string(dmg).c_str(), m_ddoWithCollision->modelVec, 30, 3.0f);
		if (m_hp < 0) {
			m_hp = 0;
		}
		if (m_hp == 0) {
			destoryPlane();
		}
	}
	virtual void update(float deltaTime) override {
		//input progress
		move(deltaTime);
	}

	virtual void move(float deltaTime) {
		//get axis
		glm::vec3 yawAxis = m_ddoWithCollision->getRotationMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 pitchAxis = m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		glm::vec3 rollAxis = m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

		//do move
		glm::mat4 yawMat = getRotationMatrixWithAxis(yawAxis, m_planeDeltaParam.yaw);
		glm::mat4 pitchMat = getRotationMatrixWithAxis(pitchAxis, m_planeDeltaParam.pitch);
		glm::mat4 rollMat = getRotationMatrixWithAxis(rollAxis, m_planeDeltaParam.roll);

		glm::mat4 updateRot = yawMat * pitchMat* rollMat;
		
		glm::vec3 moveDirection(m_ddoWithCollision->getRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * m_speed * deltaTime);
		
		//do move position
		m_ddoWithCollision->accModelMatrix(moveDirection);
		//do rotate
		m_ddoWithCollision->accRotationMatrix(updateRot);
	}

	virtual void shot() {

	}
	
	
	virtual void destoryPlane() {
		
	}

	glm::mat4 getRotationMatrixWithAxis(glm::vec3 axis, float angle)
	{
		axis = normalize(axis);
		float s = sin(angle);
		float c = cos(angle);
		float oc = 1.0 - c;

		return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
			oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
			oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}



	void registerObserver(ManagerOfManagerObserver *observer) {
		observee->registerObserver(observer);
	}
};

/*
yawMat[0][0] = glm::cos(m_planeDeltaParam.yaw);
yawMat[1][0] = -glm::sin(m_planeDeltaParam.yaw);
//yawMat[2][0] = glm::cos(yaw);
yawMat[0][1] = glm::sin(m_planeDeltaParam.yaw);
yawMat[1][1] = glm::cos(m_planeDeltaParam.yaw);
//yawMat[2][1] = glm::cos(yaw);
//yawMat[0][2] = glm::cos(yaw);
//yawMat[1][2] = glm::cos(yaw);
//yawMat[2][2] = glm::cos(yaw);

pitchMat[0][0] = glm::cos(m_planeDeltaParam.pitch);
//pitchMat[1][0] = glm::sin(yaw);
pitchMat[2][0] = glm::sin(m_planeDeltaParam.pitch);
//pitchMat[0][1] = glm::sin(yaw);
//pitchMat[1][1] = glm::cos(yaw);
//pitchMat[2][1] = glm::cos(yaw);
pitchMat[0][2] = -glm::sin(m_planeDeltaParam.pitch);
//pitchMat[1][2] = glm::cos(yaw);
pitchMat[2][2] = glm::cos(m_planeDeltaParam.pitch);

//rollMat[0][0] = glm::cos(roll);
//rollMat[1][0] = glm::sin(roll);
//rollMat[2][0] = glm::sin(roll);
//rollMat[0][1] = glm::sin(roll);
rollMat[1][1] = glm::cos(m_planeDeltaParam.roll);
rollMat[2][1] = -glm::sin(m_planeDeltaParam.roll);
//rollMat[0][2] = glm::sin(roll);
rollMat[1][2] = glm::sin(m_planeDeltaParam.roll);
rollMat[2][2] = glm::cos(m_planeDeltaParam.roll);
*/