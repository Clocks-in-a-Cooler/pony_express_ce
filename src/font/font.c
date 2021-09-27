#include <fontlibc.h>

const uint8_t oldie_font_data[] = {
    #include "oldie_font.inc"
};

const fontlib_font_t* oldie_font = (fontlib_font_t*) oldie_font_data;