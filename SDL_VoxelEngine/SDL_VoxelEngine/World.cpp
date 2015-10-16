#include "World.h"

Uint8 World::getBlock(int x, int y, int z) {
	int index = z*size*size + y*size + x; 
	if ((x >= size) || (x < 0) || (y >= size) || (y < 0) || (z >= size) || (z < 0))
		return (Uint8) 0;
	return (Uint8) blocks[index];
}

void World::setBlock(Uint8 block, int x, int y, int z) {
	int index = z*size*size + y*size + x;
	if ((x >= size) || (x < 0) || (y >= size) || (y < 0) || (z >= size) || (z < 0))
		return;
	blocks[index] = block;
}

void World::setPalette(Palette* p) {
	palette = p;
}

int World::getSize() {
	return size;
}

Uint8* World::getBlocks() {
	return blocks;
}

SDL_Color World::getColor(Uint8 id) {
	return palette->get(id - 1);
}

World::World(int _size) {
	size = _size;
	numBlocks = size*size*size;
	palette = new Palette();
	blocks = new Uint8[numBlocks];
	for (int i = 0; i < numBlocks; i++) {
		blocks[i] = 0;
	}
}