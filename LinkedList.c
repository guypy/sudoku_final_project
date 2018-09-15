#include <stdlib.h>
#include "LinkedList.h"

/**
 *
 * LinkedList Source File
 *
 * This file contains the implementation of functions that can be performed on a linked list struct.
 *
 */

/**
 * This function createes an emply linked list.
 * @return - pointer to struct of an empty LinkedList.
 */
LinkedList* createList() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/**
 * This functon appends a new node containing a given command in the game to a given LinkedList.
 * @param lst - pointer to LinkedList struct
 * @param cmd - pointer to command to be added within a Node to the LinkedList struct.
 */
void append(LinkedList * lst, Command * cmd) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->next = NULL;
    node->data = cmd;
    node->prev = lst->tail;
    node->autoFillList = NULL;
    node->generatedBoard = NULL;

    if (lst->head == NULL)
        lst->head = node;
    if (lst->tail != NULL) {
        lst->tail->next = node;
    }
    lst->tail = node;
    lst->size++;
}

/**
 * This function destroys a node within a LinkedList and updates the list accordingly.
 * @param lst - pointer to LinkedList struct.
 * @param node - pointer to node to remove from the list.
 */
void destroyNode(LinkedList *lst, Node *node) {
    if(lst->head == NULL || node == NULL)
        return;
    if(lst->head == node){
        lst->head = node->next;
    }
    if(lst->tail == node) {
        lst->tail = node->prev;
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
    if (node->generatedBoard != NULL) sb_destroyBoard(node->generatedBoard);

    free(node);
}

/**
 * This function destroys all nodes starting from a given node within a LinkedList.
 * @param lst - pointer to LinkedList struct.
 * @param node - pointer to node from which to desetroy all ndoes in the list.
 */
void destroyFromNode(LinkedList *lst, Node *node) {
    if (node == NULL) {
        return;
    }
    destroyFromNode(lst, node->next);
    destroyNode(lst, node);
}

/**
 * This function destroys a given LinkedList and all its memory allocations.
 * @param lst - pointer to LinkedList struct to destroy.
 */
void destroyList(LinkedList * lst) {
    if (lst == NULL){
        return;
    }
    destroyFromNode(lst, lst->head);
    free(lst);
}
