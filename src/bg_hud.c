/* The two numbers bg_init puts on the fight's screen: the round's clock, two digits at the top, and
   under each side's vital gauge the level of its hyper combo gauge. Each is a work of kind 11 whose
   main picks its object by the value each frame. */
#include "player.h"

/* where the clock's tens and ones stand, and where each side's level does */
static VEC3 hud_clock_pos_tbl[] = {
	{ -5.0f, 104.0f, -160.0f },
	{ 5.0f, 104.0f, -160.0f },
} ;
static VEC3 hud_lv_pos_tbl[] = {
	{ -61.3f, -45.5f, -70.0f },
	{ 61.3f, -45.5f, -70.0f },
} ;
static VEC3 hud_zero_vec = { 0.0f, 0.0f, 0.0f } ;
VEC3 hud_unit_vec = { 1.0f, 1.0f, 1.0f } ;

static void bg_clock_set( int no );
static void bg_lv_set( int no );

/****************************************************************/
void bg_hud_set( void )
{
#ifdef TARGET_DC
	if( Sys->round_time_no != 3 )	/* a fight with no time limit has one mark for its clock */
#endif
	bg_clock_set( 0 ) ;
	bg_clock_set( 1 ) ;
	bg_lv_set( 0 ) ;
	bg_lv_set( 1 ) ;
}

extern void bg_clock_main( PLAYER_WORK *ew );
extern void bg_lv_main( PLAYER_WORK *ew );
#ifdef TARGET_DC
extern void bg_clock_inf_main( PLAYER_WORK *ew );
#endif

/*==============================================================*/
/*			One digit of the clock: 0 the tens, 1 the ones		*/
/*==============================================================*/
static void bg_clock_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = bg_clock_main ;
		ew->obj = obj_dm00->obj[85] ;
		ew->pos = hud_clock_pos_tbl[no] ;
		ew->obj_flag = 0x10801 ;
		ew->type = no ;
#ifdef TARGET_DC
		if( Sys->round_time_no == 3 ){
			ew->work_func = bg_clock_inf_main ;
			ew->obj = obj_dm00->obj[95] ;
		}
#endif
	}
}

/*==============================================================*/
/*			A side's gauge level, shown while the side is in	*/
/*			the fight											*/
/*==============================================================*/
static void bg_lv_set( int no )
{
	PLAYER_WORK *ew ;

	if( FLAG_CK( pl_work[no].pl_bit, 0x7000000, 0 ) ) return ;
	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = bg_lv_main ;
		ew->obj = obj_dm00->obj[96] ;
		ew->pos = hud_lv_pos_tbl[no] ;
		ew->obj_flag = 0x10c11 ;
		ew->type = no ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->scale = hud_unit_vec ;
		ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 21845.0f ;
		ew->acc.z = 43690.0f ;
	}
}

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

#ifdef TARGET_DC
/****************************************************************/
/*			The clock's mark when there is no time limit		*/
/****************************************************************/
void bg_clock_inf_main( PLAYER_WORK *ew )
{
}
#endif

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "bg_hud.c" End of File	*/
