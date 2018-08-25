#ifndef SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
#define SUDOKU_CONSOLE_COMMANDVALIDATIONS_H

bool isNaN(char *arg);
bool validateSolve(char** args, char* cmd, int numOfArgs);
bool validateEdit(char** args, char* cmd, int numOfArgs);
bool validateMarkErrors(char** args, char* cmd, int numOfArgs);
bool validatePrintBoard(char** args, char* cmd, int numOfArgs);
bool validateSet(char** args, char* cmd, int numOfArgs);
bool validateValidate(char** args, char* cmd, int numOfArgs);
bool validateGenerate(char** args, char* cmd, int numOfArgs);
bool validateUndo(char** args, char* cmd, int numOfArgs);
bool validateRedo(char** args, char* cmd, int numOfArgs);
bool validateSave(char** args, char* cmd, int numOfArgs);
bool validateHint(char** args, char* cmd, int numOfArgs);
bool validateNumSolutions(char** args, char* cmd, int numOfArgs);
bool validateAutofill(char** args, char* cmd, int numOfArgs);
bool validateReset(char** args, char* cmd, int numOfArgs);
bool validateExit(char** args, char* cmd, int numOfArgs);

#endif //SUDOKU_CONSOLE_COMMANDVALIDATIONS_H
