#include <SDL.h>
#include <vector>
#include <string>

using namespace std;

typedef struct LabeledColor {
	string name;
	SDL_Color color;
} LabeledColor;

typedef struct LabeledPalette {
	string name;
	vector<string> palette;
} LabeledPalette;

class ColorLibrary {
private:
	vector<LabeledColor>* colors;
	vector<LabeledPalette>* palettes;
public:
	LabeledColor getRandomColor();
	LabeledColor getColor(string);
	LabeledColor getColor(int);
	LabeledPalette getPalette(string);
	LabeledPalette getPalette(int);
	int indexOfPalette(string);
	string nextPalette(string);
	string previousPalette(string);
	int getColorsLength();
	int getPalettesLength();
	void addColor(string, SDL_Color);
	void addColor(LabeledColor);
	void addPalette(string, vector<string>);
	void addPalette(LabeledPalette);
	ColorLibrary();
};
