#pragma once

#include <vector>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "src/glMain/texture.h"
#include "src/glMain/SkyboxObj.h"
class SkyboxObjManager {
	public:
		enum ENUM_SKYBOX_MANAGER {
			UNIV = 0
		};
		std::vector<std::vector<std::string>> loadFilePath = { {"texture/skybox_up", "texture/skybox_down", "texture/skybox_left", "texture/skybox_right", "texture/skybox_front", "texture/skybox_back" } };
		std::vector<SkyboxObj*> skyboxObjStorageVec;

		SkyboxObjManager(){
			for (int i = 0; i < loadFilePath.size(); i++) {
				skyboxObjStorageVec.push_back(new SkyboxObj(loadFilePath[i][0], loadFilePath[i][1], loadFilePath[i][2], loadFilePath[i][3], loadFilePath[i][4], loadFilePath[i][5]));
			}

		};
		~SkyboxObjManager() {
			for (int i = 0; i < skyboxObjStorageVec.size(); i++) {
				delete skyboxObjStorageVec[i];
			}
		}
};