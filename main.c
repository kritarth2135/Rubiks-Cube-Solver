#include "rubiks_cube.h"

int main(void) {
    rubiks_cube *cube = create_rubiks_cube();
    free_rubiks_cube(cube);
    return 0;
}