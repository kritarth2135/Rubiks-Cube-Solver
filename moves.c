#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "rubiks_cube.h"

#define CORNERS_IN_FACE 4
#define EDGES_IN_FACE 4
#define CENTRES_IN_MIDDLE_LAYER 4

#define TYPES_OF_MOVES 3
#define NUMBER_OF_FACE_MOVES 6
#define NUMBER_OF_MIDDLE_MOVES 3
#define NUMBER_OF_CUBE_MOVES 3

typedef enum {
    U,
    L,
    F,
    R,
    B,
    D
} FaceMove;

typedef enum {
    M,
    E,
    S
} MiddleMove;

typedef enum {
    X,
    Y,
    Z
} CubeMove;

const char *STRING_REPRESENTATION_OF_MOVES[] = {
    "U", "U'", "U2", "Uw", "Uw'", "Uw2",
    "D", "D'", "D2", "Dw", "Dw'", "Dw2",
    "L", "L'", "L2", "Lw", "Lw'", "Lw2",
    "R", "R'", "R2", "Rw", "Rw'", "Rw2",
    "F", "F'", "F2", "Fw", "Fw'", "Fw2",
    "B", "B'", "B2", "Bw", "Bw'", "Bw2",
    "M", "M'", "M2",
    "E", "E'", "E2",
    "S", "S'", "S2",
    "x", "x'", "x2",
    "y", "y'", "y2",
    "z", "z'", "z2",
};

const CornerCubie NORMAL_CORNERS[NUMBER_OF_COLORS][CORNERS_IN_FACE] = {
    {UFL, UFR, UBR, UBL},
    {UFL, UBL, DBL, DFL},
    {UFL, DFL, DFR, UFR},
    {UFR, DFR, DBR, UBR},
    {UBL, UBR, DBR, DBL},
    {DFL, DBL, DBR, DFR}
};

const EdgeCubie NORMAL_EDGES[NUMBER_OF_COLORS][EDGES_IN_FACE] = {
    {UF, UR, UB, UL},
    {UL, BL, DL, FL},
    {UF, FL, DF, FR},
    {UR, FR, DR, BR},
    {UB, BR, DB, BL},
    {DF, DL, DB, DR}
};

const CornerCubie PRIME_CORNERS[NUMBER_OF_COLORS][CORNERS_IN_FACE] = {
    {UFL, UBL, UBR, UFR},
    {UFL, DFL, DBL, UBL},
    {UFL, UFR, DFR, DFL},
    {UFR, UBR, DBR, DFR},
    {UBL, DBL, DBR, UBR},
    {DFL, DFR, DBR, DBL}
};

const EdgeCubie PRIME_EDGES[NUMBER_OF_COLORS][EDGES_IN_FACE] = {
    {UF, UL, UB, UR},
    {UL, FL, DL, BL},
    {UF, FR, DF, FL},
    {UR, BR, DR, FR},
    {UB, BL, DB, BR},
    {DF, DR, DB, DL}
};

const CornerOrientation TWISTS[NUMBER_OF_COLORS][CORNERS_IN_FACE] = {
    {},
    {LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST},
    {RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST},
    {RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST},
    {LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST},
    {}
};

const EdgeCubie MIDDLE_NORMAL_EDGES[NUMBER_OF_MIDDLE_MOVES][EDGES_IN_FACE] = {
    {UF, UB, DB, DF},
    {FL, BL, BR, FR},
    {UL, DL, DR, UR}
};

const Position MIDDLE_NORMAL_CENTRES[NUMBER_OF_MIDDLE_MOVES][CENTRES_IN_MIDDLE_LAYER] = {
    {FRONT, UP, BACK, DOWN},
    {FRONT, LEFT, BACK, RIGHT},
    {UP, LEFT, DOWN, RIGHT}
};

const EdgeCubie MIDDLE_PRIME_EDGES[NUMBER_OF_MIDDLE_MOVES][EDGES_IN_FACE] = {
    {UF, DF, DB, UB},
    {FL, FR, BR, BL},
    {UL, UR, DR, DL}
};

const Position MIDDLE_PRIME_CENTRES[NUMBER_OF_MIDDLE_MOVES][CENTRES_IN_MIDDLE_LAYER] = {
    {FRONT, DOWN, BACK, UP},
    {FRONT, RIGHT, BACK, LEFT},
    {UP, RIGHT, DOWN, LEFT}
};

EdgeOrientation flip_edge(EdgeOrientation orientation) {
    return !orientation;
}

CornerOrientation twist_corner(CornerOrientation orientation, CornerOrientation twist) {
    return (orientation + twist) % CORNER_SIDES;
}

// corners[1] is replaced with corners[2], corners[2] with corners[3], corners[3] with corners[4]
// and corners[4] with corners[1]
void cycle_four_corners(RubiksCube *cube, const CornerCubie corners[CORNERS_IN_FACE]) {
    CornerCubie temp_corner_position;
    CornerOrientation temp_corner_orientation;
    // As 3 swaps are needed to cycle four corners
    for (int i = 0; i < CORNERS_IN_FACE - 1; i++) {
        temp_corner_position = cube->corner_positions[corners[i]];
        temp_corner_orientation = cube->corner_orientations[corners[i]];
        cube->corner_positions[corners[i]] = cube->corner_positions[corners[i + 1]];
        cube->corner_orientations[corners[i]] = cube->corner_orientations[corners[i + 1]];
        cube->corner_positions[corners[i + 1]] = temp_corner_position;
        cube->corner_orientations[corners[i + 1]] = temp_corner_orientation;
    }
}

void cycle_four_corners_with_twists (
    RubiksCube *cube, const CornerCubie corners[CORNERS_IN_FACE], const CornerOrientation twists[CORNERS_IN_FACE]
) {
    cycle_four_corners(cube, corners);
    for (int i = 0; i < CORNERS_IN_FACE; i++) {
        // Applying the first twist to second cubie in the array as they are rotated in cycle_four_corners
        cube->corner_orientations[corners[(i + 1) % CORNERS_IN_FACE]] = twist_corner(cube->corner_orientations[corners[(i + 1) % CORNERS_IN_FACE]], twists[i]);
    }
}

void cycle_four_edges(RubiksCube *cube, const EdgeCubie edges[EDGES_IN_FACE]) {
    EdgeCubie temp_edge_position;
    EdgeOrientation temp_edge_orientation;
    for (int i = 0; i < EDGES_IN_FACE - 1; i++) {
        temp_edge_position = cube->edge_positions[edges[i]];
        temp_edge_orientation = cube->edge_orientations[edges[i]];
        cube->edge_positions[edges[i]] = cube->edge_positions[edges[i + 1]];
        cube->edge_orientations[edges[i]] = cube->edge_orientations[edges[i + 1]];
        cube->edge_positions[edges[i + 1]] = temp_edge_position;
        cube->edge_orientations[edges[i + 1]] = temp_edge_orientation;
    }
}

void cycle_four_edges_with_flipping(RubiksCube *cube, const EdgeCubie edges[EDGES_IN_FACE]) {
    cycle_four_edges(cube, edges);
    for (int i = 0; i < EDGES_IN_FACE; i++) {
        cube->edge_orientations[edges[i]] = flip_edge(cube->edge_orientations[edges[i]]);
    }
}

void cycle_four_centres(RubiksCube *cube, const Position centres[CENTRES_IN_MIDDLE_LAYER]) {
    Color temp_centre;
    for (int i = 0; i < CENTRES_IN_MIDDLE_LAYER - 1; i++) {
        temp_centre = cube->centres[centres[i]];
        cube->centres[centres[i]] = cube->centres[centres[i + 1]];
        cube->centres[centres[i + 1]] = temp_centre;
    }
}

void normal_face_move(RubiksCube *cube, FaceMove move) {
    switch (move) {
        case U:
        case D:
            cycle_four_corners(cube, NORMAL_CORNERS[move]);
            cycle_four_edges(cube, NORMAL_EDGES[move]);
            break;
        case R:
        case L:
            cycle_four_corners_with_twists(cube, NORMAL_CORNERS[move], TWISTS[move]);
            cycle_four_edges_with_flipping(cube, NORMAL_EDGES[move]);
            break;
        case F:
        case B:
            cycle_four_corners_with_twists(cube, NORMAL_CORNERS[move], TWISTS[move]);
            cycle_four_edges(cube, NORMAL_EDGES[move]);
            break;
    }
}

void prime_face_move(RubiksCube *cube, FaceMove move) {
    switch (move) {
        case U:
        case D:
            cycle_four_corners(cube, PRIME_CORNERS[move]);
            cycle_four_edges(cube, PRIME_EDGES[move]);
            break;
        case R:
        case L:
            cycle_four_corners_with_twists(cube, PRIME_CORNERS[move], TWISTS[move]);
            cycle_four_edges_with_flipping(cube, PRIME_EDGES[move]);
            break;
        case F:
        case B:
            cycle_four_corners_with_twists(cube, PRIME_CORNERS[move], TWISTS[move]);
            cycle_four_edges(cube, PRIME_EDGES[move]);
            break;
    }
}

void double_face_move(RubiksCube *cube, FaceMove move) {
    normal_face_move(cube, move);
    normal_face_move(cube, move);
}

void normal_middle_move(RubiksCube *cube, MiddleMove move) {
    cycle_four_edges_with_flipping(cube, MIDDLE_NORMAL_EDGES[move]);
    cycle_four_centres(cube, MIDDLE_NORMAL_CENTRES[move]);
}

void prime_middle_move(RubiksCube *cube, MiddleMove move) {
    cycle_four_edges_with_flipping(cube, MIDDLE_PRIME_EDGES[move]);
    cycle_four_centres(cube, MIDDLE_PRIME_CENTRES[move]);
}

void double_middle_move(RubiksCube *cube, MiddleMove move) {
    normal_middle_move(cube, move);
    normal_middle_move(cube, move);
}

void wide_face_move(RubiksCube *cube, FaceMove move) {
    normal_face_move(cube, move);
    switch (move) {
        case U:
            prime_middle_move(cube, E);
            break;
        case L:
            normal_middle_move(cube, M);
            break;
        case F:
            normal_middle_move(cube, S);
            break;
        case R:
            prime_middle_move(cube, M);
            break;
        case B:
            prime_middle_move(cube, S);
            break;
        case D:
            normal_middle_move(cube, E);
            break;
    }
}

void wide_prime_face_move(RubiksCube *cube, FaceMove move) {
    prime_face_move(cube, move);
    switch (move) {
        case U:
            normal_middle_move(cube, E);
            break;
        case L:
            prime_middle_move(cube, M);
            break;
        case F:
            prime_middle_move(cube, S);
            break;
        case R:
            normal_middle_move(cube, M);
            break;
        case B:
            normal_middle_move(cube, S);
            break;
        case D:
            prime_middle_move(cube, E);
            break;
    }
}

void double_wide_face_move(RubiksCube *cube, FaceMove move) {
    wide_face_move(cube, move);
    wide_face_move(cube, move);
}

void normal_cube_move(RubiksCube *cube, CubeMove move) {
    switch (move) {
        case X:
            prime_face_move(cube, L);
            prime_middle_move(cube, M);
            normal_face_move(cube, R);
            break;
        case Y:
            normal_face_move(cube, U);
            prime_middle_move(cube, E);
            prime_face_move(cube, D);
            break;
        case Z:
            normal_face_move(cube, F);
            normal_middle_move(cube, S);
            prime_face_move(cube, B);
            break;
    }
}

void prime_cube_move(RubiksCube *cube, CubeMove move) {
    switch (move) {
        case X:
            normal_face_move(cube, L);
            normal_middle_move(cube, M);
            prime_face_move(cube, R);
            break;
        case Y:
            prime_face_move(cube, U);
            normal_middle_move(cube, E);
            normal_face_move(cube, D);
            break;
        case Z:
            prime_face_move(cube, F);
            prime_middle_move(cube, S);
            normal_face_move(cube, B);
            break;
    }
}

void double_cube_move(RubiksCube *cube, CubeMove move) {
    normal_cube_move(cube, move);
    normal_cube_move(cube, move);
}

int get_face_move(char move) {
    switch (move) {
        case 'U': return U;
        case 'D': return D;
        case 'L': return L;
        case 'R': return R;
        case 'F': return F;
        case 'B': return B;
        default: return -1;
    }
}

int get_middle_move(char move) {
    switch (move) {
        case 'M': return M;
        case 'E': return E;
        case 'S': return S;
        default: return -1;
    }
}

int get_cube_move(char move) {
    switch (move) {
        case 'X': return X;
        case 'Y': return Y;
        case 'Z': return Z;
        default: return -1;
    }
}

void make_move(RubiksCube*cube, Move move) {
    switch (move) {
        case U_NORMAL: return normal_face_move(cube, U);
        case U_PRIME: return prime_face_move(cube, U);
        case U_TWO: return double_face_move(cube, U);
        case U_WIDE_NORMAL: return wide_face_move(cube, U);
        case U_WIDE_PRIME: return wide_prime_face_move(cube, U);
        case U_WIDE_2: return double_wide_face_move(cube, U);

        case D_NORMAL: return normal_face_move(cube, D);
        case D_PRIME: return prime_face_move(cube, D);
        case D_TWO: return double_face_move(cube, D);
        case D_WIDE_NORMAL: return wide_face_move(cube, D);
        case D_WIDE_PRIME: return wide_prime_face_move(cube, D);
        case D_WIDE_2: return double_wide_face_move(cube, D);

        case L_NORMAL: return normal_face_move(cube, L);
        case L_PRIME: return prime_face_move(cube, L);
        case L_TWO: return double_face_move(cube, L);
        case L_WIDE_NORMAL: return wide_face_move(cube, L);
        case L_WIDE_PRIME: return wide_prime_face_move(cube, L);
        case L_WIDE_2: return double_wide_face_move(cube, L);

        case R_NORMAL: return normal_face_move(cube, R);
        case R_PRIME: return prime_face_move(cube, R);
        case R_TWO: return double_face_move(cube, R);
        case R_WIDE_NORMAL: return wide_face_move(cube, R);
        case R_WIDE_PRIME: return wide_prime_face_move(cube, R);
        case R_WIDE_2: return double_wide_face_move(cube, R);

        case F_NORMAL: return normal_face_move(cube, F);
        case F_PRIME: return prime_face_move(cube, F);
        case F_TWO: return double_face_move(cube, F);
        case F_WIDE_NORMAL: return wide_face_move(cube, F);
        case F_WIDE_PRIME: return wide_prime_face_move(cube, F);
        case F_WIDE_2: return double_wide_face_move(cube, F);

        case B_NORMAL: return normal_face_move(cube, B);
        case B_PRIME: return prime_face_move(cube, B);
        case B_TWO: return double_face_move(cube, B);
        case B_WIDE_NORMAL: return wide_face_move(cube, B);
        case B_WIDE_PRIME: return wide_prime_face_move(cube, B);
        case B_WIDE_2: return double_wide_face_move(cube, B);

        case M_NORMAL: return normal_middle_move(cube, M);
        case M_PRIME: return prime_middle_move(cube, M);
        case M_TWO: return double_middle_move(cube, M);

        case E_NORMAL: return normal_middle_move(cube, E);
        case E_PRIME: return prime_middle_move(cube, E);
        case E_TWO: return double_middle_move(cube, E);

        case S_NORMAL: return normal_middle_move(cube, S);
        case S_PRIME: return prime_middle_move(cube, S);
        case S_TWO: return double_middle_move(cube, S);

        case X_NORMAL: return normal_cube_move(cube, X);
        case X_PRIME: return prime_cube_move(cube, X);
        case X_TWO: return double_cube_move(cube, X);

        case Y_NORMAL: return normal_cube_move(cube, Y);
        case Y_PRIME: return prime_cube_move(cube, Y);
        case Y_TWO: return double_cube_move(cube, Y);

        case Z_NORMAL: return normal_cube_move(cube, Z);
        case Z_PRIME: return prime_cube_move(cube, Z);
        case Z_TWO: return double_cube_move(cube, Z);
    }
}

int parse_and_make_move(RubiksCube* cube, const char *move_str) {
    int len = strlen(move_str);
    if (len == 0 || len > MOVE_STR_LEN) {
        return 1;
    }

    char base_move_str = toupper(move_str[0]);
    int base_move;

    if ((base_move = get_face_move(base_move_str)) != -1) {
        if (len == 1) {
            normal_face_move(cube, base_move);
        }
        else if (len >= 2) {
            char move_type = move_str[1];
            bool is_wide = false;
            if (move_type == 'w' || move_type == 'W') {
                if (len != 3) {
                    wide_face_move(cube, base_move);
                    return 0;
                }
                else {
                    is_wide = true;
                    move_type = move_str[2];
                }
            }

            if (move_type == '\'') {
                if (is_wide) {
                    wide_prime_face_move(cube, base_move);
                }
                else {
                    prime_face_move(cube, base_move);
                }
            }
            else if (move_type == '2') {
                if (is_wide) {
                    double_wide_face_move(cube, base_move);
                }
                else {
                    double_face_move(cube, base_move);
                }
            }
            else {
                return 1;
            }
        }
        else {
            return 1;
        }
    }
    else if ((base_move = get_middle_move(base_move_str)) != -1) {
        if (len == 1) {
            normal_middle_move(cube, base_move);
        }
        else if (len == 2) {
            char move_type = move_str[1];
            if (move_type == '\'') {
                prime_middle_move(cube, base_move);
            }
            else if (move_type == '2') {
                double_middle_move(cube, base_move);
            }
            else {
                return 1;
            }
        }
        else {
            return 1;
        }
    }
    else if ((base_move = get_cube_move(base_move_str)) != -1) {
        if (len == 1) {
            normal_cube_move(cube, base_move);
        }
        else if (len == 2) {
            char move_type = move_str[1];
            if (move_type == '\'') {
                prime_cube_move(cube, base_move);
            }
            else if (move_type == '2') {
                double_cube_move(cube, base_move);
            }
            else {
                return 1;
            }
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
    return 0;
}