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
#define POSSIBLE_SIX_EDGE_COMBINATIONS 42577920 // 12!/6! * 2^6
#define MAX_DEPTH 12

extern const char *BASIC_MOVES[NUMBER_OF_BASIC_MOVES];
extern const char *REVERSE_BASIC_MOVES[NUMBER_OF_BASIC_MOVES];

extern const char *CORNER_DB_NAME;
extern const char *FIRST_SIX_EDGE_DB_NAME;
extern const char *LAST_SIX_EDGE_DB_NAME;

uint64_t encode_centres(RubiksCube *cube);
void encode_corners_and_edges(RubiksCube *cube, uint64_t indexes[NUMBER_OF_INDEXES]);

uint8_t * load_db(
    int max_depth, const char **basic_moves, const char **reverse_basic_moves,
    int basic_moves_len, const char *db_name, int possible_combinations
);

#endif