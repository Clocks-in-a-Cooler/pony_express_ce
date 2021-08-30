#include "graphics.h"
#include "pony_express.h"
#include "envelope.h"

void initialize_graphics() {
    gfx_Begin();

    gfx_SetTextFGColor(0xff);
    gfx_SetTextBGColor(0x00);
    gfx_SetTextTransparentColor(0x00);
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
    gfx_FillScreen(0x00);
    gfx_PrintStringXY("PONY EXPRESS CE", 8, 8);

    for (int c = 0; c < MENU_OPTIONS_N; c++) {
        uint8_t colour;
        if (c == menu_option) {
            colour = (check_key(kb_KeyEnter) & KEY_HELD) ? 0xff : 0xf0;
        } else {
            colour = 0xe0;
        }
        gfx_SetColor(colour);
        gfx_FillRectangle(8, 160 + c * 16, 80, 14);
        gfx_PrintStringXY(MENU_OPTION_NAMES[c], 10, 160 + c * 16 + 3);
    }
}

void draw_game() {
    gfx_FillScreen(0x00);

    gfx_SetColor(0xff);

    gfx_Line(0, 20, LCD_WIDTH, 20);
    gfx_Line(0, LCD_HEIGHT - 20, LCD_WIDTH, LCD_HEIGHT - 20);

    char score_counter[17];

    sprintf(score_counter, "score %i", score);

    gfx_PrintStringXY(score_counter, 8, LCD_HEIGHT - 16);
    
    // draw the background
    gfx_SetColor(0xab);
    for (int c = 0; c < MAX_LANES; c++) {
        gfx_FillRectangle(0, 21 + 40 * c, LCD_WIDTH, 38);
    }

    // draw the player
    gfx_SetColor(0x6a);
    gfx_FillRectangle(PLAYER_X, ((int) 22 + 40 * lane), PLAYER_SIZE, PLAYER_SIZE);

    // draw the envelopes
    gfx_SetColor(0xff);
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        struct Envelope* e = &(envelopes[c]);
        if (!e->used) continue;
        int draw_y = e->lane * 40 + 28;
        gfx_FillRectangle(e->x, draw_y, ENVELOPE_WIDTH, ENVELOPE_HEIGHT);
    }
}