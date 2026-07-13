#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pattern_database.h"
#include "rubiks_cube.h"

const Move BASIC_MOVES[NUMBER_OF_BASIC_MOVES] = {
    U_NORMAL, D_NORMAL, L_NORMAL, R_NORMAL, F_NORMAL, B_NORMAL,
    U_PRIME, D_PRIME, L_PRIME, R_PRIME, F_PRIME, B_PRIME,
    U_TWO, D_TWO, L_TWO, R_TWO, F_TWO, B_TWO
};

const Move REVERSE_BASIC_MOVES[NUMBER_OF_BASIC_MOVES] = {
    U_PRIME, D_PRIME, L_PRIME, R_PRIME, F_PRIME, B_PRIME,
    U_NORMAL, D_NORMAL, L_NORMAL, R_NORMAL, F_NORMAL, B_NORMAL,
    U_TWO, D_TWO, L_TWO, R_TWO, F_TWO, B_TWO
};

const char *CORNER_DB_NAME = "corner_db.bin";
const char *FIRST_SIX_EDGE_DB_NAME = "first_six_edge_db.bin";
const char *LAST_SIX_EDGE_DB_NAME = "last_six_edge_db.bin";

int write_db_to_file(uint8_t *db, int size_of_db, const char *db_name) {
    FILE *file = fopen(db_name, "wb");
    if (file == NULL) {
        return 1;
    }
    fwrite(db, size_of_db, 1, file);
    fclose(file);
    return 0;
}

void create_db(
    uint8_t *corner_db, uint8_t *first_edge_db, uint8_t *second_edge_db, RubiksCube *cube, int *depth, int max_depth,
    const Move *basic_moves, const Move *reverse_basic_moves, int basic_moves_len, Move *moves, long long unsigned *no_of_nodes_processed
) {
    if (*depth < max_depth) {
        (*depth)++;
        for (int i = 0; i < basic_moves_len; i++) {
            moves[*depth - 1] = basic_moves[i];
            // Skip if current move undos the previous move
            if (*depth > 1 && reverse_basic_moves[i] == moves[*depth - 2]) {
                continue;
            }
            // Skip if the current move is same as the previous move as there are double moves for that
            // This also eliminates things like triple moves instead of prime moves, four of the same
            // moves or two moves and then a double move.
            if (*depth > 1 && moves[*depth - 1] == moves[*depth - 2]) {
                continue;
            }

            make_move(cube, basic_moves[i]);
            uint64_t indexes[NUMBER_OF_INDEXES];
            encode_corners_and_edges(cube, indexes);
            (*no_of_nodes_processed)++;

            if (corner_db[indexes[CORNER_DB_INDEX]] == 0 || corner_db[indexes[CORNER_DB_INDEX]] > *depth) {
                corner_db[indexes[CORNER_DB_INDEX]] = *depth;
            }
            if (first_edge_db[indexes[FIRST_SIX_EDGE_DB_INDEX]] == 0 || first_edge_db[indexes[FIRST_SIX_EDGE_DB_INDEX]] > *depth) {
                first_edge_db[indexes[FIRST_SIX_EDGE_DB_INDEX]] = *depth;
            }
            if (second_edge_db[indexes[LAST_SIX_EDGE_DB_INDEX]] == 0 || second_edge_db[indexes[LAST_SIX_EDGE_DB_INDEX]] > *depth) {
                second_edge_db[indexes[LAST_SIX_EDGE_DB_INDEX]] = *depth;
            }
            create_db(corner_db, first_edge_db, second_edge_db, cube, depth, max_depth, basic_moves, reverse_basic_moves, basic_moves_len, moves, no_of_nodes_processed);

            make_move(cube, reverse_basic_moves[i]);
        }
        (*depth)--;
    }
}

int create_and_store_db(
    int max_depth, const Move *basic_moves, const Move *reverse_basic_moves,
    int basic_moves_len
) {
    uint8_t *corner_db = calloc(sizeof(uint8_t), POSSIBLE_CORNER_COMBINATIONS);
    uint8_t *first_six_edge_db = calloc(sizeof(uint8_t), POSSIBLE_SIX_EDGE_COMBINATIONS);
    uint8_t *last_six_edge_db = calloc(sizeof(uint8_t), POSSIBLE_SIX_EDGE_COMBINATIONS);
    RubiksCube *cube = create_rubiks_cube();
    if (corner_db == NULL || first_six_edge_db == NULL || last_six_edge_db == NULL || cube == NULL) {
        return 1;
    }
    int depth = 0;
    Move moves[max_depth];
    long long unsigned number_of_nodes_processed = 0;
    create_db(corner_db, first_six_edge_db, last_six_edge_db, cube, &depth, max_depth, basic_moves, reverse_basic_moves, basic_moves_len, moves, &number_of_nodes_processed);
    RubiksCube *new_cube = create_rubiks_cube();
    if (!is_equal(cube, new_cube)) {
        printf("Database creation failed as the used cube is NOT in the same state as it was before starting to create the database.\n");
        free(corner_db);
        free(first_six_edge_db);
        free(last_six_edge_db);
        free(cube);
        free(new_cube);
        return 1;
    }
    free(new_cube);
    printf("Processed %llu nodes at depth %i.\n", number_of_nodes_processed, max_depth);
    if (write_db_to_file(corner_db, sizeof(uint8_t) * POSSIBLE_CORNER_COMBINATIONS, CORNER_DB_NAME) == 1) {
        return 1;
    }
    if (write_db_to_file(first_six_edge_db, sizeof(uint8_t) * POSSIBLE_SIX_EDGE_COMBINATIONS, FIRST_SIX_EDGE_DB_NAME) == 1) {
        return 1;
    }
    if (write_db_to_file(last_six_edge_db, sizeof(uint8_t) * POSSIBLE_SIX_EDGE_COMBINATIONS, LAST_SIX_EDGE_DB_NAME) == 1) {
        return 1;
    }
    free(corner_db);
    free(first_six_edge_db);
    free(last_six_edge_db);
    free(cube);
    return 0;
}

uint8_t * load_db(
    int max_depth, const Move *basic_moves, const Move *reverse_basic_moves,
    int basic_moves_len, const char *db_name, int possible_combinations
) {
    clock_t begin, end;
    FILE * file = fopen(db_name, "rb");
    if (file == NULL) {
        printf("Database not found, creating them... \n");
        begin = clock();
        if (create_and_store_db(max_depth, basic_moves, reverse_basic_moves, basic_moves_len) == 1) {
            return NULL;
        }
        end = clock();
        printf("Created all the dbs in %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
        file = fopen(db_name, "rb");
        if (file == NULL) {
            return NULL;
        }
    }

    begin = clock();
    uint8_t *db = calloc(sizeof(uint8_t), possible_combinations);
    if (db == NULL) {
        return NULL;
    }
    fread(db, sizeof(uint8_t), possible_combinations, file);
    end = clock();
    printf("Loaded %s in %f\n", db_name, (double)(end - begin) / CLOCKS_PER_SEC);

    fclose(file);
    return db;
}