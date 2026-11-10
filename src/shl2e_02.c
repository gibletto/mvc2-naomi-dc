/* War Machine's effect 02 (character 2e's shot 02): a shell of his Smart Bomb, which is thrown out
   and falls (type 0), with a trail of four parts that follow it (type 1). */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

/* where the shell starts from War Machine: x and y, by the way it is thrown */
static Sint16 shl2e_02_pos_tbl[] = {
	6, 81, -46, 81,
} ;

/* its speed, its fall and its lift, by the way it is thrown and War Machine's level (16.16) */
static Sint32 shl2e_02_spd_tbl[] = {
	-0x48000, 0x1000, -0x3000, -0x68000, 0x1800, -0x3000, -0x88000, 0x2000, -0x3000,
	-0x50000, 0x1000, -0x2000, -0x70000, 0x1800, -0x2000, -0x90000, 0x2000, -0x2000,
} ;

static void shl2e_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2e_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2e_02_main ;
		ew->ef_no = 0x2e02 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part that follows the work hw						*/
/*==============================================================*/
static void *shl2e_02_part_set( PLAYER_WORK *hw, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl2e_02_main ;
		ew->ef_no = 0x2e02 ;
		ew->ef_w = hw ;
		ew->pl_type = hw->pl_type ;
		ew->master_w = hw->master_w ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl2e_02_bomb_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_trail_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_02_main( PLAYER_WORK *ew )
{
	static void ( *shl2e_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_02_bomb_main, shl2e_02_trail_main,
	} ;

	shl2e_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl2e_02_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_bomb_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_02_bomb_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_02_bomb_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_02_bomb_init, shl2e_02_bomb_move, shl2e_02_end, shl2e_02_free,
	} ;

	shl2e_02_bomb_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_02_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -2 ;
	lw->bomb_cnt ++ ;
	ew->atck_no = pl2e_at_bomb ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( shl2e_02_pos_tbl[ew->sub_type * 2] ) ;
	else ew->pos.x -= DOT_X( shl2e_02_pos_tbl[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( shl2e_02_pos_tbl[ew->sub_type * 2 + 1] ) ;
	ew->spd.x = shl2e_02_spd_tbl[pl_w->atck_level * 3 + ew->sub_type * 9] * 1.6666666f / 65536.0f ;
	ew->acc.x = shl2e_02_spd_tbl[pl_w->atck_level * 3 + ew->sub_type * 9 + 1] * 1.6666666f / 65536.0f ;
	ew->spd.y = 0 ;
	ew->acc.y = shl2e_02_spd_tbl[pl_w->atck_level * 3 + ew->sub_type * 9 + 2] * 1.6666666f / 65536.0f ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	pl_char_set( ew, Char_Effect, 12 ) ;
	if( work_rest_cnt >= 4 ){
		shl2e_02_part_set( ew, 1, 0 ) ;
		shl2e_02_part_set( ew, 1, 0 ) ;
		shl2e_02_part_set( ew, 1, 0 ) ;
		shl2e_02_part_set( ew, 1, 0 ) ;
	}
	shl2e_02_bomb_move( ew, pl_w ) ;
}

static void shl2e_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_02_bomb_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_02_bomb_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_02_fall, shl2e_02_burst, shl2e_02_wait,
	} ;

	ew->prio = pl_w->prio ;
	shl2e_02_bomb_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.x * ew->acc.x > 0 ){
		ew->spd.x = 0 ;
		ew->acc.x = 0 ;
	}
	if( pl_w->ground_y >= ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = 12 ;
		return ;
	}
	if( ew->hit_box_no ){
		/* a hit on a body needs no test of its own */
	} else {
		hit_work_set( ew ) ;
		if( !ew->hit_flag ) return ;
	}
	ew->routine_1 ++ ;
	ew->timer0 = 1 ;
}

/*--------------------------------------------------------------*/
static void shl2e_02_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 13 ) ;
		pl_se_req( ew, 73 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )&pl_w->tlocal[0] ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		lw->bomb_cnt -- ;
		shl2e_02_end( ew, pl_w ) ;
	}
}

static void shl2e_02_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_02_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_02_trail_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_02_trail_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_02_trail_init, shl2e_02_trail_move, shl2e_02_end, shl2e_02_free,
	} ;

	shl2e_02_trail_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/* the last four places of the part, kept in spd and acc */
typedef struct SHL2E_02_LOCAL_W {
	Sint16	x[4];
	Sint16	y[4];
} SHL2E_02_LOCAL_W;
#define SHL2E_02_LW( w )	( ( SHL2E_02_LOCAL_W * )&( w )->spd )

/*==============================================================*/
static void shl2e_02_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;
	SHL2E_02_LOCAL_W *lw = SHL2E_02_LW( ew ) ;
	Uint32 i ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	for( i = 0 ; i < 4 ; i ++ ){
		lw->x[i] = ew->pos.x ;
		lw->y[i] = ew->pos.y ;
	}
	pl_char_set( ew, Char_Effect, 12 ) ;
	shl2e_02_trail_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2e_02_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL2E_02_LOCAL_W *lw = SHL2E_02_LW( ew ) ;
	PLAYER_WORK *bw = ew->before_w ;
	PLAYER_WORK *hw = ew->ef_w ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x = lw->x[3] ;
	lw->x[3] = lw->x[2] ;
	lw->x[2] = lw->x[1] ;
	lw->x[1] = lw->x[0] ;
	lw->x[0] = bw->pos.x ;
	ew->pos.y = lw->y[3] ;
	lw->y[3] = lw->y[2] ;
	lw->y[2] = lw->y[1] ;
	lw->y[1] = lw->y[0] ;
	lw->y[0] = bw->pos.y ;
	if( hw->routine_1 == 2 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl2e_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2e_02.c" End of File	*/
