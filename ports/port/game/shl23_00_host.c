/*
 * shl23_00_host.c  shl23_00.c for a host build
 *
 * player.h declares shl23_00_set without its parameters, since its callers pass two of the three;
 * gcc takes the definition's Uint8 parameters as a conflict with that, so the declaration is kept
 * under another name here.
 */

#define shl23_00_set shl23_00_set_unprototyped
#include "player.h"
#undef shl23_00_set
#include "shl23_00.c"
