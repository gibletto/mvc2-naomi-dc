/* The helpers that move a work's position, speed, scale and fade. */
#include "player.h"

/****************************************************************/
/*			A work's scale, grown the same on each axis			*/
/****************************************************************/
void scale_add( PLAYER_WORK *ew, Float v )
{
	ew->scale.x += v ;
	ew->scale.y += v ;
	ew->scale.z += v ;
}

/****************************************************************/
/*			A work's speed, with its acceleration added			*/
/****************************************************************/
void spd_add( PLAYER_WORK *ew )
{
	ew->spd.x += ew->acc.x ;
	ew->spd.y += ew->acc.y ;
	ew->spd.z += ew->acc.z ;
}

/****************************************************************/
/*			A work's position, with its speed added				*/
/****************************************************************/
void pos_add( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	ew->pos.z += ew->spd.z ;
}

/****************************************************************/
void move_pos_spd( PLAYER_WORK *ew )
{
	pos_add( ew ) ;
	spd_add( ew ) ;
}

/****************************************************************/
/*			A work's fade colour, brightened on each channel	*/
/****************************************************************/
void fade_col_add( PLAYER_WORK *ew, Float v )
{
	ew->fade.r += v ;
	ew->fade.g += v ;
	ew->fade.b += v ;
}

/****************************************************************/
void fade_trnsl_add( PLAYER_WORK *ew, Float v )
{
	ew->fade.trnsl += v ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "move_sub.c" End of File	*/
