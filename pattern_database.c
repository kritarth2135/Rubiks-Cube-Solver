#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pattern_database.h"
#include "rubiks_cube.h"

// The order of the moves in these two arrays matter as some of the conditions depend on the
// order of these arrays in create_db function
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
const char *FIRST_EDGE_DB_NAME = "first_edge_db.bin";
const char *LAST_EDGE_DB_NAME = "last_edge_db.bin";

const int FOUR_BITS_MASK = 0b1111;

int write_db_to_file(uint8_t *db, int size_of_db, const char *db_name) {
    FILE *file = fopen(db_name, "wb");
    if (file == NULL) {
        return 1;
    }
    if (fwrite(db, sizeof(uint8_t), size_of_db, file) != size_of_db) {
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}

int get_four_bits(uint8_t *array, uint64_t index) {
    uint64_t byte_index = index / 2;
    if (index % 2 == 0) {
        return array[byte_index] >> 4;
    }
    else {
        return array[byte_index] & FOUR_BITS_MASK;
    }
}

void set_four_bits(uint8_t *array, uint64_t index, int value) {
    uint64_t byte_index = index / 2;
    value &= FOUR_BITS_MASK;
    if (index % 2 == 0) {
        value <<= 4;
        array[byte_index] &= ~(FOUR_BITS_MASK << 4); // Clearing out first four bits
        array[byte_index] |= value;
    }
    else {
        array[byte_index] &= ~FOUR_BITS_MASK;
        array[byte_index] |= value;
    }
}

int create_and_store_db(
    const char *db_name, int max_depth, uint64_t possible_combinations,
    int (*db_creation_fun)(uint8_t *, RubiksCube *, int *, int, long long unsigned *, int *)
) {
    uint64_t size = (possible_combinations / 2) + 1;
    uint8_t *db = calloc(size, sizeof(uint8_t));
    if (db == NULL) {
        return 1;
    }
    RubiksCube *cube = create_rubiks_cube();
    if (cube == NULL) {
        return 1;
    }
    int depth = 0;
    int previous_move_indexes[max_depth];
    long long unsigned no_of_nodes_processed = 0;

    clock_t begin, end;
    begin = clock();
    if (db_creation_fun(db, cube, &depth, max_depth, &no_of_nodes_processed, previous_move_indexes) == 1) {
        printf("%s creation failed as the function tried to write at an invalid index.\n", db_name);
        free(cube);
        free(db);
        return 1;
    }
    end = clock();
    RubiksCube *new_cube = create_rubiks_cube();
    if (new_cube == NULL) {
        return 1;
    }
    if (!is_equal(cube, new_cube)) {
        printf("%s creation failed as the used cube is NOT in the same state as it was before starting to create the database.\n", db_name);
        free(cube);
        free(db);
        free(new_cube);
        return 1;
    }
    if (write_db_to_file(db, size, db_name) != 0) {
        free(cube);
        free(db);
        free(new_cube);
        return 1;
    }
    printf(
        "Successfully created %s in %f seconds. (Processed %llu nodes at depth %i)\n",
        db_name,
        (double)(end - begin) / CLOCKS_PER_SEC,
        no_of_nodes_processed,
        max_depth
    );
    free(db);
    free(cube);
    free(new_cube);
    return 0;
}

uint8_t * load_db(
    const char *db_name, int max_depth, uint64_t possible_combinations,
    int (*db_creation_fun)(uint8_t *, RubiksCube *, int *, int, long long unsigned *, int *)
) {
    FILE * file = fopen(db_name, "rb");
    if (file == NULL) {
        printf("%s not found, creating it... \n", db_name);
        if (create_and_store_db(db_name, max_depth, possible_combinations, db_creation_fun) == 1) {
            return NULL;
        }
        file = fopen(db_name, "rb");
        if (file == NULL) {
            return NULL;
        }
    }
    uint64_t size = (possible_combinations / 2) + 1;
    uint8_t *db = calloc(size, sizeof(uint8_t));
    if (db == NULL) {
        return NULL;
    }
    clock_t begin, end;
    begin = clock();
    if (fread(db, sizeof(uint8_t), size, file) != size) {
        fclose(file);
        free(db);
        return NULL;
    }
    end = clock();
    printf("Loaded %s in %f seconds.\n", db_name, (double)(end - begin) / CLOCKS_PER_SEC);

    fclose(file);
    return db;
}