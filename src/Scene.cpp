#include "../inc/Scene.h"

Scene::Scene(){
	this->box = NULL;
	this->bat = NULL;
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
	
	return *(this);
}

Scene& Scene::setBat(Bat* b){
	if(this->bat != NULL){
		delete this->bat;
	}
	this->bat = b;
	
	return *(this);
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
			this->balls[0]->move(5.0f);
			this->balls[0]->draw(v,p,this->lights);
		}
	}
	if(this->bat != NULL){
		this->bat->draw(v,p,this->lights);
	}
	for(unsigned int i=0; i < this->blocks.size(); i++){
		if(this->blocks[i] != NULL){
			this->blocks[i]->draw(v,p,this->lights);
		}
	}
	this->checkBallCollision();
	
	return 0;
}

Drawable* Scene::checkBallCollision(){
	if(this->balls[0] == NULL || this->bat == NULL || this->box == NULL){
		return NULL; // Scena niekompletna, więc nie sprawdzamy kolizji
	}
	glm::vec4 ball_center = this->balls[0]->getCenter();
	float radius = this->balls[0]->getRadius();
	
	// Kolizja z paletką
	if(this->checkBallCollision(this->bat, radius, ball_center)){
		return bat;
	}
	// Kolizja ze ścianą
	glm::vec4* wall_coords = this->box->getCoordinates2D();
	if( ball_center[0] - wall_coords[0][0] <= radius || wall_coords[1][0] - ball_center[0] <= radius || 
		wall_coords[0][1] - ball_center[1] <= radius || ball_center[1] - wall_coords[2][1] <= radius ){

		delete wall_coords;
		return this->box;	
	}
	delete wall_coords;
	
	// Sprawdzanie kolizji z blokami
	for(unsigned int i=0; i<this->blocks.size(); i++){
		if(this->checkBallCollision(this->blocks[i], radius, ball_center)){
			return this->blocks[i];		
		}
	}
	
	return NULL;
}

bool Scene::checkBallCollision(Drawable *obj, float radius, glm::vec4& center){
	if(obj == NULL){
		return false;
	}
	float d = 0;
	glm::vec4 *coords = obj->getCoordinates2D();
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
	delete coords;
	if(d <= radius*radius){
		return true;	
	}
	return false;
}
