#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <stdint.h>

#define NUMBER_OF_FACES 6
#define FACE_DIMENSION 3
#define NUMBER_OF_DIRECTIONS 4

typedef struct face {
    uint8_t color;
    uint8_t facets[FACE_DIMENSION][FACE_DIMENSION];
} face;

typedef struct {
    face *faces[NUMBER_OF_FACES];
} rubiks_cube;

rubiks_cube * create_rubiks_cube(void);

void free_rubiks_cube(rubiks_cube *cube);

#endif
