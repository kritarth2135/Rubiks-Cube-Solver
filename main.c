// #include <stdio.h>
#include <stdlib.h>

#include "rubiks_cube.h"

int main(void) {
    RubiksCube *cube = create_rubiks_cube();
    if (cube == NULL) {
        return 1;
    }
    print_cube(cube);
    free(cube);
    return 0;
}