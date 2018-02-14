#include <src/Window.h>

/**
*        @brief Opengl basic function call.
*		 @details Setting openGL Version, Create window, glfwInputMode, glfwCursorSetting, Depth, Cull, Blend, Load Shader. Create main camera.
*		 @param void void
*		 @return void
*/
int Window::init(int width, int height) {
	windowWidth = width;
	windowHeight = height;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(windowWidth, windowHeight, "OPENGL", NULL, NULL);
	if (m_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);

	// But on MacOS X with a retina screen it'll be 1024*2 and 768*2, so we get the actual framebuffer size:
	glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(m_window, windowWidth / 2, windowHeight / 2);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//camera & control
	control = new Control();
	control->m_window = m_window;
	control->m_cameraAdd(glm::vec3(4.0f, 3.0f, 3.0f), 55.0f, 50.0f, 45.0f, 1.0f, 0.005f, 4.0f, 3.0f, 0.1f, 100.0f);
	control->m_changeCameraIndex(0);


	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
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
	glDrawBuffer(GL_NONE);
	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

/**
*   @brief Opengl basic function call.
*	@details Create OpenglResourceManager, BufferManager, ObjectStorage. Make init Object
*	@param void
*	@return void
*/
int Window::mains() {
	openglResourceManager = new OpenglResourceManager();
	bufferManager = new BufferManager();
	shaderManager = new ShaderManager();	//load shader in this func.
	textManager = new TextManager();

	//load vertex, texture files
	//openglResourceManager->addVertexVec("smallShip", "SpaceShip.obj");
	//openglResourceManager->addVertexVec("BigShip", "Aircraft_Export_Ready.obj");
	//openglResourceManager->addVertexVec("monkeyVertex", "suzanne.obj");
	//openglResourceManager->addTextureVec("uvMapTexture", "uvmap.DDS");
	openglResourceManager->addVertexVec("room", "obj/room_thickwalls.obj");
	//openglResourceManager->addVertexVec("monkeyVertex", "suzanne.obj");
	openglResourceManager->addTextureVec("uvMapTexture", "texture/uvmap.DDS");
	
	//make object storage for print : [textureNum][vertexNum][]
	objectStorage =
		std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>>(openglResourceManager->getBLTLen(),
			std::vector<std::vector<DrawableObjectWithTexture*>>(openglResourceManager->getBLVWTLen(),
				std::vector<DrawableObjectWithTexture*>()));

	deleteStorage =
		std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>>(openglResourceManager->getBLTLen(),
			std::vector<std::vector<DrawableObjectWithTexture*>>(openglResourceManager->getBLVWTLen(),
				std::vector<DrawableObjectWithTexture*>()));


	//register to buffer
	bufferManager->initBuffer(openglResourceManager);
	//textManager->textManagerInit();	//BUG
	
	makeObject("firstObjec2", "room", "uvMapTexture", glm::vec3(0, 0, 2), glm::vec3(), glm::vec3(1, 1, 1));
	makeObject("firstObject3", "room", "uvMapTexture", glm::vec3(), glm::vec3(), glm::vec3(1, 1, 1));

	//make object
	//makeObject("firstObject", "smallShip", "uvMapTexture", glm::vec3(), glm::vec3(), glm::vec3(0.2f, .2f, .2f));
	//makeObject("firstObject2", "monkeyVertex", "uvMapTexture", glm::vec3(4.5f, 4.5f, 7.2f), glm::vec3(), glm::vec3(1.0f, 1.0f, 1.0f));

	return 0;
}


/**
*   @brief Opengl Draw Main Loop.
*	@details Draw gameObjects.
*	@param void
*	@return void
*/
int Window::draws() {
	vector<GLuint> &vertexArrayVec = bufferManager->vertexArrayObjectIDVec;
	ShaderObj* shaderMainPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::MAIN);
	ShaderObj* shaderShadowPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::SHADOW);
	//ShaderObj* shaderTextPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::TEXT);

	//main loop
	do {
		control->m_controlProgress();	//control

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(shaderShadowPtr->m_shaderID);

		glm::vec3 lightInvDir = glm::vec3(4, 6, 6);
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrixNEW = glm::mat4(1.0);

		for (size_t i = 0; i < objectStorage.size(); i++) {
			for (size_t k = 0; k < objectStorage[i].size(); k++) {
				glBindVertexArray(bufferManager->vertexArrayObjectIDVec[k]);

				for (size_t idx = 0; idx < objectStorage[i][k].size(); idx++) {
					glm::mat4 depthModelMatrix = objectStorage[i][k][idx]->getRetMatrix();
					glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrixNEW * depthModelMatrix;
					
					glUniformMatrix4fv(shaderShadowPtr->depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
					glDrawElements(
						GL_TRIANGLES,      // mode
						openglResourceManager->getBLVWTWithIndex(k)->getIndiceVecNum(),    // count
						GL_UNSIGNED_SHORT,   // type
						(void*)bufferManager->elementOffset[k]         // element array buffer offset
						);
				}
				glBindVertexArray(0);
			}
		}
		glBindVertexArray(0);


		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderMainPtr->m_shaderID);

		glm::mat4 ProjectionMatrix = control->m_getCurCameraProjectionMatrix();
		glm::mat4 ViewMatrix = control->m_getCurCameraViewMatrix();	//look at
		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);
		glm::mat4 depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix * depthModelMatrixNEW;

		glUniformMatrix4fv(shaderMainPtr->m_cameraViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(shaderMainPtr->m_depthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
		glUniform3f(shaderMainPtr->m_lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(shaderMainPtr->m_shadowMapID, 1);
		
		for (size_t i = 0; i < objectStorage.size(); i++) {
			for (size_t k = 0; k < objectStorage[i].size(); k++) {
				glBindVertexArray(bufferManager->vertexArrayObjectIDVec[k]);

				for (size_t idx = 0; idx < objectStorage[i][k].size(); idx++) {
					glm::mat4 Model = objectStorage[i][k][idx]->getRetMatrix();
					glm::mat4 mvp = ProjectionMatrix * ViewMatrix  * Model;
					glUniformMatrix4fv(shaderMainPtr->m_modelMatrixID, 1, GL_FALSE, &Model[0][0]);
					glUniformMatrix4fv(shaderMainPtr->m_MVPMatrixID, 1, GL_FALSE, &mvp[0][0]);
					glDrawElements(
						GL_TRIANGLES,      // mode
						openglResourceManager->getBLVWTWithIndex(k)->getIndiceVecNum(),    // count
						GL_UNSIGNED_SHORT,   // type
						(void*)bufferManager->elementOffset[k]         // element array buffer offset
						);
				}
				glBindVertexArray(0);
			}
		}

		//textManager->printText2DWithIndex(0, "adf", 120, 120, 40);	//BUG

		// Swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);


	return 0;
}


/**
*   @brief Create gameObject and Register.
*	@details
*	@param std::string objName object name
*	@param std::string vertexObjectName vertex object name to use
*	@param std::string textureName texture name to use
*	@param glm::vec3 modelVec position vector
*	@param glm::vec3 angleVec rotate vector
*	@param glm::vec3 scaleVec scale vector
*	@return void
*/
void Window::makeObject(std::string objName, std::string vertexObjectName, std::string textureName, glm::vec3 modelVec, glm::vec3 angleVec, glm::vec3 scaleVec) {
	int textureIndex = openglResourceManager->getBLTIndexWithName(textureName);
	int objectIndex = openglResourceManager->getBLVWTIndexWithName(vertexObjectName);

	DynamicDrawableObjectWithTexture* newGameObj = new DynamicDrawableObjectWithTexture(objName, objectStorage[textureIndex][objectIndex].size(), textureIndex, objectIndex, modelVec, angleVec, scaleVec, false);
	m_NameToDrawingObjectMap.insert(std::make_pair(objName, newGameObj));
	objectStorage[textureIndex][objectIndex].push_back(newGameObj);
}
/**
*   @brief Delete gameObject.
*	@param std::string objName object name to delete
*	@return void
*/
void Window::deleteObject(std::string objName) {
	DrawableObjectWithTexture * obj = m_NameToDrawingObjectMap[objName];
	int textureIdx = obj->m_textureIdx;
	int vertexIdx = obj->m_vertexIdx;
	deleteStorage[textureIdx][vertexIdx].push_back(obj);
	obj->bDelete = true;
}

/**
*   @brief Delete garbage object.
*	@details deAlloc memory for gameobject
*	@param void
*	@return void
*/
void Window::deleteObjectRefresh() {
	for (size_t i = 0; i < deleteStorage.size(); i++) {
		for (size_t k = 0; k < deleteStorage[i].size(); k++) {
			if (deleteStorage[i][k].size() != 0) {
				std::sort(deleteStorage[i][k].begin(), deleteStorage[i][k].end(), cmpSortFunc);	//[i][k] vector sort with index
				DrawableObjectWithTexture * deleteObj = deleteStorage[i][k][0];

				int deleteCount = 0;	//delete acc count
				int deleteIdx;			//delete index


				std::vector<DrawableObjectWithTexture*>::iterator it = deleteStorage[i][k].begin();	//iterator
				deleteIdx = deleteObj->m_idx;
				std::advance(it, deleteIdx - 1);

				it = objectStorage[i][k].erase(it);
				deleteCount++;

				for (size_t p = 1; p < deleteStorage[i][k].size(); p++) {	//loop deleteObjectNum'th
					for (int z = 0; z < deleteStorage[i][k][p]->m_idx - deleteIdx - 1; z++) {	//loop (new iterator) ~ (next delete iterator - 1)
						(*it)->m_idx -= deleteCount;	//m_idx update
						++it;				//next iterator
					}
					deleteIdx = (*it)->m_idx;	//deleteIdx update
					it = objectStorage[i][k].erase(it);
					deleteCount++;
				}

				//m_NameToDrawingObjectMap.erase() : erase map
				//dealloc
				for (size_t p = 0; p < deleteStorage[i][k].size(); p++) {
					m_NameToDrawingObjectMap.erase(deleteStorage[i][k][p]->name);
					delete deleteStorage[i][k][p];
				}
				//erase deleteStorage
				deleteStorage[i][k].erase(deleteStorage[i][k].begin(), deleteStorage[i][k].end());

			}
		}
		//int textureIdx = needToBeDeletedObjectVec[i]->m_textureIdx;
		//int vertexIdx = needToBeDeletedObjectVec[i]->m_vertexIdx;
	}
}

/**
*   @brief Gameobject sort function
*	@details sort gameobject according to object index
*	@return bool less
*/
bool Window::cmpSortFunc(DrawableObjectWithTexture* (&o1), DrawableObjectWithTexture* (&o2)) {
	if (o1->m_idx < o2->m_idx) {
		return true;
	}
	return false;
}