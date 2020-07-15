#include"shell_loop.h"
sigjmp_buf env;
char home_directory[1000];
void shell_loop()
{
    char *line,*display = NULL;
    struct cli_args args, piped_args, argz;
    if( getcwd(home_directory, 1000)==NULL )
    {
         perror("getcwd error");
    }
    init();
    while(1)
    {
        signal(SIGINT, sig_handler);
        signal(SIGTSTP, sig_handler);
        display = shell_display(home_directory);
        size_t buffer_size = INP_BUFF_SIZE;
        line = readline(display);
        if(line==NULL)
            exit(EXIT_FAILURE);
        //line = read_line();
        if(line[0]=='\0')
            continue;
        add_history(line);
        History(line);
        args = parse_args(line, ";");
        args.home_dir = home_directory;
        /* 2 sized file descriptor for piping, fd[0] - reading end, fd[1] - writing end */
        for(int i = 0; i<args.size; i++)
        {
            int in_fd, out_fd, fd[2];
            piped_args = parse_args(args.args[i], "|");
            argz = parse_args(piped_args.args[0], DELIM);
            argz.home_dir = home_directory;
            pipe(fd);
            if(argz.in_fd<0)
            {
                in_fd = STDIN_FILENO;
            }
            else
            {
                in_fd = argz.in_fd;
            }
            if(piped_args.size==1)
            {
                if(argz.out_fd>0)
                {
                    out_fd = argz.out_fd;
                }
                else
                {
                    out_fd = STDOUT_FILENO;
                }
                execute(in_fd, out_fd, argz);
                continue;
            }
            out_fd = fd[1];
            execute(in_fd, out_fd, argz);
            close(fd[1]);
            in_fd = fd[0];
            int pipe_size = piped_args.size;
            for(int j = 1; j<pipe_size; j++)
            {
                pipe(fd);
                out_fd = fd[1];
                argz = parse_args(piped_args.args[j], DELIM);
                argz.home_dir = home_directory;
                if(argz.in_fd>0)
                {
                    in_fd = argz.in_fd;
                }
                if(j==(pipe_size-1))
                {
                    if(argz.in_fd>0)
                    {
                        in_fd = argz.in_fd;
                    }
                    if(argz.out_fd>0)
                    {
                        out_fd = argz.out_fd;
                    }
                    else
                    {
                        out_fd = STDOUT_FILENO;
                    }            
                }
                execute(in_fd, out_fd, argz);
                close(fd[1]);
                in_fd = fd[0];
            }
            close(fd[0]);
            close(fd[1]);
        }
        free(line);
        free(display);
        free(args.args);
        free(piped_args.args);
        free(argz.args);
        if (sigsetjmp(env, 1) == 42) 
        {
           printf("\n");
        }
    }
}