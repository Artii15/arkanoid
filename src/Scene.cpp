#include "../inc/Scene.h"

Scene::Scene(short max_hits_count){
	this->box = NULL;
	this->bat = NULL;
	this->max_hits_count = max_hits_count;
	this->was_ball_bat_collision = false;
	for(short i=0; i < 4; i++){
		this->was_ball_wall_collision[i] = false;
	}
}

Scene::~Scene(){
	// Usuwanie otoczenia sceny
	if(this->box != NULL){
		delete this->box;
	}
	// Usuwanie paletki
	if(this->bat != NULL){
		delete this->bat;
	}
	// Usuwanie bloków
	for(unsigned int i=0; i<this->blocks.size(); i++){
		if(this->blocks[i] != NULL){
			delete this->blocks[i];
		}
	}
	// Usuwanie świateł
	for(unsigned int i=0; i < this->lights.size(); i++){
		if( this->lights[i] != NULL ){
			delete this->lights[i];
		}
	}
	// Usuwanie piłek
	for(unsigned int i=0; i < this->balls.size(); i++){
		if( this->balls[i] != NULL ){
			delete this->balls[i];
		}
	}
}

Scene& Scene::addBlock(Block* b){
	this->blocks.push_back(b);
	this->was_ball_block_collision.push_back(false);
	return *(this);
}

Scene& Scene::setBat(Bat* b){
	if(this->bat != NULL){
		delete this->bat;
	}
	this->bat = b;
	
	return *(this);
}
Bat* Scene::getBat(){
	return this->bat;
}
Scene& Scene::addBall(Ball* b){
	this->balls.push_back(b);
	
	return *(this);
}

Scene& Scene::setBox(Drawable* b){
	if(this->box != NULL){
		delete this->box;
	}
	this->box = b;
	
	return *(this);
}

Scene& Scene::addLight(struct light *l){
	this->lights.push_back(l);
	return *(this);
}

int Scene::run(const glm::mat4& v, const glm::mat4& p){
	if(this->box != NULL){
		this->box->draw(v,p,this->lights);
	}
	if(this->balls.size() > 0){
		if(this->balls[0] != NULL ){
			this->balls[0]->move(20.0f);
			this->balls[0]->draw(v,p,this->lights);
		}
	}
	if(this->bat != NULL){
		this->bat->move();
		this->bat->draw(v,p,this->lights);
	}
	for(unsigned int i=0; i < this->blocks.size(); i++){
		if(this->blocks[i] != NULL){
			this->blocks[i]->draw(v,p,this->lights);
		}
	}
	this->checkBallCollision();
	this->checkBatCollision(); // Kolizje paletki ze ścianą
	
	return 0;
}

bool Scene::checkBallCollision(){
	if(this->balls[0] == NULL || this->bat == NULL || this->box == NULL){
		return false; // Scena niekompletna, więc nie sprawdzamy kolizji
	}
	glm::vec4 ball_center = this->balls[0]->getCenter();
	float radius = this->balls[0]->getRadius();
	
	// Kolizja z paletką
	glm::vec4* bat_coords = this->bat->getCoordinates2D();
	if(this->checkBallCollision(bat_coords, radius, ball_center)){
		if(!this->was_ball_bat_collision){
			this->balls[0]->setSummaryDirection(this->bat->getDirection());
			this->balls[0]->bounce(bat_coords);
		}
		this->was_ball_bat_collision = true;
		delete bat_coords;
		return true;
	}
	else{
		this->was_ball_bat_collision = false;
		delete bat_coords;
	}
	
	// Kolizja ze ścianą
	glm::vec4* wall_coords = this->box->getCoordinates2D();
	if( ball_center[0] - wall_coords[0][0] <= radius ){
		if(!this->was_ball_wall_collision[0]){
			this->balls[0]->bounce(glm::vec3(1,0,0));
			delete wall_coords;
			this->was_ball_wall_collision[0] = true;
			return true;
		}
	}
	else{
		this->was_ball_wall_collision[0] = false;
	}
	if (wall_coords[1][0] - ball_center[0] <= radius){
		if(!this->was_ball_wall_collision[1]){
			this->balls[0]->bounce(glm::vec3(-1,0,0));
			delete wall_coords;
			this->was_ball_wall_collision[1] = true;
			return true;
		}
	}
	else{
		this->was_ball_wall_collision[1] = false;
	}
	if(wall_coords[0][1] - ball_center[1] <= radius){
		if(!this->was_ball_wall_collision[2]){
			this->balls[0]->bounce(glm::vec3(0,-1,0));
			delete wall_coords;
			this->was_ball_wall_collision[2] = true;
			return true;
		}
	}
	else{
		this->was_ball_wall_collision[2] = false;
	}
	if(ball_center[1] - wall_coords[2][1] <= radius){
		if(!this->was_ball_wall_collision[3]){
			this->balls[0]->bounce(glm::vec3(0,1,0));
			delete wall_coords;
			this->was_ball_wall_collision[3] = true;
			return true;
		}
	}
	else{
		this->was_ball_wall_collision[3] = false;
	}
	delete wall_coords;
	
	// Sprawdzanie kolizji z blokami
	for(unsigned int i=0; i<this->blocks.size(); i++){
		if(this->blocks[i] == NULL){
			continue;
		}
		glm::vec4* block_coords = this->blocks[i]->getCoordinates2D();
		if(this->checkBallCollision(block_coords, radius, ball_center)){
			if(!this->was_ball_block_collision[i]){
				this->balls[0]->bounce(block_coords);
				if(this->blocks[i]->hit().getHitsCount() >= this->max_hits_count){
					Block* tmp = this->blocks[i];
					this->blocks[i] = this->blocks[this->blocks.size()-1];
					delete tmp;
					this->blocks.pop_back();

					this->was_ball_block_collision[i] = this->was_ball_block_collision[this->was_ball_block_collision.size()-1];
					this->was_ball_block_collision.pop_back();
				}
			}
			delete block_coords;
			this->was_ball_block_collision[i] = true;
			return true;		
		}
		else{
			this->was_ball_block_collision[i] = false;
			delete block_coords;
		}
	}
	
	return false;;
}

bool Scene::checkBallCollision(glm::vec4 *coords, float radius, glm::vec4& center){
	float d = 0;
	for(short i=0; i<2; i++){
		if(center[i] < coords[3][i]){
			float e = center[i] - coords[3][i];
			d += e*e;
		}
		else if(center[i] > coords[1][i]){
			float e = center[i] - coords[1][i];
			d += e*e;
		}
	}
	if(d <= radius*radius){
		return true;	
	}
	return false;
}

Scene& Scene::checkBatCollision(){
	glm::vec4* bat_coords = this->bat->getCoordinates2D();
	glm::vec4* wall_coords = this->box->getCoordinates2D();
	
	if(bat_coords[0][0] <= wall_coords[0][0]){
		float diff = wall_coords[0][0] - bat_coords[0][0];
		this->bat->setModelMatrix(glm::translate(this->bat->getModelMatrix(), glm::vec3(diff,0,0)));
		this->bat->setDirection(1);
	}
	else if(bat_coords[1][0] >= wall_coords[1][0]){
		float diff = wall_coords[1][0] - bat_coords[1][0];
		this->bat->setDirection(1);
		this->bat->setModelMatrix(glm::translate(this->bat->getModelMatrix(), glm::vec3(diff,0,0)));
		this->bat->setDirection(-1);
	}
	
	delete bat_coords;
	delete wall_coords;
	
	return *(this);
}
