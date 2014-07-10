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
		Scene(unsigned int max_hits_count = 3);
		~Scene();
		Scene& addLight(struct light *l);
		Scene& addBlock(Block* b);
		Scene& setBat(Bat* b);
		Bat* getBat();
		Scene& addBall(Ball* b);
		Scene& setBox(Drawable* b);
		int run(const glm::mat4& v, const glm::mat4& p, bool started);
		bool checkBallCollision(); // Zwraca wskaźnik do obiektu z którym doszło do kolizji
		bool checkBallCollision(glm::vec4 *coords, float radius, glm::vec4& center);
		Scene& checkBatCollision();
		
	private:
		std::vector<struct light*> lights; // Oświetlenie sceny
		Drawable *box;
		Bat* bat;
		std::vector<Block*> blocks;
		std::vector<Ball*> balls;
		unsigned int max_hits_count;
		std::vector<bool> was_ball_block_collision;
		bool was_ball_wall_collision[4];
		bool was_ball_bat_collision;
};

#endif
