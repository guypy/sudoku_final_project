//
// Created by Guy Pinchuk on 04/08/2018.
//

#ifndef SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
#define SUDOKU_CONSOLE_COMMANDVALIDATIONS_H

bool validateSolve(char** args, char* cmd);
bool validateEdit(char** args, char* cmd);
bool validateMarkErrors(char** args, char* cmd);
bool validatePrintBoard(char** args, char* cmd);
bool validateSet(char** args, char* cmd);
bool validateValidate(char** args, char* cmd);
bool validateGenerate(char** args, char* cmd);
bool validateUndo(char** args, char* cmd);
bool validateRedo(char** args, char* cmd);
bool validateSave(char** args, char* cmd);
bool validateHint(char** args, char* cmd);
bool validateNumSolutions(char** args, char* cmd);
bool validateAutofill(char** args, char* cmd);
bool validateReset(char** args, char* cmd);
bool validateExit(char** args, char* cmd);

#endif //SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
