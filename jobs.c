#include"jobs.h"
struct process* head = NULL;
struct process* tail=NULL;
pid_t fgchild = 0;
void fg(struct cli_args arg)
{
    //char **args = arg.args;
    if(arg.size!=2)
    {
        printf("Usage: fg <jobnumber>\n");
        return;
    }
    int job_id = atoi(arg.args[1]);
    struct process* temp = head;
    pid_t pid = -1;
    bool mode;
    while(temp!=NULL)
    {
        if(temp->job_id==job_id)
        {
            pid = temp->pid;
            mode = temp->mode;
            break;
        }
        temp = temp->next;
    }
    if(pid!=-1)
    {
        int status, stat;
        if(mode==FOREGROUND)
        {
            kill(pid, SIGCONT);
            change_process_status(pid, STATUS_RUNNING);
            // tcsetpgrp(0, pid);
            signal(SIGINT, sigint_handler);
            struct sigaction s;
            s.sa_flags = SA_SIGINFO | SA_RESTART;
            s.sa_sigaction = HandleSignal;
            sigaction(SIGTSTP, &s, NULL);
            do{    
                status = waitpid(pid, &stat, WUNTRACED);
                if(status<0)
                {
                    perror("waitpid() error");
                    exit(EXIT_FAILURE);
                }
                if(WIFSTOPPED(stat))
                {
                    change_process_status(pid, STATUS_SUSPENDED);
                    printf("signal caught\n");
                }
            }while( !WIFEXITED(stat) && !WIFSIGNALED(stat) && !WIFSTOPPED(stat) );  
            if(!WIFSTOPPED(stat))
                remove_job(pid);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, getpid());
            signal(SIGTTOU, SIG_DFL);
        }
        if(mode==BACKGROUND)
        {
            //printf("%d\n", mode);
            kill(pid, SIGCONT);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, getpgid(pid));
            signal(SIGTTOU, SIG_DFL);
            int x = waitpid(getpgid(pid), &stat, WUNTRACED);
            if(x<0)
            {
                perror("waitpid() error");
                exit(EXIT_FAILURE);
            }
            if(WIFSTOPPED(stat))
            {
                change_process_status(pid, STATUS_SUSPENDED);
            }
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, getpgid(getpid()));
            //tcsetpgrp(0, getpid());
            signal(SIGTTOU, SIG_DFL);
        }
    }
}
void bg(struct cli_args arg)
{
    //char **args = arg.args;
    if(arg.size!=2)
    {
        printf("Usage: bg <jobnumber>\n");
        return;
    }
    int job_id = atoi(arg.args[1]);
    struct process* temp = head;
    pid_t pid = -1;
    while(temp!=NULL)
    {
        if(temp->job_id==job_id)
        {
            temp->mode = STATUS_RUNNING;
            pid = temp->pid;
            break;
        }
        temp = temp->next;
    }
    if(pid!=-1)
    {
        kill(pid, SIGCONT);
        change_process_status(pid, STATUS_RUNNING);
    }
}
void overkill(struct cli_args arg)
{
    struct process* temp = head;
    if(arg.size>1)
    {
        printf("Usage: overkill\n");
        return;
    }
    while(temp!=NULL)
    {
        kill(temp->pid, SIGKILL);
        temp = temp->next;
    }
}
void kjob(struct cli_args arg)
{
    if(arg.size!=3)
    {
        printf("Usage: kjob <jobnumber> <signalnumber>\n");
        return;
    }
    int job_id = atoi(arg.args[1]);
    int signalnumber = atoi(arg.args[2]);
    pid_t pid = -1;
    struct process* temp = head;
    while(temp!=NULL)
    {
        if(temp->job_id==job_id)
        {
            //temp->mode = STATUS_RUNNING;
            pid = temp->pid;
            break;
        }
        temp = temp->next;
    }
    if(pid!=-1)
    {
        kill(pid, signalnumber);
        if( signalnumber==20 )
        {
            printf("Process with pid = %d stopped by signal = %d\n", pid, signalnumber);
            change_process_status(pid, STATUS_SUSPENDED);
        }
        else if(signalnumber==18)
        {
            printf("Process with pid = %d continued\n", pid);
            //change_process_status(pid, STATUS_CONTINUED);
        }
    }
}
void jobs(struct cli_args arg)
{
    struct process* temp = head;
    while(temp!=NULL)
    {    
        int counter = 0;
        printf("[%d] ", temp->job_id);
        if(temp->status==STATUS_RUNNING)
        {
            printf("Running ");
        }
        else if(temp->status==STATUS_SUSPENDED)
        {
             printf("Stopped ");
        }
        while(temp->command[counter]!=NULL)
        {
            printf("%s ", temp->command[counter]);
            counter++;
        }
        printf("[%d]", temp->pid);
        printf("\n");
        temp = temp->next;
    }
}
void reset()
{
    struct process* temp = head;
    if(temp!=NULL)
        temp->job_id = 1;
    while(temp!=NULL)
    {
        if(temp->next!=NULL)
            temp->next->job_id = temp->job_id + 1;
        temp = temp->next;
    }
}
void add_job(pid_t pid, int status, char** name, bool mode)
{
   struct process*  new_proc = (struct process*) malloc(sizeof(struct process));
   new_proc->command = name;
   new_proc->pid = pid;
   new_proc->status = status;
   new_proc->mode = mode;
   new_proc->next = NULL;
   if(head==NULL && tail==NULL)
   {
       head = tail = new_proc;
       new_proc->job_id = 1;
   }
   else
   {
       new_proc->job_id = tail->job_id + 1;
       tail->next = new_proc;
       tail = new_proc;
   }
}
void remove_job(pid_t pid)
{
    struct process* temp = head;
    if(head==tail)
    {
        head = tail = NULL;
    }
    else
    {
        while( temp!=NULL )
        {
            if(temp->pid==pid && temp==head)
            {
                struct process* del = temp;
                head = temp->next;
                del->next = NULL;
                free(del);
            }
            else if(temp->next->pid==pid && temp->next!=tail)
            {
                //printf("%d\n",temp->next->pid);
                struct process* del = temp->next;
                temp->next = temp->next->next;
                del->next = NULL;
                free(del);
                break;
            }
            else if(temp->next->pid==pid)
            {
                struct process* del = temp->next;
                tail = temp;
                temp->next = NULL;
                //del->next = NULL;
                free(del);
                break;
            }
            temp = temp->next;
        }
    }
    reset();
}
void change_process_status(pid_t pid, int status)
{
    struct process* temp = head;
    while(temp!=NULL)
    {    
        if(temp->pid==pid)
        {
            temp->status = status;
            break;
        }
        temp = temp->next;
    }
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
            remove_job(pid);
        }
        else if(WIFSIGNALED(status))
        {
            printf("Process with pid = %d killed by signal = %d\n", pid, WTERMSIG(status));
            remove_job(pid);
        }
        else if(WIFSTOPPED(status))
        {
            printf("Process with pid = %d stopped by signal = %d\n", pid, WSTOPSIG(status));
            //change_process_status(pid, STATUS_SUSPENDED);
        }
        else if(WIFCONTINUED(status))
        {
            printf("Process with pid = %d continued\n", pid);
            //change_process_status(pid, STATUS_CONTINUED);
        }
        printf("\n[%d] done.\n", pid);
    }
}
void sigint_handler(int signum)
{
    if(fgchild)
    {
        int stat = kill(fgchild, SIGINT);
        if(stat)
        {
            perror("Can't interuppt the foreground process");
            return;
        }
    }
}
// void sigstop_handler(int signum)
// {
//     if(fgchild)
//     {
//         int stat = kill(fgchild, SIGTSTP);
//         if(stat)
//         {
//             perror("Can't stop the foreground process");
//             return;
//         }
//     }
//     else
//     {
//         signal(signum, SIG_IGN);
//     }
// }
void HandleSignal(int sig, siginfo_t *si, void *context)
{
    switch (sig)
    {
        case SIGINT: 
            if(fgchild)
            {
                int stat = kill(fgchild, SIGINT);
                if(stat)
                {
                    perror("Can't interuppt the foreground process");
                    return;
                }
            }
            break;
        case SIGTSTP:
            if(fgchild)
            {
                int stat = kill(fgchild, SIGTSTP);
                change_process_status(fgchild, STATUS_SUSPENDED);
                fgchild = 0;
                if(stat)
                {
                    perror("Can't stop the foreground process");
                    return;
                }
            }
            break;
        default:
            break;
    }   
}
void sig_handler(int signum)
{
    siglongjmp(env, 42);
}
void quit(struct cli_args arg)
{
    overkill(arg);
    exit(0);
}