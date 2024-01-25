#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int repoExists(char *address)
{
    getcwd(address, 1000);
    char temp[1000];
    getcwd(temp, 1000);
    int flag = 0;
    while (1)
    {
        if (strcmp(address, "C:\\") == 0)
        {
            address = NULL;
            chdir(temp);
            return 1;
        }
        if (flag != 0)
        {
            chdir("..");
        }
        getcwd(address, 1000);
        DIR *cur = opendir(address);
        struct dirent *dirs = readdir(cur);
        while ((dirs = readdir(cur)) != NULL)
        {
            if (strcmp(".son", dirs->d_name) == 0)
            {
                chdir(temp);
                return 0;
            }
        }
        flag++;
    }
}

int userSettings(int argc, char *argv[])
{
    if (strcmp(argv[2], "-global") == 0)
    {
        if (strcmp(argv[3], "user.name") == 0)
        {
            FILE *name = fopen("C:\\son\\name.txt", "w");
            fprintf(name, "%s", argv[4]);
            fclose(name);
        }
        if (strcmp(argv[3], "user.email") == 0)
        {
            FILE *email = fopen("C:\\son\\email.txt", "w");
            fprintf(email, "%s", argv[4]);
            fclose(email);
        }
    }
    else
    {
        char *temp = (char *)malloc(1000);
        repoExists(temp);
        printf("%s\n", temp);
        if (strcmp(argv[2], "user.name") == 0)
        {
            strcpy(temp + strlen(temp), "\\.son\\name.txt");
            printf("%s", temp);
            FILE *name = fopen(temp, "w");
            fprintf(name, "%s", argv[3]);
            fclose(name);
        }
        if (strcmp(argv[2], "user.email") == 0)
        {
            strcpy(temp + strlen(temp), "\\.son\\email.txt");
            printf("%s", temp);
            FILE *email = fopen(temp, "w");
            fprintf(email, "%s", argv[3]);
            fclose(email);
        }
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
    char *address = (char *)malloc(1000);
    if (repoExists(address) == 1)
    {
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

    char *temp_check = (char *)malloc(1000);
    if (repoExists(temp_check) == 1)
    {
        printf("No repo found!");
        return 1;
    }

    if (strcmp(argv[1], "config") == 0)
    {
        userSettings(argc, argv);
    }
}