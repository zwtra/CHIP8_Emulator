#pragma once
#include <SDL.h>

#define SCALE_ENLARGE 10
#define MAGIC_PIXEL 0x5a28

#define CPU_CUSTOM_WIDTH	64
#define CPU_CUSTOM_HEIGHT	32

typedef unsigned int GRAPHICS_STATUS;

#define GRAPHICS_STATUS_SUCCESS (GRAPHICS_STATUS)0x00000000

#define GRAPHICS_STATUS_FAIL_01 (GRAPHICS_STATUS)0xC0000001
#define GRAPHICS_STATUS_FAIL_02 (GRAPHICS_STATUS)0xC0000002
#define GRAPHICS_STATUS_FAIL_03 (GRAPHICS_STATUS)0xC0000003

typedef struct _GRAPHICS {
	SDL_Window* display;
	SDL_Renderer* renderer;
	SDL_Texture* image;

	DWORD* f_frame_buf;
	BYTE* s_frame_buf;
	int max_fps;
	int width;
	int height;
	int frame_pitch;

} GRAPHICS, *PGRAPHICS;

PGRAPHICS initialise_graphics();
GRAPHICS_STATUS redraw(PGRAPHICS);
GRAPHICS_STATUS gclear(PGRAPHICS);
GRAPHICS_STATUS convert_pixel_format(PGRAPHICS);