#ifndef SUDOKU_CONSOLE_STACK_H
#define SUDOKU_CONSOLE_STACK_H
#define INITIAL_MAX_LENGTH 1024

typedef struct StackItem{
    void* data;
    struct StackItem* prev;
} StackItem;

typedef struct {
    StackItem* ptr;
    int size;
} Stack;

Stack* stack_create();
void* stack_pop(Stack* stack);
void stack_push(Stack* stack, void* item);
void stack_destroy(Stack* stack);


#endif //SUDOKU_CONSOLE_STACK_H
