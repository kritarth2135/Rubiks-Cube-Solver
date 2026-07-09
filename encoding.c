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

void encode_cube(RubiksCube *cube, uint64_t indexes[NUMBER_OF_INDEXES]) {
    int centres[NUMBER_OF_COLORS];
    for (int i = 0; i < NUMBER_OF_COLORS; i++) {
        centres[i] = cube->centres[i];
    }
    indexes[CENTRE_DB_INDEX] = lehmer_idx(centres, NUMBER_OF_COLORS);

    int corner_positions[NUMBER_OF_CORNERS];
    int corner_orientations[NUMBER_OF_CORNERS];
    for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
        corner_positions[i] = cube->corner_positions[i];
        corner_orientations[i] = cube->corner_orientations[i];
    }
    indexes[CORNER_DB_INDEX] = (
        (lehmer_idx(corner_positions, NUMBER_OF_CORNERS) * ALL_POSSIBLE_CORNER_ORIENTATIONS) +
        to_base_10(corner_orientations, NUMBER_OF_CORNERS, CORNER_SIDES)
    );

    int first_six_edge_positions[NUMBER_OF_EDGES / 2], last_six_edge_positions[NUMBER_OF_EDGES / 2];
    int first_six_edge_orientations[NUMBER_OF_EDGES / 2], last_six_edge_orientations[NUMBER_OF_EDGES / 2];
    for (int i = 0; i < NUMBER_OF_EDGES; i++) {
        int edge = cube->edge_positions[i];
        if (edge == UF || edge == UL || edge == UB || edge == UR || edge == FL || edge == BL) {
            first_six_edge_positions[edge] = i;
            first_six_edge_orientations[edge] = cube->edge_orientations[i];
        }
        else {
            last_six_edge_positions[edge - (NUMBER_OF_EDGES / 2)] = i;
            last_six_edge_orientations[edge - (NUMBER_OF_EDGES / 2)] = cube->edge_orientations[i];
        }
    }
    indexes[FIRST_SIX_EDGE_DB_INDEX] = (
        (lehmer_idx(first_six_edge_positions, NUMBER_OF_EDGES / 2) * ALL_POSSIBLE_SIX_EDGE_ORIENTATIONS) +
        to_base_10(first_six_edge_orientations, NUMBER_OF_EDGES / 2, EDGE_SIDES)
    );
    indexes[LAST_SIX_EDGE_DB_INDEX] = (
        (lehmer_idx(last_six_edge_positions, NUMBER_OF_EDGES / 2) * ALL_POSSIBLE_SIX_EDGE_ORIENTATIONS) +
        to_base_10(last_six_edge_orientations, NUMBER_OF_EDGES / 2, EDGE_SIDES)
    );
}