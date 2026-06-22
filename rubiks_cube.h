#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <stdint.h>

const int NUMBER_OF_FACES = 6;
const int FACE_DIMENSION = 3;

typedef struct face {
    uint8_t color;
    uint8_t facets[FACE_DIMENSION][FACE_DIMENSION];
} face;

typedef struct {
    face *faces[NUMBER_OF_FACES];
} rubiks_cube;

// Colors
const uint8_t WHITE = 0;
const uint8_t GREEN = 1;
const uint8_t RED = 2;
const uint8_t BLUE = 3;
const uint8_t ORANGE = 4;
const uint8_t YELLOW = 5;

const uint8_t COLORS[NUMBER_OF_FACES] = {WHITE, GREEN, RED, BLUE, ORANGE, YELLOW};

// Directions
const int NUMBER_OF_DIRECTIONS = 4;

const int NORTH = 0;
const int SOUTH = 1;
const int EAST = 2;
const int WEST = 3;

// Relative faces in order: north, south, east, west
const uint8_t RELATIVE_FACES[NUMBER_OF_FACES][NUMBER_OF_DIRECTIONS] = {
    {ORANGE, RED, BLUE, GREEN},
    {WHITE, YELLOW, RED, ORANGE},
    {WHITE, YELLOW, BLUE, GREEN},
    {WHITE, YELLOW, ORANGE, RED},
    {WHITE, YELLOW, GREEN, BLUE},
    {RED, ORANGE, BLUE, GREEN}
};

// Facet locations
const int CENTRE_FACET[] = {1, 1};
const int NORTH_FACET[] = {0, 1};
const int SOUTH_FACET[] = {2, 1};
const int EAST_FACET[] = {1, 2};
const int WEST_FACET[] = {1, 0};
const int NORTH_EAST_FACET[] = {0, 2};
const int NORTH_WEST_FACET[] = {0, 0};
const int SOUTH_EAST_FACET[] = {2, 2};
const int SOUTH_WEST_FACET[] = {2, 0};

rubiks_cube * create_rubiks_cube(void);

#endif
