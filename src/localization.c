#include "localization.h"

// make sure not to screw any of these up... could easy end up with a segfault

// the Localization_keys enum serves as the index to these arrays here

// note: USE CAPITAL LETTERS ONLY
//       THE FONT ONLY HAS UPPERCASE GLYPHS
// note: UTILISEZ SEULEMENT DES LETTRES MAJUSCULES

char* ENGLISH_LOCALIZATIONS[] = {
    "PLAY",
    "SETTINGS",
    "EXIT",

    "SETTINGS",
    "MOVE UP",
    "MOVE DOWN",
    "LANGUAGE",
    "BACK",

    "ENGLISH",
    "PRESS A KEY",
    "SELECT LANGUAGE",

    "PAUSED",
    "CONTINUE",
    "MENU",

    "SCORE",
    "HIGHSCORE",
};

/*
 * codes for various characters
 * OTHER SPECIAL CHARACTERS NOT INCLUDED HERE WILL NOT DISPLAY PROPERLY
 * 
 * codes pour les lettres avec des accents
 * DES AUTRES CARACTÈRES QUI NE SONT PAS INCLUS ICI NE VONT PAS S'AFFICHER CORRECTEMENT
 * 
 * up arrow     0x10
 * down arrow   0x11
 * left arrow   0x12
 * right arrow  0x13
 * "À"          0xc0
 * "Á"          0xc1
 * "Â"          0xc2
 * "Ã"          0xc3
 * "Ä"          0xc4
 * "Ç"          0xc7
 * "È"          0xc8
 * "É"          0xc9
 * "Ê"          0xca
 * "Ë"          0xcb
 * "Î"          0xce
 * "Ï"          0xcf
 * "Ô"          0xd4
 * "Ö"          0xd6
 */


/*
 * notice a problem with the translations?
 * contact me on Cemetech (Candledark) or on Github (Clocks-in-a-Cooler)
 * thanks!
 * 
 * remarquez-vous un problème avec les traductions ?
 * contactez-moi sur Cemetech ( Candledark ) ou sur Github ( Clocks-in-a-Cooler )
 * merci !
 */

char SETTINGS_FRENCH[]      = { 'R', 0xc9, 'G', 'L', 'A', 'G', 'E', 'S', '\0' }; // "RÉGLAGES"
char LANGUAGE_NAME_FRENCH[] = { 'F', 'R', 'A', 'N', 0xc7, 'A', 'I', 'S', '\0' }; // "FRANÇAIS"
char BACK_TO_MENU_FRENCH[]  = { 'R', 'E', 'T', 'O', 'U', 'R', 'N', 'E', 'R', ' ', 0xc0, ' ', 'M', 0xc9, 'N', 'U', '\0' }; // "RETOURNER À MÉNU"

char* FRENCH_LOCALIZATIONS[] = {
    "JOUER",
    SETTINGS_FRENCH,
    "SORTIR",

    SETTINGS_FRENCH,
    "MONTER",
    "DESCENDRE",
    "LANGUE",
    "RETOURNER",

    LANGUAGE_NAME_FRENCH,
    "APPUYEZ UNE TOUCHE",
    "CHOISISSEZ UNE LANGUE",

    "PAUSE",
    "CONTINUER",
    "MÉNU",

    "SCORE",
    "MEILLEUR",
};

char** CANADIAN_LOCALIZATIONS = {
    "PLAY",
    SETTINGS_FRENCH,
    "EXIT",

    "SETTINGS",
    "MONTER",
    "MOVE DOWN",
    "LANGUE",
    "BACK",

    "CANADIANNE",
    "APPUYEZ A KEY",
    "CHOOSE UNE LANGUE",

    "PAUSE",
    "CONTINUE",
    "MENU",

    "SCORE",
    "BEST",
};

enum Localization localization;

void set_localization(enum Localization new_localization) {
    localization = new_localization;
}

enum Localization get_localization() {
    return localization;
}

const char* get_localized_string(enum Localization_keys key) {
    switch (localization) {
        case ENGLISH:
            return ENGLISH_LOCALIZATIONS[key];
        case FRENCH:
            return FRENCH_LOCALIZATIONS[key];
        case CANADIAN:
            return CANADIAN_LOCALIZATIONS[key];
        default:
            return ENGLISH_LOCALIZATIONS[key];
    }
}