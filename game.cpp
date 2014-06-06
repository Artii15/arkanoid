#include "inc/Drawable.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
	
	//////////// Ładowanie do pamięci karty //////////////////////
	Drawable *d = new Drawable();
	d->loadShaders("shaders/vertex/bat.txt", "shaders/fragment/bat.txt");
	d->loadObj("models/cube.obj");	
	/////////////////////////////////////////////////////////////
	
	while (!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Wylicz macierz rzutowania
		glm::mat4 p = glm::perspective(1.0f, 800.0f/600.0f, 1.0f, 100.0f);
		//Wylicz macierz widoku
		glm::mat4 v = glm::lookAt(glm::vec3(0.0f,0.0f,7.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)); 
		
		d->draw(v, p);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete d;
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}
