
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "renderer.h"

/**
* \brief Structure holding attributes for a display.
*/
typedef struct sdlDisplay {
	SDL_Renderer* renderer;
	SDL_Window* screen;
	SDL_Texture* texturePendulum;
	SDL_Texture* textureArrow;
	TTF_Font* font;
}sdlDisplay;

// sdlDisplay instance
static sdlDisplay display;

void Renderer::renderInit() {
	display.screen = NULL;
	display.renderer = NULL;

	// Initialize SDL
	fprintf(stderr, "SDL_Init_Start\n");
	if (SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}
	fprintf(stderr, "SDL_Init_End\n");


	// Initialize SDL TTF for text display
	fprintf(stderr, "SDL_TTF_Init_Start\n");
	if (TTF_Init()) {
		printf("TTF initialization failed: %s\n", TTF_GetError());
	}
	fprintf(stderr, "SDL_TTF_Init_End\n");

	// Open Font for text display
	display.font = TTF_OpenFont(PATH_TTF, 20);
	if (!display.font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}

	// Create window
	display.screen = SDL_CreateWindow("Environment_Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		DISPLAY_W, DISPLAY_H, SDL_WINDOW_SHOWN);
	if (!display.screen) {
		fprintf(stderr, "SDL: could not set video mode - exiting\n");
		exit(1);
	}
	// Create renderer
	display.renderer = SDL_CreateRenderer(display.screen, -1, SDL_RENDERER_ACCELERATED);
	if (!display.renderer) {
		fprintf(stderr, "SDL: could not create renderer - exiting\n");
		exit(1);
	}

	// Open pendulum image
	SDL_Surface* surfacePendulum = IMG_Load(PATH_PENDULUM);

	if (!surfacePendulum) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(1);
	}

	display.texturePendulum = SDL_CreateTextureFromSurface(display.renderer,
		surfacePendulum);
	if (!display.texturePendulum) {
		fprintf(stderr, "SDL: could not create pendulum texture - exiting\n");
		exit(1);
	}

	// Open arrow image
	SDL_Surface* surfaceArrow = IMG_Load(PATH_ARROW);

	if (!surfaceArrow) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(1);
	}

	display.textureArrow = SDL_CreateTextureFromSurface(display.renderer,
		surfaceArrow);
	if (!display.textureArrow) {
		fprintf(stderr, "SDL: could not create texture - exiting\n");
		exit(1);
	}
}
void Renderer::renderFinalize()
{
	SDL_DestroyTexture(display.texturePendulum);
	SDL_DestroyTexture(display.textureArrow);
	SDL_DestroyRenderer(display.renderer);
	SDL_DestroyWindow(display.screen);
}