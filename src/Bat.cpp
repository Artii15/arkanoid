#include "../inc/Bat.h"

Bat::Bat(){
	this->direction = glm::vec3(0,0,0);
	this->time = glutGet(GLUT_ELAPSED_TIME);
}

Bat::~Bat(){}

Bat& Bat::move(){
	float current_time = glutGet(GLUT_ELAPSED_TIME);
	float dt = (current_time-this->time)/1000.0f;
	this->time = current_time;
	
	this->model_matrix = glm::translate(this->model_matrix, dt*this->direction);
	return *(this);
}

Bat& Bat::setDirection(float x){
	this->direction[0] = x;
	
	return *(this);
}

const glm::vec3& Bat::getDirection(){
	return this->direction;
}
