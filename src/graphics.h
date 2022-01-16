#include <graphx.h>
#include <stdio.h>
#include <tice.h>

#define LANE_HEIGHT     40
#define LANE_Y_OFFSET   20
#define LANE_TOP_MARGIN 1

#define START_GAME_COUNTDOWN_MASK 3

void initialize_graphics();
void cleanup_graphics();

void update_first_time();

void draw_menu();
void draw_settings();
void draw_configure();
void draw_pause_menu();
void draw_game();