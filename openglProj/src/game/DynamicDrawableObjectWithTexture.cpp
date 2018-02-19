#include "src/game/DynamicDrawableObjectWithTexture.h"

glm::mat4 DynamicDrawableObjectWithTexture::getModelMatrix()
{
	return m_modelMatrix;
}

glm::mat4 DynamicDrawableObjectWithTexture::getRotationMatrix()
{
	if (needUpdateRotateMatrix) {
		m_rotateMatrix = glm::mat4_cast(m_quat);
		needUpdateRotateMatrix = false;
	}
	return m_rotateMatrix;
}

glm::mat4 DynamicDrawableObjectWithTexture::getScaleMatrix()
{
	return m_scaleMatrix;
}

glm::quat DynamicDrawableObjectWithTexture::getQuarternion()
{
	if (needUpdateQuat) {
		m_quat = glm::quat_cast(m_rotateMatrix);
		needUpdateQuat = false;
	}
	return m_quat;
}

void DynamicDrawableObjectWithTexture::setModelMatrix(glm::mat4 modelMat)
{
	(*this).modelVec = modelMat[3];
	m_modelMatrix = modelMat;
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::setRotationMatrix(glm::mat4 rotateMat)
{
	m_rotateMatrix = rotateMat;
	needUpdateQuat = true;
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::setScaleMatrix(glm::mat4 scaleMat)
{
	m_scaleMatrix = scaleMat;
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::setModelMatrix(glm::vec3 modelVec)
{
	(*this).modelVec = modelVec;
	m_modelMatrix = glm::translate(glm::mat4(), modelVec);
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::setRotationMatrix(glm::vec3 rotateAngleVec)
{
	m_quat = glm::quat(rotateAngleVec);	//Euler angle to quat
	m_rotateMatrix = glm::toMat4(m_quat);
}

void DynamicDrawableObjectWithTexture::setScaleMatrix(glm::vec3 scaleVec)
{
	m_scaleMatrix = glm::scale(glm::mat4(), scaleVec);
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::setRotationMatrix(glm::quat quat)
{
	m_quat = quat;	//Euler angle to quat
	m_rotateMatrix = glm::toMat4(m_quat);
}

void DynamicDrawableObjectWithTexture::accModelMatrix(glm::mat4 modelMat)
{
	(*this).modelVec += glm::vec3(modelMat[3]);
	m_modelMatrix[3][0] += modelMat[3][0];
	m_modelMatrix[3][1] += modelMat[3][1];
	m_modelMatrix[3][2] += modelMat[3][2];
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::accRotationMatrix(glm::mat4 rotateMat)
{
	m_rotateMatrix = rotateMat * m_rotateMatrix;
	needUpdateQuat = true;
}

void DynamicDrawableObjectWithTexture::accScaleMatrix(glm::mat4 scaleMat)
{
	m_scaleMatrix[0][0] *= scaleMat[0][0];
	m_scaleMatrix[1][1] *= scaleMat[1][1];
	m_scaleMatrix[2][2] *= scaleMat[2][2];
}

void DynamicDrawableObjectWithTexture::accModelMatrix(glm::vec3 modelVec)
{
	(*this).modelVec += modelVec;
	m_modelMatrix = glm::translate(m_modelMatrix, modelVec);
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::accRotationMatrix(float degree, glm::vec3 rotateAxis)
{
	m_rotateMatrix = glm::rotate(m_rotateMatrix, glm::radians(degree), rotateAxis);
	needUpdateQuat = true;
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::accScaleMatrix(glm::vec3 scaleVec)
{
	m_scaleMatrix = glm::scale(m_scaleMatrix, scaleVec);
	needUpdateRetMatrix = true;
}

void DynamicDrawableObjectWithTexture::accRotationMatrix(glm::quat quat)
{
	m_quat = quat * m_quat;
	needUpdateRetMatrix = true;
}


glm::mat4 DynamicDrawableObjectWithTexture::getRetMatrix()
{
	if (needUpdateRotateMatrix) {
		m_rotateMatrix = glm::mat4_cast(m_quat);
		needUpdateRotateMatrix = false;
	}
	if (needUpdateQuat) {
		m_quat = glm::quat_cast(m_rotateMatrix);
		needUpdateQuat = false;
	}

	if (needUpdateRetMatrix) {
		m_retMatrix = m_modelMatrix * m_rotateMatrix * m_scaleMatrix;
		needUpdateRetMatrix = false;
	}
	return m_retMatrix;;
}
