#include <stdio.h>
#include <SDL.h>
#include "../include/emulator.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
		return 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 0x10;

	EMULATOR emu;

	initialise_emulator(&emu, argv[1]);

	emu.gpu->s_frame_buf[0] = 0xa1;
	emu.gpu->s_frame_buf[0x64] = 0xa1;
	emu.gpu->s_frame_buf[0xff] = 0x81;

	convert_pixel_format(emu.gpu);

	redraw(emu.gpu);

	SDL_Event event;

	//emulator_start(&emu);

	while (1) {
		SDL_PollEvent(&event);
	}

	return 0;
}