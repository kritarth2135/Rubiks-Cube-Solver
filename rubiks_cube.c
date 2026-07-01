#include <stdlib.h>
#include <stdio.h>

#include "rubiks_cube.h"

const int FACE_DIMENSION = 3;

const Color solved_centre_positions[NUMBER_OF_COLORS] = {
    WHITE, GREEN, RED, BLUE, ORANGE, YELLOW
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

const Sticker CORNER_CUBIE_STICKERS[NUMBER_OF_CORNERS][NUMBER_OF_CORNER_STICKERS] = {
    {W, R, G}, {W, B, R}, {W, G, O}, {W, O, B},
    {Y, G, R}, {Y, R, B}, {Y, O, G}, {Y, B, O}
};

const Sticker EDGE_CUBIE_STICKERS[NUMBER_OF_EDGES][NUMBER_OF_EDGE_STICKERS] = {
    {W, R}, {W, G}, {W, O}, {W, B},
    {G, R}, {G, O}, {B, O}, {B, R},
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

Sticker get_corner_sticker(RubiksCube *cube, CornerCubie cubie, CornerStickerPosition position) {
    return CORNER_CUBIE_STICKERS[cube->corner_positions[cubie]][(position + cube->corner_orientations[cubie]) % NUMBER_OF_CORNER_STICKERS];
}

Sticker get_edge_sticker(RubiksCube *cube, EdgeCubie cubie, EdgeStickerPosition position) {
    return EDGE_CUBIE_STICKERS[cube->edge_positions[cubie]][(position + cube->edge_orientations[cubie]) % NUMBER_OF_EDGE_STICKERS];
}

// Using the cubie representation to get all the facets
void generate_stickers(RubiksCube *cube, Sticker stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION]) {
    stickers[UP][0][0] = get_corner_sticker(cube, UBL, CORNER_TOP);
    stickers[UP][0][1] = get_edge_sticker(cube, UB, EDGE_TOP);
    stickers[UP][0][2] = get_corner_sticker(cube, UBR, CORNER_TOP);
    stickers[UP][1][0] = get_edge_sticker(cube, UL, EDGE_TOP);
    stickers[UP][1][1] = CENTRE_STICKERS[cube->centres[UP]];
    stickers[UP][1][2] = get_edge_sticker(cube, UR, EDGE_TOP);
    stickers[UP][2][0] = get_corner_sticker(cube, UFL, CORNER_TOP);
    stickers[UP][2][1] = get_edge_sticker(cube, UF, EDGE_TOP);
    stickers[UP][2][2] = get_corner_sticker(cube, UFR, CORNER_TOP);

    stickers[LEFT][0][0] = get_corner_sticker(cube, UBL, CORNER_RIGHT);
    stickers[LEFT][0][1] = get_edge_sticker(cube, UL, EDGE_DOWN);
    stickers[LEFT][0][2] = get_corner_sticker(cube, UFL, CORNER_LEFT);
    stickers[LEFT][1][0] = get_edge_sticker(cube, BL, EDGE_TOP);
    stickers[LEFT][1][1] = CENTRE_STICKERS[cube->centres[LEFT]];
    stickers[LEFT][1][2] = get_edge_sticker(cube, FL, EDGE_TOP);
    stickers[LEFT][2][0] = get_corner_sticker(cube, DBL, CORNER_LEFT);
    stickers[LEFT][2][1] = get_edge_sticker(cube, DL, EDGE_DOWN);
    stickers[LEFT][2][2] = get_corner_sticker(cube, DFL, CORNER_RIGHT);

    stickers[FRONT][0][0] = get_corner_sticker(cube, UFL, CORNER_RIGHT);
    stickers[FRONT][0][1] = get_edge_sticker(cube, UF, EDGE_DOWN);
    stickers[FRONT][0][2] = get_corner_sticker(cube, UFR, CORNER_LEFT);
    stickers[FRONT][1][0] = get_edge_sticker(cube, FL, EDGE_DOWN);
    stickers[FRONT][1][1] = CENTRE_STICKERS[cube->centres[FRONT]];
    stickers[FRONT][1][2] = get_edge_sticker(cube, FR, EDGE_DOWN);
    stickers[FRONT][2][0] = get_corner_sticker(cube, DFL, CORNER_LEFT);
    stickers[FRONT][2][1] = get_edge_sticker(cube, DF, EDGE_DOWN);
    stickers[FRONT][2][2] = get_corner_sticker(cube, DFR, CORNER_RIGHT);

    stickers[RIGHT][0][0] = get_corner_sticker(cube, UFR, CORNER_RIGHT);
    stickers[RIGHT][0][1] = get_edge_sticker(cube, UR, EDGE_DOWN);
    stickers[RIGHT][0][2] = get_corner_sticker(cube, UBR, CORNER_LEFT);
    stickers[RIGHT][1][0] = get_edge_sticker(cube, FR, EDGE_TOP);
    stickers[RIGHT][1][1] = CENTRE_STICKERS[cube->centres[RIGHT]];
    stickers[RIGHT][1][2] = get_edge_sticker(cube, BR, EDGE_TOP);
    stickers[RIGHT][2][0] = get_corner_sticker(cube, DFR, CORNER_LEFT);
    stickers[RIGHT][2][1] = get_edge_sticker(cube, DR, EDGE_DOWN);
    stickers[RIGHT][2][2] = get_corner_sticker(cube, DBR, CORNER_RIGHT);

    stickers[BACK][0][0] = get_corner_sticker(cube, UBR, CORNER_RIGHT);
    stickers[BACK][0][1] = get_edge_sticker(cube, UB, EDGE_DOWN);
    stickers[BACK][0][2] = get_corner_sticker(cube, UBL, CORNER_LEFT);
    stickers[BACK][1][0] = get_edge_sticker(cube, BR, EDGE_DOWN);
    stickers[BACK][1][1] = CENTRE_STICKERS[cube->centres[BACK]];
    stickers[BACK][1][2] = get_edge_sticker(cube, BL, EDGE_DOWN);
    stickers[BACK][2][0] = get_corner_sticker(cube, DBR, CORNER_LEFT);
    stickers[BACK][2][1] = get_edge_sticker(cube, DB, EDGE_DOWN);
    stickers[BACK][2][2] = get_corner_sticker(cube, DBL, CORNER_RIGHT);

    stickers[DOWN][0][0] = get_corner_sticker(cube, DFL, CORNER_TOP);
    stickers[DOWN][0][1] = get_edge_sticker(cube, DF, EDGE_TOP);
    stickers[DOWN][0][2] = get_corner_sticker(cube, DFR, CORNER_TOP);
    stickers[DOWN][1][0] = get_edge_sticker(cube, DL, EDGE_TOP);
    stickers[DOWN][1][1] = CENTRE_STICKERS[cube->centres[DOWN]];
    stickers[DOWN][1][2] = get_edge_sticker(cube, DR, EDGE_TOP);
    stickers[DOWN][2][0] = get_corner_sticker(cube, DBL, CORNER_TOP);
    stickers[DOWN][2][1] = get_edge_sticker(cube, DB, EDGE_TOP);
    stickers[DOWN][2][2] = get_corner_sticker(cube, DBR, CORNER_TOP);
}

void print_cube(RubiksCube *cube) {
    Sticker stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION];
    generate_stickers(cube, stickers);

    for (int i = 0; i < FACE_DIMENSION; i++) {
        printf("      ");
        for (int j = 0; j < FACE_DIMENSION; j++) {
            printf("%c ", stickers[UP][i][j]);
        }
        printf("\n");
    }

    for (int j = 0; j < FACE_DIMENSION; j++) {
        for (int i = LEFT; i < DOWN; i++) {
            for (int k = 0; k < FACE_DIMENSION; k++) {
                printf("%c ", stickers[i][j][k]);
            }
        }
        printf("\n");
    }

    for (int i = 0; i < FACE_DIMENSION; i++) {
        printf("      ");
        for (int j = 0; j < FACE_DIMENSION; j++) {
            printf("%c ", stickers[DOWN][i][j]);
        }
        printf("\n");
    }
}