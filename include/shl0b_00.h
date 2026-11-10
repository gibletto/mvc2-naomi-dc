/* shield_req as shl0b_00.c defines it: the state comes back widened from its byte. */
#ifndef SHL0B_00_H
#define SHL0B_00_H

extern int shield_req( struct PL0B_LOCAL_W *lw, Uint8 a, Uint8 b );	/* sets the shield's state, if it is free */

#endif
