//
// Created by Guy Pinchuk on 04/08/2018.
//

#include "CommandExecutions.h"
#include "Game.h"

void executeSolve(Game* game, Command* cmd) {
    //Here we will execute Solve..
}

void executeEdit(Game* game, Command* cmd) {
    //Here we will execute Edit..
}

void executeMarkErrors(Game* game, Command* cmd) {
    //Here we will execute MarkErrors..
}

void executePrintBoard(Game* game, Command* cmd) {
    sb_print(game->board, (game->markErrors || game->mode == EDIT));
}

void executeSet(Game* game, Command* cmd) {
    //Here we will execute Set..
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
}
void executeReset(Game* game, Command* cmd) {
    //Here we will execute Reset..
}
