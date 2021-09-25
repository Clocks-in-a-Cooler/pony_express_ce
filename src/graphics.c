#include "graphics.h"
#include "pony_express.h"
#include "envelope.h"
#include "gfx/sprites.h"

#define WHITE 9
#define BLACK 1

void initialize_graphics() {
    gfx_Begin();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    gfx_SetTextFGColor(WHITE);
    gfx_SetTextBGColor(BLACK);
    gfx_SetTextTransparentColor(BLACK);
    gfx_SetMonospaceFont(8);
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
    gfx_PrintStringXY("PONY EXPRESS CE", 8, 8);

    for (int c = 0; c < MENU_OPTIONS_N; c++) {
        uint8_t colour;
        if (c == menu_option) {
            colour = (check_key(kb_KeyEnter) & KEY_HELD) ? WHITE : 0xf0;
        } else {
            colour = 0xe0;
        }
        gfx_SetColor(colour);
        gfx_FillRectangle(8, 160 + c * 16, 80, 14);
        gfx_PrintStringXY(MENU_OPTION_NAMES[c], 10, 160 + c * 16 + 3);
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
    gfx_PrintStringXY("PAUSED", 78, 98);

    for (int c = 0; c < PAUSE_MENU_OPTIONS_N; c++) {
        uint8_t colour;
        if (c == pause_menu_option) {
            colour = check_key_held(kb_KeyEnter) ? WHITE : 0xf0;
        } else {
            colour = 0xe0;
        }
        gfx_SetColor(colour);
        gfx_FillRectangle(78, 114 + c * 16, 164, 14);
        gfx_PrintStringXY(PAUSE_MENU_OPTION_NAMES[c], 80, 114 + c * 16 + 3);
    }
}

#define envelope_sprite ((gfx_sprite_t*) envelope_data)

void draw_game() {
    gfx_FillScreen(BLACK);

    gfx_SetColor(WHITE);

    gfx_Line(0, 20, LCD_WIDTH, 20);
    gfx_Line(0, LCD_HEIGHT - 20, LCD_WIDTH, LCD_HEIGHT - 20);

    // thanks, commandblockguy, LogicalJoe!
    gfx_PrintStringXY("score ", 8, LCD_HEIGHT - 16);
    gfx_PrintUInt(score, 3);
    
    // draw the background
    gfx_SetColor(0xab);
    for (int c = 0; c < MAX_LANES; c++) {
        gfx_FillRectangle(0, 21 + 40 * c, LCD_WIDTH, 38);
    }

    // draw the player
    gfx_SetColor(0x6a);
    gfx_FillRectangle(PLAYER_X, ((int) 22 + 40 * lane), PLAYER_SIZE, PLAYER_SIZE);

    // draw the envelopes
    gfx_SetColor(WHITE);
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        struct Envelope* e = &(envelopes[c]);
        if (!e->used) continue;
        int draw_y = e->lane * 40 + 28;
        // gfx_FillRectangle(e->x, draw_y, ENVELOPE_WIDTH, ENVELOPE_HEIGHT);
        gfx_Sprite(envelope_sprite, e->x, draw_y);
    }
}