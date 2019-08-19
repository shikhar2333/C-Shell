#include"directory.h"
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
void display_files(struct dir* directory, char* dir, int flag)
{
    DIR *folder;
    folder = opendir(dir);
    if(folder == NULL)
    {
        perror("Not able to read directory: ");
        return;
    }
    struct dirent *file = readdir(folder);
    while(file!=NULL)
    {
        if(file->d_name[0]=='.')
        {
            if(flag)
            printf("%s\n", file->d_name);
        }
        else if(file->d_name[0]!='.')
        {
            printf("%s\n", file->d_name);
        }
        file = readdir(folder);
    }
    closedir(folder);
}
void longlist(struct dir* directory, char *dir, int flag)
{
    DIR* folder;
    folder = opendir(dir);
    if(folder == NULL)
    {
        perror("Not able to read directory: ");
        return;
    }
    struct dirent *file = readdir(folder);
    while(file!=NULL)
    {
        struct stat file_stat;
        int status = stat(file->d_name, &file_stat);
        char date[101];
        if(status<0)
        {
            perror("stat() error");
            break;
        }
        if(S_ISDIR(file_stat.st_mode))
        {
            printf("%s", "d");
        }
        else
        {
            printf("-");
        }
        if(flag)
        {
            directory->permissions(file_stat.st_mode);
            printf(" %lld ", file_stat.st_nlink);
            struct group  *gr = getgrgid(file_stat.st_gid);
            struct passwd *pw = getpwuid(file_stat.st_uid);
            printf("%s %s %d ", pw->pw_name, gr->gr_name, file_stat.st_size);
            strftime(date, 100, "%h %d %H:%M", localtime(&(file_stat.st_mtime)));
            printf(" %s    ", date);
            printf("%s ", file->d_name);
            printf("\n");
        }
        else
        {
            if(file->d_name[0]!='.')
            {
                directory->permissions(file_stat.st_mode);
                printf(" %lld ", file_stat.st_nlink);
                struct group  *gr = getgrgid(file_stat.st_gid);
                struct passwd *pw = getpwuid(file_stat.st_uid);
                printf("%s %s %d ", pw->pw_name, gr->gr_name, file_stat.st_size);
                strftime(date, 100, "%h %d %H:%M", localtime(&(file_stat.st_mtime)));
                printf(" %s    ", date);
                printf("%s ", file->d_name);
                printf("\n");
            }
        }        
        file = readdir(folder);
    }
    closedir(folder);
}