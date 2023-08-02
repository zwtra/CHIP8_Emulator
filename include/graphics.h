#pragma once
#include <SDL.h>

typedef unsigned int GRAPHICS_STATUS;

#define GRAPHICS_STATUS_SUCCESS (GRAPHICS_STATUS)0x00000000

#define GRAPHICS_STATUS_FAIL_01 (GRAPHICS_STATUS)0xC0000001
#define GRAPHICS_STATUS_FAIL_02 (GRAPHICS_STATUS)0xC0000002
#define GRAPHICS_STATUS_FAIL_03 (GRAPHICS_STATUS)0xC0000003

typedef struct _GRAPHICS {
	SDL_Window* display;
	SDL_Renderer* renderer;
	SDL_Texture* image;

	void* frame_buffer;
	int max_fps;
	int width;
	int height;
	int frame_pitch;

} GRAPHICS, *PGRAPHICS;

PGRAPHICS initialise_graphics();
GRAPHICS_STATUS redraw(PGRAPHICS);
GRAPHICS_STATUS gclear(PGRAPHICS);