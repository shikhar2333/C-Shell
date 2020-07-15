#include"headers.h"
#include"shell_loop.h"
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<stdbool.h>
#define NR_JOBS 20
#define STATUS_RUNNING 0
#define STATUS_DONE 1
#define STATUS_SUSPENDED 2
#define STATUS_CONTINUED 3
#define STATUS_TERMINATED 4
#define FOREGROUND 0
#define BACKGROUND 1

void bg(struct cli_args);
void fg(struct cli_args);
void overkill(struct cli_args);
void kjob(struct cli_args);
void jobs(struct cli_args);
void exitshell(struct cli_args);
extern pid_t fgchild;
extern char home_directory[1000];
extern sigjmp_buf env;
void add_job(pid_t, int , char**, bool); 
void remove_job(pid_t);
void change_process_status(pid_t, int);
void sigchld_handler(int signum);
void sig_printnewline(int signum);
void sigint_handler(int signum);
void sigstop_handler(int signum); 
void sig_handler(int signum);
void HandleSignal(int, siginfo_t *, void *);

struct process{
    char **command;
    pid_t pid;
    int status;
    struct process *next;
    int job_id;
    bool mode;
};
