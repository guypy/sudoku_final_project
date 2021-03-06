#include <stdbool.h>
#include <string.h>
#include "stdio.h"
#include "GameManager.h"

/**
 *
 * GameManager Source File
 *
 * This file contains the implementation of the functions needed to run a game.
 *
 */

/**
 * This function validates the given command is allowed in the current mode of the game.
 * @param game - pointer to current game struct.
 * @param cmd - pointer to current command that had been fetched and is about to be executed.
 * @return - true if command is valid in the current mode of the game, false otherwise
 */
static bool validateMode(Game *game, Command *cmd) {
    if (cmd->validModes[game->mode] == 0)
        return false;
    return true;
}

/**
 * This function handles the validation of a command before it is executed.
 * @param cmd - pointer to current command.
 * @param game - pointer to current game struct.
 * @return - true if validation of command passed, false otherwise
 */
bool validateCommand(Command *cmd, Game* game) {
    if (strcmp(cmd->action, ACTION_SOLVE) == 0) {
        return validateSolve(cmd);
    }
    if (strcmp(cmd->action, ACTION_EDIT) == 0) {
        return validateEdit();
    }
    if (strcmp(cmd->action, ACTION_MARK_ERRORS) == 0) {
        return validateMarkErrors(cmd);
    }
    if (strcmp(cmd->action, ACTION_PRINT_BOARD) == 0) {
        return validatePrintBoard();
    }
    if (strcmp(cmd->action, ACTION_SET) == 0) {
        return validateSet(cmd, game);
    }
    if (strcmp(cmd->action, ACTION_VALIDATE) == 0) {
        return validateValidate();
    }
    if (strcmp(cmd->action, ACTION_GENERATE) == 0) {
        return validateGenerate(cmd, game);
    }
    if (strcmp(cmd->action, ACTION_UNDO) == 0) {
        return validateUndo();
    }
    if (strcmp(cmd->action, ACTION_REDO) == 0) {
        return validateRedo();
    }
    if (strcmp(cmd->action, ACTION_SAVE) == 0) {
        return validateSave(cmd);
    }
    if (strcmp(cmd->action, ACTION_HINT) == 0) {
        return validateHint(cmd, game);
    }
    if (strcmp(cmd->action, ACTION_NUM_SOLUTIONS) == 0) {
        return validateNumSolutions();
    }
    if (strcmp(cmd->action, ACTION_AUTOFILL) == 0) {
        return validateAutofill();
    }
    if (strcmp(cmd->action, ACTION_RESET) == 0) {
        return validateReset();
    }
    if (strcmp(cmd->action, ACTION_EXIT) == 0) {
        return validateExit();
    }
    return false;
}

/**
 * This functions handles the execution of a given command.
 * @param game - pointer to current game struct.
 * @param cmd - pointer to the command that is about to be executed
 */
void executeCommand(Game* game, Command * cmd){
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
        executePrintBoard(game);
    }
    if (strcmp(cmd->action, ACTION_SET) == 0) {
        executeSet(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_VALIDATE) == 0) {
        executeValidate(game);
    }
    if (strcmp(cmd->action, ACTION_GENERATE) == 0) {
        executeGenerate(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_UNDO) == 0) {
        executeUndo(game, true);
    }
    if (strcmp(cmd->action, ACTION_REDO) == 0) {
        executeRedo(game);
    }
    if (strcmp(cmd->action, ACTION_SAVE) == 0) {
        executeSave(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_HINT) == 0) {
        executeHint(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_NUM_SOLUTIONS) == 0) {
        executeNumSolutions(game);
    }
    if (strcmp(cmd->action, ACTION_AUTOFILL) == 0) {
        executeAutofill(game, cmd);
    }
    if (strcmp(cmd->action, ACTION_RESET) == 0) {
        executeReset(game);
    }
}

/**
 * This function handles the flow of the game - fetching a command from the user, validating it can be executed in
 * the given mode, validating the command itself, executing the command, freeing its memory if needed, and doing so
 * repeatedly until the game is stopped.
 * @param game - pointer to current game struct.
 */
void gameLoop(Game* game) {
    Command* cmd;
    while(true) {
        cmd = cmdMngr_fetchCommand();
        if (validateMode(game, cmd) == false) {
            errPrinter_invalidCommand();
            cmd_freeCommand(cmd);
            continue;
        }
        if (validateCommand(cmd, game) == false){
            cmd_freeCommand(cmd);
            continue;
        }
        if (strcmp(cmd->action, ACTION_EXIT) == 0) {
            printf("Exiting...\n");
            cmd_freeCommand(cmd);
            break;
        }
        executeCommand(game, cmd);

        if (strcmp(cmd->action, ACTION_AUTOFILL) != 0 &&
            strcmp(cmd->action, ACTION_SET) != 0 &&
            strcmp(cmd->action, ACTION_GENERATE) != 0) {
            cmd_freeCommand(cmd);
        }
    }
}

/**
 * This function controls the initiation of a game -  its creation, it's execution, and its conclusion
 * @return - 0 if the game had been carried out correctly, 1 in case of unexpected errors.
 */
int startGame() {
    Game* game = createGame();
    printf("Sudoku\n------\n");
    gameLoop(game);
    destroyGame(game);
    return 0;
}
