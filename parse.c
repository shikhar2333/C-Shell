#include"parse.h"
struct cli_args parse_args(char *line, char *limiter)
{
    struct cli_args args;
    args.args =  malloc(sizeof(char *)*ARGS_BUFF_SIZE);
    char *token = strtok(line, limiter);
    int args_index = 0;
    int args_buff_size = ARGS_BUFF_SIZE;
    args.in_fd = -1;
    args.out_fd = -1;
    while(token!=NULL)
    {
        if( !strcmp(token,"<") )
        {
            token = strtok(NULL, limiter);
            args.input_path = strdup(token);
            args.in_fd = open(args.input_path, O_RDONLY);
            if(args.in_fd<0)
            {
                printf("Can't open the input file given for redirection\n");
            }
        }
        else if( !strcmp(token, ">") )
        {
            token = strtok(NULL, limiter);
            args.output_path = strdup(token);
            args.out_fd = open(args.output_path, O_WRONLY| O_CREAT | O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);  
            if(args.out_fd<0)
            {
                printf("Can't open the output file for redirection\n");
            }
        }
        else if( !strcmp(token, ">>") )
        {
            token = strtok(NULL, limiter);
            args.output_path = strdup(token);
            args.out_fd = open(args.output_path, O_APPEND| O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        }
        else
        {
            args.args[args_index++] = token;
            if(args_index>=ARGS_BUFF_SIZE)
            {
                args_buff_size = (args_buff_size+ARGS_BUFF_SIZE)*sizeof(char *);
                args.args = realloc(args.args, args_buff_size);
            }
        }
        token = strtok(NULL, limiter);
    }
    args.args[args_index] = NULL;
    args.size = args_index;
    return args;
}