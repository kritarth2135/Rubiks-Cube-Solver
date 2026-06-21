#include <stdlib.h>

#include "rubiks_cube.h"

rubiks_cube * create_cube(void) {
    rubiks_cube *cube = malloc(sizeof(rubiks_cube));
    if (cube == NULL) {
        return NULL;
    }

    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        face *temp = malloc(sizeof(face));
        if (temp == NULL) {
            for (int j = i - 1; j >= 0; j--) {
                free(cube->faces[i]);
            }
            free(cube);
            return NULL;
        }

        temp->color = i;
        for (int j = 0; i < FACE_DIMENSION; i++) {
            for (int k = 0; k < FACE_DIMENSION; k++) {
                temp->facets[j][k] = i;
            }
        }
    }
}