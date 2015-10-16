__global char* blocks;
__global float* ref;
__global 

__kernel int* raycastFrom(int px, int py, int width, int height) {
	
}

__kernel void render(__global char* blocks, __global float* ref, __global char* pixels, int y1, int y2, int width, int height, int pixelScale) {
	int pixel[3];
	int p = 0;
	for (int py = y1; py < y2; py += pixelScale) {
		for (int px = 0; px < width; px += pixelScale) {
			pixel = raycastFrom(ref, px, py, width, height);
			if (pixelScale == 1) {
				p = 3 * (py*width + px);
				pixels[p + 0] = pixel[0];
				pixels[p + 1] = pixel[1];
				pixels[p + 2] = pixel[2];
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