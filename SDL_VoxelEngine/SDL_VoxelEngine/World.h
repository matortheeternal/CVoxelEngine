#ifndef AAA_HEADER
#define AAA_HEADER

#include <SDL.h>
#include <map>
#include "Palette.h"

class World
{
private:
	int size, numBlocks;
	Uint8* blocks;
	Palette* palette;
public:
	Uint8 getBlock(int, int, int);
	void setBlock(Uint8, int, int, int);
	void setPalette(Palette*);
	int getSize();
	Uint8* getBlocks();
	SDL_Color getColor(Uint8);
	World(int);
};

#endif