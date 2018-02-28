#pragma once
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <src/glMain/bufferAndResource/OpenglResourceManager.h>
#include <src/glMain/bufferAndResource/BufferManager.h>
#include <src/glMain/shader/ShaderManager.h>
#include <src/glMain/text/TextManager.h>
#include <src/glMain/skybox/SkyboxManager.h>
#include "src/glMain/particle/ParticleManager.h"
#include <src/glMain/openAL/ALManager.h>

#include <src/glMain/particle/ParticleInfo.h>
#include <src/glMain/control/Control.h>

#include <src/game/collisionObject/HasCollisionObj.h>

#include <src/game/gameObject/EnemyPlane.h>
#include <src/game/gameObject/LaserBullet.h>
#include <src/game/gameObject/parent/DrawableObjectWithTexture.h>

#include "window/Window.h"

#include "./ManagerOfManagerObserver.h"



class ManagerOfManager : ManagerOfManagerObserver{
public:
	OpenglResourceManager * openglResourceManager;
	BufferManager* bufferManager;
	ShaderManager* shaderManager;	//load shader in this func.
	TextManager* textManager;
	SkyboxObjManager* skyboxManager;
	ALManager * alManager;
	ParticleManager * particleManager;

	CameraObject * mainCameraObjectPtr;


	Control * control;

	bool isRestart = false;


	//PortAudioClass * portAudioManage

	//object storage
	std::vector<std::vector<std::list<DrawableObjectWithTexture*>>> objectStorage;
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

	float collisionBox[3] = { 0.6f, 0.2f, 0.4f };
	float missileCollisionBox[3] = { 0.02f, 0.02f, 0.2f };
	glm::vec3 planeCollisionCenterCompensationVec;
	PlaneDeltaParam* deltaParamPtr;
	CollisionProcessInfo * planeCPI;
	glm::vec3 missileCollisionCenterCompensationVec;
	
	PlayerPlane * playerPlanePtr;

	ManagerOfManager() {
		openglResourceManager = new OpenglResourceManager();
		bufferManager = new BufferManager();
		shaderManager = new ShaderManager();	//load shader in this func.
		textManager = new TextManager();
		skyboxManager = new SkyboxObjManager();
		alManager = new ALManager();
		particleManager = new ParticleManager();
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

		alManager->init();
		particleManager->init();
		particleManager->bufferInit();
		//const
		planeCPI = new CollisionProcessInfo(10);
		planeCollisionCenterCompensationVec = glm::vec3(0.0f, 0.0f, 0.5f);
		missileCollisionCenterCompensationVec = glm::vec3(0.0f, 0.0f, 0.0f);
		float missileCollisionBox[3] = { 0.02f, 0.02f, 0.2f };
		deltaParamPtr = new PlaneDeltaParam(0, 0, 0, 0.1, 0.8, 0.1, 0.1, 0.8, 0.1, 0.05, 0.05, 0.05);

	}

	void resourceLoad() {
		//load vertex, texture files to orm
		openglResourceManager->addVertexVec("smallShip", "obj/SpaceShip.obj");
		openglResourceManager->addVertexVec("room", "obj/room_thickwalls.obj");
		openglResourceManager->addVertexVec("missile", "obj/missile.obj");
		openglResourceManager->addVertexVec("monkeyVertex", "obj/suzanne.obj");
		openglResourceManager->addTextureVec("uvMapTexture", "texture/uvmap.DDS");
		//openglResourceManager->addVertexVec("BigShip", "Aircraft_Export_Ready.obj");
		
		//openglResourceManager->addTextureVec("uvMapTexture", "uvmap.DDS");
		//openglResourceManager->addVertexVec("monkeyVertex", "suzanne.obj");

		//load to buffer
		bufferManager->initBuffer(openglResourceManager);
	}

	void resourceObjectStorageInit() {
		//make object storage for print : [textureNum][vertexNum][]
		objectStorage =
			std::vector<std::vector<std::list<DrawableObjectWithTexture*>>>(openglResourceManager->getBLTLen(),
				std::vector<std::list<DrawableObjectWithTexture*>>(openglResourceManager->getBLVWTLen(),
					std::list<DrawableObjectWithTexture*>()));

		deleteStorage =
			std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>>(openglResourceManager->getBLTLen(),
				std::vector<std::vector<DrawableObjectWithTexture*>>(openglResourceManager->getBLVWTLen(),
					std::vector<DrawableObjectWithTexture*>()));
	}

	void makeGameObjects() {
		//make game object
		//float collisionBox[3] = { 1.3f, 0.3f, 1.0f };
		int redColorStartRange[4] = { 230, 10, 10, 100 };
		int redColorEndRange[4] = { 255, 40, 40, 160 };

		int greenColorStartRange[4] = {10, 160, 10, 100 };
		int greenColorEndRange[4] = { 60, 245, 60, 160 };

		DynamicDrawableObjectWithTexture* tempPlayerPlaneDDOPtr = makeObject("playerObject", "smallShip", "uvMapTexture", glm::vec3(0, 0, 0), glm::vec3(), glm::vec3(0.2, 0.2, 0.2), planeCollisionCenterCompensationVec, collisionBox);
		
		playerPlanePtr = new PlayerPlane(control, mainCameraObjectPtr, 100, planeCPI, 0.0f, 4.0f, 1.0f, textManager, (DDOWithCollision*)tempPlayerPlaneDDOPtr, *deltaParamPtr);
		playerPlanePtr->bindLaserSourceToALSound(alManager->getALSoundPtrWithName("laser"));
		playerPlanePtr->bindHitSourceToALSound(alManager->getALSoundPtrWithName("hit"));
		playerPlanePtr->bindExplosionSourceToALSound(alManager->getALSoundPtrWithName("explosion"));
		playerPlanePtr->registerObserver(this);
		playerPlanePtr->registerBackParticle(particleManager->registerNewParticleInfo(
			new ParticleInfo(true, 0, 2.0f, glm::vec3(0.0f), glm::vec3(0.0f), 0.2f, greenColorStartRange, greenColorEndRange, 0.03f, 0.07f)));
		playerPlanePtr->registerDamagedParticle(particleManager->registerNewParticleInfo(
			new ParticleInfo(false, 0, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 0.6f, redColorStartRange, redColorEndRange, 0.03f, 0.07f)));
		hasCollisionObjList.push_back(playerPlanePtr);


		DynamicDrawableObjectWithTexture* tempEnemyPlaneDDOPtr = makeObject("enemyObject", "smallShip", "uvMapTexture", glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(), glm::vec3(0.2, 0.2, 0.2), planeCollisionCenterCompensationVec, collisionBox);
		EnemyPlane* tempEnemyPlanePtr = new EnemyPlane(control, playerPlanePtr, 100, planeCPI, 0.0f, 5.0f, 1.0f, textManager, (DDOWithCollision*)tempEnemyPlaneDDOPtr, *deltaParamPtr);
		tempEnemyPlanePtr->bindLaserSourceToALSound(alManager->getALSoundPtrWithName("laser"));
		tempEnemyPlanePtr->bindHitSourceToALSound(alManager->getALSoundPtrWithName("hit"));
		tempEnemyPlanePtr->bindExplosionSourceToALSound(alManager->getALSoundPtrWithName("explosion"));
		tempEnemyPlanePtr->registerObserver(this);
		tempEnemyPlanePtr->registerBackParticle(particleManager->registerNewParticleInfo(
			new ParticleInfo(true, 20, 2.0f, glm::vec3(0.0f), glm::vec3(0.0f), 0.02f, greenColorStartRange, greenColorEndRange, 0.03f, 0.07f)));
		tempEnemyPlanePtr->registerDamagedParticle(particleManager->registerNewParticleInfo(
			new ParticleInfo(false, 0, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f), 0.6f, redColorStartRange, redColorEndRange, 0.03f, 0.07f)));
		hasCollisionObjList.push_back(tempEnemyPlanePtr);

		/*
		LaserBullet * tempLaserBullet;
		
		tempEnemyPlaneDDOPtr = makeObject("missieObj1", "missile", "uvMapTexture", glm::vec3(2.0f, 2.0f, 5.0f), glm::vec3(), glm::vec3(0.2, 0.2, 0.2), missileCollisionCenterCompensationVec, missileCollisionBox);
		tempLaserBullet = new LaserBullet(control, playerPlanePtr, 100, planeCPI, 10.0f, 10.0f, 1.0f, textManager, (DDOWithCollision*)tempEnemyPlaneDDOPtr, *deltaParamPtr);
		tempLaserBullet->registerObserver(this);
		hasCollisionObjList.push_back(tempLaserBullet);
		*/
	}

	void gameLogicUpdate() {
		if (isRestart) {
			isRestart = false;
			playerPlanePtr->m_hp = 100;
			playerPlanePtr->m_speed = 0.0f;
			playerPlanePtr->isCollisionCheck = true;
			playerPlanePtr->m_ddoWithCollision->isDrawableObjDraw = true;
			playerPlanePtr->isWillBeDeleted = false;

			for (HasCollisionObj* elem : hasCollisionObjList) {
				if (elem != playerPlanePtr) {
					elem->isWillBeDeleted = true;
				}
			}
		}
		else {

		}
	}

	void collisionCheck() {
		//collision check
		std::list<HasCollisionObj*>::iterator collisionPtrListIterator, collisionPtrListInnerIterator;

		for (collisionPtrListIterator = hasCollisionObjList.begin(); collisionPtrListIterator != hasCollisionObjList.end(); ++collisionPtrListIterator) {
			HasCollisionObj* tempCollisionPtr = *collisionPtrListIterator;
			if (tempCollisionPtr->isCollisionCheck == true) {
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
	}

	void doControlProgress() {
		control->m_controlProgress();	//control
	}

	void updateGameObject() {
		float deltaTimeInLoop = control->m_deltaTime;

		for (std::list<HasCollisionObj*>::iterator it = hasCollisionObjList.begin(); it != hasCollisionObjList.end();) {
			if ((*it)->isCollisionObjDelete) {
				(*it)->m_ddoWithCollision->isDrawableObjDelete = true;	//check one more time.
				delete *it;
				it = hasCollisionObjList.erase(it);		//remove collision obj
			}
			else {
				(*it)->update(deltaTimeInLoop);			//update() all obj
				++it;
			}
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
				for (std::list<DrawableObjectWithTexture*>::iterator it = objectStorage[i][k].begin(); it != objectStorage[i][k].end();) {
					if ((*it)->isDrawableObjDelete) {
						//erase
						DrawableObjectWithTexture* deleteDrawableObjectWithTexture = (*it);
						m_NameToDrawingObjectMap.erase((*it)->name);
						it = objectStorage[i][k].erase(it);
						delete deleteDrawableObjectWithTexture;
					}
					else if((*it)->isDrawableObjDraw){
						//draw
						glm::mat4 depthModelMatrix = (*it)->getRetMatrix();
						glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrixNEW * depthModelMatrix;

						glUniformMatrix4fv(shaderShadowPtr->depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
						glDrawElements(
							GL_TRIANGLES,      // mode
							openglResourceManager->getBLVWTWithIndex(k)->getIndiceVecNum(),    // count
							GL_UNSIGNED_SHORT,   // type
							(void*)bufferManager->elementOffset[k]         // element array buffer offset
						);
						++it;
					}
					else {
						++it;
					}
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

				for (DrawableObjectWithTexture * storageElem : objectStorage[i][k]) {
					if (storageElem->isDrawableObjDelete == false && storageElem->isDrawableObjDraw) {
						glm::mat4 Model = storageElem->getRetMatrix();
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
				}
				glBindVertexArray(0);
			}
		}
	}

	void uiDraw() {
		//textManager->printText2DWithIndex(0, "adf", 160, 120, 20);
		//textManager->printText2DWithIndex(0, "abcdefghijklmnopqrstuvwxyz", 120, 120, 40);
		//textManager->printText2DWithIndex(0, "ABCDEFGHIJKLMNOPQUSTUVWXYZ", 120, 170, 40);
		textManager->printAllLIst(control->m_deltaTime);
		textManager->printAllModelVecLIst(ViewMatrix, ProjectionMatrix, control->m_deltaTime, control->m_height, control->m_width);
	}
	void collisionBoxDraw() {
		//glDisable(GL_DEPTH_TEST);
		ShaderObj* shaderCollisionPtr = shaderManager->getShaderPtrWithEnum(ShaderManager::ENUM_SHADER_IDX::COLISION);


		glUseProgram(shaderCollisionPtr->m_shaderID);
		//collision box draw
		glBindVertexArray(bufferManager->m_collisionVertexID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferManager->m_collisionBufferID);

		GLsizeiptr vec3Size = sizeof(glm::vec3);
		glBufferData(GL_ARRAY_BUFFER, vec3Size * hasCollisionObjList.size() * 8, 0, GL_STATIC_DRAW);
		GLsizeiptr glSIzePtrForCollisionDraw = 0;
		std::vector<glm::vec3> vertexBuffer;
		for (HasCollisionObj* objPtr : hasCollisionObjList) {
			for (int x = -1; x <= 1; x += 2) {
				for (int y = -1; y <= 1; y += 2) {
					for (int z = -1; z <= 1; z += 2) {
						glm::vec3 tempCollisionBoxPoint(objPtr->m_ddoWithCollision->pro_obbClass.m_centerCompensationVec);
						float* tempCollisionAxisLen = objPtr->m_ddoWithCollision->pro_obbClass.m_axisLen;
						tempCollisionBoxPoint += glm::vec3(float(x) * tempCollisionAxisLen[0], float(y) * tempCollisionAxisLen[1], float(z) * tempCollisionAxisLen[2]);
						vertexBuffer.push_back(tempCollisionBoxPoint);
						//glBufferSubData(GL_ARRAY_BUFFER, glSIzePtrForCollisionDraw, vec3Size, (void*)&(tempCollisionBoxPoint));
						//glSIzePtrForCollisionDraw += vec3Size;
					}
				}
			}
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, vec3Size * hasCollisionObjList.size() * 8, (void*)&(vertexBuffer[0]));


		GLsizeiptr unsignedShortSize = sizeof(unsigned short);
		std::vector<unsigned short> boxElement = { 0, 1, 0, 2, 2, 3, 1, 3, 4, 5, 4, 6, 6, 7, 5, 7, 1, 5, 3, 7, 2, 6, 0, 4 };
		std::vector<unsigned short> boxElementVec(hasCollisionObjList.size() * 24);

		int i = 0, loop = 0;
		for (HasCollisionObj* objPtr : hasCollisionObjList) {
			for (int k = 0; k < 24; k++, i++) {
				boxElementVec[i] = boxElement[k] + 8 * loop;
			}
			loop++;
		}
		int boxElementIndex = 0;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferManager->m_collisionElementID);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, unsignedShortSize * hasCollisionObjList.size() * 24, (void*)&boxElement[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, unsignedShortSize * hasCollisionObjList.size() * 24, (void*)&boxElementVec[0], GL_STATIC_DRAW);

		GLsizeiptr collisionElementOffset = 0;

		for (HasCollisionObj* objPtr : hasCollisionObjList) {
			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * objPtr->m_ddoWithCollision->getModelMatrix() * objPtr->m_ddoWithCollision->getRotationMatrix();
			glUniformMatrix4fv(shaderCollisionPtr->m_MVPMatrixID, 1, GL_FALSE, &mvp[0][0]);
			glDrawElements(
				GL_LINES,      // mode
				24,    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)collisionElementOffset         // element array buffer offset
			);
			collisionElementOffset += unsignedShortSize * 24;
		}
		glBindVertexArray(0);
		//glEnable(GL_DEPTH_TEST);
	}
	void drawParticles() {
		particleManager->setUniform(ViewMatrix, ProjectionMatrix);
		particleManager->adjustNewParticles();
		particleManager->updateAndRegisterParticles(control->m_deltaTime, mainCameraObjectPtr->modelVec);
		particleManager->drawParticles();
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

		gameLogicUpdate();
		updateGameObject();

		drawShadowDepthToBuffer();
		renderToScreenStart();
		drawSkybox();
		drawObjects();
		drawParticles();

		collisionBoxDraw();

		uiDraw();
		
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
	DynamicDrawableObjectWithTexture* makeObject(std::string objName, std::string vertexObjectName, std::string textureName, glm::vec3 modelVec, glm::mat4 rotMat, glm::vec3 scaleVec, glm::vec3 compenVec, float axisLen[3]) {
		int textureIndex = openglResourceManager->getBLTIndexWithName(textureName);
		int objectIndex = openglResourceManager->getBLVWTIndexWithName(vertexObjectName);

		DynamicDrawableObjectWithTexture* newGameObj = new DDOWithCollision(objName, objectStorage[textureIndex][objectIndex].size(), textureIndex, objectIndex, modelVec, rotMat, scaleVec, compenVec, axisLen, false);
		m_NameToDrawingObjectMap.insert(std::make_pair(objName, newGameObj));
		objectStorage[textureIndex][objectIndex].push_back(newGameObj);
		return newGameObj;

	}
	
	virtual void bulletFire(glm::vec3 pos, glm::mat4 rotMat) override {
		LaserBullet * tempLaserBullet;
		DDOWithCollision *tempDDOPtr = (DDOWithCollision*)makeObject("missieObj10", "missile", "uvMapTexture", pos, rotMat, glm::vec3(0.2, 0.2, 0.2), missileCollisionCenterCompensationVec, missileCollisionBox);
		tempLaserBullet = new LaserBullet(control, playerPlanePtr, 100, planeCPI, 10.0f, 10.0f, 1.0f, textManager, tempDDOPtr, *deltaParamPtr);
		hasCollisionObjList.push_back(tempLaserBullet);
	}

	virtual void restartGame() override {
		isRestart = true;
	}

	//delete progress
	/*
	(*it)->update(deltaTimeInLoop);			//update() all obj
	update�� isCollisionObjDelete�� true�� ��� delete ����
	if ((*it)->isCollisionObjDelete) {
		(*it)->m_ddoWithCollision->isDrawableObjDelete = true;	//check one more time.
		delete *it;
		it = hasCollisionObjList.erase(it);		//remove collision obj
	}

	delete (*it);
	m_NameToDrawingObjectMap.erase((*it)->name);
	it = objectStorage[i][k].erase(it);
	*/
};