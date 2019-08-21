#include"history.h"
char *hist[NO_COMM];
int indices = 0;
void init()
{
    for(int i=0; i<NO_COMM; i++)
    {
        hist[i] = NULL;
    }
}
void History(char *arg)
{
    if( indices>0 )
    {
        //printf("%s %d\n",hist[indices-1], indices);
        if( strcmp(hist[indices-1], arg) )
        {
            hist[indices] = strdup(arg);    
            indices = (indices + 1)%NO_COMM;
        }
    }
    else
    {
        hist[indices] = strdup(arg);
        indices = (indices + 1)%NO_COMM;
    }
    //printf("%d\n", indices);
}
void display_comm(int output_no)
{
    int his_no = 1;
    int no_elements = 0;
    for(int i=0; i<NO_COMM; i++)
    {
        if(hist[i])
            no_elements++;
    }
    int no_prints = (no_elements<output_no) ? no_elements : output_no;  
    int start;
    //printf("%d\n", indices);
    start = ( indices + NO_COMM - no_prints  )%NO_COMM;
    while(1)
    {
        if(hist[start]!=NULL)
        {
            printf("%d : %s\n", his_no, hist[start]);
            his_no++;
        }
        start = (start + 1)%NO_COMM;
        if(his_no>no_prints)
            break;
    }
}
