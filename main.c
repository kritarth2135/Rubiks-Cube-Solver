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
    uint8_t *corner_db = load_corner_db();
    if (corner_db == NULL) {
        return 1;
    }
    printf("\n");

    char buffer[BUFFER_SIZE];
    print_cube(cube);
    print_cube_arrays(cube);
    printf(
        "[%lu, %lu, %lu, %lu]\n",
        encode_centres(cube),
        encode_corners(cube),
        encode_edges(cube, UF, BL),
        encode_edges(cube, BR, DR)
    );

    while (1) {
        printf("Enter move ('q' to exit): ");
        get_input(buffer);
        if (buffer[0] == 'q' && buffer[1] == '\0') {
            break;
        }
        if (parse_and_make_move(cube, buffer) != 0) {
            printf("Invalid move!\n");
        }
        printf("\n");
        print_cube(cube);
        print_cube_arrays(cube);
        printf(
            "[%lu, %lu, %lu, %lu]\n",
            encode_centres(cube),
            encode_corners(cube),
            encode_edges(cube, UF, BL),
            encode_edges(cube, BR, DR)
        );
    }

    free(cube);
    free(corner_db);
    // free(first_six_edge_db);
    // free(last_six_edge_db);
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