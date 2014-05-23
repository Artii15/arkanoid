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
			this->vertex_normals->push_back( this->strToVec4( buf ) );
			continue;
		}
		if( prefix == "vt" ){
			this->texture_vertices->push_back( this->strToVec2( buf ) );
			continue;
		}
		if( prefix == "v " ){
			this->vertices->push_back( this->strToVec4( buf ) );
			continue;
		}
		if( prefix == "f " ){
			this->fillIndices(buf, this->vertex_indices, this->texture_indices, this->normal_indices); 
			continue;
		}
	}
	
	file.close();
	return *(this);
}

glm::vec4* Drawable::strToVec4( std::string line ){
	float tmp[3];
	
	short count = 0;
	bool reading = false;
	std::string buf = "";
	for(unsigned int i=0; i<line.length(); i++){
		if( ((int)line[i] < 48 || (int)line[i] > 57) && line[i] != '.' && line[i] != '-' ){
			if(reading){
				reading = false;
				tmp[count++] = ::atof( buf.c_str() );
				buf = "";
				if(count > 2){
					break;
				}
			}
			continue;
		}
		reading = true;
		buf += line[i];
	}
	
	return new glm::vec4(tmp[0], tmp[1], tmp[2], 1.0f);
}

glm::vec2* Drawable::strToVec2( std::string line ){
	float tmp[2];
	
	short count = 0;
	bool reading = false;
	std::string buf = "";
	line += "\n";
	for(unsigned int i=0; i<line.length(); i++){
		if( ((int)line[i] < 48 || (int)line[i] > 57) && line[i] != '.' && line[i] != '-' ){
			if(reading){
				reading = false;
				tmp[count++] = ::atof( buf.c_str() );
				buf = "";
				if(count > 1){
					break;
				}
			}
			continue;
		}
		reading = true;
		buf += line[i];
	}
	
	return new glm::vec2(tmp[0], tmp[1]);
}

void Drawable::fillIndices( std::string line, std::vector< unsigned int > *vi, std::vector< unsigned int > *ti, std::vector< unsigned int > *ni ){
	unsigned int i=0;
	unsigned short nr = 0;
	std::string buf = "";
	line += "\n";
	while( i < line.length() ){
		if( ((int)line[i] < 49 || (int)line[i] > 57) ){
			if( line[i] == '/' ){
				if( nr == 0 ){
					vi->push_back( ::atoi( buf.c_str() ) - 1 );
					nr++;
				}
				else{
					ni->push_back( ::atoi( buf.c_str() ) - 1 );
					nr++;
				}
				buf = "";
			}
			else{
				if( nr == 2 ){
					ti->push_back( ::atoi( buf.c_str() ) - 1 );
					nr = 0;
					buf = "";
				}
			}
		}
		else{
			buf += line[i];
		}
		i++;
	}
}
