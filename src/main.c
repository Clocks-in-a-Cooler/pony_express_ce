// for reference:
// https://www.google.com/doodles/155th-anniversary-of-the-pony-express

#include "graphics.h"
#include "pony_express.h"

int main() {
    running = true;
    initialize_graphics();
    init_keypad();
    init_pony_express();

    while (running) {
        update_keypad();
        gfx_SetDrawBuffer();
        switch (game_state) {
            case GAME_OVER: // once i have the game over thing decided upon, i'll give this one its own blocks
            case MENU:
                update_menu();
                draw_menu();
                break;
            case SETTINGS_MENU:
                update_settings();
                draw_settings();
                break;
            case CONFIGURE_KEY:
                update_configure();
                draw_configure();
                break;
            case PLAYING:
                update_game();
                draw_game();
                break;
            case PAUSED:
                update_pause_menu();
                draw_pause_menu();
                break;
            // others to be added
        }
        frames++;

        gfx_SwapDraw();
    }

    cleanup_pony_express();
    cleanup_graphics();
}