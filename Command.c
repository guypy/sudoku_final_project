#include <stdlib.h>
#include <assert.h>
#include "Command.h"

/**
 * Command Source File
 *
 * This file implements the 'create' and 'free' functions for a command struct.
 */

/**
 * This function creates a command structure.
 * @param args - array of Strings depicting arguments of command
 * @param action - String depicting what the command "does". E.g "set"
 * @param validModes - boolean array of valid modes for the command. Indeces depict a game mode according to constants defined in Game.h
 * @param numOfArgs - Int, number of arguments
 *
 * @return Returns a pointer to the created command.
 * */
Command* cmd_createCommand(char** args, char * action, int* validModes, int numOfArgs) {
    Command * cmd = (Command*) malloc(sizeof(Command));
    assert(cmd);
    cmd->action = action;
    cmd->args = args;
    cmd->validModes = validModes;
    cmd->numOfArgs = numOfArgs;
    cmd->prevValue = -1;
    return cmd;
}

/**
 * This function destroys a command struct, freeing all memory allocations
 * @param cmd - command structure to be destroyed
 */
void cmd_freeCommand(Command* cmd) {

    int i;
    free(cmd->action);
    for (i  = 0; i < cmd->numOfArgs; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd->validModes);
    free(cmd);
}
