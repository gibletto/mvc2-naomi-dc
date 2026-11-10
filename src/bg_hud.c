/* The setters of the two numbers bg_init puts on the fight's screen: the round's clock, two digits
   at the top, and under each side's vital gauge the level of its hyper combo gauge. Their mains are
   in bg_hud_ef.c. */
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
	bg_clock_set( 0 ) ;
	bg_clock_set( 1 ) ;
	bg_lv_set( 0 ) ;
	bg_lv_set( 1 ) ;
}

extern void bg_clock_main( PLAYER_WORK *ew );
extern void bg_lv_main( PLAYER_WORK *ew );

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

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "bg_hud.c" End of File	*/
