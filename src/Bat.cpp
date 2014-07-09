#include "../inc/Bat.h"

Bat::Bat(){
	this->direction = glm::vec3(0,0,0);
}

Bat::~Bat(){}

Bat& Bat::move(){
	this->model_matrix = glm::translate(this->model_matrix, this->direction);
	return *(this);
}

Bat& Bat::setDirection(float x){
	this->direction[0] = x;
	
	return *(this);
}

const glm::vec3& Bat::getDirection(){
	return this->direction;
}
