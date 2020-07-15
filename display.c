#include"display.h"
char* shell_display(char *home_dir)
{
    char *username = malloc(sizeof(char)*DISPLAY_SIZE);
    char *sysname = malloc(sizeof(char)*DISPLAY_SIZE);
    int stat_uname = getlogin_r(username, DISPLAY_SIZE);
    int stat_sysname = gethostname(sysname, DISPLAY_SIZE);
    if(stat_uname || stat_sysname<0)
    {
        perror("display error: ");
        exit(EXIT_FAILURE);
    }
    char *working_dir = malloc(sizeof(char)*DISPLAY_SIZE);
    if( getcwd(working_dir, DISPLAY_SIZE)==NULL )
    {
        perror("getcwd error");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    int s1 = strlen(working_dir), s2 = strlen(home_dir);
    while(index<s2 && working_dir[index]==home_dir[index])
    {
        index++;
    }
    char *display_dir;
    //printf("%d %d\n", index, strlen(home_dir));
    //index--;
    //printf("%d %d\n", index, strlen(home_dir));
    if(index==s2)
    {
        int size = s1 - s2 + 1 ;
        //printf("%d\n", size);
        display_dir = malloc( sizeof(char)*(size+1) );
        int i;
        int j = 0;
        display_dir[j++] = '~';
        for( i = index; i<s1 && j<size; i++ )
        {
            display_dir[j++] = working_dir[i];
        }
        display_dir[size] = '\0';
    }
    else
    {
        strcpy(display_dir, working_dir);
    }
    char *ans = malloc(sizeof(char)*DISPLAY_SIZE);
    sprintf(ans, COLOR_CYAN "<%s@%s:%s>" COLOR_NONE, username, sysname, display_dir);
    return ans;
}