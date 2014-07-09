#include "../inc/Block.h"

Block::~Block(){}
Block::Block(){
	this->hits_count = 0;
	this->cracked_texture_path = "";
}

Block& Block::setCrackedTexturePath(const char *path){
	this->cracked_texture_path = path;
	
	return *(this);
}

Block& Block::hit(){
	if(this->hits_count == 0 && this->cracked_texture_path != ""){
		this->setDiffuseTexture(this->cracked_texture_path.c_str());
		this->setAmbientTexture(this->getTextures().diffuse, this->getSamplers().diffuse);
		this->setSpecularTexture(this->getTextures().diffuse, this->getSamplers().diffuse);
	}
	
	this->hits_count++;
	
	return *(this);
}

short Block::getHitsCount(){
	return this->hits_count;
}
