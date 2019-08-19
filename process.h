#include"headers.h"
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
void execute(struct cli_args);
void process(struct cli_args);
int check_background(struct cli_args);