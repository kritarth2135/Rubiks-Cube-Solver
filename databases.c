#include <stdint.h>

#include "pattern_database.h"


uint8_t * load_corner_db() {
    return load_db(
        CORNER_DB_NAME, CORNER_DB_GENERATION_MAX_DEPTH, POSSIBLE_CORNER_COMBINATIONS,
        encode_corners
    );
}

uint8_t * load_first_edge_db() {
    return load_db(
        FIRST_EDGE_DB_NAME, EDGE_DB_GENERATION_MAX_DEPTH, POSSIBLE_SEVEN_EDGE_COMBINATIONS,
        encode_first_seven_edges
    );
}

uint8_t * load_second_edge_db() {
    return load_db(
        SECOND_EDGE_DB_NAME, EDGE_DB_GENERATION_MAX_DEPTH, POSSIBLE_SEVEN_EDGE_COMBINATIONS,
        encode_last_seven_edges
    );
}

uint8_t * load_edge_position_db() {
    return load_db(
        EDGE_POSITIONS_DB_NAME, EDGE_DB_GENERATION_MAX_DEPTH, POSSIBLE_EDGE_POSISION_COMBINATIONS,
        encode_all_edge_positions
    );
}