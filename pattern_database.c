#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pattern_database.h"
#include "rubiks_cube.h"

void write_db_to_file(uint8_t *db, int size_of_db, const char *db_name) {
    FILE *file = fopen(db_name, "wb");
    if (file == NULL) {
        return;
    }
    fwrite(db, size_of_db, 1, file);
    fclose(file);
}

void create_db(
    uint8_t *db, RubiksCube *cube, int *depth, int max_depth,
    const char **basic_moves,const char **reverse_basic_moves, int basic_moves_len
) {
    if (*depth < max_depth) {
        (*depth)++;
        for (int i = 0; i < basic_moves_len; i++) {
            make_move(cube, basic_moves[i]);

            uint64_t index = encode_centres(cube);
            if (db[index] == 0) {
                db[index] = *depth;
            }
            create_db(db, cube, depth, max_depth, basic_moves, reverse_basic_moves, basic_moves_len);

            make_move(cube, reverse_basic_moves[i]);
        }
        (*depth)--;
    }
}

void create_and_store_db(
    int max_depth, const char **basic_moves, const char **reverse_basic_moves,
    int basic_moves_len, const char *db_name, int possible_combinations
) {
    uint8_t *db = calloc(sizeof(uint8_t), possible_combinations);
    RubiksCube *cube = create_rubiks_cube();
    int depth = 0;

    create_db(db, cube, &depth, max_depth, basic_moves, reverse_basic_moves, basic_moves_len);
    write_db_to_file(db, sizeof(uint8_t) * possible_combinations, db_name);
    free(db);
}

uint8_t * load_db(
    int max_depth, const char **basic_moves, const char **reverse_basic_moves,
    int basic_moves_len, const char *db_name, int possible_combinations
) {
    FILE * file = fopen(db_name, "rb");
    if (file == NULL) {
        create_and_store_db(max_depth, basic_moves, reverse_basic_moves, basic_moves_len, db_name, possible_combinations);
        file = fopen(db_name, "rb");
        if (file == NULL) {
            return NULL;
        }
    }
    uint8_t *db = calloc(sizeof(uint8_t), possible_combinations);
    fread(db, sizeof(uint8_t), possible_combinations, file);
    return db;
}