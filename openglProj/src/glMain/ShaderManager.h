#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <common/shader.hpp>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <src/glMain/ShaderObj.h>
#include <src/glMain/ShaderMain.h>
#include <src/glMain/ShaderShadow.h>
#include <src/glMain/ShaderText.h>


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
	/*
	int m_addTextShader(const char * vertexShader, const char * fragmentShader) {
		ShaderText* tempShaderText = new ShaderText(vertexShader, fragmentShader);
		if (tempShaderText->m_shaderID != -1) {
			m_shaderStorage.push_back(tempShaderText);
			return tempShaderText->m_shaderID;
		}
		else {
			return -1;
		}
	}
	*/

	ShaderObj* getShaderPtrWithEnum(ENUM_SHADER_IDX enum_idx) {
		return m_shaderStorage[enum_idx];
	}
	virtual ~ShaderManager() {

	}
};
