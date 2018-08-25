cmake_minimum_required(VERSION 3.10)
project(sudoku_console C)

set(CMAKE_C_STANDARD 90)

add_executable(sudoku_console main.c Game.c Game.h SudokuBoard.c SudokuBoard.h GameManager.c GameManager.h CommandManager.c CommandManager.h Command.c Command.h ErrorPrinter.c ErrorPrinter.h CommandValidations.c CommandValidations.h CommandExecutions.c CommandExecutions.h LinkedList.c LinkedList.h FileHandler.c FileHandler.h)