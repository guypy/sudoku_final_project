#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "stdio.h"
#include "CommandManager.h"
#include "Command.h"
#include "Game.h"
#include "CommandValidations.h"

static bool validateArguments(char** args, char* action, int numOfArgs) {
    if (strcmp(action, ACTION_SOLVE) == 0) {
        return validateSolve(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_EDIT) == 0) {
        return validateEdit(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_MARK_ERRORS) == 0) {
        return validateMarkErrors(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_PRINT_BOARD) == 0) {
        return validatePrintBoard(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_SET) == 0) {
        return validateSet(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_VALIDATE) == 0) {
        return validateValidate(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_GENERATE) == 0) {
        return validateGenerate(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_UNDO) == 0) {
        return validateUndo(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_REDO) == 0) {
        return validateRedo(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_SAVE) == 0) {
        return validateSave(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_HINT) == 0) {
        return validateHint(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_NUM_SOLUTIONS) == 0) {
        return validateNumSolutions(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_AUTOFILL) == 0) {
        return validateAutofill(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_RESET) == 0) {
        return validateReset(args, action, numOfArgs);
    }
    if (strcmp(action, ACTION_EXIT) == 0) {
        return validateExit(args, action, numOfArgs);
    }
    return false;
}

int* getValidModesForAction(const char *action, int *validModes){
    if (action == ACTION_SOLVE) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (action == ACTION_EDIT) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (action == ACTION_MARK_ERRORS) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_PRINT_BOARD) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (action == ACTION_SET) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_VALIDATE) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_GENERATE) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_UNDO) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_REDO) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_SAVE) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (action == ACTION_HINT) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_NUM_SOLUTIONS) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_AUTOFILL) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_RESET) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_EXIT) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    return validModes;
}

Command* cmdMngr_fetchCommand() {
    printf("Enter your command:\n");
    char* cmd_str = malloc(1024 * sizeof(char));
    char** args = malloc(1024 * sizeof(char));
    char* cmdpt = NULL;
    char* action = malloc(1024 * sizeof(char));
    char* arg;
    int numOfArgs = 0;

    assert(cmd_str);
    assert(args);
    assert(action);

    //Split input into tokens, cmdpt pointing to the first token.
    while (cmdpt == NULL) {
        fgets(cmd_str, 1024, stdin);
        cmdpt = strtok(cmd_str, " \t\r\n");
    }

    //action holds the Command action name
    strcpy(action, cmdpt);

    //Setting args to hold the rest of the input
    arg = strtok(NULL, " \t\r\n");
    while (arg != NULL) {
        args[numOfArgs] = arg;
        arg = strtok(NULL, " \t\r\n");
        numOfArgs++;
    }

    if (validateArguments(args, action, numOfArgs) == 0) {
        return NULL;
    }

    int validModes[3];
    Command* cmd = cmd_createCommand(args, action, getValidModesForAction(action, validModes), numOfArgs);

    free(cmd_str);
    return cmd;
};
