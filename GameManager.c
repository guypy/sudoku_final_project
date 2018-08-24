#include <stdbool.h>
#include <string.h>
#include "stdio.h"
#include "GameManager.h"
#include "Game.h"
#include "CommandManager.h"
#include "Command.h"
#include "ErrorPrinter.h"
#include "CommandExecutions.h"

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

void executeCommand(Game* game, Command * cmd){
    if (!validateMode(game, cmd)) {
        errPrinter_invalidCommand();
        return;
    }
    if (strcmp(cmd->action, ACTION_SOLVE) == 0) {
        executeSolve(game, cmd);
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
        if (cmd == NULL) {
            errPrinter_invalidCommand();
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