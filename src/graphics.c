#include <Windows.h>
#include "../include/graphics.h"

PGRAPHICS initialise_graphics() {

	PGRAPHICS graphics = malloc(sizeof(GRAPHICS));

	if (graphics == 0) {
		return 0;
	}

	graphics->width = 640;
	graphics->height = 320;
	graphics->display = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64, 32, 0);
	graphics->renderer = SDL_CreateRenderer(graphics->display, -1, 0);
	graphics->image = SDL_CreateTexture(graphics->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 64, 32);

	graphics->s_frame_buf = VirtualAlloc(0, 256 * 8 * 4, MEM_COMMIT, PAGE_READWRITE);
	graphics->f_frame_buf = VirtualAlloc(0, 640 * 320 * 4, MEM_COMMIT, PAGE_READWRITE);
	graphics->frame_pitch = 640 * 4;

	return graphics;

}

GRAPHICS_STATUS gclear(PGRAPHICS g) {
	memset(g->f_frame_buf, 0, g->width * g->height * 4);
	return redraw(g);
}

GRAPHICS_STATUS redraw(PGRAPHICS g) {
	if (SDL_UpdateTexture(g->image, 0, g->s_frame_buf, 64 * 4))
		return GRAPHICS_STATUS_FAIL_01;
	if (SDL_RenderClear(g->renderer))
		return GRAPHICS_STATUS_FAIL_02;
	if (SDL_RenderCopy(g->renderer, g->image, NULL, NULL))
		return GRAPHICS_STATUS_FAIL_03;
	SDL_RenderPresent(g->renderer);
	return GRAPHICS_STATUS_SUCCESS;
}

GRAPHICS_STATUS convert_pixel_format(PGRAPHICS g) {

	// Pixels are stored as binary, ie FF defines 8 pixels to be white, 01 defines rightmost pixel as white
	// There are 64 * 32 pixels in the display = 2048, 2048 / 8 = 256 | 0x100. Therefore 0x100 bytes make up our simple frame buffer
	// I have to convert this to SDL's format where a single pixel is 4 bytes...

	DWORD* temp_frame = malloc(256 * 8 * 4);

	if (temp_frame == 0)
		return GRAPHICS_STATUS_FAIL_03;

	for (int pixels = 0; pixels < 256; pixels++) {
		for (int pixel_bit = 0; pixel_bit < 8; pixel_bit++) {
			DWORD bit_value = (g->s_frame_buf[pixels] & (1 << pixel_bit)) ? 0xffffffff : 0x00000000;
			temp_frame[(pixels * 32 + pixel_bit * 4) / 4] = bit_value;
		}

	}
	memcpy(g->s_frame_buf, temp_frame, 256 * 8 * 4);

	free(temp_frame);

	return GRAPHICS_STATUS_SUCCESS;
}