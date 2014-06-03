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
	Drawable d;
	d.loadShaders("shaders/vertex/bat.txt", "shaders/fragment/bat.txt");
	GLuint p = d.getShaderProgram();
	GLuint MatrixID = glGetUniformLocation(p, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(p, "V");
	GLuint ModelMatrixID = glGetUniformLocation(p, "M");
	
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(p, "vertex");
	d.loadObj("models/cube.obj");
		///// VBO i to co z tym związane ////////////
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, d.getVertices()->size() * sizeof(glm::vec4), &(d.getVertices()->at(0)), GL_STATIC_DRAW);
	
	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.getIndices()->size() * sizeof(unsigned short), &(d.getIndices()->at(0)) , GL_STATIC_DRAW);
	
	// Get a handle for our "LightPosition" uniform
	glUseProgram(p);
	
	/////////////////////////////////////////////////////////////
	
	while (!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(p);
		
		//Wylicz macierz rzutowania
		glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 800.0f/600.0f, 1.0f, 100.0f);
		//Wylicz macierz widoku
		glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0.0f,0.0f,7.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)); 
		//Wylicz macierz modelu
		glm::mat4 ModelMatrix = glm::rotate(glm::mat4(1.0f),60.0f,glm::vec3(0.5,1,0)); 
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
