#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception{
	private: 
		std::string message;
	public:
		Exception(std::string message);
		std::string getMessage();
};

#endif
