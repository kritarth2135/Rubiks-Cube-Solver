#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <stdint.h>

#include "rubiks_cube.h"

uint64_t encode_corners(RubiksCube *cube);
uint64_t encode_edges(RubiksCube *cube);

#endif