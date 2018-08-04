//
// Created by Guy Pinchuk on 04/08/2018.
//

#include <stdbool.h>
#include "stdio.h"
#include "GameManager.h"
#include "Game.h"
#include "CommandManager.h"
#include "Command.h"
#include "ErrorPrinter.h"

int startGame() {
    Game* game = createGame();
    printf("Sudoku\n------\n");
    gameLoop(game);
}

static bool validateMode(Game *game, Command *cmd) {
    if (cmd->validModes[game->mode] == 0)
        return false;
}

void executeCommand(Game* game, Command * cmd){
    if (!validateMode(game, cmd)) {
        errPrinter_invalidCommand();
        return;
    }
}

void gameLoop(Game* game) {
    Command* cmd;
    while(1){
        cmd = cmdMngr_fetchCommand();
        if (cmd->action == EXIT) {
            cmd_freeCommand(cmd);
            break;
        }
        executeCommand(game, cmd);
        cmd_freeCommand(cmd);
    }

}