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
    SudokuBoard* board = parseFile(file);
    fclose(file);
    return board;
}

bool fileHandler_saveBoardToFile(SudokuBoard* board, char* path, bool allFixed) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return false;
    }

    int n = board->blockRows;
    int m = board->blockColumns;
    fprintf(file, "%d %d\n", n, m);
    for (int i = 0; i < n * n * m * m; i++) {
        Cell* cell = board->cells[i];
        fprintf(file, "%d", cell->value);
        if (allFixed || cell->fixed){
            fprintf(file, ".");
        }
        printf("%d", i % (n*m));
        if (i % (n*m) != n*m - 1){
            fprintf(file, " ");
        } else {
            fprintf(file, "\n");
        }
    }
    fclose(file);
    return true;
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