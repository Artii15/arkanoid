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
		void deleteTextureVertices();
		void deleteVertexNormals();
		void deleteVertexIndices();
		void deleteTextureIndices();
		void deleteNormalIndices();
		glm::vec4* strToVec4( std::string line );
		glm::vec2* strToVec2( std::string line );
		void fillIndices( std::string line, std::vector< unsigned int > *vi, std::vector< unsigned int > *ti, std::vector< unsigned int > *ni );
		
	protected: 		
		std::vector< glm::vec4* > *vertices;
		std::vector< glm::vec2* > *texture_vertices;
		std::vector< glm::vec4* > *vertex_normals;
		std::vector< unsigned int > *normal_indices;
		std::vector< unsigned int > *vertex_indices;
		std::vector< unsigned int > *texture_indices;
		
	
	public:
		Drawable();
		~Drawable();
		const std::vector< glm::vec4* >* getVertices();
		Drawable& loadObj(const char *path);
};

#endif
