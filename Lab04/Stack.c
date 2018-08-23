/*
 * File: Stack.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on January 30, 2016, 4:20 PM
 */

// ***Include libraries here ***
#include "Stack.h"
#include "stdio.h"
#include "stdlib.h"

// **** Define global, module-level, or external variables here ****

/**
 * This function initializes the stack. For a statically allocated stack such
 * as this, this just means initializing currentItemIndex to -1 and setting
 * initialized to TRUE.
 */
void StackInit(struct Stack *stack)
{
    int s;
    for (s = 0; s < 21; s++) {
        stack->stackItems[s] = 0;
    }

    stack->currentItemIndex = -1;
    stack->initialized = TRUE;

    return;
}

/**
 * This function pushes a value onto the stack. It returns SUCCESS for success
 * and STANDARD_ERROR for a failure. A failure occurs when trying to push onto full or
 * non-initialized stacks. (SUCCESS and STANDARD_ERROR are declared in the BOARD.h header file.
 */
int StackPush(struct Stack *stack, float value)
{
    int stackptr = 0;
    if (stack->currentItemIndex == -1) {
        stackptr = 0;
        stack->currentItemIndex = 0;
    } else {
        stack->currentItemIndex++;
        stackptr = stack->currentItemIndex;
    }
    stack->stackItems[stackptr] = value;

    if (StackIsFull(stack) == TRUE) {
        return STANDARD_ERROR;
    } else if (stack->initialized != TRUE) {
        return STANDARD_ERROR;
    } else {
        return SUCCESS;
    }
}

/**
 * This function returns the top of the stack into the value point location.
 * It also returns a success code: SUCCESS for success and STANDARD_ERROR for failure.
 * Failure occurs when Pop() is called on empty stacks or non-initialized
 * stacks.
 *
 * NOTE: When trying to return the output into the value variable, you'll need
 * to write it with an extra asterisk like:
 *   *value = 7.0;
 * This is because of pointers and K&R 5.1-5.2 should explain it. This is the
 * only time in your lab where you'll need to do something like that for an
 * assignment operation.
 */
int StackPop(struct Stack *stack, float *value)
{
    int stackptr = stack->currentItemIndex;
    *value = stack->stackItems[stackptr];
    stack->currentItemIndex--;

    return *value;
}

/**
 * This function checks for whether the stack is empty or not. In this simple
 * case this merely checks if the currentItemIndex == -1. Returns TRUE if the
 * stack is empty, FALSE if not. Also needs to check if the stack is
 * initialized and return FALSE if it isn't.
 */
int StackIsEmpty(const struct Stack *stack)
{
    if (stack->currentItemIndex == -1) {
        return TRUE;
    } else {
        return FALSE;
    }

    if (stack->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * This function checks for whether the stack is empty or not. For this simple
 * implementation that is merely if the currentItemIndex == STACK_SIZE - 1. 
 * Returns TRUE if the stack is full and FALSE if not. Also returns FALSE if
 * the stack isn't initialized.
 */
int StackIsFull(const struct Stack *stack)
{
    if (stack->currentItemIndex == (STACK_SIZE - 1)) {
        return TRUE;
    } else {
        return FALSE;
    }

    if (stack->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Returns the current size of the stack in terms of how many active elements
 * are in it. Returns an error of SIZE_ERROR for uninitialized stacks, 0 for
 * empty and initialized stacks, and the number of elements in the stack
 * otherwise. SIZE_ERROR is declared in the BOARD.h header file.
 */
int StackGetSize(const struct Stack *stack)
{
    if (stack->initialized != TRUE) {
        return SIZE_ERROR;
    } else {
        if (StackIsEmpty(stack) == TRUE) {
            return 0;
        } else {
            int current_stackptr = stack->currentItemIndex;
            return current_stackptr;
        }
    }
}
