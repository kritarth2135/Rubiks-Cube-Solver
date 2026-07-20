#ifndef SOLVER_H
#define SOLVER_H

#include <stdint.h>

#include "rubiks_cube.h"

#define SOLVER_MAX_DEPTH 11

int solve_cube(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, Move *solution_array,
    int *solution_len
);

#endif