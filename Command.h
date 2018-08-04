//
// Created by Guy Pinchuk on 04/08/2018.
//

#ifndef SUDOKU_CONSOLE_COMMAND_H
#define SUDOKU_CONSOLE_COMMAND_H

typedef struct {
    char* action;
    char** args; //Array of arguments
    int * validModes;
} Command;

Command* cmd_createCommand(char** args, char * action, int* validModes);
void cmd_freeCommand(Command* cmd);

#endif //SUDOKU_CONSOLE_COMMAND_H
