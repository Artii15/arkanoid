#ifndef BLOCK_H
#define BLOCK_H

#include "Drawable.h"

class Block: public Drawable{
	protected:
		short hits_count;
		std::string cracked_texture_path;
	public:
		virtual ~Block();
		Block();
		short getHitsCount();
		Block& hit();
		Block& setCrackedTexturePath(const char *path);
};

#endif
