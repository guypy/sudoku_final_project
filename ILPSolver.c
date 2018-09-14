#include <stdlib.h>
#include <assert.h>
#include "stdio.h"
#include "ILPSolver.h"
#include "SudokuBoard.h"
#include "BTSolver.h"

#define pow3(x) x*x*x

int createModel(GRBenv* env, GRBmodel** model, SudokuBoard* board){
    int dim = board->blockColumns * board->blockRows;
    char *vtype = (char*)calloc((size_t) pow3(dim), sizeof(char));
    double *lb = (double*)calloc((size_t) pow3(dim), sizeof(double));
    int row, column, value;

    for (row = 0; row < dim; row++) {
        for (column = 0; column < dim; column++) {
            for (value = 1; value <= dim; value++) {
                if (board->cells[row * (dim) + column]->value == value){
                    lb[row*dim*dim + column*dim +value - 1] = 1;
                }
                else{
                    lb[row*dim*dim + column*dim + value - 1] = 0;
                }
                vtype[row*dim*dim + column*dim + value-1] = GRB_BINARY;
            }
        }
    }
    return GRBnewmodel(env, model, NULL, pow3(dim), NULL, lb, NULL, vtype, NULL);
}

bool addNoEmptyCellConstraint(GRBmodel *model, int blockRows, int blockColumns) {
    int i, j, k, errorCode;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(indexes);
    assert(values);

    for (i = 0; i < blockRows * blockColumns; i++) {
        for (j = 0; j < blockRows * blockColumns; j++) {
            for (k = 0; k < blockRows * blockColumns; k++) {
                indexes[k] = i * blockRows * blockColumns * blockRows * blockColumns + j * blockRows * blockColumns + k;
                values[k] = 1.0;
            }
            errorCode = GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL);
            if(errorCode) {
                errPrinter_puzzleGurobiFailure("No Empty Cell Constraint", errorCode);
                free(indexes);
                free(values);
                return false;
            }
        }
    }
    return true;
}

bool addRowConstraint(GRBmodel *model, int blockRows, int blockColumns){
    int i, j, k, errorCode;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(indexes);
    assert(values);

    for (k = 0; k < blockRows * blockColumns; k++) {
        for (j = 0; j < blockRows * blockColumns; j++) {
            for (i = 0; i < blockRows * blockColumns; i++) {
                indexes[i] = i * blockRows * blockColumns * blockRows * blockColumns + j * blockRows * blockColumns + k;
                values[i] = 1.0;
            }
            errorCode = GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL);
            if(errorCode) {
                errPrinter_puzzleGurobiFailure("Row Constraint", errorCode);
                free(indexes);
                free(values);
                return false;
            }
        }
    }
    return true;
}

int addColumnConstraint(GRBmodel *model, int blockRows, int blockColumns){
    int i, j, k, errorCode;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(indexes);
    assert(values);

    for (k = 0; k < blockRows * blockColumns; k++) {
        for (i = 0; i < blockRows * blockColumns; i++) {
            for (j = 0; j < blockRows * blockColumns; j++) {
                indexes[j] = i * blockRows * blockColumns * blockRows * blockColumns + j * blockRows * blockColumns + k;
                values[j] = 1.0;
            }
            errorCode = GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL);
            if(errorCode) {
                errPrinter_puzzleGurobiFailure("Column Constraint", errorCode);
                free(indexes);
                free(values);
                return false;
            }
        }
    }
    return true;
}

bool addBlockConstraint(GRBmodel *model, int blockRows, int blockColumns){
    int m, n, i, j, k, count, errorCode;
    int* indexes = calloc((size_t) blockRows * blockColumns, sizeof(int));
    double* values = calloc((size_t) blockRows * blockColumns, sizeof(double));
    assert(indexes);
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
                errorCode = GRBaddconstr(model, blockRows * blockColumns, indexes, values, GRB_EQUAL, 1.0, NULL);
                if(errorCode) {
                    errPrinter_puzzleGurobiFailure("Block Constraint", errorCode);
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

void freeResources(GRBenv* env, GRBmodel* model, SudokuBoard* board, double* solutionMatrix) {
    if (solutionMatrix != NULL) free(solutionMatrix);
    if (model != NULL)           GRBfreemodel(model);
    if (env != NULL)             GRBfreeenv(env);
    if (board != NULL)           sb_destroyBoard(board);
}

SudokuBoard* ILP_solve(SudokuBoard* board, int* resultCode) {
    SudokuBoard* solvedBoard = sb_deepCloneBoard(board);
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int errorCode, optimizationStatus;
    int boardSize = board->blockColumns * board->blockColumns * board->blockRows * board->blockRows;
    double *solutionMatrix = calloc((size_t) pow3(board->blockRows * board->blockColumns), sizeof(double));
    assert(solutionMatrix);

    errorCode = GRBloadenv(&env, NULL);
    if (errorCode) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Load env", errorCode);
        return NULL;
    }

    GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);

    errorCode = createModel(env, &model, board);
    if (errorCode) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Create Model", errorCode);
        return NULL;
    }

    if(!addConstraints(model, board)) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Adding constraints", errorCode);
        return NULL;
    }

    if (GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE) ||
        GRBupdatemodel(model) ||
        GRBoptimize(model) ||
        GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimizationStatus)) {
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = ERROR;
        errPrinter_puzzleGurobiFailure("Running Optimization", errorCode);
        return NULL;
    }

    if(optimizationStatus == SOLVED){
        errorCode = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, boardSize * board->blockRows * board->blockColumns, solutionMatrix);
        if (errorCode) {
            freeResources(env, model, solvedBoard, solutionMatrix);
            *resultCode = ERROR;
            errPrinter_puzzleGurobiFailure("Getting solution", errorCode);
            return NULL;
        }
    } else{
        freeResources(env, model, solvedBoard, solutionMatrix);
        *resultCode = NO_SOLUTION;
        return NULL;
    }

    copySolution(solvedBoard, solutionMatrix);

    freeResources(env, model, NULL, solutionMatrix);
    *resultCode = SOLVED;
    return solvedBoard;
}
