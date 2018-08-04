//
// Created by Guy Pinchuk on 04/08/2018.
//

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

Game* createGame(){
    Game* game = (Game*) malloc(sizeof(Game));
    if (!game){ /* allocation failed */
        printf("Error: %s has failed\n", "sb_createSudokuBoard");
        exit(1);
    }
    game->mode = INIT;
    return game;
}

