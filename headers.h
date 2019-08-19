#ifndef HEADER
#define HEADER
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
struct cli_args
{
    char** args;
    int size;
    char* home_dir;
};
#define DELIM " \t\r\n\a"
#endif
