#include <ncurses.h>

#include "ui.h"

/*
 * This function deals with the UI of the program. Takes an integer as an argument to determine
 * if to show debug info or not.
 */
int ui(int debug) {
    initscr();

    addstr("Test");
    refresh();

    getch();

    return endwin();
}
