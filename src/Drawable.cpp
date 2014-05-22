#include "../inc/Drawable.h"
#include <iostream>

Drawable::Drawable(){
	this->vertices = new std::vector< glm::vec3* >();
}

Drawable::~Drawable(){
	this->deleteVertices();	
}

void Drawable::deleteVertices(){
	if( this->vertices != NULL ){
		for(unsigned int i=0; i < this->vertices->size(); i++){
			if( this->vertices->at(i) != NULL ){
				delete this->vertices->at(i);
			}
		}
		delete this->vertices;
	}
}

Drawable& Drawable::setVertices(const char *path){
	std::ifstream file(path, std::ios::in);	
	if( !file.is_open() ){
		Exception ex( "Nie udało się otworzyć pliku z modelem" );
		throw ex;
	}
	
	this->deleteVertices();
	this->vertices = new std::vector< glm::vec3* >();
	
	std::string buf;
	while( file.good() ){
		buf = "";
		getline( file, buf );
		if( buf[0] != 'v' ){
			continue;
		}

		this->vertices->push_back( this->getVec3( buf ) );
	}
	file.close();
	return *(this);
}

const std::vector< glm::vec3* >* Drawable::getVertices(){
	return this->vertices;	
}

glm::vec3* Drawable::getVec3( std::string line ){
	unsigned short int count = -1;
	bool reading = false;
	float tmp[3];
	std::string buf = "";
	if ( line[line.length()-1] != '\n' ){
		line += '\n';
	}
	for( unsigned int i=0; i < line.length(); i++ ){
		if( line[i] == 'v' || line[i] == ' ' || line[i] == '\n' ){
			if( reading ){
				count++;
				tmp[count] = ::atof( buf.c_str() );
				buf = "";
				reading = false;
				if( count >= 2 ){
					break;
				}
				
			}
			continue;
		}
		reading = true;
		buf += line[i];
	}
		
	return new glm::vec3(tmp[0], tmp[1], tmp[2]);
}
