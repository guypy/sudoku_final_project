#include <stdlib.h>
#include <assert.h>
#include "Command.h"

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

void cmd_freeCommand(Command* cmd) {
    free(cmd->action);
    free(cmd->args);
    free(cmd->validModes);
    free(cmd);
}
