#include"headers.h"
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#define BUFF_SIZE 1000
void change_dir(struct cli_args);
void echo(struct cli_args);
char* pwd();
void pinfo(struct cli_args);
void exitshell(struct cli_args);
void list(struct cli_args);