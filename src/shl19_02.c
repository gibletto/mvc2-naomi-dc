/* Abyss's second form's thrown shot (effect 02): it flies out along one of 32 directions, turned
   to face the way it goes, until its time is up or it leaves the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl19_def.h"

/* the shot's turn for each of the 32 directions */
static Sint32 shl19_02_rot_tbl[32] = {
	0xc000, 0xb800, 0xb000, 0xa800, 0xa000, 0x9800, 0x9000, 0x8800,
	0x8000, 0x7800, 0x7000, 0x6800, 0x6000, 0x5800, 0x5000, 0x4800,
	0x4000, 0x3800, 0x3000, 0x2800, 0x2000, 0x1800, 0x1000, 0x0800,
	0x0000, 0xf800, 0xf000, 0xe800, 0xe000, 0xd800, 0xd000, 0xc800,
} ;

/* where it starts from the player, for each direction */
static struct { Sint16 x, y ; } shl19_02_pos_tbl[32] = {
	{ -4, 140 }, { 13, 146 }, { 24, 143 }, { 50, 136 },
	{ 51, 134 }, { 58, 130 }, { 69, 126 }, { 79, 121 },
	{ 92, 116 }, { 111, 110 }, { 121, 102 }, { 135, 90 },
	{ 147, 83 }, { 163, 70 }, { 172, 61 }, { 175, 53 },
	{ 180, 42 }, { 165, 35 }, { 149, 37 }, { 136, 40 },
	{ 114, 47 }, { 105, 52 }, { 91, 58 }, { 87, 61 },
	{ 84, 63 }, { 72, 70 }, { 58, 77 }, { 47, 85 },
	{ 26, 96 }, { 16, 105 }, { 6, 114 }, { 0, 123 },
} ;

static void shl19_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl19_02_set( PLAYER_WORK *pl_w, Uint8 a, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl19_02_main ;
		ew->master_w = pl_w ;
		ew->type = a ;
		ew->sub_no = no ;
	}
	return ew ;
}

static void shl19_02_init( PLAYER_WORK *ew );
static void shl19_02_move( PLAYER_WORK *ew );
static void shl19_02_end( PLAYER_WORK *ew );
static void shl19_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl19_02_main( PLAYER_WORK *ew )
{
	static void ( *shl19_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl19_02_init, shl19_02_move, shl19_02_end, shl19_02_free,
	} ;

	shl19_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl19_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1902 ;
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
	ew->prio = 8 ;
	ew->disp_flag = 1 ;
	ew->disp_dir = 0 ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->timer0 = 20 ;
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ){
		ew->pos.x += DOT_X( shl19_02_pos_tbl[ew->sub_no].x ) ;
	} else {
		ew->pos.x -= DOT_X( shl19_02_pos_tbl[ew->sub_no].x ) ;
	}
	ew->pos.y += DOT_Y( shl19_02_pos_tbl[ew->sub_no].y ) ;
	if( pl_w->disp_dir ) ew->sub_no = ( 32 - ew->sub_no ) & 31 ;
	ew->dir = ew->sub_no ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl19_at_storm ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 2 ) ;
	shl19_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl19_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ) goto out ;
	dir_move( ew, 0x708 ) ;
	if( ew->timer0 ) ew->timer0 -- ;
	else if( !scr_in_ck( ew ) ){
out:
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->rot[2] = shl19_02_rot_tbl[ew->dir] ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl19_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl19_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl19_02.c" End of File	*/
