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

	glClearColor(0.0f, 0.0f, 0.7f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	
	return 0;
}

/**
*   @brief Opengl basic function call.
*	@details Create OpenglResourceManager, BufferManager, ObjectStorage. Make init Object
*	@param void
*	@return void
*/
int Window::mains() {
	managerOfManager = new ManagerOfManager();
	managerOfManager->init(m_window, windowWidth, windowHeight);
	managerOfManager->makeGameObjects();
	return 0;
}

/**
*   @brief Opengl Draw Main Loop.
*	@details Draw gameObjects.
*	@param void
*	@return void
*/
int Window::draws() {
	//vector<GLuint> &vertexArrayVec = bufferManager->vertexArrayObjectIDVec;
	
	//main loop
	do {
		managerOfManager->draws();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);


	return 0;
}

