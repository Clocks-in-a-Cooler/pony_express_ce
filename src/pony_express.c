#include "graphics.h"
#include "pony_express.h"
#include "envelope.h"

bool running = true;

enum Game_state game_state = MENU;
int menu_option            = PLAY;
int pause_menu_option      = CONTINUE;

struct Controls player_controls = {
    .up   = kb_KeyUp,
    .down = kb_KeyDown
};

int score   = 0;
long frames = 0;

#define STARTING_LANE 2
float lane           = STARTING_LANE;
int destination_lane = STARTING_LANE;

int pose = 1;

void update_menu() {
    if (check_key(kb_KeyClear) & KEY_RELEASED) {
        running = false;
        return;
    }
    if (check_key(kb_KeyEnter) & KEY_RELEASED) {
        // if (!enter_key_released) {
        //     enter_key_released = true;
        //     return;
        // }

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

void update_pause_menu() {
    if (check_key_released(kb_KeyClear)) {
        game_state = MENU;
    }

    if (check_key_released(kb_KeyEnter)) {
        switch ((enum Pause_menu_options) pause_menu_option) {
            case CONTINUE:
                game_state = PLAYING;
                break;
            case EXIT_GAME:
            case PAUSE_MENU_OPTIONS_N: // shouldn't happen, but if i don't include this the compiler will complain
                game_state = MENU;
                pause_menu_option = 0;
                break;
        }
    }

    if (check_key_pressed(kb_KeyUp)) {
        pause_menu_option--;
    }
    if (check_key_pressed(kb_KeyDown)) {
        pause_menu_option++;
    }
    pause_menu_option = pause_menu_option < 0 ? (pause_menu_option + PAUSE_MENU_OPTIONS_N) : (pause_menu_option % PAUSE_MENU_OPTIONS_N);
}

void start_game() {
    score  = 0;
    frames = 0;

    lane = destination_lane = STARTING_LANE;

    clear_envelopes();
    // remember to clear other obstacles, too

    game_state = PLAYING;
}

#define LANE_TOLERANCE 0.1
#define LANE_SWITCH_SPEED 0.3

void update_game() {
    if (check_key_released(kb_KeyClear)) {
        game_state = PAUSED;
    }

    if (check_key_pressed(player_controls.up)) {
        destination_lane = destination_lane - 1;
    }
    if (check_key_pressed(player_controls.down)) {
        destination_lane = destination_lane + 1;
    }

    // destination_lane = clamp(destination_lane, (MAX_LANES - 1), 0);
    if (destination_lane < 0) {
        destination_lane = 0;
    }
    if (destination_lane >= MAX_LANES) {
        destination_lane = MAX_LANES - 1;
    }

    // for now
    lane += (destination_lane - lane) * LANE_SWITCH_SPEED;

    if (fabsf(destination_lane - lane) < LANE_TOLERANCE) {
        lane = destination_lane;
    }

    // update and create envelopes for the player to pick up
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        struct Envelope* e = &envelopes[c];
        if (!e->used) continue;
        update_envelope(e);
        if (check_collision(e->lane, e->x, ENVELOPE_WIDTH)) {
            e->used = false;
            score++;
        }
    }

    if (!(frames % 4)) {
        pose = (pose + 1) % 8 + 1;
    }

    // gotta come up with a better way to generate envelopes...
    // groups of envelopes? lines of envelopes?
    if (!(rand() % 2) && !(frames % ENVELOPE_FREQUENCY)) {
        int lane = rand() % MAX_LANES;
        add_envelope(lane);
    }
}

bool check_collision(int lane, int x, int width) {
    if (lane != destination_lane) return false;
    if (x + width < PLAYER_X) return false;
    if (PLAYER_X + PLAYER_SIZE < x) return false;

    return true;
}