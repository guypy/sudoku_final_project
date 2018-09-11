#ifndef SUDOKU_CONSOLE_ILPSOLVER_H
#define SUDOKU_CONSOLE_ILPSOLVER_H

#include "SudokuBoard.h"
#define SOLVED 2
#define NO_SOLUTION 1
#define ERROR 0

SudokuBoard* ILP_solve(SudokuBoard* board, int* resultCode);

#endif //SUDOKU_CONSOLE_ILPSOLVER_H