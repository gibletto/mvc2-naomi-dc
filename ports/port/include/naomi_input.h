/*
 * naomi_input.h  The controls, as the front end hands them to the board
 */

#ifndef NAOMI_INPUT_H
#define NAOMI_INPUT_H

#include <stdint.h>

/* a player's JVS switches: the first byte's bits are the high byte here */
enum {
    JVS_START = 0x8000, JVS_SERVICE = 0x4000, JVS_UP = 0x2000, JVS_DOWN = 0x1000,
    JVS_LEFT = 0x0800, JVS_RIGHT = 0x0400, JVS_B1 = 0x0200, JVS_B2 = 0x0100,
    JVS_B3 = 0x0080, JVS_B4 = 0x0040, JVS_B5 = 0x0020, JVS_B6 = 0x0010,
};

typedef struct {
    uint32_t player[2];     /* JVS_* bits */
    int coin;               /* a coin went in this frame */
    int test;               /* the test button */
} HOST_INPUT;

const HOST_INPUT* host_input(void);     /* this frame's controls (the front end) */
void nla_frame_input(void);             /* builds the I/O board's report (nl_am.c) */

#endif
