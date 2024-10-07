# sdlimg
A simple [SDL2](http://libsdl.org/) image viewer.

## Features
* Drag and drop
* Filtering (Best, Linear, Nearest)
* Flip (Horizontal, Vertical)
* Movement
* Rotation
* Zoom

## Build
1. Clone this git repository
2. Install the dependencies:
    * Debian: `sudo apt install -y build-essential make libsdl2-dev libsdl2-image-dev`
    * Other Platforms: ???
3. Run `make` in the root directory of the git repository

## Quickstart
### Controls
* **Zoom in:**    + key, up arrow key, scrollwheel
* **Zoom out:**   - key, down arrow key, scrollwheel
* **Rotate:**     left/right arrow keys, mouse rmb+move
* **Move:**       mouse lmb+move
* **Reset:**      R key
* **Quit:**       Q key
* **Fullscreen:** F key, F10
* **Metadata:**   I key
* **Filtering:**  A key
* **Flip:**       J key

### Supported image formats
* BMP
* GIF
* JPEG
* LBM
* PCX
* PNG
* PNM (PPM/PGM/PBM)
* QOI
* TGA
* XCF
* XPM
* SVG
* AVIF
* JPEG-XL
* TIFF
* WebP

## License
Licensed under the terms of the MIT License.
