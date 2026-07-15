#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#define NUMBER_OF_FACES 6
#define NUMBER_OF_COLORS 6
#define NUMBER_OF_CORNERS 8
#define NUMBER_OF_EDGES 12

#define CORNER_SIDES 3
#define EDGE_SIDES 2

#define MOVE_STR_LEN 3


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

typedef enum {
    U_NORMAL, U_PRIME, U_TWO, U_WIDE_NORMAL, U_WIDE_PRIME, U_WIDE_2,
    D_NORMAL, D_PRIME, D_TWO, D_WIDE_NORMAL, D_WIDE_PRIME, D_WIDE_2,
    L_NORMAL, L_PRIME, L_TWO, L_WIDE_NORMAL, L_WIDE_PRIME, L_WIDE_2,
    R_NORMAL, R_PRIME, R_TWO, R_WIDE_NORMAL, R_WIDE_PRIME, R_WIDE_2,
    F_NORMAL, F_PRIME, F_TWO, F_WIDE_NORMAL, F_WIDE_PRIME, F_WIDE_2,
    B_NORMAL, B_PRIME, B_TWO, B_WIDE_NORMAL, B_WIDE_PRIME, B_WIDE_2,
    M_NORMAL, M_PRIME, M_TWO,
    E_NORMAL, E_PRIME, E_TWO,
    S_NORMAL, S_PRIME, S_TWO,
    X_NORMAL, X_PRIME, X_TWO,
    Y_NORMAL, Y_PRIME, Y_TWO,
    Z_NORMAL, Z_PRIME, Z_TWO,
} Move;

extern const char *STRING_REPRESENTATION_OF_MOVES[];

extern const Color solved_centre_pattern[NUMBER_OF_COLORS];
extern const CornerCubie solved_corner_positions[NUMBER_OF_CORNERS];
extern const CornerOrientation solved_corner_orientaitons[NUMBER_OF_CORNERS];
extern const EdgeCubie solved_edges_positions[NUMBER_OF_EDGES];
extern const EdgeOrientation solved_edges_orientations[NUMBER_OF_EDGES];

RubiksCube * create_rubiks_cube(void);
RubiksCube * copy_cube(RubiksCube *cube);
int is_equal(RubiksCube *cube1, RubiksCube *cube2);

void print_cube(RubiksCube *cube);
void print_cube_arrays(RubiksCube *cube);

void make_move(RubiksCube* cube, Move move);
int parse_and_make_move(RubiksCube *cube, const char *move_str);

#endif
