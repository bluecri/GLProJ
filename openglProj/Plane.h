#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OBBStruct.h>
#include <DDOWithCollision.h>
#include <DmgPrintUI.h>
#include <PlaneDeltaParam.h>
#include <string>
class Plane {
public:
	int m_hp;
	float m_speed;
	TextManager* m_textManagerPtr;
	DDOWithCollision * m_ddoWithCollision;
	PlaneDeltaParam m_planeDeltaParam;

	Plane(int hp, int speed, TextManager* textManagerPtr, DDOWithCollision * ddoWithCollision, PlaneDeltaParam planeDeltaParam) : m_hp(hp), m_speed(speed),m_textManagerPtr(textManagerPtr), m_ddoWithCollision(ddoWithCollision), m_planeDeltaParam(planeDeltaParam){

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
	virtual void update(float deltaTime) {
		//input progress
		move(deltaTime);
	}

	virtual void move(float deltaTime) {
		//do move
		glm::mat4 yawMat = glm::mat4(1.0f);
		glm::mat4 pitchMat = glm::mat4(1.0f);
		glm::mat4 rollMat = glm::mat4(1.0f);

		yawMat[0][0] = glm::cos(yaw);
		yawMat[1][0] = -glm::sin(yaw);
		//yawMat[2][0] = glm::cos(yaw);
		yawMat[0][1] = glm::sin(yaw);
		yawMat[1][1] = glm::cos(yaw);
		//yawMat[2][1] = glm::cos(yaw);
		//yawMat[0][2] = glm::cos(yaw);
		//yawMat[1][2] = glm::cos(yaw);
		//yawMat[2][2] = glm::cos(yaw);

		pitchMat[0][0] = glm::cos(pitch);
		//pitchMat[1][0] = glm::sin(yaw);
		pitchMat[2][0] = glm::sin(pitch);
		//pitchMat[0][1] = glm::sin(yaw);
		//pitchMat[1][1] = glm::cos(yaw);
		//pitchMat[2][1] = glm::cos(yaw);
		pitchMat[0][2] = -glm::sin(pitch);
		//pitchMat[1][2] = glm::cos(yaw);
		pitchMat[2][2] = glm::cos(pitch);

		//rollMat[0][0] = glm::cos(roll);
		//rollMat[1][0] = glm::sin(roll);
		//rollMat[2][0] = glm::sin(roll);
		//rollMat[0][1] = glm::sin(roll);
		rollMat[1][1] = glm::cos(roll);
		rollMat[2][1] = -glm::sin(roll);
		//rollMat[0][2] = glm::sin(roll);
		rollMat[1][2] = glm::sin(roll);
		rollMat[2][2] = glm::cos(roll);

		glm::mat4 updateRot = yawMat * pitchMat*rollMat;
		glm::vec3 moveDirection(updateRot * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * m_speed * deltaTime);
		m_ddoWithCollision->accModelMatrix(moveDirection);
		//do rotate
		m_ddoWithCollision->accRotationMatrix(yawMat*pitchMat*rollMat);
	}

	virtual void shot() {

	}
	
	
	virtual void destoryPlane() {
		
	}

};