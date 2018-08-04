#ifndef SUDOKU_CONSOLE_GAME_H
#define SUDOKU_CONSOLE_GAME_H

#include "SudokuBoard.h"
#include "LinkedList.h"

#define INIT 0
#define SOLVE 1
#define EDIT 2

typedef struct {
    int mode;
    int rows;
    int columns;
    SudokuBoard* gameBoard;
    SudokuBoard* solvedBoard;
    LinkedList* undoRedoList;
} Game;

Game* createGame();
void destroyGame(Game * game);

#endif //SUDOKU_CONSOLE_GAME_H
