#include <src/Window.h>

/**
*        @brief Opengl basic function call.
*		 @details Setting openGL Version, Create window, glfwInputMode, glfwCursorSetting, Depth, Cull, Blend, Load Shader. Create main camera.
*		 @param void void
*		 @return void
*/
int Window::init() {

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	m_window = glfwCreateWindow(windowWidth, windowHeight, "OPENGL", NULL, NULL);
	if (m_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);

	// We would expect width and height to be 1024 and 768
	int windowWidth = 1024;
	int windowHeight = 768;
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(m_window, windowWidth / 2, windowHeight / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);



	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("shader/ShadowMapping.vertexshader", "shader/ShadowMapping.fragmentshader");

	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	ShadowMapID = glGetUniformLocation(programID, "shadowMap");

	// Get a handle for our "LightPosition" uniform
	lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");





	//camera & control
	control = new Control();
	control->m_window = m_window;
	control->m_cameraAdd(glm::vec3(4.0f, 3.0f, 3.0f), 55.0f, 50.0f, 45.0f, 0.01f, 0.005f, 4.0f, 3.0f, 0.1f, 100.0f);
	control->m_changeCameraIndex(0);


	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);



	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
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

	//glBindVertexArray(VertexArrayID);

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
	do {
		control->m_controlProgress();
		//glBindVertexArray(VertexArrayID2);
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

									  // We don't use bias in the shader, but instead we draw back faces, 
									  // which are already separated from the front faces by a small distance 
									  // (if your geometry is made this way)
		glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT); // Cull back-facing triangles -> draw only front-facing triangles

							  // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(depthProgramID);

		glm::vec3 lightInvDir = glm::vec3(4, 6, 6);

		// Compute the MVP matrix from the light's point of view
		//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);

		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		//glm::mat4 depthModelMatrixNEW = glm::translate(glm::mat4(), lightInvDir);
		glm::mat4 depthModelMatrixNEW = glm::mat4(1.0);
		// or, for spot light :
		//glm::vec3 lightPos(5, 20, 20);
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

		//glm::mat4 depthModelMatrix = glm::mat4(1.0);
		//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;



		for (size_t i = 0; i < objectStorage.size(); i++) {
			for (size_t k = 0; k < objectStorage[i].size(); k++) {
				glBindVertexArray(bufferManager->vertexArrayObjectIDVec[k]);

				for (size_t idx = 0; idx < objectStorage[i][k].size(); idx++) {
					//glm::mat4 depthModelMatrix = glm::mat4(1.0);
					glm::mat4 depthModelMatrix = objectStorage[i][k][idx]->getRetMatrix();
					//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

					//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrixNEW * depthModelMatrix;
					glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrixNEW * depthModelMatrix;
					glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

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

							 // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input

		glm::mat4 ProjectionMatrix = control->m_getCurCameraProjectionMatrix();
		glm::mat4 ViewMatrix = control->m_getCurCameraViewMatrix();	//look at
		//ViewMatrix = glm::lookAt(glm::vec3(14,6,4), glm::vec3(0,1,0), glm::vec3(0,1,0));
		//glm::mat4 ModelMatrix = glm::mat4(1.0);
		//glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		//glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
		//glm::mat4 depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix * glm::mat4(1.0f);
		glm::mat4 depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix * depthModelMatrixNEW;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

		glUniform3f(lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);


		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(TextureID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(ShadowMapID, 1);
		/*
		glBindVertexArray(VertexArrayID2);
		// Draw the triangles !

		glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*)0           // element array buffer offset
		);
		glBindVertexArray(0);
		*/
		for (size_t i = 0; i < objectStorage.size(); i++) {
			for (size_t k = 0; k < objectStorage[i].size(); k++) {
				glBindVertexArray(bufferManager->vertexArrayObjectIDVec[k]);

				for (size_t idx = 0; idx < objectStorage[i][k].size(); idx++) {
					glm::mat4 Model = objectStorage[i][k][idx]->getRetMatrix();
					glm::mat4 mvp = ProjectionMatrix * ViewMatrix  * Model;
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
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

	//m_IDToDrawingObjectMap.insert(std::make_pair(newGameObj->iObjID, newGameObj));
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