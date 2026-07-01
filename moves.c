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

void u_normal(RubiksCube *cube) {
    CornerCubie temp_corner = cube->corner_positions[UFL];
    cube->corner_positions[UFL] = cube->corner_positions[UFR];
    cube->corner_positions[UFR] = cube->corner_positions[UBR];
    cube->corner_positions[UBR] = cube->corner_positions[UBL];
    cube->corner_positions[UBL] = temp_corner;

    EdgeCubie temp_edge = cube->edge_positions[UF];
    cube->edge_positions[UF] = cube->edge_positions[UR];
    cube->edge_positions[UR] = cube->edge_positions[UB];
    cube->edge_positions[UB] = cube->edge_positions[UL];
    cube->edge_positions[UL] = temp_edge;
}

void u_prime(RubiksCube *cube) {
    CornerCubie temp_corner = cube->corner_positions[UFL];
    cube->corner_positions[UFL] = cube->corner_positions[UBL];
    cube->corner_positions[UBL] = cube->corner_positions[UBR];
    cube->corner_positions[UBR] = cube->corner_positions[UFR];
    cube->corner_positions[UFR] = temp_corner;

    EdgeCubie temp_edge = cube->edge_positions[UF];
    cube->edge_positions[UF] = cube->edge_positions[UL];
    cube->edge_positions[UL] = cube->edge_positions[UB];
    cube->edge_positions[UB] = cube->edge_positions[UR];
    cube->edge_positions[UR] = temp_edge;
}

void u_wide_normal(RubiksCube *cube) {
    u_normal(cube);
    EdgeCubie temp_cubie_position = cube->edge_positions[FL];
    EdgeOrientation temp_cubie_orientation = cube->edge_orientations[FL];
    cube->edge_positions[FL] = cube->edge_positions[FR];
    cube->edge_orientations[FL] = flip_edge(cube->edge_orientations[FR]);
    cube->edge_positions[FR] = cube->edge_positions[BR];
    cube->edge_orientations[FR] = flip_edge(cube->edge_orientations[BR]);
    cube->edge_positions[BR] = cube->edge_positions[BL];
    cube->edge_orientations[BR] = flip_edge(cube->edge_orientations[BL]);
    cube->edge_positions[BL] = temp_cubie_position;
    cube->edge_orientations[BL] = flip_edge(temp_cubie_orientation);

    Color temp_center = cube->centres[FRONT];
    cube->centres[FRONT] = cube->centres[RIGHT];
    cube->centres[RIGHT] = cube->centres[BACK];
    cube->centres[BACK] = cube->centres[LEFT];
    cube->centres[LEFT] = temp_center;
}

void u_wide_prime(RubiksCube *cube) {
    u_prime(cube);
    EdgeCubie temp_cubie_position = cube->edge_positions[FL];
    EdgeOrientation temp_cubie_orientation = cube->edge_orientations[FL];
    cube->edge_positions[FL] = cube->edge_positions[BL];
    cube->edge_orientations[FL] = flip_edge(cube->edge_orientations[BL]);
    cube->edge_positions[BL] = cube->edge_positions[BR];
    cube->edge_orientations[BL] = flip_edge(cube->edge_orientations[BR]);
    cube->edge_positions[BR] = cube->edge_positions[FR];
    cube->edge_orientations[BR] = flip_edge(cube->edge_orientations[FR]);
    cube->edge_positions[FR] = temp_cubie_position;
    cube->edge_orientations[FR] = flip_edge(temp_cubie_orientation);

    Color temp_center = cube->centres[FRONT];
    cube->centres[FRONT] = cube->centres[LEFT];
    cube->centres[LEFT] = cube->centres[BACK];
    cube->centres[BACK] = cube->centres[RIGHT];
    cube->centres[RIGHT] = temp_center;
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
            u_wide_normal(cube);
            break;
        case U_WIDE_PRIME:
            u_wide_prime(cube);
            break;
        case U_WIDE_TWO:
            u_wide_normal(cube);
            u_wide_normal(cube);
            break;
        default:
            printf("Invalid move!\n");
            break;
    }
}