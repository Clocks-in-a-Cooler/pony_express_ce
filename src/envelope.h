#include <stdbool.h>

#define ENVELOPE_WIDTH 36
#define ENVELOPE_HEIGHT 24

struct Envelope {
    int x;
    int lane;
    bool used;
};

#define MAX_ENVELOPES 32
extern struct Envelope envelopes[MAX_ENVELOPES];

void add_envelope(int lane);
void update_envelope(struct Envelope* envelope);
void clear_envelopes();

#define ENVELOPE_FREQUENCY 25