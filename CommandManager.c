#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "stdio.h"
#include "CommandManager.h"

int* getValidModesForAction(const char *action, int *validModes){
    if (strcmp(action, ACTION_SOLVE) == 0) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_EDIT) == 0) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_MARK_ERRORS) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 0;
    }
    if (strcmp(action, ACTION_PRINT_BOARD) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_SET) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_VALIDATE) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_GENERATE) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 0;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_UNDO) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_REDO) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_SAVE) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_HINT) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 0;
    }
    if (strcmp(action, ACTION_NUM_SOLUTIONS) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_AUTOFILL) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 0;
    }
    if (strcmp(action, ACTION_RESET) == 0) {
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    if (strcmp(action, ACTION_EXIT) == 0) {
        validModes[INIT] = 1;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
    }
    return validModes;
}

Command* cmdMngr_fetchCommand() {
    char* cmd_str = malloc(1024 * sizeof(char));
    char** args = malloc(1024 * sizeof(char*));
    char* cmdpt = NULL;
    char* action = malloc(1024 * sizeof(char));
    char* arg;
    int* validModes;
    Command* cmd;
    int numOfArgs = 0;

    assert(cmd_str);
    assert(args);
    assert(action);

    /* Split input into tokens, cmdpt pointing to the first token. */
    while (cmdpt == NULL) {
        printf("Enter your command:\n");
        if (fgets(cmd_str, 1024, stdin) == NULL) {
            cmdpt = ACTION_EXIT;
            break;
        }
        cmdpt = strtok(cmd_str, " \t\r\n");
    }

    /* action holds the Command action name */
    strcpy(action, cmdpt);

    /* Setting args to hold the rest of the input */
    arg = strtok(NULL, " \t\r\n");
    while (arg != NULL) {
        args[numOfArgs] = (char*) malloc((strlen(arg) + 1) * sizeof(char));
        strcpy(args[numOfArgs], arg);
        arg = strtok(NULL, " \t\r\n");
        numOfArgs++;
    }
    validModes = (int*)calloc(3, sizeof(int));
    cmd = cmd_createCommand(args, action, getValidModesForAction(action, &validModes[0]), numOfArgs);
    free(cmd_str);
    return cmd;
}
