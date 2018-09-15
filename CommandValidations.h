#ifndef SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
#define SUDOKU_CONSOLE_COMMANDVALIDATIONS_H

#include "ErrorPrinter.h"
#include "Command.h"
#include "SudokuBoard.h"
#include "Game.h"

bool validateSolve(Command* cmd);
bool validateEdit();
bool validateMarkErrors(Command* cmd);
bool validatePrintBoard();
bool validateSet(Command *cmd, Game* game);
bool validateValidate();
bool validateGenerate(Command* cmd, Game* game);
bool validateUndo();
bool validateRedo();
bool validateSave(Command* cmd);
bool validateHint(Command* cmd, Game* game);
bool validateNumSolutions();
bool validateAutofill();
bool validateReset();
bool validateExit();

#endif
