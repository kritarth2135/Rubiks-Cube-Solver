#include <stdlib.h>

#include "rubiks_cube.h"

RubiksCube * create_cube(void) {
    RubiksCube *cube = malloc(sizeof(RubiksCube));
    if (cube == NULL) {
        return NULL;
    }

    for (int i = 0; i < FACES_OF_CUBE; i++) {
        cube->bitboards[i] = 0ULL;
    }

    return cube;
}