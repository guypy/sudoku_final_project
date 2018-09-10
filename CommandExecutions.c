#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include "CommandExecutions.h"
#include "Game.h"
#include "Command.h"
#include "FileHandler.h"
#include "ErrorPrinter.h"
#include "SudokuBoard.h"
#include "BTSolver.h"

void updateImpValuesInRow(int *impossibleValues);

void executeSolve(Game* game, Command* cmd) {
    game->board = fileHandler_readBoardFromFile(cmd->args[0]);
    if (game->board == NULL) {
        errPrinter_fileDoesNotExist();
        return;
    }
    game->mode = SOLVE;
}

void executeEdit(Game* game, Command* cmd) {
    if (cmd->numOfArgs > 0) {
        game->board = fileHandler_readBoardFromFile(cmd->args[0]);
        if (game->board == NULL)
            errPrinter_cannotOpenFile();
    } else {
        game->board = sb_create(DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
    }
    game->mode = EDIT;
}

void executeMarkErrors(Game* game, Command* cmd) {
    game->markErrors = (bool) atoi(cmd->args[0]);
}

void executePrintBoard(Game* game, Command* cmd) {
    sb_print(game->board, (game->markErrors || game->mode == EDIT));
}

void executeSet(Game* game, Command* cmd) {
    Cell* cell;
    int valueToSet, blockColumns, blockRows, idx, column, row;
    blockColumns = game->board->blockColumns;
    blockRows = game->board->blockRows;
    column = atoi(cmd->args[0]) - 1;
    row = atoi(cmd->args[1]) - 1;
    idx = row * (blockColumns*blockRows) + column;
    cell = game->board->cells[idx];
    valueToSet = atoi(cmd->args[2]);
    if (!cell_isValid(game->board, valueToSet, idx)){
        cell->valid = false;
    }
    game->board->cells[idx]->value = valueToSet;
    deleteFromNode(game->undoRedoList, game->currentCommandInList);
    append(game->undoRedoList, cmd);
    game->currentCommandInList = game->undoRedoList->tail;
    sb_print(game->board, game->markErrors);
    if (sb_isFull(game->board)){
        if (sb_isErroneous(game->board)){
            errPrinter_puzzleSolutioinErroneous();
        }
        else{
            printf("Puzzle solved successfully\n");
            game->mode = INIT;
        }
    }
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
    if (game->mode == EDIT) {
        if (sb_isErroneous(game->board)) {
            errPrinter_erroneousValues();
            return;
        }
        if (!sb_isSolvable(game->board)) {
            errPrinter_boardValidationFailed();
        }
    }
    char* path = cmd->args[0];
    if (!fileHandler_saveBoardToFile(game->board, path, game->mode == EDIT)){
        errPrinter_cannotCreateOrModifyFile();
    }
}

void executeHint(Game* game, Command* cmd) {
    //Here we will execute Hint..
}

void executeNumSolutions(Game* game, Command* cmd) {
    int numOfSolutions;
    if (sb_isErroneous(game->board)) {
        errPrinter_erroneousValues();
        return;
    }
    numOfSolutions = BT_numberOfSolutions(game->board);
    printf("Number of solutions: %d\n", numOfSolutions);
    if (numOfSolutions == 1) {
        printf("This is a good board!\n");
    } else {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
}

void executeAutofill(Game* game, Command* cmd) {
    if (sb_isErroneous(game->board)){
        errPrinter_erroneousValues();
    }
    else{

        append(game->undoRedoList, cmd);
    }
}

void executeReset(Game* game, Command* cmd) {
    //Here we will execute Reset..
}

int* getImpossibleValuesForCell(Game* game, Cell* cell, int index){
    int blockRows, blockColumns, cellRow;
    int* impossibleValues;
    blockRows = game->board->blockRows;
    blockColumns = game ->board->blockColumns;
    impossibleValues = (int*)calloc((size_t) (blockColumns * blockRows), sizeof(int));
    cellRow = index / (blockRows*blockColumns);
//    updateImpValuesInRow(impossibleValues, cellRow);
}

void updateImpValuesInRow(int *impossibleValues) {

}
