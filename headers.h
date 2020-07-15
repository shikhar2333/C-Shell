#ifndef HEADER
#define HEADER
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <setjmp.h>
#include<signal.h>
#include<stdbool.h>
#include <readline/readline.h>
#include<readline/history.h>
struct cli_args
{
    char* full_command;
    char** args;
    int size;
    char* home_dir;
    char* input_path;
    char* output_path;
    int in_fd;
    int out_fd;
};
#define DELIM " \t\r\n\a"
#define NO_COMM 20
#define COLOR_NONE "\033[m"
#define COLOR_RED "\033[1;37;41m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_GREEN "\033[0;32;32m"
#define COLOR_GRAY "\033[1;30m"
#endif
