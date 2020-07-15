#include"headers.h"
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include <time.h>
#include <curses.h>
#define BUFF_SIZE 1000
#define PROC "/proc/"
#define STAT "/stat"
#define EXE "/exe"
void change_dir(struct cli_args);
void echoed(struct cli_args);
char* pwd();
void pinfo(struct cli_args);
void list(struct cli_args);
void history(struct cli_args);
char *home_helper(char *home, char *rev_home);
void nightswatch(struct cli_args);
void set_env(struct cli_args);
void unset(struct cli_args);