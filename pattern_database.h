#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <stdint.h>

#include "rubiks_cube.h"

#define NUMBER_OF_INDEXES 3
#define CORNER_DB_INDEX 0
#define FIRST_SIX_EDGE_DB_INDEX 1
#define LAST_SIX_EDGE_DB_INDEX 2

uint64_t encode_centres(RubiksCube *cube);
void encode_corners_and_edges(RubiksCube *cube, uint64_t indexes[NUMBER_OF_INDEXES]);

#endif