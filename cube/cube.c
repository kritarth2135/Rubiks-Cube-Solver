#include <stdlib.h>
#include <stdio.h>

#include "cube/cube.h"

#define FACE_DIMENSION 3


typedef enum {
    CORNER_TOP,
    CORNER_RIGHT,
    CORNER_LEFT
} CornerStickerPosition;

typedef enum {
    EDGE_TOP,
    EDGE_DOWN,
} EdgeStickerPosition;


const Position DEFAULT_CENTRE_POSITIONS[NUMBER_OF_FACES] = {
    UP, LEFT, FRONT, RIGHT, BACK, DOWN
};

// These are the default colors of the cube faces in order of the
// DEFAULT_CENTRE_POSITIONS array
const Color DEFAULT_FACE_COLORS[NUMBER_OF_FACES] = {
    WHITE, ORANGE, GREEN, RED, BLUE, YELLOW
};

const CornerPosition DEFAULR_CORNER_POSITIONS[NUMBER_OF_CORNERS] = {
    UFL, UFR, UBL, UBR,
    DFL, DFR, DBL, DBR
};

const CornerOrientation DEFAULT_CORNER_ORIENTATIONS[NUMBER_OF_CORNERS] = {
    NO_TWIST, NO_TWIST, NO_TWIST, NO_TWIST,
    NO_TWIST, NO_TWIST, NO_TWIST, NO_TWIST
};

const EdgePosition DEFAULT_EDGE_POSITIONS[NUMBER_OF_EDGES] = {
    UF, UL, UB, UR,
    FL, BL, BR, FR,
    DF, DL, DB, DR
};

const EdgeOrientation DEFAULT_EDGE_ORIENTATIONS[NUMBER_OF_EDGES] = {
    SOLVED, SOLVED, SOLVED, SOLVED,
    SOLVED, SOLVED, SOLVED, SOLVED,
    SOLVED, SOLVED, SOLVED, SOLVED
};

// Must match with the order in Color enum
const char STICKERS[NUMBER_OF_COLORS] = {
    'W', 'O', 'G', 'R', 'B', 'Y'
};

// Must match the order in CornerPosition enum
const Position CORNER_CUBIE_STICKER_POSITIONS[NUMBER_OF_CORNERS][NUMBER_OF_CORNER_SIDES] = {
    {UP, FRONT, LEFT}, {UP, RIGHT, FRONT}, {UP, LEFT, BACK}, {UP, BACK, RIGHT},
    {DOWN, LEFT, FRONT}, {DOWN, FRONT, RIGHT}, {DOWN, BACK, LEFT}, {DOWN, RIGHT, BACK}
};

// Must match the order in EdgePosition enum
const Position EDGE_CUBIE_STICKER_POSITIONS[NUMBER_OF_EDGES][NUMBER_OF_EDGE_SIDES] = {
    {UP, FRONT}, {UP, LEFT}, {UP, BACK}, {UP, RIGHT},
    {LEFT, FRONT}, {LEFT, BACK}, {RIGHT, BACK}, {RIGHT, FRONT},
    {DOWN, FRONT}, {DOWN, LEFT}, {DOWN, BACK}, {DOWN, RIGHT}
};



/*
 * Creates and returns a fresh new cube in solved state according to the DEFAULT
 * values defined above.
 */
RubiksCube *
create_rubiks_cube(
    void
) {
    RubiksCube *cube = malloc(sizeof(RubiksCube));
    if (cube == NULL) {
        return NULL;
    }

    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        cube->face_colors[i] = DEFAULT_FACE_COLORS[i];
        cube->centre_positions[i] = DEFAULT_CENTRE_POSITIONS[i];
    }
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        cube->corner_positions[i] = DEFAULR_CORNER_POSITIONS[i];
        cube->corner_orientations[i] = DEFAULT_CORNER_ORIENTATIONS[i];
    }
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        cube->edge_positions[i] = DEFAULT_EDGE_POSITIONS[i];
        cube->edge_orientations[i] = DEFAULT_EDGE_ORIENTATIONS[i];
    }

    return cube;
}

/*
 * Creates and returns a copy of the cube passed as the argument.
 */
RubiksCube *
copy_cube(
    RubiksCube *cube
) {
    RubiksCube *copy = malloc(sizeof(RubiksCube));
    if (copy == NULL) {
        return NULL;
    }

    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        copy->face_colors[i] = cube->face_colors[i];
        copy->centre_positions[i] = cube->centre_positions[i];
    }
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        copy->corner_positions[i] = cube->corner_positions[i];
        copy->corner_orientations[i] = cube->corner_orientations[i];
    }
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        copy->edge_positions[i] = cube->edge_positions[i];
        copy->edge_orientations[i] = cube->edge_orientations[i];
    }

    return copy;
}

/*
 * Compares two cubes for equality. It only matches state of the cube irrespective of the color
 * combination of the faces.
 */
int
is_equal(
    RubiksCube *cube1,
    RubiksCube *cube2
) {
    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        if (cube1->centre_positions[i] != cube2->centre_positions[i]) {
            return 0;
        };
    }
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        if (cube1->corner_positions[i] != cube2->corner_positions[i]) {
            return 0;
        };
        if (cube1->corner_orientations[i] != cube2->corner_orientations[i]) {
            return 0;
        };
    }
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        if (cube1->edge_positions[i] != cube2->edge_positions[i]) {
            return 0;
        };
        if (cube1->edge_orientations[i] != cube2->edge_orientations[i]) {
            return 0;
        };
    }
    return 1;
}



char
get_corner_sticker(
    RubiksCube *cube,
    CornerPosition cubie,
    CornerStickerPosition position
) {
    return STICKERS[
        cube->face_colors[
            CORNER_CUBIE_STICKER_POSITIONS[
                cube->corner_positions[cubie]
            ][
                (position + cube->corner_orientations[cubie]) % NUMBER_OF_CORNER_SIDES
            ]
        ]
    ];
}

char get_edge_sticker(
    RubiksCube *cube,
    EdgePosition cubie,
    EdgeStickerPosition position
) {
    return STICKERS[
        cube->face_colors[
            EDGE_CUBIE_STICKER_POSITIONS[
                cube->edge_positions[cubie]
            ][
                (position + cube->edge_orientations[cubie]) % NUMBER_OF_EDGE_SIDES
            ]
        ]
    ];
}



/*
 * Using the cubie representation generate all the stickers to print the cube
 */
void generate_stickers(RubiksCube *cube, char stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION]) {
//     for (int i = 0; i < NUMBER_OF_FACES; i++) {
//
//     }

    stickers[UP][0][0] = get_corner_sticker(cube, UBL, CORNER_TOP);
    stickers[UP][0][1] = get_edge_sticker(cube, UB, EDGE_TOP);
    stickers[UP][0][2] = get_corner_sticker(cube, UBR, CORNER_TOP);
    stickers[UP][1][0] = get_edge_sticker(cube, UL, EDGE_TOP);
    stickers[UP][1][1] = STICKERS[cube->centre_positions[UP]];
    stickers[UP][1][2] = get_edge_sticker(cube, UR, EDGE_TOP);
    stickers[UP][2][0] = get_corner_sticker(cube, UFL, CORNER_TOP);
    stickers[UP][2][1] = get_edge_sticker(cube, UF, EDGE_TOP);
    stickers[UP][2][2] = get_corner_sticker(cube, UFR, CORNER_TOP);

    stickers[LEFT][0][0] = get_corner_sticker(cube, UBL, CORNER_RIGHT);
    stickers[LEFT][0][1] = get_edge_sticker(cube, UL, EDGE_DOWN);
    stickers[LEFT][0][2] = get_corner_sticker(cube, UFL, CORNER_LEFT);
    stickers[LEFT][1][0] = get_edge_sticker(cube, BL, EDGE_TOP);
    stickers[LEFT][1][1] = STICKERS[cube->centre_positions[LEFT]];
    stickers[LEFT][1][2] = get_edge_sticker(cube, FL, EDGE_TOP);
    stickers[LEFT][2][0] = get_corner_sticker(cube, DBL, CORNER_LEFT);
    stickers[LEFT][2][1] = get_edge_sticker(cube, DL, EDGE_DOWN);
    stickers[LEFT][2][2] = get_corner_sticker(cube, DFL, CORNER_RIGHT);

    stickers[FRONT][0][0] = get_corner_sticker(cube, UFL, CORNER_RIGHT);
    stickers[FRONT][0][1] = get_edge_sticker(cube, UF, EDGE_DOWN);
    stickers[FRONT][0][2] = get_corner_sticker(cube, UFR, CORNER_LEFT);
    stickers[FRONT][1][0] = get_edge_sticker(cube, FL, EDGE_DOWN);
    stickers[FRONT][1][1] = STICKERS[cube->centre_positions[FRONT]];
    stickers[FRONT][1][2] = get_edge_sticker(cube, FR, EDGE_DOWN);
    stickers[FRONT][2][0] = get_corner_sticker(cube, DFL, CORNER_LEFT);
    stickers[FRONT][2][1] = get_edge_sticker(cube, DF, EDGE_DOWN);
    stickers[FRONT][2][2] = get_corner_sticker(cube, DFR, CORNER_RIGHT);

    stickers[RIGHT][0][0] = get_corner_sticker(cube, UFR, CORNER_RIGHT);
    stickers[RIGHT][0][1] = get_edge_sticker(cube, UR, EDGE_DOWN);
    stickers[RIGHT][0][2] = get_corner_sticker(cube, UBR, CORNER_LEFT);
    stickers[RIGHT][1][0] = get_edge_sticker(cube, FR, EDGE_TOP);
    stickers[RIGHT][1][1] = STICKERS[cube->centre_positions[RIGHT]];
    stickers[RIGHT][1][2] = get_edge_sticker(cube, BR, EDGE_TOP);
    stickers[RIGHT][2][0] = get_corner_sticker(cube, DFR, CORNER_LEFT);
    stickers[RIGHT][2][1] = get_edge_sticker(cube, DR, EDGE_DOWN);
    stickers[RIGHT][2][2] = get_corner_sticker(cube, DBR, CORNER_RIGHT);

    stickers[BACK][0][0] = get_corner_sticker(cube, UBR, CORNER_RIGHT);
    stickers[BACK][0][1] = get_edge_sticker(cube, UB, EDGE_DOWN);
    stickers[BACK][0][2] = get_corner_sticker(cube, UBL, CORNER_LEFT);
    stickers[BACK][1][0] = get_edge_sticker(cube, BR, EDGE_DOWN);
    stickers[BACK][1][1] = STICKERS[cube->centre_positions[BACK]];
    stickers[BACK][1][2] = get_edge_sticker(cube, BL, EDGE_DOWN);
    stickers[BACK][2][0] = get_corner_sticker(cube, DBR, CORNER_LEFT);
    stickers[BACK][2][1] = get_edge_sticker(cube, DB, EDGE_DOWN);
    stickers[BACK][2][2] = get_corner_sticker(cube, DBL, CORNER_RIGHT);

    stickers[DOWN][0][0] = get_corner_sticker(cube, DFL, CORNER_TOP);
    stickers[DOWN][0][1] = get_edge_sticker(cube, DF, EDGE_TOP);
    stickers[DOWN][0][2] = get_corner_sticker(cube, DFR, CORNER_TOP);
    stickers[DOWN][1][0] = get_edge_sticker(cube, DL, EDGE_TOP);
    stickers[DOWN][1][1] = STICKERS[cube->centre_positions[DOWN]];
    stickers[DOWN][1][2] = get_edge_sticker(cube, DR, EDGE_TOP);
    stickers[DOWN][2][0] = get_corner_sticker(cube, DBL, CORNER_TOP);
    stickers[DOWN][2][1] = get_edge_sticker(cube, DB, EDGE_TOP);
    stickers[DOWN][2][2] = get_corner_sticker(cube, DBR, CORNER_TOP);
}


void print_cube(RubiksCube *cube) {
    char stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION];
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

void print_cube_arrays(RubiksCube *cube) {
    printf("centres:             [");
    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        printf("%i,", cube->centre_positions[i]);
    }
    printf("]\n");
    printf("corner positions:    [");
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        printf("%i,", cube->corner_positions[i]);
    }
    printf("]\n");
    printf("corner orientations: [");
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        printf("%i,", cube->corner_orientations[i]);
    }
    printf("]\n");
    printf("edge positions:      [");
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        printf("%i,", cube->edge_positions[i]);
    }
    printf("]\n");
    printf("edge orientations:   [");
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        printf("%i,", cube->edge_orientations[i]);
    }
    printf("]\n");
}
