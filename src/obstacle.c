#include "obstacle.h"
#include "pony_express.h"

struct Obstacle obstacles[MAX_OBSTACLES];

void clear_obstacles() {
    for (int c = 0; c < MAX_OBSTACLES; c++) {
        obstacles[c].used = 0;
    }
}

int get_obstacle_x(struct Obstacle* obstacle) {
    if (!obstacle->used) return -1;
    switch (obstacle->type) {
        case BOULDER:
            return (obstacle->data.boulder.x);
    }
}

int get_obstacle_lane(struct Obstacle* obstacle) {
    if (!obstacle->used) return -1;
    switch (obstacle->type) {
        case BOULDER:
            return (obstacle->data.boulder.lane);
    }
}

bool detect_obstacle(struct Obstacle* obstacle, int lane, int x) {
    if (!obstacle->used) return 0;
    switch (obstacle->type) {
        case BOULDER:
            return detect_boulder(&(obstacle->data.boulder), lane, x);
    }
}

void update_obstacle(struct Obstacle* obstacle) {
    if (!obstacle->used) return;
    switch (obstacle->type) {
        case BOULDER:
            ;
            struct Boulder_data* boulder = &(obstacle->data.boulder);
            boulder->x                  -= is_moving ? MOVE_SPEED : 0;
            if (boulder->x < -BOULDER_WIDTH) {
                obstacle->used = false;
            }
            break;
    }
}

void create_boulder_obstacle(int lane) {
    struct Boulder_data boulder = { .x = LCD_WIDTH, .lane = lane };
    union Obstacle_data data    = { .boulder = boulder };

    for (int c = 0; c < MAX_OBSTACLES; c++) {
        struct Obstacle* obstacle = &obstacles[c];
        if (obstacle->used) continue;
        obstacle->data = data;
        obstacle->used = true;
        obstacle->type = BOULDER;
        return;
    }
}