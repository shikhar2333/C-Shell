#include"builtins.h"
#include"directory.h"
#include"history.h"
#include"parse.h"
void change_dir(struct cli_args arg)
{
    char **args = arg.args;
    int is_home = 0;
    char* change_dir = NULL;
    if(args[1]==NULL)
    {
       is_home = 1;
       change_dir = arg.home_dir;
    }
    else
    {
        is_home = !strcmp(args[1], "~");
        if( is_home )
        {
            change_dir = arg.home_dir;
        }
        else
        {
            change_dir = args[1];
        }   
    }
    if(chdir(change_dir))
    {
        perror("chdir error");
        //exit(EXIT_FAILURE);
    }
}
char* pwd()
{
    char *buffer = malloc(sizeof(char)*BUFF_SIZE);
    if( getcwd(buffer, BUFF_SIZE)==NULL )
    {
       perror("getcwd() error");
    }
    return buffer;
}
void echo(struct cli_args arg)
{
    int size = 0;
    char **args = arg.args;
    while(args[++size]!=NULL);
    int i,j;
    for(i=1; i<size; i++)
    {
        if(args[i][0]=='$')
        {
            int len = strlen(args[i]);
            int index = 0;
            char *str = malloc(sizeof(char)*len);
            for(j=1; j<len; j++)
            {
                str[index++] = args[i][j];
            }
            str[index] = '\0';
            char *var = getenv(str);
            if(var!=NULL)
            {
                printf("%s ", var);
            }
            free(str);
        }
        else
        {
            printf("%s ", args[i]);
        }   
    }
    printf("\n");
}
void pinfo(struct cli_args arg)
{
    char pid[20];
    char **args = arg.args;
    if(args[1]==NULL)   //pinfo of shell program
    {
        
        pid_t pid_proc = getpid();
        sprintf(pid, "%d", pid_proc);
    }
    else                //pinfo of any given process
    {
        sprintf(pid, "%s", args[1]);
    }
    char symlink_path[100] = {PROC};
    char dir_path[100] = {PROC};

    /* code for process state and virtual memory usage */
    strcat(dir_path, pid);
    strcat(dir_path, STAT);
    int fd = open(dir_path, O_RDONLY);
    if(fd<0)
    {
        perror("can't open proc file: ");
        return;
    }
    char *proc_info = malloc(sizeof(char)*4*BUFF_SIZE);
    size_t bytes = read(fd, proc_info, BUFF_SIZE);
    if(bytes<0)
    {
        perror("Error in reading proc file: ");
        return;
    }
    struct cli_args process_stat = parse_args(proc_info, DELIM);

    /* code for finding path of the executable */
    strcat(symlink_path, pid);
    strcat(symlink_path, EXE);
    struct stat pinfo;
    if(stat(symlink_path, &pinfo)<0)
    {
        perror(args[0]);
        return;
    }
    size_t buff_size = pinfo.st_size + 1;
    if(!pinfo.st_size)
    {
        buff_size = BUFF_SIZE;
    }
    char *exec_path = malloc(sizeof(char)*buff_size);
    size_t no_bytes = readlink(symlink_path, exec_path, buff_size);
    if(no_bytes<0)
    {
        perror("readlink error()");
        return;
    }
    exec_path[no_bytes] = '\0';
    printf("pid -- %s\n", pid);
    printf("Process Status -- %s\n", process_stat.args[2]);
    printf("Memory -- %s\n", process_stat.args[23]);
    printf("Executable Path -- %s\n", exec_path);
}
void exitshell(struct cli_args arg)
{
    exit(0);
}
void list(struct cli_args arg)
{
    struct dir directory;
    directory.display_files = display_files;
    directory.permissions = permissions;
    directory.long_list = longlist;
    char **args = arg.args;
    int args_1 = (args[1]==NULL) ? 1 : 0;
    int la_flag=0, a_flag=0, l_flag=0;
    int is_dir = 0;
    if(!args_1)
    {
        la_flag = !strcmp(args[1], "-la") | !strcmp(args[1], "-al");
        a_flag = !strcmp(args[1], "-a");
        l_flag = !strcmp(args[1], "-l");
        is_dir = la_flag | a_flag | l_flag;
    }
    //printf("lflag = %d, aflag = %d, laflag = %d isdir = %d args1 = %d\n", l_flag, a_flag, la_flag, is_dir, args_1 );
    if(args_1)
    {
        directory.display_files(&directory, ".", 0);
    }
    else if(!is_dir)
    {
        if( !strcmp(args[1], "~") )
        {
           directory.display_files(&directory, arg.home_dir, 0);
        }
        else
        {
            directory.display_files(&directory, args[1], 0);
        }
    }
    else if(a_flag)                    
    {
        directory.display_files(&directory, pwd(), 1);
    }
    else if(l_flag)
    {
        directory.long_list(&directory, pwd(), 0);
    }
    else if(la_flag)
    {
        directory.long_list(&directory, pwd(), 1);
    }
}
void history(struct cli_args arg)
{
    char** args = arg.args;
    int args_1 = (args[1] == NULL )? 10 : atoi(args[1]);
    args_1 = args_1 < 10 ? args_1 : 10;
    display_comm(args_1);
}