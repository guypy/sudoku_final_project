#ifndef SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
#define SUDOKU_CONSOLE_COMMANDVALIDATIONS_H

#include "Command.h"
#include "Game.h"

void executeSolve(Game* game, Command* cmd);
void executeEdit(Game* game, Command* cmd);
void executeMarkErrors(Game* game, Command* cmd);
void executePrintBoard(Game* game, Command* cmd);
void executeSet(Game* game, Command* cmd);
void executeValidate(Game* game, Command* cmd);
void executeGenerate(Game* game, Command* cmd);
void executeUndo(Game* game, Command* cmd);
void executeRedo(Game* game, Command* cmd);
void executeSave(Game* game, Command* cmd);
void executeHint(Game* game, Command* cmd);
void executeNumSolutions(Game* game, Command* cmd);
void executeAutofill(Game* game, Command* cmd);
void executeReset(Game* game, Command* cmd);

#endif //SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
