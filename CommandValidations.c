#include <stdbool.h>
#include <stdlib.h>

bool validateSolve(char** args, char* cmd, int numOfArgs) {
    if (numOfArgs < 1)
        return false;
    return true;
}

bool validateEdit(char** args, char* cmd, int numOfArgs) {
    return true;
}

bool validateMarkErrors(char** args, char* cmd, int numOfArgs) {
    //One argument - can only be 0 or 1.
    if (numOfArgs != 1)
        return false;
    int arg = atoi(args[0]);
    if (arg != 0 && arg != 1)
        return false;
    return true;
}

bool validatePrintBoard(char** args, char* cmd, int numOfArgs) {
    //No Validations For this command.
    return true;
}

bool validateSet(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Set..
    return true;
}

bool validateValidate(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Validate..
    return true;
}

bool validateGenerate(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Generate..
    return true;
}

bool validateUndo(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Undo..
    return true;
}

bool validateRedo(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Redo..
    return true;
}
bool validateSave(char** args, char* cmd, int numOfArgs) {
    if (numOfArgs < 1)
        return false;
    return true;
}
bool validateHint(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Hint..
    return true;
}
bool validateNumSolutions(char** args, char* cmd, int numOfArgs) {
    //Here we will validate NumSolutions..
    return true;
}
bool validateAutofill(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Autofill..
    return true;
}
bool validateReset(char** args, char* cmd, int numOfArgs) {
    //Here we will validate Reset..
    return true;
}
bool validateExit(char** args, char* cmd, int numOfArgs) {
    //No Validations For this command.
    return true;
}