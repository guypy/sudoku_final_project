#include "ErrorPrinter.h"
#include "stdio.h"

void errPrinter_invalidCommand(){
    printf("ERROR: invalid command\n");
}

void errPrinter_erroneousValues(){
    printf("Error: board contains erroneous values\n");
}

void errPrinter_valueNotInRange(int min, int max){
    printf("Error: value not in range %d-%d\n", min, max);
}

void errPrinter_fixedCell(){
    printf("Error: cell is fixed\n");
}

void errPrinter_unsolvableBoard(){
    printf("Error: board is unsolvable\n");
}

void errPrinter_cellContainsValue(){
    printf("Error: cell already contains a value\n");
}

void errPrinter_cannotCreateOrModifyFile(){
    printf("Error: File cannot be created or modified\n");
}

void errPrinter_cannotOpenFile(){
    printf("Error: File cannot be opened\n");
}

void errPrinter_fileDoesNotExist(){
    printf("Error: File doesn’t exist or cannot be opened\n");
}

void errPrinter_boardValidationFailed(){
    printf("Error: board validation failed\n");
}

void errPrinter_noMovesToRedo(){
    printf("Error: no moves to redo\n");
}

void errPrinter_noMovesToUndo(){
    printf("Error: no moves to undo\n");
}

void errPrinter_puzzleGeneratorFailed(){
    printf("Error: puzzle generator failed\n");
}

void errPrinter_boardNotEmpty(){
    printf("Error: board is not empty\n");
}

void errPrinter_valueIsNotZeroOrOne(){
    printf("Error: the value should be 0 or 1\n");
}

void errPrinter_puzzleSolutionErroneous(){
    printf("Puzzle solution erroneous\n");
}

void errPrinter_puzzleGurobiFailure(char* error, int errorCode){
    printf("Puzzle solving failed due to Gurobi failure: %s, %d\n", error, errorCode);
}
