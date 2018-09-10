#ifndef SUDOKU_CONSOLE_COMMANDEXECUTIONS_H
#define SUDOKU_CONSOLE_COMMANDEXECUTIONS_H

#include "Command.h"
#include "Game.h"

void executeSolve(Game* game, Command* cmd);
void executeEdit(Game* game, Command* cmd);
void executeMarkErrors(Game* game, Command* cmd);
void executePrintBoard(Game* game, Command* cmd);
void executeSet(Game* game, Command* cmd);
void executeValidate(Game* game, Command* cmd);
void executeGenerate(Game* game, Command* cmd);
void executeUndo(Game* game, Command* cmd);
void executeRedo(Game* game, Command* cmd);
void executeSave(Game* game, Command* cmd);
void executeHint(Game* game, Command* cmd);
void executeNumSolutions(Game* game, Command* cmd);
void executeAutofill(Game* game, Command* cmd);
void executeReset(Game* game, Command* cmd);
void updateImpossibleValuesForCell(Game *game, Cell *cell, int index, int *impossibleValues);
void updateImpValuesInRow(int *impossibleValues, int cellRow, int blockRows, int blockColumns, Cell** cells);
void updateImpValuesInCol(int *impossibleValues, int cellCol, int blockRows, int blockColumns, Cell** cells);
void updateImpValuesInBlock(int *impossibleValues, int cellRow, int cellcCol, int blockRows, int blockColumns, Cell** cells);
int getPossValueForCell(int *impossibleValues, int size);
void autoFillValues(LinkedList* valuesToFill, Game* game);

#endif //SUDOKU_CONSOLE_COMMANDEXECUTIONS_H
