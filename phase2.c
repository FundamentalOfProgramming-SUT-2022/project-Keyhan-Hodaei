#include "phase1.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#ifdef __linux__

#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
#endif

#ifdef _WIN32
#include <windows.h>
void gotoXY(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(
            GetStdHandle(STD_OUTPUT_HANDLE), c);
}
#endif


int mode;
char lines[80][120];
int first_line_index;
int cursurX, cursurY;
char command[200];
char context[20000];
int commandSize;

void init() {
    mode = 0;
    for (int i = 0; i < 80; i++)
        lines[i][0] = '\0';
    first_line_index = 0;
    cursurX = 0;
    cursurY = 0;
    system("cls");
    commandSize = 0;
}

void moveUp() {
    if (mode == 1 && cursurY > 0)
        cursurY --;
}

void moveDown() {
    if (mode == 1 && cursurY < 39)
        cursurY ++;
}

void moveRight() {
    if (mode == 1 && cursurX < 120)
        cursurX ++;
}

void moveLeft() {
    if (mode == 1 && cursurX > 0)
        cursurX --;
}

void writeContext() {
    FILE* fp = fopen("temp.txt", "w");
    for (int i = 0; i < 20000; ++i) {
        fputc(context[i], fp);
    }
}

void execute() {
    char commanddup[200];
    char commanddup2[200];
    strcpy(commanddup, command);
    strcpy(commanddup2, command);
    char firstword[10];
    sscanf(commanddup, "%s", firstword);
    if (strcmp(firstword , "save") == 0) {
        writeContext();
    }
    else if (strcmp(firstword , "createfile") == 0) {
        char com[100], op[100], path[100];
        sscanf(commanddup2, "%s %s %s", com, op, path);
        create_file(path);
    }
    else if (strcmp(firstword , "cat") == 0) {
        char com[100], op[100], path[100];

        sscanf(commanddup2, "%s %s %s", com, op, path);
        cat(path);
    }
    else if (strcmp(firstword , "cat") == 0) {
        char com[100], op[100], path[100];
        sscanf(commanddup2, "%s %s %s", com, op, path);
        cat(path);
    }
    else if (strcmp(firstword , "insertstr") == 0) {
        char com[100], op[100], value[100];
        char path[100], str[100];
        int line_number, pos;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-–str") == 0) {
                sscanf(commanddup2, "%s", str);
            }
            else if (strcmp(firstword , "-–pos") == 0) {
                sscanf(commanddup2, "%s", value);
                char firstvalue[20], secondvalue[20];
                int i;
                for (i = 0; i < strlen(value) && value[i] != ':'; i++)
                    firstvalue[i] = value[i];
                firstvalue[i] = '\0';
                i++;
                for (i = 0; i < strlen(value); i++)
                    secondvalue[i - strlen(firstvalue) - 1] = value[i];
                secondvalue[i] = '\0';
                line_number = atoi(firstvalue);
                pos = atoi(secondvalue);

            }
        }
        insert(path, str, line_number, pos);
    }
    else if (strcmp(firstword , "removetstr") == 0) {
        char com[100], op[100], value[100];
        char path[100], str[100];
        int size, line_number, pos, backward = 0;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-size") == 0) {
                sscanf(commanddup2, "%d", size);
            }
            else if (strcmp(firstword , "-–pos") == 0) {
                sscanf(commanddup2, "%s", value);
                char firstvalue[20], secondvalue[20];
                int i;
                for (i = 0; i < strlen(value) && value[i] != ':'; i++)
                    firstvalue[i] = value[i];
                firstvalue[i] = '\0';
                i++;
                for (i = 0; i < strlen(value); i++)
                    secondvalue[i - strlen(firstvalue) - 1] = value[i];
                secondvalue[i] = '\0';
                line_number = atoi(firstvalue);
                pos = atoi(secondvalue);

            }
            else if (strcmp(firstword , "-b") == 0) {
                backward = 1;
            }
        }
        _remove(path, line_number, pos, size, backward);
    }

    else if (strcmp(firstword , "copystr") == 0) {
        char com[100], op[100], value[100];
        char path[100], str[100];
        int size, line_number, pos, backward = 0;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-size") == 0) {
                sscanf(commanddup2, "%d", size);
            }
            else if (strcmp(firstword , "-–pos") == 0) {
                sscanf(commanddup2, "%s", value);
                char firstvalue[20], secondvalue[20];
                int i;
                for (i = 0; i < strlen(value) && value[i] != ':'; i++)
                    firstvalue[i] = value[i];
                firstvalue[i] = '\0';
                i++;
                for (i = 0; i < strlen(value); i++)
                    secondvalue[i - strlen(firstvalue) - 1] = value[i];
                secondvalue[i] = '\0';
                line_number = atoi(firstvalue);
                pos = atoi(secondvalue);

            }
            else if (strcmp(firstword , "-b") == 0) {
                backward = 1;
            }
        }
        copy(path, line_number, pos, size, backward);
    }

    else if (strcmp(firstword , "cutstr") == 0) {
        char com[100], op[100], value[100];
        char path[100], str[100];
        int size, line_number, pos, backward = 0;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-size") == 0) {
                sscanf(commanddup2, "%d", size);
            }
            else if (strcmp(firstword , "-–pos") == 0) {
                sscanf(commanddup2, "%s", value);
                char firstvalue[20], secondvalue[20];
                int i;
                for (i = 0; i < strlen(value) && value[i] != ':'; i++)
                    firstvalue[i] = value[i];
                firstvalue[i] = '\0';
                i++;
                for (i = 0; i < strlen(value); i++)
                    secondvalue[i - strlen(firstvalue) - 1] = value[i];
                secondvalue[i] = '\0';
                line_number = atoi(firstvalue);
                pos = atoi(secondvalue);

            }
            else if (strcmp(firstword , "-b") == 0) {
                backward = 1;
            }
        }
        cut(path, line_number, pos, size, backward);
    }
    else if (strcmp(firstword , "pastestr") == 0) {
        char com[100], op[100], value[100];
        char path[100], str[100];
        int size, line_number, pos, backward = 0;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-–pos") == 0) {
                sscanf(commanddup2, "%s", value);
                char firstvalue[20], secondvalue[20];
                int i;
                for (i = 0; i < strlen(value) && value[i] != ':'; i++)
                    firstvalue[i] = value[i];
                firstvalue[i] = '\0';
                i++;
                for (i = 0; i < strlen(value); i++)
                    secondvalue[i - strlen(firstvalue) - 1] = value[i];
                secondvalue[i] = '\0';
                line_number = atoi(firstvalue);
                pos = atoi(secondvalue);

            }
        }
        paste(path, line_number, pos);
    }
    else if (strcmp(firstword , "find") == 0) {
        char com[100], op[100], value[100];
        char path[100], str[100];
        int size, line_number, pos, all = 0, at = -1, count = 0;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-–str") == 0) {
                sscanf(commanddup2, "%s", str);
            }
            else if (strcmp(firstword , "-count") == 0) {
                count = 1;
            }
            else if (strcmp(firstword , "-at") == 0) {
                sscanf(commanddup2, "%d", at);
            }
            else if (strcmp(firstword , "-all") == 0) {
                all = 1;
            }
        }
        if (at == -1 && all == 0 && count == 0)
            printf("%d\n", find(path, str, 0));
        else if (count == 1)
            printf("%d\n", find_count(path, str));
        else if (at > 0)
            printf("%d\n", find_at(path, str, at));
        else if (all == 1)
            find_all(path, str, 0); // n unused

    }

    else if (strcmp(firstword , "replace") == 0) {
        char com[100], op[100], value[100];
        char path[100], str1[100], str2[100];
        int size, line_number, pos, all = 0, at = -1, count = 0;
        sscanf(commanddup2, "%s", com);
        while (sscanf(commanddup2, "%s", op) != EOF)
        {
            if (strcmp(firstword , "-–file") == 0) {
                sscanf(commanddup2, "%s", path);
            }
            else if (strcmp(firstword , "-–str1") == 0) {
                sscanf(commanddup2, "%s", str1);
            }
            else if (strcmp(firstword , "-–str2") == 0) {
                sscanf(commanddup2, "%s", str2);
            }
        }
        replace(path, str1, str2);

    }
}


int main() {
    init();
    while (1) {
        gotoXY(cursurX, cursurY);
        if (mode == 0) {
            cursurY = 40;
        }
        if (_kbhit()) {
            int key1 = _getch();
            if (key1 == 224) {
                int key = _getch();
                switch (key) {
                    case 72:
                        moveUp();
                        break;
                    case 75:
                        moveLeft();
                        break;
                    case 77:
                        moveRight();
                        break;
                    case 80:
                        moveDown();
                        break;
                    default:
                        //printf("%d\n", key);
                        break;
                }
            }
            else {
                if (key1 == 27) {// escape
                    mode = 0;
                    cursurX = 0;
                }
                else if (mode == 0 && key1 == 'i') {
                    mode = 1;
                    cursurX = 0;
                    cursurY = 0;
                }
                else if (mode == 0 && key1 == 13) {
                    // execute
                    cursurX = 0;
                }
                else if (mode == 1 && key1 == 13) {
                    execute();
                    cursurX = 0;
                    cursurY++;
                    commandSize = 0;
                }
                else {
                    putchar(key1);
                    cursurX++;
                    if (mode == 0)
                        command[commandSize++] = key1;
                    else
                        context[cursurX + cursurY * 80];
                }
            }
        }
    }
}




