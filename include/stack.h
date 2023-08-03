#pragma once
#include <Windows.h>

#define _STACK_OK		(WORD)0x0000

#define _STACK_POPFAIL	(WORD)0x0000
#define _STACK_OVRFLW	(WORD)0xc00d

typedef struct _stack {
	WORD storage[16];
	int size;
} STACK, *PSTACK;

WORD spop(PSTACK);
WORD spush(PSTACK, WORD);
WORD init_stack(PSTACK);
