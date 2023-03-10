#ifdef __linux__ 
#include <curses.h>
#include <dirent.h>
#elif _WIN32
#include <dir.h>
#include <conio.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline int find_char(char str[], char c)
{
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == c)
            return i;
    return -1;
}

inline int find_last_char(char str[], char c)
{
    int index = -1;
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == c)
            index = i;
    return index;
}


inline int count_char(char str[], char c)
{
    int cc = 0;
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == c)
            cc++;
    return cc;
}


inline void free_string_vector(char** vec, int size)
{
    for (int i = 0; i < size; i++)
        free(vec[i]);
    free(vec);
}

inline void create_file(char path[])
{
    if (find_char(path, '/') == -1) 
    {
        FILE* new_created = fopen(path, "w");
        fclose(new_created);
    }
    else
    {
        int index = find_last_char(path, '/');
        char directory[100];
        int i;
        for (i = 0; i < index; i++)
            directory[i] = path[i];
        directory[i] = '\0';
        char filename[100];
        i++;
        for (; i < strlen(path); i++)
            filename[i - index - 1] = path[i];
        filename[i] = '\0';
        mkdir(directory);
        create_file(filename);
    }
}

inline void insert(char path[], char str[], int line_number, int pos)
{
    FILE* fp = fopen(path, "rw");
    for (int i = 1; i < line_number; i++)
    {
        char c = fgetc(fp);
        while (c != '\n' && c != EOF)
            c = fgetc(fp);
    }
    for (int i = 0; i < pos; i++)
    {
        char c = fgetc(fp);
        if (c == '\n' || c == EOF)
            break;
    }
    for (int i = 0; i < strlen(str); i++)
        fputc(str[i], fp);
    fclose(fp);

}

void cat(char path[])
{
    FILE* fp = fopen(path, "r");
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        printf("%c", c);
    }
    fclose(fp);
}

inline void _remove(char path[], int line_number, int pos, int size, int backward)
{
    FILE* fp = fopen(path, "rw");
    for (int i = 1; i < line_number; i++)
    {
        char c = fgetc(fp);
        while (c != '\n' && c != EOF)
            c = fgetc(fp);
    }
    for (int i = 0; i < pos; i++)
    {
        char c = fgetc(fp);
        if (c == '\n' || c == EOF)
            break;
    }
    if (backward)
        fseek(fp, -size, SEEK_CUR);
    for (int i = 0; i < size; i++)
        fputc('\b', fp);
    fclose(fp);


}

inline void copy(char path[], int line_number, int pos, int size, int backward)
{
    FILE* fp = fopen(path, "rw");
    for (int i = 1; i < line_number; i++)
    {
        char c = fgetc(fp);
        while (c != '\n' && c != EOF)
            c = fgetc(fp);
    }
    for (int i = 0; i < pos; i++)
    {
        char c = fgetc(fp);
        if (c == '\n' || c == EOF)
            break;
    }
    if (backward)
        fseek(fp, -size, SEEK_CUR);
    FILE* clip = fopen("clipboard.txt", "w");
    for (int i = 0; i < size; i++)
    {
        char c = fgetc(fp);
        fputc(c, clip);

    }
    fclose(fp);
    fclose(clip);
}

inline void cut(char path[], int line_number, int pos, int size, int backward)
{
    copy(path, line_number, pos, size, backward);
    _remove(path, line_number, pos, size, backward);
}

inline void paste(char path[], int line_number, int pos)
{
    FILE* fp = fopen(path, "rw");
    for (int i = 1; i < line_number; i++)
    {
        char c = fgetc(fp);
        while (c != '\n' && c != EOF)
            c = fgetc(fp);
    }
    for (int i = 0; i < pos; i++)
    {
        char c = fgetc(fp);
        if (c == '\n' || c == EOF)
            break;
    }
    FILE* clip = fopen("clipboard.txt", "w");
    char c = fgetc(clip);
    for (int i = 0; c != EOF; i++)
    {
        fputc(c, fp);
        c = fgetc(clip);
    }
    fclose(fp);
    fclose(clip);
}

inline int find(char path[], char str[], int start_index)
{
    char chunk[strlen(str)];
    FILE* fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    for (int i = start_index; i < sz - strlen(str); i++) {
        fseek(fp, i, SEEK_SET);
        fread(chunk, 1, strlen(str), fp);
        if (strcmp(chunk, str) == 0)
            return i;
    }
    return -1;
}

inline int find_count(char path[], char str[])
{
    int cnt = -1;
    int result = 0;
    while (result != -1)
    {
        result = find(path, str, result);
        cnt ++;
        if (result != -1)
            result ++;
    }
    return cnt;
}

inline int find_at(char path[], char str[], int n)
{
    int cnt = -1;
    int result = 0;
    for (int i = 0; i < n && result != -1; i++) {
        result = find(path, str, result);
        if (result != -1)
            result ++;
    }
    return result;
}


inline void find_all(char path[], char str[], int n)
{
    int result = 0;
    while (result != -1)
    {

        result = find(path, str, result);
        if (result != -1)
        printf("%d,", result);
    }
}

inline void replace(char path[], char str[], char str2[])
{
    char chunk[strlen(str)];
    FILE* fp = fopen(path, "rw");
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    for (int i = 0; i < sz - strlen(str); i++) {
        fseek(fp, i, SEEK_SET);
        fread(chunk, 1, strlen(str), fp);
        if (strcmp(chunk, str) == 0) {
            fprintf(fp, "%s", str2);
        }
    }
}

inline void handle_command() {
    char command_line[200];
    gets(command_line);

}

