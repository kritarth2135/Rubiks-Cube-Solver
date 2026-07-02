#include <stdbool.h>
#include <stdio.h>

#include "rubiks_cube.h"

int hash_move(char *move);

// const char* ALL_POSSIBLE_MOVES[] = {
    //     "U", "D", "R", "L", "F", "B",
    //     "U'", "D'", "R'", "L'", "F'", "B'",
    //     "U2", "D2", "R2", "L2", "F2", "B2",
    //     "Uw", "Dw", "Rw", "Lw", "Fw", "Bw",
    //     "Uw'", "Dw'", "Rw'", "Lw'", "Fw'", "Bw'",
    //     "Uw2", "Dw2", "Rw2", "Lw2", "Fw2", "Bw2",
    //     "M", "E", "S", "M'", "E'", "S'", "M2", "E2", "S2",
    //     "x", "y", "z", "x'", "y'", "z'", "x2", "y2", "z2"
    // };

// Precomputed hash values calculated through hash_move
typedef enum {
    U_NORMAL = 85,
    U_PRIME = 21799,
    U_TWO = 21810,
    U_WIDE_NORMAL = 21879,
    U_WIDE_PRIME = 5601063,
    U_WIDE_TWO = 5601074,

    D_NORMAL = 68,
    D_PRIME = 17447,
    D_TWO = 17458,
    D_WIDE_NORMAL = 17527,
    D_WIDE_PRIME = 4486951,
    D_WIDE_TWO = 4486962,

    R_NORMAL = 82,
    R_PRIME = 21031,
    R_TWO = 21042,
    R_WIDE_NORMAL = 21111,
    R_WIDE_PRIME = 5404455,
    R_WIDE_TWO = 5404466,

    L_NORMAL = 76,
    L_PRIME = 19495,
    L_TWO = 19506,
    L_WIDE_NORMAL = 19575,
    L_WIDE_PRIME = 5011239,
    L_WIDE_TWO = 5011250,

    F_NORMAL = 70,
    F_PRIME = 17959,
    F_TWO = 17970,
    F_WIDE_NORMAL = 18039,
    F_WIDE_PRIME = 4618023,
    F_WIDE_TWO = 4618034,

    B_NORMAL = 66,
    B_PRIME = 16935,
    B_TWO = 16946,
    B_WIDE_NORMAL = 17015,
    B_WIDE_PRIME = 4355879,
    B_WIDE_TWO = 4355890,

    M_NORMAL = 77,
    M_PRIME = 19751,
    M_TWO = 19762,
    E_NORMAL = 69,
    E_PRIME = 17703,
    E_TWO = 17714,
    S_NORMAL = 83,
    S_PRIME = 21287,
    S_TWO = 21298,

    x_NORMAL = 120,
    x_PRIME = 30759,
    x_TWO = 30770,
    y_NORMAL = 121,
    y_PRIME = 31015,
    y_TWO = 31026,
    z_NORMAL = 122,
    z_PRIME = 31271,
    z_TWO = 31282,
} Move;

int hash_move(char *move) {
    int value = 0;
    char ch;
    while ((ch = *move++)) {
        value <<= 8;
        value |= ch;
    }
    return value;
}

EdgeOrientation flip_edge(EdgeOrientation orientation) {
    if (orientation == SOLVED)
        return FLIPPED;
    else
        return SOLVED;
}

CornerOrientation twist_corner_right(CornerOrientation orientation) {
    if (orientation == NO_TWIST)
        return TWISTED_RIGHT;
    else if (orientation == TWISTED_RIGHT)
        return TWISTED_LEFT;
    else
        return NO_TWIST;
}

CornerOrientation twist_corner_left(CornerOrientation orientation) {
    if (orientation == NO_TWIST)
        return TWISTED_LEFT;
    else if (orientation == TWISTED_LEFT)
        return TWISTED_RIGHT;
    else
        return NO_TWIST;
}

// c1 is replaced with c2, c3 with c3 and so on
void cycle_four_corners(RubiksCube *cube, CornerCubie c1, CornerCubie c2, CornerCubie c3, CornerCubie c4) {
    CornerCubie temp_corner_position = cube->corner_positions[c1];
    CornerOrientation temp_corner_orientation = cube->corner_orientations[c1];
    cube->corner_positions[c1] = cube->corner_positions[c2];
    cube->corner_orientations[c1] = cube->corner_orientations[c2];
    cube->corner_positions[c2] = cube->corner_positions[c3];
    cube->corner_orientations[c2] = cube->corner_orientations[c3];
    cube->corner_positions[c3] = cube->corner_positions[c4];
    cube->corner_orientations[c3] = cube->corner_orientations[c4];
    cube->corner_positions[c4] = temp_corner_position;
    cube->corner_orientations[c4] = temp_corner_orientation;
}

void cycle_four_edges_without_flipping(RubiksCube *cube, EdgeCubie c1, EdgeCubie c2, EdgeCubie c3, EdgeCubie c4) {
    EdgeCubie temp_cubie_position = cube->edge_positions[c1];
    EdgeOrientation temp_cubie_orientation = cube->edge_orientations[c1];
    cube->edge_positions[c1] = cube->edge_positions[c2];
    cube->edge_orientations[c1] = cube->edge_orientations[c2];
    cube->edge_positions[c2] = cube->edge_positions[c3];
    cube->edge_orientations[c2] = cube->edge_orientations[c3];
    cube->edge_positions[c3] = cube->edge_positions[c4];
    cube->edge_orientations[c3] = cube->edge_orientations[c4];
    cube->edge_positions[c4] = temp_cubie_position;
    cube->edge_orientations[c4] = temp_cubie_orientation;
}

void cycle_four_edges_with_flipping(RubiksCube *cube, EdgeCubie c1, EdgeCubie c2, EdgeCubie c3, EdgeCubie c4) {
    cycle_four_edges_without_flipping(cube, c1, c2, c3, c4);
    cube->edge_orientations[c1] = flip_edge(cube->edge_orientations[c1]);
    cube->edge_orientations[c2] = flip_edge(cube->edge_orientations[c2]);
    cube->edge_orientations[c3] = flip_edge(cube->edge_orientations[c3]);
    cube->edge_orientations[c4] = flip_edge(cube->edge_orientations[c4]);
}

void cycle_four_centres(RubiksCube *cube, Position c1, Position c2, Position c3, Position c4) {
    Color temp_center = cube->centres[c1];
    cube->centres[c1] = cube->centres[c2];
    cube->centres[c2] = cube->centres[c3];
    cube->centres[c3] = cube->centres[c4];
    cube->centres[c4] = temp_center;
}

void u_normal(RubiksCube *cube) {
    cycle_four_corners(cube, UFL, UFR, UBR, UBL);
    cycle_four_edges_without_flipping(cube, UF, UR, UB, UL);
}

void u_prime(RubiksCube *cube) {
    cycle_four_corners(cube, UFL, UBL, UBR, UFR);
    cycle_four_edges_without_flipping(cube, UF, UL, UB, UR);
}

void d_normal(RubiksCube *cube) {
    cycle_four_corners(cube, DFL, DBL, DBR, DFR);
    cycle_four_edges_without_flipping(cube, DF, DL, DB, DR);
}

void d_prime(RubiksCube *cube) {
    cycle_four_corners(cube, DFL, DFR, DBR, DBL);
    cycle_four_edges_without_flipping(cube, DF, DR, DB, DL);
}

void e_normal(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, FL, BL, BR, FR);
    cycle_four_centres(cube, FRONT, LEFT, BACK, RIGHT);
}

void e_prime(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, FL, FR, BR, BL);
    cycle_four_centres(cube, FRONT, RIGHT, BACK, LEFT);
}

void make_move(RubiksCube* cube, char* move_str) {
    Move move = hash_move(move_str);

    switch (move) {
        case U_NORMAL:
            u_normal(cube);
            break;
        case U_PRIME:
            u_prime(cube);
            break;
        case U_TWO:
            u_normal(cube);
            u_normal(cube);
            break;
        case U_WIDE_NORMAL:
            u_normal(cube);
            e_prime(cube);
            break;
        case U_WIDE_PRIME:
            u_prime(cube);
            e_normal(cube);
            break;
        case U_WIDE_TWO:
            u_normal(cube);
            u_normal(cube);
            e_normal(cube);
            e_normal(cube);
            break;
        case D_NORMAL:
            d_normal(cube);
            break;
        case D_PRIME:
            d_prime(cube);
            break;
        case D_TWO:
            d_normal(cube);
            d_normal(cube);
            break;
        case D_WIDE_NORMAL:
            d_normal(cube);
            e_normal(cube);
            break;
        case D_WIDE_PRIME:
            d_prime(cube);
            e_prime(cube);
            break;
        case D_WIDE_TWO:
            d_normal(cube);
            d_normal(cube);
            e_normal(cube);
            e_normal(cube);
            break;
        case E_NORMAL:
            e_normal(cube);
            break;
        case E_PRIME:
            e_prime(cube);
            break;
        case E_TWO:
            e_normal(cube);
            e_normal(cube);
            break;
        default:
            printf("Invalid move!\n");
            break;
    }
}