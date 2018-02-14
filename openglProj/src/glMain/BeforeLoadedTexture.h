#pragma once
#include <vector>

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Vertex.h"
#include "src/game/DrawableObject.h"
#include "src/glMain/objectLoader.h"
#include "src/glMain/vboindexer.h"
#include "src/glMain/texture.h"

#include <cstring>
#include <string>

/**
*
* @brief Texture class
* @details Texture load and has texture ID. Also has custon texure name.
* @author bluecri
* @date 2018-02-12
* @version 0.0.1
*
*/
class BeforeLoadedTexture {
private:
public:
	GLuint m_p_TextureID; ///< texture ID
	std::string m_str_name; ///< custon texture name

	bool m_bCanUse = false;	///< correctly Load check

	char * m_textureFileName; ///< file path


	/**
	*   @brief BeforeLoadedTexture constructor
	*	@details Make texture with file path and return textureID
	*	@param std::string vtName texture name
	*	@param const char* textureFileName texture file path
	*	@return GLuint TextureID
	*/
	BeforeLoadedTexture(std::string vtName, const char* textureFileName) : m_str_name(vtName) {
		if (makeTextureVec(textureFileName)) {
			m_bCanUse = true;
		}
	}


	/**
	*   @brief BeforeLoadedTexture constructor
	*	@details Make texture with file path and return textureID
	*	@param const char* textureFileName texture file path
	*	@return GLuint TextureID
	*/
	bool makeTextureVec(const char* textureFileName) {
		//save file name
		m_textureFileName = new char[strlen(textureFileName) + 1];
		strcpy(m_textureFileName, textureFileName);

		try {
			m_p_TextureID = loadDDS(m_textureFileName);
		}
		catch (int err) {
			return false;
		}
		return true;
	}

	/**
	*   @brief BeforeLoadedTexture destructor
	*	@details delete all texture ID and dealloc string.
	*/
	virtual ~BeforeLoadedTexture() {
		glDeleteTextures(1, &m_p_TextureID);
		delete[] m_textureFileName;
	}
};