#ifndef SUDOKU_CONSOLE_COMMAND_H
#define SUDOKU_CONSOLE_COMMAND_H

typedef struct {
    char* action;
    char** args;
    int * validModes;
    int numOfArgs;
    int prevValue; /* Relevant only for commands that change the value of a cell - set, autofill */
} Command;

Command* cmd_createCommand(char** args, char * action, int* validModes, int numOfArgs);
void cmd_freeCommand(Command* cmd);

#endif
