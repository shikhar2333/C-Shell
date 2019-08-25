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
#include <sys/sysmacros.h>
#include <stdbool.h> 
#define NO_FILES 1000
struct dir 
{
    void (*permissions)(mode_t);
    //void (*display_files)(struct dir *, char *, int);
    void (*long_list)(struct dir *, char *, bool, bool, bool);
    void (*file_type)(mode_t);
};;
void permissions(mode_t);
//void display_files(struct dir *, char*, int);
void longlist(struct dir *, char *, bool, bool, bool);
void file_type(mode_t);