#include "ObjectParams.h"

class FlakeParams : public ObjectParams {
public:
	int scale;
	FlakeParams(int, int, int, int, int);
};