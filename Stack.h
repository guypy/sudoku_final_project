#ifndef SUDOKU_CONSOLE_STACK_H
#define SUDOKU_CONSOLE_STACK_H

typedef struct StackItem{
    int cellIdx;
    int cellValue;
    struct StackItem* prev;
} StackItem;

typedef struct {
    StackItem* ptr;
    int size;
} Stack;

Stack* stack_create();
StackItem* stack_pop(Stack* stack);
void stack_push(Stack* stack, int cellIdx, int value);
void stack_destroy(Stack* stack);


#endif
