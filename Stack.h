#ifndef SUDOKU_CONSOLE_STACK_H
#define SUDOKU_CONSOLE_STACK_H

/**
 * Stack Header File
 *
 * This interface declares a Stack and StackItem structs, as well as the functions that can be performed on them.
 *
 */

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
