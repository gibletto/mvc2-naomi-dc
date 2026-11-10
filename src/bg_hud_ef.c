/* The mains of the two numbers bg_hud.c sets: works of kind 11 that pick their object by the value
   each frame. */
#include "player.h"

extern VEC3 hud_unit_vec ;
/* the colours a level's number blinks between (the first two); the rest are read elsewhere */
static FADE_COL hud_fade_tbl[] = {
	{ 1.0f, 0.0f, 1.0f, 0.875f },
	{ 1.0f, 0.25f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.5f, 0.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 0.0f },
	{ 1.0f, DOT_X( 0.375f ), 0.0f, 1.0f },
	{ 1.0f, 0.25f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.875f },
	{ 1.0f, 1.0f, 0.0f, 0.0f },
} ;

/****************************************************************/
void bg_clock_main( PLAYER_WORK *ew )
{
	int n = vs_sys_work.round_time ;

	if( ew->type == 0 ) n /= 10 ;
	else n %= 10 ;
	n += 85 ;
	ew->obj = obj_dm00->obj[n] ;
}

/****************************************************************/
/*			The number grows and its colour runs through the	*/
/*			three phases at the top level, and blinks			*/
/*			between two colours below it						*/
/****************************************************************/
void bg_lv_main( PLAYER_WORK *ew )
{
	int lv = vs_sys_work.gauge_level[ew->type] ;
	Float s ;
	Float lo, d ;
	FADE_COL *tbl ;

	if( lv >= 5 ) lv = 5 ;
	if( lv >= 5 ){
		ew->scale.x = ew->scale.y = nlSin( bg_ang2 ) / 16.0f + 1.25f ;
	} else {
		ew->scale = hud_unit_vec ;
	}
	if( lv >= 5 ){
		ew->fade.r = nlSin( bg_ang2 + ( int )ew->acc.x ) / 2.0f + 0.5f ;
		ew->fade.g = nlSin( bg_ang2 + ( int )ew->acc.y ) / 2.0f + 0.5f ;
		ew->fade.b = nlSin( bg_ang2 + ( int )ew->acc.z ) / 2.0f + 0.5f ;
	} else {
		s = nlSin( bg_ang0 ) / 2.0f + 0.5f ;
		tbl = hud_fade_tbl ;
		lo = tbl[1].r ;
		d = tbl[0].r - lo ;
		ew->fade.r = lo + d * s ;
		lo = tbl[1].g ;
		d = tbl[0].g - lo ;
		ew->fade.g = lo + d * s ;
		lo = tbl[1].b ;
		d = tbl[0].b - lo ;
		ew->fade.b = lo + d * s ;
	}
	if( lv >= 5 ) lv = 9 ;
	ew->obj = obj_dm00->obj[96 + lv] ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "bg_hud_ef.c" End of File	*/
