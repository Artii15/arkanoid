#ifndef BALL_H
#define BALL_H

#include "Drawable.h"

class Ball: public Drawable{
	protected:
		glm::vec3 direction;
		float time;
		glm::vec4 center;
		float radius;
		
		Ball& recalculateDimensions();
		
	public:
		Ball();
		Ball& move(float speed = 1.0f);
		virtual Drawable& loadObj(const char *path);
};

#endif
