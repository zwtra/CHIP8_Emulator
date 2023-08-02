#include <Windows.h>
#include "../include/graphics.h"
#include "../include/chip8_cpu.h"

PGRAPHICS initialise_graphics() {

	PGRAPHICS graphics = malloc(sizeof(GRAPHICS));

	if (graphics == 0) {
		return 0;
	}

	graphics->width = 640;
	graphics->height = 320;
	graphics->display = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
	graphics->renderer = SDL_CreateRenderer(graphics->display, -1, 0);
	graphics->image = SDL_CreateTexture(graphics->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 320);

	graphics->frame_buffer = VirtualAlloc(0, 640 * 320 * 4, MEM_COMMIT, PAGE_READWRITE);
	graphics->frame_pitch = 640 * 4;

	return graphics;

}

GRAPHICS_STATUS gclear(PGRAPHICS g) {
	memset(g->frame_buffer, 0, g->width * g->height * 4);
	return redraw(g);
}

GRAPHICS_STATUS redraw(PGRAPHICS g) {
	if (SDL_UpdateTexture(g->image, 0, g->frame_buffer, g->frame_pitch))
		return GRAPHICS_STATUS_FAIL_01;
	if (SDL_RenderClear(g->renderer))
		return GRAPHICS_STATUS_FAIL_02;
	if (SDL_RenderCopy(g->renderer, g->image, NULL, NULL))
		return GRAPHICS_STATUS_FAIL_03;
	SDL_RenderPresent(g->renderer);
	return GRAPHICS_STATUS_SUCCESS;
}