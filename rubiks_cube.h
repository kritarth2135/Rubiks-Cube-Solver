#include <stdint.h>

#define FACES_OF_CUBE 6

#define WHITE  0
#define BLUE   1
#define ORANGE 2
#define GREEN  3
#define RED    4
#define YELLOW 5

typedef struct {
    uint64_t bitboards[FACES_OF_CUBE];
} RubiksCube;

RubiksCube * create_cube(void);