#include"parse.h"
struct cli_args parse_args(char *line, char *limiter)
{
    struct cli_args args;
    args.args =  malloc(sizeof(char *)*ARGS_BUFF_SIZE);
    char *token = strtok(line, limiter);
    int args_index = 0;
    int args_buff_size = ARGS_BUFF_SIZE;
    //size_t token_used = 0;
    while(token!=NULL)
    {
        args.args[args_index++] = token;
        if(args_index>=ARGS_BUFF_SIZE)
        {
            args_buff_size = (args_buff_size+ARGS_BUFF_SIZE)*sizeof(char *);
            args.args = realloc(args.args, args_buff_size);
        }
        token = strtok(NULL, limiter);
    }
    args.args[args_index] = NULL;
    args.size = args_index;
    return args;
}