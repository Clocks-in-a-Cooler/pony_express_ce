#include "graphics.h"
#include "pony_express.h"
#include "envelope.h"
#include "gfx/sprites.h"
#include "font/font.h"

#define WHITE 9
#define BLACK 1

void initialize_graphics() {
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

char* MENU_OPTION_NAMES[MENU_OPTIONS_N] = {
    "PLAY", "EXIT"
};

void draw_menu() {
    // no fancy stuff for now, because TheAyeStride needs a break...
    // don't want to overwhelm them with more unfinished projects
    gfx_FillScreen(BLACK);
    fontlib_SetCursorPosition(8, 8);
    fontlib_DrawString("PONY EXPRESS CE");

    for (int c = 0; c < MENU_OPTIONS_N; c++) {
        uint8_t colour;
        if (c == menu_option) {
            colour = (check_key(kb_KeyEnter) & KEY_HELD) ? WHITE : 0xf0;
        } else {
            colour = 0xe0;
        }
        gfx_SetColor(colour);
        gfx_FillRectangle(8, 160 + c * 16, 80, 14);
        fontlib_SetCursorPosition(10, 160 + c * 16 + 3);
        fontlib_DrawString(MENU_OPTION_NAMES[c]);
    }
}

char* PAUSE_MENU_OPTION_NAMES[PAUSE_MENU_OPTIONS_N] = {
    "CONTINUE", "MENU"
};

void draw_pause_menu() {
    draw_game();
    gfx_SetColor(BLACK);
    gfx_FillRectangle(70, 90, 180, 60);
    gfx_SetColor(WHITE);
    gfx_Rectangle(70, 90, 180, 60);
    fontlib_SetCursorPosition(78, 98);
    fontlib_DrawString("PAUSED");

    for (int c = 0; c < PAUSE_MENU_OPTIONS_N; c++) {
        uint8_t colour;
        if (c == pause_menu_option) {
            colour = check_key_held(kb_KeyEnter) ? WHITE : 0xf0;
        } else {
            colour = 0xe0;
        }
        gfx_SetColor(colour);
        gfx_FillRectangle(78, 114 + c * 16, 164, 14);
        fontlib_SetCursorPosition(80, 114 + c * 16 + 3);
        fontlib_DrawString(PAUSE_MENU_OPTION_NAMES[c]);
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