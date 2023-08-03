#include "../include/stack.h"

WORD spush(PSTACK stack, WORD val) {
	if (stack->size > 15) {
		return _STACK_OVRFLW;
	}
	stack->storage[stack->size] = val;
	stack->size++;
	return _STACK_OK;
}

WORD spop(PSTACK stack) {
	if (stack->size < 0) {
		return _STACK_POPFAIL;
	}
	int retval = stack->storage[--stack->size];
	return retval;
}

WORD init_stack(PSTACK stack) {
	memset(stack, 0, sizeof(STACK));
	return _STACK_OK;
}