#ifndef UI_H
#define UI_H

#include <stdint.h>

#include "cube/moves.h"
#include "cube/cube.h"

#define MAX_MESSAGE_LEN 50

int ui(
    RubiksCube *cube, uint8_t *corner_db, uint8_t *first_edge_db, uint8_t *second_edge_db,
    uint8_t *edge_position_db, Move *solution_array, int *solution_len, int debug
);
void info_print(const char *message, bool append);
void debug_print(const char *message, bool append);

#endif
