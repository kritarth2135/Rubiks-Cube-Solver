#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#include "rubiks_cube.h"

// Colors
const uint8_t WHITE = 0;
const uint8_t GREEN = 1;
const uint8_t RED = 2;
const uint8_t BLUE = 3;
const uint8_t ORANGE = 4;
const uint8_t YELLOW = 5;

const uint8_t COLORS[NUMBER_OF_FACES] = {WHITE, GREEN, RED, BLUE, ORANGE, YELLOW};

// Directions
const int NORTH = 0;
const int SOUTH = 1;
const int EAST = 2;
const int WEST = 3;

// Relative faces in order: north, south, east, west
const uint8_t RELATIVE_FACES[NUMBER_OF_FACES][NUMBER_OF_DIRECTIONS] = {
    {ORANGE, RED, BLUE, GREEN},
    {WHITE, YELLOW, RED, ORANGE},
    {WHITE, YELLOW, BLUE, GREEN},
    {WHITE, YELLOW, ORANGE, RED},
    {WHITE, YELLOW, GREEN, BLUE},
    {RED, ORANGE, BLUE, GREEN}
};

// Facet locations
const int CENTRE_FACET[] = {1, 1};
const int NORTH_FACET[] = {0, 1};
const int SOUTH_FACET[] = {2, 1};
const int EAST_FACET[] = {1, 2};
const int WEST_FACET[] = {1, 0};
const int NORTH_EAST_FACET[] = {0, 2};
const int NORTH_WEST_FACET[] = {0, 0};
const int SOUTH_EAST_FACET[] = {2, 2};
const int SOUTH_WEST_FACET[] = {2, 0};

rubiks_cube * create_rubiks_cube(void) {
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