#include <stdlib.h>
#include <stdio.h>

#include "rubiks_cube.h"

const colors COLORS[NUMBER_OF_FACES] = {WHITE, GREEN, RED, BLUE, ORANGE, YELLOW};

rubiks_cube * create_rubiks_cube(void) {
    // Default orientation: Red face on front and White on top

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

        temp->color = COLORS[i];
        for (int j = 0; j < FACE_DIMENSION; j++) {
            for (int k = 0; k < FACE_DIMENSION; k++) {
                temp->facets[j][k] = COLORS[i];
            }
        }

        cube->faces[i] = temp;
    }

    return cube;
}

void free_rubiks_cube(rubiks_cube *cube) {
    for (int i = 0; i < NUMBER_OF_FACES; i++) {
        free(cube->faces[i]);
    }
    free(cube);
    return;
}

void print_cube(rubiks_cube *cube) {
    char color_char[] = {'W', 'G', 'R', 'B', 'O', 'Y'};

    for (int i = 0; i < FACE_DIMENSION; i++) {
        printf("      ");
        for (int j = 0; j < FACE_DIMENSION; j++) {
            printf("%c ", color_char[cube->faces[0]->facets[i][j]]);
        }
        printf("\n");
    }
    for (int j = 0; j < FACE_DIMENSION; j++) {
        for (int i = 1; i < 5; i++) {
            for (int k = 0; k < FACE_DIMENSION; k++) {
                printf("%c ", color_char[cube->faces[i]->facets[j][k]]);
            }
        }
        printf("\n");
    }
    for (int i = 0; i < FACE_DIMENSION; i++) {
        printf("      ");
        for (int j = 0; j < FACE_DIMENSION; j++) {
            printf("%c ", color_char[cube->faces[5]->facets[i][j]]);
        }
        printf("\n");
    }
}