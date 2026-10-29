#include <stdbool.h>
#include <ncurses.h>

#include "cube/cube.h"
#include "ui.h"

#define CUBE_WINDOW_HEIGHT_PER 75
#define CUBE_WINDOW_WIDTH_PER 100
#define HELP_WINDOW_HEIGHT_PER 25
#define HELP_WINDOW_WIDTH_PER 100
#define FACE_HEIGHT_PER 33
#define FACE_WIDTH_PER 25
#define FACET_PER 33


const int CURSES_COLOR_MAPPING[NUMBER_OF_FACES] = {
    COLOR_WHITE,
    COLOR_MAGENTA,
    COLOR_GREEN,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_YELLOW
};


void draw_border(WINDOW *win);
WINDOW *create_window(
    int height_per, int width_per, int start_row, int start_col, int *next_row, int *next_col,
    WINDOW *win, bool border
);
void destroy_window(WINDOW *win);
void draw_cube(WINDOW *window, RubiksCube *cube);


/*
 * This function deals with the UI of the program. Takes an integer as an argument to determine
 * if to show debug info or not.
 */
int ui(RubiksCube *cube, int debug) {
    int ch, row, col;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    refresh();

    WINDOW *main = create_window(CUBE_WINDOW_HEIGHT_PER, CUBE_WINDOW_WIDTH_PER, 0, 0, &row, &col, stdscr, true);
    draw_cube(main, cube);
    WINDOW *help = create_window(HELP_WINDOW_HEIGHT_PER, HELP_WINDOW_WIDTH_PER, row, 0, &row, &col, stdscr, true);

    do {
        ch = getch();

        if (ch == KEY_RESIZE) {
            destroy_window(main);
            destroy_window(help);
            refresh();

            main = create_window(CUBE_WINDOW_HEIGHT_PER, CUBE_WINDOW_WIDTH_PER, 0, 0, &row, &col, stdscr, true);
            draw_cube(main, cube);
            help = create_window(HELP_WINDOW_HEIGHT_PER, HELP_WINDOW_WIDTH_PER, row, 0, &row, &col, stdscr, true);
        }

        if (ch == KEY_CTAB) {
        }
    } while(ch != 'q');

    destroy_window(main);
    destroy_window(help);
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
    int height_per, int width_per, int start_row, int start_col, int *next_row, int *next_col,
    WINDOW *win, bool border
) {
    int row, col;
    int begy, begx;
    if (width_per < 0 || width_per > 100 || height_per < 0 || height_per > 100) {
        return NULL;
    }

    getbegyx(win, begy, begx);
    getmaxyx(win, row, col);

    // Offsetting borders
    if (win != stdscr) {
        row -= 1;
        col -= 1;
        begy += 1;
        begx += 1;
    }

    int height = (height_per * row) / 100;
    int width = (width_per * col) / 100;

    // If there is no space for a new window (i.e. less than 3 width/height as after adding border
    // there will be no space for content), fill the remaining space, it will also prevent from
    // overextending a window
    int x_border_distance = col - (begy + start_col + width);
    int y_border_distance = row - (begx + start_row + height);
    if (x_border_distance < 3 || x_border_distance < 0) {
        width += x_border_distance;
    }
    if (y_border_distance < 3 || y_border_distance < 0) {
        height += y_border_distance;
    }

    WINDOW *window = newwin(height, width, begy + start_row, begx + start_col);
    *next_row = height + start_row;
    *next_col = width + start_col;

    if (border) {
        draw_border(window);
    }
    wrefresh(window);

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


/*
 * TODO: Make a function that makes and manages window grids. `rows` and `cols` define the grid size
 * and `format` defines how the windows are merged.
 * Ex: if I have a grid of 2 * 2, and I want to merge the two windows of the first row then I can
 * pass "01-2-3" to the `format` parameter. The windows are numbered from left-to-right top-to-bottom
 * Returns an array of windows with the indexes of the merged winodws pointing to the same merged
 * window.
 */


void draw_cube(WINDOW *window, RubiksCube *cube) {
    Color stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION];
    generate_stickers(cube, stickers);
    int row, col;
    int temp_row, temp_col;

    // This is just to set row and col for
    WINDOW *temp = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, 0, 0, &row, &col, window, true);
    destroy_window(temp);

    temp_row = row;
    temp_col = col;

    WINDOW *windows[NUMBER_OF_FACES];

    windows[UP] = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, 0, temp_col, &row, &col, window, true);
    windows[LEFT] = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, temp_row, 0, &row, &col, window, true);
    windows[FRONT] = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, temp_row, col, &row, &col, window, true);
    windows[RIGHT] = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, temp_row, col, &row, &col, window, true);
    windows[BACK] = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, temp_row, col, &row, &col, window, true);
    windows[DOWN] = create_window(FACE_HEIGHT_PER, FACE_WIDTH_PER, row, temp_col, &row, &col, window, true);

    row = 0;
    col = 0;
    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        waddch(windows[i], 'W');
        wrefresh(windows[i]);
        delwin(windows[i]);
    }

    wrefresh(window);
}
