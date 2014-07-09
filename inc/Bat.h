#ifndef BAT_H
#define BAT_H

#include "Drawable.h"

class Bat : public Drawable{
public:
	Bat();
	~Bat();
	Bat& Bat::move(float x);
};

#endif
