#include "../include/chip8_cpu.h"
#include "../include/chip8_cpu_internal.h"
#include "../include/stack.h"

PCHIP8CPU initialise_cpu(int cpu_type) {

	PCHIP8CPU chip8 = malloc(sizeof(CHIP8CPU));

	if (chip8 == 0) {
		return 0;
	}

	chip8->index_reg = 0;
	chip8->program_counter = (WORD*)0x200;
	chip8->base = 0;

	init_stack(&chip8->stack);
	memset(&(chip8->reg_v), 0, 16);

	return chip8;
}

CPU_STATUS fde_cycle(PCHIP8CPU cpu) {
	// Read WORD at PC
	WORD ins = *(WORD*)((int)cpu->program_counter + (int)cpu->base);
	ins = (ins >> 8) | (ins << 8);
	cpu->program_counter++;
	WORD ins_type = ins & 0xF000;

	switch (ins_type) {
	case 0x0000:
		return ins_type_zero(cpu, ins);
	case 0x1000:
		cpu->program_counter = (WORD*)(ins & 0x0fff);
		return CPU_STATUS_SUCCESS;
	case 0x6000:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = ins & 0x00ff;
		return CPU_STATUS_SUCCESS;
	case 0x7000:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] += ins & 0x00ff;
		break;
	case 0xA000:
		cpu->index_reg = (void*)(ins & 0x0fff);
		return CPU_STATUS_SUCCESS;
	case 0xD000:
		return ins_type_dnnn(cpu, ins);
	}

	return CPU_STATUS_INVALID;
}

CPU_STATUS ins_type_zero(PCHIP8CPU cpu, WORD ins) {
	switch (ins) {
	case 0x00E0:
		return CPU_GRAPHICS_CLEAR;
	case 0x00EE:
		cpu->program_counter = (WORD*)spop(&cpu->stack);
		return CPU_STATUS_SUCCESS;
	}
	return CPU_STATUS_INVALID;
}

CPU_STATUS ins_type_dnnn(PCHIP8CPU cpu, WORD ins) {
	int spr_col = 0; // collision flag, set to 1 if sprite turns any pixels off
	WORD x_pos, y_pos, sprite_rows;
	BYTE* ind_ptr = (BYTE*)(cpu->base + (int)cpu->index_reg);

	x_pos = cpu->reg_v[(ins & 0x0f00) >> 0x8];
	y_pos = cpu->reg_v[(ins & 0x00f0) >> 0x4];
	sprite_rows = cpu->reg_v[ins & 0x000f];
	x_pos = x_pos % 64;
	y_pos = y_pos % 32;

	for (int spr_row = 0; spr_row < sprite_rows; spr_row++) {

	}

	return CPU_STATUS_SUCCESS;

}