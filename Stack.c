#include <stdlib.h>
#include "Stack.h"
#include "SudokuBoard.h"

Stack* stack_create(){
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->size = 0;
    stack->ptr = NULL;

    return stack;
}

void* stack_pop(Stack* stack){
    if (stack->size == 0)
        return NULL;
    StackItem* removedItem = stack->ptr;
    void* data = removedItem->data;
    stack->ptr = removedItem->prev;
    free(removedItem);
    stack->size--;

    return data;
}

void stack_push(Stack* stack, void* item){
    StackItem* stackItem = (StackItem*) malloc(sizeof(StackItem));
    stackItem->data = item;
    stackItem->prev = stack->ptr;
    stack->ptr = stackItem;
    stack->size++;
}

void stack_destroy(Stack* stack) {
    while (stack_pop(stack) != NULL);
    free(stack);
}