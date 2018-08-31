#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"
#include "Command.h"

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
    if (lst->head == NULL)
        lst->head = node;
    if (lst->tail != NULL) {
        lst->tail->next = node;
    }
    lst->tail = node;
    lst->size++;
}

void delete(LinkedList * lst, Node* node) {
    if(lst->head == NULL || node == NULL)
        return;
    if(lst->head == node)
        lst->head = node->next;
    if(node->next != NULL)
        node->next->prev = node->prev;
    if(node->prev != NULL)
        node->prev->next = node->next;
    lst->size--;
    free(node);
}

void deleteFromNode(LinkedList* lst, Node *node) {
    if (node == NULL) {
        return;
    }
    deleteFromNode(lst, node->next);
    delete(lst, node);
}

void destroyList(LinkedList * lst) {
    deleteFromNode(lst, lst->head);
    free(lst);
}

//For Debugging purposes
void printList(LinkedList * lst) {
    Node * node = lst->head;
    while (node != NULL) {
        printf("%s", node->data->action);
        node = node->next;
    }
}