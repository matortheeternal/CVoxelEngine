#include "Palette.h"

SDL_Color Palette::get(Uint8 index) {
	if (index > length - 1) {
		if (!wrap) {
			return colors[length - 1];
		} 
		else {
			return colors[index % length];
		}
	}
	return colors[index];
}

void Palette::setWrap(bool b) {
	wrap = b;
}

bool Palette::getWrap() {
	return wrap;
}

Palette::Palette() {
	colors = vector<SDL_Color>(255);
	wrap = false;

	SDL_Color black;
	black.a = 0;
	black.r = 0;
	black.g = 0;
	black.b = 0;

	colors[0] = black;
	length = 1;
}

Palette::Palette(LabeledPalette base, ColorLibrary* library) {
	wrap = true;
	length = base.palette.size();
	colors = vector<SDL_Color>(length);

	for (int i = 0; i < length; i++) {
		LabeledColor c = library->getColor(base.palette.at(i));
		colors[i] = c.color;
	}
}

Palette::Palette(LabeledPalette libPalette, ColorLibrary* library, Uint8 max) {
	// load colors based on names
	vector<SDL_Color> base = vector<SDL_Color>();
	length = libPalette.palette.size();
	for (int i = 0; i < length; i++) {
		LabeledColor c = library->getColor(libPalette.palette.at(i));
		base.push_back(c.color);
	}

	colors = vector<SDL_Color>(255);
	wrap = true;
	
	base.push_back(base[0]);
	int size = base.size();
	length = max;
	float unit = (size - 1) / ((float) (length - 1));
	for (int i = 0; i < length; i++) {
		float mappedIndex = i * unit;
		int lowIndex = (int) floor(mappedIndex);
		if (lowIndex == mappedIndex) {
			colors[i] = base[lowIndex];
		} else {
			int highIndex = (int) ceil(mappedIndex);
			float highPercent = mappedIndex - lowIndex;
			float lowPercent = 1.0f - highPercent;
			SDL_Color iColor;
			iColor.a = (Uint8) floor(lowPercent * base[lowIndex].a + highPercent * base[highIndex].a);
			iColor.r = (Uint8) floor(lowPercent * base[lowIndex].r + highPercent * base[highIndex].r);
			iColor.g = (Uint8) floor(lowPercent * base[lowIndex].g + highPercent * base[highIndex].g);
			iColor.b = (Uint8) floor(lowPercent * base[lowIndex].b + highPercent * base[highIndex].b);
			colors[i] = iColor;
		}

	}
}