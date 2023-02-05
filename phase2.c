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

void init() {
    mode = 0;
    for (int i = 0; i < 80; i++)
        lines[i][0] = '\0';
    first_line_index = 0;
    cursurX = 0;
    cursurY = 0;
    system("cls");
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
                        printf("up arrow\n");
                        break;
                    case 75:
                        printf("left arrow\n");
                        break;
                    case 77:
                        printf("right arrow\n");
                        break;
                    case 80:
                        printf("down arrow\n");
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
                else {
                    putchar(key1);
                    cursurX++;
                }
            }
        }
    }
}
