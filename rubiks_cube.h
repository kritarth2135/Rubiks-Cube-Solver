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

// Colors of the cube
const int WHITE = 0;
const int GREEN = 1;
const int RED = 2;
const int BLUE = 3;
const int ORANGE = 4;
const int YELLOW = 5;

// Location of facets in a face
const int CENTRE[] = {1, 1};
const int NORTH[] = {0, 1};
const int SOUTH[] = {2, 1};
const int EAST[] = {1, 2};
const int WEST[] = {1, 0};
const int NORTH_EAST[] = {0, 2};
const int NORTH_WEST[] = {0, 0};
const int SOUTH_EAST[] = {2, 2};
const int SOUTH_WEST[] = {2, 0};

rubiks_cube * create_rubiks_cube(void);

#endif
