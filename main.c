#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cube/cube.h"
#include "cube/moves.h"
#include "db_generation/pattern_database.h"
#include "solver/solver.h"
#include "ui/ui.h"

#define DEBUG_FLAG 1
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
    uint8_t *first_edge_db = load_first_edge_db();
    if (first_edge_db == NULL) {
        return 1;
    }
    uint8_t *second_edge_db = load_second_edge_db();
    if (second_edge_db == NULL) {
        return 1;
    }
    uint8_t *edge_position_db = load_edge_position_db();
    if (edge_position_db == NULL) {
        return 1;
    }
    printf("\n");

    Move scramble[] = {
        R_NORMAL, F_TWO, L_NORMAL, B_TWO, D_NORMAL, U_TWO, R_NORMAL, B_PRIME, R_PRIME, F_NORMAL,
        D_NORMAL, B_NORMAL, R_TWO, L_NORMAL, D_PRIME, R_TWO, L_NORMAL
    };
    int scramble_len = 0;
    for (int i = 0; i < scramble_len; i++) {
        make_move(cube, scramble[i]);
    }
    Move solution[SOLVER_MAX_DEPTH];
    int solution_len = 0;

    int ret_val = ui(cube, corner_db, first_edge_db, second_edge_db, edge_position_db, solution, &solution_len, DEBUG_FLAG);

    free(cube);
    free(corner_db);
    free(first_edge_db);
    free(second_edge_db);
    free(edge_position_db);

    return ret_val;
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
