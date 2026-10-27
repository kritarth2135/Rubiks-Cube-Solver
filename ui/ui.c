#include <ncurses.h>

#include "ui.h"

void draw_border(WINDOW *win);
WINDOW *create_window(
    int height_per, int width_per, int start_row, int start_col, int *next_row, int *next_col
);
void destroy_window(WINDOW *win);

/*
 * This function deals with the UI of the program. Takes an integer as an argument to determine
 * if to show debug info or not.
 */
int ui() {
    int row, col;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    refresh();

    WINDOW *win1 = create_window(60, 100, 0, 0, &row, &col);
    WINDOW *win2 = create_window(40, 100, row, 0, &row, &col);

    getch();

    destroy_window(win1);
    destroy_window(win2);
    return endwin();
}


/*
 * Draws a simple border around the window (I didn't know about box function when I wrote this...)
 */
void draw_border(WINDOW *win) {
    int row, col;
    getmaxyx(win, row, col);

    for (int i = 0; i < row; i++) {
        wmove(win, i, 0);
        if (i == 0 || i == row - 1) {
            waddch(win, i == 0 ? ACS_ULCORNER: ACS_LLCORNER);
            for (int j = 1; j < col - 1; j++) {
                waddch(win, ACS_HLINE);
            }
            waddch(win, i == 0 ? ACS_URCORNER: ACS_LRCORNER);
        }
        else {
            waddch(win, ACS_VLINE);
            mvwaddch(win, i, col - 1, ACS_VLINE);
        }
    }

    wmove(win, 1, 1);
}


/*
 * Creates a window by taking the percentage of height and width as parameter. It also sets next_row
 * and next_col arguments to the immediate row and col indexes at the end of window.
 */
WINDOW *create_window(
    int height_per, int width_per, int start_row, int start_col, int *next_row, int *next_col
) {
    int row, col;
    if (width_per < 0 || width_per > 100 || height_per < 0 || height_per > 100) {
        return NULL;
    }

    getmaxyx(stdscr, row, col);

    int height = (height_per * row) / 100;
    int width = (width_per * col) / 100;

    WINDOW *window = newwin(height, width, start_row, start_col);
    *next_row = height + start_row;
    *next_col = width + start_col;

    draw_border(window);
    wrefresh(window);

    getch();

    return window;
}


/*
 * Destroys a window by clearing it from the stdscr
 */
void destroy_window(WINDOW *win) {
    wclear(win);
    wrefresh(win);
    delwin(win);
}
