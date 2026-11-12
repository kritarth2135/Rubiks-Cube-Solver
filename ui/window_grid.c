#include <stdlib.h>
#include <ncurses.h>

#include "ui/window_grid.h"

/*
 * Creates a window with an optional border
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

WindowGrid *allocate_window_grid(int rows, int cols, WINDOW *parent, bool parent_has_border) {
    WindowGrid *grid = malloc(sizeof(WindowGrid));
    if (grid == NULL) {
        return NULL;
    }

    grid->rows = rows;
    grid->cols = cols;
    grid->parent = parent;
    grid->parent_has_border = parent_has_border;
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

    if (grid->parent_has_border) {
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
