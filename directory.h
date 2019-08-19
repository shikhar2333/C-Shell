#include"headers.h"
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#define NO_FILES 1000
struct dir 
{
    void (*permissions)(mode_t);
    void (*display_files)(struct dir *, char *, int);
    void (*long_list)(struct dir *, char *, int);
    //void (*)
};;
void permissions(mode_t);
void display_files(struct dir *, char*, int);
void longlist(struct dir *, char *, int);