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

#define ACTION_SOLVE "solve"

static bool validateSolve(char** args, char* cmd) {
    //Here we will validate Solve..
    return false;
}

static bool validateArguments(char** args, char* cmd) {
    if (strcmp(cmd, ACTION_SOLVE) != 0) {
        return validateSolve(args, cmd);
    } else {
        printf("Error: invalid command");
        return false;
    }
}

int* getValidModesForAction(const char *action, int *validModes){
    if (action == ACTION_SOLVE) {
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
