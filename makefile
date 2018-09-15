CC = gcc
OBJS = main.o GameManager.o Game.o CommandManager.o Command.o ErrorPrinter.o CommandExecutions.o CommandValidations.o SudokuBoard.o FileHandler.o BTSolver.o ILPSolver.o Stack.o LinkedList.o
EXEC = sudoku-console
COMP_FLAG = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c GameManager.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
GameManager.o: GameManager.c GameManager.h Game.h CommandManager.h Command.h ErrorPrinter.h CommandExecutions.h CommandValidations.h SudokuBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h SudokuBoard.h LinkedList.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
CommandManager.o: CommandManager.c CommandManager.h Command.h Game.h CommandValidations.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Command.o: Command.c Command.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
ErrorPrinter.o: ErrorPrinter.c ErrorPrinter.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
CommandExecutions.o: CommandExecutions.c CommandExecutions.h Game.h Command.h FileHandler.h ErrorPrinter.h SudokuBoard.h BTSolver.h ILPSolver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
CommandValidations.o: CommandValidations.c CommandValidations.h ErrorPrinter.h Command.h SudokuBoard.h Game.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
SudokuBoard.o: SudokuBoard.c SudokuBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
FileHandler.o: FileHandler.c FileHandler.h SudokuBoard.h ErrorPrinter.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
BTSolver.o: BTSolver.c BTSolver.h Stack.h SudokuBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
ILPSolver.o: ILPSolver.c ILPSolver.h SudokuBoard.h ErrorPrinter.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Stack.o: Stack.c Stack.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
LinkedList.o: LinkedList.c LinkedList.h Command.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
