#include"headers.h"
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<stdbool.h>
#include"input.h"
#include"parse.h"
#include"process.h"
#include"display.h"
#include"history.h"
void sig_handler(int signum);
void shell_loop();