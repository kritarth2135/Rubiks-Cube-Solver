#include <stdlib.h>
#include <stdio.h>

#include "rubiks_cube.h"

const int FACE_DIMENSION = 3;

const Color solved_centre_positions[NUMBER_OF_COLORS] = {
    WHITE, GREEN, RED, ORANGE, BLUE, YELLOW
};

const CornerCubie solved_corner_positions[NUMBER_OF_CORNERS] = {
    UFL, UFR, UBL, UBR,
    DFL, DFR, DBL, DBR
};

const CornerOrientation solved_corner_orientaitons[NUMBER_OF_CORNERS] = {
    NO_TWIST, NO_TWIST, NO_TWIST, NO_TWIST,
    NO_TWIST, NO_TWIST, NO_TWIST, NO_TWIST
};

const EdgeCubie solved_edges_positions[NUMBER_OF_EDGES] = {
    UF, UL, UB, UR,
    FL, BL, BR, FR,
    DF, DL, DB, DR
};

const EdgeOrientation solved_edges_orientations[NUMBER_OF_EDGES] = {
    SOLVED, SOLVED, SOLVED, SOLVED,
    SOLVED, SOLVED, SOLVED, SOLVED,
    SOLVED, SOLVED, SOLVED, SOLVED
};

const Sticker CENTRE_STICKERS[NUMBER_OF_COLORS] = {
    W, G, R, B, O, Y
};

const Sticker CORNER_CUBIE_STICKERS[NUMBER_OF_CORNERS][CORNER_STICKERS] = {
    {W, R, G}, {W, B, R}, {W, G, O}, {W, O, B},
    {Y, G, R}, {Y, R, B}, {Y, O, G}, {Y, B, O}
};

const Sticker EDGE_CUBIE_STICKERS[NUMBER_OF_EDGES][EDGE_STICKERS] = {
    {W, R}, {W, G}, {W, O}, {W, B},
    {R, G}, {G, O}, {O, B}, {B, R},
    {Y, R}, {Y, G}, {Y, O}, {Y, B}
};

RubiksCube * create_rubiks_cube(void) {
    // Default orientation: Red face on front and White on top

    RubiksCube *cube = malloc(sizeof(RubiksCube));
    if (cube == NULL) {
        return NULL;
    }

    for (int i = 0; i < NUMBER_OF_COLORS; i++) {
        cube->centres[i] = solved_centre_positions[i];
    }

    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        cube->corner_positions[i] = solved_corner_positions[i];
        cube->corner_orientations[i] = solved_corner_orientaitons[i];
    }

    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        cube->edge_positions[i] = solved_edges_positions[i];
        cube->edge_orientations[i] = solved_edges_orientations[i];
    }

    return cube;
}

void print_cube(RubiksCube *cube) {
    for (int i = 0; i < FACE_DIMENSION; i++) {
        for (int j = 0; j < FACE_DIMENSION; j++) {

        }
    }
}