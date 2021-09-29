#include "graphics.h"
#include "pony_express.h"
#include "envelope.h"
#include "gfx/sprites.h"
#include "font/font.h"

#define WHITE 1
#define BLACK 2
#define YELLOW 3

bool use_french = false;

void initialize_graphics() {

    use_french = (os_GetSystemInfo())->hardwareType;

    gfx_Begin();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

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
    "PLAY", "EXIT"
};

// désolée, mon français est très mal...
char* FR_MENU_OPTION_NAMES[MENU_OPTIONS_N] = {
    "JOUER", "SORTIR"
};


const gfx_sprite_t* BUTTON_SPRITE = (gfx_sprite_t*) button_data;

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

#define envelope_sprite ((gfx_sprite_t*) envelope_data)

unsigned char* rider_sprites[5] = {
    rider1_data,
    rider2_data,
    rider4_data,
    rider6_data,
    rider10_data,
};

void draw_game() {
    gfx_FillScreen(BLACK);

    gfx_SetColor(WHITE);

    gfx_Line(0, 20, LCD_WIDTH, 20);
    gfx_Line(0, LCD_HEIGHT - 20, LCD_WIDTH, LCD_HEIGHT - 20);

    // thanks, commandblockguy, LogicalJoe!
    fontlib_SetCursorPosition(8, LCD_HEIGHT - 16);
    fontlib_DrawString("SCORE ");
    fontlib_DrawUInt(score, 3);
    fontlib_DrawString(use_french ? "  HAUT-SCORE " : "  HIGH-SCORE ");
    fontlib_DrawUInt(high_score, 3);
    
    // draw the background
    gfx_SetColor(0xe6);
    for (int c = 0; c < MAX_LANES; c++) {
        gfx_FillRectangle(0, 21 + 40 * c, LCD_WIDTH, 38);
    }

    // draw the player
    gfx_SetTransparentColor(0);
    int draw_y = (int) 22 + 40 * lane;

    gfx_sprite_t* rider_sprite;
    switch (pose) {
        case 1:
        case 2:
            rider_sprite = (gfx_sprite_t*) rider_sprites[1];
            break;
        case 3:
        case 4:
        case 8:
            rider_sprite = (gfx_sprite_t*) rider_sprites[2];
            draw_y--;
            break;
        case 5:
        case 6:
        case 7:
            rider_sprite = (gfx_sprite_t*) rider_sprites[3];
            draw_y -= 2;
            break;
        default:
            rider_sprite = (gfx_sprite_t*) rider_sprites[0];
    }
    gfx_TransparentSprite(rider_sprite, PLAYER_X, draw_y);

    // draw the envelopes
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        struct Envelope* e = &(envelopes[c]);
        if (!e->used) continue;
        int draw_y = e->lane * 40 + 28;
        if (e->phase < 3) {
            draw_y--;
        }
        if (e->phase >= 6 && e->phase < 9) {
            draw_y++;
        }
        gfx_Sprite(envelope_sprite, e->x, draw_y);
    }
}