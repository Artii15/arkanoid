#ifndef LIGHT_H
#define LIGHT_H
#include <GL/gl.h>
#include "glm/glm.hpp"

struct light{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
	GLuint shininess;
	GLfloat k;
	
	light(){
		ambient = glm::vec4(0,0,0,1);
		diffuse = glm::vec4(1,1,1,1);
		specular = glm::vec4(1,1,1,1);
		position = glm::vec4(0,0,0,1);
		shininess = 50;
		k = 0.05f;
	}
};

#endif
