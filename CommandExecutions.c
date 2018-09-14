#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "CommandExecutions.h"

void executeSolve(Game* game, Command* cmd) {
    restartGame(game);
    game->board = fileHandler_readBoardFromFile(cmd->args[0]);
    if (game->board == NULL) {
        errPrinter_fileDoesNotExist();
        return;
    }
    game->mode = SOLVE;
}

void executeEdit(Game* game, Command* cmd) {
    restartGame(game);
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

void executePrintBoard(Game* game) {
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
    sb_cellValidations(game->board);
    destroyNextNodesBeforeAppend(game);
    append(game->undoRedoList, cmd);
    game->undoRedoListPointer = game->undoRedoList->tail;
    sb_print(game->board, game->markErrors);
    if (sb_isFull(game->board)){
        if (sb_isErroneous(game->board)){
            errPrinter_puzzleSolutionErroneous();
        }
        else{
            printf("Puzzle solved successfully\n");
            game->mode = INIT;
        }
    }
}

void executeValidate(Game* game) {
    int resultCode = 0;
    SudokuBoard* solved;
    if (sb_isErroneous(game->board)) {
        errPrinter_erroneousValues();
        return;
    }
    solved = ILP_solve(game->board, &resultCode);
    switch (resultCode) {
        case SOLVED:
            sb_destroyBoard(solved);
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
    int valuesToFillCount = atoi(cmd->args[0]);
    int valueToRemoveCount = atoi(cmd->args[1]);
    int tryCount = 0, resultCode = 0;
    SudokuBoard* solved = NULL;
    do {
        tryCount++;

        if (!fillBoardWithRandValues(game->board, valuesToFillCount))
            continue;
        solved = ILP_solve(game->board, &resultCode);
    } while (resultCode != SOLVED && tryCount < 1000);
    
    if (tryCount == 1000) {
        errPrinter_puzzleGeneratorFailed();
        return;
    }

    sb_destroyBoard(game->board);
    game->board = solved;

    removeValuesFromBoard(game->board, valueToRemoveCount);
}

bool fillBoardWithRandValues(SudokuBoard* board, int valueToFillCount){
    int idx, val;
    while (valueToFillCount > 0) {
        do { /* Find an empty cell */
            idx = rand() % BOARD_SIZE(board->blockColumns, board->blockRows);
        } while (board->cells[idx]->value != 0);

        if (!isCellSolvable(board, idx))
            return false;

        do { /* Find a legal value for the cell */
            val = rand() % (board->blockRows * board->blockColumns) + 1;
        } while (!cell_isValid(board, val, idx));

        board->cells[idx]->value = val;
        valueToFillCount--;
    }
    return true;
}

void removeValuesFromBoard(SudokuBoard* board, int valueToRemoveCount){
    int idx;
    while (valueToRemoveCount > 0) {
        do { /* Find a filled cell */
            idx = rand() % BOARD_SIZE(board->blockColumns, board->blockRows);
        } while (board->cells[idx]->value == 0);

        board->cells[idx]->value = 0;
        valueToRemoveCount--;
    }
}

bool isCellSolvable(SudokuBoard* board, int idx) {
    int i;
    int* impossibleValues = (int*) calloc(board->blockColumns * board->blockRows, sizeof(int));
    updateImpossibleValuesForCell(board, idx, impossibleValues);
    for (i = 0; i < board->blockRows * board->blockColumns; i++){
        if (impossibleValues[i] == 0) {
            free(impossibleValues);
            return true;
        }
    }
    free(impossibleValues);
    return false;
}

void executeUndo(Game *game, bool shouldPrint) {
    Node* currentNode;
    char* action;
    currentNode = game->undoRedoListPointer;
    if (currentNode == NULL){
        errPrinter_noMovesToUndo();
        return;
    }
    action = currentNode->data->action;
    if (strcmp(action, ACTION_SET) == 0){
        undoSetCmd(game, shouldPrint);
    }
    if (strcmp(action, ACTION_AUTOFILL) == 0){
        undoAutofillCmd(game, shouldPrint);
    }
}

void undoAutofillCmd(Game* game, bool shouldPrint){
    int currentValue, prevValue, column, row, blockRows, blockColumns, idxToUndo;
    Node* nodeToUndo, *currentAutoFillNode;
    LinkedList* autoFillList;
    nodeToUndo = game->undoRedoListPointer;
    autoFillList = nodeToUndo->autoFillList;
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
    sb_cellValidations(game->board);

    if (shouldPrint == true){
        sb_print(game->board, game->markErrors);
    }
    currentAutoFillNode = autoFillList->head;
    while (currentAutoFillNode != NULL){
        currentValue = atoi(currentAutoFillNode->data->args[2]);
        prevValue = currentAutoFillNode->data->prevValue;
        column = atoi(currentAutoFillNode->data->args[0]);
        row = atoi(currentAutoFillNode->data->args[1]);

        if (shouldPrint == true){
            printUndoStep(currentValue, prevValue, column, row);
        }

        currentAutoFillNode = currentAutoFillNode->next;
    }
    game->undoRedoListPointer = game->undoRedoListPointer->prev;
}

void undoSetCmd(Game* game, bool shouldPrint){
    int currentValue, prevValue, column, row, idxToUndo, blockColumns, blockRows;
    Node* nodeToUndo;
    nodeToUndo = game->undoRedoListPointer;
    currentValue = atoi(nodeToUndo->data->args[2]);
    prevValue = nodeToUndo->data->prevValue;
    column = atoi(nodeToUndo->data->args[0]) - 1;
    row = atoi(nodeToUndo->data->args[1]) - 1;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    idxToUndo = row * (blockColumns*blockRows) + column;
    game->board->cells[idxToUndo]->value = prevValue;
    game->board->cells[idxToUndo]->valid = cell_isValid(game->board, prevValue, idxToUndo);
    sb_cellValidations(game->board);

    if (shouldPrint == true){
        sb_print(game->board, game->markErrors);

        printUndoStep(currentValue, prevValue, column, row);
    }

    game->undoRedoListPointer = game->undoRedoListPointer->prev;
}

void printUndoStep(int oldValue, int newValue, int column, int row){
    if (oldValue == 0 && newValue == 0){
        printf("Undo %d,%d: from _ to _\n", column + 1, row + 1);
    }
    if (oldValue == 0 && newValue != 0){
        printf("Undo %d,%d: from _ to %d\n", column + 1, row + 1, newValue);
    }
    if (oldValue != 0 && newValue == 0){
        printf("Undo %d,%d: from %d to _\n", column + 1, row + 1, oldValue);
    }
    if (oldValue != 0 && newValue != 0){
        printf("Undo %d,%d: from %d to %d\n", column + 1, row + 1, oldValue, newValue);
    }
}

/*
 * When we perform a set/autofill command we add the command to the undoRedoList, and the undoRedoList pointer
 * points to the move we had just done.
 *
 * When we undo a command, we undo the command the undoRedoList pointer is pointing to, and set the undoRedoListPointer
 * one command back.
 *
 * Therefore, when we redo a command that has been undone, we redo the command NEXT to the one pointed to by the
 * undoRedoListPointer.
 *
 * */
void executeRedo(Game* game) {
    Node* currentNode, *nodeToRedo;
    char* action;
    currentNode = game->undoRedoListPointer;
    if (currentNode == game->undoRedoList->tail){
        errPrinter_noMovesToRedo();
        return;
    }
    nodeToRedo = getNodeToRedo(game, currentNode);
    if (nodeToRedo == NULL){
        errPrinter_noMovesToRedo();
        return;
    }

    action = nodeToRedo->data->action;
    if (strcmp(action, ACTION_SET) == 0){
        redoSetCmd(game, nodeToRedo);
    }
    if (strcmp(action, ACTION_AUTOFILL) == 0){
        redoAutofillCmd(game, nodeToRedo);
    }
}

Node* getNodeToRedo(Game* game, Node* currentNode){
    Node* nodeToRedo;
    if (currentNode == NULL){
        if (game->undoRedoList->head == NULL){
            return NULL;
        }
        nodeToRedo = game->undoRedoList->head; /* head is different than NULL*/
    }else{
        nodeToRedo = currentNode->next;
    }
    return nodeToRedo;
}

void redoAutofillCmd(Game* game, Node* nodeToRedo){
    int blockRows, blockColumns, oldValue, newValue, row, column, idxToRedo;
    LinkedList* autoFillList;
    Node* currentAutoFillNode;
    autoFillList = nodeToRedo->autoFillList;
    currentAutoFillNode = autoFillList->head;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    while (currentAutoFillNode != NULL){
        newValue = atoi(currentAutoFillNode->data->args[2]);
        column = atoi(currentAutoFillNode->data->args[0]);
        row = atoi(currentAutoFillNode->data->args[1]);
        idxToRedo = row * (blockColumns*blockRows) + column;
        game->board->cells[idxToRedo]->value = newValue;
        game->board->cells[idxToRedo]->valid = cell_isValid(game->board, newValue, idxToRedo);
        currentAutoFillNode = currentAutoFillNode->next;
    }
    sb_cellValidations(game->board);

    sb_print(game->board, game->markErrors);

    currentAutoFillNode = autoFillList->head;
    while (currentAutoFillNode != NULL){
        newValue = atoi(currentAutoFillNode->data->args[2]);;
        oldValue = currentAutoFillNode->data->prevValue;
        column = atoi(currentAutoFillNode->data->args[0]);
        row = atoi(currentAutoFillNode->data->args[1]);

        printRedoStep(newValue, oldValue, column, row);

        currentAutoFillNode = currentAutoFillNode->next;
    }
    if (game->undoRedoListPointer != NULL){
        game->undoRedoListPointer = game->undoRedoListPointer->next;
    }else{
        game->undoRedoListPointer = game->undoRedoList->head;
    }
}

void redoSetCmd(Game* game, Node* nodeToRedo){
    int currentValue, newValue, row, column, idxToSet, blockRows, blockColumns;
    newValue = atoi(nodeToRedo->data->args[2]);
    currentValue = nodeToRedo->data->prevValue;
    column = atoi(nodeToRedo->data->args[0]) - 1;
    row = atoi(nodeToRedo->data->args[1]) - 1;
    blockRows = game->board->blockRows;
    blockColumns = game->board->blockColumns;
    idxToSet = row * (blockColumns*blockRows) + column;

    game->board->cells[idxToSet]->value = newValue;
    game->board->cells[idxToSet]->valid = cell_isValid(game->board, newValue, idxToSet);
    sb_cellValidations(game->board);

    sb_print(game->board, game->markErrors);

    printRedoStep(newValue, currentValue,column, row);

    if (game->undoRedoListPointer != NULL){
        game->undoRedoListPointer = game->undoRedoListPointer->next;
    }else{
        game->undoRedoListPointer = game->undoRedoList->head;
    }
}

void printRedoStep(int newValue, int oldValue, int column, int row){
    if (newValue == 0 && oldValue == 0){
        printf("Redo %d,%d: from _ to _\n", column + 1, row + 1);
    }
    if (newValue == 0 && oldValue != 0){
        printf("Redo %d,%d: from %d to _\n", column + 1, row + 1, oldValue);
    }
    if (newValue != 0 && oldValue == 0){
        printf("Redo %d,%d: from _ to %d\n", column + 1, row + 1, newValue);
    }
    if (newValue != 0 && oldValue != 0){
        printf("Redo %d,%d: from %d to %d\n", column + 1, row + 1, newValue, oldValue);
    }
}

void executeSave(Game* game, Command* cmd) {
    char* path = cmd->args[0];
    if (game->mode == EDIT) {
        if (sb_isErroneous(game->board)) {
            errPrinter_erroneousValues();
            return;
        }
        if (!sb_isSolvable(game->board)) {
            errPrinter_boardValidationFailed();
        }
    }
    if (!fileHandler_saveBoardToFile(game->board, path, game->mode == EDIT)){
        errPrinter_cannotCreateOrModifyFile();
    }
}

void executeHint(Game* game, Command* cmd) {
    int blockColumns, blockRows, cellIdx, cellColumn, cellRow, resultCode;
    SudokuBoard* solved;
    blockColumns = game->board->blockColumns;
    blockRows = game->board->blockRows;

    cellColumn = atoi(cmd->args[0]) - 1;
    cellRow = atoi(cmd->args[1]) - 1;

    if (sb_isErroneous(game->board)) {
        errPrinter_erroneousValues();
        return;
    }

    cellIdx = cellRow * (blockColumns*blockRows) + cellColumn;
    if(game->board->cells[cellIdx]->fixed) {
        errPrinter_fixedCell();
        return;
    }
    if(game->board->cells[cellIdx]->value) {
        errPrinter_cellContainsValue();
        return;
    }

    solved = ILP_solve(game->board, &resultCode);
    switch (resultCode) {
        case SOLVED:
            printf("Hint: set cell to %d\n", solved->cells[cellIdx]->value);
            break;
        case NO_SOLUTION:
            errPrinter_unsolvableBoard();
        case ERROR:
        default:
            return;
    }
}

void executeNumSolutions(Game* game) {
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
    int i, numOfCells, blockRows, blockColumns,value, numOfChars;
    int* impossibleValues;  /*list of bool values, 1 in index x means x is an impssible value for a cell*/
    LinkedList* valuesToFill = createList();
    Command* valueToSetCmd;
    char* action;
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
        updateImpossibleValuesForCell(game->board, i, impossibleValues);
        value = getPossValueForCell(impossibleValues, (blockRows * blockColumns));
        if (value != -1) { /*there is only one possible value*/
            args = malloc(1024 * sizeof(char));
            assert(args);
            numOfChars = getNumOfChars(i % (blockRows * blockColumns));
            args[0] = (char*)calloc((size_t) (numOfChars + 1), sizeof(char));
            sprintf(args[0], "%d", (i % (blockRows * blockColumns)));
            numOfChars = getNumOfChars(i / (blockRows * blockColumns));
            args[1] = (char*)calloc((size_t) (numOfChars + 1), sizeof(char));
            sprintf(args[1], "%d", i / (blockRows * blockColumns));
            numOfChars = getNumOfChars(value);
            args[2] = (char*)calloc((size_t) (numOfChars + 1), sizeof(char));
            sprintf(args[2], "%d", value);

            action = malloc(sizeof(char) * 4);
            sprintf(action, "%s", ACTION_SET);
            valueToSetCmd = cmd_createCommand(args, action, NULL, 3);
            append(valuesToFill, valueToSetCmd);
        }
        free(impossibleValues);
    }
    if (valuesToFill->size > 0){
        autoFillValues(valuesToFill, game);
        sb_cellValidations(game->board);
        destroyNextNodesBeforeAppend(game);
        append(game->undoRedoList, cmd);
        game->undoRedoList->tail->autoFillList = valuesToFill; /* make the 'autofill node' in the undoRedoList have a valuesToFill list */
        game->undoRedoListPointer = game->undoRedoList->tail;
    }
    sb_print(game->board, game->markErrors);
    if (sb_isFull(game->board)){
        if (sb_isErroneous(game->board)){
            errPrinter_puzzleSolutionErroneous();
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
        if (impossibleValues[i] == 0){ /*value is possible*/
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

void updateImpossibleValuesForCell(SudokuBoard *board, int index, int *impossibleValues){
    int blockRows, blockColumns, cellRow, cellCol;
    blockRows = board->blockRows;
    blockColumns = board->blockColumns;

    cellRow = index / (blockRows*blockColumns);
    cellCol = index % (blockRows * blockColumns);

    updateImpValuesInRow(impossibleValues, cellRow, blockRows, blockColumns, board->cells);
    updateImpValuesInCol(impossibleValues, cellCol, blockRows, blockColumns, board->cells);
    updateImpValuesInBlock(impossibleValues, cellRow, cellCol, blockRows, blockColumns, board->cells);
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

int getNumOfChars(int num){
    int count = 0;
    if (num == 0){
        return 1;
    }
    while (num != 0){
        num = num / 10;
        count += 1;
    }
    return count;
}

void executeReset(Game* game) {
    bool shouldPrint = false;
    Node* currentNode = game->undoRedoListPointer;
    while (currentNode != NULL){
        executeUndo(game, shouldPrint);
        currentNode = currentNode->prev;
    }
    destroyList(game->undoRedoList);
    game->undoRedoList = createList();
    printf("Board reset\n");
}
