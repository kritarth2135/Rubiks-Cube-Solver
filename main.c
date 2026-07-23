#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "rubiks_cube.h"
#include "pattern_database.h"
#include "solver.h"

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
        printf("\nEnter move ('a' to solve, 'q' to exit): ");
        get_input(buffer);
        if (buffer[0] == 'q' && buffer[1] == '\0') {
            break;
        }

        if (buffer[0] == 'a' && buffer[1] == '\0') {
            RubiksCube *copy = copy_cube(cube);
            RubiksCube *goal = create_rubiks_cube();

            Move solution[SOLVER_MAX_DEPTH];
            int solution_len = 0;
            if (solve_cube(copy, goal, corner_db, solution, &solution_len)) {
                printf("Solution: ");
                for (int i = 0; i < solution_len; i++) {
                    printf("%s ", STRING_REPRESENTATION_OF_MOVES[solution[i]]);
                }
                printf("\n");
                free(copy);
                free(goal);

                printf("Apply solution? (y/n): ");
                char choice = getchar();
                if (choice == 'y') {
                    for (int i = 0; i < solution_len; i++) {
                        make_move(cube, solution[i]);
                    }
                }
            }
            else {
                free(copy);
                free(goal);
            }

            continue;
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