#pragma once
#include "chip8_cpu.h"
CPU_STATUS ins_type_zero(PCHIP8CPU, WORD);
CPU_STATUS ins_type_eight(PCHIP8CPU, WORD);
CPU_STATUS ins_type_dnnn(PCHIP8CPU, WORD);