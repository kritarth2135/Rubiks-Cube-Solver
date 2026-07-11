#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "rubiks_cube.h"
#include "pattern_database.h"

#define BUFFER_SIZE 50

void get_input(char buffer[BUFFER_SIZE]);

int main(void) {
    RubiksCube *cube = create_rubiks_cube();
    if (cube == NULL) {
        return 1;
    }

    // Loading databases
    uint8_t *corner_db = load_db(MAX_DEPTH, BASIC_MOVES, REVERSE_BASIC_MOVES, NUMBER_OF_BASIC_MOVES, CORNER_DB_NAME, POSSIBLE_CORNER_COMBINATIONS);
    uint8_t *first_six_edge_db = load_db(MAX_DEPTH, BASIC_MOVES, REVERSE_BASIC_MOVES, NUMBER_OF_BASIC_MOVES, FIRST_SIX_EDGE_DB_NAME, POSSIBLE_SIX_EDGE_COMBINATIONS);
    uint8_t *last_six_edge_db = load_db(MAX_DEPTH, BASIC_MOVES, REVERSE_BASIC_MOVES, NUMBER_OF_BASIC_MOVES, LAST_SIX_EDGE_DB_NAME, POSSIBLE_SIX_EDGE_COMBINATIONS);
    if (corner_db == NULL || first_six_edge_db == NULL || last_six_edge_db == NULL) {
        return 1;
    }
    printf("\n");

    char buffer[BUFFER_SIZE];
    uint64_t indexes[NUMBER_OF_INDEXES];
    print_cube(cube);
    print_cube_arrays(cube);
    encode_corners_and_edges(cube, indexes);
    printf("[%lu, %lu, %lu, %lu]\n", encode_centres(cube), indexes[0], indexes[1], indexes[2]);

    while (1) {
        printf("Enter move ('q' to exit): ");
        get_input(buffer);
        if (buffer[0] == 'q' && buffer[1] == '\0') {
            break;
        }
        if (make_move(cube, buffer) != 0) {
            printf("Invalid move!\n");
        }
        printf("\n");
        print_cube(cube);
        print_cube_arrays(cube);
        encode_corners_and_edges(cube, indexes);
        printf("[%lu, %lu, %lu, %lu]\n", encode_centres(cube), indexes[0], indexes[1], indexes[2]);
    }

    free(cube);
    free(corner_db);
    free(first_six_edge_db);
    free(last_six_edge_db);
    return 0;
}

void get_input(char buffer[BUFFER_SIZE]) {
    int idx = 0;
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF && idx < BUFFER_SIZE - 1) {
        buffer[idx] = ch;
        idx++;
    }
    buffer[idx] = '\0';
}