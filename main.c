#include <stdlib.h>
#include <stdio.h>

#include "rubiks_cube.h"

#define BUFFER_SIZE 50

void get_input(char buffer[BUFFER_SIZE]);

int main(void) {
    RubiksCube *cube = create_rubiks_cube();
    if (cube == NULL) {
        return 1;
    }

    char buffer[BUFFER_SIZE];
    print_cube(cube);
    while (1) {
        printf("Enter move ('q' to exit): ");
        get_input(buffer);
        if (buffer[0] == 'q' && buffer[1] == '\0') {
            break;
        }
        if (make_move(cube, buffer) != 0) {
            printf("Invalid move!\n");
        }
        print_cube(cube);
    }

    free(cube);
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