#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#define NUMBER_OF_FACES 6
#define FACE_DIMENSION 3
#define NUMBER_OF_DIRECTIONS 4

typedef enum {
    WHITE,
    GREEN,
    RED,
    BLUE,
    ORANGE,
    YELLOW
} colors;

typedef enum {
    UP,
    LEFT,
    FRONT,
    RIGHT,
    BACK,
    DOWN
} face_positions;

typedef struct face {
    colors color;
    colors facets[FACE_DIMENSION][FACE_DIMENSION];
} face;

typedef struct {
    face *faces[NUMBER_OF_FACES];
} rubiks_cube;

rubiks_cube * create_rubiks_cube(void);

void free_rubiks_cube(rubiks_cube *cube);

void print_cube(rubiks_cube *cube);

void make_move(rubiks_cube* cube, char* move);

#endif
