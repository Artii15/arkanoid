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
#include <GL/freeglut.h>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Exception.h"
#include "vboindexer.hpp"
#include "objloader.hpp"

class Drawable{
	
	private:
		void deleteVertices();
		void deleteUVs();
		void deleteNormals();
		void deleteIndices();
		
	protected:			
		GLuint makeBuffer(void *data, int vertexCount, int vertexSize);
		GLuint makeElementBuffer(void *data, int vertexCount, int vertexSize);
		Drawable& assignVBOtoAttribute(const char* attributeName, GLuint bufVBO, int variableSize);
	
		std::vector< glm::vec4 > *vertices;
		std::vector< glm::vec2 > *uvs;
		std::vector< glm::vec4 > *normals;
		std::vector< unsigned short > *indices;
		GLuint shader_program;
		glm::mat4 model_matrix;
		GLuint vao;
		std::vector< GLuint > VBOs;
		bool shaders_loaded;
		double time; // Tymczasowe, wywalić po testach animacji
		GLuint texture;
	
	public:
		Drawable();
		~Drawable();
		const std::vector< glm::vec4 >* getVertices();
		const std::vector< unsigned short >* getIndices();
		Drawable& loadObj(const char *path);
		Drawable& loadShaders(const char * vertex_file_path,const char * fragment_file_path);
		GLuint getShaderProgram();
		Drawable& draw(const glm::mat4& v, const glm::mat4& p);
		Drawable& loadTexture(const char* path);
};

#endif
