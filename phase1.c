#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <process.h>

int find_char(char str[], char c)
{
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == c)
            return i;
    return -1;
}

int find_last_char(char str[], char c)
{
    int index = -1;
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == c)
            index = i;
    return index;
}


int count_char(char str[], char c)
{
    int cc = 0;
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == c)
            cc++;
    return cc;
}


void free_string_vector(char** vec, int size)
{
    for (int i = 0; i < size; i++)
        free(vec[i]);
    free(vec);
}

void create_file(char path[])
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
        mkdir(directory, 0777);
        create_file(filename);
    }
}

void insert(char path[], char str[], int line_number, int pos)
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
        fputc(fp, str[i]);
    fclose(fp);

}

void cat(char path[])
{
    FILE* fp = fopen(path);
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        printf("%c", c);
    }
    fclose(fp);
}

void remove(char path[], int line_number, int pos, int size, int backward)
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
        fputc(fp, '\b');
    fclose(fp);


}

void copy(char path[], int line_number, int pos, int size, int backward)
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
        fputc(clip, c);

    }
    fclose(fp);
    fclose(clip);
}

void cut(char path[], int line_number, int pos, int size, int backward)
{
    copy(path, line_number, pos, size, backward);
    remove(path, line_number, pos, size, backward);
}

void paste(char path[], int line_number, int pos)
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
    for (int i = 0; i < size; i++)
    {
        char c = fgetc(clip);
        fputc(fp, c);

    }
    fclose(fp);
    fclose(clip);
}

int main() {
    while (true)
    {
        char command[100];
        char fileAddress[100];
        scanf("%s", command);
        if (strcmp(command, "createfile") == 0) {
            scanf("%s", fileAddress);
            create_file(fileAddress);
        }
        // And so the other functions will be called and appropriate error handling will be done in phase 2
    }
}
