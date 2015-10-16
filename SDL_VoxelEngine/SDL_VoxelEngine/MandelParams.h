#include "ObjectParams.h"

class MandelParams : public ObjectParams {
public:
	float scale;
	int itMin;
	int itMax;
	float zoom;
	float zoomSub;
	float fallout;
	MandelParams(int, int, int, int, int, int, float, float, float);
};