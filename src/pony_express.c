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

int collision_countdown     = -1;
int start_game_countdown    = 0;
int invincibility_countdown = 0;

float lane           = STARTING_LANE;
int destination_lane = STARTING_LANE;

int pose = 1;

const char file_name[] = "PNYXPDAT";

void update_pose() {
    if (!(frames % 4)) {
        pose = ++pose % 8;
    }
}

void init_pony_express() {
    srandom(rtc_Time());

    ti_var_t read_settings;
    if ((read_settings = ti_Open(file_name, "r"))) {
        ti_Read(&high_score, sizeof(int), 1, read_settings);
        ti_Read(&(player_controls.up), sizeof(kb_lkey_t), 1, read_settings);
        ti_Read(&(player_controls.down), sizeof(kb_lkey_t), 1, read_settings);
    }

    ti_Close(read_settings);

    // start the timers
    timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_UP;

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
    update_pose();

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

void to_menu() {
    game_state              = MENU;
    pause_menu_option       = 0;
    invincibility_countdown = 0;
    if (high_score < score) {
        high_score = score;
    }
    
    // reset the starting lane, or weird things will happen in the menu
    lane = destination_lane = STARTING_LANE;
}

void update_pause_menu() {
    if (check_key_released(kb_KeyClear)) {
        to_menu();
    }

    if (check_key_released(kb_KeyEnter) || check_key_released(kb_Key2nd)) {
        switch ((enum Pause_menu_options) pause_menu_option) {
            case CONTINUE:
                game_state = PLAYING;
                break;
            case EXIT_GAME:
            case PAUSE_MENU_OPTIONS_N:
                to_menu();
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

    start_game_countdown = MAX_START_GAME_COUNTDOWN;
    collision_countdown  = -1;

    lane = destination_lane = STARTING_LANE;

    clear_envelopes();
    clear_obstacles();

    game_state = PLAYING;
}

#define LANE_TOLERANCE 0.1
#define LANE_SWITCH_SPEED 0.3

int get_different_lane() {
    if (destination_lane == 0) {
        return 1;
    }
    if (destination_lane == MAX_LANES - 1) {
        return MAX_LANES - 2;
    }
    if (rand() % 2) {
        return destination_lane - 1;
    } else {
        return destination_lane + 1;
    }
}

#define COLLISION_DURATION 40

void update_game() {
    if (check_key_released(kb_KeyClear)) {
        game_state = PAUSED;
    }

    update_pose();

    if (start_game_countdown) {
        start_game_countdown--;
        return;
    }

    if (collision_countdown < 0) {
        if (check_key_pressed(player_controls.up)) {
            destination_lane = destination_lane - 1;
        }
        if (check_key_pressed(player_controls.down)) {
            destination_lane = destination_lane + 1;
        }
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

    collision_countdown = collision_countdown < 0 ? -1 : collision_countdown - 1;
    
    if (invincibility_countdown > 0) {
        invincibility_countdown--;
    }

    for (int c = 0; c < MAX_OBSTACLES; c++) {
        update_obstacle(&obstacles[c]);
        if (detect_obstacle(&(obstacles[c]), lane, PLAYER_X) && invincibility_countdown <= 0) {
            // start the collision timer
            if (collision_countdown < 0) {
                collision_countdown = COLLISION_DURATION;
            }
        }
    }

    if (collision_countdown == 0) {
        // respawn in a different lane
        score -= score > OBSTACLE_PENALTY ? OBSTACLE_PENALTY : score;
        lane   = destination_lane = get_different_lane();

        invincibility_countdown = MAX_INVINCIBILITY_COUNTDOWN;
    }

    // gotta come up with a better way to generate envelopes...
    // groups of envelopes? lines of envelopes?
    if (!(rand() % 2) && !(frames % ENVELOPE_FREQUENCY) && collision_countdown < 0) {
        int lane = rand() % MAX_LANES;
        if (rand() % 2) {
            add_envelope(lane);
        } else {
            create_boulder_obstacle(lane);
        }
    }
}

bool check_collision(int lane, int x, int width) {
    if (lane != destination_lane) return false;
    if (x + width < PLAYER_X) return false;
    if (PLAYER_X + PLAYER_SIZE < x) return false;

    return true;
}