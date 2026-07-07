#include <stdint.h>

#include "rubiks_cube.h"
#include "pattern_database.h"

#define CORNER_POSITION_BITS 3
#define CORNER_ORIENTATION_BITS 2
#define EDGE_POSITION_BITS 4
#define EDGE_ORIENTATION_BITS 1

// Each corner is encoded into 5 bits, the first 3 bits indicating the position
// of the corner in the cube and the other 2 bits indicating the orientation of
// the corner.
uint64_t encode_corners(RubiksCube *cube) {
    uint64_t value = 0;
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        value <<= CORNER_POSITION_BITS;
        value |= cube->corner_positions[i];
        value <<= CORNER_ORIENTATION_BITS;
        value |= cube->corner_orientations[i];
    }
    return value;
}

// Each edge is encoded into 5 bits, the first 4 bits indicating the position
// of the edge in the cube and the other 1 bit indicating the orientation of
// the edge.
uint64_t encode_edges(RubiksCube *cube) {
    uint64_t value = 0;
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        value <<= EDGE_POSITION_BITS;
        value |= cube->edge_positions[i];
        value <<= EDGE_ORIENTATION_BITS;
        value |= cube->edge_orientations[i];
    }
    return value;
}