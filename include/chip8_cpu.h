#pragma once

typedef struct _CHIP8CPU {
	void* program_counter;
	void* index_reg;
	char timers[2];
	char reg_v[16];
} CHIP8CPU, *PCHIP8CPU;

PCHIP8CPU initialise_cpu();
