#include <stdbool.h>
#include <stdlib.h>

#include <ncurses.h>

#include "cube/cube.h"
#include "ui.h"

#define CUBE_WINDOW_ROWS 3
#define CUBE_WINDOW_COLS 4


typedef struct WindowGrid {
    WINDOW *parent;
    WINDOW ***windows;
    int rows;
    int *row_ratio;
    int cols;
    int *col_ratio;
    bool **layout;          // States which window will be blank and which will be used in the grid
} WindowGrid;


// void draw_border(WINDOW *win);
WINDOW *create_window(int height, int width, int begy, int begx, bool border);
void destroy_window(WINDOW *win, bool clear);
WindowGrid *allocate_window_grid(int rows, int cols, WINDOW *parent);
int create_windows(WindowGrid *grid, bool border);
void destroy_windows(WindowGrid *grid, bool clear);
void deallocate_window_grid(WindowGrid *grid, bool clear);
int draw_cube(WINDOW *window, RubiksCube *cube);


/*
 * This function deals with the UI of the program. Takes an integer as an argument to determine
 * if to show debug info or not.
 */
int ui(RubiksCube *cube, int debug) {
    int ch;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    if (!has_colors()) {
        return 1;
    }
    start_color();
    use_default_colors();

    if (can_change_color()) {
        init_color(COLOR_MAGENTA, 1000, 647, 0);
    }

    refresh();

    init_pair(WHITE + 1, COLOR_WHITE, COLOR_WHITE);
    init_pair(ORANGE + 1, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(GREEN + 1, COLOR_GREEN, COLOR_GREEN);
    init_pair(RED + 1, COLOR_RED, COLOR_RED);
    init_pair(BLUE + 1, COLOR_BLUE, COLOR_BLUE);
    init_pair(YELLOW + 1, COLOR_YELLOW, COLOR_YELLOW);

    WindowGrid *main_grid = allocate_window_grid(2, 1, stdscr);
    if (main_grid == NULL) {
        return 1;
    }
    main_grid->layout[0][0] = true;
    main_grid->layout[1][0] = true;
    main_grid->row_ratio[0] = 3;

    if (create_windows(main_grid, true)) {
        return 1;
    }
    if (draw_cube(main_grid->windows[0][0], cube)) {
        return 1;
    }

    do {
        ch = getch();

        if (ch == KEY_RESIZE) {
            destroy_windows(main_grid, true);
            refresh();

            if (create_windows(main_grid, true)) {
                return 1;
            }
            if (draw_cube(main_grid->windows[0][0], cube)) {
                return 1;
            }
        }

        // if (ch == '\t') {
        // }
    } while(ch != 'q');

    deallocate_window_grid(main_grid, true);
    return endwin();
}


/*
 * Draws a simple border around the window (I didn't know about box function when I wrote this...)
 */
// void draw_border(WINDOW *win) {
//     int row, col;
//     getmaxyx(win, row, col);
//
//     for (int i = 0; i < row; i++) {
//         wmove(win, i, 0);
//         if (i == 0 || i == row - 1) {
//             waddch(win, i == 0 ? ACS_ULCORNER: ACS_LLCORNER);
//             for (int j = 1; j < col - 1; j++) {
//                 waddch(win, ACS_HLINE);
//             }
//             waddch(win, i == 0 ? ACS_URCORNER: ACS_LRCORNER);
//         }
//         else {
//             waddch(win, ACS_VLINE);
//             mvwaddch(win, i, col - 1, ACS_VLINE);
//         }
//     }
//
//     wmove(win, 1, 1);
// }


/*
 * Creates a window by taking the percentage of height and width as parameter. It also sets next_row
 * and next_col arguments to the immediate row and col indexes at the end of window.
 */
WINDOW *create_window(int height, int width, int begy, int begx, bool border) {
    WINDOW *window = newwin(height, width, begy, begx);
    if (window == NULL) {
        return NULL;
    }

    if (border) {
        box(window, 0, 0);
    }

    wmove(window, 1, 1);
    wrefresh(window);

    return window;
}


/*
 * Destroys a window by clearing it from the stdscr
 */
void destroy_window(WINDOW *win, bool clear) {
    if (clear) {
        wclear(win);
        wrefresh(win);
    }
    delwin(win);
}


/*
 * These function creates and manages a window grid, it arranges multiple windows in a 2D grid,
 * takes `rows` and `cols` to define the grid size. The `layout` states which windows will be
 * there and which parts of the grid will be empty. Lastly `row_ratio` and `col_ratio` describes
 * the ratio of the height and widths of the grid.
 */


WindowGrid *allocate_window_grid(int rows, int cols, WINDOW *parent) {
    WindowGrid *grid = malloc(sizeof(WindowGrid));
    if (grid == NULL) {
        return NULL;
    }

    grid->rows = rows;
    grid->cols = cols;
    grid->parent = parent;
    grid->windows = NULL;

    grid->row_ratio = malloc(sizeof(int) * rows);
    if (grid->row_ratio == NULL) {
        free(grid);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        grid->row_ratio[i] = 1;
    }

    grid->col_ratio = malloc(sizeof(int) * cols);
    if (grid->col_ratio == NULL) {
        free(grid->row_ratio);
        free(grid);
        return NULL;
    }
    for (int i = 0; i < cols; i++) {
        grid->col_ratio[i] = 1;
    }

    grid->layout = malloc(sizeof(bool *) * rows);
    if (grid->layout == NULL) {
        free(grid->row_ratio);
        free(grid->col_ratio);
        free(grid);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        grid->layout[i] = malloc(sizeof(bool) * cols);
        if (grid->layout[i] == NULL) {
            for (int j = i - 1; j >= 0; j--) {
                free(grid->layout[j]);
            }
            free(grid->layout);
            free(grid->row_ratio);
            free(grid->col_ratio);
            free(grid);
            return NULL;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid->layout[i][j] = true;
        }
    }

    return grid;
}


int create_windows(WindowGrid *grid, bool border) {
    grid->windows = malloc(sizeof(WINDOW **) * grid->rows);
    if (grid->windows == NULL) {
        return 1;
    }
    for (int i = 0; i < grid->rows; i++) {
        grid->windows[i] = malloc(sizeof(WINDOW *) * grid->cols);
        if (grid->windows[i] == NULL) {
            for (int j = i - 1; j >= 0; j--) {
                free(grid->windows[j]);
            }
            free(grid->windows);
            return 1;
        }
    }

    int beg_row, beg_col, rows, cols;
    getbegyx(grid->parent, beg_row, beg_col);
    getmaxyx(grid->parent, rows, cols);

    if (grid->parent != stdscr) {
        beg_row += 1;
        beg_col += 1;
        rows -= 2;
        cols -= 2;
    }

    int partsy = 0;
    int partsx = 0;
    for (int i = 0; i < grid->rows; i ++) {
        partsy += grid->row_ratio[i];
    }
    for (int i = 0; i < grid->cols; i ++) {
        partsx += grid->col_ratio[i];
    }

    int curr_row = beg_row;
    int curr_col = beg_col;

    int width = cols / partsx;
    int height = rows / partsy;

    for (int i = 0; i < grid->rows; i++) {
        for(int j = 0; j < grid->cols; j++) {
            if (!grid->layout[i][j]) {
                grid->windows[i][j] = NULL;
            }
            else {
                grid->windows[i][j] = create_window(
                    height * grid->row_ratio[i], width * grid->col_ratio[j], curr_row, curr_col, border
                );
            }
            curr_col += width * grid->col_ratio[j];
        }
        curr_col = beg_col;
        curr_row += height * grid->row_ratio[i];
    }

    return 0;
}


void destroy_windows(WindowGrid *grid, bool clear) {
    for (int i = 0; i < grid->rows; i++) {
        for(int j = 0; j < grid->cols; j++) {
            destroy_window(grid->windows[i][j], clear);
        }
        free(grid->windows[i]);
    }
    free(grid->windows);
}


void deallocate_window_grid(WindowGrid *grid, bool clear) {
    for (int i = 0; i < grid->rows; i++) {
        free(grid->layout[i]);
    }
    free(grid->layout);
    free(grid->row_ratio);
    free(grid->col_ratio);
    destroy_windows(grid, clear);
    free(grid);
}


int draw_cube(WINDOW *window, RubiksCube *cube) {
    Color stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION];
    generate_stickers(cube, stickers);

    WindowGrid *grid = allocate_window_grid(CUBE_WINDOW_ROWS, CUBE_WINDOW_COLS, window);
    if (grid == NULL) {
        return 1;
    }

    bool layout[CUBE_WINDOW_ROWS][CUBE_WINDOW_COLS] = {
        {false, true, false, false},
        {true, true, true, true},
        {false, true, false, false}
    };

    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            grid->layout[i][j] = layout[i][j];
        }
    }

    create_windows(grid, true);

    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            if (grid->windows[i][j] == NULL) {
                continue;
            }
            WindowGrid *temp = allocate_window_grid(FACE_DIMENSION, FACE_DIMENSION, grid->windows[i][j]);
            if (temp == NULL) {
                return 1;
            }
            create_windows(temp, true);
        }
    }

    deallocate_window_grid(grid, false);

    return 0;
}
