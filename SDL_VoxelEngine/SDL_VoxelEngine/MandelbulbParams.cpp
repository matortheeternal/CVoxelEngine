#include "MandelbulbParams.h"

MandelbulbParams::MandelbulbParams(int size, int xOffset, int yOffset, int zOffset, int itMin, int itMax, float power, float zoom, float fallout) :
ObjectParams(size, xOffset, yOffset, zOffset) {
	this->itMin = itMin;
	this->itMax = itMax;
	this->power = power;
	this->zoom = zoom;
	this->zoomSub = zoom / 2.0f;
	this->fallout = fallout;
}