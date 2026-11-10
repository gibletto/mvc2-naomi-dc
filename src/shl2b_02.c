/* Sabretooth's effect 02 (character 2b's shot 02): the two bits that go with the end of his Weapon
   X Rush, one for each type, which follow him while it lasts and strike beside it (type 0 only). */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

/* the string of copies of his sprite */
extern int pls2b_03_set( PLAYER_WORK *pl_w, int no );

static void shl2b_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2b_02_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x2b02 ;
		ew->type = no ;
		ew->work_func = shl2b_02_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl2b_02_init( PLAYER_WORK *ew );
static void shl2b_02_move( PLAYER_WORK *ew );
static void shl2b_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl2b_02_main( PLAYER_WORK *ew )
{
	static void ( *shl2b_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl2b_02_init, shl2b_02_move, shl2b_02_free,
	} ;

	shl2b_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl2b_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = pl_w->prio ;
	ew->sub_prio += ew->type ? 4 : -4 ;
	ew->pos = pl_w->pos ;
	ew->pos.y += DOT_Y( 64.0f ) ;
	ew->sub_type = 0 ;
	if( !ew->type ){
		if( pl_w->pl_sp_type == 3 ){
			ew->atck_no = pl2b_at_clawx_last ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl2b_at_weapon_last ;
			HIT_RESET( ew ) ;
		}
		hit_work_set( ew ) ;
	}
	pl_char_set( ew, Char_Effect, ew->type + 18 ) ;
}

/*--------------------------------------------------------------*/
static void shl2b_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->sub_prio += ew->type ? 4 : -4 ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	} else {
		if( !ew->sub_type && ew->hit_flag ){
			pls2b_03_set( pl_w, 0 ) ;
			pls2b_03_set( pl_w, 1 ) ;
			ew->sub_type = 1 ;
		}
		if( !ew->type && !ew->sub_type ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2b_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2b_02.c" End of File	*/
