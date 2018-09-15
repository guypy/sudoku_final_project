#ifndef SUDOKU_CONSOLE_EXECUTIONHELPERS_H
#define SUDOKU_CONSOLE_EXECUTIONHELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Game.h"
#include "Command.h"
#include "CommandManager.h"
#include "FileHandler.h"
#include "ErrorPrinter.h"
#include "SudokuBoard.h"
#include "BTSolver.h"
#include "ILPSolver.h"
#include "CommandExecutions.h"

void updateImpossibleValuesForCell(SudokuBoard* board, int index, int *impossibleValues);
int getPossValueForCell(int *impossibleValues, int size);
void autoFillValues(LinkedList* valuesToFill, Game* game);
void undoSet(Game *game, bool shouldPrint);
void undoAutofill(Game *game, bool shouldPrint);
void undoGenerate(Game *game, bool shouldPrint);
void destroyNextNodesBeforeAppend(const Game *game);
void redoSet(Game *game, Node *nodeToRedo);
Node* getNodeToRedo(Game* game, Node* currentNode);
void redoAutofill(Game *game, Node *nodeToRedo);
void redoGenerate(Game *game, Node *nodeToRedo);
int getNumOfChars(int num);

bool fillBoardWithRandValues(SudokuBoard* board, int valueToFillCount);
void removeValuesFromBoard(SudokuBoard* board, int valueToRemoveCount);
bool isCellSolvable(SudokuBoard* board, int idx);

#endif //SUDOKU_CONSOLE_EXECUTIONHELPERS_H
