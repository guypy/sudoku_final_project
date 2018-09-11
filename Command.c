#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Command.h"

Command* cmd_createCommand(char** args, char * action, int* validModes, int numOfArgs) {
    Command * cmd = (Command*) malloc(sizeof(Command));
    assert(cmd);
    printf("in create command args are %s %s %s\n", args[0],args[1],args[2]);
    cmd->action = action;
    cmd->args = args;
    cmd->validModes = validModes;
    cmd->numOfArgs = numOfArgs;
    cmd->prevValue = -1;
    printf("after create command cmd->args are %s %s %s\n", cmd->args[0],cmd->args[1],cmd->args[2]);
    printf("after create command args are %s %s %s\n", args[0],args[1],args[2]);
    return cmd;
}

void cmd_freeCommand(Command* cmd) {
    free(cmd->action);
    free(cmd->args);
    free(cmd->validModes);
    free(cmd);
}
