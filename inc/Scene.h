#ifndef SCENE_H
#define SCENE_H

#include "Bat.h"
#include "Ball.h"
#include "Block.h"
#include "light.h"
#include <vector>

class Scene{
	public:
		Scene();
		
	private:
		std::vector<struct light*> lights; // Oświetlenie sceny
		Bat* bat;
		std::vector<Block*> blocks;
		std::vector<Ball*> balls;
};

#endif
