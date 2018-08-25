#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "CommandValidations.h"
#include "ErrorPrinter.h"

bool validateSolve(char** args, char* action, int numOfArgs) {
    if (numOfArgs < 1)
        return false;
    return true;
}

bool validateEdit(char** args, char* action, int numOfArgs) {
    return true;
}

bool validateMarkErrors(char** args, char* action, int numOfArgs) {
    //One argument - can only be 0 or 1.
    if (numOfArgs != 1)
        return false;
    if (isNaN(args[0]))
        return false;
    int arg = atoi(args[0]);
    if (arg != 0 && arg != 1)
        return false;
    return true;
}

bool validatePrintBoard(char** args, char* action, int numOfArgs) {
    //No Validations For this command.
    return true;
}

bool validateSet(char** args, char* action, int numOfArgs) {
    int i, arg;
    for (i = 0; i < numOfArgs; ++i){
        if (isNaN(args[0])){
            errPrinter_valueNotInRange(0);
        }
    }
    return true;
}

bool validateValidate(char** args, char* action, int numOfArgs) {
    //Here we will validate Validate..
    return true;
}

bool validateGenerate(char** args, char* action, int numOfArgs) {
    //Here we will validate Generate..
    return true;
}

bool validateUndo(char** args, char* action, int numOfArgs) {
    //Here we will validate Undo..
    return true;
}

bool validateRedo(char** args, char* action, int numOfArgs) {
    //Here we will validate Redo..
    return true;
}
bool validateSave(char** args, char* action, int numOfArgs) {
    //Here we will validate Save..
    return true;
}
bool validateHint(char** args, char* action, int numOfArgs) {
    //Here we will validate Hint..
    return true;
}
bool validateNumSolutions(char** args, char* action, int numOfArgs) {
    //Here we will validate NumSolutions..
    return true;
}
bool validateAutofill(char** args, char* action, int numOfArgs) {
    //Here we will validate Autofill..
    return true;
}
bool validateReset(char** args, char* action, int numOfArgs) {
    //Here we will validate Reset..
    return true;
}
bool validateExit(char** args, char* action, int numOfArgs) {
    //No Validations For this command.
    return true;
}
bool isNaN(char *arg){
    int i;
    char c;
    for (i = 0; i < strlen(arg); ++i){
        c = arg[i];
        if (c < 48 || c > 57){
            return true;
        }
    }
    return false;
}
