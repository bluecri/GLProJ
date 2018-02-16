#pragma once

#include <vector>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include "src/glMain/texture.h"
#include "src/glMain/SkyboxObj.h"
#include "src/glMain/Vertex.h"

class SkyboxObjManager {
	public:
		GLuint m_skyboxVertexID;
		GLuint m_skyboxVertexBufferID;


		enum ENUM_SKYBOX_MANAGER {
			UNIV = 0
		};
		std::vector<std::vector<std::string>> loadFilePath = { {"texture/skybox_up.dds", "texture/skybox_down.dds", "texture/skybox_left.dds", "texture/skybox_right.dds", "texture/skybox_front.dds", "texture/skybox_back.dds" } };
		std::vector<SkyboxObj*> skyboxObjStorageVec;

		SkyboxObjManager(){
			for (int i = 0; i < loadFilePath.size(); i++) {
				skyboxObjStorageVec.push_back(new SkyboxObj(loadFilePath[i][0].c_str(), loadFilePath[i][1].c_str(), loadFilePath[i][2].c_str(), loadFilePath[i][3].c_str(), loadFilePath[i][4].c_str(), loadFilePath[i][5].c_str()));
			}
		};
		void bufferInit() {
			std::vector<glm::vec3> skyboxVertices{
				// up down
				glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(50.0f, 50.0f, 50.0f),
				glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
				
				// left right
				glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f),
				glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f),

				// front back
				glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
				glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f)
			};
			std::vector<glm::vec2> skyboxIndices
			{
				glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
			};

			std::vector<glm::vec3> skyboxNormals
			{
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			};

			std::vector<Struct_VertexWithTexture> make_buffer;

			for (int i = 0; i < 24; i++) {
				Struct_VertexWithTexture tempStruct_VertexWithTexture;
				tempStruct_VertexWithTexture.vertice = skyboxVertices[i];
				tempStruct_VertexWithTexture.uv = skyboxIndices[i % 4];
				tempStruct_VertexWithTexture.normal = skyboxNormals[i / 4];
				make_buffer.push_back(tempStruct_VertexWithTexture);
			}

			glGenBuffers(1, &m_skyboxVertexBufferID);

			glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, make_buffer.size() * sizeof(Struct_VertexWithTexture), (void*)&make_buffer[0], GL_DYNAMIC_DRAW);
			

			glGenVertexArrays(1, &m_skyboxVertexID);
			glBindVertexArray(m_skyboxVertexID);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(sizeof(glm::vec3)));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

			glBindVertexArray(0);
		}

		void drawSkyBox() {
			//useProgarm
			glDepthMask(GL_FALSE);
			glBindVertexArray(m_skyboxVertexID);
			for (int i = 0; i < 6; i++) {
				glBindTexture(GL_TEXTURE5, )
					//gluniformtexture to shader
				glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
			}
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}


		~SkyboxObjManager() {
			for (int i = 0; i < skyboxObjStorageVec.size(); i++) {
				delete skyboxObjStorageVec[i];
			}
		}
};