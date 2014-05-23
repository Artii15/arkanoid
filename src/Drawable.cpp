#include "../inc/Drawable.h"

Drawable::Drawable(){
	this->vertices = NULL;
	this->indices = NULL;
	this->texture_vertices = NULL;
	this->vertex_normals = NULL;
}

Drawable::~Drawable(){
	this->deleteVertices();	
	this->deleteIndices();
	this->deleteTextureVertices();
	this->deleteVertexNormals();
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

void Drawable::deleteTextureVertices(){
	if( this->texture_vertices != NULL ){
		for(unsigned int i=0; i < this->texture_vertices->size(); i++){
			if( this->texture_vertices->at(i) != NULL ){
				delete this->texture_vertices->at(i);
			}
		}
		delete this->texture_vertices;
	}
}

void Drawable::deleteVertexNormals(){
	if( this->vertex_normals != NULL ){
		for(unsigned int i=0; i < this->vertex_normals->size(); i++){
			if( this->vertex_normals->at(i) != NULL ){
				delete this->vertex_normals->at(i);
			}
		}
		delete this->vertex_normals;
	}
}

const std::vector< glm::vec4* >* Drawable::getVertices(){
	return this->vertices;	
}

const std::vector< unsigned int >* Drawable::getIndices(){
	return this->indices;
}

Drawable& Drawable::loadObj(const char *path){
	std::ifstream file(path, std::ios::in);
	
	if( !file.is_open() ){
		throw Exception("Nie udało się otworzyć pliku modelu");
	}
	
	this->deleteVertices();
	this->deleteIndices();
	this->deleteTextureVertices();
	this->deleteVertexNormals();
	
	while( file.good() ){
		std::string buf = "";
		getline(file, buf);
	}
	
	file.close();
	return *(this);
}
