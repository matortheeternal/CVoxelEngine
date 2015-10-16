#include "ObjectParams.h"

class MandelbulbParams : public ObjectParams {
public:
	float power;
	int itMin;
	int itMax;
	float zoom;
	float zoomSub;
	float fallout;
	MandelbulbParams(int, int, int, int, int, int, float, float, float);
};