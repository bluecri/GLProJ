#pragma once
#include <string>
#include "src/game/DrawableObjectWithTexture.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class DynamicDrawableObjectWithTexture : public DrawableObjectWithTexture {
public:
	glm::mat4 m_modelMatrix;

	bool needUpdateRotateMatrix;
	glm::mat4 m_rotateMatrix;
	glm::mat4 m_scaleMatrix;

	bool needUpdateRetMatrix;
	glm::mat4 m_retMatrix;

	bool needUpdateQuat;
	glm::quat m_quat;

	DynamicDrawableObjectWithTexture(std::string name, int arrIdx, int textureidx, int vertexIdx, glm::vec3 modelVec, glm::vec3 angleVec, glm::vec3 scaleVec, bool isStatic) : DrawableObjectWithTexture(name, arrIdx, textureidx, vertexIdx, modelVec, isStatic) {
		m_modelMatrix = glm::translate(glm::mat4(), modelVec);

		m_quat = glm::quat(angleVec);	//Euler angle to quat
		m_rotateMatrix = glm::toMat4(m_quat);

		m_scaleMatrix = glm::scale(glm::mat4(), scaleVec);

		m_retMatrix = m_modelMatrix * m_rotateMatrix * m_scaleMatrix;

		needUpdateRotateMatrix = false;
		needUpdateRetMatrix = false;
		needUpdateQuat = false;

	}




	// DrawableObjectWithTexture을(를) 통해 상속됨
	virtual glm::mat4 getModelMatrix() override;

	virtual glm::mat4 getRotationMatrix() override;

	virtual glm::mat4 getScaleMatrix() override;

	virtual glm::quat getQuarternion() override;

	virtual void setModelMatrix(glm::mat4 modelMat) override;

	virtual void setRotationMatrix(glm::mat4 rotateMat) override;

	virtual void setScaleMatrix(glm::mat4 scaleMat) override;

	virtual void setModelMatrix(glm::vec3 modelVec) override;

	virtual void setRotationMatrix(glm::vec3 rotateVec) override;

	virtual void setScaleMatrix(glm::vec3 scaleVec) override;

	virtual void setRotationMatrix(glm::quat quat) override;

	virtual void accModelMatrix(glm::mat4 modelMat) override;

	virtual void accRotationMatrix(glm::mat4 rotateMat) override;

	virtual void accScaleMatrix(glm::mat4 scaleMat) override;

	virtual void accModelMatrix(glm::vec3 modelVec) override;

	virtual void accRotationMatrix(float degree, glm::vec3 rotateAxis) override;

	virtual void accScaleMatrix(glm::vec3 scaleVec) override;

	virtual void accRotationMatrix(glm::quat quat) override;

	virtual glm::mat4 getRetMatrix() override;

};