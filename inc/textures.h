#ifndef TEXTURES_H
#define TEXTURES_H
#include <GL/gl.h>

struct textures{
	// Numery tekstur
	GLuint diffuse;
	GLuint ambient;
	GLuint specular;
	
	textures(){
		diffuse = 0;
		ambient = 0;
		specular = 0;
	}
};

#endif
