#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> //only for coloring

int printf_color(char str[], char color)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color == 'r')
        SetConsoleTextAttribute(hConsole, 4);
    else if (color == 'b')
        SetConsoleTextAttribute(hConsole, 3);
    else if (color == 'y')
        SetConsoleTextAttribute(hConsole, 6);
    else if (color == 'k')
        SetConsoleTextAttribute(hConsole, 8);
    else if (color == 'p')
        SetConsoleTextAttribute(hConsole, 5);
    else if (color == 'g')
        SetConsoleTextAttribute(hConsole, 2);
    else if (color == 'R')
        SetConsoleTextAttribute(hConsole, 64);
    else if (color == 'B')
        SetConsoleTextAttribute(hConsole, 48);
    else if (color == 'Y')
        SetConsoleTextAttribute(hConsole, 96);
    else if (color == 'G')
        SetConsoleTextAttribute(hConsole, 32);
    else if (color == 'K')
        SetConsoleTextAttribute(hConsole, 128);
    else if (color == 'P')
        SetConsoleTextAttribute(hConsole, 208);
    else if (color == 'W')
        SetConsoleTextAttribute(hConsole, 240);
    printf("%s", str);
    SetConsoleTextAttribute(hConsole, 7);
    return 0;
}

int change_color(char color)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color == 'r')
        SetConsoleTextAttribute(hConsole, 4);
    else if (color == 'b')
        SetConsoleTextAttribute(hConsole, 3);
    else if (color == 'y')
        SetConsoleTextAttribute(hConsole, 6);
    else if (color == 'k')
        SetConsoleTextAttribute(hConsole, 8);
    else if (color == 'p')
        SetConsoleTextAttribute(hConsole, 5);
    else if (color == 'g')
        SetConsoleTextAttribute(hConsole, 2);
    else if (color == 'R')
        SetConsoleTextAttribute(hConsole, 64);
    else if (color == 'B')
        SetConsoleTextAttribute(hConsole, 48);
    else if (color == 'Y')
        SetConsoleTextAttribute(hConsole, 96);
    else if (color == 'G')
        SetConsoleTextAttribute(hConsole, 32);
    else if (color == 'K')
        SetConsoleTextAttribute(hConsole, 128);
    else if (color == 'P')
        SetConsoleTextAttribute(hConsole, 208);
    else if (color == 'W')
        SetConsoleTextAttribute(hConsole, 240);
    return 0;
}

int reset_color()
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    return 0;
}

char current_branch[1000];
char current_branch_name[1000];
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

int line_separator(char lines[][1000], char file_address[])
{
    FILE *file = fopen(file_address, "r");
    int index = 0;
    while (fgets(lines[index], 1000, file) != NULL)
        index++;
    fclose(file);
    return index;
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
        if (strlen(arg) == 1)
        {
            strcpy(sep[0], arg);
            count++;
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
        struct dirent *dirs;
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
    change_color('G');
    printf("\"%s\"", argv[5]);
    printf_color(" is now a shortcut-message for .", 'g');
    change_color('G');
    printf("\"%s\"", argv[3]);
    reset_color();
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
                    if (k + n - 1 != k)
                    {
                        argv[k + n - 1] = (char *)malloc(1000);
                        strcpy(argv[k + n - 1], argv[k]);
                    }
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
                    if (k + n - 1 != k)
                    {
                        argv[k + n - 1] = (char *)malloc(1000);
                        strcpy(argv[k + n - 1], argv[k]);
                    }
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

char type_of(char name[], char folder_address[])
{
    DIR *currentDir = opendir(folder_address);
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
    if ((strcmp(file_name, "tag.txt") == 0 || strcmp(file_name, "tags.txt") == 0 || strcmp(file_name, "commitInfo.txt") == 0) && check != 'y')
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
            copy_file(dir->d_name, org_f_address, copy_address, 'n');
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
            printf_color("Global username is now set to ", 'p');
            change_color('P');
            printf("\"%s\"\n", argv[4]);
            reset_color();
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
            printf_color("Global email is now set to ", 'p');
            change_color('P');
            printf("\"%s\"\n", argv[4]);
            reset_color();
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
            printf_color("Global username is now set to ", 'p');
            change_color('P');
            printf("\"%s\"\n", argv[3]);
            reset_color();
        }
        if (strcmp(argv[2], "user.email") == 0)
        {
            strcpy(temp + strlen(temp), "\\email.txt");
            FILE *email = fopen(temp, "w");
            fprintf(email, "%s", argv[3]);
            fclose(email);
            printf_color("Global username is now set to ", 'p');
            change_color('P');
            printf("\"%s\"\n", argv[3]);
            reset_color();
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
    if (argc == 3 && strcmp(argv[1], "reset") == 0)
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
    if (argc > 3 && strcmp(argv[1], "revert") == 0)
        return 1;
    if (argc >= 4 && strcmp(argv[1], "tag") == 0 && strcmp(argv[2], "-a") == 0)
        return 1;
    if (argc == 2 && strcmp(argv[1], "tag") == 0)
        return 1;
    if (argc == 4 && strcmp(argv[1], "tag") == 0 && strcmp(argv[2], "show") == 0)
        return 1;
    if (argc >= 5 && strcmp(argv[1], "diff") == 0 && strcmp(argv[2], "-f") == 0)
        return 1;
    if (argc == 5 && strcmp(argv[1], "diff") == 0 && strcmp(argv[2], "-c") == 0)
        return 1;
    if (argc == 5 && strcmp(argv[1], "merge") == 0 && strcmp(argv[2], "-b") == 0)
        return 1;
    return 0;
}

int alreadyExists(char name[], char folder_addres[])
{
    DIR *currentDir = opendir(folder_addres);
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
    char name[1000];
    sprintf(name, "commit%d.%d", n, current_branch_ID);
    forward_one(commitfolder_address, name);
    mkdir(commitfolder_address);
    char name2[1000];
    sprintf(name2, "commit%d.%d", n - 1, current_branch_ID);
    char org_f_address[1000];
    char wd[1000];
    repoExists(wd);
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
    char hide[1000];
    strcpy(hide, "attrib +h ");
    strcat(hide, cwd);
    forward_one(hide, ".son");
    system(hide);
    char master_branch[1000];
    getcwd(master_branch, 1000);
    forward_one(master_branch, ".son");
    forward_one(master_branch, "branch0.txt");
    FILE *master = fopen(master_branch, "w");
    fprintf(master, "master\ncommit0.0\n");
    fclose(master);
    char tags_address[1000];
    getcwd(tags_address, 1000);
    forward_one(tags_address, ".son\\tags.txt");
    FILE *tags = fopen(tags_address, "w");
    fclose(tags);
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
            copy_file(dir->d_name, org_f_address, commit0_address, 'n');
    }
    commit_n_folder(1);
    printf_color("A new repo was succsessfully made.\n", 'G');
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
            {
                change_color('b');
                strcpy(state, "--> staged");
            }
            else
            {
                change_color('p');
                strcpy(state, "--> Not staged");
            }
            char temp_2[1000];
            strcpy(temp_2, temp);
            last_maker(temp_2);
            printf("\n");
            for (int i = 0; i < depth - 1; i++)
                printf("      ");
            printf("|____ %d %s %s", depth, temp_2, state);
            reset_color();
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
        if (t - address >= strlen(address))
        {
            c = strstr(address, ".son");
            *(c - 1) = '\0';
            return 0;
        }
        t++;
    }
    if (*(t + 1) != '\0')
    {
        strcpy(c - 5, t + 1);
    }
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
    FILE *IDs = fopen(dot_son, "r");
    int last;
    char line[1000];
    while (fgets(line, 1000, IDs) != NULL)
        sscanf(line, "%d", &last);
    fclose(IDs);
    return last + 1;
}

int delete_line(char line[], char address[])
{
    FILE *file = fopen(address, "r");
    char lines[100][1000];
    strcpy(lines[0], line);
    int index = 0, delete = -1;
    while (fgets(lines[index], 1000, file) != NULL)
    {
        if (*(lines[index] + strlen(lines[index]) - 1) == '\n')
            *(lines[index] + strlen(lines[index]) - 1) = '\0';
        if (strcmp(line, lines[index]) == 0)
            delete = index;
        index++;
    }
    fclose(file);
    if (delete == -1)
        return 1;
    FILE *new_file = fopen(address, "w");
    for (int i = 0; i < index; i++)
        if (i != delete)
            fprintf(new_file, "%s\n", lines[i]);
    fclose(new_file);
    return 0;
}

int append_line_at_top(char line[], char address[])
{
    FILE *file = fopen(address, "r");
    char lines[100][1000];
    strcpy(lines[0], line);
    int index = 1;
    while (fgets(lines[index], 1000, file) != NULL)
        index++;
    fclose(file);
    FILE *new_file = fopen(address, "w");
    for (int i = 0; i < index; i++)
        fprintf(new_file, "%s", lines[i]);
    fclose(new_file);
    return 0;
}

int lower_case(char str[])
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (*(str + i) >= 'A' && *(str + i) <= 'Z')
            *(str + i) += 32;
    }
    return 0;
}

int alphabetic_order(char address[])
{
    FILE *file = fopen(address, "r");
    char lines[100][1000];
    int index = 0;
    while (fgets(lines[index], 1000, file) != NULL)
    {
        if (*(lines[index] + strlen(lines[index]) - 1) == '\n')
            *(lines[index] + strlen(lines[index]) - 1) = '\0';
        lower_case(lines[index]);
        index++;
    }
    fclose(file);
    for (int i = 0; i < index - 1; i++)
    {
        if (strcmp(lines[i], lines[i + 1]) > 0)
        {
            char temp[1000];
            strcpy(temp, lines[i + 1]);
            strcpy(lines[i + 1], lines[i]);
            strcpy(lines[i], temp);
            i = 0;
        }
    }

    FILE *new_file = fopen(address, "w");
    for (int i = 0; i < index; i++)
        fprintf(new_file, "%s\n", lines[i]);
    fclose(new_file);
    return 0;
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
        time_t mytime = time(NULL);
        char *time_str = ctime(&mytime);
        time_str[strlen(time_str) - 1] = '\0';
        strcpy(time_str, time_str + 4);
        char name[1000];
        if (username_read(name) == 1)
            exit(1);
        char commit_info_file[1000];
        int n = cur_commit(commit_info_file);
        commit_n_folder(n + 1);
        forward_one(commit_info_file, "commitInfo.txt");
        FILE *info = fopen(commit_info_file, "w");
        fprintf(info, "%d\n%s\n%s\n%s\n%s\n%d\n%d", commitID, argv3, name, time_str, current_branch_name, current_branch_ID, count);
        printf("'''''''''''''''''''''''''''''''''''\n");
        printf_color("Succsessful!\n\n", 'g');
        change_color('b');
        printf("Commit ID --> %d\n\n", commitID);
        printf("Commit message --> %s\n\n", argv3);
        printf("Username --> %s\n\n", name);
        printf("Time --> %s\n\n", time_str);
        printf("branch --> %s (branchId: %d)\n\n", current_branch_name, current_branch_ID);
        printf("Commited files/folders count --> %d\n\n", count);
        reset_color();
        printf("'''''''''''''''''''''''''''''''''''");
        fclose(info);
        back_one(dot_son);
        forward_one(dot_son, current_branch);
        FILE *c_br = fopen(dot_son, "a");
        fprintf(c_br, "commit%d.%d\n", n, current_branch_ID);
        fclose(c_br);
        back_one(dot_son);
        forward_one(dot_son, "added.txt");
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
        char commit_names_in_order[1000];
        repoExists(commit_names_in_order);
        forward_one(commit_names_in_order, "orderc.txt");
        char line[1000];
        sprintf(line, "commit%d.%d\n", n, current_branch_ID);
        append_line_at_top(line, commit_names_in_order);

        return 0;
    }
    else
    {
        printf("No changes have been made since the last commit.\n");
        exit(1);
    }
}

int branchID_to_branchName(int n, char result[])
{
    char dot_son[1000];
    repoExists(dot_son);
    DIR *son = opendir(dot_son);
    struct dirent *dir;
    while ((dir = readdir(son)) != NULL)
    {
        if (strstr(dir->d_name, "branch") != NULL && dir->d_name[0] != 'c')
        {
            int ID;
            sscanf(dir->d_name, "branch%d.txt", &ID);
            if (n != ID)
                continue;
            char file_address[1000];
            strcpy(file_address, dot_son);
            forward_one(file_address, dir->d_name);
            FILE *branchInfo = fopen(file_address, "r");
            char branch_name[1000];
            fgets(branch_name, 1000, branchInfo);
            if (*(branch_name + strlen(branch_name) - 1) == '\n')
                *(branch_name + strlen(branch_name) - 1) = '\0';
            fclose(branchInfo);
            strcpy(result, branch_name);
            return 0;
        }
    }
}

int log_n(int n)
{
    char orderfile[1000];
    repoExists(orderfile);
    forward_one(orderfile, "orderc.txt");
    FILE *order = fopen(orderfile, "r");
    char line[1000];
    for (int i = 0; i < n; i++)
        fgets(line, 1000, order);
    if (*(line + strlen(line) - 1) == '\n')
        *(line + strlen(line) - 1) = '\0';
    char branch_name[1000];
    int branchID, junk;
    sscanf(line, "commit%d.%d", &junk, &branchID);
    branchID_to_branchName(branchID, branch_name);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, line);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000];
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
    fgets(line, 1000, info);
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    printf("'''''''''''''''''''''''''''''''''''\n");
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("branch --> %s (branchID: %d)\n\n", branch_name, branchID);
    printf("Commited files/folders count --> %d\n\n", count);
    printf("'''''''''''''''''''''''''''''''''''");
}

int log_n_with_author(int n, char author[])
{
    char orderfile[1000];
    repoExists(orderfile);
    forward_one(orderfile, "orderc.txt");
    FILE *order = fopen(orderfile, "r");
    char line[1000];
    for (int i = 0; i < n; i++)
        fgets(line, 1000, order);
    if (*(line + strlen(line) - 1) == '\n')
        *(line + strlen(line) - 1) = '\0';
    char branch_name[1000];
    int branchID, junk;
    sscanf(line, "commit%d.%d", &junk, &branchID);
    branchID_to_branchName(branchID, branch_name);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, line);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000];
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
    fgets(line, 1000, info);
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    if (strcmp(username, author) != 0)
        return 1;
    printf("'''''''''''''''''''''''''''''''''''\n");
    printf("Succsessful!\n\n");
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("Branch --> %s (branchID: %d)\n\n", branch_name, branchID);
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
    char orderfile[1000];
    repoExists(orderfile);
    forward_one(orderfile, "orderc.txt");
    FILE *order = fopen(orderfile, "r");
    char line[1000];
    for (int i = 0; i < n; i++)
        fgets(line, 1000, order);
    if (*(line + strlen(line) - 1) == '\n')
        *(line + strlen(line) - 1) = '\0';
    char branch_name[1000];
    int branchID, junk;
    sscanf(line, "commit%d.%d", &junk, &branchID);
    branchID_to_branchName(branchID, branch_name);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, line);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000];
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
    fgets(line, 1000, info);
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
    printf("Branch --> %s (branchID: %d)\n\n", branch_name, branchID);
    printf("Commited files/folders count --> %d\n\n", count);
    printf("'''''''''''''''''''''''''''''''''''");
    return 0;
}

int log_n_with_search(int n, char word[])
{
    char orderfile[1000];
    repoExists(orderfile);
    forward_one(orderfile, "orderc.txt");
    FILE *order = fopen(orderfile, "r");
    char line[1000];
    for (int i = 0; i < n; i++)
        fgets(line, 1000, order);
    if (*(line + strlen(line) - 1) == '\n')
        *(line + strlen(line) - 1) = '\0';
    char branch_name[1000];
    int branchID, junk;
    sscanf(line, "commit%d.%d", &junk, &branchID);
    branchID_to_branchName(branchID, branch_name);
    char info_address[1000];
    repoExists(info_address);
    forward_one(info_address, line);
    forward_one(info_address, "commitInfo.txt");
    FILE *info = fopen(info_address, "r");
    int ID, count;
    char message[1000], username[1000], time[1000];
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
    fgets(line, 1000, info);
    fgets(line, 1000, info);
    sscanf(line, "%d", &count);
    if (word_exists(word, message) == 0)
        return 1;
    printf("'''''''''''''''''''''''''''''''''''\n");
    change_color('b');
    printf("Commit ID --> %d\n\n", ID);
    printf("Commit message --> %s\n\n", message);
    printf("Username --> %s\n\n", username);
    printf("Time --> %s\n\n", time);
    printf("Branch --> %s (branchID: %d)\n\n", branch_name, branchID);
    printf("Commited files/folders count --> %d\n\n", count);
    reset_color();
    printf("'''''''''''''''''''''''''''''''''''");
    return 0;
}

int make_branch(char name[])
{
    char org_commit[1000];
    int n = last_commit(org_commit);
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
    //
    char commitfolder_address[1000];
    repoExists(commitfolder_address);
    char name1[1000];
    sprintf(name1, "commit%d.%d", n + 1, m);
    forward_one(commitfolder_address, name1);
    mkdir(commitfolder_address);
    char name2[1000];
    sprintf(name2, "commit%d.%d", n, m);
    char org_f_address[1000];
    char wd[1000];
    repoExists(wd);
    strcpy(org_f_address, wd);
    forward_one(org_f_address, name2);
    copy_folder(name1, org_f_address, wd);
    //
    printf_color("A new branch with the name ", 'g');
    change_color('G');
    printf("\"%s\"", name);
    reset_color();
    printf_color(" was created!\n", 'g');
    return 0;
}

int find_head(int branchID, char address[])
{
    char dot_son[1000];
    repoExists(dot_son);
    char br_name[1000];
    sprintf(br_name, "branch%d.txt", branchID);
    char br_address[1000];
    strcpy(br_address, dot_son);
    forward_one(br_address, br_name);
    char line[1000];
    FILE *br = fopen(br_address, "r");
    while (fgets(line, 1000, br) != NULL)
        if (*(line + strlen(line) - 1) == '\n')
            *(line + strlen(line) - 1) = '\0';
    strcpy(address, line);
    int max;
    sscanf(address, "commit%d.%d", &max, &branchID);
    return max;
}

int check_if_head()
{
    char cr_cm_address[1000];
    repoExists(cr_cm_address);
    forward_one(cr_cm_address, "curr_commit.txt");
    FILE *cr_cm = fopen(cr_cm_address, "r");
    char cur_commit[1000];
    fgets(cur_commit, 1000, cr_cm);
    if (*(cur_commit + strlen(cur_commit) - 1) == '\n')
        *(cur_commit + strlen(cur_commit) - 1) = '\0';
    int n, branchID;
    sscanf(cur_commit, "commit%d.%d", &n, &branchID);
    char junk[1000];
    int head = find_head(branchID, junk);
    printf("(%d - %d)", n, head + 1);
    if (n == head + 1)
        return 1;
    else
        return 0;
}

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
            copy_file(dir->d_name, f_address, repo_address, 'n');
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
    if (count > 1 || *address != '.')
    {
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
        if (strstr(new_address, "c:\\") == NULL && strstr(new_address, "D:\\") == NULL && strstr(new_address, "C:\\") == NULL || strstr(new_address, "D:\\") != NULL)
        {
            strcpy(new_address, cwd);
            for (int i = 0; i < count; i++)
            {
                forward_one(new_address, sep[i]);
            }
        }
        strcpy(address, new_address);
        if (*(address + strlen(address) - 1) == '\\')
            *(address + strlen(address) - 1) = '\0';
    }
    if (*(address) == '.' && *(address + 1) == '\\' && strlen(address) == 2)
        strcpy(address, "current");
    return 0;
}

int remove_folder_files(char folder_address[])
{
    DIR *folder = opendir(folder_address);
    struct dirent *dir;
    while ((dir = readdir(folder)) != NULL)
    {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        if (dir->d_type == 0)
        {
            char file_address[1000];
            strcpy(file_address, folder_address);
            forward_one(file_address, dir->d_name);
            remove(file_address);
        }
        else
        {
            char folder_in_folder_address[1000];
            strcpy(folder_in_folder_address, folder_address);
            forward_one(folder_in_folder_address, dir->d_name);
            remove_folder_files(folder_in_folder_address);
        }
    }
    return 0;
}

int tagfunc(int argc, char *argv[])
{
    char tag[1000];
    strcpy(tag, argv[3]);
    char tags_file_address[1000];
    repoExists(tags_file_address);
    forward_one(tags_file_address, "tags.txt");
    if (line_exists(tags_file_address, argv[3]) == 1)
    {
        int flag = 0;
        for (int i = 3; i < argc; i++)
        {
            if (strcmp(argv[i], "-f") == 0)
                flag = 1;
        }
        if (flag == 0)
        {
            printf("Tag already exists!\nIn order to overwrite use the '-f' argument\n");
            exit(1);
        }
        else
        {
            char dot_son[1000];
            repoExists(dot_son);
            DIR *son = opendir(dot_son);
            struct dirent *dir;
            while ((dir = readdir(son)) != NULL)
            {
                if (strstr(dir->d_name, "commit") != NULL && dir->d_type == 16)
                {
                    char tag_file_address[1000];
                    strcpy(tag_file_address, dot_son);
                    forward_one(tag_file_address, dir->d_name);
                    if (alreadyExists("tag.txt", tag_file_address) == 0)
                        continue;
                    forward_one(tag_file_address, "tag.txt");
                    FILE *tag_file = fopen(tag_file_address, "r");
                    char line[1000];
                    fgets(line, 1000, tag_file);
                    fclose(tag_file);
                    if (*(line + strlen(line) - 1) == '\n')
                        *(line + strlen(line) - 1) = '\0';
                    if (strcmp(tag, line) == 0)
                    {
                        remove(tag_file_address);
                        break;
                    }
                }
            }
            char tags_file_address[1000];
            repoExists(tags_file_address);
            forward_one(tags_file_address, "tags.txt");
            delete_line(tag, tags_file_address);
        }
    }

    char message[1000];
    int commitID, c_exists, m_exists;

    for (int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            sscanf(argv[i + 1], "%d", &commitID);
            c_exists = 1;
        }
        if (strcmp(argv[i], "-m") == 0)
        {
            strcpy(message, argv[i + 1]);
            m_exists = 1;
        }
    }

    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = '\0';
    strcpy(time_str, time_str + 4);
    FILE *tag_file;
    char commit_name[1000];
    char username[1000];
    username_read(username);

    if (c_exists == 1)
    {
        commit_ID_to_number_dot_number(commitID, commit_name);
        char tag_file_address[1000];
        repoExists(tag_file_address);
        forward_one(tag_file_address, commit_name);
        forward_one(tag_file_address, "tag.txt");
        tag_file = fopen(tag_file_address, "w");
        fprintf(tag_file, "%s\n", tag);
        fprintf(tag_file, "%d\n", commitID);
        fprintf(tag_file, "%s\n", username);
        fprintf(tag_file, "%s\n", time_str);
        if (m_exists == 1)
            fprintf(tag_file, "%s\n", message);
        else
            fprintf(tag_file, "(null)");
    }
    else
    {
        char commit_address[1000];
        last_commit(commit_address);
        strcpy(commit_name, commit_address);
        char commit_info_address[1000];
        strcpy(commit_info_address, commit_address);
        forward_one(commit_info_address, "commitInfo.txt");
        FILE *commitInfo = fopen(commit_info_address, "r");
        char line[1000];
        fgets(line, 1000, commitInfo);
        fclose(commitInfo);
        sscanf(line, "%d", &commitID);

        char tag_file_address[1000];
        strcpy(tag_file_address, commit_address);
        forward_one(tag_file_address, "tag.txt");
        tag_file = fopen(tag_file_address, "w");
        fprintf(tag_file, "%s\n", tag);
        fprintf(tag_file, "%d\n", commitID);
        fprintf(tag_file, "%s\n", username);
        fprintf(tag_file, "%s\n", time_str);
        if (m_exists == 1)
            fprintf(tag_file, "%s\n", message);
        else
            fprintf(tag_file, "(null)");
    }
    fclose(tag_file);
    printf("Successfully done!\n");
    strcat(tag, "\n");
    append_line_at_top(tag, tags_file_address);
}

int show_tag(char tag[])
{
    char dot_son[1000];
    repoExists(dot_son);
    DIR *son = opendir(dot_son);
    struct dirent *dir;
    while ((dir = readdir(son)) != NULL)
    {
        if (strstr(dir->d_name, "commit") != NULL && dir->d_type == 16)
        {
            char tag_file_address[1000];
            strcpy(tag_file_address, dot_son);
            forward_one(tag_file_address, dir->d_name);
            if (alreadyExists("tag.txt", tag_file_address) == 0)
                continue;
            forward_one(tag_file_address, "tag.txt");
            FILE *tag_file = fopen(tag_file_address, "r");
            char line[1000];
            fgets(line, 1000, tag_file);
            if (*(line + strlen(line) - 1) == '\n')
                *(line + strlen(line) - 1) = '\0';
            if (strcmp(tag, line) == 0)
            {
                char commitID[1000], username[1000], time_str[1000], message[1000];
                fgets(commitID, 1000, tag_file);
                if (*(commitID + strlen(commitID) - 1) == '\n')
                    *(commitID + strlen(commitID) - 1) = '\0';
                fgets(username, 1000, tag_file);
                if (*(username + strlen(username) - 1) == '\n')
                    *(username + strlen(username) - 1) = '\0';
                fgets(time_str, 1000, tag_file);
                if (*(time_str + strlen(time_str) - 1) == '\n')
                    *(time_str + strlen(time_str) - 1) = '\0';
                fgets(message, 1000, tag_file);
                if (*(message + strlen(message) - 1) == '\n')
                    *(message + strlen(message) - 1) = '\0';
                printf("=============================================\n");
                printf("Tag --> %s\n\n", tag);
                printf("CommitID --> %s\n\n", commitID);
                printf("Username --> %s\n\n", username);
                printf("Time --> %s\n\n", time_str);
                if (strcmp(message, "(null)") == 0)
                    printf("Message --> Not specified by the user!\n\n");
                else
                    printf("Message --> %s\n\n", message);
                printf("=============================================\n");
            }
        }
    }
}

int null_space(char str[])
{
    for (int i = 0; i < strlen(str); i++)
        if (*(str + i) != ' ' && *(str + i) != '\n')
            return 0;
    return 1;
}

int delete_null(char lines[][1000], int start, int end)
{
    int count = 0;
    for (int i = start - 1; i < end; i++)
    {
        if (null_space(lines[i]) == 0)
        {
            strcpy(lines[count], lines[i]);
            count++;
        }
    }
    return count;
}

int line_number(char line[], char address[])
{
    FILE *file = fopen(address, "r");
    char file_line[1000];
    int count = 0;
    while (fgets(file_line, 1000, file) != NULL)
    {
        count++;
        if (*(file_line + strlen(file_line) - 1) == '\n')
            if (null_space(file_line) == 0)
                *(file_line + strlen(file_line) - 1) = '\0';
        if (strcmp(line, file_line) == 0)
            return count;
    }
    return -1;
}

int diff(char file1[], char file2[], int start1, int end1, int start2, int end2, char type)
{
    char name1[1000], name2[1000];
    strcpy(name1, file1);
    last_maker(name1);
    strcpy(name2, file2);
    last_maker(name2);
    char lines1[100][1000];
    int count1 = line_separator(lines1, file1);
    if (end1 == -1)
        end1 = count1;
    char lines2[100][1000];
    int count2 = line_separator(lines2, file2);
    if (end2 == -1)
        end2 = count2;
    int t1 = delete_null(lines1, start1, end1);
    int t2 = delete_null(lines2, start2, end2);
    int min = t1;
    int diff_count = 0;
    if (t2 < min)
        min = t2;
    for (int i = 0; i < min; i++)
    {
        if (*(lines1[i] + strlen(lines1[i]) - 1) == '\n')
            *(lines1[i] + strlen(lines1[i]) - 1) = '\0';
        if (*(lines2[i] + strlen(lines2[i]) - 1) == '\n')
            *(lines2[i] + strlen(lines2[i]) - 1) = '\0';
        if (strcmp(lines1[i], lines2[i]) != 0)
        {
            int th1 = line_number(lines1[i], file1);
            int th2 = line_number(lines2[i], file2);
            printf_color("====================================\n", 'b');
            printf("file: %s - line: %d\n", name1, th1);
            printf_color(lines1[i], 'y');
            printf("\n");
            printf("file: %s - line: %d\n", name2, th2);
            printf_color(lines2[i], 'r');
            printf("\n");
            printf_color("====================================\n\n", 'b');
            diff_count++;
            // break;
        }
    }
    if (diff_count == 0)
    {
        if (t1 == t2 && type == 'f')
        {
            printf_color("There is no differences!\n", 'g');
            return 0;
        }
        else if (t1 != t2 && type == 'f')
        {
            printf_color("One of the files has more lines than the other!\n", 'r');
            return 1;
        }
        else if (t1 != t2)
        {
            change_color('r');
            printf("** \"%s\", \"%s\" --> One of the files has more lines than the other!\n", name1, name2);
            reset_color();
            return 1;
        }
    }
    if (type == 'f')
    {
        change_color('y');
        printf("%d differences found!\n", diff_count);
        reset_color();
    }
    return 1;
}

int folder_diff(char folder1_address[], char folder2_address[])
{
    char items_in_folder1[100][1000];
    char items_in_folder2[100][1000];
    char items_type_1[100] = {-1};
    char items_type_2[100] = {-1};
    int count1 = 0, count2 = 0;
    DIR *folder1 = opendir(folder1_address);
    DIR *folder2 = opendir(folder2_address);
    struct dirent *dir1;
    struct dirent *dir2;
    while ((dir1 = readdir(folder1)) != NULL)
    {
        if (strcmp(dir1->d_name, "tag.txt") == 0 || strcmp(dir1->d_name, "commitInfo.txt") == 0 || strcmp(dir1->d_name, ".") == 0 || strcmp(dir1->d_name, "..") == 0)
            continue;
        strcpy(items_in_folder1[count1], dir1->d_name);
        items_type_1[count1] = dir1->d_type;
        count1++;
    }
    while ((dir2 = readdir(folder2)) != NULL)
    {
        if (strcmp(dir2->d_name, "tag.txt") == 0 || strcmp(dir2->d_name, "commitInfo.txt") == 0 || strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0)
            continue;
        strcpy(items_in_folder2[count2], dir2->d_name);
        items_type_2[count2] = dir2->d_type;
        count2++;
    }
    int flag;
    int *found2 = (int *)calloc(count2, sizeof(int));
    for (int i = 0; i < count1; i++)
    {
        flag = 0;
        for (int j = 0; j < count2; j++)
        {
            if (strcmp(items_in_folder1[i], items_in_folder2[j]) == 0)
            {
                *(found2 + j) = 1;
                flag = 1;
                char f1_address[1000];
                char f2_address[1000];
                strcpy(f1_address, folder1_address);
                forward_one(f1_address, items_in_folder1[i]);
                strcpy(f2_address, folder2_address);
                forward_one(f2_address, items_in_folder2[j]);
                if (items_type_1[i] == 16)
                    folder_diff(f1_address, f2_address);
                else if (items_type_1[i] == 0)
                    diff(f1_address, f2_address, 1, -1, 1, -1, 'F');
            }
        }
        if (flag == 0)
        {
            change_color('y');
            printf("** \"%s\" doesn't have a corresponding item!\n", items_in_folder1[i]);
            reset_color();
        }
    }
    for (int i = 0; i < count2; i++)
        if (*(found2 + i) == 0)
        {
            change_color('y');
            printf("** \"%s\" doesn't have a corresponding item!\n", items_in_folder2[i]);
            reset_color();
        }
    free(found2);
    return 0;
}

int merge_diff(char file1[], char file2[], int start1, int end1, int start2, int end2)
{
    char name1[1000], name2[1000];
    strcpy(name1, file1);
    last_maker(name1);
    strcpy(name2, file2);
    last_maker(name2);
    char lines1[100][1000];
    int count1 = line_separator(lines1, file1);
    if (end1 == -1)
        end1 = count1;
    char lines2[100][1000];
    int count2 = line_separator(lines2, file2);
    if (end2 == -1)
        end2 = count2;
    int t1 = delete_null(lines1, start1, end1);
    int t2 = delete_null(lines2, start2, end2);
    int min = t1;
    int diff_count = 0;
    if (t2 < min)
        min = t2;
    for (int i = 0; i < min; i++)
    {
        if (*(lines1[i] + strlen(lines1[i]) - 1) == '\n')
            *(lines1[i] + strlen(lines1[i]) - 1) = '\0';
        if (*(lines2[i] + strlen(lines2[i]) - 1) == '\n')
            *(lines2[i] + strlen(lines2[i]) - 1) = '\0';
        if (strcmp(lines1[i], lines2[i]) != 0)
        {
            int th1 = line_number(lines1[i], file1);
            int th2 = line_number(lines2[i], file2);
            printf_color(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", 'k');
            printf("file: %s - line: %d\n", name1, th1);
            printf_color(lines1[i], 'y');
            printf("\n");
            printf("file: %s - line: %d\n", name2, th2);
            printf_color(lines2[i], 'r');
            printf("\n");
            printf_color("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", 'k');
            diff_count++;
            // break;
        }
    }
    if (diff_count == 0)
    {
        if (t1 == t2)
            return 0;
        else if (t1 != t2)
        {
            printf_color("** ", 'r');
            printf("\"%s\", \"%s\" --> One of the files has more lines than the other!\n", name1, name2);
            return 1;
        }
    }
    return 1;
}

int merge_folder_diff(char folder1_address[], char folder2_address[])
{
    char items_in_folder1[100][1000];
    char items_in_folder2[100][1000];
    char items_type_1[100] = {-1};
    char items_type_2[100] = {-1};
    int count1 = 0, count2 = 0;
    DIR *folder1 = opendir(folder1_address);
    DIR *folder2 = opendir(folder2_address);
    struct dirent *dir1;
    struct dirent *dir2;
    while ((dir1 = readdir(folder1)) != NULL)
    {
        if (strcmp(dir1->d_name, "tag.txt") == 0 || strcmp(dir1->d_name, "commitInfo.txt") == 0 || strcmp(dir1->d_name, ".") == 0 || strcmp(dir1->d_name, "..") == 0)
            continue;
        strcpy(items_in_folder1[count1], dir1->d_name);
        items_type_1[count1] = dir1->d_type;
        count1++;
    }
    while ((dir2 = readdir(folder2)) != NULL)
    {
        if (strcmp(dir2->d_name, "tag.txt") == 0 || strcmp(dir2->d_name, "commitInfo.txt") == 0 || strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0)
            continue;
        strcpy(items_in_folder2[count2], dir2->d_name);
        items_type_2[count2] = dir2->d_type;
        count2++;
    }
    int flag = 0;
    for (int i = 0; i < count1; i++)
    {
        for (int j = 0; j < count2; j++)
        {
            if (strcmp(items_in_folder1[i], items_in_folder2[j]) == 0)
            {
                char f1_address[1000];
                char f2_address[1000];
                strcpy(f1_address, folder1_address);
                forward_one(f1_address, items_in_folder1[i]);
                strcpy(f2_address, folder2_address);
                forward_one(f2_address, items_in_folder2[j]);
                if (items_type_1[i] == 16)
                    merge_folder_diff(f1_address, f2_address);
                else if (items_type_1[i] == 0)
                    if (merge_diff(f1_address, f2_address, 1, -1, 1, -1) == 1)
                        flag = 1;
            }
        }
    }
    if (flag == 1)
        return 1;
    return 0;
}

int reset_file(char name[], char folder[])
{
    char address_in_commit_l[1000];
    strcpy(address_in_commit_l, folder);
    repo_to_commit(address_in_commit_l, 'l');
    forward_one(address_in_commit_l, name);
    char address_in_commit_c[1000];
    strcpy(address_in_commit_c, folder);
    repo_to_commit(address_in_commit_c, 'c');
    copy_file(name, address_in_commit_l, address_in_commit_c, 'n');
    return 0;
}

int reset_folder(char name[], char folder[])
{
    char address[1000];
    strcpy(address, folder);
    forward_one(address, name);
    char address_l[1000];
    char address_c[1000];
    strcpy(address_l, address);
    strcpy(address_c, folder);
    repo_to_commit(address_l, 'l');
    repo_to_commit(address_c, 'c');
    copy_folder(name, address_l, address_c);
    // reset_folder(dir->d_name, address);
    return 0;
}

int branch_exists(char branch[])
{
    char dot_son[1000];
    repoExists(dot_son);
    DIR *son = opendir(dot_son);
    struct dirent *dir;
    while ((dir = readdir(son)) != NULL)
    {
        if (strstr(dir->d_name, "branch") != NULL && dir->d_name[0] != 'c')
        {
            char address[1000];
            strcpy(address, dot_son);
            forward_one(address, dir->d_name);
            FILE *br_file = fopen(address, "r");
            char line[1000];
            fgets(line, 1000, br_file);
            if (*(line + strlen(line) - 1) == '\n')
                *(line + strlen(line) - 1) = '\0';
            if (strcmp(line, branch) == 0)
                return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // diff("c:\\Users\\ASUS\\Desktop\\pcopy.txt", "c:\\Users\\ASUS\\Desktop\\sin.txt", 1, 2, 1, 2);
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
        printf_color("Invalid Input!\n", 'R');
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
    back_one(dot);
    forward_one(dot, current_branch);
    FILE *br_name = fopen(dot, "r");
    fgets(current_branch_name, 1000, br_name);
    fclose(br_name);
    if (*(current_branch_name + strlen(current_branch_name) - 1) == '\n')
        *(current_branch_name + strlen(current_branch_name) - 1) = '\0';
    // curr_commit
    back_one(dot);
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
        char added_address[1000];
        strcpy(added_address, repo_address);
        forward_one(added_address, "added.txt");
        FILE *added = fopen(added_address, "a");
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
                    {
                        change_color('b');
                        strcpy(state, "--> staged");
                    }
                    else
                    {
                        change_color('p');
                        strcpy(state, "--> Not staged");
                    }
                    printf("\n|____ 1 %s %s", name, state);
                    reset_color();
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
                    {
                        change_color('b');
                        strcpy(state, "--> staged");
                    }
                    else
                    {
                        change_color('p');
                        strcpy(state, "--> Not staged");
                    }
                    printf("\n|____ 1 %s %s", name, state);
                    reset_color();
                }
            }
            return 0;
        }
        else if (strcmp(argv[2], "-f") == 0)
        {
            if (check_if_head() == 0)
            {
                printf_color("You can't 'add' or 'commit' anything while you're not on HEAD!\nTry this --> 'son checkout HEAD'\n", 'y');
                exit(1);
            }
            for (int i = 3; i < argc; i++)
            {
                char temp_arg[1000];
                strcpy(temp_arg, argv[i]);
                address_converter(temp_arg);
                argv[i] = (char *)malloc(1000);
                strcpy(argv[i], temp_arg);
            }
            for (int i = 3; i < argc; i++)
            {
                if (strcmp(argv[i], "current") != 0)
                {
                    char name[1000];
                    strcpy(name, argv[i]);
                    last_maker(name);
                    char folder_address[1000];
                    strcpy(folder_address, argv[i]);
                    back_one(folder_address);
                    char type = type_of(name, folder_address);
                    if (alreadyExists(name, folder_address) == 0)
                    {
                        change_color('Y');
                        printf("\"%s\"", name);
                        reset_color();
                        printf_color(" doesn't seem to exist!\n", 'y');
                        count++;
                        continue;
                    }
                    char cur_commit_folder[1000];
                    strcpy(cur_commit_folder, argv[i]);
                    repo_to_commit(cur_commit_folder, 'c');
                    char folder_delete[1000];
                    strcpy(folder_delete, cur_commit_folder);
                    back_one(cur_commit_folder);
                    if (type == 'F')
                    {
                        remove_folder_files(folder_delete);
                        mkdir(folder_delete);
                        copy_folder(name, argv[i], cur_commit_folder);
                        if (line_exists(added_address, argv[i]) == 0)
                            fprintf(added, "%s\n", argv[i]);
                        append_added(added, argv[i]);
                    }
                    else if (type == 'f')
                    {
                        copy_file(name, argv[i], cur_commit_folder, 'n');
                        if (line_exists(added_address, argv[i]) == 0)
                            fprintf(added, "%s\n", argv[i]);
                    }
                }
                else
                {
                    char current_folder[1000];
                    repoExists(current_folder);
                    char cur_commit_folder[1000];
                    strcpy(cur_commit_folder, current_folder);
                    back_one(cur_commit_folder);
                    repo_to_commit(cur_commit_folder, 'c');
                    back_one(current_folder);
                    DIR *repo = opendir(current_folder);
                    struct dirent *dir;
                    while ((dir = readdir(repo)) != NULL)
                    {
                        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".son") == 0)
                            continue;
                        char folder_delete[1000];
                        strcpy(folder_delete, cur_commit_folder);
                        forward_one(folder_delete, dir->d_name);
                        char org_f_address[1000];
                        strcpy(org_f_address, current_folder);
                        forward_one(org_f_address, dir->d_name);
                        if (dir->d_type == 16)
                        {
                            remove_folder_files(folder_delete);
                            mkdir(folder_delete);
                            copy_folder(dir->d_name, org_f_address, cur_commit_folder);
                            if (line_exists(added_address, org_f_address) == 0)
                                fprintf(added, "%s\n", org_f_address);
                            append_added(added, org_f_address);
                        }
                        else
                        {
                            copy_file(dir->d_name, org_f_address, cur_commit_folder, 'n');
                            if (line_exists(added_address, org_f_address) == 0)
                                fprintf(added, "%s\n", org_f_address);
                        }
                    }
                }
            }
            change_color('p');
            printf("%d items were succsessfully added to staging area.\n", argc - 3 - count);
            reset_color();
            fclose(added);
        }
        else if (strcmp(argv[2], "-redo") == 0)
        {
            fclose(added);
            FILE *added_file = fopen(added_address, "r");
            char address[1000];
            while (fgets(address, 1000, added_file) != NULL)
            {
                if (*(address + strlen(address) - 1) == '\n')
                    *(address + strlen(address) - 1) = '\0';
                char name[1000];
                strcpy(name, address);
                last_maker(name);
                char cur_to_commit[1000];
                strcpy(cur_to_commit, address);
                repo_to_commit(cur_to_commit, 'c');
                char cm_up_folder[1000];
                strcpy(cm_up_folder, cur_to_commit);
                back_one(cm_up_folder);
                char type = type_of(name, cm_up_folder);
                char directory_address[1000];
                strcpy(directory_address, address);
                back_one(directory_address);
                if (type == 'F')
                    copy_folder(name, cur_to_commit, directory_address);
                else if (type == 'f')
                    copy_file(name, cur_to_commit, directory_address, 'n');
            }
            fclose(added_file);
            printf_color("Successfully done!\n", 'g');
        }
        else
        {
            if (check_if_head() == 0)
            {
                printf_color("You can't 'add' or 'commit' anything while you're not on HEAD!\nTry this --> 'son checkout HEAD'\n", 'y');
                exit(1);
            }
            char temp_arg[1000];
            strcpy(temp_arg, argv[2]);
            address_converter(temp_arg);
            argv[2] = (char *)malloc(1000);
            strcpy(argv[2], temp_arg);
            if (strcmp(argv[2], "current") != 0)
            {
                char name[1000];
                strcpy(name, argv[2]);
                last_maker(name);
                char folder_address[1000];
                strcpy(folder_address, argv[2]);
                back_one(folder_address);
                char type = type_of(name, folder_address);
                printf("(%s)(%s)", name, folder_address);
                if (alreadyExists(name, folder_address) == 0)
                {
                    printf_color("** ", 'y');
                    printf("%s", name);
                    printf_color(" doesn't seem to exist!\n", 'y');
                    return 1;
                }
                char cur_commit_folder[1000];
                strcpy(cur_commit_folder, argv[2]);
                repo_to_commit(cur_commit_folder, 'c');
                char folder_delete[1000];
                strcpy(folder_delete, cur_commit_folder);
                back_one(cur_commit_folder);
                if (type == 'F')
                {
                    remove_folder_files(folder_delete);
                    mkdir(folder_delete);
                    copy_folder(name, argv[2], cur_commit_folder);
                    if (line_exists(added_address, argv[2]) == 0)
                        fprintf(added, "%s\n", argv[2]);
                    append_added(added, argv[2]);
                }
                else if (type == 'f')
                {
                    copy_file(name, argv[2], cur_commit_folder, 'n');
                    if (line_exists(added_address, argv[2]) == 0)
                        fprintf(added, "%s\n", argv[2]);
                }
            }
            else
            {
                char current_folder[1000];
                repoExists(current_folder);
                char cur_commit_folder[1000];
                strcpy(cur_commit_folder, current_folder);
                back_one(cur_commit_folder);
                repo_to_commit(cur_commit_folder, 'c');
                back_one(current_folder);
                DIR *repo = opendir(current_folder);
                struct dirent *dir;
                while ((dir = readdir(repo)) != NULL)
                {
                    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".son") == 0)
                        continue;
                    char folder_delete[1000];
                    strcpy(folder_delete, cur_commit_folder);
                    forward_one(folder_delete, dir->d_name);
                    char org_f_address[1000];
                    strcpy(org_f_address, current_folder);
                    forward_one(org_f_address, dir->d_name);
                    if (dir->d_type == 16)
                    {
                        remove_folder_files(folder_delete);
                        mkdir(folder_delete);
                        copy_folder(dir->d_name, org_f_address, cur_commit_folder);
                        if (line_exists(added_address, org_f_address) == 0)
                            fprintf(added, "%s\n", org_f_address);
                        append_added(added, org_f_address);
                    }
                    else
                    {
                        copy_file(dir->d_name, org_f_address, cur_commit_folder, 'n');
                        if (line_exists(added_address, org_f_address) == 0)
                            fprintf(added, "%s\n", org_f_address);
                    }
                }
            }
            printf_color("** Succsessfully added to staging area.\n", 'g');
            fclose(added);
        }
        return 0;
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        char address[1000];
        strcpy(address, argv[2]);
        address_converter(address);
        char folder[1000];
        strcpy(folder, address);
        back_one(folder);
        char name[1000];
        strcpy(name, address);
        last_maker(name);
        char type = type_of(name, folder);
        if (type == 'f')
        {
            reset_file(name, folder);
            change_color('G');
            printf("%s", name);
            printf_color(" was reset!", 'g');
            return 0;
        }
        else if (type == 'F')
        {
            char address_remove[1000];
            strcpy(address_remove, address);
            repo_to_commit(address_remove, 'c');
            char remove[1000];
            strcpy(remove, "rmdir /s/q ");
            strcat(remove, address_remove);
            system(remove);
            mkdir(address_remove);
            reset_folder(name, folder);
            change_color('G');
            printf("%s", name);
            printf_color(" was reset!", 'g');
            return 0;
        }
    }
    else if (strcmp(argv[1], "status") == 0)
    {
        char cwd[1000];
        getcwd(cwd, 1000);
        char dot_son[1000];
        repoExists(dot_son);
        char added_address[1000];
        strcpy(added_address, dot_son);
        forward_one(added_address, "added.txt");
        DIR *currDir = opendir(cwd);
        struct dirent *dir;
        char MAD;
        char file_name[1000];
        char org_f_address[1000];
        int stg, added, delete;
        int M, A, D;
        char org_to_commit[1000];
        int count = 0;
        while ((dir = readdir(currDir)) != NULL)
        {
            if (dir->d_type != 0 || strcmp(dir->d_name, "commitInfo.txt") == 0)
                continue;
            strcpy(org_f_address, cwd);
            forward_one(org_f_address, dir->d_name);
            added = line_exists(added_address, org_f_address);
            if (added == 1)
            {
                strcpy(org_to_commit, org_f_address);
                repo_to_commit(org_to_commit, 'c');
                int check = file_changed(org_f_address, org_to_commit);
                if (check == 1)
                {
                    printf("    %s --> -M\n", dir->d_name);
                    count++;
                    continue;
                }
                if (check == 3)
                {
                    printf("    %s --> -A\n", dir->d_name);
                    count++;
                    continue;
                }
                if (check == 0)
                {
                    strcpy(org_to_commit, org_f_address);
                    repo_to_commit(org_to_commit, 'l');
                    char last_commit_dir[1000];
                    strcpy(last_commit_dir, org_to_commit);
                    back_one(last_commit_dir);
                    added = alreadyExists(dir->d_name, last_commit_dir);
                    if (added == 0)
                    {
                        printf("    %s --> +A\n", dir->d_name);
                        count++;
                        continue;
                    }
                    check = file_changed(org_f_address, org_to_commit);
                    if (check == 0)
                    {
                        printf("    %s --> +0\n", dir->d_name);
                        count++;
                        continue;
                    }
                    else if (check == 1)
                    {
                        printf("    %s --> +M\n", dir->d_name);
                        count++;
                        continue;
                    }
                }
            }
        }
        char cur_commit_dir[1000];
        strcpy(cur_commit_dir, cwd);
        repo_to_commit(cur_commit_dir, 'c');
        DIR *commit_folder = opendir(cur_commit_dir);
        struct dirent *commit_dir;
        while ((commit_dir = readdir(commit_folder)) != NULL)
        {
            if (commit_dir->d_type != 0 || strcmp(commit_dir->d_name, "commitInfo.txt") == 0)
                continue;
            char comtorepo[1000];
            strcpy(comtorepo, cur_commit_dir);
            commit_to_repo(comtorepo);
            delete = alreadyExists(commit_dir->d_name, comtorepo);
            if (delete == 0)
            {
                printf("    %s --> -D\n", commit_dir->d_name);
                count++;
                continue;
            }
        }
        // strcpy(org_to_commit, org_f_address);
        // repo_to_commit(org_to_commit, 'l');
        // strcpy(cur_commit_dir, org_to_commit);
        // back_one(cur_commit_dir);
        // DIR *commitp = opendir(cur_commit_dir);
        // struct dirent *commit_dirp;
        // while ((commit_dirp = readdir(commitp)) != NULL)
        // {
        //     if (commit_dirp->d_type != 0 || strcmp(commit_dir->d_name, "commitInfo.txt") == 0)
        //         continue;
        //     char comtorepo[1000];
        //     strcpy(comtorepo, cur_commit_dir);
        //     commit_to_repo(comtorepo);
        //     delete = alreadyExists(commit_dirp->d_name, comtorepo);
        //     if (delete == 0)
        //     {
        //         printf("    %s --> +D\n", commit_dirp->d_name);
        //         continue;
        //     }
        // }
        if (count == 0)
            printf("Every item is untracked!\n");
        return 0;
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        if (check_if_head() == 0)
        {
            printf_color("You can't 'add' or 'commit' anything while you're not on HEAD!\nTry this --> 'son checkout HEAD'\n", 'y');
            exit(1);
        }
        if (strcmp(argv[2], "-s") == 0)
        {
            if (argc != 4)
            {
                printf_color("Invalid input!", 'R');
                return 1;
            }
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
            printf_color("Commit message is too long! Try again with a message shorter than 73 characters.\n", 'r');
            return 1;
        }
        if (argc != 4)
        {
            printf_color("Invalid commit message!\n", 'r');
            return 1;
        }
        commitfunc(argv[3]);
        return 0;
    }
    else if (strcmp(argv[1], "set") == 0)
    {
        if (argc != 6)
        {
            printf_color("The last arg(shortcut-name) is empty!\n", 'r');
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
            printf_color("Shortcut already exists! Use the 'replace' command to change it.\n", 'y');
            exit(1);
        }
        shortcut_write(argc, argv);
        return 0;
    }
    else if (strcmp(argv[1], "replace") == 0)
    {
        if (shortcut_replace(argc, argv) == 1)
        {
            change_color('R');
            printf("\"%s\"", argv[5]);
            reset_color();
            printf_color(" doesn't exist!\n", 'r');
            exit(1);
        }
        printf("\"%s\" was replaced and now is a shortcut for \"%s\".\n", argv[5], argv[3]);
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        if (shortcut_remove(argc, argv) == 1)
        {
            change_color('Y');
            printf("\"%s\"", argv[3]);
            reset_color();
            printf_color(" doesn't exist!\n", 'y');
            exit(1);
        }
        change_color('G');
        printf("\"%s\"", argv[3]);
        reset_color();
        printf_color(" was successfully removed!\n", 'g');
    }
    else if (strcmp(argv[1], "log") == 0)
    {
        char junk[1000];
        int n = cur_commit(junk);
        if (argc > 2 && strcmp(argv[2], "-n") == 0)
        {
            int depth;
            sscanf(argv[3], "%d", &depth);
            for (int i = 1; i <= depth; i++)
                log_n(i);
        }
        else if (argc > 3 && strcmp(argv[2], "-author") == 0)
        {
            char order_address[1000];
            repoExists(order_address);
            forward_one(order_address, "orderc.txt");
            n = line_counter(order_address);
            for (int i = 1; i <= n; i++)
                log_n_with_author(i, argv[3]);
        }
        else if (argc > 6 && (strcmp(argv[2], "-since") == 0 || strcmp(argv[2], "-before") == 0))
        {
            char order_address[1000];
            repoExists(order_address);
            forward_one(order_address, "orderc.txt");
            n = line_counter(order_address);
            char given_time[1000];
            strcpy(given_time, argv[3]);
            strcat(given_time, " ");
            strcat(given_time, argv[4]);
            strcat(given_time, " ");
            strcat(given_time, argv[5]);
            strcat(given_time, " ");
            strcat(given_time, argv[6]);
            for (int i = 1; i <= n; i++)
                log_n_with_time(i, argv[2], given_time);
        }
        else if (argc > 3 && strcmp(argv[2], "-search") == 0)
        {
            char order_address[1000];
            repoExists(order_address);
            forward_one(order_address, "orderc.txt");
            n = line_counter(order_address);
            for (int i = 1; i <= n; i++)
                log_n_with_search(i, argv[3]);
        }
        else
        {
            char order_address[1000];
            repoExists(order_address);
            forward_one(order_address, "orderc.txt");
            n = line_counter(order_address);
            for (int i = 1; i <= n; i++)
                log_n(i);
        }
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
            printf_color("\nbranch list:\n\n", 'p');
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
                    change_color('p');
                    printf("    --> %s\n", br_name);
                    reset_color();
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
                    printf_color("No commit exists with the given ID!\n", 'r');
                    return 1;
                }
            }
            else if (argv[2][0] == 'H')
            {
                char junk[1000];
                int n = find_head(current_branch_ID, junk);
                sprintf(commit_folder_name, "commit%d.%d", n, current_branch_ID);
                sscanf(commit_folder_name, "commit%d.%d", &th, &branch);
            }
            else
            {
                if (branch_name_to_number_dot_number(argv[2], commit_folder_name) == 0)
                    sscanf(commit_folder_name, "commit%d.%d", &th, &branch);
                else
                {
                    printf_color("Branch doesn't exist!\n", 'r');
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
            printf_color("Successfully checked out!\n", 'g');
            return 0;
        }
        printf_color("There are changes that have to be commited first!\n", 'y');
    }
    else if (strcmp(argv[1], "tag") == 0)
    {
        if (argc == 4 && strcmp(argv[1], "tag") == 0 && strcmp(argv[2], "show") == 0)
        {
            show_tag(argv[3]);
            return 0;
        }
        if (argc == 2 && strcmp(argv[1], "tag") == 0)
        {
            char tags[100][1000];
            char tags_file_address[1000];
            repoExists(tags_file_address);
            forward_one(tags_file_address, "tags.txt");
            alphabetic_order(tags_file_address);
            FILE *tags_file = fopen(tags_file_address, "r");
            int index = 0;
            while (fgets(tags[index], 1000, tags_file) != NULL)
            {
                if (*(tags[index] + strlen(tags[index]) - 1) == '\n')
                    *(tags[index] + strlen(tags[index]) - 1) = '\0';
                index++;
            }
            for (int i = 0; i < index; i++)
                show_tag(tags[i]);
            return 0;
        }
        tagfunc(argc, argv);
    }
    else if (strcmp(argv[1], "diff") == 0)
    {
        if (strcmp(argv[2], "-f") == 0)
        {
            char file1_address[1000], file2_address[1000];
            strcpy(file1_address, argv[3]);
            strcpy(file2_address, argv[4]);
            address_converter(file1_address);
            address_converter(file2_address);
            char file1_name[1000], file2_name[1000];
            strcpy(file1_name, file1_address);
            strcpy(file2_name, file2_address);
            last_maker(file1_name);
            last_maker(file2_name);
            char folder1[1000], folder2[1000];
            strcpy(folder1, file1_address);
            strcpy(folder2, file2_address);
            back_one(folder1);
            back_one(folder2);
            if (alreadyExists(file1_name, folder1) == 0)
            {
                printf_color("**** ", 'r');
                printf("\"%s\" doesn't exist!\n", file1_name);
                return 1;
            }
            if (alreadyExists(file2_name, folder2) == 0)
            {
                printf_color("**** ", 'r');
                printf("\"%s\" doesn't exist!\n", file2_name);
                return 1;
            }
            int start1 = 1, start2 = 1, end1 = -1, end2 = -1;
            for (int i = 5; i < argc - 1; i++)
            {
                if (strcmp(argv[i], "-line1") == 0)
                    sscanf(argv[i + 1], "%d-%d", &start1, &end1);
                if (strcmp(argv[i], "-line2") == 0)
                    sscanf(argv[i + 1], "%d-%d", &start2, &end2);
            }
            diff(file1_address, file2_address, start1, end1, start2, end2, 'f');
        }

        else if (strcmp(argv[2], "-c") == 0)
        {
            int ID1, ID2;
            sscanf(argv[3], "%d", &ID1);
            sscanf(argv[4], "%d", &ID2);
            char name1[1000], name2[1000];
            char address_1[1000], address_2[1000];
            commit_ID_to_number_dot_number(ID1, name1);
            commit_ID_to_number_dot_number(ID2, name2);
            repoExists(address_1);
            repoExists(address_2);
            forward_one(address_1, name1);
            forward_one(address_2, name2);
            folder_diff(address_1, address_2);
            return 0;
        }
    }
    else if (strcmp(argv[1], "merge") == 0)
    {
        if (branch_exists(argv[3]) == 0 || branch_exists(argv[4]) == 0)
        {
            printf_color("Not successful!\n", 'r');
            printf_color("Make sure these branches actually exist!\n", 'Y');
            return 1;
        }
        if (strcmp(argv[2], "-b") == 0)
        {
            char br1[1000], br2[1000];
            strcpy(br1, argv[3]);
            strcpy(br2, argv[4]);
            char commit1[1000], commit2[1000];
            branch_name_to_number_dot_number(br1, commit1);
            branch_name_to_number_dot_number(br2, commit2);
            char cm1_address[1000], cm2_address[1000];
            repoExists(cm1_address);
            repoExists(cm2_address);
            forward_one(cm1_address, commit1);
            forward_one(cm2_address, commit2);
            int ID, th, branch;
            sscanf(commit1, "commit%d.%d", &th, &branch);
            if (branch != current_branch_ID)
            {
                printf_color("You can't merge into a branch which you're not on!\n", 'R');
                return 1;
            }
            if (merge_folder_diff(cm1_address, cm2_address) == 0)
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
                    char cm_dest[1000];
                    sprintf(cm_dest, "commit%d.%d", th + 1, branch);
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
                    fprintf(cur_cm, "commit%d.%d", th + 2, branch);
                    fclose(cur_cm);

                    char br_address[1000];
                    repoExists(br_address);
                    char br_ID[1000];
                    sprintf(br_ID, "branch%d.txt", branch);
                    forward_one(br_address, br_ID);
                    FILE *br = fopen(br_address, "a");
                    fprintf(br, "commit%d.%d\n", th + 1, branch);
                    fclose(br);

                    char dest[1000];
                    repoExists(dest);
                    copy_folder(cm_dest, cm1_address, dest);
                    copy_folder(cm_dest, cm2_address, dest);

                    commit_n_folder(th + 2);

                    printf_color("Successfully merged!\n", 'g');
                    return 0;
                }
                printf_color("There are changes that have to be commited first!\n", 'r');
            }
        }
    }
}