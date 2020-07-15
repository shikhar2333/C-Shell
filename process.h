#include"headers.h"
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#define MAX_JOBS 30
void execute(int, int, struct cli_args);
void process(int , int, struct cli_args);
int check_background(struct cli_args);