#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CommandValidations.h"
#include "Command.h"
#include "Game.h"
#include "SudokuBoard.h"

bool validateSolve(Command* cmd) {
    if (cmd->numOfArgs < 1)
        return false;
    return true;
}

bool validateEdit(Command* cmd) {
    return true;
}

bool validateMarkErrors(Command* cmd) {
    int arg;
    if (cmd->numOfArgs < 1) {
        return false;
    }
    if (isNaN(cmd->args[0])) {
        errPrinter_valueIsNotZeroOrOne();
        return false;
    }
    arg = atoi(cmd->args[0]);
    if (arg != 0 && arg != 1){
        errPrinter_valueIsNotZeroOrOne();
        return false;
    }
    return true;
}

bool validatePrintBoard(Command* cmd) {
    return true;
}

bool validateSet(Command *cmd, Game* game) {
    Cell* cell;
    int i, column, row, idx, blockColumns, blockRows;

    if(cmd->numOfArgs < 3) {
        return false;
    }
    blockColumns = game->board->blockColumns;
    blockRows = game->board->blockRows;
    /* validate arguments are valid integers */
    for (i = 0; i < cmd->numOfArgs; ++i){
        if (isNaN(cmd->args[i])){
            errPrinter_valueNotInRange(0, blockColumns*blockRows);
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
    return true;
}

bool validateGenerate(Command* cmd, Game* game) {
    int i, x, y;
    int boardSize = BOARD_SIZE(game->board->blockColumns, game->board->blockRows);
    if (cmd->numOfArgs < 2) {
        return false;
    }
    for (i = 0; i < cmd->numOfArgs; ++i){
        if (isNaN(cmd->args[i])){
            errPrinter_valueNotInRange(0, boardSize);
            return false;
        }
    }
    x = atoi(cmd->args[0]);
    y = atoi(cmd->args[1]);

    if (x > boardSize || x < 0 || y > boardSize || y < 0) {
        errPrinter_valueNotInRange(0, boardSize);
        return false;
    }
    if(!sb_isEmpty(game->board)) {
        errPrinter_boardNotEmpty();
        return false;
    }

    return true;
}

bool validateUndo(Command* cmd) {
    return true;
}

bool validateRedo(Command* cmd) {
    return true;
}

bool validateSave(Command* cmd) {
    if (cmd->numOfArgs < 1)
        return false;
    return true;
}

bool validateHint(Command* cmd, Game* game) {
    int cellColumn, cellRow;
    int blockColumns = game->board->blockColumns;
    int blockRows = game->board->blockRows;

    if (cmd->numOfArgs < 2)
        return false;

    if (isNaN(cmd->args[0]) || isNaN(cmd->args[1])){
        errPrinter_valueNotInRange(1, blockColumns * blockRows);
        return false;
    }

    cellColumn = atoi(cmd->args[0]);
    cellRow = atoi(cmd->args[1]);

    if (cellColumn > blockColumns * blockRows || cellColumn < 1 || cellRow > blockColumns * blockRows || cellRow < 1) {
        errPrinter_valueNotInRange(1, blockColumns * blockRows);
        return false;
    }
    return true;
}

bool validateNumSolutions(Command* cmd) {
    return true;
}

bool validateAutofill(Command* cmd, Game* game) {
    return true;
}

bool validateReset(Command* cmd) {
    return true;
}

bool validateExit(Command* cmd) {
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
