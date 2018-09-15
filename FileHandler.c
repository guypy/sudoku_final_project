#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FileHandler.h"

SudokuBoard *parseFile(FILE *file) {
    int n = 0, m = 0, i = 0, c = 0;
    SudokuBoard* board;
    Cell* cell;
    fscanf(file, "%d", &m);
    fscanf(file, "%d", &n);
    board = sb_create(n, m);
    for (i = 0; i < n * n * m * m; i++) {
        cell = board->cells[i];
        fscanf(file, "%d", &cell->value);
        c = getc(file);
        if (c == '.'){
            cell->fixed = true;
        }
    }
    sb_cellValidations(board);
    return board;
}

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
