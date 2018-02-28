#pragma once

#include "GL/glew.h"

#include <glm/gtx/transform.hpp>

#include <cstring>

#include "./Vertex.h"
#include "./objectLoader.h"
#include "./vboindexer.h"
#include "./texture.h"


class BeforeLoadedVertexWithTexture {
	private:
	public:
		//GLuint vertexbufferID;
		std::string m_str_name;

		bool m_bCanUse = false;

		char * m_objectFileName;
		
		std::vector<Struct_VertexWithTexture> m_vertexVec;
		std::vector<unsigned short> m_indicesVec;
	
		BeforeLoadedVertexWithTexture(std::string vtName, const char* objectFileName) : m_str_name(vtName){
			if (makeVertexIndiceVec(objectFileName)) {
				m_bCanUse = true;
			}
		}

		bool makeVertexIndiceVec(const char* objectFileName) {
			std::vector<glm::vec3> outVertexVec;
			std::vector<glm::vec2> outUvVec;
			std::vector<glm::vec3> outNormalVec;

			//save file name
			m_objectFileName = new char[strlen(objectFileName) + 1];
			strcpy(m_objectFileName, objectFileName);

			bool res = loadOBJ(m_objectFileName, outVertexVec, outUvVec, outNormalVec);
			if (!res) {
				printf("%s load obj fail\n", m_objectFileName);
				return false;
			}

			indexVBOWithTexture(outVertexVec, outUvVec, outNormalVec, m_indicesVec, m_vertexVec);

			return true;
		}

		int getVertexVecNum() {
			return m_vertexVec.size();
		}

		int getIndiceVecNum() {
			return m_indicesVec.size();
		}

		int getVertexVecSize() {
			return m_vertexVec.size() * sizeof(Struct_VertexWithTexture);
		}

		int getIndiceVecSize() {
			return m_indicesVec.size() * sizeof(unsigned short);
		}

		virtual ~BeforeLoadedVertexWithTexture() {
			delete[] m_objectFileName;
		}
};