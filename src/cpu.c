#include "../include/chip8_cpu.h"
#include "../include/chip8_cpu_internal.h"
#include "../include/stack.h"

CPU_FRAME_STATUS write_frame_buf(BYTE* frame_buf, int x_pos, int y_pos, int pixel_val) {
	// assuming blank display, pixel_offset = offset from x = 0
	// for this example pixel_offset is 1, and origin is 0
	// therefore we want 011111111 pixel display

	int s_flag = CPU_FRAME_VF_0;

	int pixel_offset = x_pos % 8;

	int x_raw = x_pos / 8;
	int y_raw = y_pos * 8;

	DWORD new_pixel = pixel_val >> pixel_offset;
	DWORD old_pixel = frame_buf[(y_raw) + (x_raw)];

	if ((new_pixel ^ old_pixel) != new_pixel) {
		frame_buf[(y_raw)+(x_raw)] = frame_buf[(y_raw) + (x_raw)] ^ (pixel_val >> pixel_offset);
		s_flag = CPU_FRAME_VF_1;
	}
	else {
		frame_buf[(y_raw)+(x_raw)] = pixel_val >> pixel_offset;
	}
	new_pixel = (pixel_val << (32 - pixel_offset) >> 24) ^ frame_buf[(y_raw)+(x_raw)+1];
	old_pixel = frame_buf[(y_raw)+(x_raw) + 1];

	if (new_pixel == old_pixel) return s_flag;

	if ((new_pixel ^ old_pixel) != new_pixel) {
		frame_buf[(y_raw)+(x_raw)+1] = (BYTE)new_pixel;
		s_flag = CPU_FRAME_VF_1;
	}
	else {
		frame_buf[(y_raw)+(x_raw)+1] = (BYTE)new_pixel;
	}
	return s_flag;
}

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
	case 0x2000:
		spush(&cpu->stack, (WORD)cpu->program_counter);
		cpu->program_counter = (WORD*)(ins & 0x0fff);
		return CPU_STATUS_SUCCESS;

	case 0x3000:
		if (cpu->reg_v[((ins & 0x0f00) >> 0x8)] == (ins & 0xff))
			cpu->program_counter++;
		return CPU_STATUS_SUCCESS;

	case 0x4000:
		if (cpu->reg_v[((ins & 0x0f00) >> 0x8)] != (ins & 0xff))
			cpu->program_counter++;
		return CPU_STATUS_SUCCESS;

	case 0x5000:
		if (cpu->reg_v[((ins & 0x0f00) >> 0x8)] == cpu->reg_v[((ins & 0x00f0) >> 0x4)])
			cpu->program_counter++;
		return CPU_STATUS_SUCCESS;

	case 0x6000:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = ins & 0x00ff;
		return CPU_STATUS_SUCCESS;
	case 0x7000:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] += ins & 0x00ff;
		return CPU_STATUS_SUCCESS;

	case 0x8000:
		return ins_type_eight(cpu, ins);

	case 0x9000:
		if (cpu->reg_v[((ins & 0x0f00) >> 0x8)] != cpu->reg_v[((ins & 0x00f0) >> 0x4)])
			cpu->program_counter++;
		return CPU_STATUS_SUCCESS;

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

CPU_STATUS ins_type_eight(PCHIP8CPU cpu, WORD ins) {
	WORD ins_type = ins & 0x000F;
	BYTE reg_x, reg_y;
	DWORD scratch_reg;
	reg_x = cpu->reg_v[(ins & 0x0f00) >> 0x8];
	reg_y = cpu->reg_v[(ins & 0x00f0) >> 0x4];

	switch (ins_type) {
	case 0x0000:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = reg_y;
		return CPU_STATUS_SUCCESS;
	case 0x0001:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = reg_x | reg_y;
		return CPU_STATUS_SUCCESS;
	case 0x0002:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = reg_x & reg_y;
		return CPU_STATUS_SUCCESS;
	case 0x0003:
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = reg_x ^ reg_y;
		return CPU_STATUS_SUCCESS;
	case 0x0004:
		scratch_reg = reg_x + reg_y;
		cpu->reg_v[(ins & 0x0f00) >> 0x8] = (BYTE)scratch_reg;
		if (scratch_reg > 255)
			cpu->reg_v[0xf] = 0x1;
		else
			cpu->reg_v[0xf] = 0x0;
		return CPU_STATUS_SUCCESS;
	}

	return CPU_STATUS_SUCCESS;
}

CPU_STATUS ins_type_dnnn(PCHIP8CPU cpu, WORD ins) {
	cpu->reg_v[0xf] = 0;
	WORD x_pos, y_pos, sprite_rows;
	BYTE* ind_ptr = (BYTE*)(cpu->base + (int)cpu->index_reg);

	x_pos = cpu->reg_v[(ins & 0x0f00) >> 0x8];
	y_pos = cpu->reg_v[(ins & 0x00f0) >> 0x4];
	sprite_rows = ins & 0x000f;
	x_pos = x_pos % 64;
	y_pos = y_pos % 32;

	for (int spr_row = 0; spr_row < sprite_rows; spr_row++) {
		BYTE pixel_draw = ind_ptr[spr_row];
		if (write_frame_buf(cpu->frame_buf, x_pos, y_pos + spr_row, pixel_draw) == CPU_FRAME_VF_1) {
			cpu->reg_v[0xf] = 0x1;
		}
	}

	return CPU_GRAPHICS_REDRW;

}