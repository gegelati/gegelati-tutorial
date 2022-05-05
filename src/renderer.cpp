
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cmath>
#include <float.h>
#include <cstdio>

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

void Renderer::displayText(const char* text, int posX, int posY) {
	// Color of text
	SDL_Color colorGreen = { 0, 255, 0, 255 };
	SDL_Color colorWhite = { 255, 255, 255, 255 };

	SDL_Surface* textSurf = TTF_RenderText_Blended(display.font, text,
		colorGreen);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(display.renderer,
		textSurf);

	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	SDL_Rect textRect;

	textRect.x = posX;
	textRect.y = posY;
	textRect.w = width;
	textRect.h = height;
	SDL_RenderCopy(display.renderer, texture, NULL, &textRect);

	/* Free resources */
	SDL_FreeSurface(textSurf);
	SDL_DestroyTexture(texture);
}

bool Renderer::renderEnv(double state, double torque, uint64_t frame, uint64_t generation) {
	static long int i = 0;
	static double max_fps = 0.;
	static double avg_fps = 0.;
	static double min_fps = DBL_MAX;

	// Select the color for drawing. It is set to red here. 
	SDL_SetRenderDrawColor(display.renderer, 255, 255, 255, 255);
	// Clear the entire screen to our selected color.
	SDL_RenderClear(display.renderer);

	// Position of the pendulum in the window
	SDL_Rect destPendulum = { 225, 250, 49, 234 };
	SDL_Point centerPendulum = { 25, 15 };

	// Convert the angle to degree with the offset to match the python training
	double angle = 180.f - state * 180.f / ((float)M_PI);

	// Display the pendulum
	SDL_RenderCopyEx(display.renderer, display.texturePendulum, NULL, &destPendulum, angle, &centerPendulum, SDL_FLIP_NONE);

	if (fabs(torque) > 0.0) {
		double scale = std::sqrt(std::fabs(torque));
		// Position of the pendulum in the window
		const int arrowWidth = (int)(178 * scale);
		const int arrowHeight = (int)(69 * scale);

		SDL_Rect destArrow = { (DISPLAY_W - arrowWidth) / 2, (int)(DISPLAY_H / 2 + 14 + scale * 50.0), arrowWidth , arrowHeight };
		SDL_Point centerArrow = { arrowWidth / 2, arrowHeight / 2 };

		// Display arrow
		SDL_RenderCopyEx(display.renderer, display.textureArrow, NULL, &destArrow, NULL, NULL, (torque > 0.0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	// Print Generation text
	char generationString[100];
	sprintf(generationString, "   gen: %04lld", generation);
	Renderer::displayText(generationString, 0, 0);

	// Print FrameNumber text
	char frameNumber[17];
	sprintf(frameNumber, "frame: %4lld", frame);
	Renderer::displayText(frameNumber, 0, 22);

	// Proceed to the actual display
	SDL_RenderPresent(display.renderer);

	// Smoother rendering
	SDL_Delay(25);

	SDL_Event event;
	// Grab next events off the queue.
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_q:
			return true;
		}
	case SDL_QUIT:
		return true;
	default:
		break;
	}

	return false;
}

void Renderer::renderFinalize()
{
	SDL_DestroyTexture(display.texturePendulum);
	SDL_DestroyTexture(display.textureArrow);
	SDL_DestroyRenderer(display.renderer);
	SDL_DestroyWindow(display.screen);
}