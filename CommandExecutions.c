#include <stdlib.h>
#include <printf.h>
#include "CommandExecutions.h"
#include "Game.h"
#include "Command.h"
#include "FileHandler.h"

void executeSolve(Game* game, Command* cmd) {
    //Here we will execute Solve..
}

void executeEdit(Game* game, Command* cmd) {
    game->mode = EDIT;
    if (cmd->numOfArgs > 0) {
        game->board = fileHandler_readBoardFromFile(cmd->args[0]);
    } else {
        game->board = sb_create(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE);
    }

}

void executeMarkErrors(Game* game, Command* cmd) {
    game->markErrors = (bool) atoi(cmd->args[0]);
}

void executePrintBoard(Game* game, Command* cmd) {
    sb_print(game->board, (game->markErrors || game->mode == EDIT));
}

void executeSet(Game* game, Command* cmd) {
    //Here we will execute Set..
    append(game->undoRedoList, cmd);
}

void executeValidate(Game* game, Command* cmd) {
    //Here we will execute Validate..
}

void executeGenerate(Game* game, Command* cmd) {
    //Here we will execute Generate..
}

void executeUndo(Game* game, Command* cmd) {
    //Here we will execute Undo..
}

void executeRedo(Game* game, Command* cmd) {
    //Here we will execute Redo..
}
void executeSave(Game* game, Command* cmd) {
    //Here we will execute Save..
}
void executeHint(Game* game, Command* cmd) {
    //Here we will execute Hint..
}
void executeNumSolutions(Game* game, Command* cmd) {
    //Here we will execute NumSolutions..
}
void executeAutofill(Game* game, Command* cmd) {
    //Here we will execute Autofill..
    append(game->undoRedoList, cmd);
}
void executeReset(Game* game, Command* cmd) {
    //Here we will execute Reset..
}
