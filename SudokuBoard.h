#ifndef SUDOKU_CONSOLE_SUDOKUBOARD_H
#define SUDOKU_CONSOLE_SUDOKUBOARD_H

#include <stdbool.h>

typedef struct {
    int value;
    int fixed; /* boolean value depicting whether the cell on the board is fixed or not */
    int valid;
    int* impossible_values;
} Cell;

typedef struct{
    int rows;
    int columns;
    Cell** cells; //Array of size rows*rows*columns*columns
} SudokuBoard;

SudokuBoard* sb_create(int rows, int columns);
void sb_print(SudokuBoard* sb, bool markErrors);
void sb_removeUnfixedCells(SudokuBoard *sb);
void sb_destroyBoard(SudokuBoard* sb);
SudokuBoard* sb_deepCloneBoard(SudokuBoard *template_sb);
int sb_isFull(SudokuBoard *sb);

#endif //SUDOKU_CONSOLE_SUDOKUBOARD_H