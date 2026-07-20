#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#include "solver.h"
#include "rubiks_cube.h"
#include "pattern_database.h"

int corner_heuristic(RubiksCube *cube, uint8_t *db) {
    uint64_t index = encode_corners(cube);
    return get_four_bits(db, index);
}

int solve(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, int *depth, int max_depth,
    long long unsigned *no_of_nodes_processed, int *prev_moves_indexes
) {
    if (is_equal(cube, goal_state)) {
        return 1;
    }
    if (*depth < max_depth) {
        (*depth)++;
        for (int i = 0; i < NUMBER_OF_BASIC_MOVES; i++) {
            prev_moves_indexes[*depth - 1] = i;

            make_move(cube, BASIC_MOVES[i]);
            (*no_of_nodes_processed)++;

            int estimated_cost = *depth + corner_heuristic(cube, corner_db);
            if (estimated_cost > SOLVER_MAX_DEPTH) {
                make_move(cube, REVERSE_BASIC_MOVES[i]);
                continue;
            }

            if (solve(cube, goal_state, corner_db, depth, max_depth, no_of_nodes_processed, prev_moves_indexes)) {
                return 1;
            }
            make_move(cube, REVERSE_BASIC_MOVES[i]);
        }
        (*depth)--;
    }
    return 0;
}

int solve_cube(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, Move *solution_array,
    int *solution_len
) {
    int depth = 0;
    long long unsigned no_of_nodes_processed = 0;
    int prev_moves_indexes[SOLVER_MAX_DEPTH];
    bool found_solution = false;

    clock_t begin, end;
    begin = clock();
    for (int i = 0; i < SOLVER_MAX_DEPTH; i++) {
        if (solve(cube, goal_state, corner_db, &depth, i + 1, &no_of_nodes_processed, prev_moves_indexes)) {
            found_solution = true;
            break;
        }
    }
    end = clock();

    if (found_solution) {
        for (int i = 0; i < depth; i++) {
            solution_array[i] = BASIC_MOVES[prev_moves_indexes[i]];
        }
        *solution_len = depth;
        printf(
            "Solution of lenght %i found in %f seconds. (Processed %llu nodes)\n",
            depth,
            (double)(end - begin) / CLOCKS_PER_SEC,
            no_of_nodes_processed
        );
        return 1;
    }
    else {
        printf(
            "Unable to find solution. (Processed %llu nodes in %f seconds)\n",
            no_of_nodes_processed,
            (double)(end - begin) / CLOCKS_PER_SEC
        );
        return 0;
    }
}