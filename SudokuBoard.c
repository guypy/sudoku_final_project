#include <stdlib.h>
#include <stdio.h>
#include "SudokuBoard.h"
#include "Game.h"

#define BOARD_SIZE(rows, columns) ((rows)*(columns))

SudokuBoard* sb_create(int rows, int columns){
    int i;
    SudokuBoard* res = (SudokuBoard*) malloc(sizeof(SudokuBoard));
    if (!res){ /* allocation failed */
        printf("Error: %s has failed\n", "sb_create");
        exit(1);
    }
    res->rows = rows;
    res->columns = columns;
    res->cells = malloc(sizeof(Cell*) * BOARD_SIZE(rows, columns));
    for (i = 0; i < BOARD_SIZE(rows, columns); ++i){
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
        printf("%d", cell->value);
        if (cell->fixed)
            printf(".");
        else if (!cell->valid && markErrors)
            printf("*");
        printf(" ");
    } else {
        printf("   ");
    }
}

void sb_print(SudokuBoard* sb, bool markErrors) {
    int i;
    printf("----------------------------------\n");
    for (i = 0; i < BOARD_SIZE(sb->rows, sb->columns); ++i){
        if (i % (sb->rows * sb->columns) == 0 && i != 0)
            printf("|\n----------------------------------\n| ");
        else if (i % (sb->columns) == 0 && i != 0)
            printf("|\n| ");
        else if (i % sb->rows == 0) {
            printf("| ");
        }
        printCell(sb->cells[i], markErrors);
    }
    printf("|\n----------------------------------\n");

}

void sb_removeUnfixedCells(SudokuBoard *sb){
    int i;
    for (i = 0; i < BOARD_SIZE(sb->rows, sb->columns); ++i){
        if (sb->cells[i]->fixed == 0){
            sb->cells[i]->value = 0;
        }
    }
}

SudokuBoard* sb_deepCloneBoard(SudokuBoard *template_sb){
    int i,j;
    SudokuBoard* new_sb = sb_create(template_sb->rows, template_sb->columns);
    for (i = 0; i < BOARD_SIZE(template_sb->rows, template_sb->columns); ++i){
        new_sb->cells[i]->value = template_sb->cells[i]->value;
        new_sb->cells[i]->fixed = template_sb->cells[i]->fixed;
        /* clone impossible values array of cell */
        for (j = 0; j < (template_sb->rows * template_sb->columns); ++j){
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
    for (i = 0; i < BOARD_SIZE(sb->rows, sb->columns); i++) {
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
    for (i = 0; i < BOARD_SIZE(sb->rows, sb->columns); ++i){
        if (sb->cells[i]->value == 0){
            return 0;
        }
    }
    return 1;
}