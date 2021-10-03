#include "boulder.h"

int detect_boulder(struct Boulder_data* boulder, int lane, int x) {
    if (lane != boulder->lane) {
        return 0;
    }
    return 0;
}