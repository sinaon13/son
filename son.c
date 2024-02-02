#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

char current_branch[1000];
int current_branch_ID;
char current_commit[1000];

int count_words(char arg[])
{
    int count = 0;
    if (*arg != ' ')
        count++;
    for (int i = 0; i < strlen(arg) - 1; i++)
    {
        if (*(arg + i) == ' ' && *(arg + i + 1) != ' ')
            count++;
    }
    return count;
}

int count_words_slash(char arg[])
{
    int count = 0;
    if (*arg != '\\')
        count++;
    for (int i = 0; i < strlen(arg) - 1; i++)
    {
        if (*(arg + i) == '\\' && *(arg + i + 1) != '\\')
            count++;
    }
    return count;
}

int separator(char arg[], char sep[][1000])
{
    int count = 0;
    if (*arg != ' ')
    {
        for (int j = 1; j < strlen(arg); j++)
        {
            if (*(arg + j) == ' ' && *(arg + j - 1) != ' ')
            {
                memcpy(sep[count], arg, j);
                *(sep[count] + j) = '\0';
                count++;
                break;
            }
            if (j == strlen(arg) - 1)
            {
                memcpy(sep[count], arg, j + 1);
                *(sep[count] + j + 1) = '\0';
                count++;
                break;
            }
        }
    }
    for (int i = 0; i < strlen(arg) - 1; i++)
    {
        if (*(arg + i) == ' ' && *(arg + i + 1) != ' ')
        {
            for (int j = i + 1; j < strlen(arg); j++)
            {
                if (*(arg + j) == ' ' && *(arg + j - 1) != ' ')
                {
                    memcpy(sep[count], arg + i + 1, j - i - 1);
                    *(sep[count] + j - i - 1) = '\0';
                    count++;
                    break;
                }
                if (j == strlen(arg) - 1)
                {
                    memcpy(sep[count], arg + i + 1, j - i);
                    *(sep[count] + j - i) = '\0';
                    count++;
                }
            }
        }
    }
}

int slash_separator(char arg[], char sep[][1000])
{
    int count = 0;
    if (*arg != '\\')
    {
        for (int j = 1; j < strlen(arg); j++)
        {
            if (*(arg + j) == '\\' && *(arg + j - 1) != '\\')
            {
                memcpy(sep[count], arg, j);
                *(sep[count] + j) = '\0';
                count++;
                break;
            }
            if (j == strlen(arg) - 1)
            {
                memcpy(sep[count], arg, j + 1);
                *(sep[count] + j + 1) = '\0';
                count++;
                break;
            }
        }
    }
    for (int i = 0; i < strlen(arg) - 1; i++)
    {
        if (*(arg + i) == '\\' && *(arg + i + 1) != '\\')
        {
            for (int j = i + 1; j < strlen(arg); j++)
            {
                if (*(arg + j) == '\\' && *(arg + j - 1) != '\\')
                {
                    memcpy(sep[count], arg + i + 1, j - i - 1);
                    *(sep[count] + j - i - 1) = '\0';
                    count++;
                    break;
                }
                if (j == strlen(arg) - 1)
                {
                    memcpy(sep[count], arg + i + 1, j - i);
                    *(sep[count] + j - i) = '\0';
                    count++;
                }
            }
        }
    }
}

int word_exists(char word[], char str[])
{
    int n = count_words(str);
    char sep_str[n][1000];
    separator(str, sep_str);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(word, sep_str[i]) == 0)
            return 1;
    }
    return 0;
}

int str_to_int(char str[])
{
    int pow = 1;
    int number = 0;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        number += (*(str + i) - '0') * pow;
        pow *= 10;
    }
    return number;
}

int line_counter(char address[])
{
    FILE *file = fopen(address, "r");
    char line[1000];
    int count = 0;
    int flag;
    while (fgets(line, 1000, file) != NULL)
    {
        flag = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (*(line + i) != ' ' && *(line + i) != '\n')
            {
                flag = 1;
                break;
            }
        }

        if (flag)
            count++;
    }
    return count;
}

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

int new_branch_number()
{
    int max = 0, n;
    char dot_son[1000];
    repoExists(dot_son);
    DIR *son = opendir(dot_son);
    struct dirent *dir;
    while ((dir = readdir(son)) != NULL)
    {
        if (strstr(dir->d_name, "branch") != NULL)
        {
            sscanf(dir->d_name, "branch%d", &n);
            if (n > max)
                max = n;
        }
    }
    return max + 1;
}

int alias_write(int argc, char *argv[])
{
    if (strcmp(argv[2], "-global") == 0)
    {
        FILE *alias = fopen("C:\\son\\alias.txt", "a");
        fprintf(alias, "%s %s\n", argv[3] + 6, argv[4]);
        printf("%s is now an alias to %s.", argv[3] + 6, argv[4]);
        fclose(alias);
    }
    else
    {
        char *wd = (char *)malloc(1000);
        repoExists(wd);
        strcpy(wd + strlen(wd), "\\alias.txt");
        FILE *alias = fopen(wd, "a");
        fprintf(alias, "%s %s\n", argv[2] + 6, argv[3]);
        printf("\"%s\" is now an alias to \"%s\".", argv[2] + 6, argv[3]);
        fclose(alias);
        free(wd);
    }
    return 0;
}

int shortcut_write(int argc, char *argv[])
{
    char wd[1000];
    repoExists(wd);
    strcpy(wd + strlen(wd), "\\shortcut.txt");
    FILE *shortcut = fopen(wd, "a");
    fprintf(shortcut, "%s %s\n", argv[5], argv[3]);
    printf("\"%s\" is now a shortcut-message for \"%s\".", argv[5], argv[3]);
    fclose(shortcut);
    return 0;
}

int shortcut_replace(int argc, char *argv[])
{
    char wd[1000];
    repoExists(wd);
    strcpy(wd + strlen(wd), "\\shortcut.txt");
    char line[1000];
    char file_line[1000];
    char check[1000];
    FILE *shortcut = fopen(wd, "r");
    char lines[1000][1000];
    int index = 0;
    int flag = 0;
    while (fgets(file_line, 1000, shortcut) != NULL)
    {
        if (*(file_line + strlen(file_line) - 1) == '\n')
            *(file_line + strlen(file_line) - 1) = '\0';
        sscanf(file_line, "%s ", check);
        if (strcmp(argv[5], check) == 0)
        {
            flag = 1;
            strcpy(line, argv[5]);
            strcat(line, " ");
            strcat(line, argv[3]);
            strcpy(lines[index], line);
            printf("%s\n", lines[index]);
        }
        else
            strcpy(lines[index], file_line);
        index++;
    }
    fclose(shortcut);
    if (flag == 0)
        return 1;
    FILE *shortcut_new = fopen(wd, "w");
    for (int i = 0; i < index; i++)
    {
        fprintf(shortcut_new, "%s\n", lines[i]);
    }
    fclose(shortcut_new);
    return 0;
}

int shortcut_remove(int argc, char *argv[])
{
    char wd[1000];
    repoExists(wd);
    strcpy(wd + strlen(wd), "\\shortcut.txt");
    char file_line[1000];
    char check[1000];
    FILE *shortcut = fopen(wd, "r");
    char lines[1000][1000];
    int yes[1000] = {0};
    int index = 0;
    int flag = 0;
    while (fgets(file_line, 1000, shortcut) != NULL)
    {
        if (*(file_line + strlen(file_line) - 1) == '\n')
            *(file_line + strlen(file_line) - 1) = '\0';
        sscanf(file_line, "%s ", check);
        if (strcmp(argv[3], check) != 0)
        {
            strcpy(lines[index], file_line);
            yes[index] = 1;
        }
        else
            flag = 1;
        index++;
    }
    if (flag == 0)
        return 1;
    fclose(shortcut);
    FILE *shortcut_new = fopen(wd, "w");
    for (int i = 0; i < index; i++)
    {
        if (yes[i] != 1)
            continue;
        fprintf(shortcut_new, "%s\n", lines[i]);
    }
    fclose(shortcut_new);
    return 0;
}

int shortcut_message(char message[])
{
    char wd[1000];
    repoExists(wd);
    strcpy(wd + strlen(wd), "\\shortcut.txt");
    char file_line[1000];
    char check[1000];
    FILE *shortcut = fopen(wd, "r");
    while (fgets(file_line, 1000, shortcut) != NULL)
    {
        if (*(file_line + strlen(file_line) - 1) == '\n')
            *(file_line + strlen(file_line) - 1) = '\0';
        sscanf(file_line, "%s ", check);
        if (strcmp(message, check) == 0)
        {
            strcpy(message, file_line + strlen(check) + 1);
            fclose(shortcut);
            return 0;
        }
    }
    return 1;
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
    // global
    int done[100] = {0};
    for (int i = 1; i < argc; i++)
    {
        for (int j = total_global_als_count - 1; j >= 0; j--)
        {
            if (strcmp(argv[i], global_als_list[j][0]) == 0)
            {
                int n = count_words(als_list[j][1]);
                char sep[n][1000];
                separator(global_als_list[j][1], sep);
                for (int k = argc - 1; k > i; k--)
                {
                    argv[k + n - 1] = (char *)malloc(1000);
                    strcpy(argv[k + n - 1], argv[k]);
                }
                for (int k = i; k < n + i; k++)
                {
                    argv[k] = (char *)malloc(1000);
                    strcpy(argv[k], sep[k - i]);
                }
                argc += n - 1;
                i += n - 1;
            }
        }
    }
    // local
    for (int i = 1; i < argc; i++)
    {
        if (done[i] == 1)
            continue;
        for (int j = total_als_count - 1; j >= 0; j--)
        {
            if (strcmp(argv[i], als_list[j][0]) == 0)
            {
                int n = count_words(als_list[j][1]);
                char sep[n][1000];
                separator(als_list[j][1], sep);
                for (int k = argc - 1; k > i; k--)
                {
                    argv[k + n - 1] = (char *)malloc(1000);
                    strcpy(argv[k + n - 1], argv[k]);
                }
                for (int k = i; k < n + i; k++)
                {
                    argv[k] = (char *)malloc(1000);
                    strcpy(argv[k], sep[k - i]);
                    done[k] = 1;
                }
                argc += n - 1;
                i += n - 1;
            }
        }
    }
    return argc;
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

int copy_file(char file_name[], char org_address[], char copy_directory[], char check)
{
    if (strcmp(file_name, "commitInfo.txt") == 0 && check != 'y')
        return 1;
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
            copy_file(dir->d_name, org_f_address, copy_address, 'a');
    }
    return 0;
}

int userSettings(int argc, char *argv[])
{
    if (strcmp(argv[2], "-global") == 0)
    {
        if (strcmp(argv[3], "user.name") == 0)
        {
            FILE *all = fopen("c:\\son\\repos.txt", "r");
            char n[1000];
            while (fgets(n, 1000, all) != NULL)
            {
                if (*(n + strlen(n) - 1) == '\n')
                    *(n + strlen(n) - 1) = '\0';
                forward_one(n, ".son\\name.txt");
                FILE *name = fopen(n, "w");
                fprintf(name, "%s", argv[4]);
                fclose(name);
            }
            printf("Global username is now set to \"%s\"\n", argv[4]);
        }
        if (strcmp(argv[3], "user.email") == 0)
        {
            FILE *all = fopen("c:\\son\\repos.txt", "r");
            char n[1000];
            while (fgets(n, 1000, all) != NULL)
            {
                if (*(n + strlen(n) - 1) == '\n')
                    *(n + strlen(n) - 1) = '\0';
                forward_one(n, ".son\\email.txt");
                FILE *email = fopen(n, "w");
                fprintf(email, "%s", argv[4]);
                fclose(email);
            }
            printf("Global email is now set to \"%s\"\n", argv[4]);
        }
    }
    else
    {
        char *temp = (char *)malloc(1000);
        repoExists(temp);
        if (strcmp(argv[2], "user.name") == 0)
        {
            strcpy(temp + strlen(temp), "\\name.txt");
            FILE *name = fopen(temp, "w");
            fprintf(name, "%s", argv[3]);
            fclose(name);
            printf("Local username is now set to \"%s\"\n", argv[3]);
        }
        if (strcmp(argv[2], "user.email") == 0)
        {
            strcpy(temp + strlen(temp), "\\email.txt");
            FILE *email = fopen(temp, "w");
            fprintf(email, "%s", argv[3]);
            fclose(email);
            printf("Local email is now set to \"%s\"\n", argv[3]);
        }
    }
}

int validInput(int argc, char *argv[])
{
    if (argc < 2)
        return 0;
    if (strcmp(argv[0], "son") != 0)
        return 0;
    if (argc == 5 && strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0 && ((strcmp(argv[3], "user.email") == 0 || strcmp(argv[3], "user.name")) == 0))
        return 1;
    if (argc == 4 && strcmp(argv[1], "config") == 0 && ((strcmp(argv[2], "user.email") == 0 || strcmp(argv[2], "user.name")) == 0))
        return 1;
    if (argc == 2 && strcmp(argv[1], "init") == 0)
        return 1;
    if (argc == 5 && strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0 && (strstr(argv[3], "alias.") != NULL))
        return 1;
    if (argc == 4 && strcmp(argv[1], "config") == 0 && (strstr(argv[2], "alias.") != NULL))
        return 1;
    if (argc == 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-f") != 0 && strcmp(argv[2], "-n") != 0 && strcmp(argv[2], "-redo") != 0)
        return 1;
    if (argc > 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-f") == 0)
        return 1;
    if (argc == 4 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-n") == 0)
        return 1;
    if (argc == 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-redo") == 0)
        return 1;
    if (argc == 2 && strcmp(argv[1], "status") == 0)
        return 1;
    if (strcmp(argv[1], "commit") == 0 && (strcmp(argv[2], "-m") == 0 || strcmp(argv[2], "-s") == 0))
        return 1;
    if (strcmp(argv[1], "set") == 0 && strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
        return 1;
    if (strcmp(argv[1], "replace") == 0 && strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
        return 1;
    if (strcmp(argv[1], "remove") == 0 && strcmp(argv[2], "-s") == 0)
        return 1;
    if (argc == 2 && strcmp(argv[1], "log") == 0)
        return 1;
    if (argc == 4 && strcmp(argv[1], "log") == 0 && strcmp(argv[2], "-n") == 0)
        return 1;
    if (argc == 4 && strcmp(argv[1], "log") == 0 && strcmp(argv[2], "-author") == 0)
        return 1;
    if (argc == 7 && strcmp(argv[1], "log") == 0 && strcmp(argv[2], "-since") == 0)
        return 1;
    if (argc == 7 && strcmp(argv[1], "log") == 0 && strcmp(argv[2], "-before") == 0)
        return 1;
    if (argc == 4 && strcmp(argv[1], "log") == 0 && strcmp(argv[2], "-search") == 0)
        return 1;
    if (argc == 3 && strcmp(argv[1], "branch") == 0)
        return 1;
    if (argc == 2 && strcmp(argv[1], "branch") == 0)
        return 1;
    if (argc == 3 && strcmp(argv[1], "checkout") == 0)
        return 1;
    return 0;
}

int alreadyExists(char name[], char address[])
{
    DIR *currentDir = opendir(address);
    struct dirent *dir;
    while ((dir = readdir(currentDir)) != NULL)
    {
        if (strcmp(name, dir->d_name) == 0)
            return 1;
    }
    return 0;
}

int commit_ID_to_number_dot_number(int ID, char result[])
{
    int check_id;
    char dot_son[1000];
    repoExists(dot_son);
    DIR *son = opendir(dot_son);
    struct dirent *dir;
    while ((dir = readdir(son)) != NULL)
    {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        if (strstr(dir->d_name, "commit") != NULL && strcmp(dir->d_name, "commitId.txt") != 0)
        {
            char commit_info_address[1000];
            strcpy(commit_info_address, dot_son);
            forward_one(commit_info_address, dir->d_name);
            forward_one(commit_info_address, "commitInfo.txt");
            FILE *commitInfo = fopen(commit_info_address, "r");
            char commitIDstr[1000];
            fgets(commitIDstr, 1000, commitInfo);
            sscanf(commitIDstr, "%d", &check_id);
            if (check_id == ID)
            {
                strcpy(result, dir->d_name);
                fclose(commitInfo);
                return 0;
            }
        }
    }
    return 1;
}

int branch_name_to_number_dot_number(char branch_name[], char result[])
{
    char dot_son[1000];
    repoExists(dot_son);
    DIR *son = opendir(dot_son);
    struct dirent *dir;
    while ((dir = readdir(son)) != NULL)
    {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        if (strstr(dir->d_name, "branch") != NULL && dir->d_name[0] != 'c')
        {
            char br_address[1000];
            strcpy(br_address, dot_son);
            forward_one(br_address, dir->d_name);
            FILE *br = fopen(br_address, "r");
            char line[1000];
            fgets(line, 1000, br);
            if (*(line + strlen(line) - 1) == '\n')
                *(line + strlen(line) - 1) = '\0';
            if (strcmp(line, branch_name) != 0)
                continue;
            while (fgets(line, 1000, br) != NULL)
                strcpy(result, line);
            if (*(result + strlen(result) - 1) == '\n')
                *(result + strlen(result) - 1) = '\0';
            return 0;
        }
    }
    return 1;
}

int last_maker(char address[])
{
    for (int i = strlen(address) - 1; i >= 0; i--)
    {
        if (*(address + i) == '\\')
        {
            strcpy(address, address + i + 1);
            return 0;
        }
    }
    return 0;
}

int file_changed(char file1[], char file2[])
{
    FILE *one, *two;
    one = fopen(file1, "rb");
    two = fopen(file2, "rb");
    if (one == NULL)
        return 2;
    if (two == NULL)
        return 3;
    char buffer1, buffer2;
    int index = 0;
    while (1)
    {
        index++;
        if (feof(one) && feof(two))
            return 0;
        if (feof(one) || feof(two))
            return 1;
        fread(&buffer1, sizeof(buffer1), 1, one);
        fread(&buffer2, sizeof(buffer2), 1, two);
        if (buffer1 != buffer2 && index != 1)
            return 1;
    }
    return 0;
}

int folder_changed(char folder1[], char folder2[])
{
    DIR *Folder1 = opendir(folder1);
    struct dirent *f1;
    int done[1000] = {0};
    int flag, index = 0;
    int b[1000] = {0};
    while ((f1 = readdir(Folder1)) != NULL)
    {
        if (strcmp(f1->d_name, ".") != 0 && strcmp(f1->d_name, "..") != 0 && strcmp(f1->d_name, ".son") != 0)
        {
            flag = 0;
            DIR *Folder2 = opendir(folder2);
            struct dirent *f2;
            index = 0;
            while ((f2 = readdir(Folder2)) != NULL)
            {
                if (strcmp(f2->d_name, ".") != 0 && strcmp(f2->d_name, "..") != 0 && strcmp(f2->d_name, ".son") != 0)
                {
                    if (strcmp(f1->d_name, f2->d_name) == 0)
                    {
                        b[index] = 1;
                        flag = 1;
                        char temp1[1000], temp2[1000];
                        strcpy(temp1, folder1);
                        forward_one(temp1, f1->d_name);
                        strcpy(temp2, folder2);
                        forward_one(temp2, f2->d_name);
                        if (f1->d_type == 16 && f2->d_type == 16)
                        {
                            if (folder_changed(temp1, temp2) == 1)
                                return 1;
                        }
                        else if (file_changed(temp1, temp2) == 1)
                            return 1;
                    }
                    index++;
                }
            }
            if (flag == 0)
                return 1;
        }
    }
    for (int i = 0; i < index; i++)
    {
        if (b[i] != 1)
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
    sprintf(m, "%d.%d", n, current_branch_ID);
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
            copy_file(dir->d_name, org_f_address, commitfolder_address, 'a');
    }
    repoExists(wd);
    char name[1000];
    sprintf(name, "commit%d.%d", n, current_branch_ID);
    char name2[1000];
    sprintf(name2, "commit%d.%d", n - 1, current_branch_ID);
    char org_f_address[1000];
    strcpy(org_f_address, wd);
    forward_one(org_f_address, name2);
    copy_folder(name, org_f_address, wd);
    return 0;
}

int cur_commit(char address[])
{
    repoExists(address);
    int n, junk;
    char dot_son[1000];
    repoExists(dot_son);
    forward_one(dot_son, "curr_commit.txt");
    FILE *cr_cm = fopen(dot_son, "r");
    char cr[1000];
    fgets(cr, 1000, cr_cm);
    fclose(cr_cm);
    if (*(cr + strlen(cr) - 1) == '\n')
        *(cr + strlen(cr) - 1) = '\0';
    forward_one(address, cr);
    sscanf(cr, "commit%d.%d", &n, &junk);
    return n;
}

int last_commit(char address[])
{
    repoExists(address);
    int n, junk;
    char dot_son[1000];
    repoExists(dot_son);
    forward_one(dot_son, "curr_commit.txt");
    FILE *cr_cm = fopen(dot_son, "r");
    char cr[1000];
    fgets(cr, 1000, cr_cm);
    fclose(cr_cm);
    if (*(cr + strlen(cr) - 1) == '\n')
        *(cr + strlen(cr) - 1) = '\0';
    sscanf(cr, "commit%d.%d", &n, &junk);
    sprintf(cr, "commit%d.%d", n - 1, junk);
    forward_one(address, cr);
    return n - 1;
}

int makeRepo()
{
    char cwd[1000];
    getcwd(cwd, 1000);
    mkdir(".son");
    char master_branch[1000];
    getcwd(master_branch, 1000);
    forward_one(master_branch, ".son");
    forward_one(master_branch, "branch0.txt");
    FILE *master = fopen(master_branch, "w");
    fprintf(master, "master\ncommit0.0\n");
    fclose(master);
    char cur_cm[1000];
    strcpy(cur_cm, cwd);
    forward_one(cur_cm, ".son\\curr_commit.txt");
    FILE *curr_commit = fopen(cur_cm, "w");
    fprintf(curr_commit, "commit1.0");
    fclose(curr_commit);
    char branch[1000];
    getcwd(branch, 1000);
    forward_one(branch, ".son");
    forward_one(branch, "curr_branch.txt");
    FILE *curr_branch = fopen(branch, "w");
    fprintf(curr_branch, "branch0.txt");
    fclose(curr_branch);
    FILE *repo_address = fopen(".\\.son\\repo_address.txt", "w");
    fprintf(repo_address, "%s", cwd);
    fclose(repo_address);
    FILE *cID = fopen(".\\.son\\commitID.txt", "w");
    fprintf(cID, "%d\n", 1000);
    fclose(cID);
    FILE *repos = fopen("c:\\son\\repos.txt", "a");
    fprintf(repos, "%s\n", cwd);
    fclose(repos);
    mkdir(".\\.son\\commit0.0");
    char commit0_address[1000];
    getcwd(commit0_address, 1000);
    strcpy(commit0_address + strlen(commit0_address), "\\.son\\commit0.0");
    char cmInfo[1000];
    strcpy(cmInfo, commit0_address);
    forward_one(cmInfo, "commitInfo.txt");
    FILE *commitInfo = fopen(cmInfo, "w");
    fprintf(commitInfo, "1000\n");
    fclose(commitInfo);
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
            copy_file(dir->d_name, org_f_address, commit0_address, 'a');
    }
    commit_n_folder(1);
    printf("A new repo was succsessfully made.\n");
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

int append_added(FILE *file, char address[])
{
    DIR *cur_folder = opendir(address);
    struct dirent *d;
    char type;
    char added[1000];
    repoExists(added);
    forward_one(added, "added.txt");
    while ((d = readdir(cur_folder)) != NULL)
    {
        char temp[1000], temp_line[1000];
        strcpy(temp, address);
        forward_one(temp, d->d_name);
        sprintf(temp_line, "%s", temp);
        type = type_of(d->d_name, address);
        if (line_exists(added, temp_line) == 0)
            if (type == 'F')
            {
                fprintf(file, "%s\n", temp);
                append_added(file, temp);
            }
            else if (type == 'f')
            {
                fprintf(file, "%s\n", temp);
            }
    }
    return 0;
}

int check_if_staged_or_not(char type, char address[])
{
    char cur_commit_address[1000];
    cur_commit(cur_commit_address);
    char repo_address[1000];
    repoExists(repo_address);
    back_one(repo_address);
    char temp[1000];
    strcpy(temp, address + strlen(repo_address));
    strcpy(cur_commit_address + strlen(cur_commit_address), temp);
    char added[1000];
    repoExists(added);
    forward_one(added, "added.txt");
    int in_added = line_exists(added, address);
    int changed;
    if (type == 'f')
        changed = file_changed(address, cur_commit_address);
    else
        changed = folder_changed(address, cur_commit_address);
    if (changed == 0 && in_added == 1)
        return 1;
    else
        return 0;
}

int added_n(char address[], int depth, int max_depth)
{
    if (depth == max_depth + 1)
        return 0;
    DIR *cur_folder = opendir(address);
    struct dirent *d;
    char type;
    char added[1000];
    repoExists(added);
    while ((d = readdir(cur_folder)) != NULL)
    {
        char temp[1000], temp_line[1000];
        strcpy(temp, address);
        forward_one(temp, d->d_name);
        type = type_of(d->d_name, address);
        if (type == 'F' || type == 'f')
        {
            int stg;
            if (type == 'F')
                stg = check_if_staged_or_not('F', temp);
            else
                stg = check_if_staged_or_not('f', temp);
            char werid_temp[1000];
            sprintf(werid_temp, "%d %s", depth, temp);
            char state[1000];
            if (stg == 1)
                strcpy(state, "--> staged");
            else
                strcpy(state, "--> Not staged");
            char temp_2[1000];
            strcpy(temp_2, temp);
            last_maker(temp_2);
            printf("\n");
            for (int i = 0; i < depth; i++)
                printf("    ");
            printf("%d %s %s", depth, temp_2, state);
        }
        if (type == 'F')
        {
            added_n(temp, ++depth, max_depth);
            depth--;
        }
    }
    return 0;
}

int repo_to_commit(char address[], char which)
{
    char cur[1000];
    if (which == 'c')
        cur_commit(cur);
    else
        last_commit(cur);
    char temp[1000];
    repoExists(temp);
    back_one(temp);
    char temp_address[1000];
    strcpy(temp_address, address);
    strcpy(temp_address, temp_address + strlen(temp));
    strcpy(cur + strlen(cur), temp_address);
    strcpy(address, cur);
}

int commit_to_repo(char address[])
{
    char *c = strstr(address, "commit");
    char *t = c;
    while (*t != '\\')
    {
        if (t - address > strlen(address))
        {
            c = strstr(address, ".son");
            *(c - 1) = '\0';
            return 0;
        }
        t++;
    }
    char temp[1000];
    strcpy(temp, t + 1);
    strcpy(c - 5, temp);
    return 0;
}

int username_read(char username[])
{
    char dot_son[1000];
    repoExists(dot_son);
    if (alreadyExists("name.txt", dot_son) == 1)
    {
        forward_one(dot_son, "name.txt");
        FILE *b = fopen(dot_son, "r");
        fgets(username, 1000, b);
        fclose(b);
    }
    else
    {
        printf("You haven't set any usernames yet!\n");
        return 1;
    }
    if (*(username + strlen(username) - 1) == '\n')
        *(username + strlen(username) - 1) = '\0';
    return 0;
}

int cur_commitID()
{
    char dot_son[1000];
    repoExists(dot_son);
    forward_one(dot_son, "commitID.txt");
    FILE *cID = fopen(dot_son, "r");
    char IDstring[100];
    int ID;
    fgets(IDstring, 100, cID);
    while (1)
    {
        sscanf(IDstring, "%d", &ID);
        if (fgets(IDstring, 100, cID) == NULL)
        {
            fclose(cID);
            return ID;
        }
    }
}

int commitfunc(char *argv3)
{
    char dot_son[1000];
    repoExists(dot_son);
    forward_one(dot_son, "added.txt");
    FILE *added = fopen(dot_son, "r");
    char a[1000];
    char b = a[0];
    fgets(a, 1000, added);
    fclose(added);
    int count = line_counter(dot_son);
    if (a[0] != b)
    {
        int commitID = cur_commitID();
        commitID++;
        time_t mytime = time(NULL);
        char *time_str = ctime(&mytime);
        time_str[strlen(time_str) - 1] = '\0';
        strcpy(time_str, time_str + 4);
        char name[1000];
        if (username_read(name) == 1)
            exit(1);
        char commit_info_file[1000];
        int n = cur_commit(commit_info_file);
        forward_one(commit_info_file, "commitInfo.txt");
        FILE *info = fopen(commit_info_file, "w");
        fprintf(info, "%d\n%s\n%s\n%s\n%d", commitID, argv3, name, time_str, count);
        printf("'''''''''''''''''''''''''''''''''''\n");
        printf("Succsessful!\n\n");
        printf("Commit ID --> %d\n\n", commitID);
        printf("Commit message --> %s\n\n", argv3);
        printf("Username --> %s\n\n", name);
        printf("Time --> %s\n\n", time_str);
        printf("Commited files/folders count --> %d\n\n", count);
        printf("'''''''''''''''''''''''''''''''''''");
        fclose(info);
        back_one(dot_son);
        forward_one(dot_son, current_branch);
        FILE *c_br = fopen(dot_son, "a");
        fprintf(c_br, "commit%d.%d\n", n, current_branch_ID);
        fclose(c_br);
        back_one(dot_son);
        forward_one(dot_son, "added.txt");
        commit_n_folder(n);
        FILE *addednew = fopen(dot_son, "w");
        fclose(addednew);
        back_one(dot_son);
        forward_one(dot_son, "commitId.txt");
        FILE *ID = fopen(dot_son, "a");
        fprintf(ID, "%d\n", commitID);
        fclose(ID);
        char curr_commit_address[1000];
        repoExists(curr_commit_address);
        forward_one(curr_commit_address, "curr_commit.txt");
        FILE *cr_cm = fopen(curr_commit_address, "w");
        fprintf(cr_cm, "commit%d.%d", n + 1, current_branch_ID);
        fclose(cr_cm);

        return 0;
    }
    else
    {
        printf("No changes have been made since the last commit.\n");
        exit(1);
    }
}

int log_n(int n)
{
    char name[1000];
    sprintf(name, "commit%d.%d", n, current_branch_ID);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, name);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000], line[1000];
    fgets(line, 1000, info);
    sscanf(line, "%d", &ID);
    fgets(line, 1000, info);
    strcpy(message, line);
    if (*(message + strlen(message) - 1) == '\n')
        *(message + strlen(message) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(username, line);
    if (*(username + strlen(username) - 1) == '\n')
        *(username + strlen(username) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(time, line);
    if (*(time + strlen(time) - 1) == '\n')
        *(time + strlen(time) - 1) = '\0';
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    printf("'''''''''''''''''''''''''''''''''''\n");
    printf("Succsessful!\n\n");
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("Commited files/folders count --> %d\n\n", count);
    printf("'''''''''''''''''''''''''''''''''''");
}

int log_n_with_author(int n, char author[])
{
    char name[1000];
    sprintf(name, "commit%d.%d", n, current_branch_ID);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, name);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000], line[1000];
    fgets(line, 1000, info);
    sscanf(line, "%d", &ID);
    fgets(line, 1000, info);
    strcpy(message, line);
    if (*(message + strlen(message) - 1) == '\n')
        *(message + strlen(message) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(username, line);
    if (*(username + strlen(username) - 1) == '\n')
        *(username + strlen(username) - 1) = '\0';
    if (strcmp(username, author) != 0)
        return 1;
    fgets(line, 1000, info);
    strcpy(time, line);
    if (*(time + strlen(time) - 1) == '\n')
        *(time + strlen(time) - 1) = '\0';
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    printf("'''''''''''''''''''''''''''''''''''\n");
    printf("Succsessful!\n\n");
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("Commited files/folders count --> %d\n\n", count);
    printf("'''''''''''''''''''''''''''''''''''");
    return 0;
}

int compare_time(char time[][1000], char given_time[][1000])
{
    // year
    if (str_to_int(time[3]) > str_to_int(given_time[3]))
        return 1;
    else if (str_to_int(time[3]) < str_to_int(given_time[3]))
        return 0;
    // month
    // day
    if (str_to_int(time[1]) > str_to_int(given_time[1]))
        return 1;
    else if (str_to_int(time[1]) < str_to_int(given_time[1]))
        return 0;
    // HH:MM:SS
    int hour, minute, second;
    sscanf(time[2], "%d:%d:%d", &hour, &minute, &second);
    int given_hour, given_minute, given_second;
    sscanf(given_time[2], "%d:%d:%d", &given_hour, &given_minute, &given_second);
    // hour
    if (hour > given_hour)
        return 1;
    else if (hour < given_hour)
        return 0;
    // minute
    if (minute > given_minute)
        return 1;
    else if (minute < given_minute)
        return 0;
    // second
    if (second > given_second)
        return 1;
    else if (second < given_second)
        return 0;
    return 2;
}

int log_n_with_time(int n, char which[], char given_time[])
{
    char name[1000];
    sprintf(name, "commit%d.%d", n, current_branch_ID);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, name);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000], line[1000];
    fgets(line, 1000, info);
    sscanf(line, "%d", &ID);
    fgets(line, 1000, info);
    strcpy(message, line);
    if (*(message + strlen(message) - 1) == '\n')
        *(message + strlen(message) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(username, line);
    if (*(username + strlen(username) - 1) == '\n')
        *(username + strlen(username) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(time, line);
    if (*(time + strlen(time) - 1) == '\n')
        *(time + strlen(time) - 1) = '\0';
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    if (strcmp(which, "-since") == 0)
    {
        char septime[4][1000];
        char sepgiventime[4][1000];
        separator(time, septime);
        separator(given_time, sepgiventime);
        if (compare_time(septime, sepgiventime) != 1)
            return 1;
    }
    if (strcmp(which, "-before") == 0)
    {
        char septime[4][1000];
        char sepgiventime[4][1000];
        separator(time, septime);
        separator(given_time, sepgiventime);
        if (compare_time(septime, sepgiventime) != 0)
            return 1;
    }
    printf("'''''''''''''''''''''''''''''''''''\n");
    printf("Succsessful!\n\n");
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("Commited files/folders count --> %d\n\n", count);
    printf("'''''''''''''''''''''''''''''''''''");
    return 0;
}

int log_n_with_search(int n, char word[])
{
    char name[1000];
    sprintf(name, "commit%d.%d", n, current_branch_ID);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, name);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000], line[1000];
    fgets(line, 1000, info);
    sscanf(line, "%d", &ID);
    fgets(line, 1000, info);
    strcpy(message, line);
    if (*(message + strlen(message) - 1) == '\n')
        *(message + strlen(message) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(username, line);
    if (*(username + strlen(username) - 1) == '\n')
        *(username + strlen(username) - 1) = '\0';
    fgets(line, 1000, info);
    strcpy(time, line);
    if (*(time + strlen(time) - 1) == '\n')
        *(time + strlen(time) - 1) = '\0';
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    if (word_exists(word, message) == 0)
        return 1;
    printf("'''''''''''''''''''''''''''''''''''\n");
    printf("Succsessful!\n\n");
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("Commited files/folders count --> %d\n\n", count);
    printf("'''''''''''''''''''''''''''''''''''");
    return 0;
}

int make_branch(char name[])
{
    char org_commit[1000];
    int n = last_commit(org_commit);
    printf("(%s)", org_commit);
    char cop_folder[1000];
    strcpy(cop_folder, org_commit);
    back_one(cop_folder);
    int m = new_branch_number();
    char commit_name[1000];
    sprintf(commit_name, "commit%d.%d", n, m);
    copy_folder(commit_name, org_commit, cop_folder);
    char branch_file[1000];
    sprintf(branch_file, "branch%d.txt", m);
    forward_one(cop_folder, branch_file);
    char copy_info[1000];
    strcpy(copy_info, org_commit);
    char info_folder[1000];
    strcpy(info_folder, cop_folder);
    back_one(info_folder);
    forward_one(info_folder, commit_name);
    forward_one(copy_info, "commitInfo.txt");
    copy_file("commitInfo.txt", copy_info, info_folder, 'y');
    FILE *br = fopen(cop_folder, "w");
    fprintf(br, "%s\ncommit%d.%d\n", name, n, m);
    fclose(br);
    printf("A new branch with the name \"%s\" was created\n", name);
    return 0;
}

int check_if_head() {}

int copy_n_commit_to_repo(char commit_folder_address[])
{
    char dot_son[1000];
    repoExists(dot_son);
    char repo_address[1000];
    strcpy(repo_address, dot_son);
    back_one(repo_address);
    DIR *repo = opendir(repo_address);
    struct dirent *dir;
    while ((dir = readdir(repo)) != NULL)
    {
        if (strcmp(dir->d_name, ".son") == 0 || strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        char f_name[1000];
        strcpy(f_name, repo_address);
        forward_one(f_name, dir->d_name);
        remove(f_name);
    }
    DIR *commit_folder = opendir(commit_folder_address);
    while ((dir = readdir(commit_folder)) != NULL)
    {
        if (strcmp(dir->d_name, "commitInfo.txt") == 0 || strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        char f_address[1000];
        strcpy(f_address, commit_folder_address);
        forward_one(f_address, dir->d_name);
        if (dir->d_type == 0)
            copy_file(dir->d_name, f_address, repo_address, 'a');
        else
            copy_folder(dir->d_name, f_address, repo_address);
    }
    return 0;
}

int address_converter(char address[])
{
    char cwd[1000];
    getcwd(cwd, 1000);
    int count = count_words_slash(address);
    char sep[count][1000];
    slash_separator(address, sep);
    for (int i = 0; i < count; i++)
    {
        if (strcmp(sep[i], ".") == 0)
        {
            char wd[1000];
            getcwd(wd, 1000);
            strcpy(sep[i], wd);
        }
        if (strcmp(sep[i], "..") == 0)
        {
            chdir("..");
            char wd[1000];
            getcwd(wd, 1000);
            strcpy(sep[i], wd);
        }
    }
    chdir(cwd);
    char new_address[1000];
    strcpy(new_address, sep[0]);
    for (int i = 1; i < count; i++)
    {
        forward_one(new_address, sep[i]);
    }
    if (*new_address != 'c' && *new_address != 'd' && *new_address != 'C' && *new_address != 'D')
    {
        strcpy(new_address, cwd);
        for (int i = 0; i < count; i++)
        {
            forward_one(new_address, sep[i]);
        }
    }
    strcpy(address, new_address);
    return 0;
}

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
    // alias read
    alias_read(argc, argv);
    // alias replace
    argc = alias_replace(argc, argv);
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
    // curr_branch
    char dot[1000];
    repoExists(dot);
    forward_one(dot, "curr_branch.txt");
    FILE *c_br = fopen(dot, "r");
    fgets(current_branch, 1000, c_br);
    sscanf(current_branch, "branch%d", &current_branch_ID);
    fclose(c_br);
    // curr_commit
    repoExists(dot);
    forward_one(dot, "curr_commit.txt");
    FILE *c_cm = fopen(dot, "r");
    fgets(current_commit, 1000, c_cm);
    fclose(c_cm);
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
        int count = 0;
        char type;
        char org_f_address[1000];
        char cwd[1000];
        getcwd(cwd, 1000);
        char temp[1000];
        strcpy(temp, repo_address);
        forward_one(temp, "added.txt");
        FILE *added = fopen(temp, "a");
        if (strcmp(argv[2], "-n") == 0)
        {
            DIR *curr = opendir(".");
            struct dirent *dir;
            while ((dir = readdir(curr)) != NULL)
            {
                if (strcmp(dir->d_name, ".son") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0)
                    continue;
                strcpy(org_f_address, cwd);
                forward_one(org_f_address, dir->d_name);
                char name[1000];
                strcpy(name, org_f_address);
                last_maker(name);
                if (dir->d_type == 16)
                {
                    int stg = check_if_staged_or_not('F', org_f_address);
                    char state[1000];
                    if (stg == 1)
                        strcpy(state, "--> staged");
                    else
                        strcpy(state, "--> Not staged");
                    printf("\n    1 %s %s", name, state);
                    int max_depth;
                    sscanf(argv[3], "%d", &max_depth);
                    added_n(org_f_address, 2, max_depth);
                }
                else
                {
                    int stg = check_if_staged_or_not('f', org_f_address);
                    char werid_temp[1000];
                    sprintf(werid_temp, "1 %s", org_f_address);
                    int add = line_exists(name, werid_temp);
                    char state[1000];
                    if (stg == 1)
                        strcpy(state, "--> staged");
                    else
                        strcpy(state, "--> Not staged");
                    printf("\n    1 %s %s", name, state);
                }
            }
            return 0;
        }
        else if (strcmp(argv[2], "-f") == 0)
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
                        char temp_line[1000];
                        sprintf(temp_line, "1 %s", org_f_address);
                        char cur_commit_address[1000];
                        strcpy(cur_commit_address, org_f_address);
                        repo_to_commit(cur_commit_address, 'c');
                        back_one(cur_commit_address);
                        if (type == 'F')
                        {
                            copy_folder(dir->d_name, org_f_address, cur_commit_address);
                            if (line_exists(temp, temp_line) == 0)
                                fprintf(added, "%s\n", org_f_address);
                            append_added(added, org_f_address);
                        }
                        else if (type == 'f')
                        {
                            copy_file(dir->d_name, org_f_address, cur_commit_address, 'a');
                            if (line_exists(temp, temp_line) == 0)
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
            printf("%d items were succsessfully added to staging area.\n", argc - 3 - count);
        }
        else if (strcmp(argv[2], "-redo") == 0)
        {
            fclose(added);
            FILE *a = fopen(temp, "r");
            char address[1000];
            while (fgets(address, 1000, a) != NULL)
            {
                if (*(address + strlen(address) - 1) == '\n')
                    *(address + strlen(address) - 1) = '\0';
                char temp_up_address[1000];
                char name[1000];
                strcpy(temp_up_address, address);
                back_one(temp_up_address);
                strcpy(name, address);
                last_maker(name);
                char type = type_of(name, temp_up_address);
                int stg;
                if (type == 'F')
                    stg = check_if_staged_or_not('F', address);
                else if (type == 'f')
                    stg = check_if_staged_or_not('f', address);
                if (stg != 1)
                {
                    char org[1000];
                    strcpy(org, address);
                    repo_to_commit(org, 'c');
                    char file_name[1000];
                    strcpy(file_name, org);
                    last_maker(file_name);
                    back_one(address);
                    if (type == 'f')
                        copy_file(file_name, org, address, 'a');
                    else if (type == 'F')
                        copy_folder(file_name, org, address);
                }
            }
            fclose(a);
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
                    char temp_line[1000];
                    sprintf(temp_line, "1 %s", org_f_address);
                    char cur_commit_address[1000];
                    strcpy(cur_commit_address, org_f_address);
                    repo_to_commit(cur_commit_address, 'c');
                    back_one(cur_commit_address);
                    if (type == 'F')
                    {
                        copy_folder(dir->d_name, org_f_address, cur_commit_address);
                        if (line_exists(temp, temp_line) == 0)
                            fprintf(added, "%s\n", org_f_address);
                        append_added(added, org_f_address);
                    }
                    else if (type == 'f')
                    {
                        copy_file(dir->d_name, org_f_address, cur_commit_address, 'a');
                        if (line_exists(temp, temp_line) == 0)
                            fprintf(added, "%s\n", org_f_address);
                    }
                }
            }
            if (flag == 0)
            {
                printf("%s doesn't seem to exist!\n", argv[2]);
            }
            else
                printf("%s was succsessfully added to staging area.\n", argv[2]);
        }
        fclose(added);
        return 0;
    }
    else if (strcmp(argv[1], "status") == 0)
    {
        char cwd[1000];
        getcwd(cwd, 1000);
        char added_address[1000];
        strcpy(added_address, repo_address);
        forward_one(added_address, "added.txt");
        DIR *currDir = opendir(cwd);
        struct dirent *dir;
        char MAD;
        char file_name[1000];
        char org_f_address[1000];
        int stg, added, delete;
        int M, A, D;
        char org_to_commit[1000];
        while ((dir = readdir(currDir)) != NULL)
        {
            if (dir->d_type != 0 || strcmp(dir->d_name, "commitInfo.txt") == 0)
                continue;
            strcpy(org_f_address, cwd);
            forward_one(org_f_address, dir->d_name);
            added = line_exists(added_address, org_f_address);
            if (added == 0)
                continue;
            strcpy(org_to_commit, org_f_address);
            repo_to_commit(org_to_commit, 'c');
            int check = file_changed(org_f_address, org_to_commit);
            if (check == 1)
            {
                printf("    %s --> -M\n", dir->d_name);
                continue;
            }
            if (check == 3)
            {
                printf("    %s --> -A\n", dir->d_name);
                continue;
            }
            if (check == 0)
            {
                strcpy(org_to_commit, org_f_address);
                repo_to_commit(org_to_commit, 'l');
                check = file_changed(org_f_address, org_to_commit);
                if (check == 0)
                {
                    printf("    %s --> +0\n", dir->d_name);
                    continue;
                }
                else if (check == 1)
                {
                    printf("    %s --> +M\n", dir->d_name);
                    continue;
                }
                char last_commit_dir[1000];
                strcpy(last_commit_dir, org_to_commit);
                back_one(last_commit_dir);
                added = alreadyExists(dir->d_name, last_commit_dir);
                if (added == 0)
                {
                    printf("    %s --> +A\n", dir->d_name);
                    continue;
                }
            }
        }
        strcpy(org_to_commit, org_f_address);
        repo_to_commit(org_to_commit, 'c');
        char last_commit_dir[1000];
        strcpy(last_commit_dir, org_to_commit);
        back_one(last_commit_dir);
        DIR *commit = opendir(last_commit_dir);
        struct dirent *commit_dir;
        while ((commit_dir = readdir(commit)) != NULL)
        {
            if (commit_dir->d_type != 0 || strcmp(dir->d_name, "commitInfo.txt") == 0)
                continue;
            char comtorepo[1000];
            strcpy(comtorepo, last_commit_dir);
            commit_to_repo(comtorepo);
            delete = alreadyExists(commit_dir->d_name, comtorepo);
            if (delete == 0)
            {
                printf("    %s --> -D\n", commit_dir->d_name);
                continue;
            }
        }
        strcpy(org_to_commit, org_f_address);
        repo_to_commit(org_to_commit, 'l');
        strcpy(last_commit_dir, org_to_commit);
        back_one(last_commit_dir);
        DIR *commitp = opendir(last_commit_dir);
        struct dirent *commit_dirp;
        while ((commit_dirp = readdir(commitp)) != NULL)
        {
            if (commit_dirp->d_type != 0 || strcmp(dir->d_name, "commitInfo.txt") == 0)
                continue;
            char comtorepo[1000];
            strcpy(comtorepo, last_commit_dir);
            commit_to_repo(comtorepo);
            delete = alreadyExists(commit_dirp->d_name, comtorepo);
            if (delete == 0)
            {
                printf("    %s --> +D\n", commit_dirp->d_name);
                continue;
            }
        }
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        if (strcmp(argv[2], "-s") == 0)
        {
            char message[1000];
            strcpy(message, argv[3]);
            if (shortcut_message(message) == 1)
            {
                printf("\"%s\" doesn't exist!\n", argv[3]);
                exit(1);
            }
            argv[3] = (char *)malloc(1000);
            strcpy(argv[3], message);
        }
        if (strlen(argv[3]) > 72)
        {
            printf("Commit message is too long! Try again with a message shorter than 73 characters.\n");
            return 1;
        }
        if (argc == 3)
        {
            printf("Commit message is empty!\n");
            return 1;
        }
        commitfunc(argv[3]);
        return 0;
    }
    else if (strcmp(argv[1], "set") == 0)
    {
        if (argc != 6)
        {
            printf("The last arg(shortcut-name) is empty!\n");
            return 1;
        }
        char line[1000];
        strcpy(line, argv[5]);
        strcpy(line + strlen(line), " ");
        strcpy(line + strlen(line), argv[3]);
        char address[1000];
        repoExists(address);
        forward_one(address, "shortcut.txt");
        if (line_exists(address, line) == 1)
        {
            printf("Shortcut already exists! Use the 'replace' command to change it.\n");
            exit(1);
        }
        shortcut_write(argc, argv);
        return 0;
    }
    else if (strcmp(argv[1], "replace") == 0)
    {
        if (shortcut_replace(argc, argv) == 1)
        {
            printf("\"%s\" doesn't exist!\n", argv[5]);
            exit(1);
        }
        printf("\"%s\" was replaced and now is a shortcut for \"%s\".\n", argv[5], argv[3]);
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        if (shortcut_remove(argc, argv) == 1)
        {
            printf("\"%s\" doesn't exist!\n", argv[3]);
            exit(1);
        }
        printf("\"%s\" was successfully removed!\n", argv[3]);
    }
    else if (strcmp(argv[1], "log") == 0)
    {
        char junk[1000];
        int n = cur_commit(junk);
        if (argc > 2 && strcmp(argv[2], "-n") == 0)
        {
            int depth;
            sscanf(argv[3], "%d", &depth);
            for (int i = n - 1; i > n - depth - 1; i--)
                log_n(i);
        }
        else if (argc > 3 && strcmp(argv[2], "-author") == 0)
        {
            for (int i = n - 1; i > 0; i--)
                log_n_with_author(i, argv[3]);
        }
        else if (argc > 6 && (strcmp(argv[2], "-since") == 0 || strcmp(argv[2], "-before") == 0))
        {
            char given_time[1000];
            strcpy(given_time, argv[3]);
            strcat(given_time, " ");
            strcat(given_time, argv[4]);
            strcat(given_time, " ");
            strcat(given_time, argv[5]);
            strcat(given_time, " ");
            strcat(given_time, argv[6]);
            for (int i = n - 1; i > 0; i--)
                log_n_with_time(i, argv[2], given_time);
        }
        else if (argc > 3 && strcmp(argv[2], "-search") == 0)
        {
            for (int i = n - 1; i > 0; i--)
                log_n_with_search(i, argv[3]);
        }
        else
            for (int i = n - 1; i > 0; i--)
                log_n(i);
    }
    else if (strcmp(argv[1], "branch") == 0)
    {
        if (argc == 3)
        {
            make_branch(argv[2]);
            return 0;
        }
        else
        {
            printf("\nbranch list:\n\n");
            char dot_son[1000];
            repoExists(dot_son);
            DIR *son = opendir(dot_son);
            struct dirent *dir;
            while ((dir = readdir(son)) != NULL)
            {
                if (strstr(dir->d_name, "branch") != NULL && dir->d_name[0] == 'b')
                {
                    char address[1000];
                    strcpy(address, dot_son);
                    forward_one(address, dir->d_name);
                    FILE *br = fopen(address, "r");
                    char br_name[1000];
                    fgets(br_name, 1000, br);
                    if (*(br_name + strlen(br_name) - 1) == '\n')
                        *(br_name + strlen(br_name) - 1) = '\0';
                    printf("    --> %s\n", br_name);
                }
            }
            return 0;
        }
    }
    else if (strcmp(argv[1], "checkout") == 0)
    {
        char added_file[1000];
        repoExists(added_file);
        forward_one(added_file, "added.txt");
        FILE *added = fopen(added_file, "r");
        char line[1000];
        int flag = 0;
        while (fgets(line, 1000, added) != NULL)
        {
            flag = 1;
            if (strcmp(line, "\n") == 0)
            {
                flag = 0;
                break;
            }
        }
        if (flag == 0)
        {
            int ID, branch, th;
            char commit_folder_name[1000];
            if (argv[2][0] == '1')
            {
                sscanf(argv[2], "%d", &ID);
                if (commit_ID_to_number_dot_number(ID, commit_folder_name) == 0)
                    sscanf(commit_folder_name, "commit%d.%d", &th, &branch);
                else
                {
                    printf("No commit exists with the given ID!\n");
                    return 1;
                }
            }
            else
            {
                if (branch_name_to_number_dot_number(argv[2], commit_folder_name) == 0)
                    sscanf(commit_folder_name, "commit%d.%d", &th, &branch);
                else
                {
                    printf("Branch doesn't exist!\n");
                    return 1;
                }
            }
            char current_branch_address[1000];
            repoExists(current_branch_address);
            forward_one(current_branch_address, "curr_branch.txt");
            FILE *cur_br = fopen(current_branch_address, "w");
            fprintf(cur_br, "branch%d.txt", branch);
            fclose(cur_br);

            char current_commit_address[1000];
            repoExists(current_commit_address);
            forward_one(current_commit_address, "curr_commit.txt");
            FILE *cur_cm = fopen(current_commit_address, "w");
            fprintf(cur_cm, "commit%d.%d", th + 1, branch);
            fclose(cur_cm);

            char commit_folder_address[1000];
            repoExists(commit_folder_address);
            forward_one(commit_folder_address, commit_folder_name);
            copy_n_commit_to_repo(commit_folder_address);
            printf("Successfully checked out!\n");
            return 0;
        }
        printf("There's changes that have to be commited first!\n");
    }
}