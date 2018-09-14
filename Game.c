#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

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
    return game;
}

void destroyGame(Game* game) {
    destroyList(game->undoRedoList);
    if (game->board != NULL) sb_destroyBoard(game->board);
    free(game);
}

void restartGame(Game* game){
    game->mode = INIT;
    destroyList(game->undoRedoList);
    game->undoRedoList = createList();
    game->markErrors = true;
    game->undoRedoListPointer = game->undoRedoList->tail;
    sb_destroyBoard(game->board);
}
