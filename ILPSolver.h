#ifndef SUDOKU_CONSOLE_ILPSOLVER_H
#define SUDOKU_CONSOLE_ILPSOLVER_H

#include "gurobi_c.h"
#include "ErrorPrinter.h"
#include "SudokuBoard.h"

/**
 *
 * ILPSolver Header File
 *
 * This interface declares functions needed to solve a sudokuboard using the Gurobi ILP solver.
 * It also declares constants for the state of a solution.
 *
 */

#define SOLVED 2
#define NO_SOLUTION 1
#define ERROR 0

SudokuBoard* ILP_solve(SudokuBoard* board, int* resultCode);

#endif
