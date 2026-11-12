#ifndef CUBE_H
#define CUBE_H


#define NUMBER_OF_FACES 6
#define NUMBER_OF_COLORS 6
#define NUMBER_OF_CORNERS 8
#define NUMBER_OF_EDGES 12
#define FACE_DIMENSION 3

#define NUMBER_OF_CORNER_SIDES 3
#define NUMBER_OF_EDGE_SIDES 2


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
} CornerPosition;

typedef enum {
    UF, UL, UB, UR,
    FL, BL, BR, FR,
    DF, DL, DB, DR
} EdgePosition;

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
    Color face_colors[NUMBER_OF_FACES];
    Position centre_positions[NUMBER_OF_FACES];
    CornerPosition corner_positions[NUMBER_OF_CORNERS];
    CornerOrientation corner_orientations[NUMBER_OF_CORNERS];
    EdgePosition edge_positions[NUMBER_OF_EDGES];
    EdgeOrientation edge_orientations[NUMBER_OF_EDGES];
} RubiksCube;


RubiksCube * create_rubiks_cube(void);
RubiksCube * copy_cube(RubiksCube *cube);
int is_equal(RubiksCube *cube1, RubiksCube *cube2);

void generate_stickers(RubiksCube *cube, Color stickers[NUMBER_OF_COLORS][FACE_DIMENSION][FACE_DIMENSION]);
// void print_cube(RubiksCube *cube);
void print_cube_arrays(RubiksCube *cube);


#endif
