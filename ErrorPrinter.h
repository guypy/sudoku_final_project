#ifndef SUDOKU_CONSOLE_ERRORPRINTER_H
#define SUDOKU_CONSOLE_ERRORPRINTER_H

/**
 * ErrorPrinter Header File
 *
 * This interface contains the declaration of functions that print various errors throughout the game.
 */

void errPrinter_invalidCommand();
void errPrinter_erroneousValues();
void errPrinter_valueNotInRange(int min, int max);
void errPrinter_fixedCell();
void errPrinter_unsolvableBoard();
void errPrinter_cellContainsValue();
void errPrinter_cannotCreateOrModifyFile();
void errPrinter_cannotOpenFile();
void errPrinter_fileDoesNotExist();
void errPrinter_boardValidationFailed();
void errPrinter_noMovesToRedo();
void errPrinter_noMovesToUndo();
void errPrinter_puzzleGeneratorFailed();
void errPrinter_boardNotEmpty();
void errPrinter_valueIsNotZeroOrOne();
void errPrinter_puzzleSolutionErroneous();
void errPrinter_puzzleGurobiFailure(char* error, int errorCode);

#endif
