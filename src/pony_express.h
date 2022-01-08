#include "customize_keys.h"
#include "obstacle.h"
#include <math.h>
#include <tice.h>
#include <fileioc.h>

/*
    game state, controls, and updating the game, all conveniently in one file
*/

extern bool running;

enum Game_state {
    MENU = 0,
    SETTINGS_MENU,
    CONFIGURE_KEY,
    PLAYING,
    PAUSED,
    GAME_OVER,
};

extern enum Game_state game_state;

enum Menu_options {
    PLAY = 0,
    SETTINGS,
    EXIT,
    MENU_OPTIONS_N,
};

extern int menu_option;

enum Settings_options {
    CONFIGURE_UP = 0,
    CONFIGURE_DOWN,
    LANGUAGE,
    BACK,
    SETTINGS_OPTIONS_N,
};

extern int settings_option;

enum Pause_menu_options {
    CONTINUE = 0,
    EXIT_GAME,
    PAUSE_MENU_OPTIONS_N
};

extern int pause_menu_option;

struct Controls {
    kb_lkey_t up;
    kb_lkey_t down;
};

extern struct Controls player_controls;

#define OBSTACLE_PENALTY 3

extern int score;
extern int high_score;
extern long frames;

extern int collision_countdown;
extern int start_game_countdown;
extern int invincibility_countdown;

#define COLLISION_MAX_COUNTDOWN  50
#define COLLISION_HALF_COUNTDOWN 35

#define MAX_START_GAME_COUNTDOWN 34

#define MAX_INVINCIBILITY_COUNTDOWN 150

#define is_moving (collision_countdown < 0 || collision_countdown > COLLISION_HALF_COUNTDOWN)

#define max(x, y) x > y ? x : y
#define min(x, y) x < y ? x : y
#define clamp(a, upper, lower) min(max(a, lower), upper)

#define MAX_LANES     5
#define STARTING_LANE 2
extern float lane;
extern int destination_lane;
extern int pose;

void init_pony_express();
void cleanup_pony_express();

void update_menu();
void start_settings();
void update_settings();
void update_configure();
void update_pause_menu();
void start_game();
void update_game();

bool check_collision(int lane, int x, int width);

#define MOVE_SPEED 3 // adjust as needed

#define PLAYER_SIZE 36
#define PLAYER_X    20