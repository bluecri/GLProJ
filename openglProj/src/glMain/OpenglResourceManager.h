#pragma once
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include <string>
#include <map>

#include "src/glMain/BeforeLoadedVertexWithTexture.h"
#include "src/glMain/BeforeLoadedTexture.h"
/**
*
* @brief Manage resource class
* @details manage vertex, texture info
* @author bluecri
* @date 2018-02-12
* @version 0.0.1
*
*/
class OpenglResourceManager{
	private:
	public:
		std::vector<BeforeLoadedVertexWithTexture*> m_beforeLoadedVertexWithTextureVec;
		std::map<std::string, int> m_bLVWTVnameToIndexMap;

		std::vector<BeforeLoadedTexture*> m_beforeLoadedTextureVec;
		std::map<std::string, int> m_bLTnameToIndexMap;

		OpenglResourceManager();

		BeforeLoadedVertexWithTexture * getBLVWTWithIndex(int idx);			///< Get BeforeLoaderVertexWithTexture Ptr with index
		BeforeLoadedTexture * getBLTWithIndex(int idx);						///< Get BeforeLoadedTexture Ptr with index
		BeforeLoadedVertexWithTexture * getBLVWTWithName(std::string name);	///< Get BeforeLoaderVertexWithTexture Ptr with name
		BeforeLoadedTexture * getBLTWithName(std::string name);				///< Get BeforeLoadedTexture Ptr with name

		int getBLVWTIndexWithName(std::string name);	///< Get BeforeLoaderVertexWithTexture's index with name
		int getBLTIndexWithName(std::string name);		///< Get BeforeLoadedTexture's index with name

		int getBLVWTLen();		///< Get BeforeLoaderVertexWithTexture vector length
		int getBLTLen();		///< Get BeforeLoadedTexture vector length

		bool addVertexVec(std::string vtName, const char * objectFileName);		//< Add new vertex to BeforeLoaderVertexWithTexture vector
		bool addTextureVec(std::string vtName, const char * textureFileName);	//< Add new texture to BeforeLoadedTexture vector

		int getTotalBLVWTVVertexNum();		//< get total vertex num
		int getTotalBLVWTVVertexSize();		//< get total vertex size (vertex num * size)
		int getTotalBLVWTVIndiceNum();		//< get total texture num
		int getTotalBLVWTVIndiceSize();		//< get total texture size (vertex num * size)

		virtual ~OpenglResourceManager();
};