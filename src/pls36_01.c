/* Thanos's player effect 01: the shadow figures of Gauntlet Space, a pair that stays beside
   him with the two works that fly out of it, shown in his animation group 22. */
#include "player.h"
#include "pl_char.h"
#include "pl36_def.h"

/* the effect's own work, from obj_flag: the player it is for */
typedef struct PLS36_01_LOCAL_W {
	PLAYER_WORK	*w;
} PLS36_01_LOCAL_W;
#define PLS36_01_LW( w )	( ( PLS36_01_LOCAL_W * )&( w )->obj_flag )

static void pls36_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls36_01_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *ew_w ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->be_flag = 1 ;
		ew->work_func = pls36_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x3601 ;
		PLS36_01_LW( ew )->w = pl_w ;
		ew->timer0 = 0 ;
	}
	ew_w = ew ;
	if( ( ew = work_get( ew_w, W_PLS, LIST_BEHIND ) ) ){
		ew->be_flag = 1 ;
		ew->work_func = pls36_01_main ;
		ew->master_w = ew_w ;
		ew->pl_type = ew_w->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 1 ;
		ew->ef_no = 0x3601 ;
		PLS36_01_LW( ew )->w = pl_w ;
		ew->timer0 = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->be_flag = 1 ;
		ew->work_func = pls36_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 1 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x3601 ;
		PLS36_01_LW( ew )->w = pl_w ;
		ew->timer0 = 60 ;
	}
	return ew ;
}

static void pls36_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls36_01_main( PLAYER_WORK *ew )
{
	static void ( *pls36_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls36_01_wait, pls36_01_move, pls36_01_end, pls36_01_free,
	} ;

	pls36_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void pls36_01_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_01_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It waits its frames, then takes its looks from		*/
/*			the player											*/
/*==============================================================*/
static void pls36_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	if( ew->timer0 ){
		ew->timer0 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, PLS36_01_LW( ew )->w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = PLS36_01_LW( ew )->w->pl_no ;
	ew->pl_type = PLS36_01_LW( ew )->w->pl_type ;
	ew->scale.x = PLS36_01_LW( ew )->w->scale.x ;
	ew->scale.y = PLS36_01_LW( ew )->w->scale.y ;
	ew->atck_level = PLS36_01_LW( ew )->w->atck_level ;
	ew->work_no = PLS36_01_LW( ew )->w->work_no ;
	ew->col_cnt = PLS36_01_LW( ew )->w->col_cnt ;
	ew->scale = PLS36_01_LW( ew )->w->scale ;
	ew->prio = PLS36_01_LW( ew )->w->prio ;
	ew->disp_flag = 1 ;
	ew->dir = 0 ;
	if( !ew->type ) pls36_01_init0( ew, pl_w ) ;
	else pls36_01_init1( ew, pl_w ) ;
}

/*==============================================================*/
static void pls36_01_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w = PLS36_01_LW( ew )->w ;
	Sint16 x ;
	Uint8 no ;

	ew->prio = 7 ;
	x = -64 ;
	if( w->disp_dir ) x = 64 ;
	ew->pos.x = w->pos.x + DOT_X( x ) ;
	ew->pos.y = w->pos.y + DOT_Y( 144.0f ) ;
	ew->timer0 = 140 ;
	ew->timer1 = 0 ;
	no = 9 ;
	if( ew->sub_type ) no = 8 ;
	pl_char_set( ew, 22, no ) ;
}

/*==============================================================*/
static void pls36_01_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	Uint8 prio ;

	prio = 7 ;
	if( ew->sub_type ) prio = 0 ;
	ew->prio = prio ;
	em_w = PLS36_01_LW( ew )->w->target_w ;
	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->ground_y + DOT_Y( 176.0f ) ;
	ew->timer0 = 130 ;
	ew->timer1 = 0x102 ;
	if( pl_w->act_st != N_SUPER || pl_w->routine_1 ) ew->disp_flag = 0 ;
	pl_char_set( ew, 22, 71 ) ;
}

static void pls36_01_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_01_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls36_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->type ) pls36_01_move0( ew, pl_w ) ;
	else pls36_01_move1( ew, pl_w ) ;
}

/*==============================================================*/
static void pls36_01_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->sub_type ){
		if( pl_w->ground_y + DOT_Y( 176.0f ) > ew->pos.y ) ew->pos.y += DOT_Y( 2.0f ) ;
		if( !ew->routine_1 ){
			ew->disp_flag ^= 1 ;
			pl_char_move( ew ) ;
			if( ew->timer0 == 30 ) pl_char_set( ew, 22, 19 ) ;
			if( ew->timer0 > 80 && ew->cg_type ){
				if( ( ew->dir += 32 ) & 0x80 ){
					ew->timer1 ^= 1 ;
					shl36_02_set( ew, ew->timer1 ) ;
				}
			}
			if( pl_w->act_st == N_SUPER && !pl_w->routine_1 ){
				if( -- ew->timer0 >= 0 ) return ;
			}
			ew->routine_1 ++ ;
			ew->disp_flag |= 2 ;
			pl_char_set( ew, 22, 18 ) ;
		} else {
			if( pl_char_move( ew ) < 0 ){
				ew->routine_0 ++ ;
				ew->disp_flag = 0 ;
			}
		}
	} else {
		if( pl_w->routine_1 || !pl_w->be_flag ) pls36_01_free( ew, pl_w ) ;
		else {
			ew->pos = pl_w->pos ;
			pl_char_move( ew ) ;
		}
	}
}

/*==============================================================*/
static void pls36_01_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 prio ;

	if( ew->timer0 ){
		pl_char_move( ew ) ;
		prio = 7 ;
		if( ew->cg_type ) prio = 0 ;
		ew->prio = prio ;
		if( pl_w->act_st == N_SUPER && !pl_w->routine_1 ){
			if( ew->timer0 > 60 && ew->timer0 < 120 ){
				if( ( ew->dir += 32 ) & 0x80 ){
					ew->timer1 ^= 1 ;
					shl36_02_set( ew, ew->timer1 ) ;
				}
			}
			if( -- ew->timer0 > 0 ) return ;
		}
		ew->timer0 = 0 ;
		pl_char_set( ew, 22, 72 ) ;
	} else {
		if( pl_char_move( ew ) < 0 ) pls36_01_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls36_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls36_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->be_flag = 0 ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls36_01.c" End of File	*/
