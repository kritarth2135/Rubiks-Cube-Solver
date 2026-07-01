#include <stdbool.h>
#include <stdio.h>

#include "rubiks_cube.h"

typedef enum {
    U_NORMAL = 'U',
    D_NORMAL = 'D',
    R_NORMAL = 'R',
    L_NORMAL = 'L',
    F_NORMAL = 'F',
    B_NORMAL = 'B',
    U_PRIME = 'U' + '\'',
    D_PRIME = 'D' + '\'',
    R_PRIME = 'R' + '\'',
    L_PRIME = 'L' + '\'',
    F_PRIME = 'F' + '\'',
    B_PRIME = 'B' + '\'',
    U_TWO = 'U' + '2',
    D_TWO = 'D' + '2',
    R_TWO = 'R' + '2',
    L_TWO = 'L' + '2',
    F_TWO = 'F' + '2',
    B_TWO = 'B' + '2',
    U_WIDE_NORMAL = 'U' + 'w',
    D_WIDE_NORMAL = 'D' + 'w',
    R_WIDE_NORMAL = 'R' + 'w',
    L_WIDE_NORMAL = 'L' + 'w',
    F_WIDE_NORMAL = 'F' + 'w',
    B_WIDE_NORMAL = 'B' + 'w',
    U_WIDE_PRIME = 'U' + 'w' + '\'',
    D_WIDE_PRIME = 'D' + 'w' + '\'',
    R_WIDE_PRIME = 'R' + 'w' + '\'',
    L_WIDE_PRIME = 'L' + 'w' + '\'',
    F_WIDE_PRIME = 'F' + 'w' + '\'',
    B_WIDE_PRIME = 'B' + 'w' + '\'',
    U_WIDE_TWO = 'U' + 'w' + '2',
    D_WIDE_TWO = 'D' + 'w' + '2',
    R_WIDE_TWO = 'R' + 'w' + '2',
    L_WIDE_TWO = 'L' + 'w' + '2',
    F_WIDE_TWO = 'F' + 'w' + '2',
    B_WIDE_TWO = 'B' + 'w' + '2',
    M_NORMAL = 'M',
    E_NORMAL = 'E',
    S_NORMAL = 'S',
    M_PRIME = 'M' + '\'',
    E_PRIME = 'E' + '\'',
    S_PRIME = 'S' + '\'',
    M_TWO = 'M' + '2',
    E_TWO = 'E' + '2',
    S_TWO = 'S' + '2',
    x_NORMAL = 'X',
    y_NORMAL = 'Y',
    z_NORMAL = 'Z',
    x_PRIME = 'X' + '\'',
    y_PRIME = 'Y' + '\'',
    z_PRIME = 'Z' + '\'',
    x_TWO = 'X' + '2',
    y_TWO = 'Y' + '2',
    z_TWO = 'Z' + '2',
} Moves;

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

void make_move(RubiksCube* cube, char* move) {
    int move_value = 0;
    char c;
    while((c = *move++)) {
        move_value += c;
    }

    switch (move_value) {
        case U_NORMAL:
            u_normal(cube);
            break;
        case U_PRIME:
            u_prime(cube);
            break;
        default:
            printf("Invalid move!\n");
            break;
    }
}