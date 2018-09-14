#ifndef SUDOKU_CONSOLE_COMMANDEXECUTIONS_H
#define SUDOKU_CONSOLE_COMMANDEXECUTIONS_H

#include "Game.h"
#include "Command.h"
#include "FileHandler.h"
#include "ErrorPrinter.h"
#include "SudokuBoard.h"
#include "BTSolver.h"
#include "ILPSolver.h"

void executeSolve(Game* game, Command* cmd);
void executeEdit(Game* game, Command* cmd);
void executeMarkErrors(Game* game, Command* cmd);
void executePrintBoard(Game* game);
void executeSet(Game* game, Command* cmd);
void executeValidate(Game* game);
void executeGenerate(Game* game, Command* cmd);
bool fillBoardWithRandValues(SudokuBoard* board, int valueToFillCount);
void removeValuesFromBoard(SudokuBoard* board, int valueToRemoveCount);
bool isCellSolvable(SudokuBoard* board, int idx);
void executeUndo(Game *game, bool shouldPrint);
void executeRedo(Game* game);
void executeSave(Game* game, Command* cmd);
void executeHint(Game* game, Command* cmd);
void executeNumSolutions(Game* game);
void executeAutofill(Game* game, Command* cmd);
void executeReset(Game* game);
void updateImpossibleValuesForCell(SudokuBoard* board, int index, int *impossibleValues);
void updateImpValuesInRow(int *impossibleValues, int cellRow, int blockRows, int blockColumns, Cell** cells);
void updateImpValuesInCol(int *impossibleValues, int cellCol, int blockRows, int blockColumns, Cell** cells);
void updateImpValuesInBlock(int *impossibleValues, int cellRow, int cellcCol, int blockRows, int blockColumns, Cell** cells);
int getPossValueForCell(int *impossibleValues, int size);
void autoFillValues(LinkedList* valuesToFill, Game* game);
void undoSetCmd(Game* game, bool shouldPrint);
void printUndoStep(int oldValue, int newValue, int column, int row);
void undoAutofillCmd(Game* game, bool shouldPrint);
void destroyNextNodesBeforeAppend(const Game *game);
void printRedoStep(int newValue, int oldValue, int column, int row);
void redoSetCmd(Game* game, Node* nodeToRedo);
Node* getNodeToRedo(Game* game, Node* currentNode);
void redoAutofillCmd(Game* game, Node* nodeToRedo);

#endif
