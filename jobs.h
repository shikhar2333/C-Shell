#include"headers.h"
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<stdbool.h>
void bg(struct cli_args);
void fg(struct cli_args);
extern pid_t fgchild;
extern sigjmp_buf env;
void add_job(pid_t, bool , char**); 
void sigchld_handler(int signum);
void sigint_handler(int signum);
void sigstop_handler(int signum); 
void sig_handler(int signum);
typedef struct bg_process
{
    pid_t pid;
    bool status;
    char **name;
    int job_id;
    struct bg_process* next;
}bg_process;