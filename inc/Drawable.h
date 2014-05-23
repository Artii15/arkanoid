#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

#include "Exception.h"

class Drawable{
	
	private:
		void deleteVertices();
		void deleteIndices();
		void deleteTextureVertices();
		void deleteVertexNormals();
		
	protected: 		
		std::vector< glm::vec4* > *vertices;
		std::vector< unsigned int > *indices;
		std::vector< glm::vec2* > *texture_vertices;
		std::vector< glm::vec4* > *vertex_normals;
	
	public:
		Drawable();
		~Drawable();
		
		const std::vector< glm::vec4* >* getVertices();
		const std::vector< unsigned int >* getIndices();
		Drawable& loadObj(const char *path);
};

#endif
