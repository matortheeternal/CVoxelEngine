// Using SDL and standard IO
#include <SDL.h>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Renderer.h"
#include "ObjectGenerator.h"

// Frame constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const float pi = 3.14159f;
int BPP = 4;

// SDL functions
bool init();
bool loadMedia();
void close();

// Rendering functions
void generateFrame();
void renderFrame();

// SDL variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gSurface = NULL;
SDL_Event event;

// Voxel Engine variables
int numRandomPalettes = 10;
int minRandomPaletteSize = 4;
int maxRandomPaletteSize = 16;
Renderer* renderer;
World* world;
Camera* camera;
ObjectGenerator* generator;
ColorLibrary* library;
float speed = 1.0f;
Palette* palette;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool init() {
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create window
		gWindow = SDL_CreateWindow("Voxel Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

void close() {
	// Deallocate surface
	SDL_FreeSurface(gSurface);
	gSurface = NULL;

	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

void captureImage(int width, int height) {
	SDL_Surface* imageSurface = new SDL_Surface();
	imageSurface = SDL_CreateRGBSurface(0, width, height, 24, 0, 0, 0, 0);
	if (imageSurface != NULL) {
		// Get the pixel array for the surface
		Uint8 *pixels = (Uint8 *)imageSurface->pixels;
		renderer->renderFrame(pixels, width, height, 1);
		int rn = rand() % 1000000;
		string filename = "image-" + std::to_string(rn) + ".bmp";
		SDL_SaveBMP(imageSurface, filename.c_str());
	}
}

void generateFrame() {
	// If the surface must be locked
	if (SDL_MUSTLOCK(gSurface))
	{
		// Lock the surface
		SDL_LockSurface(gSurface);
	}

	// Get the pixel array for the surface
	Uint8 *pixels = (Uint8 *) gSurface->pixels;
	renderer->renderFrame(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, BPP);
	//renderer->renderFrame(gSurface, 1);

	// Unlock surface
	if (SDL_MUSTLOCK(gSurface))
	{
		SDL_UnlockSurface(gSurface);
	}
}

void renderFrame() {
	// Generate the frame
	generateFrame();

	// Apply the image
	SDL_BlitSurface(gSurface, NULL, gScreenSurface, NULL);

	// Update the surface
	SDL_UpdateWindowSurface(gWindow);
}

void printCameraCoords(){
	printf("Camera (%f, %f, %f) (%f, %f, %f)\n", 
		camera->x, camera->y, camera->z, camera->rotX, camera->rotY, camera->rotZ);
}

void printLightingInfo() {
	printf("Render Distance = %d, Rate = %f, Falloff =  %f\n", renderer->renderDistance,
		renderer->rate, renderer->falloff);
}

void printColor(SDL_Color c) {
	printf("%d,%d,%d\n", c.r, c.g, c.b);
}

void addColors() {
	// basic monochrome
	library->addColor("Black", SDL_Color{ 0, 0, 0 });
	library->addColor("Gray", SDL_Color{ 128, 128, 128 });
	library->addColor("White", SDL_Color{ 255, 255, 255 });
	// primary colors
	library->addColor("Red", SDL_Color{ 255, 0, 0 });
	library->addColor("Green", SDL_Color{ 0, 255, 0 });
	library->addColor("Blue", SDL_Color{ 0, 0, 255 });
	// secondary colors
	library->addColor("Yellow", SDL_Color{ 256, 256, 0 });
	library->addColor("Cyan", SDL_Color{ 0, 256, 256 });
	library->addColor("Magenta", SDL_Color{ 256, 0, 256 });
	// tertiary colors
	library->addColor("Olive", SDL_Color{ 128, 128, 0 });
	library->addColor("Teal", SDL_Color{ 0, 128, 128 });
	library->addColor("Purple", SDL_Color{ 128, 0, 128 });
	// quaternary colors
	library->addColor("Orange", SDL_Color{ 256, 128, 0 });
	library->addColor("Pnik", SDL_Color{ 256, 0, 128 });
	library->addColor("Bright Green", SDL_Color{ 0, 256, 128 });
	library->addColor("Aqua", SDL_Color{ 128, 256, 0 });
	library->addColor("Sky Blue", SDL_Color{ 0, 128, 256 });
	library->addColor("Lavender", SDL_Color{ 128, 0, 256 });
	// monochromatic shades
	library->addColor("Black Gray", SDL_Color{ 32, 32, 32 });
	library->addColor("Dark Gray", SDL_Color{ 64, 64, 64 });
	library->addColor("Low Gray", SDL_Color{ 96, 96, 96 });
	library->addColor("High Gray", SDL_Color{ 160, 160, 160 });
	library->addColor("Light Gray", SDL_Color{ 192, 192, 192 });
	library->addColor("White Gray", SDL_Color{ 224, 224, 224 });
	// red shades
	library->addColor("Black Red", SDL_Color{ 64, 0, 0 });
	library->addColor("Dark Red", SDL_Color{ 128, 0, 0 });
	library->addColor("Low Red", SDL_Color{ 192, 0, 0 });
	library->addColor("High Red", SDL_Color{ 255, 64, 64 });
	library->addColor("Light Red", SDL_Color{ 255, 128, 128 });
	library->addColor("White Red", SDL_Color{ 255, 192, 192 });
	// green shades
	library->addColor("Black Green", SDL_Color{ 0, 64, 0 });
	library->addColor("Dark Green", SDL_Color{ 0, 128, 0 });
	library->addColor("Low Green", SDL_Color{ 0, 192, 0 });
	library->addColor("High Green", SDL_Color{ 64, 255, 64 });
	library->addColor("Light Green", SDL_Color{ 128, 255, 128 });
	library->addColor("White Green", SDL_Color{ 192, 255, 192 });
	library->addColor("Black Red", SDL_Color{ 0, 0, 64 });
	// blue shades
	library->addColor("Dark Blue", SDL_Color{ 0, 0, 128 });
	library->addColor("Low Blue", SDL_Color{ 0, 0, 192 });
	library->addColor("High Blue", SDL_Color{ 64, 64, 255 });
	library->addColor("Light Blue", SDL_Color{ 128, 128, 255 });
	library->addColor("White Blue", SDL_Color{ 192, 192, 255 });

}

void addPalettes() {
	library->addPalette("Primary", vector<string>({ "Blue", "Green", "Red", "White" }));
	library->addPalette("Secondary", vector<string>({"Yellow", "Cyan", "Magenta", "Gray"}));
	library->addPalette("Tertiary", vector<string>({"Olive", "Teal", "Purple", "Black"}));
	library->addPalette("Quaternary", vector<string>({"Sky Blue", "Lavender", "Aqua", "Bright Green", "Orange", "Pink"}));
	library->addPalette("Monochrome", vector<string>({"Black", "White"}));
	library->addPalette("Fire", vector<string>({"Black", "Red", "Yellow", "White", "Yellow", "Red", "Black"}));
	library->addPalette("Barbie", vector<string>({ "Pink", "Magenta", "Purple", "White", "Black"}));
	library->addPalette("Tropical", vector<string>({ "Dark Green", "Sky Blue", "Teal", "Dark Blue", "Bright Green", "White", "Black" }));
	
	// add random palettes
	for (int i = 1; i <= numRandomPalettes; i++) {
		int length = rand() % (maxRandomPaletteSize - 1) + minRandomPaletteSize;
		vector<string> colorNames = vector<string>();
		for (int j = 0; j < length; j++) {
			LabeledColor c = library->getRandomColor();
			colorNames.push_back(c.name);
		}
		string paletteName = "Random-" + to_string(i);
		library->addPalette(paletteName, colorNames);
	}
}

void printPaletteOptions() {
	printf("Palette options:\n");
	int length = library->getPalettesLength();
	for (int i = 0; i < length; i++) {
		LabeledPalette p = library->getPalette(i);
		printf("  %s\n", p.name.c_str());
	}
}

void printObjectTypeOptions() {
	printf("ObjectType options:\n");
	printf("  box\n");
	printf("  bulb\n");
	printf("  cube\n");
	printf("  sponge\n");
	printf("  dust\n");
	printf("  octflake\n");
}

void LoadPalette(string PaletteChoice, string ObjectType, int minIt, int maxIt) {
	LabeledPalette libPalette = library->getPalette(PaletteChoice);
	if ((ObjectType == "box") || (ObjectType == "bulb")) {
		palette = new Palette(libPalette, library, maxIt - minIt);
	}
	else if (ObjectType == "cube") {
		palette = new Palette(libPalette, library, 128);
	}
	else {
		palette = new Palette(libPalette, library);
	}

	// Set world's palette
	world->setPalette(palette);
}

void NewRandomPalette() {
	// add random palettes
	numRandomPalettes++;
	int i = numRandomPalettes;
	int length = rand() % (maxRandomPaletteSize - 1) + minRandomPaletteSize;
	vector<string> colorNames = vector<string>();
	for (int j = 0; j < length; j++) {
		LabeledColor c = library->getRandomColor();
		colorNames.push_back(c.name);
	}
	string paletteName = "Random-" + to_string(i);
	library->addPalette(paletteName, colorNames);
}

void CaptureImageAction() {
	printf("\n== Capture image ==\n");
	
	// initialize variables
	int width = 0;
	int height = 0;
	float fovV = 0;
	float fovH = 0;
	float oldFovV = camera->fovV;
	float oldFovH = camera->fovH;

	// get aspect ratio from user
	char toggle;
	printf("Use smart capture? y/n ");
	cin >> toggle;
	if (toggle == 'y') {
		string resolution;
		printf("Enter a resolution.\n  Examples: 4096x4096, 2560x1920, 3840x2160, 3840x1080\n");
		printf("Resolution = ");
		cin >> resolution;
		vector<string> vs = split(resolution, 'x');
		width = atoi(vs.at(0).c_str());
		height = atoi(vs.at(1).c_str());
		fovH = pi / 2.5;
		fovV = (height * 2 * pi) / (9 * width);
	} else {
		printf("Width = ");
		cin >> width;
		printf("Height = ");
		cin >> height;
		printf("FovH = ");
		cin >> fovH;
		printf("FovV = ");
		cin >> fovV;
	}

	// capture image
	double start = clock();
	camera->fovH = fovH;
	camera->fovV = fovV;
	captureImage(width, height);
	double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	printf("DONE! (%#.3fs)\n", duration);
	camera->fovH = oldFovH;
	camera->fovV = oldFovV;
}

using namespace std;

int main(int argc, char* args[]) {
	// define variables
	int WorldSize;
	int ObjectSize;
	int minIt;
	int maxIt;
	float scale;
	float zoom;
	float fallout;
	string PaletteChoice = "Primary";
	string ObjectType;

	// initialize colors and palettes
	srand((unsigned int) time(NULL));
	library = new ColorLibrary();
	addColors();
	addPalettes();
	printf("%d colors found\n", library->getColorsLength());
	printf("%d palettes found\n", library->getPalettesLength());

	// Get user input
	printf("\nGetting user input...\n");
	printf("World Size = ");
	cin >> WorldSize;
	printObjectTypeOptions();
	printf("Object type = ");
	cin >> ObjectType;
	printf("Object Size = ");
	cin >> ObjectSize;
	if (ObjectType == "octflake") {
		printf("Scale = ");
		cin >> scale;
	} 
	else if ((ObjectType == "box") || (ObjectType == "bulb")) {
		printf("Minimum Iterations = ");
		cin >> minIt;
		printf("Maximum Iterations = ");
		cin >> maxIt;
		printf("Scale = ");
		cin >> scale;
		printf("Zoom = ");
		cin >> zoom;
		printf("Fallout = ");
		cin >> fallout;
	}

	// Initialize world variables
	int offset = (WorldSize - ObjectSize) / 2;
	int cOffset = offset - 2;
	printf("\nInitializing world...\n");
	world = new World(WorldSize);
	// initialize camera, fovH = ~1.2566, fovV = 0.5235
	camera = new Camera((float) cOffset, (float) cOffset, (float) cOffset, 0, 0, pi / 2.0f, pi / 2.5f, pi / 6.0f);
	renderer = new Renderer(world, camera);
	renderer->defaultOptions();
	generator = new ObjectGenerator(world);

	// Load palette to world
	LoadPalette(PaletteChoice, ObjectType, minIt, maxIt);

	// Initialize local variables
	bool quit = false;
	int frames = 0;
	clock_t start = clock();
	double duration;

	// Generate an object in the world
	printf("Generating world...\n");
	if ((ObjectType == "box") || (ObjectType == "bulb")) {
		MandelParams* mParams = new MandelParams(ObjectSize, offset, offset, offset, minIt, maxIt, scale, zoom, fallout);
		if (ObjectType == "box") {
			generator->mtGenerateMandelbox(mParams);
		}
		else if (ObjectType == "bulb") {
			generator->mtGenerateMandelbulb(mParams);
		}
	}
	else if (ObjectType == "octflake") {
		FlakeParams* fParams = new FlakeParams(ObjectSize, offset, offset, offset, (int) scale);
		if (ObjectType == "octflake") {
			generator->generateOctFlake(fParams);
		}
	}
	else {
		ObjectParams* oParams = new ObjectParams(ObjectSize, offset, offset, offset);
		if (ObjectType == "sponge") {
			generator->generateMengerSponge(oParams);
		}
		else if (ObjectType == "cube") {
			generator->generateGradientCube(oParams);
		}
		else if (ObjectType == "dust") {
			generator->generateCantorDust(oParams);
		}
	}

	// how long did it take?
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	printf("%#.2f seconds spent generating\n", duration);

	// Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		gSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
	}

	printf("%d, %d, %d, %d\n", gSurface->pitch, gSurface->format, gSurface->w, gSurface->h);

	// prepare keyboard listener
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	// While the user hasn't quit
	while (quit == false)
	{
		// Render a frame
		renderFrame();
		frames++;
		if (frames == 60) {
			duration = (clock() - start) / (double) CLOCKS_PER_SEC;;
			printf("FPS: %f\n", 60 / duration);
			printCameraCoords();
			printLightingInfo();
			start = clock();
			frames = 0;
		}

		// If there's an event to handle
		if (SDL_PollEvent(&event))
		{
			// If the user has Xed out the window
			if (event.type == SDL_QUIT)
			{
				// Quit the program
				quit = true;
			}
		}

		// handle key events
		SDL_PumpEvents();
		// move based on key presses
		if (keys[SDL_GetScancodeFromKey(SDLK_a)]) {
			camera->x += speed * cos(camera->rotY - pi / 2);
			camera->y += speed * sin(camera->rotY - pi / 2);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_d)]) {
			camera->x += speed * cos(camera->rotY + pi / 2);
			camera->y += speed * sin(camera->rotY + pi / 2);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_w)]) {
			camera->x += speed * cos(camera->rotY);
			camera->y += speed * sin(camera->rotY);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_s)]) {
			camera->x += speed * cos(camera->rotY + pi);
			camera->y += speed * sin(camera->rotY + pi);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_q)]) {
			camera->z -= speed;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_e)]) {
			camera->z += speed;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_j)]) {
			camera->rotY -= pi / 32;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_l)]) {
			camera->rotY += pi / 32;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_i)]) {
			camera->rotX += pi / 32;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_k)]) {
			camera->rotX -= pi / 32;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_EQUALS)]) {
			if (BPP < 10) BPP++;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_MINUS)]) {
			if (BPP > 1) BPP--;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_x)]) {
			if (speed < 16) speed *= 1.11f;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_z)]) {
			if (speed > 0.125f) speed *= 0.9f;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_p)]) {
			printPaletteOptions();
			printf("New palette:\n");
			cin >> PaletteChoice;
			LoadPalette(PaletteChoice, ObjectType, minIt, maxIt);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_c)]) {
			CaptureImageAction();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_RIGHTBRACKET)]) {
			PaletteChoice = library->nextPalette(PaletteChoice);
			printf("Next palette...  ");
			printf("New palette choice = %s\n", PaletteChoice.c_str());
			LoadPalette(PaletteChoice, ObjectType, minIt, maxIt);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_LEFTBRACKET)]) {
			PaletteChoice = library->previousPalette(PaletteChoice);
			printf("Previous palette...  ");
			printf("New palette choice = %s\n", PaletteChoice.c_str());
			LoadPalette(PaletteChoice, ObjectType, minIt, maxIt);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_BACKSLASH)]) {
			NewRandomPalette();
			PaletteChoice = library->getPalette(library->getPalettesLength() - 1).name;
			printf("New random palette!  ");
			printf("New palette choice = %s\n", PaletteChoice.c_str());
			LoadPalette(PaletteChoice, ObjectType, minIt, maxIt);
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_COMMA)]) {
			renderer->renderDistance += 10;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_PERIOD)]) {
			renderer->renderDistance -= 10;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_7)]) {
			renderer->rate -= 0.05f;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_8)]) {
			renderer->rate += 0.05f;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_9)]) {
			renderer->falloff -= 0.5f;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_0)]) {
			renderer->falloff += 0.5f;
		}
	}

	// Free resources and close SDL
	close();

	return 0;
}