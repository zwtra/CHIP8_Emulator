#include "../include/emulator.h"
#include <Windows.h>
#include <stdio.h>

const char __FONT_ROM[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80};

EMULATOR_STATUS initialise_emulator(PEMULATOR emu, void* program) {
	if (emu == 0) {
		return EMULATOR_STATUS_FAIL_01;
	}

	emu->cpu = initialise_cpu();
	emu->gpu = initialise_graphics();

	emu->memory = VirtualAlloc(0, 4000, MEM_COMMIT, PAGE_READWRITE);

	if (emu->memory == 0)
		return EMULATOR_STATUS_FAIL_02;

	if (emu->cpu == 0 || emu->gpu == 0)
		return EMULATOR_STATUS_FAIL_03;

	memset(emu->memory, 0, 4000);

	if (emu_initialise_memory(emu, program))
		return EMULATOR_STATUS_FAIL_04;

	// Set Base Address of program in CPU struct

	emu->cpu->base = (BYTE*)emu->memory;

	return EMULATOR_STATUS_SUCCESS;
}

EMULATOR_STATUS emu_initialise_memory(PEMULATOR emu, void* program) {
	// Copy font into memory
	memcpy((void*)((int)emu->memory + 0x50), __FONT_ROM, 80);
	// Open program 
	HANDLE hProg = CreateFileA(program, GENERIC_READ, 0x3, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hProg == INVALID_HANDLE_VALUE)
		return EMULATOR_STATUS_FAIL_04;


	if (ReadFile(hProg, (void*)((int)emu->memory + 0x200), 0x800, &emu->program_size, 0))
		return EMULATOR_STATUS_SUCCESS;
	return EMULATOR_STATUS_FAIL_01;
}

EMULATOR_STATUS emulator_start(PEMULATOR emu) {
	CPU_STATUS cpu_return;
	// main loop - fetch execute decode - CPU handles all memory operations, we just
	while ((int)emu->cpu->program_counter < (int)(emu->program_size + 0x200)) { // gross casts
		cpu_return = fde_cycle(emu->cpu);
		if (cpu_return) {
			emulator_handle_cpu_req(cpu_return);
		}
	}
	return EMULATOR_STATUS_SUCCESS;
}