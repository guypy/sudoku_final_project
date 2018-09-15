#ifndef SUDOKU_CONSOLE_FILEHANDLER_H
#define SUDOKU_CONSOLE_FILEHANDLER_H

#include "SudokuBoard.h"
#include "ErrorPrinter.h"

/**
 *
 * FileHandler Header File
 *
 * This interface contains the declaration of functions that read files to create sudoku boards and save sudoku
 * boards to files.
 *
 */

SudokuBoard* fileHandler_readBoardFromFile(char* path);
bool fileHandler_saveBoardToFile(SudokuBoard* board, char* path, bool allFixed);

#endif
