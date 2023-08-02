#pragma once
#include <Windows.h>

typedef struct _stack {
	WORD storage[16];
	int size;
} STACK, *PSTACK;

WORD spop(PSTACK);
WORD spush(PSTACK, WORD);
WORD init_stack(PSTACK);
