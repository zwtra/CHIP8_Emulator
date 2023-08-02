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

	SDL_Event event;

	while (1) {
		SDL_PollEvent(&event);
		emulator_start(&emu);
	}

	return 0;
}