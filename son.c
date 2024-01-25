#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int repoExists()
{
    char cwd[1000];
    getcwd(cwd, 1000);
    int flag = 0;
    while (1)
    {
        if (strcmp(cwd, "C:\\") == 0)
            return 0;
        if (flag != 0)
        {
            chdir("..");
        }
        getcwd(cwd, 1000);
        DIR *cur = opendir(cwd);
        struct dirent *dirs = readdir(cur);
        while ((dirs = readdir(cur)) != NULL)
        {
            if (strcmp(".son", dirs->d_name) == 0)
                return 1;
        }
        flag++;
    }
}

int validInput(int argc, char *argv[])
{
    if (argc < 2)
        return 0;
    if (strcmp(argv[0], "son") != 0)
        return 0;
    if (argc == 5 && strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0 && (strcmp(argv[3], "user.email") == 0 || strcmp(argv[3], "user.name") == 0))
        return 1;
    if (argc == 4 && strcmp(argv[1], "config") == 0 && (strcmp(argv[2], "user.email") == 0 || strcmp(argv[2], "user.name") == 0))
        return 1;
    if (argc == 2 && strcmp(argv[1], "init") == 0)
        return 1;
    return 0;
}

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
    char cwd[1000];
    getcwd(cwd, 1000);
    if (repoExists() == 0)
    {
        chdir(cwd);
        mkdir(".son");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (validInput(argc, argv) == 0)
    {
        printf("Invalid Input!");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0 && argc == 2)
    {
        makeRepo(argc, argv);
        return 0;
    }
}