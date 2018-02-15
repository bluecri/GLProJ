#pragma once
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>

#include <src/glMain/Text2DObj.h>

class TextManager {
public:
	GLuint Text2DVertexBufferID;			///< text buffer
	GLuint Text2DUVBufferID;
	GLuint text2DVertexID;

	std::vector<ShaderText*> shaderTextVec;	//shader list
	std::vector<GLuint> textTextureVec;	//texture list
	std::vector<Text2DObj*> text2DObjPtrVec;	//text2dObj list

	//
	TextManager() {
	};

	void textManagerInit() {
		std::vector<std::pair<std::string, std::string>> text2DShaderListVec;
		text2DShaderListVec.push_back(std::make_pair("shader/TextVertexShader.vertexshader", "shader/TextVertexShader.fragmentshader"));

		std::vector<std::string> textTextureListVec;
		textTextureListVec.push_back("texture/Holstein.DDS");

		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);

		//TEXT buffer vertex bind
		
		glGenVertexArrays(1, &text2DVertexID);
		glBindVertexArray(text2DVertexID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);


		//text shader register
		ShaderText* shaderTextPtr;
		for (int i = 0; i < text2DShaderListVec.size(); i++) {
			shaderTextPtr = new ShaderText(text2DShaderListVec[i].first.c_str(), text2DShaderListVec[i].second.c_str());
			if (shaderTextPtr->m_shaderID != -1) {
				shaderTextVec.push_back(shaderTextPtr);
			}
		}

		//text texture register
		GLuint tempTextureID;
		for (int i = 0; i < textTextureListVec.size(); i++) {
			tempTextureID = loadDDS(textTextureListVec[i].c_str());
			if (tempTextureID != -1) {
				textTextureVec.push_back(tempTextureID);
			}
		}
		
		text2DObjPtrVec.push_back(new Text2DObj(textTextureVec[0], shaderTextVec[0]->m_shaderID, shaderTextVec[0]->m_text2DUniformID));
	}
	/*
	void addText2DObjPtr(Text2DObj* newText) {
		text2DObjPtrVec.push_back(newText);
	}
	*/

	void printText2DWithIndex(int index, const char * text, int x, int y, int size) {
		unsigned int length = strlen(text);

		// Fill buffers
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		for (unsigned int i = 0; i<length; i++) {

			glm::vec2 vertex_up_left = glm::vec2(x + i * size/2, y + size);
			glm::vec2 vertex_up_right = glm::vec2(x + i * size/2 + size, y + size);
			glm::vec2 vertex_down_right = glm::vec2(x + i * size/2 + size, y);
			glm::vec2 vertex_down_left = glm::vec2(x + i * size/2, y);

			vertices.push_back(vertex_up_left);
			vertices.push_back(vertex_down_left);
			vertices.push_back(vertex_up_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_down_left);

			char character = text[i];
			float uv_x = (character % 16) / 16.0f;
			float uv_y = (character / 16) / 16.0f;

			glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
			glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
			glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
			glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));

			UVs.push_back(uv_up_left);
			UVs.push_back(uv_down_left);
			UVs.push_back(uv_up_right);

			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_right);
			UVs.push_back(uv_down_left);
		}

		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

		// Bind shader
		glUseProgram(text2DObjPtrVec[index]->Text2DShaderID);
		glBindVertexArray(text2DVertexID);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, text2DObjPtrVec[index]->Text2DTextureID);
		glUniform1i(text2DObjPtrVec[index]->Text2DUniformID, 4);

		// Draw call
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);
	}


};