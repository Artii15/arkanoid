#ifndef BAT_H
#define BAT_H

#include "Drawable.h"

class Bat : public Drawable{
	protected: 
		glm::vec3 direction;	
	
	public:
		Bat();
		~Bat();
		Bat& move();
		Bat& setDirection(float x);
};

#endif
