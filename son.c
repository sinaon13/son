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
        printf("%s is now an alias to %s.", argv[2] + 6, argv[3]);
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

int alias_replace(int argc, char *argv[])
{
    // local
    int done[100] = {0};
    for (int i = 1; i < argc; i++)
    {
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
    // global
    for (int i = 1; i < argc; i++)
    {
        if (done[i] == 1)
            continue;
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
            copy_file(dir->d_name, org_f_address, copy_address);
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
    if (argc == 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-f") != 0 && strcmp(argv[2], "-n") != 0 && strcmp(argv[2], "-redo") != 0)
        return 1;
    if (argc > 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-f") == 0)
        return 1;
    if (argc == 4 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-n") == 0)
        return 1;
    if (argc == 3 && strcmp(argv[1], "add") == 0 && strcmp(argv[2], "-redo") == 0)
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

int file_changed(char file1[], char file2[])
{
    FILE *one, *two;
    one = fopen(file1, "rb");
    two = fopen(file2, "rb");
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
    char cwd[1000];
    getcwd(cwd, 1000);
    mkdir(".son");
    FILE *repo_address = fopen(".\\.son\\repo_address.txt", "w");
    fprintf(repo_address, "%s", cwd);
    fclose(repo_address);
    mkdir(".\\.son\\commit0");
    char commit0_address[1000];
    getcwd(commit0_address, 1000);
    strcpy(commit0_address + strlen(commit0_address), "\\.son\\commit0");
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
    commit_n_folder(1);
    printf("A new repo was succsesfully made.\n");
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

int check_if_staged_or_modified(char type, char address[])
{
    char cur_commit_address[1000];
    cur_commit(cur_commit_address);
    printf("(%s)\n", cur_commit_address);
    char t[1000];
    repoExists(t);
    forward_one(t, "repo_address.txt");
    FILE *repo_ress = fopen(t, "r");
    char repo_address[1000];
    fgets(repo_address, 1000, repo_ress);
    char temp[1000];
    strcpy(temp, address + strlen(repo_address));
    strcpy(cur_commit_address + strlen(cur_commit_address), temp);
    int yn;
    char name[1000];
    strcpy(name, address);
    last_maker(name);
    char temp_address_already[1000];
    strcpy(temp_address_already, cur_commit_address);
    back_one(temp_address_already);
    if (alreadyExists(name, temp_address_already) == 0)
        return 0;
    if (type == 'f')
        yn = file_changed(address, cur_commit_address);
    else
        yn = folder_changed(address, cur_commit_address);
    if (yn == 0)
        return 1;
    else
        return 0;
}

int append_added_n(FILE *file, char address[], int depth, int max_depth)
{
    if (depth == max_depth + 1)
        return 0;
    DIR *cur_folder = opendir(address);
    struct dirent *d;
    char type;
    char added[1000];
    repoExists(added);
    forward_one(added, "added_n.txt");
    while ((d = readdir(cur_folder)) != NULL)
    {
        char temp[1000], temp_line[1000];
        strcpy(temp, address);
        forward_one(temp, d->d_name);
        sprintf(temp_line, "%d %s", depth, temp);
        type = type_of(d->d_name, address);
        if (type == 'F' || type == 'f')
        {
            int stg;
            if (type == 'F')
                stg = check_if_staged_or_modified('F', temp);
            else
                stg = check_if_staged_or_modified('f', temp);
            char werid_temp[1000];
            sprintf(werid_temp, "%d %s", depth, temp);
            int add = line_exists(added, werid_temp);
            char state[1000];
            if (stg == 1 && add == 1)
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
            if (line_exists(added, temp_line) == 0)
            {
                fprintf(file, "%d %s\n", depth, temp);
            }
            append_added_n(file, temp, ++depth, max_depth);
            depth--;
        }
        else if (type == 'f')
        {
            if (line_exists(added, temp_line) == 0)
                fprintf(file, "%d %s\n", depth, temp);
        }
    }
    return 0;
}

int repo_to_commit(char address[])
{
    char cur[1000];
    cur_commit(cur);
    char temp[1000];
    repoExists(temp);
    back_one(temp);
    char temp_address[1000];
    strcpy(temp_address, address);
    strcpy(temp_address, temp_address + strlen(temp));
    strcpy(cur + strlen(cur), temp_address);
    strcpy(address, cur);
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
    // find_current_commit
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
        int count = 0;
        char type;
        char org_f_address[1000];
        char cwd[1000];
        getcwd(cwd, 1000);
        char temp[1000];
        strcpy(temp, repo_address);
        forward_one(temp, "added.txt");
        char temp_n[1000];
        strcpy(temp_n, repo_address);
        forward_one(temp_n, "added_n.txt");
        FILE *added = fopen(temp, "a");
        FILE *added_n = fopen(temp_n, "a");
        if (strcmp(argv[2], "-n") == 0)
        {
            DIR *curr = opendir(".");
            struct dirent *dir;
            while ((dir = readdir(curr)) != NULL)
            {
                if (strcmp(dir->d_name, ".son") == 0)
                    continue;
                strcpy(org_f_address, cwd);
                forward_one(org_f_address, dir->d_name);
                type = type_of(dir->d_name, cwd);
                char temp_line[1000];
                sprintf(temp_line, "1 %s", org_f_address);
                if (type == 'F')
                {
                    int stg = check_if_staged_or_modified('F', org_f_address);
                    char werid_temp[1000];
                    sprintf(werid_temp, "1 %s", org_f_address);
                    int add = line_exists(temp_n, werid_temp);
                    char state[1000];
                    if (stg == 1 && add == 1)
                        strcpy(state, "--> staged");
                    else
                        strcpy(state, "--> Not staged");
                    char temp[1000];
                    strcpy(temp, org_f_address);
                    last_maker(temp);
                    printf("\n    1 %s %s", temp, state);
                    if (line_exists(temp_n, temp_line) == 0)
                        fprintf(added_n, "1 %s\n", org_f_address);
                    int pow = 1;
                    int max_depth = 0;
                    for (int i = strlen(argv[3]) - 1; i >= 0; i--)
                    {
                        max_depth += pow * (*(argv[3] + i) - '0');
                        pow *= 10;
                    }
                    append_added_n(added_n, org_f_address, 2, max_depth);
                }
                else if (type == 'f')
                {
                    int stg = check_if_staged_or_modified('f', org_f_address);
                    char werid_temp[1000];
                    sprintf(werid_temp, "1 %s", org_f_address);
                    int add = line_exists(temp_n, werid_temp);
                    char state[1000];
                    if (stg == 1 && add == 1)
                        strcpy(state, "--> staged");
                    else
                        strcpy(state, "--> Not staged");
                    char temp[1000];
                    strcpy(temp, org_f_address);
                    last_maker(temp);
                    printf("\n    1 %s %s", temp, state);
                    if (line_exists(temp_n, temp_line) == 0)
                        fprintf(added_n, "1 %s\n", org_f_address);
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
                        repo_to_commit(cur_commit_address);
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
                            copy_file(dir->d_name, org_f_address, cur_commit_address);
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
            printf("%d items were succsesfully added to staging area.\n", argc - 3 - count);
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
                    stg = check_if_staged_or_modified('F', address);
                else if (type == 'f')
                    stg = check_if_staged_or_modified('f', address);
                printf("(%s)(%d)\n", name, stg);
                if (stg != 1)
                {
                    char org[1000];
                    strcpy(org, address);
                    repo_to_commit(org);
                    char file_name[1000];
                    strcpy(file_name, org);
                    last_maker(file_name);
                    back_one(address);
                    if (type == 'f')
                        copy_file(file_name, org, address);
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
                    repo_to_commit(cur_commit_address);
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
                        copy_file(dir->d_name, org_f_address, cur_commit_address);
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
                printf("%s was succsesfully added to staging area.\n", argv[2]);
        }
        fclose(added);
        return 0;
    }
}