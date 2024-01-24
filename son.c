#include <stdio.h>
#include <string.h>
#include <dirent.h>

int alreadyExists(int argc, char *argv[], char name[])
{
    DIR *currentDir = opendir(".");
    struct dirent *dir;
    while ((dir = readdir(currentDir)) != NULL)
    {
        if (strcmp(name, dir->d_name) == 0)
            return 1;
    }
    return 0;
}

int makeRepo(int argc, char *argv[])
{
    mkdir(".son");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("No inputs!");
        return 1;
    }
    if (strcmp(argv[1], "init") == 0 && argc == 2)
    {
        makeRepo(argc, argv);
        return 0;
    }
}