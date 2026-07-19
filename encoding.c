#include <stdbool.h>
#include <stdint.h>

#include "rubiks_cube.h"
#include "pattern_database.h"

#define MAX_SIZE 12
#define ALL_POSSIBLE_CORNER_ORIENTATIONS 2187  // 3^7
#define ALL_POSSIBLE_SIX_EDGE_ORIENTATIONS 64  // 2^6

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

uint8_t NUMBER_OF_ONES_IN_BINARY_REPRESENTATION[(1 << MAX_SIZE) - 1] = {0};
bool is_computed = false;

int count_ones(int number) {
    int count = 0;
    while(number > 0) {
        number &= number - 1;
        count++;
    }
    return count;
}

void compute_ones_table(void) {
    if (is_computed) {
        return;
    }
    for (int i = 0; i < (1 << MAX_SIZE) - 1; i++) {
        NUMBER_OF_ONES_IN_BINARY_REPRESENTATION[i] = count_ones(i);
    }
    is_computed = true;
}

int lehmer_idx(int array[], int size) {
    if (size > MAX_SIZE) {
        return -1;
    }
    compute_ones_table();
    int bit_string = 0; // To mark the permutations that are encountered
    int count_array[size];

    for (int i = 0; i < size; i++) {
        bit_string |= 1 << (size - 1 - array[i]);
        count_array[i] = array[i] - NUMBER_OF_ONES_IN_BINARY_REPRESENTATION[bit_string >> (size - array[i])];
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
        value = value * base + array[i];
    }
    return value;
}

uint64_t encode_centres(RubiksCube *cube) {
    int centres[NUMBER_OF_COLORS];
    for (int i = 0; i < NUMBER_OF_COLORS; i++) {
        centres[i] = cube->centres[i];
    }
    return lehmer_idx(centres, NUMBER_OF_COLORS);
}

uint64_t encode_corners(RubiksCube *cube) {
    int corner_positions[NUMBER_OF_CORNERS];
    int corner_orientations[NUMBER_OF_CORNERS];
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        corner_positions[i] = cube->corner_positions[i];
        corner_orientations[i] = cube->corner_orientations[i];
    }
    return (
        (lehmer_idx(corner_positions, NUMBER_OF_CORNERS) * ALL_POSSIBLE_CORNER_ORIENTATIONS) +
        to_base_10(corner_orientations, NUMBER_OF_CORNERS - 1, CORNER_SIDES)
    );
}

uint64_t encode_edges(RubiksCube *cube, EdgeCubie start_cubie, EdgeCubie end_cubie) {
    int number_of_edges = end_cubie - start_cubie + 1;
    int edge_positions[number_of_edges];
    int edge_orientations[number_of_edges];

    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        EdgeCubie edge = cube->edge_positions[i];
        if (edge >= start_cubie && edge <= end_cubie) {
            edge_positions[edge - start_cubie] = i;
            edge_orientations[edge - start_cubie] = cube->edge_orientations[i];
        }
    }
    return (
        (lehmer_idx(edge_positions, number_of_edges) * ALL_POSSIBLE_SIX_EDGE_ORIENTATIONS) +
        to_base_10(edge_orientations, number_of_edges, EDGE_SIDES)
    );
}