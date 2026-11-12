/*
 * rd_list.h  The frame's lists, as the back ends read them
 */

#ifndef RD_LIST_H
#define RD_LIST_H

#include "rd.h"

#define RD_LISTS 3
#ifdef __3DS__
#define RD_MAX_TRIS 8192    /* in its 64 MB (the game's attract mode sends at most about 4300 to one list) */
#else
#define RD_MAX_TRIS 32768
#endif
#define RD_MAX_STATES 8192

typedef struct {
    uint16_t state;
    RD_VTX v[3];
} RD_TRI;

typedef struct {
    RD_TRI tris[RD_LISTS][RD_MAX_TRIS];
    int ntris[RD_LISTS];
    RD_STATE states[RD_MAX_STATES];
    int nstates;
    RD_VTX background[3];   /* its corners: top left, top right, bottom left */
} RD_FRAME;

extern RD_FRAME rd_frame;

typedef struct {
    float table[128];       /* 0 to 1, in steps of 1/255 as the registers hold them */
    uint32_t color;
    float density;
} RD_FOG;

extern RD_FOG rd_fog;

/* the quality words rd_quality last took, each 1 when given; a back end does what it can of them */
typedef struct {
    uint8_t half, fast, nofog, nosort;
} RD_QUALITY;

extern RD_QUALITY rd_q;

#endif
