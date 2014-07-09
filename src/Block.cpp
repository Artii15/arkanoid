#include "../inc/Block.h"

Block::~Block(){}
Block::Block(){
	this->hits_count = 0;
}

Block& Block::addCrackedTexturePath(const char *path){
	this->cracked_texture_paths.push_back(path);
	
	return *(this);
}

Block& Block::hit(){
	this->hits_count++;
	if(this->cracked_texture_paths.size() >= this->hits_count){
		this->setDiffuseTexture(this->cracked_texture_paths[hits_count-1].c_str());
		this->setAmbientTexture(this->getTextures().diffuse, this->getSamplers().diffuse);
		this->setSpecularTexture(this->getTextures().diffuse, this->getSamplers().diffuse);
	}
	
	return *(this);
}

unsigned int Block::getHitsCount(){
	return this->hits_count;
}
