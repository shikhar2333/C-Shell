#include"process.h"
#include"builtins.h"
#include"jobs.h"
const char *builtins[] = {"cd", "echo", "pinfo", "ls", "history", "nightswatch", "setenv", "unsetenv"};
const char *job_commands[] = {"fg", "bg", "overkill", "kjob", "jobs", "quit"};

void execute(int in_fd, int out_fd, struct cli_args arg)
{
    void (*builtins_ptr[])(struct cli_args) = {change_dir, echoed, pinfo, list, history, nightswatch, set_env, unset};
    void (*jobs_ptr[])(struct cli_args) = {fg, bg, overkill, kjob, jobs};
    char **args = arg.args;
    for(int i=0; i<sizeof(builtins)/sizeof(char *); i++)
    {
        if( !strcmp(builtins[i],args[0]) )
        {
            if(out_fd!=1)
            {
                int save_stdout = dup(1);
                dup2(out_fd, 1);
                close(out_fd);
                builtins_ptr[i](arg); 
                dup2(save_stdout, 1);
                close(save_stdout);
                return;      
            }
            builtins_ptr[i](arg); 
            return;
        }
    }
    for(int i=0; i<sizeof(job_commands)/sizeof(char *); i++)
    {
        if( !strcmp(job_commands[i],args[0]) )
        {
            jobs_ptr[i](arg);
            return;
        }
    }
    if( !strcmp(args[0],"pwd") )
    {
        char *present_dir = pwd();
        printf("%s\n",present_dir);
        return;
    }
    process(in_fd, out_fd, arg);
}
void process(int in_fd, int out_fd, struct cli_args arg)
{
    pid_t pid_proc;
    pid_proc = fork();
    int stat,status;
    int no_tokens = arg.size;
    char **args = malloc(sizeof(char*)*no_tokens);
    int i;
    for(i=0; i<no_tokens; i++)
    {
        args[i] = strdup(arg.args[i]);
    }
    args[i] = NULL;
    int background = check_background(arg);
    signal(SIGCHLD, sigchld_handler);
    if(!background)
    {
        signal(SIGINT, sigint_handler);
        struct sigaction s;
        s.sa_flags = SA_SIGINFO | SA_RESTART;
        s.sa_sigaction = HandleSignal;
        sigaction(SIGTSTP, &s, NULL);
    }
    // else
    // {
    //     signal(SIGINT, SIG_IGN);
    //     signal(SIGTSTP, SIG_IGN);
    // }
    if(background)
    {
        args[no_tokens-1] = NULL;
    }
    if(pid_proc<0)
    {
        perror("Failed fork");
        return;
    }
    else
    {
        if(pid_proc==0)
        {   
            if(in_fd!=0)
            {
                dup2(in_fd, 0);
                close(in_fd);
            }
            if(out_fd!=1)
            {
                dup2(out_fd, 1);
                close(out_fd);
            }    
            if(background)
            {
                setpgid(0, 0);
                printf("%d\n", getpgid(pid_proc));
            }     
            int check = execvp(args[0], args);
            if(check<0)
            {
                strcat(args[0], " failed");
                perror(args[0]);
            }
            exit(EXIT_SUCCESS);
        }
        else
        {
            if(!background)
            {
                //tcsetpgrp(0, pid_proc);
                fgchild = pid_proc;
                do{    
                    status = waitpid(pid_proc, &stat, WUNTRACED);
                    if(status<0)
                    {
                        perror("waitpid() error");
                        exit(EXIT_FAILURE);
                    }
                    if(WIFSTOPPED(stat))
                    {
                        add_job(pid_proc, STATUS_SUSPENDED, args, FOREGROUND);
                        printf("signal caught\n");
                    }
                }while( !WIFEXITED(stat) && !WIFSIGNALED(stat) && !WIFSTOPPED(stat) );  
                //tcsetpgrp(0, getpid());
                fgchild = 0; 
            }  
            else
            {
                printf("Background process with pid = %d started\n", pid_proc);
                add_job(pid_proc, STATUS_RUNNING, args, BACKGROUND);
            }
                  
        } 
    }
    return;
}
int check_background(struct cli_args arg)
{ 
    char *last = arg.args[arg.size-1];
    char char_last = last[strlen(last)-1];
    if(char_last=='&')
    {
        return 1;
    }
    return 0;
}

