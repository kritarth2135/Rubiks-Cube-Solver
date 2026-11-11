#include <stdbool.h>
#include <stdlib.h>

#include <ncurses.h>

#include "cube/cube.h"
#include "cube/moves.h"
#include "solver/solver.h"
#include "ui.h"

#define CUBE_WINDOW_ROWS 3
#define CUBE_WINDOW_COLS 4


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


const Position FACE_MAPPING[CUBE_WINDOW_ROWS][CUBE_WINDOW_COLS] = {
    {-1, UP, -1, -1},
    {LEFT, FRONT, RIGHT, BACK},
    {-1, DOWN, -1, -1}
};


// void draw_border(WINDOW *win);
WINDOW *create_window(int height, int width, int begy, int begx, bool border);
void destroy_window(WINDOW *win, bool clear);
WindowGrid *allocate_window_grid(int rows, int cols, WINDOW *parent, bool parent_has_border);
int create_windows(WindowGrid *grid, bool border);
void destroy_windows(WindowGrid *grid, bool clear);
void deallocate_window_grid(WindowGrid *grid, bool clear);
int draw_cube(WINDOW *window, RubiksCube *cube);


/*
 * This function deals with the UI of the program. Takes an integer as an argument to determine
 * if to show debug info or not.
 */
int ui(
    RubiksCube *cube, uint8_t *corner_db, uint8_t *first_edge_db, uint8_t *second_edge_db,
    uint8_t *edge_position_db, Move *solution_array, int *solution_len, int debug
) {
    RubiksCube *solved_cube = create_rubiks_cube();
    if (solved_cube == NULL) {
        return 1;
    }
    int ch;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    meta (stdscr, TRUE);
    noecho();

    if (!has_colors()) {
        return 1;
    }
    start_color();
    use_default_colors();

    if (can_change_color()) {
        init_color(COLOR_MAGENTA, 1000, 647, 0);
        init_color(COLOR_YELLOW, 1000, 1000, 0);
    }

    refresh();

    init_pair(WHITE + 1, COLOR_BLACK, COLOR_WHITE);
    init_pair(ORANGE + 1, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(GREEN + 1, COLOR_BLACK, COLOR_GREEN);
    init_pair(RED + 1, COLOR_BLACK, COLOR_RED);
    init_pair(BLUE + 1, COLOR_BLACK, COLOR_BLUE);
    init_pair(YELLOW + 1, COLOR_BLACK, COLOR_YELLOW);

    WINDOW *cube_window = NULL;
    WINDOW *info_window = NULL;
    WINDOW *debug_window = NULL;
    WindowGrid *main_grid = NULL;
    WindowGrid *debug_grid = NULL;

    if (debug) {
        debug_grid = allocate_window_grid(1, 2, stdscr, false);
        if (debug_grid == NULL) {
            return 1;
        }
        debug_grid->col_ratio[0] = 3;
        if (create_windows(debug_grid, false)) {
            return 1;
        }
        debug_window = debug_grid->windows[0][1];
        box(debug_window, 0, 0);
        wrefresh(debug_window);
        main_grid = allocate_window_grid(2, 1, debug_grid->windows[0][0], false);
    }
    else {
        main_grid = allocate_window_grid(2, 1, stdscr, false);
    }
    if (main_grid == NULL) {
        return 1;
    }
    main_grid->row_ratio[0] = 3;

    if (create_windows(main_grid, true)) {
        return 1;
    }
    cube_window = main_grid->windows[0][0];
    info_window = main_grid->windows[1][0];

    if (draw_cube(cube_window, cube)) {
        return 1;
    }
    wmove(info_window, 1, 1);

    do {
        ch = getch();

        if (ch == KEY_RESIZE) {
            destroy_windows(main_grid, true);
            refresh();

            if (debug) {
                if (create_windows(debug_grid, true)) {
                    return 1;
                }
                box(debug_window, 0, 0);
                wrefresh(debug_window);
            }

            if (create_windows(main_grid, true)) {
                return 1;
            }
        }
        else if (ch == 'a') {
            korfs_solver(cube, solved_cube, corner_db, first_edge_db, second_edge_db, edge_position_db, solution_array, solution_len);
            wclear(info_window);
            box(info_window, 0, 0);
            wmove(info_window, 1, 1);
            wprintw(info_window, "Solution: ");
            if (*solution_len == 0) {
                wprintw(info_window, "Already Solved");
            }
            else {
                for (int i = 0; i < *solution_len; i++) {
                    make_move(cube, solution_array[i]);
                    wprintw(info_window, "%s, ", STRING_REPRESENTATION_OF_MOVES[solution_array[i]]);
                }
                *solution_len = 0;
            }
            wrefresh(info_window);
        }
        else if (ch == 'z') {
            free(cube);
            cube = create_rubiks_cube();
            if (cube == NULL) {
                return 1;
            }
        }
        else {
            if (ch == 'w') {
                int next_ch = getch();
                switch(next_ch) {
                    case 'u': make_move(cube, U_WIDE_NORMAL); break;
                    case 'd': make_move(cube, D_WIDE_NORMAL); break;
                    case 'r': make_move(cube, R_WIDE_NORMAL); break;
                    case 'l': make_move(cube, L_WIDE_NORMAL); break;
                    case 'f': make_move(cube, F_WIDE_NORMAL); break;
                    case 'b': make_move(cube, B_WIDE_NORMAL); break;
                }
            }
            else if (ch == 'W') {
                int next_ch = getch();
                switch(next_ch) {
                    case 'u': make_move(cube, U_WIDE_PRIME); break;
                    case 'd': make_move(cube, D_WIDE_PRIME); break;
                    case 'r': make_move(cube, R_WIDE_PRIME); break;
                    case 'l': make_move(cube, L_WIDE_PRIME); break;
                    case 'f': make_move(cube, F_WIDE_PRIME); break;
                    case 'b': make_move(cube, B_WIDE_PRIME); break;
                }
            }
            else {
                switch(ch) {
                    case 'u': make_move(cube, U_NORMAL); break;
                    case 'd': make_move(cube, D_NORMAL); break;
                    case 'r': make_move(cube, R_NORMAL); break;
                    case 'l': make_move(cube, L_NORMAL); break;
                    case 'f': make_move(cube, F_NORMAL); break;
                    case 'b': make_move(cube, B_NORMAL); break;

                    case 'U': make_move(cube, U_PRIME); break;
                    case 'D': make_move(cube, D_PRIME); break;
                    case 'R': make_move(cube, R_PRIME); break;
                    case 'L': make_move(cube, L_PRIME); break;
                    case 'F': make_move(cube, F_PRIME); break;
                    case 'B': make_move(cube, B_PRIME); break;

                    case 'm': make_move(cube, M_NORMAL); break;
                    case 'e': make_move(cube, E_NORMAL); break;
                    case 's': make_move(cube, S_NORMAL); break;

                    case 'M': make_move(cube, M_PRIME); break;
                    case 'E': make_move(cube, E_PRIME); break;
                    case 'S': make_move(cube, S_PRIME); break;

                    case 'x': make_move(cube, X_NORMAL); break;
                    case 'y': make_move(cube, Y_NORMAL); break;
                    case 'z': make_move(cube, Z_NORMAL); break;

                    case 'X': make_move(cube, X_PRIME); break;
                    case 'Y': make_move(cube, Y_PRIME); break;
                    case 'Z': make_move(cube, Z_PRIME); break;
                }
            }
        }

        if (draw_cube(cube_window, cube)) {
            return 1;
        }

        wmove(info_window, 1, 1);
    } while(ch != 'q');

    if (debug) {
        deallocate_window_grid(debug_grid, true);
    }
    deallocate_window_grid(main_grid, true);
    free(solved_cube);
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


int draw_cube(WINDOW *window, RubiksCube *cube) {
    Color stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION];
    generate_stickers(cube, stickers);

    WindowGrid *grid = allocate_window_grid(CUBE_WINDOW_ROWS, CUBE_WINDOW_COLS, window, true);
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
            WindowGrid *temp = allocate_window_grid(FACE_DIMENSION, FACE_DIMENSION, grid->windows[i][j], true);
            if (temp == NULL) {
                return 1;
            }
            create_windows(temp, false);

            for (int k = 0; k < FACE_DIMENSION; k++) {
                for (int l = 0; l < FACE_DIMENSION; l++) {
                    WINDOW *curr_win = temp->windows[k][l];
                    wmove(curr_win, 0, 0);

                    int pair = stickers[FACE_MAPPING[i][j]][k][l];
                    wattron(curr_win, COLOR_PAIR(pair + 1));
                    int y, x;
                    getmaxyx(curr_win, y, x);
                    for (int m = 0; m < y; m++) {
                        for (int n = 0; n < x; n++) {
                            waddch(curr_win, ' ');
                        }
                        waddch(curr_win, '\n');
                    }
                    box(curr_win, 0, 0);
                    wattroff(curr_win, COLOR_PAIR(pair + 1));
                    wrefresh(curr_win);
                }
            }

            deallocate_window_grid(temp, false);
        }
    }

    deallocate_window_grid(grid, false);

    return 0;
}
