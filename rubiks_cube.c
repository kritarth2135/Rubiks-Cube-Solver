#include <stdlib.h>
#include <stdio.h>

#include "rubiks_cube.h"

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
    FL, FR, BL, BR,
    DF, DL, DB, DR
};

const EdgeOrientation solved_edges_orientations[NUMBER_OF_EDGES] = {
    SOLVED, SOLVED, SOLVED, SOLVED,
    SOLVED, SOLVED, SOLVED, SOLVED,
    SOLVED, SOLVED, SOLVED, SOLVED
};

RubiksCube * create_rubiks_cube(void) {
    // Default orientation: Red face on front and White on top

    RubiksCube *cube = malloc(sizeof(RubiksCube));
    if (cube == NULL) {
        return NULL;
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
    printf("cube");
}