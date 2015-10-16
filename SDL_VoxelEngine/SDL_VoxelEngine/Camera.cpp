#include "Camera.h"

Camera::Camera(float _x, float _y, float _z, float _rotY, float _rotZ, float _rotX, 
	float _fovH, float _fovV) {
	x = _x;
	y = _y;
	z = _z;
	rotY = _rotY;
	rotZ = _rotZ;
	rotX = _rotX;
	fovH = _fovH;
	fovV = _fovV;
}