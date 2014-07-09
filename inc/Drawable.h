#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <GL/glew.h>
#include <vector>
#include <sstream>
#include <string>
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
#include "tga.h"
#include "vboindexer.hpp"
#include "objloader.hpp"
#include "materials_samplers.h"
#include "textures.h"
#include "light.h"

class Drawable{
	
	private:
		void deleteVertices();
		void deleteUVs();
		void deleteNormals();
		void deleteIndices();
		void deleteTextures();
		
	protected:			
		GLuint makeBuffer(void *data, int vertexCount, int vertexSize);
		GLuint makeElementBuffer(void *data, int vertexCount, int vertexSize);
		Drawable& assignVBOtoAttribute(const char* attributeName, GLuint bufVBO, int variableSize);
	
		std::vector< glm::vec4 > *vertices;
		std::vector< glm::vec2 > *uvs;
		std::vector< glm::vec4 > *normals;
		std::vector< unsigned short > *indices;
		// pierwsze pole tablicy - lewy górny róg, druga - prawy górny, ostatnia - lewy dolny
		// Służy do określania współrzędnych prostokąta otaczającego obiekt w przestrzeni 2D
		// Używane do detekcji kolizji
		glm::vec3 coordinates_2D[4]; 
		GLuint shader_program;
		glm::mat4 model_matrix;
		GLuint vao;
		std::vector< GLuint > VBOs;
		bool shaders_loaded;
		struct textures textures;
		struct materials_samplers samplers;
		GLuint loadTexture(const char* filename, GLuint sampler_nr);
		Drawable& activateTextures();
		Drawable& setTextureUniforms();
		Drawable& setLightUniforms(const std::vector<struct light*>& lights);
		Drawable& recalculateCoordinates2D();
		const glm::vec3* getCoordinates2D(); // Zwrócone w przestrzeni świata
	
	public:
		Drawable();
		virtual ~Drawable();
		const std::vector< glm::vec4 >* getVertices();
		const std::vector< unsigned short >* getIndices();
		virtual Drawable& loadObj(const char *path);
		Drawable& loadShaders(const char * vertex_file_path,const char * fragment_file_path);
		GLuint getShaderProgram();
		Drawable& draw(const glm::mat4& v, const glm::mat4& p, const std::vector<struct light*>& lights);
		Drawable& setAmbientTexture(const char* filename);
		Drawable& setSpecularTexture(const char* filename);
		Drawable& setDiffuseTexture(const char* filename);
		Drawable& setAmbientTexture(GLuint tex, GLuint sampler_nr);
		Drawable& setSpecularTexture(GLuint tex, GLuint sampler_nr);
		Drawable& setDiffuseTexture(GLuint tex, GLuint sampler_nr);
		const struct textures& getTextures();
		const struct materials_samplers& getSamplers();
		const glm::mat4& getModelMatrix();
		Drawable& setModelMatrix(const glm::mat4 &m);
};

#endif
