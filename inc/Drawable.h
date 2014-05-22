#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include "Exception.h"

class Drawable{
	
	private:
		void deleteVertices();
		void deleteIndices();
		glm::vec3* getVec3( std::string line, char type );
		std::vector< glm::vec3* >* loadFromFile( const char *path, char type );

	protected: 
		std::vector< glm::vec3* > *vertices;
		std::vector< unsigned int > *indices;
	
	public:
		Drawable();
		~Drawable();
		
		Drawable& setVertices(const char *path);
		const std::vector< glm::vec3* >* getVertices();
};

#endif
