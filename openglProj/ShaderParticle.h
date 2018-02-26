#pragma once
#include <src/glMain/ShaderObj.h>

class ShaderParticle : public ShaderObj {
public:
	ShaderParticle(const char * vertexShader, const char * fragmentShader) : ShaderObj(vertexShader, fragmentShader) {
		if (m_shaderID != -1) {
			m_textureID = glGetUniformLocation(m_shaderID, "myTextureSampler");
			
			m_cameraRight_worldspace_ID = glGetUniformLocation(m_shaderID, "CameraRight_worldspace");
			m_cameraUp_worldspace_ID = glGetUniformLocation(m_shaderID, "CameraUp_worldspace");
			m_viewProjMatrixID = glGetUniformLocation(m_shaderID, "VP");
			//m_MVPMatrixID = glGetUniformLocation(m_shaderID, "MVP");
			//m_cameraViewMatrixID = glGetUniformLocation(m_shaderID, "V");
			//m_modelMatrixID = glGetUniformLocation(m_shaderID, "M");
		}
	}
};