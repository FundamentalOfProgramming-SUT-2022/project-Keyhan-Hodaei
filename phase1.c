#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <process.h>

int main() {
    char command[100];
    char fileAddress[100];
    scanf("%s", command);
    if (strcmp(command, "createfile") == 0) {
        getchar(); getchar(); getchar(); getchar(); getchar(); getchar(); getchar(); getchar(); // --file
        scanf("%s", fileAddress);
    }
    int slashCount = 0, lastSlashIndex;
    char *dirName;
    char *fileName;
    char *token;
    char *token2;
    int check;
    // file names in which there are no spaces...
    if (fileAddress[0] != '"') {
        //printf("%s", fileAddress);
        for (int i = 0; i < 100; ++i) {
            if (fileAddress[i] == '/') {
                slashCount++;
                lastSlashIndex = i;
            }
        }
        //printf("%d\n", lastSlashIndex);
        token = strtok(fileAddress, "/");
        dirName = token;
        check = mkdir(dirName);
        //printf("%d", check);
        //printf("%s", strerror(errno));
        for (int i = 0; i < lastSlashIndex; ++i) {
//            token = strtok(NULL, "/");
//            dirName = token;
//            //clrscr();
//            check = mkdir(dirName);
//            printf("%s\n", dirName);
//            if (check) {
//                printf("Unable to create directory");
//                exit(1);
            token2 = strtok(NULL, "/");
            strcat(token, "/");
            strcat(token, token2);
            mkdir(token);
            }
            getch();
            //system("dir/p");
            getch();
        }
        for (int i = lastSlashIndex; fileAddress[i] != '\0'; ++i) {
            token = strtok(fileAddress, "/");
            fileName = fopen(token, "w");
        }
    }
    // file names that contain spaces...
    // ----------------------------------------------------------having problems with handling spaces---------------------------------------------------------//
//    else {
//        for (int i = 1; i < 100; ++i) {
//            if (fileAddress[i] == '/') {
//                slashCount++;
//                lastSlashIndex = i;
//            }
//        }
//        char *dirName;
//        char *fileName;
//        char *token;
//        int check;
//        for (int i = 0; i < lastSlashIndex; ++i) {
//            token = strtok(fileAddress, "/");
//            dirName = token;
//            //clrscr();
//            check = mkdir(dirName);
//            if (check) {
//                printf("Unable to create directory");
//                exit(1);
//            }
//            getch();
//            //system("dir/p");
//            getch();
//        }
//        for (int i = lastSlashIndex; i < 100; ++i) {
//            token = strtok(fileAddress, "/");
//            fileName = fopen(token, "w");
//        }
//    }
//    return 0;
//}
