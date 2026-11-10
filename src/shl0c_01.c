/* Spider-Man's Web Throw (character 0c's shot 01): a line of web that leaves his hand while he
   holds the throw, and grabs the opponent it touches, or flies off as a ball when it misses. */
#include "player.h"
#include "pl_char.h"
#include "pl0c_def.h"

extern int pls0c_02_set( PLAYER_WORK *pl_w, int no, int lvl, int frame ) ;

/* the ball's body size and speed (16.16, x and y), by strength */
Uint8 shl0c_01_size[3][4] = {
	{ 0x10, 0x10, 0x10, 0xc0 }, { 0x70, 0x10, 0x10, 0x90 }, { 0xb0, 0x10, 0x10, 0x10 },
} ;
Sint32 shl0c_01_spd[6] = {
	-0x80000, 0, -0x60000, 0x4c000, 0, 0x80000,
} ;

/* the animation: the ball by strength, then the line by strength */
Uint8 shl0c_01_char_tbl[6] = { 45, 55, 55, 65, 75, 75 } ;

static void shl0c_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0c_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0c_01_main ;
		ew->ef_no = 0x0c01 ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl0c_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0c_01_main( PLAYER_WORK *ew )
{
	static void ( *shl0c_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0c_01_init, shl0c_01_move, shl0c_01_free,
	} ;

	shl0c_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl0c_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

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
	ew->routine_0 ++ ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x + lw->hand_x ;
	ew->pos.y = pl_w->pos.y + lw->hand_y ;
	ew->atck_no = ew->atck_level + 66 ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, ew->atck_level + 36 ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
}

static void shl0c_01_line( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_01_ball( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_01_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0c_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0c_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0c_01_line, shl0c_01_ball, shl0c_01_vanish,
	} ;

	shl0c_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0c_01_line( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	Uint8 *sz ;
	Sint32 *dt ;
	int no ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SPECIAL ){
		ew->pos.x = pl_w->pos.x + lw->hand_x ;
		ew->pos.y = pl_w->pos.y + lw->hand_y ;
		if( pl_char_move( ew ) >= 0 ){
			if( ew->hit_flag ){
				em = ew->em_w ;
				if( pl_sp_catch_ck( ew ) && !( lw->web_flag[em->work_no >> em->pl_no >> 1] & WEB_HOLD ) ){
					pls0c_02_set( pl_w, 12, ew->atck_level, ew->cg_type ) ;
					lw->line_st = 1 ;
					pl_w->catch_ng_timer = 32 ;
					pl_w->dm_flag = 0 ;
					pl_w->catch_no = 0xc4 ;
					pl_w->catch_grip_old = -1 ;
					pl_ef_catch_set( ew, em ) ;
					ew->routine_0 = 2 ;
					ew->disp_flag = 0 ;
					em->wall_ng_timer = 2 ;
					return ;
				}
				ew->routine_1 = 2 ;
				lw->line_st = -1 ;
				no = shl0c_01_char_tbl[ew->atck_level + 3] + ew->cg_type ;
				pl_char_set( ew, Char_Effect, no ) ;
				return ;
			}
			hit_work_set( ew ) ;
			return ;
		}
	}
	ew->routine_1 ++ ;
	lw->line_st = -1 ;
	sz = shl0c_01_size[ew->atck_level] ;
	ew->size_top = *sz++ ;
	ew->size_bottom = *sz++ ;
	ew->size_front = *sz++ ;
	ew->size_back = *sz ;
	dt = &shl0c_01_spd[ew->atck_level * 2] ;
	ew->spd.x = *dt++ * 1.6666666f / 65536.0f ;
	ew->spd.y = *dt * 1.6666666f / 65536.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	no = shl0c_01_char_tbl[ew->atck_level] + ew->cg_type ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*--------------------------------------------------------------*/
static void shl0c_01_ball( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )&pl_w->tlocal[0] ;
	int no ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		ew->routine_1 = 2 ;
		lw->line_st = -1 ;
		no = shl0c_01_char_tbl[ew->atck_level + 3] + ew->cg_type ;
		pl_char_set( ew, Char_Effect, no ) ;
		return ;
	}
	hit_work_set( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void shl0c_01_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0c_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0c_01.c" End of File	*/
