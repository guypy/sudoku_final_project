#include <stdlib.h>
#include <stdio.h>
#include "BTSolver.h"

void clearExhaustedValues(Cell* cell, int len){
    int i;
    for (i = 0; i < len; i++) {
        cell->exhaustedValues[i] = 0;
    }
}

void setAllFilledCellsAsFixed(SudokuBoard* board){
    int i;
    const int boardSize = board->blockColumns * board->blockColumns * board->blockRows * board->blockRows;
    for (i=0; i < boardSize; i++) {
        if (board->cells[i]->value != 0)
            board->cells[i]->fixed = 1;
    }
}

int BT_numberOfSolutions(SudokuBoard* board){
    int i = 0, numOfSolutions = 0, value;
    const int boardSize = board->blockColumns * board->blockColumns * board->blockRows * board->blockRows;
    Stack * stack = stack_create();
    StackItem* popped;
    Cell* cell;
    board = sb_deepCloneBoard(board);
    setAllFilledCellsAsFixed(board);

    while (i < boardSize && board->cells[i]->fixed != 0){
        ++i;
    }

    while (true) {
        if (i == boardSize){
            ++numOfSolutions;
        } else {
            /*Pushing all non-exhausted values of the cell to the stack.*/
            for (value = 1; value < board->blockRows * board->blockColumns + 1; value++) {
                if (board->cells[i]->exhaustedValues[value - 1] == true) {
                    continue;
                }
                if (!cell_isValid(board, value, i)){
                    continue;
                }
                stack_push(stack, i, value);
            }
        }

        popped = stack_pop(stack);

        if (popped == NULL)
            break;

        while (popped->cellIdx < i) {
            if (i < boardSize) {
                clearExhaustedValues(board->cells[i], board->blockRows * board->blockColumns);
                board->cells[i]->value = 0;
            }
            --i;
        }

        board->cells[popped->cellIdx]->value = popped->cellValue;
        board->cells[popped->cellIdx]->exhaustedValues[popped->cellValue - 1] = true;

        i = popped->cellIdx + 1;
        free(popped);
        while (i < boardSize && board->cells[i]->fixed != 0){
            ++i;
        }
    }

    stack_destroy(stack);
    sb_destroyBoard(board);
    return numOfSolutions;
}
