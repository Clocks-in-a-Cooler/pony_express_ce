// include specific obstacles' headers here

enum Obstacle_type {
    PLACEHOLDER = 0,
};

union Obstacle_data {

};

struct Obstacle {
    int lane;
    int x;
    union Obstacle_data data;
};