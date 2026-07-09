#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <stdint.h>

#include "rubiks_cube.h"

#define NUMBER_OF_INDEXES 4
#define CENTRE_DB_INDEX 0
#define CORNER_DB_INDEX 1
#define FIRST_SIX_EDGE_DB_INDEX 2
#define LAST_SIX_EDGE_DB_INDEX 3

void encode_cube(RubiksCube *cube, uint64_t indexes[NUMBER_OF_INDEXES]);

#endif