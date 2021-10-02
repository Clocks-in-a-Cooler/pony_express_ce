#include "graphics.h"
#include "pony_express.h"
#include "envelope.h"

bool running = true;

enum Game_state game_state = MENU;
int menu_option            = PLAY;
int settings_option        = CONFIGURE_UP;
int pause_menu_option      = CONTINUE;

struct Controls player_controls = {
    .up   = kb_KeyUp,
    .down = kb_KeyDown
};

int score      = 0;
int high_score = 0;
long frames    = 0;

#define STARTING_LANE 2
float lane           = STARTING_LANE;
int destination_lane = STARTING_LANE;

int pose = 1;

const char file_name[] = "PNYXPDAT";

void init_pony_express() {
    srandom(rtc_Time());

    ti_var_t read_settings;
    if ((read_settings = ti_Open(file_name, "r"))) {
        ti_Read(&high_score, sizeof(int), 1, read_settings);
        ti_Read(&(player_controls.up), sizeof(kb_lkey_t), 1, read_settings);
        ti_Read(&(player_controls.down), sizeof(kb_lkey_t), 1, read_settings);
    }

    ti_Close(read_settings);

    // anything else?
}

void cleanup_pony_express() {
    ti_var_t write_settings = ti_Open(file_name, "w");
    ti_Write(&high_score, sizeof(int), 1, write_settings);
    ti_Write(&(player_controls.up), sizeof(kb_lkey_t), 1, write_settings);
    ti_Write(&(player_controls.down), sizeof(kb_lkey_t), 1, write_settings);
    ti_Close(write_settings);
}

void update_menu() {
    if (check_key(kb_KeyClear) & KEY_RELEASED) {
        running = false;
        return;
    }
    if (check_key(kb_KeyEnter) & KEY_RELEASED) {
        if (enter_key_latch) {
            enter_key_latch = false;
            return;
        }

        switch ((enum Menu_options) menu_option) {
            case PLAY:
                start_game();
                break;
            case SETTINGS:
                start_settings();
                break;
            case EXIT:
            case MENU_OPTIONS_N: // this one shouldn't even happen, but i have to suppress the compiler warnings
                running = false;
                return;

        }
    }
    if (check_key(kb_Key2nd) & KEY_RELEASED) {
        switch ((enum Menu_options) menu_option) {
            case PLAY:
                start_game();
                break;
            case SETTINGS:
                start_settings();
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

void return_to_menu() {
    game_state  = MENU;
    menu_option = 0;
}

void start_settings() {
    settings_option = 0;
    game_state      = SETTINGS_MENU;
}

void update_settings() {
    if (check_key_released(kb_KeyClear)) {
        return_to_menu();
        return;
    }

    if (check_key_released(kb_KeyEnter) || check_key_released(kb_Key2nd)) {
        switch (settings_option) {
            case CONFIGURE_UP:
            case CONFIGURE_DOWN:
                game_state = CONFIGURE_KEY;
                break;
            case LANGUAGE:
                break;
            case BACK:
                game_state  = MENU;
                menu_option = 0;
                break;
            case SETTINGS_OPTIONS_N:
                break;
        }
    }

    if (check_key_pressed(kb_KeyDown)) {
        settings_option++;
    }
    if (check_key_pressed(kb_KeyUp)) {
        settings_option--;
    }

    settings_option = settings_option < 0 ? (settings_option + SETTINGS_OPTIONS_N) : (settings_option % SETTINGS_OPTIONS_N);
}

void update_configure() {
    switch (settings_option) {
        case CONFIGURE_UP:
        case CONFIGURE_DOWN:
            ;
            kb_lkey_t next_key = get_next_key();
            if (next_key) {
                if (settings_option == CONFIGURE_UP) {
                    player_controls.up = next_key;
                } else if (settings_option == CONFIGURE_DOWN) {
                    player_controls.down = next_key;
                }
                game_state = SETTINGS_MENU;
            }
            break;
        case LANGUAGE:
        default:
            // nothing
            ;
    }
}

void update_pause_menu() {
    if (check_key_released(kb_KeyClear)) {
        game_state        = MENU;
        pause_menu_option = 0;
        if (high_score < score) {
            high_score = score;
        }
    }

    if (check_key_released(kb_KeyEnter) || check_key_released(kb_Key2nd)) {
        switch ((enum Pause_menu_options) pause_menu_option) {
            case CONTINUE:
                game_state = PLAYING;
                break;
            case EXIT_GAME:
            case PAUSE_MENU_OPTIONS_N: // shouldn't happen, but if i don't include this the compiler will complain
                game_state        = MENU;
                pause_menu_option = 0;
                if (high_score < score) {
                    high_score = score;
                }
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