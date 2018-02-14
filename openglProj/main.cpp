#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "src/Window.h"

int main() {
	Window windowClass;
	windowClass.init();
	windowClass.mains();
	windowClass.draws();
	return 0;
}
