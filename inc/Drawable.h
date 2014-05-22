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
		
	protected: 
		glm::vec3* strToVec3( std::string line );
		void addIndicesFromString( std::string src, std::vector< unsigned short >* dst );
		
		std::vector< glm::vec3* > *vertices;
		std::vector< unsigned short > *indices;
		
	
	public:
		Drawable();
		~Drawable();
		
		Drawable& setVertices(const char *path);
		const std::vector< glm::vec3* >* getVertices();
		const std::vector< unsigned short >* getIndices();
};

#endif
