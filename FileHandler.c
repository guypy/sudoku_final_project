#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FileHandler.h"
#include "SudokuBoard.h"
#include "ErrorPrinter.h"

SudokuBoard *parseFile(FILE *file);

SudokuBoard* fileHandler_readBoardFromFile(char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("fopen");
        errPrinter_cannotOpenFile();
        return NULL;
    }
    return parseFile(file);
}

SudokuBoard *parseFile(FILE *file) {
    int n, m;
    fscanf(file, "%d", &n);
    fscanf(file, "%d", &m);
    SudokuBoard* board = sb_create(n, m);
    assert(board);
    for (int i = 0; i < n * m * n * m; i++) {
        Cell* cell = board->cells[i];
        fscanf(file, "%d", &cell->value);
        int c = getc(file);
        if (c == '.'){
            cell->fixed = true;
        }
    }
    return board;
}