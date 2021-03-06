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
		std::string cracked_texture_path;
		
	public:
		Ball();
		virtual ~Ball();
		Ball& move(float speed = 1.0f);
		virtual Drawable& loadObj(const char *path);
		float getRadius(); // W przestrzeni świata
		glm::vec4 getCenter(); // W przestrzeni świata
		Ball& bounce(glm::vec4* rect);
		Ball& setSummaryDirection(glm::vec3 dir); // Oblicza wektor wypadkowy na podstawie swojego wektora i podanego w parametrze
		Ball& bounce(glm::vec3 normal);
		Ball& resetTimer();
};

#endif
