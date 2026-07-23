#ifndef SOLVER_H
#define SOLVER_H

#include <stdint.h>

#include "rubiks_cube.h"

#define SOLVER_MAX_DEPTH 18

int solve_cube(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, uint8_t *first_edge_db,
    uint8_t *second_edge_db, Move *solution_array, int *solution_len
);

#endif