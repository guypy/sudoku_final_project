#include "ExecutionHelpers.h"

/* ------------------------ */
/* ---- Shared Helpers ---- */
/* ------------------------ */

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

void redoGenerate(Game *game, Node *nodeToRedo) {
    sb_destroyBoard(game->board);
    game->board = sb_deepCloneBoard(nodeToRedo->generatedBoard);
    executePrintBoard(game);
    printf("Redo: Generated board\n");
}


/* ---------------------- */
/* ---- Undo Helpers ---- */
/* ---------------------- */

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
