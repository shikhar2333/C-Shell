LDFLAGS=-lncurses
shell: history.c input.c builtins.c directory.c display.c parse.c  process.c shell.c jobs.c shell_loop.c
	gcc -o shell history.c input.c builtins.c directory.c display.c parse.c process.c shell.c jobs.c shell_loop.c -lncurses -lreadline
