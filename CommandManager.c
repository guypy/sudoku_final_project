//
// Created by Guy Pinchuk on 04/08/2018.
//
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stdio.h"
#include "CommandManager.h"
#include "Command.h"
#include "Game.h"
#include "CommandValidations.h"

static bool validateArguments(char** args, char* cmd) {
    if (strcmp(cmd, ACTION_SOLVE) != 0) {
        return validateSolve(args, cmd);
    }
    if (strcmp(cmd, ACTION_MARK_ERRORS) != 0) {
        return validateMarkErrors(args, cmd);
    }
    if (strcmp(cmd, ACTION_PRINT_BOARD) != 0) {
        return validatePrintBoard(args, cmd);
    }
    if (strcmp(cmd, ACTION_SET) != 0) {
        return validateSet(args, cmd);
    }
    if (strcmp(cmd, ACTION_VALIDATE) != 0) {
        return validateValidate(args, cmd);
    }
    if (strcmp(cmd, ACTION_GENERATE) != 0) {
        return validateGenerate(args, cmd);
    }
    if (strcmp(cmd, ACTION_UNDO) != 0) {
        return validateUndo(args, cmd);
    }
    if (strcmp(cmd, ACTION_REDO) != 0) {
        return validateRedo(args, cmd);
    }
    if (strcmp(cmd, ACTION_SAVE) != 0) {
        return validateSave(args, cmd);
    }
    if (strcmp(cmd, ACTION_HINT) != 0) {
        return validateHint(args, cmd);
    }
    if (strcmp(cmd, ACTION_NUM_SOLUTIONS) != 0) {
        return validateNumSolutions(args, cmd);
    }
    if (strcmp(cmd, ACTION_AUTOFILL) != 0) {
        return validateAutofill(args, cmd);
    }
    if (strcmp(cmd, ACTION_RESET) != 0) {
        return validateReset(args, cmd);
    }
    printf("Error: invalid command");
    return false;
}

int* getValidModesForAction(const char *action, int *validModes){
    if (action == ACTION_SOLVE) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
    }
    if (action == ACTION_MARK_ERRORS) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
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
        validModes[INIT] = 1;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
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
        validModes[SOLVE] = 0;
        validModes[EDIT] = 0;
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
    int i = 0;
    if (!cmd_str){ /* allocation failed */
        printf("Error: %s has failed\n", "prsr_fetchCmd");
        exit(1);
    }

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
        args[i] = arg;
        arg = strtok(NULL, " \t\r\n");
        i++;
    }

    if (validateArguments(args, action) == 0) {
        printf("ERROR: invalid command\n");
        return NULL;
    }

    int validModes[3];
    Command* cmd = cmd_createCommand(args, action, getValidModesForAction(action, validModes));

    free(cmd_str);
    return cmd;

};
