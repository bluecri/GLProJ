#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/glMain/OpenglResourceManager.h"
#include "src/glMain/Vertex.h"

#include <vector>

/**
*
* @brief Manage vertex, texture buffer.
* @details With openglResource Manager, reservation vertex & texture buffer and allocation info to buffer. This class has vertexBufferID(static, dynamic, element), vertexArrayIDs. For performance, use only 1 buffer.
* @author bluecri
* @date 2018-02-12
* @version 0.0.1
*
*/
class BufferManager {
public:
	GLuint m_staticVertexBufferID;		///< static buffer
	GLuint m_dynamicVertexBufferID;		///< dynamic buffer
	GLuint m_elementBufferID;			///< element buffer
	//GLuint staticVertexID;

	OpenglResourceManager* m_openglResourceManagerPtr;	

	std::vector<GLuint> vertexArrayObjectIDVec;	///< vertex array storage vector. (use : glBindVertexArray(vertexArrayVec[k]))

	std::vector<GLsizeiptr> vertexOffset;	///< vertexOffset accumulation storage vector
	std::vector<GLsizeiptr> elementOffset;	///< elementOffset accumulation storage vector
	GLuint vertexbuffer;
	

	BufferManager() {
	}

	void initBuffer(OpenglResourceManager* openglResourceManagerPtr);
	
	virtual ~BufferManager();
};