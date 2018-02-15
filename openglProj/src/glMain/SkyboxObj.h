#pragma once

#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "src/glMain/texture.h"

class SkyboxObj {
	public:
		enum ENUM_TEXTURE_IDX {
			UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK
		};
		GLuint textureID[6];

		SkyboxObj(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path ) {
			textureID[UP] = loadDDS(up_path);
			textureID[DOWN] = loadDDS(down_path);
			textureID[LEFT] = loadDDS(left_path);
			textureID[RIGHT] = loadDDS(right_path);
			textureID[FRONT] = loadDDS(front_path);
			textureID[BACK] = loadDDS(back_path);
		}
		~SkyboxObj() {
			for (int i = 0; i < 6; i++) {
				glDeleteTextures(1, &textureID[i]);
			}
	}
};