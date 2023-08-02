#include <Windows.h>
#include "../include/chip8_cpu.h"

PCHIP8CPU initialise_cpu() {

	PCHIP8CPU chip8 = malloc(sizeof(CHIP8CPU));

	if (chip8 == 0) {
		return 0;
	}

	chip8->index_reg = 0;
	chip8->program_counter = 0;

	memset(&(chip8->reg_v), 0, 16);
	return chip8;
}