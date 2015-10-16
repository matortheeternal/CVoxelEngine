#include "FlakeParams.h"

FlakeParams::FlakeParams(int size, int xOffset, int yOffset, int zOffset, int scale) :
ObjectParams(size, xOffset, yOffset, zOffset) {
	this->scale = scale;
}