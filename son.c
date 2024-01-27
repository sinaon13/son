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
            chdir(temp);
            *address = '\0';
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
                strcpy(address + strlen(address), "\\.son");
                return 0;
            }
        }
        flag++;
    }
}

int alias_write(int argc, char *argv[])
{
    if (strcmp(argv[2], "-global") == 0)
    {
        FILE *alias = fopen("C:\\son\\alias.txt", "a");
        fprintf(alias, "%s %s\n", argv[3] + 6, argv[4]);
        fclose(alias);
    }
    else
    {
        char *wd = (char *)malloc(1000);
        repoExists(wd);
        strcpy(wd + strlen(wd), "\\alias.txt");
        FILE *alias = fopen(wd, "a");
        fprintf(alias, "%s %s\n", argv[2] + 6, argv[3]);
        fclose(alias);
        free(wd);
    }
    return 0;
}

char als_list[100][2][100];
char global_als_list[100][2][100];
int total_als_count = 0;
int total_global_als_count = 0;

int alias_read(int argc, char *argv[])
{
    // gloabl
    FILE *global_alias = fopen("C:\\son\\alias.txt", "r");
    char *line = (char *)malloc(1000);
    char temp_1[100], temp_2[100];
    while (fgets(line, 1000, global_alias) != NULL)
    {
        sscanf(line, "%s", temp_1);
        strcpy(temp_2, line + strlen(temp_1) + 1);
        *(temp_2 + strlen(temp_2) - 1) = '\0';
        strcpy(global_als_list[total_global_als_count][0], temp_1);
        strcpy(global_als_list[total_global_als_count][1], temp_2);
        total_global_als_count++;
    }
    fclose(global_alias);
    // local
    char *wd = (char *)malloc(1000);
    repoExists(wd);
    strcpy(wd + strlen(wd), "\\alias.txt");
    FILE *local_alias = fopen(wd, "r");
    while (fgets(line, 1000, local_alias) != NULL)
    {
        sscanf(line, "%s", temp_1);
        strcpy(temp_2, line + strlen(temp_1) + 1);
        *(temp_2 + strlen(temp_2) - 1) = '\0';
        strcpy(als_list[total_als_count][0], temp_1);
        strcpy(als_list[total_als_count][1], temp_2);
        total_als_count++;
    }
    fclose(local_alias);
    free(wd);
    free(line);
    return 0;
}

int alias_replace(int argc, char *argv[])
{
    // local
    for (int i = 1; i < argc; i++)
    {
        for (int j = total_als_count - 1; j >= 0; j--)
        {
            if (strcmp(argv[i], als_list[j][0]) == 0)
            {
                argv[i] = malloc(100);
                strcpy(argv[i], als_list[j][1]);
            }
        }
    }
    // global
    for (int i = 1; i < argc; i++)
    {
        for (int j = total_global_als_count - 1; j >= 0; j--)
        {
            if (strcmp(argv[i], global_als_list[j][0]) == 0)
            {
                argv[i] = malloc(100);
                strcpy(argv[i], global_als_list[j][1]);
            }
        }
    }
    return 0;
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
            strcpy(temp + strlen(temp), "\\name.txt");
            printf("%s", temp);
            FILE *name = fopen(temp, "w");
            fprintf(name, "%s", argv[3]);
            fclose(name);
        }
        if (strcmp(argv[2], "user.email") == 0)
        {
            strcpy(temp + strlen(temp), "\\email.txt");
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
    if (argc == 5 && strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0 && (strstr(argv[3], "alias.") != NULL))
        return 1;
    if (argc == 4 && strcmp(argv[1], "config") == 0 && (strstr(argv[2], "alias.") != NULL))
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
    mkdir(".son");
    return 0;
}

int main(int argc, char *argv[])
{
    // repo address
    char *repo_address = (char *)malloc(1000);
    repoExists(repo_address);
    printf("(%s)", repo_address);
    // alias read
    alias_read(argc, argv);
    // alias replace
    alias_replace(argc, argv);
    // print argv
    for (int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    // check validation
    if (validInput(argc, argv) == 0)
    {
        printf("Invalid Input!");
        return 1;
    }
    // check init
    if (strcmp(argv[1], "init") == 0 && *repo_address == '\0')
    {
        makeRepo(argc, argv);
        return 0;
    }

    if (repo_address == NULL)
    {
        printf("No repo found!");
        return 1;
    }

    if (strcmp(argv[1], "config") == 0 && (strstr(argv[2], "user.") != NULL || strstr(argv[3], "user.") != NULL))
    {
        userSettings(argc, argv);
        return 0;
    }

    if (strcmp(argv[1], "config") == 0 && (strstr(argv[2], "alias.") != NULL || strstr(argv[3], "alias.") != NULL))
    {
        alias_write(argc, argv);
        return 0;
    }
}