#ifndef SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
#define SUDOKU_CONSOLE_COMMANDVALIDATIONS_H

#include "ErrorPrinter.h"
#include "Command.h"
#include "SudokuBoard.h"
#include "Game.h"

bool isNaN(char *arg);
bool validateSolve(Command* cmd);
bool validateEdit(Command* cmd);
bool validateMarkErrors(Command* cmd);
bool validatePrintBoard(Command* cmd);
bool validateSet(Command *cmd, Game* game);
bool validateValidate(Command* cmd);
bool validateGenerate(Command* cmd);
bool validateUndo(Command* cmd);
bool validateRedo(Command* cmd);
bool validateSave(Command* cmd);
bool validateHint(Command* cmd);
bool validateNumSolutions(Command* cmd);
bool validateAutofill(Command* cmd, Game* game);
bool validateReset(Command* cmd);
bool validateExit(Command* cmd);

#endif //SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
