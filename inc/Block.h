#ifndef BLOCK_H
#define BLOCK_H

#include "Drawable.h"

class Block: public Drawable{
	protected:
		unsigned int hits_count;
		std::vector<std::string> cracked_texture_paths;
	public:
		virtual ~Block();
		Block();
		unsigned int getHitsCount();
		Block& hit();
		Block& addCrackedTexturePath(const char *path);
};

#endif
