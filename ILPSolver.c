#include <stdlib.h>
#include <assert.h>
#include "stdio.h"
#include "ILPSolver.h"
#include "gurobi_c.h"

#define SOLVED 2

SudokuBoard* ILP_solve(SudokuBoard* board) {
    board = sb_deepCloneBoard(board);
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int error, optimizationStatus;
    int boardSize = board->blockColumns * board->blockColumns * board->blockRows * board->blockRows;
    double *solutionMatrix = calloc(boardSize, sizeof(double));
    assert(solutionMatrix);

    error = GRBloadenv(&env, "sudokuModel.log");
    if (error) {
        return NULL;
    }

    error = GRBnewmodel(env, &model, "sudokuModel", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        return NULL;
    }

    if (!addVariablesToModel(model, boardSize)) {
        return NULL;
    }

    if(!addConstraints(model, board)) {
        return NULL;
    }

    if (GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE) ||
        GRBupdatemodel(model) ||
        GRBoptimize(model) ||
        GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimizationStatus)) {
        return NULL;
    }

    if(optimizationStatus == SOLVED){
        /* Get the solved board */
        error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, boardSize * board->blockRows * board->blockColumns, solutionMatrix);
        if (error) {
            return 0;
        }
    } else{
        freeResources(env,model,val,obj,vtype,lb,ind);
        return -1;
    }
    updateBoard(game, board, solutionMatrix);
    free(ind); free(lb); free(vtype); free(obj); free(val);
    GRBfreemodel(model);
    GRBfreeenv(env);
    /*freeResources(env,model,val,obj,vtype,lb,ind);*/
    return 1;


    return NULL;
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

