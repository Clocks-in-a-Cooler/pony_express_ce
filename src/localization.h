#include <string.h>
#include <tice.h>

#ifndef _LOCALIZATION
#define _LOCALIZATION

enum Localization {
    ENGLISH = 1,
    FRENCH,
    CANADIAN,
};

enum Localization_keys {
    MENU_OPTION_PLAY = 0,
    MENU_OPTION_SETTINGS,
    MENU_OPTION_EXIT,

    SETTINGS_MENU_TITLE,
    SETTINGS_MENU_UP, // key for moving up
    SETTINGS_MENU_DOWN, // key for moving down
    SETTINGS_MENU_LANGUAGE,
    SETTINGS_MENU_EXIT,

    SETTINGS_LANGUAGE_NAME,
    SETTINGS_PRESS_KEY,
    SETTINGS_SELECT_LANGUAGE,

    PAUSE_MENU_TITLE,
    PAUSE_MENU_CONTINUE,
    PAUSE_MENU_EXIT,

    GAMEPLAY_SCORE,
    GAMEPLAY_HIGHSCORE,
};

#define MENU_OPTION_LOCALIZATION_OFFSET     0
#define SETTINGS_OPTION_LOCALIZATION_OFFSET SETTINGS_MENU_UP
#define PAUSE_OPTION_LOCALIZATION_OFFSET    PAUSE_MENU_CONTINUE

void set_localization(enum Localization new_localization);

enum Localization get_localization();

const char* get_localized_string(enum Localization_keys key);

#endif