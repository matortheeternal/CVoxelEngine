#include "Camera.h"

Camera::Camera(double _x, double _y, double _z, double _rotY, double _rotZ, double _rotX, 
	double _fovH, double _fovV) {
	x = _x;
	y = _y;
	z = _z;
	rotY = _rotY;
	rotZ = _rotZ;
	rotX = _rotX;
	fovH = _fovH;
	fovV = _fovV;
}