#include "../inc/Ball.h"

Ball::Ball(){
	this->direction = glm::vec3(0, -1, 0);
	this->time = glutGet(GLUT_ELAPSED_TIME);
}

Ball& Ball::move(float speed){
	float current_time = glutGet(GLUT_ELAPSED_TIME);
	float dt = (current_time-this->time)/1000.0f;
	this->time = current_time;
	
	this->model_matrix = glm::translate(this->model_matrix, speed*dt*this->direction);
	
	return *(this);
}
