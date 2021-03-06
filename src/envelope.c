#include "envelope.h"
#include "pony_express.h"

struct Envelope envelopes[MAX_ENVELOPES];

void add_envelope(int lane) {
    struct Envelope e = {
        .x     = LCD_WIDTH,
        .lane  = lane,
        .used  = true,
        .phase = rand() % ENVELOPE_MAX_PHASE,
    };

    // ever wonder why the counters are named "c" and not "i"?
    // well... i get to make the "c++" joke, that's why.
    // no, seriously
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        if (envelopes[c].used) continue;
        envelopes[c] = e;
        return;
    }
}

void update_envelope(struct Envelope* envelope) {
    if (envelope->used) {
        envelope->x    -= is_moving ? MOVE_SPEED : 0;
        if (!(frames % 4)) {
            envelope->phase = (envelope->phase + 1) % ENVELOPE_MAX_PHASE;
        }
    }
    envelope->used = envelope->x > -ENVELOPE_WIDTH && envelope->used;
}

void clear_envelopes() {
    for (int c = 0; c < MAX_ENVELOPES; c++) {
        envelopes[c].used = 0;
    }
}