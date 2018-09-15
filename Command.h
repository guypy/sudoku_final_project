#ifndef SUDOKU_CONSOLE_COMMAND_H
#define SUDOKU_CONSOLE_COMMAND_H

/**
 *
 * Command Header File
 *
 * This interface defines a command structure, which is created according to the parsed command entered by the user.
 * It is used to create and free commands.
 *
 * Each command contains the following:
 * action - string describing what the command "does". E.g. "set"
 * args - array of strings relevant to the action above.
 * validModes - boolean array of valid modes for the command. Indeces depict a game mode according to constants defined in Game.h
 * numOfArgs - number of arguments for this command.
 * prevValue - previous value of changed cell - relevant only for set and autofill commands
 *
 */

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
