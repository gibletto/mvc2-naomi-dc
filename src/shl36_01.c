/* Thanos's effect 01 (character 36's shot 01): the pair of blasts that come in from both sides of
   the screen in his Gauntlet Reality hyper combo, one each side, striking where he stood and
   sparking as they rush in. */
#include "player.h"
#include "pl_char.h"
#include "pl36_def.h"

/* a spark of effect 00: its kind in the high byte of no, a place on screen */
extern PLAYER_WORK *pls36_00_set( PLAYER_WORK *w, Float x, Float y, Uint16 no );

static void shl36_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl36_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i ;
	Sint16 x ;

	x = -224 ;
	if( pl_w->disp_dir ) x = 224 ;
	for( i = 0 ; i < 2 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl36_01_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3601 ;
			ew->type = i ;
			ew->sub_type = pl_w->disp_dir ^ i ;
			ew->obj_flag = ew->pos.x = pl_w->pos.x + DOT_X( x ) ;
			ew->pos.y = pl_w->ground_y ;
		}
	}
	return ew ;
}

static void shl36_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_01_end( PLAYER_WORK *ew );
static void shl36_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl36_01_main( PLAYER_WORK *ew )
{
	static void ( *shl36_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_01_init, shl36_01_move, shl36_01_end, shl36_01_free,
	} ;

	shl36_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl36_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 no ;
	Sint16 x ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->prio = 0 ;
	ew->disp_dir = ew->type ;
	ew->dir = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	no = 57 ;
	if( ew->sub_type ) no = 58 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	x = 128 ;
	if( ew->disp_dir ) x = -128 ;
	ew->pos.x += DOT_X( x ) ;
	pl_char_set( ew, 22, 0 ) ;
	shl36_01_move( ew, pl_w ) ;
}

static void shl36_01_start( PLAYER_WORK *ew );
static void shl36_01_wait( PLAYER_WORK *ew );
static void shl36_01_rush( PLAYER_WORK *ew );
static void shl36_01_rush_near( PLAYER_WORK *ew );
static void shl36_01_rush_end( PLAYER_WORK *ew );
static void shl36_01_stop( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl36_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl36_01_start, shl36_01_wait, shl36_01_rush, shl36_01_rush_near, shl36_01_rush_end, shl36_01_stop,
	} ;

	int no = ew->routine_1 ;

	if( no && no != 5 && ( pl_w->act_st != N_SUPER || pl_w->routine_1 ) ){
		ew->routine_1 = 5 ;
		ew->timer0 = 1 ;
	}
	shl36_01_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			The blast gathers: sparks fly off it until its		*/
/*			animation ends, and it starts to move in			*/
/*==============================================================*/
static void shl36_01_start( PLAYER_WORK *ew )
{
	Float spd, acc, x ;

	x = ew->pos.x + ( ( int )( Rnd() & 62 ) - 32 ) * 1.6666666f ;
	pls36_00_set( ew, x, ew->pos.y, 0x100 ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 20 ;
		spd = -7.5f ;
		acc = -DOT_X( 0.125f ) ;
		if( ew->disp_dir ){
			spd = 7.5f ;
			acc = DOT_X( 0.125f ) ;
		}
		ew->spd.x = spd ;
		ew->acc.x = acc ;
	}
	else if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->hit_flag = 0 ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl36_01_wait( PLAYER_WORK *ew )
{
	Uint8 no ;

	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		ew->timer1 = 0xc000 ;
		no = 59 ;
		if( ew->sub_type ) no = 60 ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl36_01_rush( PLAYER_WORK *ew )
{
	Float d ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !( ew->timer1 += 0x2000 ) ) ew->hit_flag = 0 ;
	d = ew->pos.x - ew->obj_flag ;
	if( d < 0.0f ) d = -d ;
	if( d < 80.0f ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 1 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl36_01_rush_near( PLAYER_WORK *ew )
{
	Float d ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !( ew->timer1 += 0x2000 ) ) ew->hit_flag = 0 ;
	d = ew->pos.x - ew->obj_flag ;
	if( d < 0.0f ) d = -d ;
	if( d < 40.0f ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 2 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl36_01_rush_end( PLAYER_WORK *ew )
{
	Float d, ofs ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !( ew->timer1 += 0x2000 ) ) ew->hit_flag = 0 ;
	d = ew->pos.x - ew->obj_flag ;
	if( d < 0.0f ) d = -d ;
	ofs = DOT_X( 8.0f ) ;
	if( d < ofs ){
		ew->routine_1 ++ ;
		ew->timer0 = 30 ;
		if( ew->disp_dir ) ofs = -DOT_X( 8.0f ) ;
		ew->pos.x = ew->obj_flag + ofs ;
		pl_se_req( ew, 49 ) ;
	}
	hit_work_set( ew ) ;
}

/* where the sparks burst about it as it lands */
static Sint16 shl36_01_burst[8][2] = {
	{ 4, 272 },
	{ -2, 224 },
	{ 0, 192 },
	{ -4, 160 },
	{ 0, 144 },
	{ -8, 112 },
	{ 2, 96 },
	{ 0, 64 },
} ;

/*--------------------------------------------------------------*/
static void shl36_01_stop( PLAYER_WORK *ew )
{
	Sint16 *p ;
	Sint16 i ;
	Float x, y ;

	pl_char_move( ew ) ;
	if( !( ew->timer1 += 0x2000 ) ) ew->hit_flag = 0 ;
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		if( !ew->type ) pl_voice_req( ew, 35 ) ;
		p = shl36_01_burst[0] ;
		for( i = 0 ; i < 8 ; i ++ ){
			x = ew->pos.x + *p ++ * 1.6666666f ;
			y = ew->pos.y + *p ++ * 2.142857f ;
			pls36_00_set( ew, x, y, 1 ) ;
		}
		ew->atck_no = pl36_at_reality ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl36_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl36_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl36_01.c" End of File	*/
