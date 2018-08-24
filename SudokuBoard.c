#include <stdlib.h>
#include <stdio.h>
#include "SudokuBoard.h"
#include "Game.h"

#define BOARD_SIZE(rows, columns) ((rows)*(rows)*(columns)*(columns))

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
        if(!res->cells[i]){ /* allocation failed */
            printf("Error: %s has failed\n", "sb_create");
            exit(1);
        }
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

void printDashes(int n, int m) {
    for (int i = 0; i < 4 * n * n + m + 1; i++)
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
            new_sb->cells[i]->impossible_values[j] = template_sb->cells[i]->impossible_values[j];
        }
    }
    return new_sb;
}

void destroyCell(Cell* c) {
    free(c);
}

void sb_destroyBoard(SudokuBoard* sb) {
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