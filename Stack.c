#include <stdlib.h>
#include "Stack.h"
#include "SudokuBoard.h"

Stack* stack_create(){
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->size = 0;
    stack->ptr = NULL;

    return stack;
}

StackItem* stack_pop(Stack* stack){
    if (stack->size == 0)
        return NULL;
    StackItem* removedItem = stack->ptr;
    stack->ptr = removedItem->prev;
    stack->size--;

    return removedItem;
}

void stack_push(Stack* stack, int cellIdx, int value){
    StackItem* stackItem = (StackItem*) malloc(sizeof(StackItem));
    stackItem->cellIdx = cellIdx;
    stackItem->cellValue = value;
    stackItem->prev = stack->ptr;
    stack->ptr = stackItem;
    stack->size++;
}

void stack_destroy(Stack* stack) {
    StackItem* popped;
    while ((popped = stack_pop(stack)) != NULL) {
        free(popped);
    }
    free(stack);
}