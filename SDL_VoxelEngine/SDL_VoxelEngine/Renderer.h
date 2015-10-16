#include <SDL.h>
#include "World.h"
#include "Camera.h"
#include <thread>

class Renderer {
private:
	World* world;
	Camera* camera;
	SDL_Color skybox;
public:
	void defaultOptions();
	void renderFrame(Uint8*, int, int, int);
	void Renderer::renderThread(Uint8*, int, int, int, int, float*, int);
	SDL_Color raycastFrom(int, int, int, int, float*);
	SDL_Color raycast(float, float, float, float*);
	SDL_Color calculateColor(SDL_Color, float, float, float, float, float, float);
	bool inBounds(float, float, float);
	Renderer(World*, Camera*);
	float falloff;
	float rate;
	int renderDistance;
};