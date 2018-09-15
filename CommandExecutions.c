#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CommandExecutions.h"
#include "Game.h"

/**
 *
 *  Command Executions Source File
 *
 *  This file contains the implementation of functions that carry out the execution of all possible commands throughout the game.
 *  Some functions may use helper methods defined in ExecutionHelpers.c.
 *
 */

/**
 * This function executes the "solve" command, which parses a file according to the path given by the user, and creates
 * the game board accordingly. The mode of the game is switched to "solve".
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to the "solve" command being executed.
 */
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

/**
 * This function executes the "edit" command. This command can either create an empty game board, or parse a file
 * according to the path given by the user and create the game board accordingly. The mode of the game is switched to
 * "edit".
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to the "edit" command being executed.
 */
void executeEdit(Game* game, Command* cmd) {
    restartGame(game);
    if (cmd->numOfArgs > 0) {
        game->board = fileHandler_readBoardFromFile(cmd->args[0]);
        if (game->board == NULL){
            errPrinter_cannotOpenFile();
            return;
        }
        sb_setAllCellsUnfixed(game->board);
    } else {
        game->board = sb_create(DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
    }
    game->mode = EDIT;
    executePrintBoard(game);
}

/**
 * This function executes the "mark_errors" command, which changes the state of the "markErrors" attribute of the
 * current game to 0 or 1 according to input of the user.
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to the "mark_errors" command being executed.
 */
void executeMarkErrors(Game* game, Command* cmd) {
    game->markErrors = (bool) atoi(cmd->args[0]);
}

/**
 * This command executes the "print_board" command, which prints the current game board.
 * @param game - pointer to current game struct.
 */
void executePrintBoard(Game* game) {
    sb_print(game->board, (game->markErrors || game->mode == EDIT));
}

/**
 * This function executes the "set" command, which updates the value of a cell in the game board.
 * Column, row, and new value of cell are stored as arguments of the command.
 * If the command changes the board it is stored in the undoRedo list of the game.
 * In solve mode, if the board is full and valid after the command is executed, the mode of the game is switched
 * to "init".
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to the "set" command being executed.
 */
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
    if (game->mode == SOLVE && sb_isFull(game->board)){
        if (sb_isErroneous(game->board)){
            errPrinter_puzzleSolutionErroneous();
        } else{
            printf("Puzzle solved successfully\n");
            game->mode = INIT;
        }
    }
}

/**
 * This function executes the "validate" command, which determines whether the current game board is solvable, by
 * using an external ILP solver to solve it. It is only executed if the current board is valid.
 *
 * @param game -pointer to current game struct.
 */
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

/**
 * This function executes the "generate" command, which randomly generates a game board.
 * The command randomly enters X valid values in the board, solves it using an external ILP solver, and deltes all but
 * Y values. X and Y are arguments of the command.
 * If the command changes the board it is stored in the undoRedo list of the game.
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to the "generate" command being executed.
 */
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

/**
 * This function executes the "undo" command which reverts the last move that changed the board.
 *
 * The command undone is the command that the undoRedoListPointer is currently pointing to.
 * The undoRedoListPointer is moved one command back.
 *
 * @param game - pointer to current game struct.
 * @param shouldPrint - boolean deciding whether the moves that are being undone and the board are printed at the end
 * of this command.
 */
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

/**
 * This function executes the "redo" command, which performs the last move that has been undone by the user.
 *
 * The command "redone" is the command next to the one pointed to by the undoRedoListPointer, since this is the last
 * command that was undone.
 * The undoRedoListPointer is moved one command forward.
 *
 * @param game - pointer to current game struct.
 */
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

/**
 * This function executes the "save" command, which saves the board to a file according to the path entered by the user.
 * In "edit" mode the board is saved only if it does not contain erroneous values and is solvable.
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to "save" command being executed.
 */
void executeSave(Game* game, Command* cmd) {
    char* path = cmd->args[0];
    if (game->mode == EDIT) {
        if (sb_isErroneous(game->board)) {
            errPrinter_erroneousValues();
            return;
        }
        if (!sb_isSolvable(game->board)) {
            errPrinter_boardValidationFailed();
            return;
        }
    }
    if (!fileHandler_saveBoardToFile(game->board, path, game->mode == EDIT)){
        errPrinter_cannotCreateOrModifyFile();
        return;
    }
    printf("Saved to: %s\n", path);
}

/**
 * This function executes the "hint" command, which tells the user what value to set in the column and row they entered,
 * by solving the board using an external ILP solver.
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to "hint" command being executed.
 */
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

/**
 * This function executes the "num_solutions" command, which uses exhaustive backtracking to compute the number of
 * solutions for the given board. It is only executed if the board contains valid values.
 *
 * @param game - pointer to current game struct.
 */
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

/**
 * This function executes the "autofill" command, which fills the board with "obvious" values - cells that have only one
 * valid value that can be filled by the rules of the game "sudoku".
 * It is only executed if the board contains valid values.
 * If the board has been changed, the command is added to the undoRedoList of the game.
 * In solve mode, if the board is full and valid after the command is executed, the mode of the game is switched
 * to "init".
 *
 * @param game - pointer to current game struct.
 * @param cmd - pointer to "autofill" command being executed.
 */
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

/**
 * This function executes the "reset" command which reverts the board to its initial state by undoing all commands
 * in the undoRedoList of the game.
 *
 * @param game - pointer to current game struct.
 */
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
