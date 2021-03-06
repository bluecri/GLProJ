// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <src/glMain/texture.h>

#include <src/glMain/objectLoader.h>
#include <src/glMain/vboindexer.h>
#include "src/glMain/OpenglResourceManager.h"
#include <src/glMain/BufferManager.h>
#include <src/game/DrawableObjectWithTexture.h>
#include <src/game/DynamicDrawableObjectWithTexture.h>
#include <src/glMain/Control.h>
#include <src/glMain/ShaderManager.h>
#include <src/glMain/TextManager.h>
#include <src/glMain/SkyboxManager.h>
#include <vector>
#include <algorithm>

using namespace std;

/**
*
* @brief Window main class
* @details Program basic functions
* @author bluecri
* @date 2018-02-12
* @version 0.0.1
*
*/
class Window {
public:
	GLFWwindow * m_window;

	OpenglResourceManager * openglResourceManager;
	BufferManager * bufferManager;
	ShaderManager * shaderManager;
	TextManager * textManager;
	SkyboxObjManager * skyboxManager;
	
	Control * control;

	std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> objectStorage;
	std::vector<std::vector<std::vector<DrawableObjectWithTexture*>>> deleteStorage;

	//std::map<int, DrawableObjectWithTexture*>m_IDToDrawingObjectMap;
	std::map<std::string, DrawableObjectWithTexture*>m_NameToDrawingObjectMap;

	//shader
	GLuint programID;
	GLuint depthProgramID;


	//uniform in shader
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint TextureID;
	GLuint LightID;

	GLuint DepthBiasID;
	GLuint ShadowMapID;
	GLuint lightInvDirID;
	GLuint depthMatrixID;

	GLuint depthTexture;
	GLuint FramebufferName;

	int windowWidth;
	int windowHeight;

	int init(int width, int height);
	int mains();
	int draws();
	void makeObject(std::string objName, std::string vertexObjectName, std::string textureName, glm::vec3 modelVec, glm::vec3 angleVec, glm::vec3 scaleVec);
	void deleteObject(std::string objName);

	void deleteObjectRefresh();

	static bool cmpSortFunc(DrawableObjectWithTexture* (&o1), DrawableObjectWithTexture* (&o2));


	GLuint tempVID;
	GLuint vertexbuffer;
	GLuint colorbuffer;
};


static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};