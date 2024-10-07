/**
 * sdlimg - A bare-bones SDL2 image viewer application
 * Copyright (c) 2018-2024 Niklas Benfer <https://github.com/palomena>
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iso646.h>

/**
 * SDL logo window icon
 */
static Uint16 pixels[16*16] = {
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
    0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
    0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
    0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
    0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
    0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
    0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
    0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
    0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
    0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
    0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
    0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
    0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
    0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
    0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
    0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
    0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
    0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
    0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
    0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
    0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
    0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
};

static struct ImageViewer {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *image;
	SDL_Point offset;
	SDL_RendererFlip flip;
	const char *path;
	double angle;
	double zoom;
} viewer;

Uint32 customDrawEvent;

static void show_help(void) {
	static const char help_string[] = {
		"sdlimg - A simple SDL2 image viewer\n"
		"Copyright (c) 2018-2024 Niklas Benfer\n"
		"Command Line Usage:\n"
		"    ./sdlimg [path to the image file]\n"
		"\n"
		"Supported image formats:\n"
		"    BMP, GIF, JPEG, LBM, PCX, PNG,\n"
		"    PNM (PPM/PGM/PBM), QOI, TGA, XCF,\n"
		"    XPM, SVG, AVIF, JPEG-XL, TIFF, WebP\n"
		"\n"
		"Controls:\n"
		"    Zoom in:    + key, up arrow key, scrollwheel\n"
		"    Zoom out:   - key, down arrow key, scrollwheel\n"
		"    Rotate:     left/right arrow keys, mouse rmb+move\n"
		"    Move:       mouse lmb+move\n"
		"    Reset:      R key\n"
		"    Quit:       Q key\n"
		"    Fullscreen: F key, F10\n"
		"    Metadata:   I key\n"
		"    Filtering:  A key\n"
		"    Flip:       J key\n"
		"    Insert:     Insert key\n"
	};
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL2 Image Viewer", help_string, NULL);
}

static void show_error(const char *message) {
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, message);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Image Viewer: ERROR", message, NULL);
}

static void ensure_valid_pointer(void *ptr) {
	if (ptr == NULL) {
		show_error(SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

static const char* filename(const char *path) {
	const char *name = SDL_strrchr(path, '/');
	if (name != NULL) {
		name++;
	} else {
		name = path;
	}
	return name;
}

static const char* filetype(const char *path) {
	return SDL_strrchr(path, '.');
}

static void reset_image_viewer(void) {
	viewer.angle = 0.00;
	viewer.offset.x = 0;
	viewer.offset.y = 0;
	viewer.zoom = 1.00;
}

static void open_image_viewer(void) {
	static const int imgFlags = (
		IMG_INIT_AVIF |
		IMG_INIT_JPG |
		IMG_INIT_JXL |
		IMG_INIT_PNG |
		IMG_INIT_TIF |
		IMG_INIT_WEBP
	);
	SDL_DisplayMode displayMode;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(imgFlags);
	SDL_GetCurrentDisplayMode(0, &displayMode);
	viewer.window = SDL_CreateWindow(
		"SDL2 Image Viewer",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		displayMode.w / 2,
		displayMode.h / 2,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	ensure_valid_pointer(viewer.window);
	SDL_Surface *icon = SDL_CreateRGBSurfaceFrom(pixels, 16,16,16,16*2, 0x0f00,0x00f0,0x000f,0xf000);
	if(icon != NULL) {
		SDL_SetWindowIcon(viewer.window, icon);
		SDL_FreeSurface(icon);
	}
	viewer.renderer = SDL_CreateRenderer(viewer.window, -1, SDL_RENDERER_SOFTWARE);
	ensure_valid_pointer(viewer.renderer);
	SDL_SetRenderDrawColor(viewer.renderer, 0, 0, 0, 255);
	SDL_RenderClear(viewer.renderer);
	SDL_RenderPresent(viewer.renderer);
	viewer.angle = 0.00;
	viewer.flip = SDL_FLIP_NONE;
	viewer.zoom = 1.00;
	customDrawEvent = SDL_RegisterEvents(1);
}

static void close_image(void) {
	if (viewer.image != NULL) {
		SDL_DestroyTexture(viewer.image);
		viewer.image = NULL;
		viewer.path = NULL;
		reset_image_viewer();
	}
}

static void close_image_viewer(void) {
	close_image();
	SDL_DestroyRenderer(viewer.renderer);
	SDL_DestroyWindow(viewer.window);
	IMG_Quit();
	SDL_Quit();
}

static void adjust_zoom_level_to_fit_screen(void) {
	int window_width, window_height;
	int image_width, image_height;
	SDL_QueryTexture(viewer.image, NULL, NULL, &image_width, &image_height);
	SDL_GetWindowSize(viewer.window, &window_width, &window_height);
	if (image_width > window_width or image_height > window_height) {
		viewer.zoom = (double)window_width / (double)image_width;
	}
}

static void open_image(const char *path) {
	SDL_Log("Loading image file \"%s\"", path);
	SDL_Surface *surface = IMG_Load(path);
	if (surface != NULL) {
		close_image();
		viewer.image = SDL_CreateTextureFromSurface(viewer.renderer, surface);
		viewer.path = path;
		ensure_valid_pointer(viewer.image);
		SDL_FreeSurface(surface);
		SDL_SetWindowTitle(viewer.window, filename(path));
		adjust_zoom_level_to_fit_screen();
	} else {
		show_error(IMG_GetError());
	}
}

static void draw_image(void) {
	SDL_RenderClear(viewer.renderer);
	if (viewer.image != NULL) {
		int window_width, window_height;
		int width, height;
		SDL_QueryTexture(viewer.image, NULL, NULL, &width, &height);
		width *= viewer.zoom;
		height *= viewer.zoom;
		SDL_GetWindowSize(viewer.window, &window_width, &window_height);
		SDL_Rect dstrect = {
			viewer.offset.x + (window_width  - width) / 2,
			viewer.offset.y + (window_height - height) / 2,
			width,
			height
		};
		SDL_Point center = {width / 2, height / 2};
		SDL_RenderCopyEx(viewer.renderer, viewer.image, NULL, &dstrect, viewer.angle, &center, viewer.flip);
	}
	SDL_RenderPresent(viewer.renderer);
}

static void trigger_refresh(void) {
	static Uint32 lastRefresh = 0;
	Uint32 ticks = SDL_GetTicks();
	if (ticks - lastRefresh > 20) {
		SDL_Event event = {.type = customDrawEvent};
		SDL_PushEvent(&event);
		lastRefresh = ticks;
	}
}

// Function to calculate GCD using the Euclidean algorithm
static int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b; // Remainder of a divided by b
        a = temp;  // Update a to b
    }
    return a; // When b becomes 0, a is the GCD
}

static void show_image_metadata(void) {
	if (viewer.image != NULL) {
		static char buffer[2048];
		int width, height;
		SDL_QueryTexture(viewer.image, NULL, NULL, &width, &height);
		int aspect_x = width  / gcd(width, height);
		int aspect_y = height / gcd(width, height);
		SDL_snprintf(
			buffer, sizeof(buffer),
			"Filename: %s\n"
			"Filetype: %s\n"
			"Width: %d\n"
			"Height: %d\n"
			"Aspect Ratio: %d:%d\n"
			"Zoom: %f\n",
			filename(viewer.path),
			filetype(viewer.path),
			width, height,
			aspect_x, aspect_y,
			viewer.zoom
		);
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_INFORMATION,
			"Image Information",
			buffer,
			viewer.window
		);
	}
}

static void handle_keyboard_event(SDL_KeyboardEvent *event) {
	switch (event->keysym.sym) {
		default:
			// Triggers a redraw upon return
			break;
		case SDLK_INSERT:
			// TODO: SDL3 can also get non-text clipboard contents!
			char *path = SDL_GetClipboardText();
			open_image(path);
			SDL_free(path);
			break;
		case SDLK_MINUS:
		case SDLK_DOWN:
			viewer.zoom *= 0.90;
			break;
		case SDLK_PLUS:
		case SDLK_UP:
			viewer.zoom *= 1.10;
			break;
		case SDLK_LEFT:
			viewer.angle -= 10.00;
			break;
		case SDLK_RIGHT:
			viewer.angle += 10.00;
			break;
		case SDLK_f:
		case SDLK_F10:
			if (SDL_GetWindowFlags(viewer.window) & SDL_WINDOW_FULLSCREEN) {
				SDL_SetWindowFullscreen(viewer.window, 0);
			} else {
				SDL_SetWindowFullscreen(viewer.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			break;
		case SDLK_q: {
			static SDL_Event quitEvent = {.type = SDL_QUIT};
			SDL_PushEvent(&quitEvent);
			break;
		}
		case SDLK_i:
			show_image_metadata();
			break;
		case SDLK_r:
			reset_image_viewer();
			break;
		case SDLK_h:
			show_help();
			break;
		case SDLK_j: {
			static const SDL_RendererFlip flips[] = {
				SDL_FLIP_NONE,
				SDL_FLIP_HORIZONTAL,
				SDL_FLIP_VERTICAL
			};
			static int index = 0;
			index = (index+1) % SDL_arraysize(flips);
			viewer.flip = flips[index];
			break;
		}
		case SDLK_a: {
			static const SDL_ScaleMode modes[] = {
				SDL_ScaleModeBest,
				SDL_ScaleModeLinear,
				SDL_ScaleModeNearest
			};
			static int index = 0;
			index = (index+1) % SDL_arraysize(modes);
			SDL_SetTextureScaleMode(viewer.image, modes[index]);
			break;
		}
	}
}

static void handle_window_event(SDL_WindowEvent *event) {
	switch (event->event) {
		default:
			break;
		case SDL_WINDOWEVENT_RESTORED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			trigger_refresh();
			break;
	}
}

static void handle_custom_event(SDL_Event *event) {
	if (event->type == customDrawEvent) {
		draw_image();
	}
}

static int update_image_viewer(void) {
	SDL_Event event;
	while (SDL_WaitEventTimeout(&event, 1000)) {
		switch (event.type) {
			default:
				handle_custom_event(&event);
				break;
			case SDL_QUIT:
				return 0;
			case SDL_DROPFILE:
				open_image(event.drop.file);
				trigger_refresh();
				break;
			case SDL_KEYUP:
				handle_keyboard_event(&event.key);
				trigger_refresh();
				break;
			case SDL_WINDOWEVENT:
				handle_window_event(&event.window);
				break;
			case SDL_MOUSEMOTION:
				if (event.button.button == 1) {
					viewer.offset.x += event.motion.xrel;
					viewer.offset.y += event.motion.yrel;
					trigger_refresh();
				} else if (event.button.button == 4) {
					viewer.angle += (double)(event.motion.xrel + event.motion.yrel);
					SDL_Log("Set angle to %f\n", viewer.angle);
					trigger_refresh();
				}
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0) {
					viewer.zoom *= 1.10;
				} else {
					viewer.zoom *= 0.90;
				}
				trigger_refresh();
				break;
		}
	}
	trigger_refresh(); // triggers a refresh every second
	return 1;
}

int main(int argc, char *argv[]) {
	open_image_viewer();
	if (argc > 1) {
		open_image(argv[1]);
		trigger_refresh();
	}
	while (update_image_viewer());
	close_image_viewer();
	return 0;
}
