#include"builtins.h"
#include"directory.h"
#include"history.h"
#include"parse.h"
#include"display.h"
void change_dir(struct cli_args arg)
{
    char **args = arg.args;
    int is_home = 0;
    char *change_dir = malloc(100*sizeof(char));
    if(args[1]==NULL)
    {
       is_home = 1;
       change_dir = strdup(arg.home_dir);
    }
    else
    {
        is_home = (args[1][0]=='~') ? 1 : 0;
        if( is_home )
        {
            change_dir = home_helper(arg.home_dir, args[1]);
        }
        else
        {
            change_dir = strdup(args[1]);
        }   
    }
    if(chdir(change_dir))
    {
        perror("chdir error");
    }
    free(change_dir);
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
void echoed(struct cli_args arg)
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
    if(args[1]==NULL)    //pinfo of shell program
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
    directory.permissions = permissions;
    directory.long_list = longlist;
    directory.file_type = file_type;
    char **args = arg.args;
    int args_1 = (args[1]==NULL) ? 1 : 0;
    bool la_flag=0, a_flag=0, l_flag=0;
    bool no_dir = 0;
    int no_dirs = 0;
    if(!args_1)
    {
        for(int i = 1; args[i]!=NULL; i++)
        {
            if( !strcmp(args[i], "-a") )
            {
                a_flag = 1;
                //break;
            }
            else if( !strcmp(args[i], "-l") )
            {
                l_flag = 1;
                //break;
            }
            else if( !strcmp(args[i], "-la") || !strcmp(args[i], "-al"))
            {
                a_flag = 1;
                l_flag = 1;
                //break;
            }
            else
            {
                no_dirs++;
            }   
        }
        no_dir = a_flag | l_flag;
        bool dir_flag = (!no_dir) ? 1 : 0;
        if(no_dirs==0)
        {
            printf("%s\n", pwd());
            directory.long_list(&directory, ".", dir_flag, a_flag, l_flag);
            return;
        }
        //printf("%d %d %d\n", no_dir, l_flag, a_flag);
        for(int j = 1; args[j]!=NULL; j++)
        {
            if( !strcmp(args[j], "-l") || !strcmp(args[j], "-la") || !strcmp(args[j], "-al") || !strcmp(args[j], "-a" ) )
            {
                continue;
            }
            if( args[j][0] == '~' )
            {
                char *buffer = malloc(100*sizeof(char));
                buffer = home_helper(arg.home_dir, args[j]);
                printf("%s\n", buffer);
                directory.long_list(&directory, buffer, dir_flag, a_flag, l_flag);
                free(buffer);
            }
            else
            {
                printf("%s\n", args[j]);
                directory.long_list(&directory, args[j], dir_flag, a_flag, l_flag);
            }
        }
    }
    else if(args_1)
    {
        //printf("%d %d %d\n", args_1, l_flag, a_flag);
        directory.long_list(&directory, ".", args_1, a_flag, l_flag);
    }
}
void history(struct cli_args arg)
{
    char** args = arg.args;
    int args_1 = (args[1] == NULL )? 10 : atoi(args[1]);
    args_1 = args_1 < 10 ? args_1 : 10;
    display_comm(args_1);
}
char *home_helper(char *home, char *rev_home)
{
    char *home_abspath = malloc(100*sizeof(char));
    strcpy(home_abspath, home);
    int index = strlen(home_abspath);
    int len = strlen(rev_home);
    for(int i=1; i<len; i++)
    {
        home_abspath[index++] = rev_home[i];
    }
    return home_abspath;
}
void nightswatch(struct cli_args arg)
{
    char **args = arg.args;
    int interval, interupt, dirty;
    if(args[1]!=NULL)
    {
        if(args[2]!=NULL)
            interval = atoi(args[2]);
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
    WINDOW* curr = initscr();
	WINDOW * window; 
	window = newwin(600,700,1,1);
	keypad(window, TRUE);
	noecho();
	curs_set(0);
	nodelay(window,1);
    int start_time = time(NULL), now = time(NULL),prev = time(NULL);
    int down = 4;
    char* cpu = NULL;
    FILE* fp = fopen("/proc/interrupts", "r");
    if(fp==NULL)
    {
        exit(EXIT_FAILURE);
    }
    size_t size = 100;
    getline(&cpu, &size, fp);
    fclose(fp);
    if(args[3]!=NULL)
    {
        interupt = !strcmp(args[3], "interrupt");
        dirty = !strcmp(args[3], "dirty");
    }
    else
    {
        return;
    }
    while(1)
    {
        now = time(NULL);
        if(wgetch(window) == 'q')
        {
            wclear(window);
            break;
        }
        int val = (now - start_time)%interval;
        if(interupt )
        {
            if( !val )
            {
                if( now!=prev )
                {
                    mvwaddstr(window,2,10,cpu);
                    FILE *fp = fopen("/proc/interrupts", "r");
                    if (fp == NULL)
                    {
                        exit(EXIT_FAILURE);
                    }
                    char* line = NULL;
                    size_t len = 0;
                    size_t read;
                    int counter = 0;
                    prev = now;
                    while ((read = getline(&line, &len, fp)) != -1) 
                    {
                        counter++;
                        if(counter==3)
                            break;
                    }
                    mvwaddstr(window, down, 11, line);
                    //printf("%s\n", line);
                    down = down+1;
                    fclose(fp);
                }
            }
        }
        else if(dirty)
        {
            if( !val )
            {
                if(now!=prev)
                {
                    FILE *fp = fopen("/proc/meminfo", "r");
                    if (fp == NULL)
                    {
                        exit(EXIT_FAILURE);
                    }
                    char* line = NULL;
                    size_t len = 0;
                    size_t read;
                    int counter = 0;
                    prev = now;
                    while ((read = getline(&line, &len, fp)) != -1) 
                    {
                        counter++;
                        if(counter==17)
                            break;
                    }
                    mvwaddstr(window, down, 11, line);
                    // printf("%s\n", line);
                    down=down+1;
                    fclose(fp);
                }
            }
        }
        else
        {
            printf("Usage: nightswatch -n <time_interval> <interrupt/dirty>\n");
            break;
        }       
        if(down>35)
        {
            wclear(window);
            down = 4;
        }
    }
    noecho();
	curs_set(1);
	delwin(window);
	endwin();
}