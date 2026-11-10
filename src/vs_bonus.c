/* The bonus lines of the fight's end (kind 12 works): a line each for the score, the other bonuses
   and the time, or for the time, the longest combo and the wins of the two players' result. Each
   shows after the one before it and they all go together. */
#include "player.h"

extern int sprintf( char *s, const char *fmt, ... ) ;

/* one step of a line's script (its reader is elsewhere) */
typedef struct BONUS_STEP {
	Uint8	unk_0, unk_1, unk_2, unk_3 ;
	Float	unk_4 ;
} BONUS_STEP;

/* a score line's own work, from atck_flag (0x19c): its digits, and the number they show */
typedef struct BONUS_LOCAL_W {
	Sint8	buf[0x2c] ;	/* 000: the digits, 0 to 9, or -1 for a blank */
	Uint32	hi ;		/* 02c: the number's high digits in BCD, or all of it */
	Uint32	lo ;		/* 030: and its low digits */
} BONUS_LOCAL_W;
/* a work's step list, from unk_d4: the script it plays, and the step it is at */
typedef struct BONUS_SPR_W {
	Sint32	obj_flag, unk_d0 ;
	BONUS_STEP *step ;	/* 008 */
	Sint32	no ;		/* 00c */
} BONUS_SPR_W;
#define BONUS_SW( w )	( ( BONUS_SPR_W * )&( w )->obj_flag )
#define BONUS_LW( w )	( ( BONUS_LOCAL_W * )&( w )->atck_flag )

/* scales that nothing reads */
static VEC3 bonus_scale_tbl[] = {
	{ 1.5f, 1.5f, 1.0f }, { 0.66f, 0.66f, 1.0f }, { 0.5f, 1.0f, 1.0f }, { 1.0f, 0.5f, 1.0f },
	{ 2.5f, 2.5f, 1.0f },
} ;
static VEC3 *bonus_scale_p[] = {
	&bonus_scale_tbl[0], &bonus_scale_tbl[0], &bonus_scale_tbl[1], &bonus_scale_tbl[2],
	&bonus_scale_tbl[3], &bonus_scale_tbl[4],
} ;

static BONUS_STEP bonus_line0[] = {
	{ 3, 0xff, 0, 0x1f, -24.5f },
	{ 3, 0xff, 0, 0x12, 2.5f },
	{ 3, 0xff, 0, 0x1d, 2.5f },
	{ 3, 0xff, 0, 0xa, 2.75f },
	{ 3, 0xff, 0, 0x15, 3.0f },
	{ 4, 0, 3, 0, 24.75f },
	{ 4, 1, 0, 0, 3.5f },
	{ 4, 2, 0, 0, 3.5f },
	{ 4, 3, 0, 0, 3.5f },
	{ 4, 4, 0, 0, 3.5f },
	{ 4, 5, 0, 0, 3.5f },
	{ 4, 6, 0, 0, 3.5f },
	{ 4, 7, 0, 0, 3.5f },
	{ 4, 8, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP bonus_line1[] = {
	{ 3, 0xff, 0, 0x1d, -24.5f },
	{ 3, 0xff, 0, 0x12, 2.75f },
	{ 3, 0xff, 0, 0x16, 3.25f },
	{ 3, 0xff, 0, 0xe, 3.75f },
	{ 4, 0, 3, 0, 25.75f },
	{ 4, 1, 0, 0, 3.5f },
	{ 4, 2, 0, 0, 3.5f },
	{ 4, 3, 0, 0, 3.5f },
	{ 4, 4, 0, 0, 3.5f },
	{ 4, 5, 0, 0, 3.5f },
	{ 4, 6, 0, 0, 3.5f },
	{ 4, 7, 0, 0, 3.5f },
	{ 4, 8, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP bonus_line2[] = {
	{ 3, 0xff, 0, 0xc, -24.5f },
	{ 3, 0xff, 0, 0x18, 3.5f },
	{ 3, 0xff, 0, 0x16, 4.0f },
	{ 3, 0xff, 0, 0x19, 4.0f },
	{ 3, 0xff, 0, 0x15, 3.0f },
	{ 3, 0xff, 0, 0xe, 2.75f },
	{ 3, 0xff, 0, 0x1d, 3.0f },
	{ 3, 0xff, 0, 0xe, 3.0f },
	{ 4, 0, 3, 0, 12.25f },
	{ 4, 1, 0, 0, 3.5f },
	{ 4, 2, 0, 0, 3.5f },
	{ 4, 3, 0, 0, 3.5f },
	{ 4, 4, 0, 0, 3.5f },
	{ 4, 5, 0, 0, 3.5f },
	{ 4, 6, 0, 0, 3.5f },
	{ 4, 7, 0, 0, 3.5f },
	{ 4, 8, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP bonus_line3[] = {
	{ 3, 0xff, 0, 0x1d, -24.5f },
	{ 3, 0xff, 0, 0x18, 3.5f },
	{ 3, 0xff, 0, 0x1d, 3.5f },
	{ 3, 0xff, 0, 0xa, 3.0f },
	{ 3, 0xff, 0, 0x15, 3.25f },
	{ 4, 0, 3, 0, 22.25f },
	{ 4, 1, 0, 0, 3.5f },
	{ 4, 2, 0, 0, 3.5f },
	{ 4, 3, 0, 0, 3.5f },
	{ 4, 4, 0, 0, 3.5f },
	{ 4, 5, 0, 0, 3.5f },
	{ 4, 6, 0, 0, 3.5f },
	{ 4, 7, 0, 0, 3.5f },
	{ 4, 8, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP bonus_line4[] = {
	{ 3, 0xff, 0, 0x20, -12.0f },
	{ 3, 0xff, 0, 0x12, 4.0f },
	{ 3, 0xff, 0, 0x17, 2.75f },
	{ 3, 0xff, 0, 0x1c, 3.5f },
	{ 4, 0, 3, 0, 17.25f },
	{ 4, 1, 0, 0, 3.5f },
	{ 4, 2, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP bonus_line5[] = {
	{ 3, 0xff, 0, 0x16, -27.0f },
	{ 3, 0xff, 0, 0xa, 3.75f },
	{ 3, 0xff, 0, 0x21, 3.0f },
	{ 3, 0xff, 0, 0xc, 4.5f },
	{ 3, 0xff, 0, 0x18, 3.25f },
	{ 3, 0xff, 0, 0x16, 3.75f },
	{ 3, 0xff, 0, 0xb, 3.75f },
	{ 3, 0xff, 0, 0x18, 3.25f },
	{ 4, 0, 3, 0, 17.25f },
	{ 4, 1, 0, 0, 3.5f },
	{ 4, 2, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP bonus_line6[] = {
	{ 3, 0xff, 0, 0x19, -24.0f },
	{ 3, 0xff, 0, 0x15, 3.0f },
	{ 3, 0xff, 0, 0xa, 3.0f },
	{ 3, 0xff, 0, 0x22, 3.25f },
	{ 3, 0xff, 0, 0x1d, 4.25f },
	{ 3, 0xff, 0, 0x12, 2.5f },
	{ 3, 0xff, 0, 0x16, 3.0f },
	{ 3, 0xff, 0, 0xe, 3.5f },
	{ 4, 0xff, 0, 0x27, 18.0f },
	{ 4, 0xff, 0, 0x27, 5.25f },
	{ 4, 0, 3, 0, -8.0f },
	{ 4, 1, 0, 0, 7.0f },
	{ 4, 2, 0, 0, 3.5f },
	{ 4, 3, 0, 0, 7.0f },
	{ 4, 4, 0, 0, 3.5f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static BONUS_STEP *bonus_line_tbl[] = {
	bonus_line0, bonus_line1, bonus_line2, bonus_line3, bonus_line4, bonus_line5, bonus_line6,
} ;

static Sint16 bonus_line_time_tbl[8] = { 0, 4, 8, 12, 0, 4, 8, 0 } ;

static PLAYER_WORK *bonus_line_set( PLAYER_WORK *prev_w, int no );
static void bonus_score( PLAYER_WORK *ew );
static void bonus_count( PLAYER_WORK *ew );
static void bonus_time( PLAYER_WORK *ew );

/****************************************************************/
/*			The fight's bonus lines for one player (type 3		*/
/*			to 0), or the two players' (type 6 to 4)			*/
/****************************************************************/
void bonus_screen_set( int two_p )
{
	PLAYER_WORK *ew ;

	na_req( 63 ) ;
	if( !two_p ){
		if( !( ew = bonus_line_set( 0, 3 ) ) ) return ;
		ew->pos.y = -10.0f ;
		bonus_add( &BONUS_LW( ew )->hi, &BONUS_LW( ew )->lo ) ;
		bonus_score( ew ) ;
		if( ( ew = bonus_line_set( ew, 2 ) ) ){
			ew->pos.y = 0.0f ;
			unk_9a_bonus( &BONUS_LW( ew )->hi, &BONUS_LW( ew )->lo ) ;
			bonus_score( ew ) ;
			if( ( ew = bonus_line_set( ew, 1 ) ) ){
				ew->pos.y = 5.0f ;
				time_bonus( &BONUS_LW( ew )->hi, &BONUS_LW( ew )->lo ) ;
				bonus_score( ew ) ;
				if( ( ew = bonus_line_set( ew, 0 ) ) ){
					ew->pos.y = 10.0f ;
					vital_bonus( &BONUS_LW( ew )->hi, &BONUS_LW( ew )->lo ) ;
					bonus_score( ew ) ;
				}
			}
		}
	} else {
		if( !( ew = bonus_line_set( 0, 6 ) ) ) return ;
		ew->pos.y = 0.0f ;
		BONUS_LW( ew )->lo = ( Uint16 )vs_sys_work.fight_frame ;
		bonus_time( ew ) ;
		if( ( ew = bonus_line_set( ew, 5 ) ) ){
			ew->pos.y = 5.0f ;
			BONUS_LW( ew )->lo = vs_sys_work.combo_max[vs_sys_work.winner_id] ;
			bonus_count( ew ) ;
			if( ( ew = bonus_line_set( ew, 4 ) ) ){
				ew->pos.y = 10.0f ;
				BONUS_LW( ew )->lo = vs_sys_work.win_cnt[vs_sys_work.winner_id] ;
				bonus_count( ew ) ;
			}
		}
	}
	ew->sub_type = 1 ;
}

static void bonus_line_main( PLAYER_WORK *ew );

/*==============================================================*/
static PLAYER_WORK *bonus_line_set( PLAYER_WORK *prev_w, int no )
{
	PLAYER_WORK *ew ;
	int list = 3 ;

	if( !prev_w ) list = 1 ;
	if( ( ew = work_get( prev_w, W_FRONT, list ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = bonus_line_main ;
		ew->obj = 0 ;
		ew->obj_flag = 0 ;
		ew->ef_no = 8 ;
		ew->type = no ;
		ew->pos.x = 0.0f ;
		ew->pos.y = 0.0f ;
		ew->pos.z = -40.0f ;
		ew->ef_w = prev_w ;
		ew->timer0 = bonus_line_time_tbl[no] ;
		ew->timer1 = 120 - ew->timer0 ;
	}
	return ew ;
}

static void bonus_line_init( PLAYER_WORK *ew );
static void bonus_line_run( PLAYER_WORK *ew );
static void bonus_line_free( PLAYER_WORK *ew );

/*==============================================================*/
static void bonus_line_main( PLAYER_WORK *ew )
{
	static void ( *bonus_line_jmp[] )( PLAYER_WORK *ew ) = {
		bonus_line_init, bonus_line_run, bonus_line_free, bonus_line_free,
	} ;

	bonus_line_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void bonus_line_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->pos.x = 0.0f ;
	ew->pos.z = -40.0f ;
	BONUS_SW( ew )->step = bonus_line_tbl[ew->type] ;
	BONUS_SW( ew )->no = 0 ;
	bonus_line_run( ew ) ;
}

static void bonus_line_wait( PLAYER_WORK *ew );
static void bonus_line_stay( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void bonus_line_run( PLAYER_WORK *ew )
{
	static void ( *bonus_line_run_jmp[] )( PLAYER_WORK *ew ) = {
		bonus_line_wait, bonus_line_stay,
	} ;

	bonus_line_run_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			It shows when the delay ends, or at once when a		*/
/*			player is pressing a button							*/
/*==============================================================*/
static void bonus_line_wait( PLAYER_WORK *ew )
{
	if( !( ( Sys->entry_side & 1 ) && ( sw_work[0].now & KEY_ATCK ) ) ){
		if( !( ( Sys->entry_side & 2 ) && ( sw_work[1].now & KEY_ATCK ) ) ){
			if( -- ew->timer0 > 0 ) return ;
		}
	}
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	bonus_line_stay( ew ) ;
}

/*--------------------------------------------------------------*/
/* the last line of the chain makes them all go, after its time or a button */
static void bonus_line_stay( PLAYER_WORK *ew )
{
	int flag ;

	if( ew->sub_type ){
		if( !( ( Sys->entry_side & 1 ) && ( sw_work[0].now & KEY_ATCK ) ) ){
			if( !( ( Sys->entry_side & 2 ) && ( sw_work[1].now & KEY_ATCK ) ) ) goto count ;
		}
		if( ew->timer1 >= 30 ) ew->timer1 = 30 ;
	count:
		if( -- ew->timer1 > 0 ) return ;
		if( ew->ef_w ){
			flag = 1 ;
			while( flag ){
				if( !ew->behind_w->ef_w ) flag = 0 ;
				bonus_line_free( ew->behind_w ) ;
			}
		}
		bonus_line_free( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void bonus_line_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			A number of up to nine digits: the high digit,		*/
/*			and eight more; a blank for a leading zero			*/
/*==============================================================*/
static void bonus_score( PLAYER_WORK *ew )
{
	Sint8 *buf = BONUS_LW( ew )->buf ;
	Uint32 hi, i ;

	hi = BONUS_LW( ew )->hi ;
	if( hi > 9 ) hi = 9 ;
	sprintf( ( char * )buf, "%1x", hi ) ;
	if( hi ){
		sprintf( ( char * )( buf + 1 ), "%08x", BONUS_LW( ew )->lo ) ;
	} else {
		buf[0] = ' ' ;
		sprintf( ( char * )( buf + 1 ), "%8x", BONUS_LW( ew )->lo ) ;
	}
	for( i = 0 ; i < 9 ; i ++ ){
		if( ( buf[i] -= '0' ) < 0 ) buf[i] = -1 ;
	}
}

/*==============================================================*/
static void bonus_count( PLAYER_WORK *ew )
{
	Sint8 *buf = BONUS_LW( ew )->buf ;
	Uint32 n ;
	Uint32 i ;

	n = BONUS_LW( ew )->lo ;
	if( n >= 1000 ) n = 999 ;
	sprintf( ( char * )buf, "%3d", n ) ;
	for( i = 0 ; i < 3 ; i ++ ){
		if( ( buf[i] -= '0' ) < 0 ) buf[i] = -1 ;
	}
}

/*==============================================================*/
/*			A time in frames as minutes, seconds and			*/
/*			hundredths; over ten minutes it shows 9:59.99		*/
/*==============================================================*/
static void bonus_time( PLAYER_WORK *ew )
{
	Uint8 *buf = ( Uint8 * )BONUS_LW( ew )->buf ;
	Uint16 t = BONUS_LW( ew )->lo, m, s, c ;
	Uint16 i ;

	m = t / 3600 ;
	if( m > 9 ){
		buf[0] = '9' ;
		buf[1] = '5' ;
		buf[2] = '9' ;
		buf[3] = '9' ;
		buf[4] = '9' ;
		goto conv ;
	}
	buf[0] = m + '0' ;
	t = t % 3600 ;
	s = t / 60 ;
	sprintf( ( char * )( buf + 1 ), "%02d", s ) ;
	t = t % 60 ;
	c = t * 100 / 60 ;
	sprintf( ( char * )( buf + 3 ), "%02d", c ) ;
conv :
	for( i = 0 ; i < 5 ; i ++ ){
		if( ( buf[i] -= '0' ) < 0 ) buf[i] = 0xff ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "vs_bonus.c" End of File	*/
