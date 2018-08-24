#ifndef SUDOKU_CONSOLE_GAMEMANAGER_H
#define SUDOKU_CONSOLE_GAMEMANAGER_H

#include "Game.h"

int startGame();
void gameLoop(Game* game);
void executeCommand(Game* game, Command * cmd);

#endif //SUDOKU_CONSOLE_GAMEMANAGER_H
