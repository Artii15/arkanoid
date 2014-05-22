#include "../inc/Drawable.h"
#include <iostream>

Drawable::Drawable(){
	this->vertices = NULL;
	this->indices = NULL;
}

Drawable::~Drawable(){
	this->deleteVertices();	
	this->deleteIndices();
}

void Drawable::deleteIndices(){
	if( this->indices != NULL ){
		delete this->indices;
	}
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
	this->deleteIndices();
	
	this->vertices = new std::vector< glm::vec3* >();
	this->indices = new std::vector< unsigned short >();
	
	while( file.good() ){
		std::string buf = "";
		getline( file, buf );
		if( buf[0] == 'v' ){
			vertices->push_back( this->strToVec3( buf ) );
			continue;
		}
		if( buf[0] == 'f' ){
			this->addIndicesFromString( buf, indices );
		}
	}
	file.close();
	
	return *(this);
}

const std::vector< glm::vec3* >* Drawable::getVertices(){
	return this->vertices;	
}

glm::vec3* Drawable::strToVec3( std::string line ){
	unsigned short int count = -1;
	bool reading = false;
	float tmp[3];
	std::string buf = "";
	if ( line[line.length()-1] != '\n' ){
		line += '\n';
	}
	for( unsigned int i=0; i < line.length(); i++ ){
		if( ((int)line[i] < 48 || (int)line[i] > 57) && line[i] != '.' && line[i] != '-' ){
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

void Drawable::addIndicesFromString( std::string src, std::vector< unsigned short >* dst ){
	bool reading = false;
	std::string buf = "";
	if ( src[src.length()-1] != '\n' ){
		src += '\n';
	}
	for( unsigned int i=0; i < src.length(); i++ ){
		if( (int)src[i] < 48 || (int)src[i] > 57 ){
			if( reading ){
				dst->push_back( ::atoi( buf.c_str() ) );
				buf = "";
				reading = false;				
			}
			continue;
		}
		reading = true;
		buf += src[i];
	}
}

const std::vector< unsigned short >* Drawable::getIndices(){
	return this->indices;
}
