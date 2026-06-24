#include <stdio.h>

#include "rubiks_cube.h"

int main(void) {
    rubiks_cube *cube = create_rubiks_cube();
    if (cube == NULL) {
        return 1;
    }
    print_cube(cube);
    free_rubiks_cube(cube);
    return 0;
}