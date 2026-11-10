/* Captain America's effect 01 (his pls0b): the pieces of the entrance in which he jumps in, three
   types: type 0 is not shown, puts out nine of type 1 and flies across the screen, type 1 follows
   it as a trail and type 2 is put by him every four frames of his flight and runs out there. */
#include "player.h"
#include "pl_char.h"

/* the effect's own work, from obj_mtx */
typedef struct PLS0B_01_LOCAL_W {
	Float	x;		/* 000: how far it has gone */
} PLS0B_01_LOCAL_W;
#define PLS0B_01_LW( w )	( ( PLS0B_01_LOCAL_W * )&( w )->obj_mtx )

static void pls0b_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0b_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls0b_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->ef_no = 0xb01 ;
		ew->disp_flag = 0 ;
	}
	return ew ;
}

/****************************************************************/
/*			A piece that follows the work ew, which is the		*/
/*			master's											*/
/****************************************************************/
PLAYER_WORK *pls0b_01_set2( PLAYER_WORK *ew, Uint8 a, Uint8 b )
{
	PLAYER_WORK *nw ;

	if( ( nw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
		nw->work_func = pls0b_01_main ;
		nw->master_w = ew->master_w ;
		nw->ef_w = ew ;
		nw->pl_type = ew->pl_type ;
		nw->type = a ;
		nw->sub_type = b ;
		nw->ef_no = 0xb01 ;
		nw->disp_flag = 0 ;
	}
	return nw ;
}

static void pls0b_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0b_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0b_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0b_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void pls0b_01_main( PLAYER_WORK *ew )
{
	static void ( *pls0b_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0b_01_init, pls0b_01_move, pls0b_01_end, pls0b_01_free,
	} ;

	pls0b_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls0b_01_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS0B_01_LOCAL_W *lw = PLS0B_01_LW( ew ) ;
	int x ;

	pls0b_01_set2( ew, 1, 0 ) ;
	pls0b_01_set2( ew, 1, 1 ) ;
	pls0b_01_set2( ew, 1, 2 ) ;
	pls0b_01_set2( ew, 1, 3 ) ;
	pls0b_01_set2( ew, 1, 4 ) ;
	pls0b_01_set2( ew, 1, 5 ) ;
	pls0b_01_set2( ew, 1, 6 ) ;
	pls0b_01_set2( ew, 1, 7 ) ;
	pls0b_01_set2( ew, 1, 8 ) ;
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 104.0f ) ;
	x = 0x100000 ;
	if( !ew->disp_dir ) x = -0x100000 ;
	ew->spd.x = x * 1.6666666f / 65536.0f ;
	ew->spd.y = -3.75f ;
	ew->acc.x = ew->spd.x / 64.0f ;
	ew->acc.y = ew->spd.y / 64.0f ;
	lw->x = 0.0f ;
	pl_char_set( ew, Char_Effect, 1 ) ;
}

/*==============================================================*/
static void pls0b_01_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS0B_01_LOCAL_W *lw = PLS0B_01_LW( ew ) ;
	PLAYER_WORK *par_w = ew->ef_w ;
	int x ;
	int k ;

	ew->disp_flag = 0 ;
	ew->sub_prio = 4 ;
	x = ( ew->sub_type << 6 ) << 16 ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x = par_w->pos.x + x * 1.6666666f / 65536.0f ;
	ew->pos.y = par_w->pos.y + ( 0x800 - ( Sint16 )( ( ew->sub_type << 3 ) << 8 ) ) * 2.142857f / 256.0f ;
	k = ew->sub_type - 1 ;
	if( k < 0 ) k = 0 ;
	lw->x = ( k << 22 ) * 1.6666666f / 65536.0f ;
	ew->timer0 = 0 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*==============================================================*/
static void pls0b_01_t2_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;

	ew->sub_prio = -2 ;
	x = 0x2000 ;
	if( !ew->disp_dir ) x = -0x2000 ;
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x + x * 1.6666666f / 256.0f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 101.0f ) ;
	pl_char_set( ew, Char_Effect, 1 ) ;
}

/*==============================================================*/
static void pls0b_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0b_01_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0b_01_head_init, pls0b_01_trail_init, pls0b_01_t2_init,
	} ;

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
	pls0b_01_init_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Type 0, first: waits for the fight to start, or		*/
/*			his own work to be well on							*/
/*==============================================================*/
static void pls0b_01_head_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st > VS_APPEAR || pl_w->routine_3 > 2 ){
		ew->routine_1 ++ ;
		ew->timer0 = 32 ;
	}
}

/*==============================================================*/
/*			Then, counts the frames down						*/
/*==============================================================*/
static void pls0b_01_head_count( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( -- ew->timer0 == 0 ) ew->routine_1 ++ ;
}

/*==============================================================*/
/*			Then flies, making sparks, until it is off the		*/
/*			screen												*/
/*==============================================================*/
static void pls0b_01_head_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS0B_01_LOCAL_W *lw = PLS0B_01_LW( ew ) ;
	Float x ;

	x = ew->spd.x ;
	if( x < 0.0f ) x = -x ;
	lw->x += x ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->disp_dir ){
		if( scr_work.scr_right - ew->pos.x < 0.0f ){
			ew->routine_0 ++ ;
			return ;
		}
	} else if( scr_work.scr_left - ew->pos.x > 0.0f ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !( Sys->game_timer0 & 3 ) ){
		burst_set( pl_w, &ew->pos, 1, 0x89 ) ;
		pl_se_req( ew, 73 ) ;
	}
}

/*==============================================================*/
static void pls0b_01_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0b_01_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0b_01_head_wait, pls0b_01_head_count, pls0b_01_head_fly,
	} ;

	ew->disp_flag = 0 ;
	pls0b_01_head_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Type 1: grows by the distance the first has gone	*/
/*==============================================================*/
static void pls0b_01_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *par_w = ew->ef_w ;
	PLS0B_01_LOCAL_W *par_lw = PLS0B_01_LW( par_w ) ;
	PLS0B_01_LOCAL_W *lw = PLS0B_01_LW( ew ) ;
	Float f ;

	ew->disp_flag = 0 ;
	if( par_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	f = par_lw->x - lw->x ;
	if( f > 0.0f ){
		f /= 16.0f ;
		if( f < 0.0f ) f = 0.0f ;
		if( f > 3.0f ){
			ew->routine_0 ++ ;
			return ;
		}
		pl_char_set2( ew, Char_Effect, 0, f ) ;
	}
	ew->disp_flag = 1 ;
}

/*==============================================================*/
static void pls0b_01_t2_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls0b_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0b_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0b_01_head_move, pls0b_01_trail_move, pls0b_01_t2_move,
	} ;

	pls0b_01_move_jmp[ew->type]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0b_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0b_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0b_01.c" End of File	*/
