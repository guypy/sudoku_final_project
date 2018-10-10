#include <stdio.h>
#include "FileHandler.h"

/**
 *
 * FileHandler Source File
 *
 * This file contains the implementation of functions that read files to create sudoku boards and save sudoku
 * boards to files.
 *
 */

/**
 * This function parses a given file to create a sudoku board
 * @param file - file to read from.
 * @return - pointer to sudoku board created according to the given file.
 */
SudokuBoard *parseFile(FILE *file) {
    int n = 0, m = 0, i = 0, c = 0;
    SudokuBoard* board;
    Cell* cell;
    if (!fscanf(file, "%d", &n) || !fscanf(file, "%d", &m))
        return NULL;
    board = sb_create(n, m);
    for (i = 0; i < n * n * m * m; i++) {
        cell = board->cells[i];
        if (!fscanf(file, "%d", &cell->value))
            return NULL;
        c = getc(file);
        if (c == '.'){
            cell->fixed = true;
        }
    }
    sb_cellValidations(board);
    return board;
}

/**
 * This function handles the creation of a sudoku board based on the information within a given file.
 * @param path - path to file
 * @return - pointer to sudoku board created according to the given file.
 */
SudokuBoard* fileHandler_readBoardFromFile(char* path) {
    SudokuBoard* board;
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }
    board = parseFile(file);
    fclose(file);
    return board;
}

/**
 * This file saves a given sudoku board to a file.
 * @param board - pointer to sudoku board to save.
 * @param path - path to file in which to save the given board.
 * @param allFixed - boolean representing whether all values in the board should be saved as fixed values (true in
 * "edit" mode and false in "solve" mode)
 * @return - boolean representing whether or not the board was successfully saved to the file.
 */
bool fileHandler_saveBoardToFile(SudokuBoard* board, char* path, bool allFixed) {
    int i;
    int n = board->blockColumns;
    int m = board->blockRows;
    Cell* cell;
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%d %d\n", n, m);
    for (i = 0; i < n * n * m * m; i++) {
        cell = board->cells[i];
        fprintf(file, "%d", cell->value);
        if (cell->value != 0 && (allFixed || cell->fixed)){
            fprintf(file, ".");
        }
        if (i % (n*m) != n*m - 1){
            fprintf(file, " ");
        } else {
            fprintf(file, "\n");
        }
    }
    fclose(file);
    return true;
}
