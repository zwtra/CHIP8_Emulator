#include <stdio.h>
#include <SDL.h>
#include "../include/emulator.h"

int pix_at_pos(BYTE* frame_buf, int x_pos, int y_pos, int pixel_val) {
	// assuming blank display, pixel_offset = offset from x = 0
	// for this example pixel_offset is 1, and origin is 0
	// therefore we want 011111111 pixel display

	int s_flag = 0;

	int pixel_offset = x_pos % 8;

	int x_raw = x_pos / 8;
	int y_raw = y_pos * 8;

	DWORD new_pixel = pixel_val >> pixel_offset;
	DWORD old_pixel = frame_buf[(y_raw) + (x_raw)];

	if ((new_pixel ^ old_pixel) != new_pixel) {
		frame_buf[(y_raw)+(x_raw)] = frame_buf[(y_raw) + (x_raw)] ^ (pixel_val >> pixel_offset);
		s_flag = 1;
	}
	else {
		frame_buf[(y_raw)+(x_raw)] = pixel_val >> pixel_offset;
	}
	new_pixel = (pixel_val << (32 - pixel_offset) >> 24) ^ frame_buf[(y_raw)+(x_raw)+1];
	old_pixel = frame_buf[(y_raw)+(x_raw) + 1];

	if (new_pixel == old_pixel) return s_flag;

	if ((new_pixel ^ old_pixel) != new_pixel) {
		frame_buf[(y_raw)+(x_raw)+1] = (BYTE)new_pixel;
		s_flag = 1;
	}
	else {
		frame_buf[(y_raw)+(x_raw)+1] = (BYTE)new_pixel;
	}
	return s_flag;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		return 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 0x10;

	EMULATOR emu;

	initialise_emulator(&emu, argv[1]);

	pix_at_pos(emu.gpu->s_frame_buf, 1, 2, 0xff);

	convert_pixel_format(emu.gpu);
	redraw(emu.gpu);

	SDL_Event event;

	while (1) {
		SDL_PollEvent(&event);
		//emulator_start(&emu);
	}

	return 0;
}