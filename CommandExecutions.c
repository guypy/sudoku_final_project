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
    executePrintBoard(game);
}

void executeEdit(Game* game, Command* cmd) {
    restartGame(game);
    if (cmd->numOfArgs > 0) {
        game->board = fileHandler_readBoardFromFile(cmd->args[0]);
        if (game->board == NULL){
            errPrinter_cannotOpenFile();
            return;
        }
    } else {
        game->board = sb_create(DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
    }
    game->mode = EDIT;
    executePrintBoard(game);
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

    if (oldValue == valueToSet) {
        sprintf(cmd->action, "redundant_set");
        executePrintBoard(game);
        return;
    }

    cmd->prevValue = oldValue;
    game->board->cells[idx]->value = valueToSet;
    sb_cellValidations(game->board);

    destroyNextNodesBeforeAppend(game);
    append(game->undoRedoList, cmd);
    game->undoRedoListPointer = game->undoRedoList->tail;
    executePrintBoard(game);
    if (sb_isFull(game->board)){
        if (sb_isErroneous(game->board)){
            errPrinter_puzzleSolutionErroneous();
        } else{
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
    int valueToRemoveCount = BOARD_SIZE(game->board->blockColumns, game->board->blockRows) - atoi(cmd->args[1]);
    int tryCount = 0, resultCode = 0;
    SudokuBoard* solved = NULL;

    if (valueToRemoveCount == BOARD_SIZE(game->board->blockColumns, game->board->blockRows)) {
        sprintf(cmd->action, "redundant_generate");
        executePrintBoard(game);
        return;
    }

    do {
        tryCount++;
        sb_empty(game->board);
        if (!fillBoardWithRandValues(game->board, valuesToFillCount))
            continue;
        solved = ILP_solve(game->board, &resultCode);
    } while (resultCode != SOLVED && tryCount < 1000);
    
    if (tryCount == 1000) {
        errPrinter_puzzleGeneratorFailed();
        sb_empty(game->board);
        return;
    }

    sb_destroyBoard(game->board);
    game->board = solved;

    removeValuesFromBoard(game->board, valueToRemoveCount);
    executePrintBoard(game);

    destroyNextNodesBeforeAppend(game);
    append(game->undoRedoList, cmd);
    game->undoRedoListPointer = game->undoRedoList->tail;
    game->undoRedoListPointer->generatedBoard = sb_deepCloneBoard(game->board);
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
        undoSet(game, shouldPrint);
    }
    if (strcmp(action, ACTION_AUTOFILL) == 0){
        undoAutofill(game, shouldPrint);
    }
    if (strcmp(action, ACTION_GENERATE) == 0) {
        undoGenerate(game, shouldPrint);
    }
    game->undoRedoListPointer = game->undoRedoListPointer->prev;
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
        redoSet(game, nodeToRedo);
    }
    if (strcmp(action, ACTION_AUTOFILL) == 0){
        redoAutofill(game, nodeToRedo);
    }
    if (strcmp(action, ACTION_GENERATE) == 0) {
        redoGenerate(game, nodeToRedo);
    }

    if (game->undoRedoListPointer != NULL){
        game->undoRedoListPointer = game->undoRedoListPointer->next;
    } else {
        game->undoRedoListPointer = game->undoRedoList->head;
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
        return;
    }
    printf("Saved to: %s\n", path);
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
            sb_destroyBoard(solved);
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
    } else if (numOfSolutions > 1) {
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
    } else {
        sprintf(cmd->action, "redundant_autofill");
        destroyList(valuesToFill);
    }
    executePrintBoard(game);
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
