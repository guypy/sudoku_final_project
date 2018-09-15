#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "CommandValidations.h"

/**
 * Command Validations Source File
 *
 * This file contains the implementation of functions that validate the commands entered by the user before they
 * are executed.
 *
 */

/**
 * This functions checks whether its given input is a string that represents a number or not (is not a number)
 * @param arg - String
 * @return - boolean value: true if the String arg does not represent a number. false if arg represents a number.
 */
bool isNaN(char *arg){
    int i;
    char c;
    for (i = 0; i < (int) strlen(arg); ++i){
        c = arg[i];
        if (c < 48 || c > 57){
            return true;
        }
    }
    return false;
}

/**
 * This function validates the command "solve" - validation passes if a file name has been entered.
 *
 * @param cmd - pointer to "solve" command.
 * @return - boolean value: true if validation passes and false otherwise.
 */
bool validateSolve(Command* cmd) {
    if (cmd->numOfArgs < 1) {
        errPrinter_invalidCommand();
        return false;
    }
    return true;
}

/**
 * This function validates the command "edit"
 * @return - the "edit" command is always valid and therefore returned value is always true.
 */
bool validateEdit() {
    return true;
}

/**
 * This function validates the "mark_errors" command - validation passes only if the given arguments are 0 or 1
 *
 * @param cmd - pointer to "mark_errors" command.
 * @return - boolean value: true if validation passes and false otherwise.
 */
bool validateMarkErrors(Command* cmd) {
    int arg;
    if (cmd->numOfArgs < 1) {
        errPrinter_invalidCommand();
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

/**
 * This function validates the "print_board" command.
 * @return - this command is always valid and therefore returned value is always true.
 */
bool validatePrintBoard() {
    return true;
}

/**
 * This function validates the "set" command. Validation passes if the arguments are valid integers in correct range,
 * and if the cell being set is not fixed.
 *
 * @param cmd - pointer to "set" command.
 * @param game - pointer to current game struct.
 * @return - boolean value: true if validation passes and false otherwise.
 */
bool validateSet(Command *cmd, Game* game) {
    Cell* cell;
    int i, column, row, idx, blockColumns, blockRows, value;

    if(cmd->numOfArgs < 3) {
        errPrinter_invalidCommand();
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
    column = atoi(cmd->args[0]) - 1;
    row = atoi(cmd->args[1]) - 1;
    value = atoi(cmd->args[2]);

    /* validate column and row are in correct range*/
    if (column < 0 || column > (blockColumns * blockRows) - 1 ||
            row < 0 || row > (blockColumns * blockRows) - 1 ||
            value < 0 || value > (blockColumns * blockRows)){

        errPrinter_valueNotInRange(0, blockColumns*blockRows);
        return false;
    }

    /* validate cell is not fixed */
    idx = row * (blockColumns*blockRows) + column;
    cell = game->board->cells[idx];
    if (cell_isFixed(cell)){
        errPrinter_fixedCell();
        return false;
    }
    return true;
}

/**
 * This function validates the "validate" command.
 * @return - This command cannot contain invalid arguments, therefore returned value is always true.
 */
bool validateValidate() {
    return true;
}

/**
 * This function validates the "generate" command. Validation passes if entered arguments are numbers in correct range,
 * and the board is empty.
 * @param cmd - pointer to "generate" command.
 * @param game - pointer to current game struct.
 * @return - boolean value: true if validation passes and false otherwise.
 */
bool validateGenerate(Command* cmd, Game* game) {
    int i, x, y;
    int boardSize = BOARD_SIZE(game->board->blockColumns, game->board->blockRows);
    if (cmd->numOfArgs < 2) {
        errPrinter_invalidCommand();
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

/**
 * This function validates the "undo" command.
 * @return - this command cannot contain any arguments that can be invalid therefore the returned value is always true.
 */
bool validateUndo() {
    return true;
}

/**
 * This function validates the "redo" command.
 * @return - this command cannot contain any arguments that can be invalid therefore the returned value is always true.
 */
bool validateRedo() {
    return true;
}

/**
 * This function validates the "save" command. Validation passes if a given filepath was entered.
 * @param cmd - pointer to "save" command
 * @return - boolean value: true if validation passes and false otherwise.
 */
bool validateSave(Command* cmd) {
    if (cmd->numOfArgs < 1) {
        errPrinter_invalidCommand();
        return false;
    }
    return true;
}

/**
 * This function validates the "hint" command. Validation passes if given arguments are valid numbers in correct range.
 * @param cmd - pointer to "hint" command.
 * @param game - pointer to current game struct.
 * @return - boolean value: true if validation passes and false otherwise.
 */
bool validateHint(Command* cmd, Game* game) {
    int cellColumn, cellRow;
    int blockColumns = game->board->blockColumns;
    int blockRows = game->board->blockRows;

    if (cmd->numOfArgs < 2) {
        errPrinter_invalidCommand();
        return false;
    }

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

/**
 * This function validates the "num_solutions" command.
 * @return - This command cannot contain arguments that can be invalid, therefore the returned value is always true.
 */
bool validateNumSolutions() {
    return true;
}

/**
 * This function validates the "num_solutions" command.
 * @return - This command cannot contain arguments that can be invalid, therefore the returned value is always true.
 */
bool validateAutofill() {
    return true;
}

/**
 * This function validates the "num_solutions" command.
 * @return - This command cannot contain arguments that can be invalid, therefore the returned value is always true.
 */
bool validateReset() {
    return true;
}

/**
 * This function validates the "num_solutions" command.
 * @return - This command cannot contain arguments that can be invalid, therefore the returned value is always true.
 */
bool validateExit() {
    return true;
}
