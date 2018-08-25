#include <stdbool.h>
#include <string.h>
#include "stdio.h"
#include <stdbool.h>
#include "GameManager.h"
#include "Game.h"
#include "CommandManager.h"
#include "ErrorPrinter.h"
#include "CommandExecutions.h"
#include "Command.h"
#include "CommandValidations.h"

int startGame() {
    Game* game = createGame();
    printf("Sudoku\n------\n");
    gameLoop(game);
    destroyGame(game);
    return 0;
}

static bool validateMode(Game *game, Command *cmd) {
    if (cmd->validModes[game->mode] == 0)
        return false;
    return true;
}

bool validateCommand(Command* cmd) {
    if (strcmp(cmd->action, ACTION_SOLVE) == 0) {
        return validateSolve(cmd);
    }
    if (strcmp(cmd->action, ACTION_EDIT) == 0) {
        return validateEdit(cmd);
    }
    if (strcmp(cmd->action, ACTION_MARK_ERRORS) == 0) {
        return validateMarkErrors(cmd);
    }
    if (strcmp(cmd->action, ACTION_PRINT_BOARD) == 0) {
        return validatePrintBoard(cmd);
    }
    if (strcmp(cmd->action, ACTION_SET) == 0) {
        return validateSet(cmd);
    }
    if (strcmp(cmd->action, ACTION_VALIDATE) == 0) {
        return validateValidate(cmd);
    }
    if (strcmp(cmd->action, ACTION_GENERATE) == 0) {
        return validateGenerate(cmd);
    }
    if (strcmp(cmd->action, ACTION_UNDO) == 0) {
        return validateUndo(cmd);
    }
    if (strcmp(cmd->action, ACTION_REDO) == 0) {
        return validateRedo(cmd);
    }
    if (strcmp(cmd->action, ACTION_SAVE) == 0) {
        return validateSave(cmd);
    }
    if (strcmp(cmd->action, ACTION_HINT) == 0) {
        return validateHint(cmd);
    }
    if (strcmp(cmd->action, ACTION_NUM_SOLUTIONS) == 0) {
        return validateNumSolutions(cmd);
    }
    if (strcmp(cmd->action, ACTION_AUTOFILL) == 0) {
        return validateAutofill(cmd);
    }
    if (strcmp(cmd->action, ACTION_RESET) == 0) {
        return validateReset(cmd);
    }
    if (strcmp(cmd->action, ACTION_EXIT) == 0) {
        return validateExit(cmd);
    }
    return false;
}

void executeCommand(Game* game, Command * cmd){
    if (!validateMode(game, cmd)) {
        errPrinter_invalidCommand();
        return;
    }
    if (strcmp(cmd->action, ACTION_SOLVE) == 0) {
        executeSolve(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_EDIT) == 0) {
        executeEdit(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_MARK_ERRORS) == 0) {
        executeMarkErrors(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_PRINT_BOARD) == 0) {
        executePrintBoard(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_SET) == 0) {
        executeSet(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_VALIDATE) == 0) {
        executeValidate(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_GENERATE) == 0) {
        executeGenerate(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_UNDO) == 0) {
        executeUndo(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_REDO) == 0) {
        executeRedo(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_SAVE) == 0) {
        executeSave(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_HINT) == 0) {
        executeHint(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_NUM_SOLUTIONS) == 0) {
        executeNumSolutions(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_AUTOFILL) == 0) {
        executeAutofill(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_RESET) == 0) {
        executeReset(game, cmd);
    }
}

void gameLoop(Game* game) {
    Command* cmd;
    while(true) {
        cmd = cmdMngr_fetchCommand();
        if (validateCommand(cmd) == false){
            errPrinter_invalidCommand();
            cmd_freeCommand(cmd);
            continue;
        }
        if (strcmp(cmd->action, ACTION_EXIT) == 0) {
            cmd_freeCommand(cmd);
            break;
        }
        executeCommand(game, cmd);

        if (strcmp(cmd->action, ACTION_AUTOFILL) != 0 &&
            strcmp(cmd->action, ACTION_SET) != 0) {
            cmd_freeCommand(cmd);
        }
    }
}