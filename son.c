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

char type_of(char name[])
{
    DIR *currentDir = opendir(".");
    struct dirent *dir;
    while ((dir = readdir(currentDir)) != NULL)
    {
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            return 'N';
        if (strcmp(name, dir->d_name) == 0)
            if (dir->d_type == 16)
                return 'F';
            else
            {
                return 'f';
            }
    }
}

int add_file(char file_name[], char copy_address[])
{
    char temp_copy_address[1000];
    strcpy(temp_copy_address, copy_address);
    char file_address[1000];
    getcwd(file_address, 1000);
    strcpy(file_address + strlen(file_address), "\\");
    strcpy(file_address + strlen(file_address), file_name);
    strcpy(copy_address + strlen(copy_address), "\\");
    strcpy(copy_address + strlen(copy_address), file_name);
    FILE *org, *copy;
    org = fopen(file_address, "rb");
    copy = fopen(copy_address, "wb");
    strcpy(copy_address, temp_copy_address);
    char buffer;
    while (!feof(org))
    {
        fread(&buffer, sizeof(buffer), 1, org);
        fwrite(&buffer, sizeof(buffer), 1, copy);
    }
    fclose(org);
    fclose(copy);
    return 0;
}

int add_folder(char folder_name[], char copy_address[])
{
    DIR *currentDir = opendir(".");
    struct dirent *dir;
    char wd[1000];
    char temp_copy_address[1000];
    strcpy(temp_copy_address, copy_address);
    while ((dir = readdir(currentDir)) != NULL)
    {
        char type = type_of(dir->d_name);
        if (type == 'F')
        {
            strcpy(temp_copy_address, copy_address);
            strcpy(copy_address + strlen(copy_address), "\\");
            strcpy(copy_address + strlen(copy_address), dir->d_name);
            mkdir(copy_address);
            getcwd(wd, 1000);
            chdir(dir->d_name);
            add_folder(dir->d_name, copy_address);
            strcpy(copy_address, temp_copy_address);
            chdir(wd);
        }
        else if (type == 'f')
        {
            add_file(dir->d_name, temp_copy_address);
            strcpy(copy_address, temp_copy_address);
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
    if (argc == 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-f") != 0)
        return 1;
    if (argc > 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-f") == 0)
        return 1;
    return 0;
}

int alreadyExists(char name[])
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
    mkdir(".\\.son\\staging");
    return 0;
}

int main(int argc, char *argv[])
{
    // repo address
    char *repo_address = (char *)malloc(1000);
    repoExists(repo_address);
    // alias read
    alias_read(argc, argv);
    // alias replace
    alias_replace(argc, argv);
    // repo check
    if (*repo_address == '\0' && (strcmp(argv[1], "init") != 0 || argc != 2))
    {
        printf("No repo found!\n");
        return 1;
    }
    // check validation
    if (validInput(argc, argv) == 0)
    {
        printf("Invalid Input!\n");
        return 1;
    }
    // check init
    if (strcmp(argv[1], "init") == 0)
    {
        if (*repo_address == '\0')
            makeRepo(argc, argv);
        else
            printf("Repo already exists!\n");
        return 0;
    }
    // user.name / user.email
    if (strcmp(argv[1], "config") == 0 && (strstr(argv[2], "user.") != NULL || strstr(argv[3], "user.") != NULL))
    {
        userSettings(argc, argv);
        return 0;
    }
    // alias
    if (strcmp(argv[1], "config") == 0 && (strstr(argv[2], "alias.") != NULL || strstr(argv[3], "alias.") != NULL))
    {
        alias_write(argc, argv);
        return 0;
    }
    // add
    if (strcmp(argv[1], "add") == 0)
    {
        int fail = 0;
        char copy_adress[1000];
        repoExists(copy_adress);
        strcpy(copy_adress + strlen(copy_adress), "\\staging");
        char temp_copy_address[1000];
        strcpy(temp_copy_address, copy_adress);
        char wd[1000];
        getcwd(wd, 1000);
        if (strcmp(argv[2], "-f") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                if (alreadyExists(argv[i]) == 0)
                {
                    fail++;
                    printf("%s doesn't seem to exist!\n", argv[i]);
                }
                else
                {
                    if (type_of(argv[i]) == 'f')
                    {
                        add_file(argv[i], copy_adress);
                        strcpy(copy_adress, temp_copy_address);
                        chdir(wd);
                    }
                    else
                    {
                        strcpy(copy_adress + strlen(copy_adress), "\\");
                        strcpy(copy_adress + strlen(copy_adress), argv[i]);
                        mkdir(copy_adress);
                        chdir(argv[i]);
                        add_folder(argv[i], copy_adress);
                        strcpy(copy_adress, temp_copy_address);
                        chdir(wd);
                    }
                }
            }
            printf("%d folder/files were succesfully added to staging area.\n", argc - 3 - fail);
        }
        else
        {
            if (alreadyExists(argv[2]) == 0)
                printf("%s doesn't seem to exist!\n", argv[2]);
            else
            {
                if (type_of(argv[2]) == 'f')
                {
                    add_file(argv[2], copy_adress);
                    strcpy(copy_adress, temp_copy_address);
                    chdir(wd);
                }
                else
                {
                    strcpy(copy_adress + strlen(copy_adress), "\\");
                    strcpy(copy_adress + strlen(copy_adress), argv[2]);
                    mkdir(copy_adress);
                    chdir(argv[2]);
                    add_folder(argv[2], copy_adress);
                    strcpy(copy_adress, temp_copy_address);
                    chdir(wd);
                }
            }
            printf("folder/file was succsesfully added to staging area.\n");
        }
    }
}