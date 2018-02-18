#pragma once
#include <src/glMain/ShaderObj.h>

class ShaderText : public ShaderObj{	
	public:
		ShaderText(const char * vertexShader, const char * fragmentShader) : ShaderObj(vertexShader, fragmentShader) {
			if (m_shaderID != -1) {
				m_text2DUniformID = glGetUniformLocation(m_shaderID, "myTextureSampler");
			}
		}
};