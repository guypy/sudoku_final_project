#ifndef SUDOKU_CONSOLE_SUDOKUBOARD_H
#define SUDOKU_CONSOLE_SUDOKUBOARD_H

#include <stdbool.h>

typedef struct {
    int value;
    bool fixed; /* boolean value depicting whether the cell on the board is fixed or not */
    bool valid;
    int* exhaustedValues;
} Cell;

typedef struct{
    int blockRows;
    int blockColumns;
    Cell** cells;
} SudokuBoard;

SudokuBoard* sb_create(int blockRows, int blockColumns);
void sb_print(SudokuBoard* sb, bool markErrors);
void sb_removeUnfixedCells(SudokuBoard *sb);
void sb_destroyBoard(SudokuBoard* sb);
SudokuBoard* sb_deepCloneBoard(SudokuBoard *template_sb);
int sb_isFull(SudokuBoard *sb);
bool sb_isSolvable(SudokuBoard *board);
bool sb_isErroneous(SudokuBoard *board);
void sb_cellValidations(SudokuBoard *sb);
bool cell_isValid(SudokuBoard *sb, int cell_value, int idxInBoard);
bool checkRow(SudokuBoard* sb, int cell_value, int idxInBoard);
bool checkColumn(SudokuBoard* sb, int cell_value, int idxInBoard);
bool checkBlock(SudokuBoard* sb, int cell_value, int idxInBoard);
bool isCellFixed(Cell *cell);

#endif
