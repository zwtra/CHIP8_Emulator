#include "../include/stack.h"

WORD spush(PSTACK stack, WORD val) {
	if (stack->size > 15) {
		return 0x1;
	}
	stack->storage[stack->size] = val;
	stack->size++;
	return 0;
}

WORD spop(PSTACK stack) {
	if (stack->size < 0) {
		return 0;
	}
	int retval = stack->storage[--stack->size];
	return retval;
}

WORD init_stack(PSTACK stack) {
	memset(stack, 0, sizeof(STACK));
}