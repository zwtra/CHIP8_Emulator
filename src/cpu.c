#include "../include/chip8_cpu.h"
#include "../include/chip8_cpu_internal.h"

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

CPU_STATUS fde_cycle(PCHIP8CPU cpu) {
	// Read WORD at PC
	WORD ins = *(WORD*)cpu->program_counter;
	WORD ins_type = ins & 0x1000;
	
	switch (ins_type) {
	case 0x0000:
		return ins_type_zero(ins);
	case 0x1000:
		break;
	case 0x6000:
		break;
	case 0x7000:
		break;
	case 0xA000:
		break;
	case 0xD000:
		break;
	}

	return CPU_STATUS_SUCCESS;
}

CPU_STATUS ins_type_zero(WORD ins) {
	switch (ins) {
	case 0xE000:
		return CPU_GRAPHICS_CLEAR;
	}
	return CPU_STATUS_INVALID;
}