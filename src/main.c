//testing file

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

int main() {
    printf("Hello, World!\n");
    initscr();
    endwin();
    return 0;
}
