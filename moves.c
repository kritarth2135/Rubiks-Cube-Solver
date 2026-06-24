#include <stdbool.h>

#include "rubiks_cube.h"

const int NORTH = 0;
const int SOUTH = 1;
const int EAST = 2;
const int WEST = 3;

// Relative faces in order: north, south, east, west
const face_positions RELATIVE_FACES[NUMBER_OF_FACES][NUMBER_OF_DIRECTIONS] = {
    {BACK, FRONT, RIGHT, LEFT},
    {UP, DOWN, FRONT, BACK},
    {UP, DOWN, RIGHT, LEFT},
    {UP, DOWN, BACK, FRONT},
    {UP, DOWN, LEFT, RIGHT},
    {FRONT, BACK, LEFT, RIGHT}
};

// Facet locations
const int NORTH_FACET[] = {0, 1};
const int SOUTH_FACET[] = {2, 1};
const int EAST_FACET[] = {1, 2};
const int WEST_FACET[] = {1, 0};
const int NORTH_EAST_FACET[] = {0, 2};
const int NORTH_WEST_FACET[] = {0, 0};
const int SOUTH_EAST_FACET[] = {2, 2};
const int SOUTH_WEST_FACET[] = {2, 0};

const char* ALL_POSSIBLE_MOVES[] = {
    "U", "D", "R", "L", "F", "B",
    "U'", "D'", "R'", "L'", "F'", "B'",
    "U2", "D2", "R2", "L2", "F2", "B2",
    "Uw", "Dw", "Rw", "Lw", "Fw", "Bw",
    "Uw'", "Dw'", "Rw'", "Lw'", "Fw'", "Bw'",
    "Uw2", "Dw2", "Rw2", "Lw2", "Fw2", "Bw2",
    "M", "E", "S",
    "x", "y", "z"
};

void U(rubiks_cube* cube, bool prime, bool two) {
}

void D(rubiks_cube* cube, bool prime, bool two) {
}

void R(rubiks_cube* cube, bool prime, bool two) {
}

void L(rubiks_cube* cube, bool prime, bool two) {
}

void F(rubiks_cube* cube, bool prime, bool two) {
}

void B(rubiks_cube* cube, bool prime, bool two) {
}

void Uw(rubiks_cube* cube, bool prime, bool two) {
}

void Dw(rubiks_cube* cube, bool prime, bool two) {
}

void Rw(rubiks_cube* cube, bool prime, bool two) {
}

void Lw(rubiks_cube* cube, bool prime, bool two) {
}

void Fw(rubiks_cube* cube, bool prime, bool two) {
}

void Bw(rubiks_cube* cube, bool prime, bool two) {
}

void M(rubiks_cube* cube) {
}

void E(rubiks_cube* cube) {
}

void S(rubiks_cube* cube) {
}

void x(rubiks_cube* cube) {
}

void y(rubiks_cube* cube) {
}

void z(rubiks_cube* cube) {
}

void make_move(rubiks_cube* cube, char* move) {
}