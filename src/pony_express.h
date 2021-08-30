#include "customize_keys.h"
#include <math.h>

/*
    game state, controls, and updating the game, all conveniently in one file
*/

extern bool running;

enum Game_state {
    MENU = 0,
    PLAYING,
    PAUSED,
    GAME_OVER,
};

extern enum Game_state game_state;

enum Menu_options {
    PLAY = 0,
    EXIT,
    MENU_OPTIONS_N,
};

extern int menu_option;

struct Controls {
    kb_lkey_t up;
    kb_lkey_t down;
};

extern struct Controls player_controls;

extern int score;
extern long frames;

#define max(x, y) x > y ? x : y
#define min(x, y) x < y ? x : y
#define clamp(a, upper, lower) min(max(a, lower), upper)

#define MAX_LANES 5
extern float lane;
extern int destination_lane;

void update_menu();
void start_game();
void update_game();

bool check_collision(int lane, int x, int width);

#define MOVE_SPEED 3 // adjust as needed

#define PLAYER_SIZE 36
#define PLAYER_X    20