#include "boulder.h"
#include "pony_express.h"

int detect_boulder(struct Boulder_data* boulder, int lane, int x) {
    if (lane != boulder->lane) {
        return 0;
    }
    if (boulder->x > PLAYER_X + PLAYER_SIZE + 4) {
        return 0;
    }

    if (boulder->x + BOULDER_WIDTH < PLAYER_X) {
        return 0;
    }

    return 1;
}