#include"process.h"
#include"builtins.h"
const char *builtins[] = {"cd", "echo", "pinfo", "exit", "ls", "history", "nightswatch"};
void execute(struct cli_args arg)
{
    void (*builtins_ptr[])(struct cli_args) = {change_dir, echoed, pinfo, exitshell, list, history, nightswatch};
    char **args = arg.args;
    for(int i=0; i<sizeof(builtins)/sizeof(char *); i++)
    {
        if( !strcmp(builtins[i],args[0]) )
        {
            builtins_ptr[i](arg);
            return;
        }
    }
    if( !strcmp(args[0],"pwd") )
    {
        char *present_dir = pwd();
        printf("%s\n",present_dir);
        return;
    }
    process(arg);
}
void sigchld_handler(int signum)
{
  pid_t pid;
  int status;
  pid = waitpid(-1, &status, WNOHANG);
  if(pid>0)
  {
        if(WIFEXITED(status))
        {
            printf("Process with pid = %d exited normally with exit status = %d\n", pid, WEXITSTATUS(status));
        }
        else if(WIFSIGNALED(status))
        {
            printf("Process with pid = %d killed by signal = %d\n", pid, WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
            printf("Process with pid = %d stopped by signal = %d\n", pid, WSTOPSIG(status));
        }
        else if(WIFCONTINUED(status))
        {
            printf("Process with pid = %d continued\n", pid);
        }
        printf("\n[%d] done.\n", pid);
   }
}
void sigint_handler(int signum)
{
    printf("caught signal %d\n", signum);
}
void process(struct cli_args arg)
{
    pid_t pid_proc;
    pid_proc = fork();
    int stat,status;
    char **args = arg.args;
    int no_tokens = arg.size;
    int background = check_background(arg);
    signal(SIGCHLD, sigchld_handler);
    if(background)
    {
        args[no_tokens-1] = NULL;
    }
    //signal(SIGINT, sigint_handler);
    if(pid_proc<0)
    {
        perror("Failed fork");
        return;
    }
    else
    {
        if(pid_proc==0)
        {
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
                do{
                    status = waitpid(pid_proc, &stat, WUNTRACED);
                    if(status<0)
                    {
                        perror("waitpid() error");
                        exit(EXIT_FAILURE);
                    }
                }while( !WIFEXITED(stat) && !WIFSIGNALED(stat));   
            }  
            else
            {
                printf("Background process with pid = %d started\n", pid_proc);
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

