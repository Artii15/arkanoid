#include "../inc/Bat.h"

Bat::Bat(){

}

Bat::~Bat(){
}
Bat& Bat::move(float x){
	model_matrix = glm::translate(model_matrix, glm::vec3(x, 0.0f, 0.0f));
	return *(this);
}
