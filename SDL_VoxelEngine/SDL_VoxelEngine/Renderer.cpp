#include "Renderer.h"
#include <math.h>

SDL_Color calculateColor(SDL_Color, float, float, float, float, float, float);

const int MAX_THREADS = 4;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

template <typename T> T max(T a, T b)
{
	return a > b ? a : b;
}

void Renderer::defaultOptions() {
	falloff = 4.0f;
	rate = 1.0f;
	ambientIntensity = 0.2f;
	directionalIntensity = 0.3f;
	renderDistance = 100;

}

void Renderer::renderThread(Uint8* pixels, int y1, int y2, int width, int height, float* ref, int pixelScale) {
	SDL_Color pixel;
	int p = 0;
	for (int py = y1; py < y2; py += pixelScale) {
		for (int px = 0; px < width; px += pixelScale) {
			pixel = raycastFrom(px, py, width, height, ref);
			if (pixelScale == 1) {
				p = 3 * (py*width + px);
				pixels[p + 0] = pixel.b;
				pixels[p + 1] = pixel.g;
				pixels[p + 2] = pixel.r;
			}
			else {
				for (int pys = 0; pys < pixelScale; pys++) {
					for (int pxs = 0; pxs < pixelScale; pxs++) {
						p = 3 * ((py + pys) * width + px + pxs);
						pixels[p + 0] = pixel.b;
						pixels[p + 1] = pixel.g;
						pixels[p + 2] = pixel.r;
					}
				}
			}
		}
	}
}

void Renderer::renderFrame(Uint8* pixels, int width, int height, int pixelScale) {
	//Set up reference frame
	double yaw = camera->rotY;
	double pitch = camera->rotX;
	float ref[9] = {
		 sin(pitch) * cos(yaw), sin(pitch) * sin(yaw), -cos(pitch),
		 -sin(yaw), cos(yaw), 0,
		 cos(pitch) * cos(yaw), cos(pitch) * sin(yaw), 2 * sin(pitch) 
	};

	//Raycast for each pixel
	thread* array = new thread[MAX_THREADS];

	// start up to MAX_THREADS threads
	int y = 0;
	int cy = height / MAX_THREADS;
	for (int t = 0; t < MAX_THREADS; t++) {
		array[t] = thread(&Renderer::renderThread, this, pixels, y, y + cy, width, height, ref, pixelScale);
		y += cy;
	}

	// start thread for final bit of work
	if (y != height) {
		renderThread(pixels, y, height, width, height, ref, pixelScale);
	}

	// join threads
	for (int t = 0; t < MAX_THREADS; t++) {
		array[t].join();
	}
}

SDL_Color Renderer::raycastFrom(int px, int py, int width, int height, float* ref) {
	float w2 = width / 2.0f;
	float h2 = height / 2.0f;

	float x = camera->x;
	float y = camera->y;
	float z = camera->z;

	float fovH = camera->fovH / 2;
	float fovV = camera->fovV / 2;
	float yawr = ((px - w2) / w2) * fovH;
	float pitchr = ((py - h2) / h2) * fovV; // correction because view window isn't 1:1

	float relative[3] = { cos(pitchr) * cos(yawr), cos(pitchr) * sin(yawr), -sin(pitchr) };
	float ray[3] = { relative[0] * ref[0] + relative[1] * ref[3] + relative[2] * ref[6],
		relative[0] * ref[1] + relative[1] * ref[4] + relative[2] * ref[7],
		relative[0] * ref[2] + relative[1] * ref[5] + relative[2] * ref[8] };

	return raycast(x, y, z, ray);
}

SDL_Color Renderer::raycast(float x, float y, float z, float* ray) {
	float startX = x;
	float startY = y;
	float startZ = z;

	float dx = ray[0] * renderDistance;
	float dy = ray[1] * renderDistance;
	float dz = ray[2] * renderDistance;

	float rayMagnitude = sqrt(dx * dx + dy * dy + dz * dz);

	float ax, ay, az;

	int sx, sy, sz, n;

	float sv = numeric_limits<float>::min();

	sx = (int) sgn(dx);
	sy = (int) sgn(dy);
	sz = (int) sgn(dz);

	ax = abs(dx) / rayMagnitude;
	ay = abs(dy) / rayMagnitude;
	az = abs(dz) / rayMagnitude;

	ax = ((ax > sv) ? ax : sv);
	ay = ((ay > sv) ? ay : sv);
	az = ((az > sv) ? az : sv);

	float tDeltaX = 1 / ax;
	float tDeltaY = 1 / ay;
	float tDeltaZ = 1 / az;

	float tMaxX = (float) abs((sx == 1) ? (1 - (fmod(x, 1.0f))) : (fmod(x, 1.0f))) / ax;
	float tMaxY = (float) abs((sy == 1) ? (1 - (fmod(y, 1.0f))) : (fmod(y, 1.0f))) / ay;
	float tMaxZ = (float) abs((sz == 1) ? (1 - (fmod(z, 1.0f))) : (fmod(z, 1.0f))) / az;

	n = (int) (abs(dx) + abs(dy) + abs(dz));

	int face = -1;

	while (n-- != 0) {
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				face = 0;
				x += sx;
				tMaxX += tDeltaX;
			}
			else {
				face = 2;
				z += sz;
				tMaxZ += tDeltaZ;
			}
		}
		else {
			if (tMaxY < tMaxZ) {
				face = 1;
				y += sy;
				tMaxY += tDeltaY;
			}
			else {
				face = 2;
				z += sz;
				tMaxZ += tDeltaZ;
			}
		}
		Uint8 block = world->getBlock((int) x, (int) y, (int) z);
		if (block != 0) {
			SDL_Color c = world->getColor(block);
			return calculateColor(c, face, startX, x, startY, y, startZ, z);
		}
	}

	return skybox;
}

inline bool Renderer::inBounds(float x, float y, float z) {
	int b = world->getSize();
	int dx = ((int)abs(camera->x - x));
	int dy = ((int)abs(camera->y - y));
	int dz = ((int)abs(camera->z - z));
	return x >= 0.0f && x < b && y >= 0.0f && y < b && z >= 0.0f && z < b && dx < 50 && dy < 50 && dz < 50;
}

Renderer::Renderer(World* _world, Camera* _camera) {
	world = _world;
	camera = _camera;
	skybox.b = 0;
	skybox.g = 0;
	skybox.r = 0;
}

SDL_Color Renderer::calculateColor(SDL_Color c, int face, float x1, float x2, float y1, 
	float y2, float z1, float z2) {
	float ray[] = { x1 - x2, y1 - y2, z1 - z2 };
	float distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
	float dot = abs(ray[face]);

	// compute light intensity from ambient, directional, and diffuse light intensities
	float diffuseIntensity = (falloff / pow(distance, rate)) * dot;
	float lightIntensity = ambientIntensity + (1 - ambientIntensity - directionalIntensity) * diffuseIntensity;

	// calculate color components
	int red = (int) (c.r * lightIntensity);
	red = (red > 255) ? 255 : red;
	int green = (int) (c.g * lightIntensity);
	green = (green > 255) ? 255 : green;
	int blue = (int) (c.b * lightIntensity);
	blue = (blue > 255) ? 255 : blue;
	c.r = red;
	c.b = blue;
	c.g = green;

	// return calculated color
	return c;
}