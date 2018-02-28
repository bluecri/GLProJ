#pragma once
#include "./ShaderObj.h"

class ShaderSkybox : public ShaderObj{	
	public:
		ShaderSkybox(const char * vertexShader, const char * fragmentShader) : ShaderObj(vertexShader, fragmentShader) {
			if (m_shaderID != -1) {
				m_textureID = glGetUniformLocation(m_shaderID, "myTextureSampler");
				m_MVPMatrixID = glGetUniformLocation(m_shaderID, "MVP");
				m_cameraViewMatrixID = glGetUniformLocation(m_shaderID, "V");
				m_modelMatrixID = glGetUniformLocation(m_shaderID, "M");
			}
		}
};