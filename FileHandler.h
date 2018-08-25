#include "SudokuBoard.h"

#ifndef SUDOKU_CONSOLE_FILEHANDLER_H
#define SUDOKU_CONSOLE_FILEHANDLER_H
SudokuBoard* fileHandler_readBoardFromFile(char* path);
bool fileHandler_saveBoardToFile(SudokuBoard* board, char* path, bool allFixed);
SudokuBoard *parseFile(FILE *file);
#endif //SUDOKU_CONSOLE_FILEHANDLER_H
