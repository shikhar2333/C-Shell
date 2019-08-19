#include"input.h"
char *read_line()
{
    int ch;
    char *line = malloc(sizeof(char)*INP_BUFF_SIZE);
    ch = getc(stdin);
    int line_index = 0;
    int inp_buff_size = INP_BUFF_SIZE;
    while(ch!=EOF && ch!=10)
    {
        line[line_index++] = ch;
        ch = getc(stdin);
        if(line_index>=inp_buff_size)
        {
            inp_buff_size+=INP_BUFF_SIZE;
            line = realloc(line, inp_buff_size);
        } 
    }
    line[line_index] = '\0';
    return line;
    // char *line = malloc(sizeof(char)*INP_BUFF_SIZE);
    // size_t buffer_size = INP_BUFF_SIZE;
    // getline(&line, &buffer_size, stdin);
    // line[strlen(line)-1] = '\0';
    //return line;
}