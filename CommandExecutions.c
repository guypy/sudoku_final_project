#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <assert.h>
#include "CommandExecutions.h"
#include "Game.h"
#include "Command.h"
#include "FileHandler.h"
#include "ErrorPrinter.h"
#include "SudokuBoard.h"
#include "BTSolver.h"

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
    Node* undoRedoListPointer;
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
    undoRedoListPointer = game->undoRedoListPointer;
    if (undoRedoListPointer != NULL){
        destroyFromNode(game->undoRedoList, game->undoRedoListPointer->next);
    }
    append(game->undoRedoList, cmd);
    game->undoRedoListPointer = game->undoRedoList->tail;
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
    int i, numOfCells, blockRows, blockColumns,value;
    int* impossibleValues;  // list of bool values, 1 in index x means x is an impssible value for a cell
    LinkedList* valuesToFill = createList();
    Command* valueToSetCmd;
    char** args;
    char row, column, valueToSet;

    if (sb_isErroneous(game->board)){
        errPrinter_erroneousValues();
    }
    else{
        blockRows = game->board->blockRows;
        blockColumns = game->board->blockColumns;
        numOfCells = (blockRows ^ 2) * (blockColumns ^ 2);
        for (i = 0; i < numOfCells; ++i){
            impossibleValues = (int*)calloc((size_t) (blockColumns * blockRows), sizeof(int));
            assert(impossibleValues);
            updateImpossibleValuesForCell(game, game->board->cells[i], i, impossibleValues);
            value = getPossValueForCell(impossibleValues, (blockRows * blockColumns));
            if (value != -1) { // there is only one possible value
                args = malloc(1024 * sizeof(char));
                assert(args);
                column = (char) ('0' + (i % (blockRows * blockColumns)));
                row = (char) ('0' + i / (blockRows * blockColumns));
                valueToSet = (char) ('0' + value);
                args[0] = &column; // column to set
                args[1] = &row; // row to set
                args[2] = &valueToSet; // value to set
                valueToSetCmd = cmd_createCommand(args, "SET", NULL, 3);
                append(valuesToFill, valueToSetCmd);
            }
            free(impossibleValues);
        }
        autoFillValues(valuesToFill, game);
        append(game->undoRedoList, cmd);
    }
}

void autoFillValues(LinkedList* valuesToFill, Game* game){
    int idxToSet, row, column, blockRows, blockColumns, valueToSet;
    Node* currentNode = valuesToFill->head;
    Command* currentCmd;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    while (currentNode != NULL){
        currentCmd = currentNode->data;
        row = atoi(currentCmd->args[1]);
        column = atoi(currentCmd->args[0]);
        idxToSet = row * (blockColumns*blockRows) + column;
        valueToSet = atoi(currentCmd->args[2]);
        game->board->cells[idxToSet]->value = valueToSet;
        currentNode = currentNode->next;
    }
}

int getPossValueForCell(int *impossibleValues, int size){
    int num, i, possValue;
    num = 0;
    possValue = -1;
    for (i = 0; i < size; ++i){
        if (impossibleValues[i] == 0){ // value is possible
            num += 1;
            possValue = i + 1;
            if (num > 1){
                possValue = -1;
                break;
            }
        }
    }
    return possValue;
}


void updateImpossibleValuesForCell(Game *game, Cell *cell, int index, int *impossibleValues){
    int blockRows, blockColumns, cellRow, cellCol;
    blockRows = game->board->blockRows;
    blockColumns = game ->board->blockColumns;
    cellRow = index / (blockRows*blockColumns);
    updateImpValuesInRow(impossibleValues, cellRow, blockRows, blockColumns, game->board->cells);
    cellCol = index % (blockRows * blockColumns);
    updateImpValuesInCol(impossibleValues, cellCol, blockRows, blockColumns, game->board->cells);
    updateImpValuesInBlock(impossibleValues, cellRow, cellCol, blockRows, blockColumns, game->board->cells);
}

void updateImpValuesInCol(int *impossibleValues, int cellCol, int blockRows, int blockColumns, Cell** cells) {
    int i, checkedCellIdx, checkedCellValue;
    for (i = 0; i < (blockRows * blockColumns); ++i){
        checkedCellIdx = i * (blockRows*blockColumns) + cellCol;
        checkedCellValue = (cells[checkedCellIdx])->value;
        impossibleValues[checkedCellValue - 1] = 1;
    }
}

void updateImpValuesInRow(int *impossibleValues, int cellRow, int blockRows, int blockColumns, Cell** cells) {
    int i, checkedCellIdx, checkedCellValue;
    for (i = 0; i< (blockRows * blockColumns); ++i){
        checkedCellIdx = cellRow * (blockColumns * blockRows) + i;
        checkedCellValue = (cells[checkedCellIdx])->value;
        impossibleValues[checkedCellValue - 1] = 1;
    }
}

void updateImpValuesInBlock(int *impossibleValues, int cellRow, int cellcCol, int blockRows, int blockColumns, Cell** cells){
    int i, j, checkedCellIdx, checkedCellValue, blockRowIdx, blockColIdx;
    blockRowIdx = cellRow / blockRows;
    blockColIdx = cellcCol / blockColumns;
    for (i = 0; i < blockRows; ++i){
        for (j = 0; j < blockColumns; ++j){
            checkedCellIdx = (blockRowIdx * blockRows + i) * blockRows*blockColumns + blockColIdx * blockColumns + j;
            checkedCellValue = (cells[checkedCellIdx])->value;
            impossibleValues[checkedCellValue - 1] = 1;
        }
    }
}

void executeReset(Game* game, Command* cmd) {
    //Here we will execute Reset..
}
