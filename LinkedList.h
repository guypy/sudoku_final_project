//
// Created by Guy Pinchuk on 04/08/2018.
//

#ifndef SUDOKU_CONSOLE_LINKEDLIST_H
#define SUDOKU_CONSOLE_LINKEDLIST_H

#include "Command.h"

typedef struct Node  {
    Command * data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node * head;
    Node * tail;
    int size;
} LinkedList;

void append(LinkedList * lst, Command * cmd);
void delete(LinkedList * lst, Node* node);
void deleteFromNode(LinkedList* lst, Node *node);
void destroyList(LinkedList * lst);
void printList(LinkedList * lst);


#endif //SUDOKU_CONSOLE_LINKEDLIST_H
