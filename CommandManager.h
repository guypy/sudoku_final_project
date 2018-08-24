#ifndef SUDOKU_CONSOLE_INPUTMANAGER_H
#define SUDOKU_CONSOLE_INPUTMANAGER_H

#include "Command.h"

#define ACTION_SOLVE "solve"
#define ACTION_MARK_ERRORS "mark_errors"
#define ACTION_PRINT_BOARD "print_board"
#define ACTION_SET "set"
#define ACTION_VALIDATE "validate"
#define ACTION_GENERATE "generate"
#define ACTION_UNDO "undo"
#define ACTION_REDO "redo"
#define ACTION_SAVE "save"
#define ACTION_HINT "hint"
#define ACTION_NUM_SOLUTIONS "num_solutions"
#define ACTION_AUTOFILL "autofill"
#define ACTION_RESET "reset"
#define ACTION_EXIT "exit"

Command* cmdMngr_fetchCommand();

#endif //SUDOKU_CONSOLE_INPUTMANAGER_H



