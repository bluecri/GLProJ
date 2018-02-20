#pragma once
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <src/glMain/OpenglResourceManager.h>
#include <src/glMain/BufferManager.h>
#include <src/glMain/ShaderManager.h>
#include <src/glMain/TextManager.h>
#include <src/glMain/SkyboxManager.h>
#include <src/game/DrawableObjectWithTexture.h>

class ManagerOfManager {
public :
	OpenglResourceManager * openglResourceManager;
	BufferManager* bufferManager;
	ShaderManager* shaderManager;	//load shader in this func.
	TextManager* textManager;
	SkyboxObjManager* skyboxManager;

	CameraObject * mainCameraObjectPtr;
	Control * control;



	//object storage
	std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> objectStorage;
	std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> deleteStorage;

	std::map<std::string, DrawableObjectWithTexture*>m_NameToDrawingObjectMap;

	std::list<HasCollisionObj*> hasCollisionObjList;

	glm::vec3 lightInvDir = glm::vec3(-4, 6, 6);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrixNEW = glm::mat4(1.0);

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;	//look at
	glm::mat4 ModelMatrixSkybox;

	ManagerOfManager(){
		openglResourceManager = new OpenglResourceManager();
		bufferManager = new BufferManager();
		shaderManager = new ShaderManager();	//load shader in this func.
		textManager = new TextManager();
		skyboxManager = new SkyboxObjManager();
	}

	void init(GLFWwindow* window, int width, int height) {
		control = new Control(window, width, height);
		mainCameraObjectPtr = new CameraObject(glm::vec3(4.0f, 3.0f, 3.0f), 55.0f, 50.0f, 45.0f, 3.0f, 0.005f, 4.0f, 3.0f, 0.1f, 140.0f);
		//camera & control
		//control->m_cameraAdd(glm::vec3(4.0f, 3.0f, 3.0f), 55.0f, 50.0f, 45.0f, 3.0f, 0.005f, 4.0f, 3.0f, 0.1f, 140.0f);
		control->m_cameraObjectVec.push_back(mainCameraObjectPtr);
		control->m_changeCameraIndex(0);

		resourceLoad();
		resourceObjectStorageInit();

		textManager->textManagerInit();
		skyboxManager->bufferInit();
	}

	void resourceLoad() {
		//load vertex, texture files to orm
		openglResourceManager->addVertexVec("smallShip", "obj/SpaceShip.obj");
		openglResourceManager->addVertexVec("room", "obj/room_thickwalls.obj");
		openglResourceManager->addTextureVec("uvMapTexture", "texture/uvmap.DDS");
		//openglResourceManager->addVertexVec("BigShip", "Aircraft_Export_Ready.obj");
		//openglResourceManager->addVertexVec("monkeyVertex", "suzanne.obj");
		//openglResourceManager->addTextureVec("uvMapTexture", "uvmap.DDS");
		//openglResourceManager->addVertexVec("monkeyVertex", "suzanne.obj");

		//load to buffer
		bufferManager->initBuffer(openglResourceManager);
	}

	void resourceObjectStorageInit() {
		//make object storage for print : [textureNum][vertexNum][]
		objectStorage =
			std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>>(openglResourceManager->getBLTLen(),
				std::vector<std::vector<DrawableObjectWithTexture*>>(openglResourceManager->getBLVWTLen(),
					std::vector<DrawableObjectWithTexture*>()));

		deleteStorage =
			std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>>(openglResourceManager->getBLTLen(),
				std::vector<std::vector<DrawableObjectWithTexture*>>(openglResourceManager->getBLVWTLen(),
					std::vector<DrawableObjectWithTexture*>()));
	}

	void makeGameObjects() {
		//make game object
		//float collisionBox[3] = { 1.3f, 0.3f, 1.0f };
		float collisionBox[3] = { 0.6f, 0.2f, 0.4f };
		glm::vec3 planeCollisionCenterCompensationVec(0.0f, 0.0f, 0.5f);
		PlaneDeltaParam tempDeltaParam(0, 0, 0, 0.1, 0.8, 0.1, 0.1, 0.8, 0.1, 0.05, 0.05, 0.05);

		DynamicDrawableObjectWithTexture* tempPlayerPlaneDDOPtr = makeObject("playerObject", "smallShip", "uvMapTexture", glm::vec3(0, 0, 0.1), glm::vec3(), glm::vec3(0.2, 0.2, 0.2), planeCollisionCenterCompensationVec, collisionBox);
		CollisionProcessInfo * planeCPI = new CollisionProcessInfo(10);
		PlayerPlane* playerPlanePtr = new PlayerPlane(control, mainCameraObjectPtr, 100, planeCPI, 0.0f, 4.0f, 1.0f, textManager, (DDOWithCollision*)tempPlayerPlaneDDOPtr, tempDeltaParam);
		hasCollisionObjList.push_back(playerPlanePtr);


		DynamicDrawableObjectWithTexture* tempEnemyPlaneDDOPtr = makeObject("enemyObject", "smallShip", "uvMapTexture", glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(), glm::vec3(0.2, 0.2, 0.2), planeCollisionCenterCompensationVec, collisionBox);
		EnemyPlane* tempEnemyPlanePtr = new EnemyPlane(playerPlanePtr, 100, planeCPI, 0.0f, 3.0f, 1.0f, textManager, (DDOWithCollision*)tempEnemyPlaneDDOPtr, tempDeltaParam);
		hasCollisionObjList.push_back(tempEnemyPlanePtr);

	}

	void collisionCheck() {
		//collision check
		std::list<HasCollisionObj*>::iterator collisionPtrListIterator, collisionPtrListInnerIterator;

		for (collisionPtrListIterator = hasCollisionObjList.begin(); collisionPtrListIterator != hasCollisionObjList.end(); ++collisionPtrListIterator) {
			HasCollisionObj* tempCollisionPtr = *collisionPtrListIterator;
			for (collisionPtrListInnerIterator = std::next(collisionPtrListIterator); collisionPtrListInnerIterator != hasCollisionObjList.end(); ++collisionPtrListInnerIterator) {
				HasCollisionObj* tempCompCollisionPtr = *collisionPtrListInnerIterator;
				if (tempCollisionPtr->collisionCheck(tempCompCollisionPtr)) {
					//collision occur!
					tempCollisionPtr->collisionOccur(tempCompCollisionPtr->m_cpi);
					tempCompCollisionPtr->collisionOccur(tempCollisionPtr->m_cpi);
				}
			}
		}
	}

	void doControlProgress() {
		control->m_controlProgress();	//control
	}

	void updateGameObject() {
		float deltaTimeInLoop = control->m_deltaTime;
		//update() all obj
		for (auto &elem : hasCollisionObjList) {
			elem->update(deltaTimeInLoop);
		}
	}

	void drawShadowDepthToBuffer() {
		ShaderObj* shaderShadowPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::SHADOW);

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, bufferManager->FramebufferName);

		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(shaderShadowPtr->m_shaderID);

		

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
	}

	void renderToScreenStart() {
		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, control->m_width, control->m_height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	}
	void updateCameraMVP() {
		ProjectionMatrix = mainCameraObjectPtr->getProjectionMatrix();
		ViewMatrix = mainCameraObjectPtr->getViewMatrix();	//look at
		ModelMatrixSkybox = mainCameraObjectPtr->getModelMatrix();
	}
	void drawSkybox() {
		//draw skybox
		//get camera matrix
		

		skyboxManager->setUniformModelMatrixWithDivide(ModelMatrixSkybox, 1.1f);
		skyboxManager->setUniformViewMatrix(ViewMatrix);
		skyboxManager->setUniformProjectionMatrix(ProjectionMatrix);
		skyboxManager->drawSkyBox();
	}
	void drawObjects() {
		ShaderObj* shaderMainPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::MAIN);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
		glUseProgram(shaderMainPtr->m_shaderID);

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

		// should active texture # and bind(->texture #)
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bufferManager->depthTexture);
		glUniform1i(shaderMainPtr->m_shadowMapID, 1);

		for (size_t i = 0; i < objectStorage.size(); i++) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, openglResourceManager->getBLTWithIndex(i)->m_p_TextureID);
			glUniform1i(shaderMainPtr->m_textureID, 0);
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
	}

	void uiDraw() {
		textManager->printText2DWithIndex(0, "adf", 160, 120, 40);
		//textManager->printText2DWithIndex(0, "abcdefghijklmnopqrstuvwxyz", 120, 120, 40);
		//textManager->printText2DWithIndex(0, "ABCDEFGHIJKLMNOPQUSTUVWXYZ", 120, 170, 40);

	}
	void collisionBoxDraw() {
		ShaderObj* shaderCollisionPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::COLISION);

		//collision box draw
		glBindVertexArray(bufferManager->m_collisionVertexID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferManager->m_collisionBufferID);

		GLsizeiptr vec3Size = sizeof(glm::vec3);
		glBufferData(GL_ARRAY_BUFFER, vec3Size * hasCollisionObjList.size() * 8, 0, GL_STATIC_DRAW);
		GLsizeiptr glSIzePtrForCollisionDraw = 0;
		for (HasCollisionObj* objPtr : hasCollisionObjList) {
			for (int x = -1; x <= 1; x += 2) {
				for (int y = -1; y <= 1; y += 2) {
					for (int z = -1; z <= 1; z += 2) {
						glm::vec3 tempCollisionBoxPoint(objPtr->m_ddoWithCollision->pro_obbClass.m_centerCompensationVec);
						float* tempCollisionAxisLen = objPtr->m_ddoWithCollision->pro_obbClass.m_axisLen;
						tempCollisionBoxPoint += glm::vec3(float(x) * tempCollisionAxisLen[0], float(y) * tempCollisionAxisLen[1], float(z) * tempCollisionAxisLen[2]);

						glBufferSubData(GL_ARRAY_BUFFER, glSIzePtrForCollisionDraw, vec3Size, (void*)&(tempCollisionBoxPoint));
						glSIzePtrForCollisionDraw += vec3Size;
					}
				}
			}
		}
		GLsizeiptr unsignedShortSize = sizeof(unsigned short);
		std::vector<unsigned short> boxElement = { 0, 1, 0, 2, 2, 3, 1, 3, 4, 5, 4, 6, 6, 7, 5, 7, 1, 5, 3, 7, 2, 6, 0, 4 };
		int boxElementIndex = 0;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferManager->m_collisionElementID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, unsignedShortSize * hasCollisionObjList.size() * 24, (void*)&boxElement[0], GL_STATIC_DRAW);

		GLsizeiptr collisionElementOffset = 0;

		glUseProgram(shaderCollisionPtr->m_shaderID);
		for (HasCollisionObj* objPtr : hasCollisionObjList) {
			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * objPtr->m_ddoWithCollision->getModelMatrix() * objPtr->m_ddoWithCollision->getRotationMatrix();
			glUniformMatrix4fv(shaderCollisionPtr->m_MVPMatrixID, 1, GL_FALSE, &mvp[0][0]);
			glDrawElements(
				GL_LINES,      // mode
				24,    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0         // element array buffer offset
			);
		}
		glBindVertexArray(0);
	}

	void endDraws() {
		// Swap buffers
		glfwSwapBuffers(control->m_window);
		glfwPollEvents();
	}

	void draws() {
		collisionCheck();
		doControlProgress();

		updateCameraMVP();
		updateGameObject();

		drawShadowDepthToBuffer();
		renderToScreenStart();
		drawSkybox();
		drawObjects();
		uiDraw();

		collisionBoxDraw();
		endDraws();
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
	DynamicDrawableObjectWithTexture* makeObject(std::string objName, std::string vertexObjectName, std::string textureName, glm::vec3 modelVec, glm::vec3 angleVec, glm::vec3 scaleVec, glm::vec3 compenVec, float axisLen[3]) {
		int textureIndex = openglResourceManager->getBLTIndexWithName(textureName);
		int objectIndex = openglResourceManager->getBLVWTIndexWithName(vertexObjectName);

		DynamicDrawableObjectWithTexture* newGameObj = new DDOWithCollision(objName, objectStorage[textureIndex][objectIndex].size(), textureIndex, objectIndex, modelVec, angleVec, scaleVec, compenVec, axisLen, false);
		m_NameToDrawingObjectMap.insert(std::make_pair(objName, newGameObj));
		objectStorage[textureIndex][objectIndex].push_back(newGameObj);
		return newGameObj;
	}
	/**
	*   @brief Delete gameObject.
	*	@param std::string objName object name to delete
	*	@return void
	*/
	void deleteObject(std::string objName) {
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
	void deleteObjectRefresh() {
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
	static bool cmpSortFunc(DrawableObjectWithTexture* (&o1), DrawableObjectWithTexture* (&o2)) {
		if (o1->m_idx < o2->m_idx) {
			return true;
		}
		return false;
	}
};