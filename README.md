# CVoxelEngine
A C++ Voxel Engine, to succeed my Java Voxel Engine.  Uses SDL, and may use OpenCL in the future.

## Building
You can build this project with Microsoft Visual Studio.  Project files are provided.

### Libraries
You need to install the SDL dlls in your system root directory.  I have provided the 64-bit SDL dlls in the lib folder of this repository, which you'd install in %SystemRoot%\SysWOW64 (e.g. C:\Windows\SysWOW64).

## Release
Check out the releases section for compiled builds.

## Usage
### Fractal generation
#### General
* **World size:** This value cubed is the number of bytes your world will take up in RAM.  Note, as of v0.1.1 the world is allocated as a single contiguous array, so you may not be able to get as high as the amount of largest uninterrupted chunk of ram (with 6gb of RAM I can only make a 1gb - 1024^3 world)
* **Object type:** box = mandelbox, bulb = mandelbulb, octflake = octahedron flake, sponge = menger sponge, cube = a gradient cube, dust = cantor dust
* **Object size:** This value is the size of the object you're generating.  I recommend generating an object a little smaller than your world size (give like 3-4 blocks of padding on the sides).  Note that certain fractals require object sizes that fulfill certain conditions to be generated properly.

#### box (Mandelbox)
* **Object size:** Any value
* **Minimum Iterations:** I recommend a value of 4, but you can deviate from that as you see fit.  (higher values will create sparser objects)
* **Maximum Iterations:** I recommend a value between 16 and 50.  Higher values will increase generation time, and each successive increase in the iterations adds less voxels to the object than the last.
* **Scale:** A positive or negative value.  I recommended it be in the range 1.0 > |v| > 3.0
* **Zoom:** I recommend a value around 4.0.  You can tweak this as you see fit, a value less than 4.0 will usually lead to a spherical fractal object (depending on your scale value).
* **Fallout:** A good choice for this value depends on your scale.  Something around f =  1.5 * v^2   is pretty good (v being scale).

#### bulb (Mandelbulb)
* **Object size:** Any value
* **Minimum Iterations:** I recommend a value of 4, but you can deviate from that as you see fit.  (higher values will create sparser objects)
* **Maximum Iterations:** I recommend a value between 16 and 50.  Higher values will increase generation time, and each successive increase in the iterations adds less voxels to the object than the last.
* **Scale:** Generally, most mandelbulbs are generated with integer scale values between 5 and 16.  8 is the most commonly used value.
* **Zoom:** I recommend 2.0.
* **Fallout:** I recommend 1024.

#### octflake
* **Object size:** Relative to scale.  Apply 2 * v +  1 to scale repeatedly to get higher iteration objects.
* **Scale:** I recommend a value >= 7, so you can see the individual octahedrons.

#### sponge
* **Object size:** A power of 3

#### dust
* **Object size:** A power of 3

### Controls
* **W A S D**: For moving around (forwards/left/backwards/right)
* **I J K L**: For rotating the camera (up/left/down/right)
* **Q E**: For increasing or decreasing elevation (up/down)
* **Z X**: For increasing or decreasing camera speed (slow/fast)
* **- +**: For increasing or decresing the pixel scale of the rendered image (larger scale = shorter render times).
* **< >**: For increasing or decreasing render distance (farther/closer)
* **P**: To change the palette.  Will have to enter a palette name in the command prompt window.
* **C**: To capture a screenshot.  Will have you enter a width, height, horizontal field of view, and vertical field of view.  Field of view values should be entered as radians (2 * pi = 360 degrees)
* **9 0**: For increasing or decreasing the distance at which lighting begins to fall off (-/+).
* **7 8**: For increasing or decreasing the rate at which lighting falls off (-/+)
* **\\**: To generate a new random palette.
