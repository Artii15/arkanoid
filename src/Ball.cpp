#include "../inc/Ball.h"

Ball::Ball(){
	this->direction = glm::vec3(0, -1, 0);
	this->time = glutGet(GLUT_ELAPSED_TIME);
	this->center = glm::vec4(0,0,0,1);
	this->radius = 0;
}

Ball::~Ball(){}

Ball& Ball::move(float speed){
	float current_time = glutGet(GLUT_ELAPSED_TIME);
	float dt = (current_time-this->time)/1000.0f;
	this->time = current_time;
	
	this->model_matrix = glm::translate(this->model_matrix, speed*dt*this->direction);
	
	return *(this);
}

Ball& Ball::recalculateDimensions(){
	glm::vec4 start_point = this->vertices->at(0);
	float diameter = 0;
	glm::vec4 farthest_point = start_point;
	for(unsigned int i=1; i<this->vertices->size(); i++){
		glm::vec4 tmp_vec = this->vertices->at(i)-start_point;
		float tmp_diameter = glm::length(tmp_vec);
		
		if(tmp_diameter > diameter){
			diameter = tmp_diameter;
			farthest_point = this->vertices->at(i);
		}
	}
	this->center = start_point + (farthest_point-start_point)/2.0f;
	this->radius = diameter/2.0f;
	
	return *(this);
}

Drawable& Ball::loadObj(const char *path){
	Drawable::loadObj(path);
	this->recalculateDimensions();
	
	return *(this);
}

float Ball::getRadius(){
	// Zakładając że skalowanie jest zawsze proporcjonalne, dla kuli inne jest bez sensu
	return this->radius*this->model_matrix[0][0];
}
glm::vec4 Ball::getCenter(){
	return	this->model_matrix*this->center;
}
