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

char type_of(char name[], char cur_address[])
{
    DIR *currentDir = opendir(cur_address);
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

int forward_one(char address[], char name[])
{
    strcpy(address + strlen(address), "\\");
    strcpy(address + strlen(address), name);
    return 0;
}

int back_one(char address[])
{
    int index = strlen(address) - 1;
    while (1)
    {
        if (*(address + index) == '\\')
        {
            *(address + index) = '\0';
            return 0;
        }
        index--;
        if (index == 0)
            return 1;
    }
    return 1;
}

int copy_file(char file_name[], char org_address[], char copy_directory[])
{
    char copy_address[1000];
    strcpy(copy_address, copy_directory);
    forward_one(copy_address, file_name);
    FILE *org, *copy;
    org = fopen(org_address, "rb");
    copy = fopen(copy_address, "wb");
    char buffer;
    int n;
    while (1)
    {
        if (feof(org))
            break;
        n = fread((void *)&buffer, sizeof(buffer), 1, org);
        fwrite((void *)&buffer, sizeof(buffer), n, copy);
    }
    fclose(org);
    fclose(copy);
    return 0;
}

int copy_folder(char folder_name[], char org_address[], char copy_directory[])
{
    char copy_address[1000];
    strcpy(copy_address, copy_directory);
    forward_one(copy_address, folder_name);
    mkdir(copy_address);
    DIR *currentDir = opendir(org_address);
    struct dirent *dir;
    while ((dir = readdir(currentDir)) != NULL)
    {
        char org_f_address[1000];
        strcpy(org_f_address, org_address);
        forward_one(org_f_address, dir->d_name);
        char type = type_of(dir->d_name, org_address);
        if (type == 'F')
            copy_folder(dir->d_name, org_f_address, copy_address);
        else if (type == 'f')
        {
            copy_file(dir->d_name, org_f_address, copy_address);
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

int file_changed(char file1[], char file2[])
{
    FILE *one, *two;
    one = fopen(file1, "rb");
    two = fopen(file2, "rb");
    char buffer1, buffer2;
    while (1)
    {
        if (feof(one) && feof(two))
            return 0;
        if (feof(one) || feof(two))
            return 1;
        fread(&buffer1, sizeof(buffer1), 1, one);
        fread(&buffer2, sizeof(buffer2), 1, two);
        if (buffer1 != buffer2)
            return 1;
    }
    return 0;
}

int commit_n_folder(int n)
{
    char commitfolder_address[1000];
    repoExists(commitfolder_address);
    char wd[1000];
    strcpy(wd, commitfolder_address);
    forward_one(commitfolder_address, "commit");
    char *m = (char *)malloc(100);
    sprintf(m, "%d", n);
    strcpy(commitfolder_address + strlen(commitfolder_address), m);
    mkdir(commitfolder_address);
    *(wd + strlen(wd) - 5) = '\0';
    DIR *currentDir = opendir(wd);
    struct dirent *dir;
    while ((dir = readdir(currentDir)) != NULL)
    {
        if (strcmp(dir->d_name, ".son") == 0)
            continue;
        char org_f_address[1000];
        strcpy(org_f_address, wd);
        char type = type_of(dir->d_name, org_f_address);
        forward_one(org_f_address, dir->d_name);
        if (type == 'F')
            copy_folder(dir->d_name, org_f_address, commitfolder_address);
        else if (type == 'f')
            copy_file(dir->d_name, org_f_address, commitfolder_address);
    }
    return 0;
}

int cur_commit(char address[])
{
    char repo[1000];
    repoExists(repo);
    DIR *cur_dir = opendir(repo);
    struct dirent *dir;
    int max = 0, n;
    while ((dir = readdir(cur_dir)) != NULL)
    {
        if (strstr(dir->d_name, "commit") != NULL)
        {
            sscanf(dir->d_name, "commit%d", &n);
            if (n > max)
                max = n;
        }
    }
    char temp[100];
    sprintf(temp, "commit%d", max);
    forward_one(repo, temp);
    strcpy(address, repo);
}

int makeRepo()
{
    mkdir(".son");
    mkdir(".\\.son\\staging");
    mkdir(".\\.son\\commit0");
    char commit0_address[1000];
    getcwd(commit0_address, 1000);
    strcpy(commit0_address + strlen(commit0_address), "\\.son\\commit0");
    char cwd[1000];
    getcwd(cwd, 1000);
    DIR *currentDir = opendir(".");
    struct dirent *dir;
    while ((dir = readdir(currentDir)) != NULL)
    {
        if (strcmp(dir->d_name, ".son") == 0)
            continue;
        char org_f_address[1000];
        strcpy(org_f_address, cwd);
        char type = type_of(dir->d_name, org_f_address);
        forward_one(org_f_address, dir->d_name);
        if (type == 'F')
            copy_folder(dir->d_name, org_f_address, commit0_address);
        else if (type == 'f')
            copy_file(dir->d_name, org_f_address, commit0_address);
    }
    return 0;
}

int line_exists(char address[], char line[])
{
    FILE *file = fopen(address, "r");
    char file_line[1000];
    while (fgets(file_line, 1000, file) != NULL)
    {
        if (*(file_line + strlen(file_line) - 1) == '\n')
            *(file_line + strlen(file_line) - 1) = '\0';
        if (strcmp(file_line, line) == 0)
            return 1;
    }
    return 0;
}

char items[100][1000];
int total_items = 0;

int main(int argc, char *argv[])
{
    // repo address
    char *repo_address = (char *)malloc(1000);
    repoExists(repo_address);
    // repo check
    if (*repo_address == '\0' && (strcmp(argv[1], "init") != 0 || argc != 2))
    {
        printf("No repo found!\n");
        return 1;
    }
    // items_list and command0
    // back_one(repo_address);
    // DIR *curr = opendir(repo_address);
    // *(repo_address + strlen(repo_address)) = '\\';
    // struct dirent *dir;
    // while ((dir = readdir(curr)) != NULL)
    // {
    //     if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
    //     {
    //         strcpy(items[total_items], dir->d_name);
    //         total_items++;
    //     }
    // }
    // alias read
    alias_read(argc, argv);
    // alias replace
    alias_replace(argc, argv);
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
            makeRepo();
        else
            printf("Repo already exists!\n");
        return 0;
    }
    // find_curren_commit
    char *cur_commit_address = (char *)malloc(1000);
    cur_commit(cur_commit_address);
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
    if (strcmp(argv[1], "add") == 0)
    {
        if (strcmp(argv[2], "-n") == 0)
        {
        }
        int count = 0;
        char type;
        char org_f_address[1000];
        char cur_commit_address[1000];
        cur_commit(cur_commit_address);
        char cwd[1000];
        getcwd(cwd, 1000);
        char temp[1000];
        strcpy(temp, repo_address);
        forward_one(temp, "added.txt");
        FILE *added = fopen(temp, "a");
        if (strcmp(argv[2], "-f") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                DIR *curr = opendir(".");
                struct dirent *dir;
                int flag = 0;
                while ((dir = readdir(curr)) != NULL)
                {
                    if (strcmp(argv[i], dir->d_name) == 0)
                    {
                        flag = 1;
                        strcpy(org_f_address, cwd);
                        forward_one(org_f_address, dir->d_name);
                        type = type_of(dir->d_name, cwd);
                        if (type == 'F')
                        {
                            copy_folder(dir->d_name, org_f_address, cur_commit_address);
                            if (line_exists(temp, org_f_address) == 0)
                                fprintf(added, "%s\n", org_f_address);
                        }
                        else if (type == 'f')
                        {
                            copy_file(dir->d_name, org_f_address, cur_commit_address);
                            if (line_exists(temp, org_f_address) == 0)
                                fprintf(added, "%s\n", org_f_address);
                        }
                    }
                }
                if (flag == 0)
                {
                    printf("%s doesn't seem to exist!\n", argv[i]);
                    count++;
                }
            }
            printf("%d items were succsesfully added to staging area.\n", argc - 3 - count);
        }
        else
        {
            DIR *curr = opendir(".");
            struct dirent *dir;
            int flag = 0;
            while ((dir = readdir(curr)) != NULL)
            {
                if (strcmp(argv[2], dir->d_name) == 0)
                {
                    flag = 1;
                    strcpy(org_f_address, cwd);
                    forward_one(org_f_address, dir->d_name);
                    type = type_of(dir->d_name, cwd);
                    if (type == 'F')
                    {
                        copy_folder(dir->d_name, org_f_address, cur_commit_address);
                        if (line_exists(temp, org_f_address) == 0)
                            fprintf(added, "%s\n", org_f_address);
                    }
                    else if (type == 'f')
                    {
                        copy_file(dir->d_name, org_f_address, cur_commit_address);
                        if (line_exists(temp, org_f_address) == 0)
                            fprintf(added, "%s\n", org_f_address);
                    }
                }
            }
            if (flag == 0)
            {
                printf("%s doesn't seem to exist!\n", argv[2]);
            }
            else
                printf("%s was succsesfully added to staging area.\n", argv[2]);
        }
        fclose(added);
        return 0;
    }
}