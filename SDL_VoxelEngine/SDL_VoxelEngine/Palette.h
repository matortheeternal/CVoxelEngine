#include <SDL.h>
#include <vector>
#include "math.h"
#include "ColorLibrary.h"

using namespace std;

class Palette {
private:
    vector<SDL_Color> colors;
	int length;
	bool wrap;
public:
	SDL_Color get(Uint8);
	void set(Uint8, SDL_Color);
	int getLength();
	bool getWrap();
	void setWrap(bool);
	Palette();
	Palette(LabeledPalette, ColorLibrary*);
	Palette(LabeledPalette, ColorLibrary*, Uint8);
};