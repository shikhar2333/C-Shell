#include"jobs.h"
bg_process* front = NULL, *rear = NULL;
pid_t fgchild = 0;
sigjmp_buf env;
void fg(struct cli_args arg)
{
    char **args = arg.args;
    if(args[1]==NULL)
    {
        printf("Usage: fg <jobnumber>\n");
        return;
    }
    int job_no = atoi(args[1]);
}
void add_job(pid_t pid, bool status, char** name)
{
    bg_process* new_proc = (bg_process*)malloc(sizeof(bg_process));
    new_proc->pid = pid;
    new_proc->status = 0;
    new_proc->name = name;
    new_proc->next = NULL;
    if(front==NULL || rear == NULL)
    {
        front = rear = new_proc;
        new_proc->job_id = 1;
    }
    else
    {
        new_proc->job_id = rear->job_id + 1;
        rear->next = new_proc;
        rear = new_proc;
    }  
    //printf("%d\n", new_proc->job_id);
}
void remove_job(int job_id)
{
    bg_process** temp = &front;
    if((*temp)==NULL)
    {
        return;
    }
    else
    {
        while(1)
        {
            if((*temp)!=NULL && (*temp)->job_id==job_id)
            {
                break;
            }
            *temp = (*temp)->next;
        }
        bg_process* del = *temp;
        if(front==rear)
        {
            front = rear = NULL;
        }
        else
        {
            *temp = (*temp)->next;
        }
        free(del);
    }
    
}
void sigchld_handler(int signum)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    if(pid>0)
    {
        bg_process* temp = front;
        while(temp!=NULL && temp->pid!=pid)
        {
            // printf("%d %d\n", pid, temp->pid);
            temp = temp->next;
        }
        if(WIFEXITED(status))
        {
            printf("Process with pid = %d exited normally with exit status = %d\n", pid, WEXITSTATUS(status));
            remove_job(temp->job_id);
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
    if(fgchild)
    {
        int stat = kill(fgchild, SIGINT);
        if(stat)
        {
            perror("Can't interuppt the foreground process");
            return;
        }
    }
    else
    {
        signal(signum, sig_handler);
    }
}
void sigstop_handler(int signum)
{
    if(fgchild)
    {
        int stat = kill(fgchild, SIGTSTP);
        if(stat)
        {
            perror("Can't stop the foreground process");
            return;
        }
    }
    else
    {
        signal(signum, SIG_IGN);
    }
}
void sig_handler(int signum)
{
    siglongjmp(env, 42);
}