#include <SDL.h>
#include <thread>
#include <ctime>
#include "World.h"
#include "MandelParams.h"
#include "FlakeParams.h"

struct Vector3f {
  float x, y, z;
};

class ObjectGenerator {
private:
	float mag(Vector3f);
	World* world;
public:
	void generateMengerSponge(ObjectParams*);
	void generateGradientCube(ObjectParams*);
	void generateFlatCube(ObjectParams*);
	void generateMandelbox(MandelParams*);
	void generateCantorDust(ObjectParams*);
	void octahedron(FlakeParams*);
	void generateOctFlake(FlakeParams*);
	void generateMandelbulb(MandelParams*);
	void mtGenerateMandelbulb(MandelParams*);
	inline void mandelbulbThread(int, int, MandelParams);
	void mtGenerateMandelbox(MandelParams*);
	inline void mandelboxThread(int, int, MandelParams);
	inline Vector3f add(Vector3f, Vector3f);
	inline Vector3f mult(float, Vector3f);
	inline Vector3f boxformula(Vector3f);
	inline Vector3f bulbformula(Vector3f, float);
	inline float phi(Vector3f);
	inline float theta(Vector3f);
	ObjectGenerator(World*);
};