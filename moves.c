#include <stdbool.h>
#include <stdio.h>

#include "rubiks_cube.h"

#define TOTAL_MOVES 54

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
    return !orientation;
}

CornerOrientation twist_corner(CornerOrientation orientation, CornerOrientation twist) {
    return orientation += twist % CORNER_SIDES;
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

void cycle_four_corners_with_twists (
    RubiksCube *cube, CornerCubie c1, CornerCubie c2, CornerCubie c3, CornerCubie c4, CornerOrientation twists[4]
) {
    cycle_four_corners(cube, c1, c2, c3, c4);
    cube->corner_orientations[c2] = twist_corner(cube->corner_orientations[c2], twists[0]);
    cube->corner_orientations[c3] = twist_corner(cube->corner_orientations[c3], twists[1]);
    cube->corner_orientations[c4] = twist_corner(cube->corner_orientations[c4], twists[2]);
    cube->corner_orientations[c1] = twist_corner(cube->corner_orientations[c1], twists[3]);
}

void cycle_four_edges_without_flipping(RubiksCube *cube, EdgeCubie e1, EdgeCubie e2, EdgeCubie e3, EdgeCubie e4) {
    EdgeCubie temp_cubie_position = cube->edge_positions[e1];
    EdgeOrientation temp_cubie_orientation = cube->edge_orientations[e1];
    cube->edge_positions[e1] = cube->edge_positions[e2];
    cube->edge_orientations[e1] = cube->edge_orientations[e2];
    cube->edge_positions[e2] = cube->edge_positions[e3];
    cube->edge_orientations[e2] = cube->edge_orientations[e3];
    cube->edge_positions[e3] = cube->edge_positions[e4];
    cube->edge_orientations[e3] = cube->edge_orientations[e4];
    cube->edge_positions[e4] = temp_cubie_position;
    cube->edge_orientations[e4] = temp_cubie_orientation;
}

void cycle_four_edges_with_flipping(RubiksCube *cube, EdgeCubie e1, EdgeCubie e2, EdgeCubie e3, EdgeCubie e4) {
    cycle_four_edges_without_flipping(cube, e1, e2, e3, e4);
    cube->edge_orientations[e1] = flip_edge(cube->edge_orientations[e1]);
    cube->edge_orientations[e2] = flip_edge(cube->edge_orientations[e2]);
    cube->edge_orientations[e3] = flip_edge(cube->edge_orientations[e3]);
    cube->edge_orientations[e4] = flip_edge(cube->edge_orientations[e4]);
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

void r_normal(RubiksCube *cube) {
    CornerOrientation twists[] = {RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST};
    cycle_four_corners_with_twists(cube, UFR, DFR, DBR, UBR, twists);
    cycle_four_edges_with_flipping(cube, UR, FR, DR, BR);
}

void r_prime(RubiksCube *cube) {
    CornerOrientation twists[] = {RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST};
    cycle_four_corners_with_twists(cube, UFR, UBR, DBR, DFR, twists);
    cycle_four_edges_with_flipping(cube, UR, BR, DR, FR);
}

void l_normal(RubiksCube *cube) {
    CornerOrientation twists[] = {LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST};
    cycle_four_corners_with_twists(cube, UFL, UBL, DBL, DFL, twists);
    cycle_four_edges_with_flipping(cube, UL, BL, DL, FL);
}

void l_prime(RubiksCube *cube) {
    CornerOrientation twists[] = {LEFT_TWIST, RIGHT_TWIST, LEFT_TWIST, RIGHT_TWIST};
    cycle_four_corners_with_twists(cube, UFL, DFL, DBL, UBL, twists);
    cycle_four_edges_with_flipping(cube, UL, FL, DL, BL);
}

void m_normal(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, UF, UB, DB, DF);
    cycle_four_centres(cube, FRONT, UP, BACK, DOWN);
}

void m_prime(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, UF, DF, DB, UB);
    cycle_four_centres(cube, FRONT, DOWN, BACK, UP);
}

void e_normal(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, FL, BL, BR, FR);
    cycle_four_centres(cube, FRONT, LEFT, BACK, RIGHT);
}

void e_prime(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, FL, FR, BR, BL);
    cycle_four_centres(cube, FRONT, RIGHT, BACK, LEFT);
}

void s_normal(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, UL, DL, DR, UR);
    cycle_four_centres(cube, UP, LEFT, DOWN, RIGHT);
}

void s_prime(RubiksCube *cube) {
    cycle_four_edges_with_flipping(cube, UL, UR, DR, DL);
    cycle_four_centres(cube, UP, RIGHT, DOWN, LEFT);
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

        case R_NORMAL:
            r_normal(cube);
            break;
        case R_PRIME:
            r_prime(cube);
            break;
        case R_TWO:
            r_normal(cube);
            r_normal(cube);
            break;
        case R_WIDE_NORMAL:
            r_normal(cube);
            m_prime(cube);
            break;
        case R_WIDE_PRIME:
            r_prime(cube);
            m_normal(cube);
            break;
        case R_WIDE_TWO:
            r_normal(cube);
            r_normal(cube);
            m_normal(cube);
            m_normal(cube);
            break;

        case L_NORMAL:
            l_normal(cube);
            break;
        case L_PRIME:
            l_prime(cube);
            break;
        case L_TWO:
            l_normal(cube);
            l_normal(cube);
            break;
        case L_WIDE_NORMAL:
            l_normal(cube);
            m_normal(cube);
            break;
        case L_WIDE_PRIME:
            l_prime(cube);
            m_prime(cube);
            break;
        case L_WIDE_TWO:
            l_normal(cube);
            l_normal(cube);
            m_normal(cube);
            m_normal(cube);
            break;

        case M_NORMAL:
            m_normal(cube);
            break;
        case M_PRIME:
            m_prime(cube);
            break;
        case M_TWO:
            m_normal(cube);
            m_normal(cube);
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

        case S_NORMAL:
            s_normal(cube);
            break;
        case S_PRIME:
            s_prime(cube);
            break;
        case S_TWO:
            s_normal(cube);
            s_normal(cube);
            break;

        default:
            printf("Invalid move!\n");
            break;
    }
}