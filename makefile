LDFLAGS=-lncurses
shell1: history.c input.c builtins.c directory.c display.c parse.c  process.c shell.c
	gcc -o shell1 history.c input.c builtins.c directory.c display.c parse.c process.c shell.c -lncurses 
