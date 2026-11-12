/*
 * nl_fog.c  The NAOMI library's fog (nlfog.c): the table, its colour and the density
 */

#include "nl_port.h"

void nlFogTableSet(const float* table) {
    rd_fog_table(table);
}

void nlFogTableOff(void) {
    static const float off[128];
    rd_fog_table(off);
}

void nlFogTableColor(uint32_t argb) {
    rd_fog_color(argb);
}

void nlFogDensity(uint32_t density) {
    rd_fog_density(density);
}

/* the float's top eight mantissa bits over its exponent */
int nlFogDensityF(float fdensity) {
    union {
        float f;
        int32_t i;
    } u;
    int32_t exp, mant, density;
    u.f = fdensity;
    exp = (u.i >> 23) - 127;
    mant = (uint8_t)((int16_t)(u.i >> 16) | 0x80);
    if (exp < 0) {
        mant >>= -exp;
        exp = 0;
    }
    density = mant << 8 | (exp & 0xff);
    rd_fog_density((uint32_t)density);
    return density;
}
