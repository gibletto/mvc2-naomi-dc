/*
 * at02_scene_host.c  at02_scene.c for a host build
 *
 * at02_scene.c declares obj_scale_max (disp.c's, a Float) as returning an int, and the arcade's code
 * reads it so, from r0. On x86 a float comes back on the x87 stack, and a caller that takes an int
 * leaves it there until the stack overflows into NaNs; so at02_scene.c calls a routine that does
 * return an int.
 */

#include "types.h"

#define obj_scale_max at02_obj_scale_max
#include "at02_scene.c"
#undef obj_scale_max

extern Float obj_scale_max( VEC3 *scale ) ;

int at02_obj_scale_max( VEC3 *scale )
{
	return ( int )obj_scale_max( scale ) ;
}
