#include"input.h"
#include"parse.h"
#include"process.h"
//#include <readline/readline.h>
#include"display.h"
void shell_loop();
int main(int argc, char **argv)
{
    shell_loop();
    return 0;
}
void shell_loop()
{
    char *line = NULL;
    struct cli_args args, argz;
    char *home_directory = malloc(sizeof(char)*1000);
    if( getcwd(home_directory, 1000)==NULL )
    {
         perror("getcwd error");
    }
    while(1)
    {
        shell_display(home_directory);
        //line = readline(display_str);
        line = read_line();
        args = parse_args(line, ";");
        args.home_dir = home_directory;
        //if(line[0]!='\0')
        //{
        for(int i = 0; i<args.size; i++)
        {
            argz = parse_args(args.args[i], DELIM);
            argz.home_dir = home_directory;
            execute(argz);
        }
        //}
    }
}
