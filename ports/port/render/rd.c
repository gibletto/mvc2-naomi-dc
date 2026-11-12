/*
 * rd.c  The frame's triangles, gathered by list for a back end
 */

#include <math.h>
#include <string.h>

#include "rd.h"
#include "rd_list.h"

uint32_t rd_palette[1024];
uint32_t rd_palette_serial;

RD_FRAME rd_frame;
RD_QUALITY rd_q;

void rd_quality(const char* words) {
    rd_q.half = words != NULL && strstr(words, "half") != NULL;
    rd_q.fast = words != NULL && strstr(words, "fast") != NULL;
    rd_q.nofog = words != NULL && strstr(words, "nofog") != NULL;
    rd_q.nosort = words != NULL && strstr(words, "nosort") != NULL;
}

void rd_begin_frame(void) {
    int i;
    for (i = 0; i < RD_LISTS; i++) {
        rd_frame.ntris[i] = 0;
    }
    rd_frame.nstates = 0;
}

RD_FOG rd_fog;

void rd_background_plane(const RD_VTX* v) {
    memcpy(rd_frame.background, v, sizeof rd_frame.background);
}

void rd_background(uint32_t argb) {
    RD_VTX v[3];
    memset(v, 0, sizeof v);
    v[1].x = RD_WIDTH;
    v[2].y = RD_HEIGHT;
    v[0].base = v[1].base = v[2].base = argb;
    rd_background_plane(v);
}

/* the fog registers keep their values from frame to frame */
void rd_fog_table(const float* table) {
    int i;
    for (i = 0; i < 128; i++) {
        float f = table[i];
        rd_fog.table[i] = (float)(int)((f < 0.0f ? 0.0f : f > 1.0f ? 1.0f : f) * 255.0f + 0.5f) / 255.0f;
    }
}

void rd_fog_color(uint32_t argb) {
    rd_fog.color = argb;
}

void rd_fog_density(uint32_t reg) {
    rd_fog.density = (float)(reg >> 8 & 255) / 128.0f * ldexpf(1.0f, (int8_t)(reg & 255));
}

/* the frame's copy of s: the last one again when it is the same */
static int add_state(const RD_STATE* s) {
    if (rd_frame.nstates > 0 && memcmp(&rd_frame.states[rd_frame.nstates - 1], s, sizeof *s) == 0) {
        return rd_frame.nstates - 1;
    }
    if (rd_frame.nstates == RD_MAX_STATES) {
        return -1;
    }
    rd_frame.states[rd_frame.nstates] = *s;
    return rd_frame.nstates++;
}

static void add_tri(int list, int state, const RD_VTX* a, const RD_VTX* b, const RD_VTX* c) {
    RD_TRI* t;
    if (rd_frame.ntris[list] == RD_MAX_TRIS) {
        return;
    }
    t = &rd_frame.tris[list][rd_frame.ntris[list]++];
    t->state = (uint16_t)state;
    t->v[0] = *a;
    t->v[1] = *b;
    t->v[2] = *c;
}

void rd_strip(const RD_STATE* s, const RD_VTX* v, int n) {
    int st = add_state(s), i;
    if (st < 0) {
        return;
    }
    for (i = 2; i < n; i++) {
        if (i & 1) {
            add_tri(s->list, st, &v[i - 1], &v[i - 2], &v[i]);     /* one winding for the whole strip */
        } else {
            add_tri(s->list, st, &v[i - 2], &v[i - 1], &v[i]);
        }
    }
}

void rd_quad(const RD_STATE* s, const RD_VTX* v) {
    int st = add_state(s);
    if (st < 0) {
        return;
    }
    add_tri(s->list, st, &v[0], &v[1], &v[2]);
    add_tri(s->list, st, &v[0], &v[2], &v[3]);
}
