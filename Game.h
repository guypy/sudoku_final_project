#ifndef SUDOKU_CONSOLE_GAME_H
#define SUDOKU_CONSOLE_GAME_H

#include <stdbool.h>
#include "SudokuBoard.h"
#include "LinkedList.h"

/**
 * Game Header File
 *
 * This interface declares a game struct, and the functions needed to create it, destroy it, and restart it.
 * It also defines constants for the possible modes of the game.
 *
 * Each game contains the following:
 * mode - integer describing the current mode of the game according to the constants defined.
 * markErrors - boolean representing whether or not erroneous values will be printed with an "*" mark when the board
 * is printed.
 * undoRedoList - list of moves that change the board of the game, and can be undone/repeated afterwards.
 * undoRedoListPointer - the current command in the undoRedoList the game is on - meaning the last command that had
 * been executed that changed the board.
 *
 */

#define INIT 0
#define SOLVE 1
#define EDIT 2

#define DEFAULT_BLOCK_SIZE 3

typedef struct {
    int mode;
    bool markErrors;
    SudokuBoard* board;
    LinkedList* undoRedoList;
    Node* undoRedoListPointer;
} Game;

Game* createGame();
void destroyGame(Game * game);
void restartGame(Game* game);

#endif
