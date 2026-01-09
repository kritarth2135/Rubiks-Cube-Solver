#include <stdio.h>

#include "rubiks_cube.h"

int main(void) {
    RubiksCube *cube = create_cube();
    if (cube == NULL) {
        return 1;
    }

    for (int i = 0; i < FACES_OF_CUBE; i++) {
        printf("%i", cube->bitboards[i]);
    }
}