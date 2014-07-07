#ifndef MATERIALS_SAMPLERS_H
#define MATERIALS_SAMPLERS_H

#include <GL/gl.h>

struct materials_samplers{
	// Numery jednostek teksturujących
	GLuint diffuse;
	GLuint specular;
	GLuint ambient;
	
	materials_samplers(){
		diffuse = GL_TEXTURE0;
		ambient = GL_TEXTURE1;
		specular = GL_TEXTURE2;
	}
};

#endif
