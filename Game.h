#ifndef SUDOKU_CONSOLE_GAME_H
#define SUDOKU_CONSOLE_GAME_H

#include <stdbool.h>
#include "SudokuBoard.h"
#include "LinkedList.h"

#define INIT 0
#define SOLVE 1
#define EDIT 2

#define DEFAULT_BLOCK_SIZE 3

typedef struct {
    int mode;
    bool markErrors;
    SudokuBoard* board;
    SudokuBoard* solvedBoard;
    LinkedList* undoRedoList;
    Node* undoRedoListPointer;
} Game;

Game* createGame();
void destroyGame(Game * game);
void restartGame(Game* game);

#endif
