#include <ncurses.h>

#include "ui.h"

void draw_border(WINDOW *win);

/*
 * This function deals with the UI of the program. Takes an integer as an argument to determine
 * if to show debug info or not.
 */
int ui() {
    int ch, col, row;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    while (true) {
        ch = getch();
        if (ch == KEY_F(1))
            break;
        else if (ch == KEY_RESIZE) {
            clear();
            refresh();
        }

        getmaxyx(stdscr, row, col);

        draw_border(stdscr);

        move(1, 1);
        mvprintw(row / 2, (col - 33) / 2, "Current Terminal Size: %3i by %3i", col, row);
        refresh();
    }
    refresh();
    getch();
    return endwin();
}


void draw_border(WINDOW *win) {
    int row, col;
    getmaxyx(win, row, col);

    for (int i = 0; i < row; i++) {
        wmove(win, i, 0);
        if (i == 0 || i == row - 1) {
            addch(i == 0 ? ACS_ULCORNER: ACS_LLCORNER);
            for (int j = 1; j < col - 1; j++) {
                addch(ACS_HLINE);
            }
            addch(i == 0 ? ACS_URCORNER: ACS_LRCORNER);
        }
        else {
            addch(ACS_VLINE);
            mvwaddch(win, i, col - 1, ACS_VLINE);
        }
    }
}
