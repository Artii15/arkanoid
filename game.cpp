#include "inc/Drawable.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

static void error_callback(int error, const char* description){
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	
	}
	
	window = glfwCreateWindow(800, 600, "Arkanoid 3D", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
		
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
		
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	
	while (!glfwWindowShouldClose(window)){

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
