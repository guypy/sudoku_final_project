#ifndef SUDOKU_CONSOLE_LINKEDLIST_H
#define SUDOKU_CONSOLE_LINKEDLIST_H

#include "Command.h"
#include "SudokuBoard.h"

typedef struct Node  {
    Command * data;
    struct Node* next;
    struct Node* prev;
    struct LinkedList* autoFillList;
    SudokuBoard* generatedBoard;
} Node;

typedef struct LinkedList {
    Node * head;
    Node * tail;
    int size;
} LinkedList;

LinkedList* createList();
void append(LinkedList * lst, Command * cmd);
void destroyNode(LinkedList *lst, Node *node);
void destroyFromNode(LinkedList *lst, Node *node);
void destroyList(LinkedList * lst);

#endif
