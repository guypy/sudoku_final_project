#ifndef SUDOKU_CONSOLE_COMMANDEXECUTIONS_H
#define SUDOKU_CONSOLE_COMMANDEXECUTIONS_H

#include "Game.h"
#include "Command.h"
#include "CommandManager.h"
#include "FileHandler.h"
#include "ErrorPrinter.h"
#include "SudokuBoard.h"
#include "BTSolver.h"
#include "ILPSolver.h"
#include "ExecutionHelpers.h"

void executeSolve(Game* game, Command* cmd);
void executeEdit(Game* game, Command* cmd);
void executeMarkErrors(Game* game, Command* cmd);
void executePrintBoard(Game* game);
void executeSet(Game* game, Command* cmd);
void executeValidate(Game* game);
void executeGenerate(Game* game, Command* cmd);
void executeUndo(Game *game, bool shouldPrint);
void executeRedo(Game* game);
void executeSave(Game* game, Command* cmd);
void executeHint(Game* game, Command* cmd);
void executeNumSolutions(Game* game);
void executeAutofill(Game* game, Command* cmd);
void executeReset(Game* game);

#endif
