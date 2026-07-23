#include <stdint.h>
#include <stdio.h>

#include "pattern_database.h"
#include "rubiks_cube.h"

int create_corner_db(
    uint8_t *corner_db, RubiksCube *cube, int *depth, int max_depth,
    long long unsigned *no_of_nodes_processed, int *prev_moves_indexes
) {
    if (*depth < max_depth) {
        (*depth)++;
        for (int i = 0; i < NUMBER_OF_BASIC_MOVES; i++) {
            prev_moves_indexes[*depth - 1] = i;
            // Skip moving the same face consecutively
            if (
                *depth > 1 &&
                prev_moves_indexes[*depth - 2] % NUMBER_OF_FACES == i % NUMBER_OF_FACES
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
                prev_moves_indexes[*depth - 2] % NUMBER_OF_FACES == (i % NUMBER_OF_FACES) - 1
            ) {
                continue;
            }

            make_move(cube, BASIC_MOVES[i]);
            uint64_t index = encode_corners(cube);
            if (index >= POSSIBLE_CORNER_COMBINATIONS) {
                return 1;
            }
            (*no_of_nodes_processed)++;

            int value = get_four_bits(corner_db, index);
            if (value != 0 && value <= *depth) {
                make_move(cube, REVERSE_BASIC_MOVES[i]);
                continue;
            }

            set_four_bits(corner_db, index, *depth);
            if (create_corner_db(corner_db, cube, depth, max_depth, no_of_nodes_processed, prev_moves_indexes) == 1) {
                return 1;
            }
            make_move(cube, REVERSE_BASIC_MOVES[i]);
        }
        (*depth)--;
    }
    return 0;
}

uint8_t * load_corner_db() {
    uint8_t *db = load_db(CORNER_DB_NAME, CORNER_DB_GENERATION_MAX_DEPTH, POSSIBLE_CORNER_COMBINATIONS, create_corner_db);
    if (db == NULL) {
        return NULL;
    }
    return db;
}