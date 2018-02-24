#pragma once
#include "src/game/DrawableObject.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

class DrawableObjectWithTexture : public DrawableObject{
public:
	bool m_isStatic;
	int m_vertexIdx;	//used by window.h
	int m_textureIdx;	//used by window.h
	int m_idx;			//used by window.h
	
	DrawableObjectWithTexture(std::string name, int arrIdx, int textureIdx, int vertexIdx, glm::vec3 modelVec, bool isStatic) : DrawableObject(name, modelVec), m_idx(arrIdx), m_isStatic(isStatic), m_vertexIdx(vertexIdx), m_textureIdx(textureIdx){

	}

	virtual glm::mat4 getModelMatrix() = 0;
	virtual glm::mat4 getRotationMatrix() = 0;
	virtual glm::mat4 getScaleMatrix() = 0;
	virtual glm::quat getQuarternion() = 0;

	virtual void setModelMatrix(glm::mat4 modelMat) = 0;
	virtual void setRotationMatrix(glm::mat4 rotateMat) = 0;
	virtual void setScaleMatrix(glm::mat4 scaleMat) = 0;

	virtual void setModelMatrix(glm::vec3 modelVec) = 0;
	virtual void setRotationMatrix(glm::vec3 rotateVec) = 0;
	virtual void setScaleMatrix(glm::vec3 scaleVec) = 0;

	virtual void setRotationMatrix(glm::quat quat) = 0;

	virtual void accModelMatrix(glm::mat4 modelMat) = 0;
	virtual void accRotationMatrix(glm::mat4 rotateMat) = 0;
	virtual void accScaleMatrix(glm::mat4 scaleMat) = 0;

	virtual void accModelMatrix(glm::vec3 modelVec) = 0;
	virtual void accRotationMatrix(float degree, glm::vec3 rotateAxis) = 0;
	virtual void accScaleMatrix(glm::vec3 scaleVec) = 0;

	virtual void accRotationMatrix(glm::quat quat) = 0;


	virtual glm::mat4 getRetMatrix() = 0;



	bool isStatic() {
		return m_isStatic;
	}

	virtual ~DrawableObjectWithTexture() {}
};