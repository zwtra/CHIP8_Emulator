#pragma once
#include "chip8_cpu.h"
#include "graphics.h"

typedef unsigned int EMULATOR_STATUS;

#define EMULATOR_STATUS_SUCCESS (EMULATOR_STATUS)0x00000000

#define EMULATOR_STATUS_FAIL_01 (EMULATOR_STATUS)0xC0000001
#define EMULATOR_STATUS_FAIL_02 (EMULATOR_STATUS)0xC0000002
#define EMULATOR_STATUS_FAIL_03 (EMULATOR_STATUS)0xC0000003
#define EMULATOR_STATUS_FAIL_04 (EMULATOR_STATUS)0xC0000004

typedef struct _EMULATOR {
	PCHIP8CPU cpu;
	PGRAPHICS gpu;

	void* memory;
	void* ds_frame;
	int program_size;

} EMULATOR, *PEMULATOR;

EMULATOR_STATUS initialise_emulator(PEMULATOR, void*);
EMULATOR_STATUS emu_initialise_memory(PEMULATOR, void*);
