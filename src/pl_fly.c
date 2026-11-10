/* Flight, for the characters that can fly (fly_flag): steering by the stick, and the take-off
   each flier does when the flight starts. */
#include "player.h"
#include "pl_char.h"

extern Uint8 pl_nml_atck_ck( PLAYER_WORK *pl_w );	/* s_pl14 declares it as an int */

static void fly_end( PLAYER_WORK *pl_w );
static void fly_dir_set( PLAYER_WORK *pl_w, Uint8 dir );
static void fly_stop_set( PLAYER_WORK *pl_w, Uint8 dir );
static void fly_y_ck( PLAYER_WORK *pl_w );
static void fly_start( PLAYER_WORK *pl_w );
static void fly_start_init( PLAYER_WORK *pl_w );
static void fly_start_move( PLAYER_WORK *pl_w );
static void fly_start_fall( PLAYER_WORK *pl_w );
static void fly_start2_init( PLAYER_WORK *pl_w );
static void fly_start2_move( PLAYER_WORK *pl_w );
static void fly_start2_fall( PLAYER_WORK *pl_w );
static void fly_start2( PLAYER_WORK *pl_w );
static void fly_start3_init( PLAYER_WORK *pl_w );
static void fly_start3_wait( PLAYER_WORK *pl_w );
static void fly_start3_move( PLAYER_WORK *pl_w );
static void fly_start3_end( PLAYER_WORK *pl_w );
static void fly_start3( PLAYER_WORK *pl_w );
static void fly_start4( PLAYER_WORK *pl_w );
static void fly_start5( PLAYER_WORK *pl_w );
static void fly_pos_move( PLAYER_WORK *pl_w, Sint16 spd, Uint8 ang );
static void fly_spd_set( PLAYER_WORK *pl_w, Sint16 spd, Sint16 acc );

/* speeds by the stick, in 8.8 (not read) */
static Sint16 fly_spd_tbl0[16][2] = {
	{ 0, 0 }, { 0x0600, 0 }, { -0x0600, 0 }, { 0, 0 },
	{ 0, -0x0600 }, { 0x0400, -0x0400 }, { -0x0400, -0x0400 }, { 0, 0 },
	{ 0, 0x0600 }, { 0x0400, 0x0400 }, { -0x0400, 0x0400 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
} ;
static Sint16 fly_spd_tbl1[16][2] = {
	{ 0, 0 }, { 0x0800, 0 }, { -0x0800, 0 }, { 0, 0 },
	{ 0, -0x0800 }, { 0x0500, -0x0500 }, { -0x0500, -0x0500 }, { 0, 0 },
	{ 0, 0x0800 }, { 0x0500, 0x0500 }, { -0x0500, 0x0500 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
} ;
/* the angle of flight, in 32nds of a turn, by the stick */
static Sint8 fly_ang_tbl[16] = { 24, 8, 24, 0, 16, 12, 20, 0, 0, 4, 28, 0, 0, 0, 0, 0 } ;
/* the flying animation, and the one for stopping, by the stick */
static Uint8 fly_char_tbl[16] = { 8, 4, 8, 8, 6, 5, 7, 2, 2, 3, 9, 2, 2, 2, 2, 2 } ;
static Uint8 fly_stop_tbl[16] = { 10, 12, 16, 16, 14, 13, 15, 10, 10, 11, 17, 10, 10, 10, 10, 10 } ;

/****************************************************************/
/*			The flight: the stick steers, and letting go of		*/
/*			it stops											*/
/****************************************************************/
void pl_fly( PLAYER_WORK *pl_w )
{
	Uint8 ang ;
	Sint16 spd[2][2] ;	/* not used */

	if( !pl_w->fly_flag ){
		fly_end( pl_w ) ;
		return ;
	}
	pl_dir_set( pl_w ) ;
	if( pl_nml_atck_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	pl_w->fly_lever = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ;
	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( !pl_w->fly_lever ) return ;
		pl_w->routine_2 ++ ;
		fly_dir_set( pl_w, pl_w->fly_lever ) ;
	}
	else {
		pl_char_move( pl_w ) ;
		if( !pl_w->fly_lever ){
			pl_w->routine_2 = 0 ;
			fly_stop_set( pl_w, pl_w->fly_lever ) ;
		}
		else {
			if( pl_w->dir != pl_w->fly_lever ) fly_dir_set( pl_w, pl_w->fly_lever ) ;
			ang = fly_ang_tbl[pl_w->dir] ;
			if( pl_w->pl_dir ){
				ang = 32 - ang ;
				ang &= 31 ;
			}
			switch( pl_w->pl_type ){
			case PL_SENTINEL :
				fly_pos_move( pl_w, 800, ang ) ;
				break ;
			default :
				fly_pos_move( pl_w, 600, ang ) ;
				break ;
			}
			pl_w->pl_stance = STANCE_AIR ;
			fly_y_ck( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			The flight is over: fall							*/
/*==============================================================*/
static void fly_end( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = pl_w->spd.y = pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void fly_dir_set( PLAYER_WORK *pl_w, Uint8 dir )
{
	pl_w->dir = dir ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_char_set( pl_w, Char_Fly, fly_char_tbl[pl_w->dir & 15] ) ;
}

/*==============================================================*/
static void fly_stop_set( PLAYER_WORK *pl_w, Uint8 dir )
{
	pl_w->dir = dir ;
	pl_char_set( pl_w, Char_Fly, fly_stop_tbl[pl_w->dir & 15] ) ;
}

/*==============================================================*/
/*			Keeps the flier above the ground					*/
/*==============================================================*/
static void fly_y_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y <= pl_w->ground_y + DOT_Y( 16.0f ) ) pl_w->pos.y = pl_w->ground_y + DOT_Y( 16.0f ) ;
}

/****************************************************************/
/*			The take-off, by the character						*/
/****************************************************************/
void pl_fly_start( PLAYER_WORK *pl_w )
{
	switch( pl_w->pl_type ){
	case PL_MORRIGAN :
	case PL_SPIDERMAN :
	case PL_VENOM :
	case PL_TRON :
	case PL_SONSON :
	case PL_OMEGA_RED :
		pl_w->pl_tbl[TBL_AIR_DASH]( pl_w ) ;
		return ;
	case PL_ROGUE :
		fly_start2( pl_w ) ;
		break ;
	case PL_DOOM :
	case PL_WAR_MACHINE :
	case PL_IRON_MAN :
		fly_start3( pl_w ) ;
		break ;
	case PL_STORM :
	case PL_MAGNETO :
	case PL_DHALSIM :
		fly_start4( pl_w ) ;
		break ;
	case PL_BLACKHEART :
		fly_start5( pl_w ) ;
		break ;
	default :
		fly_start( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			A hop back											*/
/*==============================================================*/
static void fly_start( PLAYER_WORK *pl_w )
{
	static void ( *fly_start_jmp[] )( PLAYER_WORK *pl_w ) = {
		fly_start_init,
		fly_start_move,
		fly_start_fall,
	} ;

	fly_start_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fly_start_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 16 ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->spd.x = -DOT_X( 9.5f ) ;
	pl_w->acc.x = 0.3125f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Dash, 4 ) ;
}

/*--------------------------------------------------------------*/
static void fly_start_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void fly_start_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			A hop back, with no special moves or attacks for	*/
/*			a while												*/
/*==============================================================*/
static void fly_start2_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->sp_ng_timer = 10 ;
	pl_w->atck_ng_timer = 10 ;
	pl_w->timer0 = 16 ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->spd.x = -DOT_X( 9.5f ) ;
	pl_w->acc.x = 0.3125f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Dash, 4 ) ;
}

/*--------------------------------------------------------------*/
static void fly_start2_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void fly_start2_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void fly_start2( PLAYER_WORK *pl_w )
{
	static void ( *fly_start2_jmp[] )( PLAYER_WORK *pl_w ) = {
		fly_start2_init,
		fly_start2_move,
		fly_start2_fall,
	} ;

	fly_start2_jmp[pl_w->routine_2]( pl_w ) ;
}

/* speeds in 16.16 (not read) */
static Sint32 fly_spd_tbl2[16][2] = {
	{ 0x30f580, 0 }, { -0x2ff580, 0 }, { 0x30f580, 0 }, { 0, 0 },
	{ 0, 0x2cf600 }, { -0x1bf980, 0x18fa00 }, { 0x1cf980, 0x18fa00 }, { 0, 0 },
	{ 0, -0x2bf600 }, { -0x1bf980, -0x17fa00 }, { 0x1cf980, -0x17fa00 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
} ;

/*==============================================================*/
/*			A rise, with a voice								*/
/*==============================================================*/
static void fly_start3_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 4 ;
	fly_dir_set( pl_w, pl_w->dir ) ;
	if( pl_w->pl_type == PL_WAR_MACHINE && !pl_w->fly_flag ) pls2e_00_set( pl_w, 4, 0 ) ;
	switch( pl_w->pl_type ){
	case PL_WAR_MACHINE :
	case PL_IRON_MAN :
		pl_voice_req( pl_w, 34 ) ;
		break ;
	case PL_DOOM :
		pl_voice_req( pl_w, 36 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void fly_start3_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 24 ;
		fly_spd_set( pl_w, 1000, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fly_start3_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	fly_y_ck( pl_w ) ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		fly_stop_set( pl_w, pl_w->dir ) ;
	}
}

/*--------------------------------------------------------------*/
static void fly_start3_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void fly_start3( PLAYER_WORK *pl_w )
{
	static void ( *fly_start3_jmp[] )( PLAYER_WORK *pl_w ) = {
		fly_start3_init,
		fly_start3_wait,
		fly_start3_move,
		fly_start3_end,
	} ;

	fly_start3_jmp[pl_w->routine_2]( pl_w ) ;
}

/* speeds in 16.16 (not read) */
static Sint32 fly_spd_tbl3[16][2] = {
	{ 0x30f380, 0 }, { -0x2ff380, 0 }, { 0x30f380, 0 }, { 0, 0 },
	{ 0, 0x2cf400 }, { -0x1bf680, 0x18f700 }, { 0x1cf680, 0x18f700 }, { 0, 0 },
	{ 0, -0x2bf400 }, { -0x1bf680, -0x17f700 }, { 0x1cf680, -0x17f700 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
} ;
static Sint32 fly_spd_tbl4[16][2] = {
	{ 0x30f380, 0 }, { -0x2ff380, 0 }, { 0x30f380, 0 }, { 0, 0 },
	{ 0, 0x2cf400 }, { -0x1bf680, 0x18f700 }, { 0x1cf680, 0x18f700 }, { 0, 0 },
	{ 0, -0x2bf400 }, { -0x1bf680, -0x17f700 }, { 0x1cf680, -0x17f700 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
} ;
static Sint32 fly_spd_tbl5[16][2] = {
	{ 0x30f380, 0 }, { -0x2ff380, 0 }, { 0x30f380, 0 }, { 0, 0 },
	{ 0, 0x2cf400 }, { -0x1bf680, 0x18f700 }, { 0x1cf680, 0x18f700 }, { 0, 0 },
	{ 0, -0x2bf400 }, { -0x1bf680, -0x17f700 }, { 0x1cf680, -0x17f700 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
} ;

/*==============================================================*/
/*			A jump to the side, toward the stick				*/
/*==============================================================*/
static void fly_start5( PLAYER_WORK *pl_w )
{
	Float spd ;
	Float acc ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
		spd = DOT_X( 8.0f ) ;
		acc = 0.3125f ;
		if( ( pl_w->dir & 3 ) == 1 ) pl_char_set( pl_w, Char_Extra, 3 ) ;
		else {
			spd = -DOT_X( 8.0f ) ;
			acc = -0.3125f ;
			pl_char_set( pl_w, Char_Extra, 2 ) ;
		}
		if( pl_w->pl_dir ){
			spd = -spd ;
			acc = -acc ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
	}
	else if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
	else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		fly_y_ck( pl_w ) ;
	}
}

/*==============================================================*/
/*			A dash toward the stick								*/
/*==============================================================*/
static void fly_start4( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 24 ;
		fly_spd_set( pl_w, 1250, 20 ) ;
		fly_dir_set( pl_w, pl_w->dir ) ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	fly_y_ck( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Moves the flier spd along the angle ang				*/
/*==============================================================*/
static void fly_pos_move( PLAYER_WORK *pl_w, Sint16 spd, Uint8 ang )
{
	Float s ;
	Sint16 rad ;

	s = spd * 256.0f ;
	rad = ( ( 40 - ang ) & 31 ) << 11 ;
	pl_w->pos.x = pl_w->pos.x + s * nlCos( rad ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	pl_w->pos.y = pl_w->pos.y + s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
}

/*==============================================================*/
/*			Sets the speed spd along the stick's angle, and		*/
/*			acc along the angle a quarter turn on				*/
/*==============================================================*/
static void fly_spd_set( PLAYER_WORK *pl_w, Sint16 spd, Sint16 acc )
{
	Float s ;
	Uint8 ang ;
	Sint16 rad ;

	ang = fly_ang_tbl[pl_w->dir] ;
	if( pl_w->pl_dir ){
		ang = 32 - ang ;
		ang = ang & 31 ;
	}
	s = spd * 256.0f ;
	rad = ( ( 40 - ang ) & 31 ) << 11 ;
	pl_w->spd.x = s * nlCos( rad ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	pl_w->spd.y = s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
	s = acc * 256.0f ;
	ang += 16 ;
	ang &= 31 ;
	rad = ( ( 40 - ang ) & 31 ) << 11 ;
	pl_w->acc.x = s * nlCos( rad ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	pl_w->acc.y = s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_fly.c" End of File	*/
