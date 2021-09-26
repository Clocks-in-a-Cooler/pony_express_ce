#include <keypadc.h>
#include <tice.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SEEN     1
#define KEY_PRESSED  2
#define KEY_HELD     3
#define KEY_RELEASED 4

extern bool enter_key_released;

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

#define KEY_NAME_LENGTH 7
char* KEY_NAMES[] = {
    "",
    // { 25, 0 }, { 26, 0 }, { 27, 0 }, { 24, 0 }, "", "",  "","", // down, left, right, up, in that order
    "down",  "left",  "right",  "up",    "",      "",      "",      "",
    "enter", "+",     "-",      "*",     "/",     "^",     "clear", "",
    "(-)",   "3",     "6",      "9",     ")",     "tan",   "vars",  "",
    ".",     "2",     "5",      "8",     "(",     "cos",   "prgm",  "stat",
    "0",     "1",     "4",      "7",     ",",     "sin",   "apps",  "XT0n",
    "",      "sto",   "ln",     "log",   "x^2",   "x^-1",  "math",  "alpha",
    "graph", "table", "zoom",   "window","y=",    "2nd",   "mode",  "del"
};

#define get_key_name(kb_l_code) KEY_NAMES[kb_lkey_t_to_sk_key_t(kb_l_code)]