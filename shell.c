#include"input.h"
#include"parse.h"
#include"process.h"
//#include <readline/readline.h>
#include"display.h"
#include"history.h"
void shell_loop();
int main(int argc, char **argv)
{
    shell_loop();
    return 0;
}
void shell_loop()
{
    char *line,*display = NULL;
    struct cli_args args, argz;
    char home_directory[1000];
    if( getcwd(home_directory, 1000)==NULL )
    {
         perror("getcwd error");
    }
    init();
    while(1)
    {
        display = shell_display(home_directory);
        //line = readline(display_str);
        line = read_line();
        History(line);
        args = parse_args(line, ";");
        args.home_dir = home_directory;
        for(int i = 0; i<args.size; i++)
        {
            argz = parse_args(args.args[i], DELIM);
            argz.home_dir = home_directory;
            execute(argz);
        }
        free(line);
        free(display);
        free(args.args);
    }
}
