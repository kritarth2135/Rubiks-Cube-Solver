#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <stdint.h>

#include "rubiks_cube.h"

#define NUMBER_OF_INDEXES 3
#define CORNER_DB_INDEX 0
#define FIRST_SIX_EDGE_DB_INDEX 1
#define LAST_SIX_EDGE_DB_INDEX 2

#define NUMBER_OF_BASIC_MOVES 18
#define POSSIBLE_CORNER_COMBINATIONS 88179840 // 8! * 3^7
#define POSSIBLE_SEVEN_EDGE_COMBINATIONS 510935040 // 12!/5! * 2^7
#define POSSIBLE_EDGE_POSISION_COMBINATIONS 479001600 // 12!
#define CORNER_DB_GENERATION_MAX_DEPTH 11
#define EDGE_DB_GENERATION_MAX_DEPTH 10


extern const Move BASIC_MOVES[NUMBER_OF_BASIC_MOVES];
extern const Move REVERSE_BASIC_MOVES[NUMBER_OF_BASIC_MOVES];

extern const char *CORNER_DB_NAME;
extern const char *FIRST_EDGE_DB_NAME;
extern const char *SECOND_EDGE_DB_NAME;
extern const char *EDGE_POSITIONS_DB_NAME;



uint64_t encode_corners(RubiksCube *cube);
uint64_t encode_first_seven_edges(RubiksCube* cube);
uint64_t encode_last_seven_edges(RubiksCube* cube);
uint64_t encode_all_edge_positions(RubiksCube *cube);

int get_four_bits(uint8_t *array, uint64_t index);
void set_four_bits(uint8_t *array, uint64_t index, int value);

int write_db_to_file(uint8_t *db, int size_of_db, const char *db_name);
uint8_t * load_db(
    const char *db_name, int max_depth, uint64_t possible_combinations,
    uint64_t (*indexing_function)(RubiksCube*)
);

uint8_t * load_corner_db();
uint8_t * load_first_edge_db();
uint8_t * load_second_edge_db();
uint8_t * load_edge_position_db();

#endif