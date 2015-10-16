#include "ObjectGenerator.h"

const int MAX_THREADS = 4;
int layersGenerated = 0;

void ObjectGenerator::generateMengerSponge(ObjectParams* p) {
	// place block and return if dimension is 1
	if (p->size == 1) {
		world->setBlock(1, p->xOffset, p->yOffset, p->zOffset);
		return;
	}

	// 1/3 of dimension
	int d1 = p->size / 3;

	// recursion loop
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			if ((y == 1) && (x == 1))
				continue;
			for (int z = 0; z < 3; z++) {
				if (((y == 1) && (z == 1)) || ((x == 1) && (z == 1)))
					continue;
				// recursion
				if (p->size > 3) {
					generateMengerSponge(new ObjectParams(d1, x * d1 + p->xOffset, y * d1 + p->yOffset, z * d1 + p->zOffset));
				} 
				else {
					world->setBlock(1, x + p->xOffset, y + p->yOffset, z + p->zOffset);
				}
			}
		}
	}
}

void ObjectGenerator::generateCantorDust(ObjectParams* p) {
	// place block and return if size is 1
	if (p->size == 1) {
		world->setBlock(1, p->xOffset, p->yOffset, p->zOffset);
		return;
	}

	// 1/3 of size
	int d1 = p->size / 3;

	// recursion loop
	for (int x = 0; x < 3; x+=2)
		for (int y = 0; y < 3; y+=2)
			for (int z = 0; z < 3; z+=2)
				generateCantorDust(new ObjectParams(d1, x * d1 + p->xOffset, y * d1 + p->yOffset, z * d1 + p->zOffset));
}

void ObjectGenerator::octahedron(FlakeParams* p) {
	int width = 0;
	int d1 = (p->size - 1) / 2;
	for (int y = 0; y < p->size; y++) {
		for (int z = 0; z < p->size; z++) {
			for (int x = 0; x < p->size; x++) {
				if (abs(z - d1) + abs(x - d1) <= width) {
					world->setBlock(1, x + p->xOffset, y + p->yOffset, z + p->zOffset);
				}
			}
		}
		if (y > d1 - 1)
			width--;
		else
			width++;
	}
}

void ObjectGenerator::generateOctFlake(FlakeParams* p) {
	int d1 = (p->size - 1) / 2;
	int d2 = (d1 - 1) / 2;
	if (d1 < p->scale) {
		octahedron(p);
	}
	else {
		int d2x = d2 + 1 + p->xOffset;
		int d2y = d2 + 1 + p->yOffset;
		int d2z = d2 + 1 + p->zOffset;
		generateOctFlake(new FlakeParams(d1, p->xOffset, d2y, d2z, p->scale));
		generateOctFlake(new FlakeParams(d1, d2x, p->yOffset, d2z, p->scale));
		generateOctFlake(new FlakeParams(d1, d2x, d2y, p->zOffset, p->scale));
		generateOctFlake(new FlakeParams(d1, d2x, d1 + 1 + p->yOffset, d2z, p->scale));
		generateOctFlake(new FlakeParams(d1, d2x, d2y, d1 + 1 + p->zOffset, p->scale));
		generateOctFlake(new FlakeParams(d1, d1 + 1 + p->xOffset, d2y, d2z, p->scale));
	}
}

void ObjectGenerator::generateMandelbox(MandelParams* p) {
	// define vector components
	float xc;
	float yc;
	float zc;
  
	// define other needed variables
	float falloutSquared = p->fallout * p->fallout;
	int iterations;
	struct Vector3f base; 
	struct Vector3f current;

	// iterate for all x,y,z
	for (int x = 0; x < p->size; x++) {
		printf("%d/%d layers done\n", x, p->size);
		xc = (p->zoom * x) / (p->size - 1) - p->zoomSub;
		for (int y = 0; y < p->size; y++) {
			yc = (p->zoom * y) / (p->size - 1) - p->zoomSub;
			for (int z = 0; z < p->size; z++) {
				zc = (p->zoom * z) / (p->size - 1) - p->zoomSub;
        
				// reset iterations, set base vector, reset current vector
				iterations = -1;
				base = { xc, yc, zc };
				current = { 0, 0, 0 };

				// iterate until fallout or maximum iterations
				while ((mag(current) < falloutSquared) && (iterations < p->itMax)) {
					current = add(mult(p->scale, boxformula(current)), base);
					iterations++;
				}

				// place block if fallout reached in itMin -> itMax range
				if ((iterations >= p->itMin) && (iterations < p->itMax)) {
					Uint8 id = iterations - p->itMin;
					world->setBlock(id, p->xOffset + x, p->yOffset + y, p->zOffset + z);
				}
			}
		}
	}
}

void ObjectGenerator::generateMandelbulb(MandelParams* p) {
	// define vector components
	float xc;
	float yc;
	float zc;

	// define other needed variables
	float falloutSquared = p->fallout * p->fallout;
	int iterations;
	struct Vector3f base;
	struct Vector3f current;
	
	// iterate for all x,y,z
	for (int x = 0; x < p->size; x++) {
		printf("%d/%d layers done\n", x, p->size);
		xc = (p->zoom * x) / (p->size - 1) - p->zoomSub;
		for (int y = 0; y < p->size; y++) {
			yc = (p->zoom * y) / (p->size - 1) - p->zoomSub;
			for (int z = 0; z < p->size; z++) {
				zc = (p->zoom * z) / (p->size - 1) - p->zoomSub;

				// reset iterations, set base vector, reset current vector
				iterations = -1;
				base = { xc, yc, zc };
				current = { 0, 0, 0 };

				// iterate until fallout or maximum iterations
				while ((mag(current) < falloutSquared) && (iterations < p->itMax)) {
					current = add(bulbformula(current, p->scale), base);
					iterations++;
				}

				// place block if fallout reached in itMin -> itMax range
				if ((iterations >= p->itMin) && (iterations < p->itMax)) {
					Uint8 id = iterations - p->itMin;
					world->setBlock(id, p->xOffset + x, p->yOffset + y, p->zOffset + z);
				}
			}
		}
	}
}

inline void ObjectGenerator::mandelbulbThread(int x1, int x2, MandelParams p) {
	// define vector components
	float xc;
	float yc;
	float zc;

	// define other needed variables
	int iterations;
	struct Vector3f base;
	struct Vector3f current;

	// iterate for all x,y,z
	for (int x = x1; x < x2; x++) {
		xc = (p.zoom * x) / (p.size - 1) - p.zoomSub;
		for (int y = 0; y < p.size; y++) {
			yc = (p.zoom * y) / (p.size - 1) - p.zoomSub;
			for (int z = 0; z < p.size; z++) {
				zc = (p.zoom * z) / (p.size - 1) - p.zoomSub;

				// reset iterations, set base vector, reset current vector
				iterations = -1;
				base = { xc, yc, zc };
				current = { 0, 0, 0 };

				// iterate until fallout or maximum iterations
				while ((mag(current) < p.fallout) && (iterations < p.itMax)) {
					current = add(bulbformula(current, p.scale), base);
					iterations++;
				}

				// place block if fallout reached in itMin -> itMax range
				if ((iterations >= p.itMin) && (iterations < p.itMax)) {
					Uint8 id = iterations - p.itMin;
					world->setBlock(id, p.xOffset + x, p.yOffset + y, p.zOffset + z);
				}
			}
		}
		layersGenerated++;
		printf("Layers generated %d/%d...\n", layersGenerated, p.size);
	}
}

void ObjectGenerator::mtGenerateMandelbulb(MandelParams* p) {
	thread* array = new thread[MAX_THREADS];

	// start up to MAX_THREADS threads
	int x = 0;
	int cx = p->size / MAX_THREADS;
	for (int t = 0; t < MAX_THREADS; t++) {
		array[t] = thread(&ObjectGenerator::mandelbulbThread, this, x, x + cx, *p);
		x += cx;
	}

	// start thread for final bit of work
	if (x != p->size) {
		mandelbulbThread(x, p->size, *p);
	}

	// join threads
	for (int t = 0; t < MAX_THREADS; t++) {
		array[t].join();
	}
}

inline void ObjectGenerator::mandelboxThread(int x1, int x2, MandelParams p) {
	// define vector components
	float xc;
	float yc;
	float zc;
  
	// define other needed variables
	int iterations;
	struct Vector3f base; 
	struct Vector3f current;
  
	 // iterate for all x,y,z
	for (int x = x1; x < x2; x++) {
		xc = (p.zoom * x) / (p.size - 1) - p.zoomSub;
		for (int y = 0; y < p.size; y++) {
			yc = (p.zoom * y) / (p.size - 1) - p.zoomSub;
			for (int z = 0; z < p.size; z++) {
				zc = (p.zoom * z) / (p.size - 1) - p.zoomSub;

				// reset iterations, set base vector, reset current vector
				iterations = -1;
				base = { xc, yc, zc };
				current = { 0, 0, 0 };

				// iterate until fallout or maximum iterations
				while ((mag(current) < p.fallout) && (iterations < p.itMax)) {
					current = add(mult(p.scale, boxformula(current)), base);
					iterations++;
				}

				// place block if fallout reached in itMin -> itMax range
				if ((iterations >= p.itMin) && (iterations < p.itMax)) {
					Uint8 id = iterations - p.itMin;
					world->setBlock(id, p.xOffset + x, p.yOffset + y, p.zOffset + z);
				}
			}
		}
		layersGenerated++;
		printf("Layers generated %d/%d...\n", layersGenerated, p.size);
	}
}

void ObjectGenerator::mtGenerateMandelbox(MandelParams* p) {
	thread* array = new thread[MAX_THREADS];

	// start up to MAX_THREADS threads
	int x = 0;
	int cx = p->size / MAX_THREADS;
	for (int t = 0; t < MAX_THREADS; t++) {
		array[t] = thread(&ObjectGenerator::mandelboxThread, this, x, x + cx, *p);
		x += cx;
	}
	
	// start thread for final bit of work
	if (x != p->size) {
		mandelboxThread(x, p->size, *p);
	}

	// join threads
	for (int t = 0; t < MAX_THREADS; t++) {
		array[t].join();
	}
}

inline float ObjectGenerator::mag(Vector3f vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline Vector3f ObjectGenerator::add(Vector3f vec1, Vector3f vec2) {
	struct Vector3f r;
	r.x = vec1.x + vec2.x;
	r.y = vec1.y + vec2.y;
	r.z = vec1.z + vec2.z;
	return r;
}

inline Vector3f ObjectGenerator::mult(float f, Vector3f vec) {
	struct Vector3f r;
	r.x = f * vec.x;
	r.y = f * vec.y;
	r.z = f * vec.z;
	return r;
}

inline Vector3f ObjectGenerator::bulbformula(Vector3f vec, float n) {
	float t = theta(vec);
	float p = phi(vec);
	float k = pow(mag(vec), n);
	return Vector3f { k * sin(n * t) * cos(n * p), k * sin(n * t) * sin(n * p), k * cos(n * t) };
}

inline float ObjectGenerator::theta(Vector3f vec) {
	return (acos(vec.z / (mag(vec) + 0.000000001f)));
}

inline float ObjectGenerator::phi(Vector3f vec) {
	return (atan(vec.y / (vec.x + 0.000000001f)));
}

inline Vector3f ObjectGenerator::boxformula(Vector3f vec) {
	float x = vec.x;
	float y = vec.y;
	float z = vec.z;

	if (x > 1)
		x = 2 - x;
	else if (x < -1)
		x = -2 - x;

	if (y > 1)
		y = 2 - y;
	else if (y < -1)
		y = -2 - y;

	if (z > 1)
		z = 2 - z;
	else if (z < -1)
		z = -2 - z;

	struct Vector3f output = { x, y, z };
	float m = mag(output);

	if (m < 0.5)
		output = mult(4.0f, output);
	else if (m < 1.0f)
		output = mult(1.0f / (m * m), output);

	return output;
}

void ObjectGenerator::generateGradientCube(ObjectParams* p) {
	for (int x = 0; x < p->size; x++) {
		for (int y = 0; y < p->size; y++) {
			for (int z = 0; z < p->size; z++) {
				int block = x + y + z;
				world->setBlock(block, x + p->xOffset, y + p->yOffset, z + p->zOffset);
			}
		}
	}
}

void ObjectGenerator::generateFlatCube(ObjectParams* p) {
	for (int x = 0; x < p->size; x++) {
		for (int y = 0; y < p->size; y++) {
			for (int z = 0; z < p->size; z++) {
				int block = 1;
				(x > p->size / 2) ? block++ : 0;
				(y > p->size / 2) ? block++ : 0;
				(z > p->size / 2) ? block++ : 0;
				world->setBlock(block, x + p->xOffset, y + p->yOffset, z + p->zOffset);
			}
		}
	}
}

ObjectGenerator::ObjectGenerator(World* _world) {
	world = _world;
}