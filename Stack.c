#include <stdlib.h>
#include "Stack.h"

/**
 *
 * Stack Source File
 *
 * This file contains the implementation of functions that can be performed on a Stack struct.
 *
 */

/**
 * This fuction creates an empty stack struct.
 * @return - pointer to struct of an empty stack.
 */
Stack* stack_create(){
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->size = 0;
    stack->ptr = NULL;

    return stack;
}

/**
 * This function removes the value at the top of the stack and returns a pointer to it.
 * @param stack - pointer to stack.
 * @return - Pointer to popped item in stack.
 */
StackItem* stack_pop(Stack* stack){
    StackItem* removedItem;
    if (stack->size == 0)
        return NULL;

    removedItem = stack->ptr;
    stack->ptr = removedItem->prev;
    stack->size--;

    return removedItem;
}

/**
 * This function pushes a new item, to the top of a given stack
 * @param stack - pointer to stack to add new item to.
 * @param cellIdx - index of cell to enter to StackItem
 * @param value - value of cell to enter to StackItem
 */
void stack_push(Stack* stack, int cellIdx, int value){
    StackItem* stackItem = (StackItem*) malloc(sizeof(StackItem));
    stackItem->cellIdx = cellIdx;
    stackItem->cellValue = value;
    stackItem->prev = stack->ptr;
    stack->ptr = stackItem;
    stack->size++;
}

/**
 * This function destroys a given stack and all its allocated memory.
 * @param stack - pointer to stack to destroy.
 */
void stack_destroy(Stack* stack) {
    StackItem* popped;
    while ((popped = stack_pop(stack)) != NULL) {
        free(popped);
    }
    free(stack);
}
