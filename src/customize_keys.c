#include "customize_keys.h"

#define KEYS_N 57
#define to_sk_key_t(group, offset) (group << 3 | offset)

bool enter_key_latch;

short keys[KEYS_N];

void init_keypad() {
    memset((void*) keys, 0, KEYS_N);
    enter_key_latch = kb_IsDown(kb_KeyEnter);
}

void update_keypad() {
    kb_Scan();
    for (int c = 1; c < KEYS_N; c++) {
        short* current_key = &(keys[c]);
        if (kb_IsDown(sk_key_t_to_kb_lkey_t(c))) {
            if (*current_key & KEY_HELD) {
                *current_key = *current_key & KEY_SEEN;
            } else {
                *current_key = KEY_PRESSED | KEY_SEEN;
            }
        } else {
            if (*current_key & KEY_HELD) {
                *current_key = KEY_RELEASED;
            } else {
                *current_key = 0;
            }
        }
    }
}

kb_lkey_t get_next_key() {
    for (int c = 1; c < KEYS_N; c++) {
        if (keys[c] & KEY_PRESSED) return sk_key_t_to_kb_lkey_t(c);
    }
    return 0;
}

short check_key(kb_lkey_t kb_lcode) {
    return keys[kb_lkey_t_to_sk_key_t(kb_lcode)];
}

kb_lkey_t sk_key_t_to_kb_lkey_t(sk_key_t sk_code) {
    sk_code--;
    uint8_t group  = 7 - (sk_code >> 3);
    uint8_t offset = sk_code & 7;

    return (kb_lkey_t)(group << 8 | 1 << offset);
}

sk_key_t kb_lkey_t_to_sk_key_t(kb_lkey_t kb_lcode) {
    if (!kb_lcode) return 0;

    uint8_t group      = (7 - (kb_lcode >> 8)) << 3;
    uint8_t offset_bit = (uint8_t) kb_lcode & 255;
    uint8_t offset     = 0;

    // find the offset from the offset bit
    while (!(offset_bit & 1)) {
        offset++;
        // thanks, commandblockguy, for catching this one... my sleepy fingers did it correctly in the Javascript, but not here
        offset_bit = offset_bit >> 1;
        if (offset > 8) return 0; // failsafe
    }

    return ((group | offset) + 1);
}