#ifndef SCENE_H
#define SCENE_H

#include "Bat.h"
#include "Ball.h"
#include "Block.h"
#include "light.h"
#include <vector>
#include "glm/glm.hpp"

class Scene{
	public:
		Scene(short max_hits_count = 3);
		~Scene();
		Scene& addLight(struct light *l);
		Scene& addBlock(Block* b);
		Scene& setBat(Bat* b);
		Bat* getBat();
		Scene& addBall(Ball* b);
		Scene& setBox(Drawable* b);
		int run(const glm::mat4& v, const glm::mat4& p);
		bool checkBallCollision(); // Zwraca wskaźnik do obiektu z którym doszło do kolizji
		bool checkBallCollision(glm::vec4 *coords, float radius, glm::vec4& center);
		
	private:
		std::vector<struct light*> lights; // Oświetlenie sceny
		Drawable *box;
		Bat* bat;
		std::vector<Block*> blocks;
		std::vector<Ball*> balls;
		short max_hits_count;
};

#endif
