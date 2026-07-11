#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pattern_database.h"
#include "rubiks_cube.h"

const char *BASIC_MOVES[NUMBER_OF_BASIC_MOVES] = {
    "U", "D", "L", "R", "F", "B",
    "U'", "D'", "L'", "R'", "F'", "B'",
    "U2", "D2", "L2", "R2", "F2", "B2"
};

const char *REVERSE_BASIC_MOVES[NUMBER_OF_BASIC_MOVES] = {
    "U'", "D'", "L'", "R'", "F'", "B'",
    "U", "D", "L", "R", "F", "B",
    "U2", "D2", "L2", "R2", "F2", "B2"
};

const char *CORNER_DB_NAME = "centre_db.bin";
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
    const char **basic_moves,const char **reverse_basic_moves, int basic_moves_len
) {
    if (*depth < max_depth) {
        (*depth)++;
        for (int i = 0; i < basic_moves_len; i++) {
            make_move(cube, basic_moves[i]);

            uint64_t indexes[NUMBER_OF_INDEXES];
            encode_corners_and_edges(cube, indexes);
            if (corner_db[indexes[CORNER_DB_INDEX]] == 0 || corner_db[indexes[CORNER_DB_INDEX]] > *depth) {
                corner_db[indexes[CORNER_DB_INDEX]] = *depth;
            }
            if (first_edge_db[FIRST_SIX_EDGE_DB_INDEX] == 0 || first_edge_db[FIRST_SIX_EDGE_DB_INDEX] > *depth) {
                first_edge_db[FIRST_SIX_EDGE_DB_INDEX] = *depth;
            }
            if (second_edge_db[LAST_SIX_EDGE_DB_INDEX] == 0 || second_edge_db[LAST_SIX_EDGE_DB_INDEX] > *depth) {
                second_edge_db[LAST_SIX_EDGE_DB_INDEX] = *depth;
            }
            create_db(corner_db, first_edge_db, second_edge_db, cube, depth, max_depth, basic_moves, reverse_basic_moves, basic_moves_len);

            make_move(cube, reverse_basic_moves[i]);
        }
        (*depth)--;
    }
}

int create_and_store_db(
    int max_depth, const char **basic_moves, const char **reverse_basic_moves,
    int basic_moves_len
) {
    uint8_t *corner_db = calloc(sizeof(uint8_t), POSSIBLE_CORNER_COMBINATIONS);
    uint8_t *first_edge_db = calloc(sizeof(uint8_t), POSSIBLE_SIX_EDGE_COMBINATIONS);
    uint8_t *second_edge_db = calloc(sizeof(uint8_t), POSSIBLE_SIX_EDGE_COMBINATIONS);
    RubiksCube *cube = create_rubiks_cube();
    if (corner_db == NULL || first_edge_db == NULL || second_edge_db == NULL || cube == NULL) {
        return 1;
    }
    int depth = 0;

    create_db(corner_db, first_edge_db, second_edge_db, cube, &depth, max_depth, basic_moves, reverse_basic_moves, basic_moves_len);
    if (write_db_to_file(corner_db, sizeof(uint8_t) * POSSIBLE_CORNER_COMBINATIONS, CORNER_DB_NAME) == 1) {
        return 1;
    }
    if (write_db_to_file(first_edge_db, sizeof(uint8_t) * POSSIBLE_SIX_EDGE_COMBINATIONS, FIRST_SIX_EDGE_DB_NAME) == 1) {
        return 1;
    }
    if (write_db_to_file(second_edge_db, sizeof(uint8_t) * POSSIBLE_SIX_EDGE_COMBINATIONS, LAST_SIX_EDGE_DB_NAME) == 1) {
        return 1;
    }
    free(corner_db);
    free(first_edge_db);
    free(second_edge_db);
    return 0;
}

uint8_t * load_db(
    int max_depth, const char **basic_moves, const char **reverse_basic_moves,
    int basic_moves_len, const char *db_name, int possible_combinations
) {
    clock_t begin, end;
    FILE * file = fopen(db_name, "rb");
    if (file == NULL) {
        printf("Database not found, creating them...\n");
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
    else {
        printf("Database found, loading it...\n");
    }

    begin = clock();
    uint8_t *db = calloc(sizeof(uint8_t), possible_combinations);
    if (db == NULL) {
        return NULL;
    }
    fread(db, sizeof(uint8_t), possible_combinations, file);
    end = clock();
    printf("Loaded db in %f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    return db;
}