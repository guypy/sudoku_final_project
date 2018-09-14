#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"

LinkedList* createList() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void append(LinkedList * lst, Command * cmd) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->next = NULL;
    node->data = cmd;
    node->prev = lst->tail;
    node->autoFillList = NULL;
    if (lst->head == NULL)
        lst->head = node;
    if (lst->tail != NULL) {
        lst->tail->next = node;
    }
    lst->tail = node;
    lst->size++;
}

void destroyNode(LinkedList *lst, Node *node) {
    if(lst->head == NULL || node == NULL)
        return;
    if(lst->head == node){
        lst->head = node->next;
    }
    if(node->next != NULL){
        node->next->prev = node->prev;
    }
    if(node->prev != NULL){
        node->prev->next = node->next;
    }
    lst->size--;

    destroyList(node->autoFillList);

    if (node->data != NULL) cmd_freeCommand(node->data);

    free(node);
}

void destroyFromNode(LinkedList *lst, Node *node) {
    if (node == NULL) {
        return;
    }
    destroyFromNode(lst, node->next);
    destroyNode(lst, node);
}

void destroyList(LinkedList * lst) {
    if (lst == NULL){
        return;
    }
    destroyFromNode(lst, lst->head);
    free(lst);
}
