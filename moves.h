#ifndef MOVES_H
#define MOVES_H


#include "rubiks_cube.h"

#define MOVE_STR_LEN 3
#define NUMBER_OF_MOVES 54


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


extern const char *STRING_REPRESENTATION_OF_MOVES[NUMBER_OF_MOVES];

void make_move(RubiksCube* cube, Move move);
int parse_and_make_move(RubiksCube *cube, const char *move_str);


#endif