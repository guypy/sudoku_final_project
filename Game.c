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
    return game;
}

void destroyGame(Game* game) {
    destroyList(game->undoRedoList);
    sb_destroyBoard(game->board);
    sb_destroyBoard(game->solvedBoard);
    free(game);
}
