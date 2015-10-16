#include "MandelParams.h"

MandelParams::MandelParams(int size, int xOffset, int yOffset, int zOffset, int itMin, int itMax, float scale, float zoom, float fallout) : 
		ObjectParams(size, xOffset, yOffset, zOffset) {
	this->itMin = itMin;
	this->itMax = itMax;
	this->scale = scale;
	this->zoom = zoom;
	this->zoomSub = zoom / 2.0f;
	this->fallout = fallout;
}