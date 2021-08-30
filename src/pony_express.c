#include "graphics.h"
#include "pony_express.h"

bool running = true;

enum Game_state game_state = MENU;
int menu_option            = PLAY;

struct Controls player_controls = {
    .up   = kb_KeyUp,
    .down = kb_KeyDown
};

int score   = 0;
long frames = 0;

#define STARTING_LANE 2
float lane           = STARTING_LANE;
int destination_lane = STARTING_LANE;

void update_menu() {
    if (check_key(kb_KeyClear) & KEY_RELEASED) {
        running = false;
        return;
    }
    if (check_key(kb_KeyEnter) & KEY_PRESSED) {
        switch ((enum Menu_options) menu_option) {
            case PLAY:
                start_game();
                break;
            case EXIT:
            case MENU_OPTIONS_N: // this one shouldn't even happen, but i have to suppress the compiler warnings
                running = false;
                return;

        }
    }
    if (check_key(kb_KeyUp) & KEY_PRESSED) {
        menu_option--;
    }
    if (check_key(kb_KeyDown) & KEY_PRESSED) {
        menu_option++;
    }
    menu_option = menu_option < 0 ? (menu_option + MENU_OPTIONS_N) : (menu_option % MENU_OPTIONS_N);
}

void start_game() {
    score  = 0;
    frames = 0;

    lane = destination_lane = STARTING_LANE;

    game_state = PLAYING;
}

void update_game() {
    if (check_key_released(kb_KeyClear)) {
        game_state = MENU;
    }

    if (check_key_pressed(player_controls.up)) {
        destination_lane = (int)(lane - 1);
    }
    if (check_key_pressed(player_controls.down)) {
        destination_lane = (int)(lane + 1);
    }

    destination_lane = clamp(destination_lane, MAX_LANES, 0);

    // for now
    lane = destination_lane;
}