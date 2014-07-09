#include "../inc/Ball.h"

Ball::Ball(){
	this->direction = glm::normalize(glm::vec3(0, -1, 0));
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

Ball& Ball::bounce(glm::vec4* rect){
	glm::vec4 center_world = this->model_matrix*this->center;
	glm::vec3 normal;
	
	float distances[4];
	distances[0] = rect[0][0]-center_world[0]; // Od lewej ściany
	distances[1] = center_world[1]-rect[0][1]; // Od górnej ściany
	distances[2] = center_world[0]-rect[1][0]; // Od prawej ściany
	distances[3] = rect[2][1]-center_world[1]; // Od dolnej ściany
	
	short min_index = 0;
	float min = distances[0];
	
	for(short i=1; i<4; i++){
		if(distances[i] >= 0 && (distances[i]<min || min<0)){
			min = distances[i];
			min_index = i;
		}
	}
	switch(min_index){
		case 0:
			normal = glm::vec3(-1,0,0);
			break;
		case 1:
			normal = glm::vec3(0,1,0);
			break;
		case 2:
			normal = glm::vec3(1,0,0);
			break;
		case 3:
			normal = glm::vec3(0,-1,0);
			break;
	}
	if(glm::dot(this->direction, normal) < 0){
		this->direction = glm::reflect(this->direction, normal);
	}
	
	return *(this);
}

Ball& Ball::setSummaryDirection(glm::vec3 dir){
	if(glm::length(dir) > 0){
		this->direction = glm::normalize(this->direction+glm::normalize(dir));
	}
	
	return *(this);
}

Ball& Ball::bounce(glm::vec3 normal){
	this->direction = glm::reflect(this->direction, normal);
	
	return *(this);
}
