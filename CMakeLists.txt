cmake_minimum_required(VERSION 3.9.6)
project(sudoku_console C)

set(CMAKE_C_STANDARD 90)

add_executable(sudoku_console
        main.c
        Game.c
        Game.h
        SudokuBoard.c
        SudokuBoard.h
        GameManager.c
        GameManager.h
        CommandManager.c
        CommandManager.h
        Command.c
        Command.h
        ErrorPrinter.c
        ErrorPrinter.h
        CommandValidations.c
        CommandValidations.h
        CommandExecutions.c
        CommandExecutions.h
        LinkedList.c
        LinkedList.h
        FileHandler.c
        FileHandler.h
        Stack.c
        Stack.h
        BTSolver.c
        BTSolver.h ILPSolver.c ILPSolver.h)

target_link_libraries(sudoku_console /Library/gurobi702/mac64/lib/libgurobi70.so)
