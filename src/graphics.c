#include "graphics.h"
#include "pony_express.h"
#include "obstacle.h"
#include "envelope.h"
#include "gfx/sprites.h"
#include "font/font.h"

#define WHITE 1
#define BLACK 2
#define YELLOW 3
#define DUSTY_BROWN 4

bool use_french = false;

unsigned char* rider_sprites[6] = {
    rider1_sprite_data,
    rider2_sprite_data,
    rider4_sprite_data,
    rider6_sprite_data,
    rider10_sprite_data,
    rider11_sprite_data,
};

gfx_sprite_t* get_rider_pose(int* draw_y) {
    if (collision_countdown > 20) {
        return (gfx_sprite_t*) rider10_sprite;
    } else if (collision_countdown > 0) {
        return (gfx_sprite_t*) rider11_sprite;
    } else {
        switch (pose + 1) {
            case 1:
            case 2:
                return (gfx_sprite_t*) rider_sprites[1];
            case 3:
            case 4:
            case 8:
                *draw_y = *draw_y - 1;
                return (gfx_sprite_t*) rider_sprites[2];
            case 5:
            case 6:
            case 7:
                *draw_y = *draw_y - 2;
                return (gfx_sprite_t*) rider_sprites[3];
            default:
                return (gfx_sprite_t*) rider_sprites[0];
        }
    }
}

void draw_rider() {
    // draw the player
    gfx_SetTransparentColor(0);
    int draw_y = (int) LANE_Y_OFFSET + LANE_TOP_MARGIN + 1 + LANE_HEIGHT * lane;

    gfx_sprite_t* rider_sprite = get_rider_pose(&draw_y);

    gfx_TransparentSprite(rider_sprite, PLAYER_X, draw_y);
}

void initialize_graphics() {

    use_french = (os_GetSystemInfo())->hardwareType;

    gfx_Begin();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    gfx_SetMonospaceFont(8);
    gfx_SetTextFGColor(WHITE);
    gfx_SetTextBGColor(BLACK);
    gfx_SetTextTransparentColor(BLACK);

    fontlib_SetFont(oldie_font, 0);
    fontlib_SetForegroundColor(WHITE);
    fontlib_SetBackgroundColor(BLACK);
    fontlib_SetTransparency(true);
    gfx_SetTransparentColor(0); // the magenta we had earlier
}

void cleanup_graphics() {
    gfx_End();
}

char* EN_MENU_OPTION_NAMES[MENU_OPTIONS_N] = {
    "PLAY", "OPTIONS", "EXIT"
};

// désolée, mon français est très mal...
char* FR_MENU_OPTION_NAMES[MENU_OPTIONS_N] = {
    "JOUER", "OPTIONS", "SORTIR"
};


const gfx_sprite_t* BUTTON_SPRITE = (gfx_sprite_t*) button_sprite_data;

#define TEXT_HEIGHT          8
#define MENU_BUTTON_Y_OFFSET 160
#define MENU_BUTTON_X_OFFSET 10
#define MENU_LABEL_X_OFFSET  8
#define MENU_LABEL_Y_OFFSET  3
#define MENU_BUTTON_HEIGHT   16
#define MENU_BUTTON_WIDTH    96

void draw_menu() {
    // no fancy stuff for now, because TheAyeStride needs a break...
    // don't want to overwhelm them with more unfinished projects
    gfx_FillScreen(BLACK);
    fontlib_SetCursorPosition(8, 8);
    fontlib_DrawString(use_french ? "LE PONY EXPRESS" : "PONY EXPRESS CE");
    // serieusement, c'est le nom du doodle: «155e anniversaire du Pony Express (service de distribution rapide du courrier aux États-Unis)»

    for (int c = 0; c < MENU_OPTIONS_N; c++) {
        if (c == menu_option) {
            // probably look terrible
            fontlib_SetForegroundColor(check_key(kb_KeyEnter) & KEY_HELD ? WHITE : YELLOW);
        }

        gfx_TransparentSprite(BUTTON_SPRITE, MENU_BUTTON_X_OFFSET, MENU_BUTTON_Y_OFFSET + c * MENU_BUTTON_HEIGHT);
        fontlib_SetCursorPosition(MENU_BUTTON_X_OFFSET + MENU_LABEL_X_OFFSET, MENU_BUTTON_Y_OFFSET + c * MENU_BUTTON_HEIGHT + MENU_LABEL_Y_OFFSET);
        fontlib_DrawString(use_french ? FR_MENU_OPTION_NAMES[c] : EN_MENU_OPTION_NAMES[c]);

        fontlib_SetForegroundColor(WHITE);
    }

    // draw the middle starting lane and the player
    gfx_SetColor(DUSTY_BROWN);
    gfx_FillRectangle(0, LANE_Y_OFFSET + LANE_TOP_MARGIN + LANE_HEIGHT * STARTING_LANE, LCD_WIDTH, 38);
    draw_rider();
}

const char* EN_SETTINGS_MENU_OPTION_NAMES[SETTINGS_OPTIONS_N] = {
    "CONFIGURE UP", "CONFIGURE DOWN", "LANGUAGE", "BACK",
};

#define SETTINGS_MENU_X_OFFSET 20
#define SETTINGS_MENU_Y_OFFSET 100

void draw_settings() {
    gfx_FillScreen(BLACK);
    fontlib_SetCursorPosition(8, 8);
    fontlib_DrawString("SETTINGS");
    fontlib_SetCursorPosition(8, LCD_HEIGHT - 16);
    fontlib_DrawString("BUILD 0");

    for (int c = 0; c < SETTINGS_OPTIONS_N; c++) {
        int draw_x = SETTINGS_MENU_X_OFFSET;
        int draw_y = SETTINGS_MENU_Y_OFFSET + button_sprite_height * 2 * c;

        if (settings_option == c) {
            gfx_SetColor(WHITE);
            gfx_Rectangle_NoClip(draw_x, draw_y, button_sprite_width * 2, button_sprite_height);

            if (check_key_held(kb_KeyEnter) || check_key_held(kb_Key2nd)) {
                gfx_FillRectangle_NoClip(draw_x, draw_y, button_sprite_width * 2, button_sprite_height);
            }
        }

        fontlib_SetCursorPosition(draw_x + MENU_LABEL_X_OFFSET, draw_y + MENU_LABEL_Y_OFFSET);
        fontlib_DrawString(EN_SETTINGS_MENU_OPTION_NAMES[c]);

        draw_x += button_sprite_width / 2;
        draw_y += button_sprite_height + MENU_LABEL_Y_OFFSET;
        switch (c) {
            case CONFIGURE_UP:
                gfx_PrintStringXY("current: ", draw_x, draw_y);
                gfx_PrintString(get_key_name(player_controls.up));
                break;
            case CONFIGURE_DOWN:
                gfx_PrintStringXY("current: ", draw_x, draw_y);
                gfx_PrintString(get_key_name(player_controls.down));
                break;
            case LANGUAGE:
                gfx_PrintStringXY("current: ", draw_x, draw_y);
                gfx_PrintString(use_french ? "FRANÇAIS" : "ENGLISH");
                break;
            default:
                ;
        }
    }
}

const char* EN_PRESS_A_KEY = "PRESS A KEY...";

#define CONFIGURE_KEY_DIALOGUE_WIDTH (button_sprite_width * 2)
#define CONFIGURE_KEY_DIALOGUE_HEIGHT (MENU_BUTTON_HEIGHT * 2)
#define CONFIGURE_KEY_LABEL_X_OFFSET MENU_LABEL_X_OFFSET
#define CONFIGURE_KEY_LABEL_Y_OFFSET ((CONFIGURE_KEY_DIALOGUE_HEIGHT - 8) / 2)
#define CONFIGURE_KEY_X_OFFSET ((LCD_WIDTH - CONFIGURE_KEY_DIALOGUE_WIDTH) / 2)
#define CONFIGURE_KEY_Y_OFFSET ((LCD_HEIGHT - CONFIGURE_KEY_DIALOGUE_HEIGHT) / 2)

const char* EN_SELECT_LANGUAGE = "SELECT LANGUAGE...";

#define CONFIGURE_LANGUAGE_DIALOGUE_WIDTH 
// finish

void draw_configure() {
    draw_settings();
    switch (settings_option) {
        case CONFIGURE_UP:
        case CONFIGURE_DOWN:
            gfx_SetColor(WHITE);
            gfx_Rectangle(CONFIGURE_KEY_X_OFFSET, CONFIGURE_KEY_Y_OFFSET, CONFIGURE_KEY_DIALOGUE_WIDTH, CONFIGURE_KEY_DIALOGUE_HEIGHT);
            gfx_SetColor(BLACK);
            gfx_FillRectangle(CONFIGURE_KEY_X_OFFSET + 1, CONFIGURE_KEY_Y_OFFSET + 1, CONFIGURE_KEY_DIALOGUE_WIDTH - 2, CONFIGURE_KEY_DIALOGUE_HEIGHT - 2);
            if (frames % 50 > 25) {
                fontlib_SetCursorPosition(CONFIGURE_KEY_X_OFFSET + CONFIGURE_KEY_LABEL_X_OFFSET, CONFIGURE_KEY_Y_OFFSET + CONFIGURE_KEY_LABEL_Y_OFFSET);
                fontlib_DrawString(EN_PRESS_A_KEY);
            }
            break;
        case LANGUAGE:
        default:
            // nothing
            ;
    }
}

const char* PAUSE_MENU_OPTION_NAMES[PAUSE_MENU_OPTIONS_N] = {
    "CONTINUE", "MENU"
}; // should be the same in french? maybe?

#define PAUSE_MENU_X_PADDING 8
#define PAUSE_MENU_Y_PADDING 4
#define PAUSE_MENU_WIDTH     (MENU_BUTTON_WIDTH + 2 * PAUSE_MENU_X_PADDING)
#define PAUSE_MENU_HEIGHT    (MENU_BUTTON_HEIGHT * (PAUSE_MENU_OPTIONS_N + 1) + 2 * PAUSE_MENU_Y_PADDING)

// center the pause menu on-screen
#define PAUSE_MENU_X_OFFSET ((LCD_WIDTH - PAUSE_MENU_WIDTH) / 2)
#define PAUSE_MENU_Y_OFFSET ((LCD_HEIGHT - PAUSE_MENU_HEIGHT) / 2)

void draw_pause_menu() {
    draw_game();
    gfx_SetColor(BLACK);
    gfx_FillRectangle_NoClip(PAUSE_MENU_X_OFFSET, PAUSE_MENU_Y_OFFSET, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
    gfx_SetColor(WHITE);
    gfx_Rectangle_NoClip(PAUSE_MENU_X_OFFSET, PAUSE_MENU_Y_OFFSET, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
    fontlib_SetCursorPosition(PAUSE_MENU_X_OFFSET + PAUSE_MENU_X_PADDING, PAUSE_MENU_Y_OFFSET + PAUSE_MENU_Y_PADDING);
    fontlib_DrawString("PAUSED");

    for (int c = 0; c < PAUSE_MENU_OPTIONS_N; c++) {
        if (c == pause_menu_option) {
            fontlib_SetForegroundColor(check_key(kb_KeyEnter) & KEY_HELD ? WHITE : YELLOW);
        }
        int draw_x = PAUSE_MENU_X_OFFSET + PAUSE_MENU_X_PADDING;
        int draw_y = PAUSE_MENU_Y_OFFSET + PAUSE_MENU_Y_PADDING + (c + 1) * MENU_BUTTON_HEIGHT;
        
        gfx_TransparentSprite_NoClip(BUTTON_SPRITE, draw_x, draw_y);
        fontlib_SetCursorPosition(draw_x + MENU_LABEL_X_OFFSET, draw_y + MENU_LABEL_Y_OFFSET);
        fontlib_DrawString(PAUSE_MENU_OPTION_NAMES[c]);
        fontlib_SetForegroundColor(WHITE);
    }
}

// copying what Epsilon5 did in Hailstorm CE for the frame counter
// why it works, i'm not sure
// cargo cult programmming at its finest

void draw_game() {
    gfx_FillScreen(BLACK);

    gfx_SetColor(WHITE);

    gfx_Line(0, 20, LCD_WIDTH, 20);
    gfx_Line(0, LCD_HEIGHT - 20, LCD_WIDTH, LCD_HEIGHT - 20);

    // thanks, commandblockguy, LogicalJoe!
    fontlib_SetCursorPosition(8, LCD_HEIGHT - 16);
    fontlib_DrawString("SCORE ");
    fontlib_DrawUInt(score, 3);
    fontlib_DrawString(use_french ? "  MEILLEUR " : "  HIGHSCORE ");
    fontlib_DrawUInt(high_score, 3);

    fontlib_SetCursorPosition(2, 2);
    fontlib_DrawString("FPS ");
    fontlib_DrawInt(32768 / timer_1_Counter, 3);

    timer_1_Counter = 0;
    
    // draw the background
    gfx_SetColor(DUSTY_BROWN);
    for (int c = 0; c < MAX_LANES; c++) {
        gfx_FillRectangle(0, LANE_Y_OFFSET + LANE_TOP_MARGIN + LANE_HEIGHT * c, LCD_WIDTH, 38);
    }

    draw_rider();

    // draw the envelopes
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        struct Envelope* e = &(envelopes[c]);
        if (!e->used) continue;
        int draw_y = e->lane * LANE_HEIGHT + LANE_Y_OFFSET + 8;
        if (e->phase < 3) {
            draw_y--;
        }
        if (e->phase >= 6 && e->phase < 9) {
            draw_y++;
        }
        gfx_Sprite(envelope_sprite, e->x, draw_y);
    }

    for (int c = 0; c < MAX_OBSTACLES; c++) {
        struct Obstacle* o = &(obstacles[c]);
        if (!o->used) continue;
        switch (o->type) {
            case BOULDER:
                ;
                struct Boulder_data* b = &(o->data.boulder);

                int draw_y = b->lane * LANE_HEIGHT + LANE_Y_OFFSET + 5;
                gfx_TransparentSprite(boulder_sprite, b->x, draw_y);
                break;
        }
    }

    if (start_game_countdown) {
        gfx_SetColor(BLACK);
        
        int mask_height = start_game_countdown * START_GAME_COUNTDOWN_MASK;

        if (mask_height <= 0) {
            return;
        }

        gfx_FillRectangle(0, 0, LCD_WIDTH, mask_height);
        gfx_FillRectangle(0, (LCD_HEIGHT - mask_height), LCD_WIDTH, mask_height);
    }
}