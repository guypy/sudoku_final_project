#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stdio.h"
#include "CommandManager.h"

/**
 * Command Manager Source File
 *
 * This file implements managing the fetching of commands entered by the user, as well as converting the to the
 * relevant command structs.
 *
 */

/**
 * This function returns a boolean array of valid game modes for a given "action". Indeces of the array correspond
 * to game modes.
 *
 * @param action - String, describing what the command that is being created "does". E.g. "set"
 * @param validModes - boolean array of valid game modes. Indeces correspond to game modes.
 * @return validModes
 */
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

/**
 * This function advances the current character pointer in the file until a newline marker or an EOF is reached.
 * It is used in case a fetched command expands across more than the given buffer size in the function
 * cmdMngr_fetchCommand().
 */
void emptyBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/**
 * This function fetches a command from the user, and creates the corresponding command struct.
 *
 * @return - command struct corresponding to the information entered by the user.
 */
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
            sprintf(cmd_str, "%s\n", ACTION_EXIT);
        }
        if(strlen(cmd_str) > 256)
            emptyBuffer();
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
