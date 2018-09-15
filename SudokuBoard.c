#include <stdlib.h>
#include <stdio.h>
#include "SudokuBoard.h"

/**
 *
 * SudokuBoard Source File
 *
 * This file contains the implementation of the functions that can be performed on a SudokuBoard struct and a Cell
 * struct.
 *
 */

/**
 * This function creats an empty sudoku board, according to the given block dimensions.
 * @param blockRows - number of rows in one block in the board.
 * @param blockColumns - number of columns in one block in the board.
 * @return - pointer to the created sudoku board.
 */
SudokuBoard* sb_create(int blockRows, int blockColumns){
    int i;
    SudokuBoard* res = (SudokuBoard*) malloc(sizeof(SudokuBoard));
    if (!res){ /* allocation failed */
        printf("Error: %s has failed\n", "sb_create");
        exit(1);
    }
    res->blockRows = blockRows;
    res->blockColumns = blockColumns;
    res->cells = malloc(sizeof(Cell*) * BOARD_SIZE(blockRows, blockColumns));
    for (i = 0; i < BOARD_SIZE(blockRows, blockColumns); ++i){
        res->cells[i] = (Cell*)calloc(1, sizeof(Cell));
        res->cells[i]->exhaustedValues = calloc(1, sizeof(int) * (blockRows * blockColumns));
        if(!res->cells[i]){ /* allocation failed */
            printf("Error: %s has failed\n", "sb_create");
            exit(1);
        }
        res->cells[i]->valid = true;
        res->cells[i]->fixed = false;
    }

    return res;
}

/**
 * This function prints a given cell, considering if it is fixed or valid. Fixed cells are always valid.
 * @param cell - pointer to cell to be printed.
 * @param markErrors - boolean representing whether or not invalid values should be marked with a "*".
 */
void printCell(Cell *cell, bool markErrors) {
    if (cell->value) {
        printf(" %2d", cell->value);
        if (cell->fixed)
            printf(".");
        else if (!cell->valid && markErrors)
            printf("*");
        else
            printf(" ");
    } else {
        printf("    ");
    }
}

/**
 * This function determines whether or not a given cell is fixed.
 * @param cell - pointer to cell.
 * @return - true if cell is fixed, false otherwise.
 */
bool cell_isFixed(Cell *cell){
    return cell->fixed;
}

/**
 * This function handles printing dashes as part of printing the game board.
 * @param n - columns
 * @param m - rows
 */
void printDashes(int n, int m) {
    int i;
    for (i = 0; i < 4 * n * m + m + 1; i++)
        printf("-");
}

/**
 * This function handles printing the given sudoku board, considering whether or not invalid values shoudl be
 * printed with a "*".
 * @param sb - pointer to sudoku board to print.
 * @param markErrors - boolean deciding whether to add a "*" next to invalid values while printing.
 */
void sb_print(SudokuBoard* sb, bool markErrors) {
    int i;
    printDashes(sb->blockRows, sb->blockColumns);
    printf("\n");
    for (i = 0; i < BOARD_SIZE(sb->blockRows, sb->blockColumns); ++i){
        if (i % (sb->blockRows * sb->blockColumns * sb->blockColumns) == 0 && i != 0) {
            printf("|\n");
            printDashes(sb->blockRows, sb->blockColumns);
            printf("\n|");
        } else if (i % (sb->blockColumns * sb->blockRows) == 0 && i != 0)
            printf("|\n|");
        else if (i % sb->blockRows == 0) {
            printf("|");
        }
        printCell(sb->cells[i], markErrors);
    }
    printf("|\n");
    printDashes(sb->blockRows, sb->blockColumns);
    printf("\n");
}

/**
 * This functions deep clones a given board.
 * @param template_sb - pointer to sudoku board to deep clone
 * @return - pointer to new, deep cloned board.
 */
SudokuBoard* sb_deepCloneBoard(SudokuBoard *template_sb){
    int i,j;
    SudokuBoard* new_sb = sb_create(template_sb->blockRows, template_sb->blockColumns);
    for (i = 0; i < BOARD_SIZE(template_sb->blockRows, template_sb->blockColumns); ++i){
        new_sb->cells[i]->value = template_sb->cells[i]->value;
        new_sb->cells[i]->fixed = template_sb->cells[i]->fixed;
        new_sb->cells[i]->valid = template_sb->cells[i]->valid;
        /* clone impossible values array of cell */
        for (j = 0; j < (template_sb->blockRows * template_sb->blockColumns); ++j){
            new_sb->cells[i]->exhaustedValues[j] = template_sb->cells[i]->exhaustedValues[j];
        }
    }
    return new_sb;
}

/**
 * This function destroys a given cell
 * @param c - pointer to cell.
 */
void destroyCell(Cell* c) {
    free(c->exhaustedValues);
    free(c);
}

/**
 * This function destroys a given board
 * @param sb - pointer to board.
 */
void sb_destroyBoard(SudokuBoard* sb) {
    int i;
    if (sb == NULL){
        return;
    }
    for (i = 0; i < BOARD_SIZE(sb->blockRows, sb->blockColumns); i++) {
        destroyCell(sb->cells[i]);
    }
    free(sb->cells);
    free(sb);
}

void sb_setAllCellsUnfixed(SudokuBoard* board) {
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockRows, board->blockColumns); ++i){
        board->cells[i]->fixed = false;
    }
}

/**
 * This functions checks if the sudoku board is full, i.e there are no cells with value '0'.
 * @return: 1 if full, 0 if not full
 */
int sb_isFull(SudokuBoard *sb){
    int i;
    for (i = 0; i < BOARD_SIZE(sb->blockRows, sb->blockColumns); ++i){
        if (sb->cells[i]->value == 0){
            return 0;
        }
    }
    return 1;
}

/**
 * This function checks if the given board has a valid solution by using an external ILP solver.
 * @param board - pointer to sudoku board
 * @return - true if board has a solution, false otherwise
 */
bool sb_isSolvable(SudokuBoard* board) {
    int resultCode = 0;
    SudokuBoard* solved;
    solved = ILP_solve(board, &resultCode);
    if (resultCode == SOLVED) {
        sb_destroyBoard(solved);
        return true;
    }
    return false;
}

/**
 * This function checks if the board contains invalid values.
 * @param board - pointer to board.
 * @return - true if board contains invalid values, false otherwise.
 */
bool sb_isErroneous(SudokuBoard *board) {
    int i;
    sb_cellValidations(board);
    for (i = 0; i < BOARD_SIZE(board->blockRows, board->blockColumns); ++i) {
        if (!board->cells[i]->valid)
            return true;
    }
    return false;
}

/**
 * This function updates the "valid" attribute of all cells in a given board.
 * @param board - pointer to sudoku board.
 */
void sb_cellValidations(SudokuBoard *board){
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockRows, board->blockColumns); ++i){
        Cell* cell = board->cells[i];
        cell->valid = cell_isValid(board, cell->value, i);
    }
}

/**
 * This function determines whether the given board is empty. i.e. all cells are of valves 0.
 * @param board - pointer to sudoku board.
 * @return - true if board is empty, false otherwise.
 */
bool sb_isEmpty(SudokuBoard* board){
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockColumns, board->blockRows); i++) {
        if (board->cells[i]->value)
            return false;
    }
    return true;
}

/**
 * This function empties the given board, meaning sets the value of all of its cells to 0.
 * @param board - pointer to sudoku board.
 */
void sb_empty(SudokuBoard* board){
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockColumns, board->blockRows); i++) {
        board->cells[i]->value = 0;
    }

}

/**
 * This function checks the row of the cell in a given index, and determines if the given value can be entered as
 * a valid one for that cell accordingly.
 * @param sb - pointer to sudoku board,
 * @param cellValue - value to be checked.
 * @param idxInBoard - index of cell whose row will be checked
 * @return - true if given value is valid according to row, false otherwise (it exists in the given row)
 */
bool checkRow(SudokuBoard* sb, int cellValue, int idxInBoard){
    int value, i, cellRow, checkedCellIdx;
    int n = sb->blockRows;
    int m = sb->blockColumns;
    cellRow = idxInBoard / (n*m);
    for (i = 0; i < (n*m); ++i){
        checkedCellIdx = cellRow*(n*m) + i;
        if (idxInBoard == checkedCellIdx)
            continue;
        value = sb->cells[checkedCellIdx]->value;
        if (value == cellValue) {
            return false;
        }
    }
    return true;
}

/**
 * This function checks the column of the cell in the given index, and determines if the given value can be entered
 * as a valid one for that cell accordingly.
 * @param sb - pointer to sudoku board,
 * @param cellValue - value to be checked
 * @param idxInBoard - index of cell whose column will be checked
 * @return - true if given value is valid according to column, false otherwise (it exits in given column)
 */
bool checkColumn(SudokuBoard* sb, int cellValue, int idxInBoard) {
    int value, i, cellColumn;
    int n = sb->blockRows;
    int m = sb->blockColumns;
    cellColumn = idxInBoard % (n*m);
    for (i = 0; i < (n*m); ++i){
        int checkedCellIdx = i*(n*m) + cellColumn;
        if (idxInBoard == checkedCellIdx)
            continue;
        value = sb->cells[checkedCellIdx]->value;
        if (value == cellValue) {
            return false;
        }
    }
    return true;
}

/**
 * Thisfunction checkes the block of the cell in the given index, and determines if the given value can be entered
 * as a valid one for that cell accordingly.
 * @param sb - pointer to sudoku board,
 * @param cellValue - value to be checked
 * @param idxInBoard - index of cell whose column will be checked
 * @return - true if given value is valid according to block, false otherwise (it exits in given block)
 */
bool checkBlock(SudokuBoard* sb, int cellValue, int idxInBoard) {
    int value, i, j;
    int n = sb->blockRows;
    int m = sb->blockColumns;
    int cellColumn = idxInBoard % (n*m);
    int cellRow = idxInBoard / (n*m);
    int columnBlock = cellColumn / m;
    int rowBlock = cellRow / n;
    for (i = 0; i < n; ++i){
        for(j = 0; j < m; ++j) {
            int checkedCellIdx = (rowBlock * n + i) * n*m + columnBlock * m + j;
            if (idxInBoard == checkedCellIdx)
                continue;
            value = sb->cells[checkedCellIdx]->value;
            if (value == cellValue) {
                return false;
            }
        }
    }
    return true;
}

/**
 * This function returns whether or not a given value can be entered in the cell in the given index, accordign to
 * the rules of sudoku (i.e. value does not already exist in row, column, or block).
 * @param sb - pointer to sudoku board
 * @param cellValue - value to be checked
 * @param idxInBoard - index of cell to check
 * @return - true if value can be entered, false otherwise.
 */
bool cell_isValid(SudokuBoard *sb, int cellValue, int idxInBoard){
    return cellValue == 0 ||
           (checkRow(sb, cellValue, idxInBoard)    &&
            checkColumn(sb, cellValue, idxInBoard) &&
            checkBlock(sb, cellValue, idxInBoard));
}
