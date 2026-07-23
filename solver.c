#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#include "solver.h"
#include "rubiks_cube.h"
#include "pattern_database.h"

int get_heuristic(RubiksCube *cube, uint8_t *db) {
    uint64_t index = encode_corners(cube);
    return get_four_bits(db, index);
}

int get_max(int value1, int value2, int value3) {
    if (value1 > value2) {
        if (value1 > value3) {
            return value1;
        }
        else {
            return value3;
        }
    }
    else {
        if (value2 > value3) {
            return value2;
        }
        else {
            return value3;
        }
    }
}

int solve(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db,
    uint8_t *first_edge_db, uint8_t *second_edge_db, int *depth, int max_depth,
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

            if (is_equal(cube, goal_state)) {
                return 1;
            }

            int estimated_cost = *depth + get_max(
                get_heuristic(cube, corner_db),
                get_heuristic(cube, first_edge_db),
                get_heuristic(cube, second_edge_db)
            );
            if (estimated_cost > SOLVER_MAX_DEPTH) {
                make_move(cube, REVERSE_BASIC_MOVES[i]);
                continue;
            }

            if (solve(cube, goal_state, corner_db, first_edge_db, second_edge_db, depth, max_depth, no_of_nodes_processed, prev_moves_indexes)) {
                return 1;
            }
            make_move(cube, REVERSE_BASIC_MOVES[i]);
        }
        (*depth)--;
    }
    return 0;
}

int solve_cube(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, uint8_t *first_edge_db,
    uint8_t *second_edge_db, Move *solution_array, int *solution_len
) {
    int depth = 0;
    long long unsigned no_of_nodes_processed = 0;
    int prev_moves_indexes[SOLVER_MAX_DEPTH];
    bool found_solution = false;

    clock_t begin, end;
    begin = clock();
    for (int i = 0; i < SOLVER_MAX_DEPTH; i++) {
        printf("Trying to find solution at depth: %i\n", i);
        if (solve(cube, goal_state, corner_db, first_edge_db, second_edge_db, &depth, i + 1, &no_of_nodes_processed, prev_moves_indexes)) {
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