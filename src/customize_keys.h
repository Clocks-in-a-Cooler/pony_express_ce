#include <keypadc.h>
#include <tice.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SEEN     1
#define KEY_PRESSED  2
#define KEY_HELD     3
#define KEY_RELEASED 4

extern bool enter_key_latch;

void init_keypad();
void update_keypad();

short check_key(kb_lkey_t kb_lcode);

#define check_key_pressed(kb_l_code) check_key(kb_l_code) & KEY_PRESSED
#define check_key_held(kb_l_code) check_key(kb_l_code) & KEY_HELD
#define check_key_released(kb_l_code) check_key(kb_l_code) & KEY_RELEASED

// assumes that update_keypad() is already called. you have been warned!
kb_lkey_t get_next_key();

// helpers

kb_lkey_t sk_key_t_to_kb_lkey_t(sk_key_t sk_code);
sk_key_t kb_lkey_t_to_sk_key_t(kb_lkey_t kb_lcode);

extern char** KEY_NAMES;

char UP_ARROW[]    = { 0x18, '\0' };
char DOWN_ARROW[]  = { 0x19, '\0' };
char LEFT_ARROW[]  = { 0x1a, '\0' };
char RIGHT_ARROW[] = { 0x1b, '\0' };

#define KEY_NAME_LENGTH 9
char* EN_KEY_NAMES[] = {
    "",
    // DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, UP_ARROW, "", "", "", "",
    "down",  "left",  "right",  "up",    "",      "",      "",      "",
    "enter", "+",     "-",      "*",     "/",     "^",     "clear", "",
    "(-)",   "3",     "6",      "9",     ")",     "tan",   "vars",  "",
    ".",     "2",     "5",      "8",     "(",     "cos",   "prgm",  "stat",
    "0",     "1",     "4",      "7",     ",",     "sin",   "apps",  "XT0n",
    "",      "sto",   "ln",     "log",   "x^2",   "x^-1",  "math",  "alpha",
    "graph", "table", "zoom",   "window","y=",    "2nd",   "mode",  "del"
};

char FR_RESOL[]  = { 'r', 0xe9, 's', 'o', 'l', '\0' }; // "résol"
char FR_WINDOW[] = { 'f', 'e', 'n', 0xea, 't', 'r', 'e', '\0' }; // "fenêtre"

char* FR_KEY_NAMES[] = {
    "",
    // DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, UP_ARROW, "", "", "", "",
    "bas",    "gauche",  "droit", "haut",    "",     "",       "",        "",
    "entrer", "+",       "-",     "*",       "/",    "^",      "annul",   "",
    "(-)",    "3",       "6",     "9",       ")",    "n/d",    "var",     "",
    ".",      "2",       "5",     "8",       "(",    FR_RESOL, "prgm",    "stats",
    "0",      "1",       "4",     "7",       ",",    "trig",   "matrice", "XT0n",
    "",       "sto",     "ln",    "log",     "x^2",  "< >",    "math",    "alpha",
    "graphe", "trace",   "zoom",  FR_WINDOW, "f(x)", "2nd",    "mode",    "del"
};

#define get_key_name(kb_l_code) KEY_NAMES[kb_lkey_t_to_sk_key_t(kb_l_code)]