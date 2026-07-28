#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"
#include "rubiks_cube.h"
#include "pattern_database.h"

void insertion_sort(int *value_array, int *key_array, int size) {
    for (int i = 1; i < size; i++) {
        int key = key_array[i];
        int value = value_array[i];

        int j = i - 1;
        while (j >= 0 && key_array[j] > key) {
            key_array[j + 1] = key_array[j];
            value_array[j + 1] = value_array[j];
            j--;
        }
        key_array[j] = key;
        value_array[j] = value;
    }
}

int get_max(int value1, int value2, int value3, int value4) {
    int max = value1;
    if (value2 > max) {
        max = value2;
    }
    if (value3 > max) {
        max = value3;
    }
    if (value4 > max) {
        max = value4;
    }
    return max;
}

int solve(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, uint8_t *edge_position_db,
    uint8_t *first_edge_db, uint8_t *second_edge_db, int *depth, int max_depth,
    long long unsigned *no_of_nodes_processed, int *prev_moves_indexes
) {
    int indexes[NUMBER_OF_BASIC_MOVES] = {0};
    int costs[NUMBER_OF_BASIC_MOVES] = {0};
    for (int i = 0; i < NUMBER_OF_BASIC_MOVES; i++) {
        indexes[i] = i;
        costs[i] = get_max(
            get_four_bits(corner_db, encode_corners(cube)),
            get_four_bits(first_edge_db, encode_first_seven_edges(cube)),
            get_four_bits(second_edge_db, encode_last_seven_edges(cube)),
            get_four_bits(edge_position_db, encode_all_edge_positions(cube))
        );
    }
    insertion_sort(indexes, costs, NUMBER_OF_BASIC_MOVES);

    if (*depth < max_depth) {
        (*depth)++;
        for (int i = 0; i < NUMBER_OF_BASIC_MOVES; i++) {
            if (*depth == 1) {
                printf("#");
                fflush(stdout);
            }

            prev_moves_indexes[*depth - 1] = indexes[i];
            // Skip moving the same face consecutively
            if (
                *depth > 1 &&
                prev_moves_indexes[*depth - 2] % NUMBER_OF_FACES == indexes[i] % NUMBER_OF_FACES
            ) {
                continue;
            }
            // As moving opposite faces is commutative, we only allow one order to move opposite faces
            // and forbid the opposite order
            if (
                *depth > 1 &&
                // This condition is required because without it this will also skip moves like Left face move
                // after the D face move or Front face move after a right face move
                prev_moves_indexes[*depth - 2] % 2 == 0 &&
                prev_moves_indexes[*depth - 2] % NUMBER_OF_FACES == (indexes[i] % NUMBER_OF_FACES) - 1
            ) {
                continue;
            }

            make_move(cube, BASIC_MOVES[indexes[i]]);
            (*no_of_nodes_processed)++;

            if (is_equal(cube, goal_state)) {
                return 1;
            }

            int estimated_cost = *depth + get_max(
                get_four_bits(corner_db, encode_corners(cube)),
                get_four_bits(first_edge_db, encode_first_seven_edges(cube)),
                get_four_bits(second_edge_db, encode_last_seven_edges(cube)),
                get_four_bits(edge_position_db, encode_all_edge_positions(cube))
            );
            if (estimated_cost > max_depth) {
                make_move(cube, REVERSE_BASIC_MOVES[indexes[i]]);
                continue;
            }

            if (
                solve(
                    cube, goal_state, corner_db, edge_position_db, first_edge_db, second_edge_db,
                    depth, max_depth, no_of_nodes_processed, prev_moves_indexes
                )
            ) {
                return 1;
            }
            make_move(cube, REVERSE_BASIC_MOVES[indexes[i]]);
        }
        (*depth)--;
    }
    return 0;
}

int solve_cube(
    RubiksCube *cube, RubiksCube *goal_state, uint8_t *corner_db, uint8_t *first_edge_db,
    uint8_t *second_edge_db, uint8_t *edge_position_db, Move *solution_array, int *solution_len
) {
    if (is_equal(cube, goal_state)) {
        printf("Cube is already solved.\n");
        return 1;
    }

    int depth = 0;
    long long unsigned no_of_nodes_processed = 0;
    int prev_moves_indexes[SOLVER_MAX_DEPTH];
    bool found_solution = false;

    RubiksCube *original_state = copy_cube(cube);
    if (original_state == NULL) {
        return 0;
    }

    clock_t begin, end;
    begin = clock();
    for (int i = 0; i < SOLVER_MAX_DEPTH; i++) {
        printf("\nTrying to find solution at depth: %i\n", i + 1);
        if (
            solve(
                cube, goal_state, corner_db, edge_position_db, first_edge_db, second_edge_db,
                &depth, i + 1, &no_of_nodes_processed, prev_moves_indexes
            )
        ) {
            found_solution = true;
            break;
        }
    }
    end = clock();

    if (found_solution) {
        if (!is_equal(cube, goal_state)) {
            printf("\nSolver failed as the cube is in inconsistent state after running the solver.\n");
            return 0;
        }
        for (int i = 0; i < depth; i++) {
            solution_array[i] = BASIC_MOVES[prev_moves_indexes[i]];
        }
        *solution_len = depth;
        printf(
            "\nSolution of lenght %i found in %f seconds. (Processed %llu nodes)\n",
            depth,
            (double)(end - begin) / CLOCKS_PER_SEC,
            no_of_nodes_processed
        );
        free(original_state);
        return 1;
    }
    else {
        if (!is_equal(cube, original_state)) {
            printf("\nSolver failed as the cube is in inconsistent state after running the solver.\n");
            return 0;
        }
        printf(
            "\nUnable to find solution. (Processed %llu nodes in %f seconds)\n",
            no_of_nodes_processed,
            (double)(end - begin) / CLOCKS_PER_SEC
        );
        free(original_state);
        return 0;
    }
}