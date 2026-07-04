#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#define NUMBER_OF_COLORS 6
#define NUMBER_OF_CORNERS 8
#define NUMBER_OF_EDGES 12

#define CORNER_SIDES 3
#define EDGE_SIDES 2

typedef enum {
    WHITE,
    ORANGE,
    GREEN,
    RED,
    BLUE,
    YELLOW
} Color;

typedef enum {
    UP,
    LEFT,
    FRONT,
    RIGHT,
    BACK,
    DOWN
} Position;

// Location specified by abbrevation of face positions (Up, Down, Left, Right, Front, Back)
// These are ordered according to the solved state of a cube
typedef enum {
    UFL, UFR, UBL, UBR,
    DFL, DFR, DBL, DBR
} CornerCubie;

typedef enum {
    UF, UL, UB, UR,
    FL, BL, BR, FR,
    DF, DL, DB, DR
} EdgeCubie;

typedef enum {
    NO_TWIST,
    LEFT_TWIST,
    RIGHT_TWIST
} CornerOrientation;

typedef enum {
    SOLVED,
    FLIPPED
} EdgeOrientation;

typedef struct {
    Color centres[NUMBER_OF_COLORS];
    CornerCubie corner_positions[NUMBER_OF_CORNERS];
    CornerOrientation corner_orientations[NUMBER_OF_CORNERS];
    EdgeCubie edge_positions[NUMBER_OF_EDGES];
    EdgeOrientation edge_orientations[NUMBER_OF_EDGES];
} RubiksCube;

extern const Color solved_centre_pattern[NUMBER_OF_COLORS];
extern const CornerCubie solved_corner_positions[NUMBER_OF_CORNERS];
extern const CornerOrientation solved_corner_orientaitons[NUMBER_OF_CORNERS];
extern const EdgeCubie solved_edges_positions[NUMBER_OF_EDGES];
extern const EdgeOrientation solved_edges_orientations[NUMBER_OF_EDGES];

RubiksCube * create_rubiks_cube(void);

void print_cube(RubiksCube *cube);

int make_move(RubiksCube* cube, char *move);

#endif
