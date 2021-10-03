#ifndef _BOULDER
#define _BOULDER

struct Boulder_data {
    int x;
    int lane;
};

#define BOULDER_WIDTH 30

int detect_boulder(struct Boulder_data* boulder, int lane, int x);

#endif