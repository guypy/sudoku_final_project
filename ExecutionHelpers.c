#include "ExecutionHelpers.h"

/**
 *
 * ExecutionHelpers Source File
 *
 * This file contains the implementation of functions that assist with the execution of commands entered by the user.
 * They are used by functions in CommandExecutions.c
 *
 */

/* ------------------------ */
/* ---- Shared Helpers ---- */
/* ------------------------ */

/**
 * This function clears the undoRedoList starting from the UndoRedoListPointer of the game onwards.
 * @param game - pointer to current game struct.
 */
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

/**
 * This function returns the one index in the array it receives, in which the value in the array is 0.
 * If there is more than one index in which this occurs, the functions returns -1
 * @param impossibleValues - boolean array in which indeces are values that can be entered in the sudoku game.
 * the value in the array is 0 if the index can be entered for a given cell and 1 otherwise.
 * @param size - size of the impossibleValues array
 */
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

/**
 * This functions marks values in the same column as a given cell as "impossible" in the impossibleValues boolean array.
 * @param impossibleValues - boolean array in which indeces are values that can be entered in the sudoku game.
 * the value in the array is 0 if the index can be entered for a given cell and 1 otherwise.
 * @param cellCol - column of cell in board
 * @param blockRows - number of rows in one block in the board
 * @param blockColumns - number of columns in one block in the board
 * @param cells - the array of cells in the board of the game
 */
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

/**
 * This function marks values in the same row as a given cell as "impossible" in the impossibleValues boolean array.
 * @param impossibleValues - boolean array in which indeces are values that can be entered in the sudoku game.
 * the value in the array is 0 if the index can be entered for a given cell and 1 otherwise.
 * @param cellRow - row of cell in board
 * @param blockRows - number of rows in one block in the board
 * @param blockColumns - number of columns in one block in the board
 * @param cells - the array of cells in the board of the game
 */
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

/**
 * This function marks values in the same block as a given cell as "impossible" in the impossibleValues boolean array.
 * @param impossibleValues - boolean array in which indeces are values that can be entered in the sudoku game.
 * the value in the array is 0 if the index can be entered for a given cell and 1 otherwise.
 * @param cellRow - row of cell in board.
 * @param cellcCol - column of cell in board.
 * @param blockRows - number of rows in one block in the board
 * @param blockColumns - number of columns in one block in the board
 * @param cells - the array of cells in the board of the game
 */
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

/**
 * This function marks all of the values that are "impossible" for a given cell (in the same row, column, block) in
 * the impossibleValues boolean array.
 * @param board - current game board
 * @param index - index of cell to check
 * @param impossibleValues - boolean array in which indeces are values that can be entered in the sudoku game.
 * the value in the array is 0 if the index can be entered for a given cell and 1 otherwise.
 */
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

/**
 * This calculates number of chars that a given number would take if it was converted to a string.
 * @param num - integer
 * @return - calculated number of chars.
 */
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


/* -------------------------- */
/* ---- Autofill Helpers ---- */
/* -------------------------- */

/**
 * This function fills the board of the game with values that need to be "autofilled", from the valuesToFill list
 * @param valuesToFill - list of values that need to be "autofilled"
 * @param game - pointer to current game struct.
 */
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


/* ---------------------- */
/* ---- Redo Helpers ---- */
/* ---------------------- */

/**
 * This function determines which node in the undoRedoList is the node that needs to be "redone" - either the node next
 * to the node pointed to by the undoRedoListPointer, or the head of the undoRedoList if we undid all moves.
 *
 * @param game - pointer to current game struct.
 * @param currentNode - current node pointed to by the undoRedoListPointer
 * @return - node to redo.
 */
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

/**
 * This function prints the step that has be "redone"
 * @param newValue
 * @param oldValue
 * @param column - column of changed cell.
 * @param row - row of changed cell.
 */
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
        printf("Redo %d,%d: from %d to %d\n", column + 1, row + 1, oldValue, newValue);
    }
}

/**
 * This function re-performs an autofill command that has been undone.
 * @param game - pointer to current game struct.
 * @param nodeToRedo - node in the undoRedoList containing the "autofill" commands that is being done again.
 */
void redoAutofill(Game *game, Node *nodeToRedo){
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

    executePrintBoard(game);

    currentAutoFillNode = autoFillList->head;
    while (currentAutoFillNode != NULL){
        newValue = atoi(currentAutoFillNode->data->args[2]);;
        oldValue = currentAutoFillNode->data->prevValue;
        column = atoi(currentAutoFillNode->data->args[0]);
        row = atoi(currentAutoFillNode->data->args[1]);

        printRedoStep(newValue, oldValue, column, row);

        currentAutoFillNode = currentAutoFillNode->next;
    }
}

/**
 * This function re-performs a set command that has been undone.
 * @param game - pointer to current game struct.
 * @param nodeToRedo - node in the undoRedoList containing the "set" command that is being done again.
 */
void redoSet(Game *game, Node *nodeToRedo){
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

    executePrintBoard(game);

    printRedoStep(newValue, currentValue,column, row);
}

/**
 * This function re-performs a generate command that has been undone.
 * @param game - pointer to current game struct.
 * @param nodeToRedo - node in the undoRedoList containing the "generate" command that is being done again.
 */
void redoGenerate(Game *game, Node *nodeToRedo) {
    sb_destroyBoard(game->board);
    game->board = sb_deepCloneBoard(nodeToRedo->generatedBoard);
    executePrintBoard(game);
    printf("Redo: Generated board\n");
}


/* ---------------------- */
/* ---- Undo Helpers ---- */
/* ---------------------- */

/**
 * This function prints the step that us being undone.
 * @param oldValue
 * @param newValue
 * @param column - column of changed cell.
 * @param row - row of changed cell.
 */
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

/**
 * This function reverts the autofill command.
 * @param game - pointer to current game struct.
 * @param shouldPrint - boolean deciding whether or not to print the autofilled values that are being undone and
 * the board after the command is executed.
 *
 */
void undoAutofill(Game *game, bool shouldPrint){
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
        executePrintBoard(game);
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
}

/**
 * This function reverts the set command.
 * @param game - pointer to current game struct.
 * @param shouldPrint - boolean deciding whether or not to print the set value being undone and the board after
 * the command is executed.
 */
void undoSet(Game *game, bool shouldPrint){
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
        executePrintBoard(game);

        printUndoStep(currentValue, prevValue, column, row);
    }
}

/**
 * This function reverts the generate command and sets the board back to an emmpty board.
 * @param game - pointer to current game struct.
 * @param shouldPrint - boolean deciding whether or not to print that the "generate" command had been undone.
 */
void undoGenerate(Game *game, bool shouldPrint) {
    sb_empty(game->board);
    if(shouldPrint) {
        printf("Undo Generate, board is empty\n");
        executePrintBoard(game);
    }
}


/* -------------------------- */
/* ---- Generate Helpers ---- */
/* -------------------------- */

/**
 * This function fills a given sudoku board with a given number of valid random values.
 * @param board - pointer to sudoku board struct to fill.
 * @param valueToFillCount - number of valid random values to fill.
 * @return - boolean value representing whether or not we succeeded in filling the correct number of valid values.
 * (false will be returned if we reached a board that is not solvable)
 */
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

/**
 * This function empties a given number of random cells from the board
 * @param board - pointer to sudoku board struct.
 * @param valueToRemoveCount  - number of cells to empty.
 */
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

/**
 * This function determines whether or not a given cell can be filled with a valid value.
 * @param board - pointer to sudoku board struct.
 * @param idx - index of cell to check
 * @return - true if there is more than one valid value for the cell, false if there are no valid values left for the
 * given cell.
 */
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
