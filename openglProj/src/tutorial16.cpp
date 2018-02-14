// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <src/glMain/texture.h>
#include <common/controls.hpp>
#include <src/glMain/objectLoader.h>
#include <src/glMain/vboindexer.h>
#include <src/glMain/OpenglResourceManager.h>
#include <src/glMain/BufferManager.h>
#include <src/game/DrawableObjectWithTexture.h>
#include <src/game/DynamicDrawableObjectWithTexture.h>
#include <vector>

void makeObject(OpenglResourceManager* openglResourceManager, std::string objName, std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> &objectStorage, std::map<std::string, DrawableObjectWithTexture*> &m_NameToDrawingObjectMap, std::string vertexObjectName, std::string textureName, glm::vec3 modelVec, glm::vec3 angleVec, glm::vec3 scaleVec) {
	int textureIndex = openglResourceManager->getBLTIndexWithName(textureName);
	int objectIndex = openglResourceManager->getBLVWTIndexWithName(vertexObjectName);

	DynamicDrawableObjectWithTexture* newGameObj = new DynamicDrawableObjectWithTexture(objName, objectStorage[textureIndex][objectIndex].size(), textureIndex, objectIndex, modelVec, angleVec, scaleVec, false);

	//m_IDToDrawingObjectMap.insert(std::make_pair(newGameObj->iObjID, newGameObj));
	m_NameToDrawingObjectMap.insert(std::make_pair(objName, newGameObj));

	objectStorage[textureIndex][objectIndex].push_back(newGameObj);
}
int main(void)
{
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
	window = glfwCreateWindow(1024, 768, "Tutorial 16 - Shadows", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// We would expect width and height to be 1024 and 768
	int windowWidth = 1024;
	int windowHeight = 768;
	// But on MacOS X with a retina screen it'll be 1024*2 and 768*2, so we get the actual framebuffer size:
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//GLuint VertexArrayID;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	GLuint VertexArrayID2;
	glGenVertexArrays(1, &VertexArrayID2);

	// Create and compile our GLSL program from the shaders
	GLuint depthProgramID = LoadShaders("shader/DepthRTT.vertexshader", "shader/DepthRTT.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

	// Load the texture
	GLuint Texture = loadDDS("texture/uvmap.DDS");

	OpenglResourceManager* openglResourceManager = new OpenglResourceManager();
	BufferManager* bufferManager = new BufferManager();

	//load vertex, texture files
	//openglResourceManager->addVertexVec("smallShip", "SpaceShip.obj");
	//openglResourceManager->addVertexVec("BigShip", "Aircraft_Export_Ready.obj");
	openglResourceManager->addVertexVec("room", "obj/room_thickwalls.obj");
	//openglResourceManager->addVertexVec("monkeyVertex", "suzanne.obj");
	openglResourceManager->addTextureVec("uvMapTexture", "texture/uvmap.DDS");

	std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> objectStorage;
	std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> deleteStorage;

	//std::map<int, DrawableObjectWithTexture*>m_IDToDrawingObjectMap;
	std::map<std::string, DrawableObjectWithTexture*>m_NameToDrawingObjectMap;

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

	//make object
	makeObject(openglResourceManager, "firstObjec2", objectStorage, m_NameToDrawingObjectMap, "room", "uvMapTexture", glm::vec3(0, 0, 2), glm::vec3(), glm::vec3(1, 1, 1));
	makeObject(openglResourceManager, "firstObject3", objectStorage, m_NameToDrawingObjectMap, "room", "uvMapTexture", glm::vec3(), glm::vec3(), glm::vec3(1, 1, 1));
	
	//makeObject("firstObject", "smallShip", "uvMapTexture", glm::vec3(), glm::vec3(), glm::vec3(0.2f, .2f, .2f));
	//makeObject("firstObject2", "monkeyVertex", "uvMapTexture", glm::vec3(4.5f, 4.5f, 7.2f), glm::vec3(), glm::vec3(1.0f, 1.0f, 1.0f));


	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------

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

	// The quad's FBO. Used only for visualizing the shadowmap.
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("shader/ShadowMapping.vertexshader", "shader/ShadowMapping.fragmentshader");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");

	// Get a handle for our "LightPosition" uniform
	GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");




	/*
	glBindVertexArray(VertexArrayID2);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	0,                  // attribute
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
	1,                                // attribute
	2,                                // size
	GL_FLOAT,                         // type
	GL_FALSE,                         // normalized?
	0,                                // stride
	(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
	2,                                // attribute
	3,                                // size
	GL_FLOAT,                         // type
	GL_FALSE,                         // normalized?
	0,                                // stride
	(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBindVertexArray(0);
	//glBindVertexArray(VertexArrayID);
	*/


	int test1 = 0;
	bool testbool = true;

	do {

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
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
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
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

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


		if (testbool) {
			//objectStorage[0][0][0]->accModelMatrix(glm::vec3(0.0, 0.0, 0.1));
			++test1;
		}
		else {
			//objectStorage[0][0][0]->accModelMatrix(glm::vec3(0.0, 0.0, -0.1));
			--test1;
		}
		if (test1 == 10) {
			testbool = false;
		}
		else if (test1 == -1) {
			testbool = true;
		}
			


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteProgram(depthProgramID);

	glDeleteTextures(1, &Texture);

	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(1, &depthTexture);

	//glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
