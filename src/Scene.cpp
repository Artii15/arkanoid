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
