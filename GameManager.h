#ifndef SUDOKU_CONSOLE_GAMEMANAGER_H
#define SUDOKU_CONSOLE_GAMEMANAGER_H

#include "Game.h"
#include "CommandManager.h"
#include "Command.h"
#include "ErrorPrinter.h"
#include "CommandExecutions.h"
#include "Command.h"
#include "CommandValidations.h"
#include "SudokuBoard.h"

int startGame();
void gameLoop(Game* game);
void executeCommand(Game* game, Command * cmd);

#endif //SUDOKU_CONSOLE_GAMEMANAGER_H
