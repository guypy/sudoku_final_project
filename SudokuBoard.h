#ifndef SUDOKU_CONSOLE_SUDOKUBOARD_H
#define SUDOKU_CONSOLE_SUDOKUBOARD_H

#include <stdbool.h>

typedef struct {
    int value;
    bool fixed; /* boolean value depicting whether the cell on the board is fixed or not */
    bool valid;
    int* impossible_values;
} Cell;

typedef struct{
    int blockRows;
    int blockColumns;
    Cell** cells; //Array of size blockRows * blockRows * blockColumns * blockColumns
} SudokuBoard;

SudokuBoard* sb_create(int blockRows, int blockColumns);
void sb_print(SudokuBoard* sb, bool markErrors);
void sb_removeUnfixedCells(SudokuBoard *sb);
void sb_destroyBoard(SudokuBoard* sb);
SudokuBoard* sb_deepCloneBoard(SudokuBoard *template_sb);
int sb_isFull(SudokuBoard *sb);
bool sb_isSolvable(SudokuBoard *board);
bool sb_isErroneous(SudokuBoard *board);
bool sb_cellValidations(SudokuBoard *sb);
bool cell_isValid(SudokuBoard *sb, Cell* cell, int idxInBoard);
bool checkRow(SudokuBoard* sb, Cell* cell, int idxInBoard);
bool checkColumn(SudokuBoard* sb, Cell* cell, int idxInBoard);
bool checkBlock(SudokuBoard* sb, Cell* cell, int idxInBoard);

#endif //SUDOKU_CONSOLE_SUDOKUBOARD_H