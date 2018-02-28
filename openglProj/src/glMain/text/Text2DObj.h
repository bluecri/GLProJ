#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Text2DObj {
public:
	GLuint Text2DTextureID;
	GLuint Text2DShaderID;
	GLuint Text2DUniformID;

	Text2DObj(int textureID, GLuint shaderID, GLuint shaderUniformID) {
		Text2DTextureID = textureID;
		Text2DShaderID = shaderID;
		Text2DUniformID = shaderUniformID;
	}

	~Text2DObj() {
	}
};