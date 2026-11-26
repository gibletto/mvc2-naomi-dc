/*
 * hunt.h  A scripted run for comparing moves with an emulator (hunt.c)
 */

#ifndef HUNT_H
#define HUNT_H

#include "naomi_input.h"

int hunt_init(int no_log);     /* no_log: no line as the script moves on */
int hunt_frame(HOST_INPUT* in, unsigned frame);

#endif
