#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

/**
 * Game Source File
 *
 * This file contains the implementation of the functions that can be done on a game struct.
 *
 */

/**
 * This function creates a game struct, with the default settings.
 * @return - pointer to game struct.
 */
Game* createGame(){
    Game* game = (Game*) malloc(sizeof(Game));
    if (!game){ /* allocation failed */
        printf("Error: %s has failed\n", "sb_create");
        exit(1);
    }
    game->mode = INIT;
    game->undoRedoList = createList();
    game->markErrors = true;
    game->undoRedoListPointer = game->undoRedoList->tail;
    game->board = NULL;
    return game;
}

/**
 * This function destroys a given game struct and all its allocated memory.
 * @param game - pointer to game struct to destroy.
 */
void destroyGame(Game* game) {
    destroyList(game->undoRedoList);
    if (game->board != NULL) sb_destroyBoard(game->board);
    free(game);
}

/**
 * This function restarts a game by setting it back to its default parameters (INIT mode, markErrors is true,
 * empty undoRedoList, no board)
 * @param game
 */
void restartGame(Game* game){
    if (game == NULL){
        return;
    }
    game->mode = INIT;
    destroyList(game->undoRedoList);

    game->undoRedoList = createList();
    game->markErrors = true;
    game->undoRedoListPointer = game->undoRedoList->tail;
    sb_destroyBoard(game->board);
}
