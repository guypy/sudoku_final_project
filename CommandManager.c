#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "stdio.h"
#include "CommandManager.h"
#include "Command.h"
#include "Game.h"
#include "CommandValidations.h"

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
        validModes[INIT] = 0;
        validModes[SOLVE] = 1;
        validModes[EDIT] = 1;
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

    int validModes[3];
    Command* cmd = cmd_createCommand(args, action, getValidModesForAction(action, validModes), numOfArgs);

    free(cmd_str);
    return cmd;
};
