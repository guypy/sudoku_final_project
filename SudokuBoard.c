#include <stdlib.h>
#include <stdio.h>
#include "SudokuBoard.h"
#include "Game.h"
#include "ILPSolver.h"

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
        res->cells[i]->exhaustedValues = calloc(1, sizeof(int) * BOARD_SIZE(blockRows, blockColumns));
        if(!res->cells[i]){ /* allocation failed */
            printf("Error: %s has failed\n", "sb_create");
            exit(1);
        }
        res->cells[i]->valid = true;
        res->cells[i]->fixed = false;
    }

    return res;
}

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

bool isCellFixed(Cell *cell){
    return cell->fixed;
}

void printDashes(int n, int m) {
    int i;
    for (i = 0; i < 4 * n * n + m + 1; i++)
        printf("-");
}

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

void sb_removeUnfixedCells(SudokuBoard *sb){
    int i;
    for (i = 0; i < BOARD_SIZE(sb->blockRows, sb->blockColumns); ++i){
        if (sb->cells[i]->fixed == 0){
            sb->cells[i]->value = 0;
        }
    }
}

SudokuBoard* sb_deepCloneBoard(SudokuBoard *template_sb){
    int i,j;
    SudokuBoard* new_sb = sb_create(template_sb->blockRows, template_sb->blockColumns);
    for (i = 0; i < BOARD_SIZE(template_sb->blockRows, template_sb->blockColumns); ++i){
        new_sb->cells[i]->value = template_sb->cells[i]->value;
        new_sb->cells[i]->fixed = template_sb->cells[i]->fixed;
        /* clone impossible values array of cell */
        for (j = 0; j < (template_sb->blockRows * template_sb->blockColumns); ++j){
            new_sb->cells[i]->exhaustedValues[j] = template_sb->cells[i]->exhaustedValues[j];
        }
    }
    return new_sb;
}

void destroyCell(Cell* c) {
    free(c->exhaustedValues);
    free(c);
}

void sb_destroyBoard(SudokuBoard* sb) {
    if (sb == NULL){
        return;
    }
    int i = 0;
    for (i = 0; i < BOARD_SIZE(sb->blockRows, sb->blockColumns); i++) {
        destroyCell(sb->cells[i]);
    }
    free(sb);
}


/*
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

bool sb_isSolvable(SudokuBoard* board) {
    int resultCode = 0;
    ILP_solve(board, &resultCode);
    if (resultCode == SOLVED)
        return true;
    return false;
}

bool sb_isErroneous(SudokuBoard *board) {
    int i;
    sb_cellValidations(board);
    for (i = 0; i < BOARD_SIZE(board->blockRows, board->blockColumns); ++i) {
        if (!board->cells[i]->valid)
            return true;
    }
    return false;
}

void sb_cellValidations(SudokuBoard *board){
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockRows, board->blockColumns); ++i){
        Cell* cell = board->cells[i];
        cell->valid = cell_isValid(board, cell->value, i);
    }
}

bool sb_isEmpty(SudokuBoard* board){
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockColumns, board->blockRows); i++) {
        if (board->cells[i]->value)
            return false;
    }
    return true;
}

void sb_empty(SudokuBoard* board){
    int i;
    for (i = 0; i < BOARD_SIZE(board->blockColumns, board->blockRows); i++) {
        board->cells[i]->value = 0;
    }

}

bool cell_isValid(SudokuBoard *sb, int cellValue, int idxInBoard){
    return cellValue == 0 ||
           (checkRow(sb, cellValue, idxInBoard)    &&
            checkColumn(sb, cellValue, idxInBoard) &&
            checkBlock(sb, cellValue, idxInBoard));
}

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
