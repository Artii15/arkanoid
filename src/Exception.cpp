#include "../inc/Exception.h"

Exception::Exception(std::string message){
	this->message = message;
}

std::string Exception::getMessage(){
	return this->message;
}
