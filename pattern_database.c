#include <stdint.h>

#include "rubiks_cube.h"
#include "pattern_database.h"

#define CUBIE_POSITION_BITS 3
#define CUBIE_ORIENTATION_BITS 2

// Each cubie is encoded into 5 bits, the first 3 bits indicating the position
// of the cubie in the cube and the other 2 bits indicating the orientation of
// the cubie. So all the cubies are encoded into a 40 bit unsigned integer.
uint64_t encode_corners(RubiksCube *cube) {
    uint64_t value = 0;
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        value <<= CUBIE_POSITION_BITS;
        value |= cube->corner_positions[i];
        value <<= CUBIE_ORIENTATION_BITS;
        value |= cube->corner_orientations[i];
    }
    return value;
}

unsigned int encode_edges(RubiksCube *cube) {
    return 0;
}