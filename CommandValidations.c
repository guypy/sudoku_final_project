#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "CommandValidations.h"
#include "ErrorPrinter.h"
#include "Command.h"
#include "SudokuBoard.h"
#include "Game.h"

bool validateSolve(Command* cmd) {
    if (cmd->numOfArgs < 1)
        return false;
    return true;
}

bool validateEdit(Command* cmd) {
    return true;
}

bool validateMarkErrors(Command* cmd) {
    //One argument - can only be 0 or 1.
    if (cmd->numOfArgs != 1)
        return false;
    if (isNaN(cmd->args[0]))
        return false;
    int arg = atoi(cmd->args[0]);
    if (arg != 0 && arg != 1)
        return false;
    return true;
}

bool validatePrintBoard(Command* cmd) {
    //No Validations For this command.
    return true;
}

bool validateSet(Command *cmd, Game* game) {
    Cell* cell;
    int i, arg, column, row, value_to_enter, idx, blockColumns, blockRows;
    blockColumns = game->board->blockColumns;
    blockRows = game->board->blockRows;
    /* validate arguments are valid integers */
    for (i = 0; i < 2; ++i){
        if (isNaN(cmd->args[i])){
            errPrinter_valueNotInRange(blockColumns*blockRows);
            return false;
        }
    }
    /* validate cell is not fixed */
    column = atoi(cmd->args[0]) - 1;
    row = atoi(cmd->args[1]) - 1;
    idx = row * (blockColumns*blockRows) + column;
    cell = game->board->cells[idx];
    if (isCellFixed(cell)){
        errPrinter_fixedCell();
        return false;
    }
    return true;
}

bool validateValidate(Command* cmd) {
    //Here we will validate Validate..
    return true;
}

bool validateGenerate(Command* cmd) {
    //Here we will validate Generate..
    return true;
}

bool validateUndo(Command* cmd) {
    return true;
}

bool validateRedo(Command* cmd) {
    //Here we will validate Redo..
    return true;
}
bool validateSave(Command* cmd) {
    if (cmd->numOfArgs < 1)
        return false;
    return true;
}
bool validateHint(Command* cmd) {
    //Here we will validate Hint..
    return true;
}
bool validateNumSolutions(Command* cmd) {
    return true;
}
bool validateAutofill(Command* cmd, Game* game) {
    return true;
}
bool validateReset(Command* cmd) {
    //Here we will validate Reset..
    return true;
}
bool validateExit(Command* cmd) {
    //No Validations For this command.
    return true;
}
bool isNaN(char *arg){
    int i;
    char c;
    for (i = 0; i < strlen(arg); ++i){
        c = arg[i];
        if (c < 48 || c > 57){
            return true;
        }
    }
    return false;
}
