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
		glm::vec3* getVec3( std::string line );

	protected: 
		std::vector< glm::vec3* > *vertices;
	
	public:
		Drawable();
		~Drawable();
		
		Drawable& setVertices(const char *path);
		const std::vector< glm::vec3* >* getVertices();
};

#endif
