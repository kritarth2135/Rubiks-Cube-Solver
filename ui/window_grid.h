#ifndef WINDOW_GRID_H
#define WINDOW_GRID_H

#include <ncurses.h>

typedef struct WindowGrid {
    WINDOW *parent;
    bool parent_has_border;
    WINDOW ***windows;
    int rows;
    int *row_ratio;
    int cols;
    int *col_ratio;
    bool **layout;          // States which window will be blank and which will be used in the grid
} WindowGrid;

WINDOW *create_window(int height, int width, int begy, int begx, bool border);
void destroy_window(WINDOW *win, bool clear);
WindowGrid *allocate_window_grid(int rows, int cols, WINDOW *parent, bool parent_has_border);
int create_windows(WindowGrid *grid, bool border);
void destroy_windows(WindowGrid *grid, bool clear);
void deallocate_window_grid(WindowGrid *grid, bool clear);

#endif
