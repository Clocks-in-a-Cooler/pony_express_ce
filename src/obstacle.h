#include <stdbool.h>

// include specific obstacles' headers here
#include "boulder.h"

#ifndef _OBSTACLE
#define _OBSTACLE

enum Obstacle_type {
    BOULDER = 0,
};

union Obstacle_data {
    struct Boulder_data boulder;
};

// it's an ob-struct-ion!
struct Obstacle {
    union Obstacle_data data;
    enum Obstacle_type type;
    bool used;
};


#define MAX_OBSTACLES 32
extern struct Obstacle obstacles[MAX_OBSTACLES];

void clear_obstacles();

int get_obstacle_x(struct Obstacle* obstacle);
int get_obstacle_lane(struct Obstacle* obstacle);
bool detect_obstacle(struct Obstacle* obstacle, int lane, int x);

void update_obstacle(struct Obstacle* obstacle);

void create_boulder_obstacle(int lane);

#endif