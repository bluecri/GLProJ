#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "src/Window.h"

int main() {
	Window windowClass;
	windowClass.init(1024, 768);
	windowClass.mains();
	windowClass.draws();
	return 0;
}
