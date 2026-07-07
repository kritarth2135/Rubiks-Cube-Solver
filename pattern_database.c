#include <stdint.h>

#include "rubiks_cube.h"
#include "pattern_database.h"

#define MAX_SIZE 12
#define ALL_POSSIBLE_CORNER_ORIENTATIONS 2187  // 3^7
#define ALL_POSSIBLE_EDGE_ORIENTATIONS 2048    // 2^11

// Precomputed factorials
const int FACTORIAL[] = {
    1,
    1,
    2,
    6,
    24,
    120,
    720,
    5040,
    40320,
    362880,
    3628800,
    39916800,
    479001600
};

int lehmer_idx(int array[], int size) {
    if (size > MAX_SIZE) {
        return -1;
    }
    int count_array[size];

    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[i]) {
                count++;
            }
        }
        count_array[i] = count;
    }

    int idx = 0;
    for (int i = 0; i < size; i++) {
        idx += count_array[i] * FACTORIAL[size - 1 - i];
    }
    return idx;
}

int to_base_10(int array[], int size, int base) {
    int value = 0;
    for (int i = 0; i < size; i++) {
        value = value * base + array[size - 1 - i];
    }
    return value;
}

uint64_t encode_corners(RubiksCube *cube) {
    int positions[NUMBER_OF_CORNERS];
    int orientations[NUMBER_OF_CORNERS];
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        positions[i] = cube->corner_positions[i];
        orientations[i] = cube->corner_orientations[i];
    }
    return (lehmer_idx(positions, NUMBER_OF_CORNERS) * ALL_POSSIBLE_CORNER_ORIENTATIONS) + to_base_10(orientations, NUMBER_OF_CORNERS - 1, CORNER_SIDES);
}

uint64_t encode_edges(RubiksCube *cube) {
    int positions[NUMBER_OF_EDGES];
    int orientations[NUMBER_OF_EDGES];
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        positions[i] = cube->edge_positions[i];
        orientations[i] = cube->edge_orientations[i];
    }
    return (lehmer_idx(positions, NUMBER_OF_EDGES) * ALL_POSSIBLE_EDGE_ORIENTATIONS) + to_base_10(orientations, NUMBER_OF_EDGES - 1, EDGE_SIDES);
}