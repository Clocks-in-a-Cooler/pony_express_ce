// for reference:
// https://www.google.com/doodles/155th-anniversary-of-the-pony-express

#include "graphics.h"
#include "pony_express.h"

int main() {
    running = true;
    initialize_graphics();
    init_keypad();

    while (running) {
        update_keypad();
        gfx_SetDrawBuffer();
        switch (game_state) {
            case MENU:
                update_menu();
                draw_menu();
                break;
            case PLAYING:
                update_game();
                draw_game();
                break;
            // others to be added
        }

        gfx_SwapDraw();
    }

    cleanup_graphics();
}