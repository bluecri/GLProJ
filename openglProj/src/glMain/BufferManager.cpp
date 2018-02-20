#include "src/glMain/BufferManager.h"


void BufferManager::initBuffer(OpenglResourceManager* openglResourceManagerPtr) {
	m_openglResourceManagerPtr = openglResourceManagerPtr;
	glGenBuffers(1, &m_staticVertexBufferID);
	glGenBuffers(1, &m_elementBufferID);
	glGenBuffers(1, &m_collisionBufferID);
	glGenBuffers(1, &m_collisionElementID);
	glGenFramebuffers(1, &FramebufferName);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//setup array buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_staticVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_openglResourceManagerPtr->getTotalBLVWTVVertexSize(), 0, GL_STATIC_DRAW);


	vertexArrayObjectIDVec.clear();
	vertexOffset.clear();

	vertexOffset.push_back(0);
	for (int i = 0; i < m_openglResourceManagerPtr->getBLVWTLen(); i++) {
		BeforeLoadedVertexWithTexture * tempBlvwt = m_openglResourceManagerPtr->getBLVWTWithIndex(i);	//obj에 해당하는 vertex&indice Vec
		int vertexVecSize = tempBlvwt->getVertexVecSize();

		glBufferSubData(GL_ARRAY_BUFFER, vertexOffset[i], vertexVecSize, (void*)&(tempBlvwt->m_vertexVec[0]));
		vertexOffset.push_back(vertexOffset[i] + vertexVecSize);	//buffer offset save
	}

	//setup element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_openglResourceManagerPtr->getTotalBLVWTVIndiceSize(), 0, GL_STATIC_DRAW);

	elementOffset.push_back(0);
	for (int i = 0; i < m_openglResourceManagerPtr->getBLVWTLen(); i++) {
		BeforeLoadedVertexWithTexture * tempBlvwt = m_openglResourceManagerPtr->getBLVWTWithIndex(i);	//obj에 해당하는 vertex&indice Vec
		int vertexVecSize = tempBlvwt->getIndiceVecSize();

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, elementOffset[i], vertexVecSize, (void*)&(tempBlvwt->m_indicesVec[0]));
		elementOffset.push_back(elementOffset[i] + vertexVecSize);
	}

	//setting vertex (vertexArrayObjectIDVec)
	for (int i = 0; i < m_openglResourceManagerPtr->getBLVWTLen(); i++) {
		GLuint tempVertexID;
		glGenVertexArrays(1, &tempVertexID);
		vertexArrayObjectIDVec.push_back(tempVertexID);

		glBindVertexArray(tempVertexID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_staticVertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(0 + vertexOffset[i]));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(sizeof(float) * 3 + vertexOffset[i]));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(sizeof(float) * 5 + vertexOffset[i]));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);

		glBindVertexArray(0);	//unbind!
	}

	//collision vertex & element buffer
	glGenVertexArrays(1, &m_collisionVertexID);
	glBindVertexArray(m_collisionVertexID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_collisionBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_collisionElementID);
	glBindVertexArray(0);	//unbind!


	//depth frame buffer for shadow
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	// No color output in the bound framebuffer, only depth.

	GLenum DrawBuffers[1] = { GL_DEPTH_ATTACHMENT };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

BufferManager::~BufferManager() {
	glDeleteBuffers(1, &m_staticVertexBufferID);
	glDeleteBuffers(1, &m_dynamicVertexBufferID);
	glDeleteBuffers(1, &m_elementBufferID);

	for (size_t i = 0; i < vertexArrayObjectIDVec.size(); i++) {
		glDeleteVertexArrays(1, &vertexArrayObjectIDVec[i]);
	}
}