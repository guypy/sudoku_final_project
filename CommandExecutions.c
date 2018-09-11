#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <assert.h>
#include <string.h>
#include "CommandExecutions.h"

void destroyNextNodesBeforeAppend(const Game *game);

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
    int valueToSet, blockColumns, blockRows, idx, column, row, oldValue;
    blockColumns = game->board->blockColumns;
    blockRows = game->board->blockRows;
    column = atoi(cmd->args[0]) - 1;
    row = atoi(cmd->args[1]) - 1;
    idx = row * (blockColumns*blockRows) + column;
    cell = game->board->cells[idx];
    valueToSet = atoi(cmd->args[2]);
    cell->valid = cell_isValid(game->board, valueToSet, idx);
    oldValue = game->board->cells[idx]->value;
    cmd->prevValue = oldValue;
    game->board->cells[idx]->value = valueToSet;
    destroyNextNodesBeforeAppend(game);
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
    int resultCode = 0;
    if (sb_isErroneous(game->board)) {
        errPrinter_erroneousValues();
        return;
    }
    SudokuBoard* solved = ILP_solve(game->board, &resultCode);
    switch (resultCode) {
        case SOLVED:
            game->solvedBoard = solved;
            printf("Validation passed: board is solvable\n");
            break;
        case NO_SOLUTION:
            printf("Validation failed: board is unsolvable\n");
        case ERROR:
        default:
            return;
    }
}

void executeGenerate(Game* game, Command* cmd) {
    //Here we will execute Generate..
}

void executeUndo(Game* game, Command* cmd) {
    Node* currentNode;
    char* action;
    currentNode = game->undoRedoListPointer;
    if (currentNode == NULL){
        errPrinter_noMovesToUndo();
        return;
    }
    action = currentNode->data->action;
    if (strcmp(action, "set") == 0){
        undoSetCmd(game);
    }
    if (strcmp(action, "autofill") == 0){
        undoAutofillCmd(game);
    }
}

void undoAutofillCmd(Game* game){
    int currentValue, prevValue, column, row, blockRows, blockColumns, idxToUndo;
    Node* currentNode, *currentAutoFillNode;
    LinkedList* autoFillList;
    currentNode = game->undoRedoListPointer;
    autoFillList = currentNode->autoFillList;
    currentAutoFillNode = autoFillList->head;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    while (currentAutoFillNode != NULL){
        prevValue = currentAutoFillNode->data->prevValue;
        column = atoi(currentAutoFillNode->data->args[0]);
        row = atoi(currentAutoFillNode->data->args[1]);
        idxToUndo = row * (blockColumns*blockRows) + column;
        game->board->cells[idxToUndo]->value = prevValue;
        game->board->cells[idxToUndo]->valid = cell_isValid(game->board, prevValue, idxToUndo);
        currentAutoFillNode = currentAutoFillNode->next;
    }
    sb_print(game->board, game->markErrors);
    currentAutoFillNode = autoFillList->head;
    while (currentAutoFillNode != NULL){
        currentValue = atoi(currentAutoFillNode->data->args[2]);
        prevValue = currentAutoFillNode->data->prevValue;
        column = atoi(currentAutoFillNode->data->args[0]);
        row = atoi(currentAutoFillNode->data->args[1]);

        printUndoStep(currentValue, prevValue, column, row);

        currentAutoFillNode = currentAutoFillNode->next;
    }
    game->undoRedoListPointer = game->undoRedoListPointer->prev;
}

void undoSetCmd(Game* game){
    int currentValue, prevValue, column, row, idxToUndo, blockColumns, blockRows;
    Node* currentNode;
    currentNode = game->undoRedoListPointer;
    currentValue = atoi(currentNode->data->args[2]);
    prevValue = currentNode->data->prevValue;
    column = atoi(currentNode->data->args[0]) - 1;
    row = atoi(currentNode->data->args[1]) - 1;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    idxToUndo = row * (blockColumns*blockRows) + column;
    game->board->cells[idxToUndo]->value = prevValue;
    game->board->cells[idxToUndo]->valid = cell_isValid(game->board, prevValue, idxToUndo);

    sb_print(game->board, game->markErrors);

    printUndoStep(currentValue, prevValue, column, row);

    game->undoRedoListPointer = game->undoRedoListPointer->prev;
}

void printUndoStep(int currentValue, int prevValue, int column, int row){
    if (currentValue == 0 && prevValue == 0){
        printf("Undo %d,%d: from _ to _\n", column + 1, row + 1);
    }
    if (currentValue == 0 && prevValue != 0){
        printf("Undo %d,%d: from _ to %d\n", column + 1, row + 1, prevValue);
    }
    if (currentValue != 0 && prevValue == 0){
        printf("Undo %d,%d: from %d to _\n", column + 1, row + 1, currentValue);
    }
    if (currentValue != 0 && prevValue != 0){
        printf("Undo %d,%d: from %d to %d\n", column + 1, row + 1, currentValue, prevValue);
    }
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

    if (sb_isErroneous(game->board)){
        errPrinter_erroneousValues();
        return;
    }
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    numOfCells = (blockRows * blockRows) * (blockColumns * blockColumns);
    for (i = 0; i < numOfCells; ++i){
        if (game->board->cells[i]->value != 0){
            continue;
        }
        impossibleValues = (int*)calloc((size_t) (blockColumns * blockRows), sizeof(int));
        assert(impossibleValues);
        updateImpossibleValuesForCell(game, game->board->cells[i], i, impossibleValues);
        value = getPossValueForCell(impossibleValues, (blockRows * blockColumns));
        if (value != -1) { // there is only one possible value
            args = malloc(1024 * sizeof(char));
            assert(args);
            args[0] =(char*)malloc(snprintf(NULL, 0, "%d", (i % (blockRows * blockColumns))));
            sprintf(args[0], "%d", (i % (blockRows * blockColumns)));
            args[1] = (char*)malloc(snprintf(NULL, 0, "%d", i / (blockRows * blockColumns)));
            sprintf(args[1], "%d", i / (blockRows * blockColumns));
            args[2] = (char*)malloc(snprintf(NULL, 0, "%d", value));
            sprintf(args[2], "%d", value);
            valueToSetCmd = cmd_createCommand(args, "SET", NULL, 3);
            append(valuesToFill, valueToSetCmd);
        }
        free(impossibleValues);
    }
    if (valuesToFill->size > 0){
        autoFillValues(valuesToFill, game);
        destroyNextNodesBeforeAppend(game);
        append(game->undoRedoList, cmd);
        game->undoRedoList->tail->autoFillList = valuesToFill; /* make the 'autofill node' in the undoRedoList have a valuesToFill list */
        game->undoRedoListPointer = game->undoRedoList->tail;
    }
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

void destroyNextNodesBeforeAppend(const Game *game) {
    if (game->undoRedoListPointer != NULL){
            destroyFromNode(game->undoRedoList, game->undoRedoListPointer->next);
        } else{
        /* if we undid all moves in the list, the undoRedoListPointer == NULL but there are still nodes in the list */
            if (game->undoRedoList->head != NULL){
                destroyFromNode(game->undoRedoList, game->undoRedoList->head);
            }
        }
}

void autoFillValues(LinkedList* valuesToFill, Game* game){
    int idxToSet, row, column, blockRows, blockColumns, valueToSet, oldValue;
    Node* currentNode;
    Command* currentCmd;
    if (valuesToFill->size == 0){
        return;
    }
    currentNode = valuesToFill->head;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    while (currentNode != NULL){
        currentCmd = currentNode->data;
        row = atoi(currentCmd->args[1]);
        column = atoi(currentCmd->args[0]);
        idxToSet = row * (blockColumns*blockRows) + column;
        valueToSet = atoi(currentCmd->args[2]);
        oldValue = game->board->cells[idxToSet]->value;
        currentCmd->prevValue = oldValue;
        game->board->cells[idxToSet]->value = valueToSet;
        game->board->cells[idxToSet]->valid = cell_isValid(game->board, valueToSet, idxToSet);
        printf("Cell <%d,%d> set to %d\n", column + 1, row + 1, valueToSet);
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
        if (checkedCellValue > 0){
            impossibleValues[checkedCellValue - 1] = 1;
        }
    }
}

void updateImpValuesInRow(int *impossibleValues, int cellRow, int blockRows, int blockColumns, Cell** cells) {
    int i, checkedCellIdx, checkedCellValue;
    for (i = 0; i< (blockRows * blockColumns); ++i){
        checkedCellIdx = cellRow * (blockColumns * blockRows) + i;
        checkedCellValue = (cells[checkedCellIdx])->value;
        if (checkedCellValue > 0){
            impossibleValues[checkedCellValue - 1] = 1;
        }
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
            if (checkedCellValue > 0){
                impossibleValues[checkedCellValue - 1] = 1;
            }
        }
    }
}

void executeReset(Game* game, Command* cmd) {
    //Here we will execute Reset..
}
