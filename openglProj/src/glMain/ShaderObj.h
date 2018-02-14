#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <common/shader.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class ShaderObj {

	public:
		GLuint m_shaderID;

		GLuint m_MVPMatrixID;
		GLuint m_cameraViewMatrixID;
		GLuint m_modelMatrixID;
		GLuint m_depthBiasID;
		GLuint m_shadowMapID;
		GLuint m_lightInvDirID;

		GLuint depthMatrixID;
		
		ShaderObj(const char * vertexShader, const char * fragmentShader) {
			m_shaderID = LoadShaders(vertexShader, fragmentShader);

			m_MVPMatrixID = -1;
			m_cameraViewMatrixID = -1;
			m_modelMatrixID = -1;
			m_depthBiasID = -1;
			m_shadowMapID = -1;
			m_lightInvDirID = -1;

			depthMatrixID = -1;
		}

		GLuint getShaderID() {
			return m_shaderID;
		}
		
};