#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

const int NUMBER_OF_FACES = 6;
const int FACE_DIMENSION = 3;

typedef struct face {
    int color;
    int facets[FACE_DIMENSION][FACE_DIMENSION];
    struct face *adjacent_face_north;
    struct face *adjacent_face_south;
    struct face *adjacent_face_east;
    struct face *adjacent_face_west;
} face;

typedef struct {
    face *faces[NUMBER_OF_FACES];
} rubiks_cube;

// Colors
const int WHITE = 0;
const int GREEN = 1;
const int RED = 2;
const int BLUE = 3;
const int ORANGE = 4;
const int YELLOW = 5;

// Directions
const int NUMBER_OF_DIRECTIONS = 4;
const int NORTH = 0;
const int SOUTH = 1;
const int EAST = 2;
const int WEST = 3;

// Relative faces in order: north, south, east, west
const int RELATIVE_FACES[NUMBER_OF_FACES][NUMBER_OF_DIRECTIONS] = {
    {ORANGE, RED, BLUE, GREEN},
    {WHITE, YELLOW, RED, ORANGE},
    {WHITE, YELLOW, BLUE, GREEN},
    {WHITE, YELLOW, ORANGE, RED},
    {WHITE, YELLOW, GREEN, BLUE},
    {RED, ORANGE, BLUE, GREEN}
};

// Facets
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
