#pragma once
#include <Windows.h>
#include "stack.h"

typedef unsigned int CPU_STATUS;

#define CPU_STATUS_SUCCESS (CPU_STATUS)0x00000000

#define CPU_GRAPHICS_UPDAT (CPU_STATUS)0x30000005
#define CPU_GRAPHICS_REDRW (CPU_STATUS)0x30000006
#define CPU_GRAPHICS_CLEAR (CPU_STATUS)0x30000007

#define CPU_STATUS_INVALID (CPU_STATUS)0xC0000001
#define CPU_STATUS_SIGSEGV (CPU_STATUS)0xC0000002
#define CPU_STATUS_FAIL_03 (CPU_STATUS)0xC0000003

typedef struct _CHIP8CPU {
	BYTE* base;
	WORD* program_counter;
	void* index_reg;
	BYTE* frame_buf;

	char timers[2];
	char reg_v[16];
	STACK stack;

} CHIP8CPU, *PCHIP8CPU;

PCHIP8CPU initialise_cpu(int);
CPU_STATUS fde_cycle(PCHIP8CPU);

