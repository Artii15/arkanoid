#include "../inc/Drawable.h"

Drawable::Drawable(){
	this->vertices = NULL;
	this->texture_vertices = NULL;
	this->vertex_normals = NULL;
	this->vertex_indices = NULL;
	this->texture_indices = NULL;
	this->normal_indices = NULL;
}

Drawable::~Drawable(){
	this->deleteVertices();	
	this->deleteTextureVertices();
	this->deleteVertexNormals();
	this->deleteVertexIndices();
	this->deleteNormalIndices();
	this->deleteTextureIndices();
}

void Drawable::deleteVertexIndices(){
	if( this->vertex_indices != NULL ){
		delete this->vertex_indices;
	}
}

void Drawable::deleteTextureIndices(){
	if( this->texture_indices != NULL ){
		delete this->texture_indices;
	}
}

void Drawable::deleteNormalIndices(){
	if( this->normal_indices != NULL ){
		delete this->normal_indices;
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

Drawable& Drawable::loadObj(const char *path){
	std::ifstream file(path, std::ios::in);
	
	if( !file.is_open() ){
		throw Exception("Nie udało się otworzyć pliku modelu");
	}
	
	this->deleteVertices();
	this->deleteTextureVertices();
	this->deleteVertexNormals();
	this->deleteVertexIndices();
	this->deleteTextureIndices();
	this->deleteNormalIndices();
	
	this->vertices = new std::vector< glm::vec4* >();
	this->texture_vertices = new std::vector< glm::vec2* >();
	this->vertex_normals = new std::vector< glm::vec4* >();
	this->vertex_indices = new std::vector< unsigned int >();
	this->texture_indices = new std::vector< unsigned int >();
	this->normal_indices = new std::vector< unsigned int >();
	
	while( file.good() ){
		std::string buf = "";
		getline(file, buf);
		std::string prefix = buf.substr(0, 2);
		if( prefix == "vn" ){
		
			continue;
		}
		if( prefix == "vt" ){
		
			continue;
		}
		if( prefix == "v " ){
		
			continue;
		}
		if( prefix == "f " ){
		
			continue;
		}
	}
	
	file.close();
	return *(this);
}
