#include "phase1.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int mode;
char lines[80][120];
int first_line_index;
int cursurX, cursurY;

void init()
{
    mode = 0;
    for (int i = 0; i < 80; i++)
        lines[i][0] = '\0';
    first_line_index = 0;
    cursurX = 0;
    cursurY = 0;
    system("clear");
}

int main()
{
    init();
    while (1)
    {
        
    }
}
