#include"process.h"
#include"builtins.h"
const char *builtins[] = {"cd", "echo", "pinfo", "exit", "ls"};
void execute(struct cli_args arg)
{
    void (*builtins_ptr[])(struct cli_args) = {change_dir, echo, pinfo, exitshell, list};
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
  if(pid!=-1)
  {
        if(WIFEXITED(status))
        {
            printf("extied status = %d and pid = %d\n", WEXITSTATUS(status), pid);
        }
        else if(WIFSIGNALED(status))
        {
            printf("killed by signal = %d and pid = %d\n", WTERMSIG(status), pid);
        }
        else if(WIFSTOPPED(status))
        {
            printf("stopped by signal = %d and pid = %d\n", WSTOPSIG(status), pid);
        }
        else if(WIFCONTINUED(status))
        {
            printf("continued pid = %d\n", pid);
        }
        printf("\n[%d] done.\n", pid);
   }
}
void sigint_handler(int signum)
{
    // pid_t pid;
    // int status;
    // pid = waitpid(-1, &status, WNOHANG);
    // printf("%d\n", pid);
    // if(pid==0)
    // {
    //     int stat = kill(pid, SIGTERM);
    //     if(!stat)
    //     {
    //         printf("killed process with pid = %d\n", pid);
    //     }
    // }
    printf("caught signal %d\n", signum);
}
void process(struct cli_args arg)
{
    pid_t pid_proc;
    pid_proc = fork();
    int stat;
    char **args = arg.args;
    int no_tokens = arg.size;
    int background = check_background(arg);
    signal(SIGCHLD, sigchld_handler);
    //signal(SIGINT, sigint_handler);
    if(pid_proc<0)
    {
        perror("Failed fork");
    }
    else
    {
        if(pid_proc==0)
        {
            signal(SIGINT, SIG_IGN);
            if(background)
            {
                args[no_tokens-1] = NULL;
            }
            if(execvp(args[0], args)<0)
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
                int status = waitpid(pid_proc, &stat, WUNTRACED);
                if(status<0)
                {
                    perror("waitpid error");
                }
            }           
        } 
    }
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

