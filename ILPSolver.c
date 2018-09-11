#include <stdlib.h>
#include <assert.h>
#include "stdio.h"
#include "ILPSolver.h"
#include "gurobi_c.h"
#include "ErrorPrinter.h"

bool addVariablesToModel(GRBmodel *model, int boardSize) {
    int* coeffs;
    char* variableTypes;
    int i;

    coeffs = calloc((size_t) boardSize, sizeof(int));
    assert(coeffs);
    variableTypes = calloc((size_t ) boardSize, sizeof(char));
    assert(variableTypes);

    for (i = 0; i < boardSize; i++) {
        coeffs[i] = 1;
        variableTypes[i] = GRB_BINARY;
    }
    if (GRBaddvars(model, boardSize, 0, NULL, NULL, NULL, NULL, NULL, NULL, variableTypes, NULL) ||
        GRBupdatemodel(model)) {
        free(coeffs);
        free(variableTypes);
        return false;
    }

    free(coeffs);
    free(variableTypes);
    return true;
}

bool addNoEmptyCellConstraint(GRBmodel *model, int blockRows, int blockColumns) {
    int i, j, k;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    assert(indexes);
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(values);

    for (i = 0; i < blockRows * blockColumns; i++) {
        for (j = 0; j < blockRows * blockColumns; j++) {
            for (k = 0; k < blockRows * blockColumns; k++) {
                indexes[k] = i * blockRows * blockColumns * blockRows * blockColumns + j * blockRows * blockColumns + k;
                values[k] = 1.0;
            }
            if(GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL)) {
                free(indexes);
                free(values);
                return false;
            }
        }
    }
    return true;
}

bool addRowConstraint(GRBmodel *model, int blockRows, int blockColumns){
    int i, j, k;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    assert(indexes);
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(values);

    for (k = 0; k < blockRows * blockColumns; k++) {
        for (j = 0; j < blockRows * blockColumns; j++) {
            for (i = 0; i < blockRows * blockColumns; i++) {
                indexes[i] = i * blockRows * blockColumns * blockRows * blockColumns + j * blockRows * blockColumns + k;
                values[i] = 1.0;
            }

            if(GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL)) {
                free(indexes);
                free(values);
                return false;
            }
        }
    }
    return true;
}

int addColumnConstraint(GRBmodel *model, int blockRows, int blockColumns){
    int i, j, k;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    assert(indexes);
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(values);

    for (k = 0; k < blockRows * blockColumns; k++) {
        for (i = 0; i < blockRows * blockColumns; i++) {
            for (j = 0; j < blockRows * blockColumns; j++) {
                indexes[j] = i * blockRows * blockColumns * blockRows * blockColumns + j * blockRows * blockColumns + k;
                values[j] = 1.0;
            }

            if(GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL)) {
                free(indexes);
                free(values);
                return false;
            }
        }
    }
    return true;
}

bool addBlockConstraint(GRBmodel *model, int blockRows, int blockColumns){
    int m, n, i, j, k, count;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    assert(indexes);
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(values);

    for (i = 0; i < blockRows * blockColumns; i++) {
        for (j = 0; j < blockColumns; j++) {
            for (k = 0; k < blockRows; k++) {
                count = 0;
                for (m = j*blockRows; m < (j+1)*blockRows; m++) {
                    for (n = k*blockColumns; n < (k+1)*blockColumns; n++) {
                        indexes[count] = m*blockRows * blockColumns*blockRows * blockColumns + n*blockRows * blockColumns + i;
                        values[count] = 1.0;
                        count++;
                    }
                }
                if(GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL)) {
                    free(indexes);
                    free(values);
                    return false;
                }
            }
        }
    }
    return true;
}

bool addConstraints(GRBmodel *model, SudokuBoard* board) {
    if (addNoEmptyCellConstraint(model, board->blockRows, board->blockColumns) &&
        addRowConstraint(model, board->blockRows, board->blockColumns)         &&
        addColumnConstraint(model, board->blockRows, board->blockColumns)      &&
        addBlockConstraint (model, board->blockRows, board->blockColumns)) {
        return true;
    }
    return false;
}

void copySolution(SudokuBoard* board, const double *solutionMatrix) {
    int row, column, k;
    for (row = 0; row < board->blockRows * board->blockColumns; row++) {
        for (column = 0; column < board->blockRows * board->blockColumns; column++) {
            for (k = 1; k <= board->blockRows * board->blockColumns; k++) {
                if(solutionMatrix[row*board->blockRows * board->blockColumns*board->blockRows * board->blockColumns + column*board->blockRows * board->blockColumns + k-1]){
                    board->cells[row * (board->blockColumns * board->blockRows) + column]->value = k;
                }
            }
        }
    }
}

void freeResources(GRBenv* env, GRBmodel* model, SudokuBoard* board, double* solutionsMatrix) {
    if (env != NULL)             GRBfreeenv(env);
    if (model != NULL)           GRBfreemodel(model);
    if (board != NULL)           sb_destroyBoard(board);
    if (solutionsMatrix != NULL) free(solutionsMatrix);
}

SudokuBoard* ILP_solve(SudokuBoard* board, int* resultCode) {
    SudokuBoard* solvedBoard = sb_deepCloneBoard(board);
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int errorCode, optimizationStatus;
    int boardSize = board->blockColumns * board->blockColumns * board->blockRows * board->blockRows;
    double *solutionMatrix = calloc((size_t) boardSize, sizeof(double));
    assert(solutionMatrix);

    errorCode = GRBloadenv(&env, NULL);
    if (errorCode) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Load env");
        return NULL;
    }

    errorCode = GRBnewmodel(env, &model, NULL, 0, NULL, NULL, NULL, NULL, NULL);
    if (errorCode) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Create Model");
        return NULL;
    }

    if (!addVariablesToModel(model, boardSize)) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Adding variables to model");
        return NULL;
    }

    if(!addConstraints(model, board)) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Adding constraints");
        return NULL;
    }

    if (GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE) ||
        GRBupdatemodel(model) ||
        GRBoptimize(model) ||
        GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimizationStatus)) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Running Optimization");
        return NULL;
    }

    if(optimizationStatus == SOLVED){
        errorCode = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, boardSize * board->blockRows * board->blockColumns, solutionMatrix);
        if (errorCode) {
            freeResources(env, model, solvedBoard, solutionMatrix);
            *resultCode = ERROR;
            errPrinter_puzzleGurobiFailure("Getting solution");
            return NULL;
        }
    } else{
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = NO_SOLUTION;
        return NULL;
    }

    copySolution(solvedBoard, solutionMatrix);

    GRBfreemodel(model);
    GRBfreeenv(env);
    freeResources(env,model, NULL, solutionMatrix);
    *resultCode = SOLVED;
    return solvedBoard;
}
