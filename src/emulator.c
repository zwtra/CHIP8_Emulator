#include "../include/emulator.h"
#include <Windows.h>

const char __FONT_ROM[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80};

EMULATOR_STATUS initialise_emulator(PEMULATOR emu, void* program) {
	if (emu == 0) {
		return EMULATOR_STATUS_FAIL_01;
	}
	emu->cpu = initialise_cpu();
	emu->gpu = initialise_graphics();

	emu->ds_frame = VirtualAlloc(0, 640 * 320, MEM_COMMIT, PAGE_READWRITE);
	emu->memory = VirtualAlloc(0, 4000, MEM_COMMIT, PAGE_READWRITE);

	if (emu->memory == 0 || emu->ds_frame == 0)
		return EMULATOR_STATUS_FAIL_02;

	if (emu->cpu == 0 || emu->gpu == 0)
		return EMULATOR_STATUS_FAIL_03;

	memset(emu->memory, 0, 4000);

	if (emu_initialise_memory(emu, program))
		return EMULATOR_STATUS_FAIL_04;

	emu->cpu->program_counter = (int)emu->memory + 0x200;

	return EMULATOR_STATUS_SUCCESS;
}

EMULATOR_STATUS emu_initialise_memory(PEMULATOR emu, void* program) {
	// Copy font into memory
	memcpy((int)emu->memory + 0x50, __FONT_ROM, 80);
	// Open program 
	HANDLE hProg = CreateFileA(program, GENERIC_READ, 0x3, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hProg == INVALID_HANDLE_VALUE)
		return EMULATOR_STATUS_FAIL_04;

	DWORD program_size = 0;

	if (ReadFile(hProg, (int)emu->memory + 0x200, 0x800, &program_size, 0))
		return EMULATOR_STATUS_SUCCESS;
	return EMULATOR_STATUS_FAIL_01;
}