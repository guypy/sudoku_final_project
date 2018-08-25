#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FileHandler.h"
#include "SudokuBoard.h"
#include "ErrorPrinter.h"

SudokuBoard* fileHandler_readBoardFromFile(char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }
    return parseFile(file);
}

SudokuBoard *parseFile(FILE *file) {
    int n, m;
    fscanf(file, "%d", &n);
    fscanf(file, "%d", &m);
    SudokuBoard* board = sb_create(n, m);
    for (int i = 0; i < n * n * m * m; i++) {
        Cell* cell = board->cells[i];
        fscanf(file, "%d", &cell->value);
        int c = getc(file);
        if (c == '.'){
            cell->fixed = true;
        }
    }
    sb_cellValidations(board);
    return board;
}