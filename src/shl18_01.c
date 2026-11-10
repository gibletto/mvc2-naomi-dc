/* Abyss's effect 01 (character 18's shot 01): the three dark balls of pl18_sp_ball. Each starts beside
   him, turns towards the opponent a step at a time while it flies, and fades into a flash when
   it hits or he is done. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl18_def.h"

/* one orb: its first direction (32 to a circle), where it starts in front of him, its animation
   and its attack */
typedef struct SHL18_01_TBL {
	Sint8	dir;
	Uint8	unk_1;
	Sint16	x;
	Sint16	y;
	Uint8	char_no;
	Uint8	atck_no;
} SHL18_01_TBL;

static SHL18_01_TBL shl18_01_tbl[] = {
	{ 24, 0, -132, 138, 3, 50 },
	{ 28, 0, -132, 138, 3, 50 },
	{ 20, 0, -132, 138, 3, 50 },
} ;

static void shl18_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl18_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl18_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

static void shl18_01_init( PLAYER_WORK *ew );
static void shl18_01_move( PLAYER_WORK *ew );
static void shl18_01_end( PLAYER_WORK *ew );
static void shl18_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl18_01_main( PLAYER_WORK *ew )
{
	static void ( *shl18_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_01_init, shl18_01_move, shl18_01_end, shl18_01_free,
	} ;

	shl18_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl18_01_init( PLAYER_WORK *ew )
{
	const SHL18_01_TBL *tbl ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1801 ;
	pl_w = ew->master_w ;
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
	ew->disp_flag = 1 ;
	ew->disp_dir = 0 ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->timer0 = 1 ;
	ew->timer1 = 100 ;
	tbl = &shl18_01_tbl[ew->type] ;
	ew->dir = tbl->dir ;
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ) ew->pos.x += 1.6666666f * tbl->x ;
	else {
		ew->pos.x += - ( 1.6666666f * tbl->x ) ;
		ew->dir = ( 32 - ew->dir ) & 31 ;
	}
	ew->pos.y += 2.142857f * tbl->y ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = tbl->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, tbl->char_no ) ;
	shl18_01_move( ew ) ;
}

static void shl18_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_01_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl18_01_move( PLAYER_WORK *ew )
{
	static void ( *shl18_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl18_01_fly, shl18_01_vanish,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl18_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl18_01_aim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It flies on towards the opponent until it hits		*/
/*			something											*/
/*==============================================================*/
static void shl18_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	shl18_01_aim( ew, pl_w ) ;
	dir_spd_move( ew, 300 ) ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

static void shl18_01_vanish_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_01_vanish_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl18_01_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl18_01_vanish_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl18_01_vanish_init, shl18_01_vanish_move,
	} ;

	shl18_01_vanish_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl18_01_vanish_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_2 ++ ;
	ew->timer0 = 10 ;
	ew->scale.x = ew->scale.y = ew->alpha = 1.0f ;
	shl18_01_vanish_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			It swells and fades over the ten frames				*/
/*==============================================================*/
static void shl18_01_vanish_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->scale.x += ( 3.0f - ew->scale.x ) / ew->timer0 ;
	ew->scale.y += ( 3.0f - ew->scale.y ) / ew->timer0 ;
	ew->alpha += ( 0.0f - ew->alpha ) / ew->timer0 ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl18_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl18_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Every few frames, a step of its direction			*/
/*			towards the opponent								*/
/*==============================================================*/
static void shl18_01_aim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	register Uint8 to ;

	if( -- ew->timer0 == 0 ){
		em_w = pl_w->target_w ;
		to = caldir_pos_256( &ew->pos, &em_w->pos ) >> 3 ;
		if( to != ew->dir ){
			if( ( ( to - ew->dir ) & 31 ) <= 15 ) ew->dir ++ ;
			else ew->dir -- ;
			ew->dir &= 31 ;
		}
		if( -- ew->timer1 == 0 ) ew->timer0 = -1 ;
		else ew->timer0 = 5 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl18_01.c" End of File	*/
