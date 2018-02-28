#pragma once

#include "./ShaderObj.h"
#include "./ShaderMain.h"
#include "./ShaderShadow.h"
#include "./ShaderText.h"

class ShaderManager{
public:
	std::vector<ShaderObj*> m_shaderStorage;
	enum ENUM_SHADER_IDX
	{
		MAIN = 0, SHADOW = 1, COLISION = 2
	};
	ShaderManager() {
		m_addMainShader("shader/ShadowMapping.vertexshader", "shader/ShadowMapping.fragmentshader");
		m_addShadowShader("shader/DepthRTT.vertexshader", "shader/DepthRTT.fragmentshader");
		m_addMainShader("shader/Simple.vertexshader", "shader/Simple.fragmentshader");
	}
	
	int m_addMainShader(const char * vertexShader, const char * fragmentShader) {
		ShaderMain* tempShaderMain = new ShaderMain(vertexShader, fragmentShader);
		if (tempShaderMain->m_shaderID != -1) {
			m_shaderStorage.push_back(tempShaderMain);
			return tempShaderMain->m_shaderID;
		}
		else {
			return -1;
		}
	}

	int m_addShadowShader(const char * vertexShader, const char * fragmentShader) {
		ShaderShadow* tempShaderShadow = new ShaderShadow(vertexShader, fragmentShader);
		if (tempShaderShadow->m_shaderID != -1) {
			m_shaderStorage.push_back(tempShaderShadow);
			return tempShaderShadow->m_shaderID;
		}
		else {
			return -1;
		}
	}

	ShaderObj* getShaderPtrWithEnum(ENUM_SHADER_IDX enum_idx) {
		return m_shaderStorage[enum_idx];
	}
	virtual ~ShaderManager() {

	}
};
