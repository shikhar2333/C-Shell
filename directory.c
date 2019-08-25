#include"directory.h"
void file_type(mode_t mode)
{
    switch (mode & S_IFMT) 
    {
        case S_IFBLK:  printf("b");      break;
        case S_IFCHR:  printf("c");      break;
        case S_IFDIR:  printf("d");      break;
        case S_IFIFO:  printf("|");      break;
        case S_IFLNK:  printf("l");      break;
        case S_IFREG:  printf("-");      break;
        case S_IFSOCK: printf("s");      break;
        default:       printf("?");      break;
    }
}
void permissions(mode_t mode)
{
    char *c;
    c = (mode & S_IRUSR) ? "r" : "-";
    printf("%s", c);
     c = (mode & S_IWUSR) ? "w" : "-";
    printf("%s", c);
     c = (mode & S_IXUSR) ? "x" : "-";
    printf("%s", c);

    c = (mode & S_IRGRP) ? "r" : "-";
    printf("%s", c);
    c = (mode & S_IWGRP) ? "w" : "-";
    printf("%s", c);
    c = (mode & S_IXGRP) ? "x" : "-";
    printf("%s", c);

    c = (mode & S_IROTH) ? "r" : "-";
    printf("%s", c);
    c = (mode & S_IWOTH) ? "w" : "-";
    printf("%s", c);
    c = (mode & S_IXOTH) ? "x" : "-";
    printf("%s", c);
}
void longlist(struct dir* directory, char *dir, bool dir_flag, bool flag_a, bool flag_l)
{
    DIR* folder;
    struct group* group;
    struct passwd* user;
    
    folder = opendir(dir);
    if(folder == NULL)
    {
        perror("Not able to read directory: ");
        return;
    }
    struct dirent *file;
    struct stat file_stat;
    while((file = readdir(folder)) != NULL)
    {
        if( (flag_a && !flag_l) || dir_flag)        // -a flag or ls without any arguements
        {
            if( dir_flag && file->d_name[0]=='.' )
            {
                continue;
            }
            printf("%s\n", file->d_name);
            continue;
        }
        else if( flag_l || flag_l )
        {
            if( (!flag_a && flag_l) && file->d_name[0]=='.' )
            {
                continue;
            }
            struct stat file_stat;
            char buffer[100];
            sprintf(buffer, "%s/%s", dir, file->d_name);
            int status = stat(buffer, &file_stat);
            char time[61];
            if(status<0)
            {
                perror("stat() error");
                closedir(folder);
                break;
            }     
            directory->file_type(file_stat.st_mode);
            directory->permissions(file_stat.st_mode);
            printf(" %3lld ", file_stat.st_nlink);
            group = getgrgid(file_stat.st_gid);
            user = getpwuid(file_stat.st_uid);
            printf("%15s %15s %10d ", user->pw_name, group->gr_name, file_stat.st_size);
            strftime(time, 60, "%b %d %H:%M", localtime(&(file_stat.st_mtime)));
            printf(" %20s    %s\n", time, file->d_name);
        }
    }
    closedir(folder);
}