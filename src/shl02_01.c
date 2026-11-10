/* Guile's Sonic Hurricane (character 02's shot 01): a boom that grows in front of him and hits every
   six frames while he holds it, with a second layer drawn on it. */
#include "player.h"
#include "pl_char.h"
#include "pl02_def.h"

static void shl02_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl02_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl02_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x201 ;
		ew->type = no ;
	}
	return ew ;
}

/*==============================================================*/
/*			A work that rides on the effect hw					*/
/*==============================================================*/
static void *shl02_01_part_set( PLAYER_WORK *hw, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl02_01_main ;
		ew->master_w = hw->master_w ;
		ew->ef_w = hw ;
		ew->ef_no = 0x201 ;
		ew->type = no ;
	}
	return ew ;
}

static void shl02_01_boom_main( PLAYER_WORK *ew );
static void shl02_01_part_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_01_main( register PLAYER_WORK *ew )
{
	static void ( *shl02_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_01_boom_main, shl02_01_part_main,
	} ;

	shl02_01_jmp[ew->type]( ew ) ;
}

static void shl02_01_init( PLAYER_WORK *ew );
static void shl02_01_move( PLAYER_WORK *ew );
static void shl02_01_fade( PLAYER_WORK *ew );
static void shl02_01_end( PLAYER_WORK *ew );
static void shl02_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_01_boom_main( PLAYER_WORK *ew )
{
	static void ( *shl02_01_boom_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_01_init, shl02_01_move, shl02_01_fade, shl02_01_end, shl02_01_free,
	} ;

	shl02_01_boom_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL02_LOCAL_W *lw = ( PL02_LOCAL_W * )pl_w->tlocal ;

	lw->hurricane_flag = 1 ;
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl02_at_hurricane ;
	HIT_RESET( ew ) ;
	ew->atck_attr = ATR_BACK_POS ;
	ew->timer1 = 0 ;
	ew->sub_type = 19 ;
	ew->pos.x = ew->master_w->pos.x ;
	if( !ew->disp_dir ) ew->pos.x -= 120.0f ;
	else ew->pos.x += 120.0f ;
	ew->pos.y = ew->master_w->pos.y + 120.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->scale.x = 0.1f ;
	ew->scale.y = 0.1f ;
	ew->timer0 = 130 ;
	ew->size_front = 0xb0 ;
	ew->size_back = 0xb0 ;
	ew->prio = 14 ;
	pl_char_set( ew, Char_Effect, 4 ) ;
	shl02_01_part_set( ew, 1 ) ;
}

/*--------------------------------------------------------------*/
static void shl02_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 = 3 ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	if( !ew->disp_dir ) ew->pos.x -= 120.0f ;
	else ew->pos.x += 120.0f ;
	ew->pos.y = ew->master_w->pos.y + 120.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->scale.x < 1.0f ){
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	} else {
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	ew->timer1 ++ ;
	if( ew->timer1 == 6 ){
		ew->sub_type -- ;
		if( ew->sub_type == 0 ){
			ew->atck_no = pl02_at_hurricane_end ;
			HIT_RESET( ew ) ;
			ew->routine_0 += 2 ;
			ew->timer0 = 2 ;
		} else {
			if( !( ew->sub_type & 3 ) ){
				ew->atck_no = pl02_at_hurricane_3 ;
				HIT_RESET( ew ) ;
			} else {
				ew->atck_no = pl02_at_hurricane_2 ;
				HIT_RESET( ew ) ;
			}
			if( ew->master_w->pos.x > ew->pos.x ){
				if( ew->master_w->target_w->pos.x <= ew->pos.x ) ew->atck_attr = ATR_BACK_POS ;
			} else {
				if( ew->master_w->target_w->pos.x >= ew->pos.x ) ew->atck_attr = ATR_BACK_POS ;
			}
		}
		ew->timer1 = 0 ;
	}
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->alpha = 1.0f ;
		return ;
	}
	hit_work_set( ew ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_01_fade( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 = 3 ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	if( !ew->disp_dir ) ew->pos.x -= 120.0f ;
	else ew->pos.x += 120.0f ;
	ew->pos.y = ew->master_w->pos.y + 120.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->alpha -= 0.05f ;
	if( ew->alpha <= 0.0f ) ew->routine_0 = 3 ;
	pl_char_move( ew ) ;
}

static void shl02_01_part_init( PLAYER_WORK *ew );
static void shl02_01_part_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl02_01_part_main( PLAYER_WORK *ew )
{
	static void ( *shl02_01_part_jmp[] )( PLAYER_WORK *ew ) = {
		shl02_01_part_init, shl02_01_part_move, shl02_01_end, shl02_01_free,
	} ;

	shl02_01_part_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl02_01_part_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->scale.x = 0.1f ;
	ew->scale.y = 0.1f ;
	ew->size_front = 0xb0 ;
	ew->size_back = 0xb0 ;
	ew->prio = 10 ;
	pl_char_set( ew, Char_Effect, 5 ) ;
	ew->pos = ew->ef_w->pos ;
}

/*--------------------------------------------------------------*/
static void shl02_01_part_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL02_LOCAL_W *lw = ( PL02_LOCAL_W * )pl_w->tlocal ;

	if( ew->alpha <= 0.0f ) ew->routine_0 ++ ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !lw->hurricane_flag ) ew->routine_0 ++ ;
	ew->pos = ew->ef_w->pos ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->alpha = ew->ef_w->alpha ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl02_01_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL02_LOCAL_W *lw = ( PL02_LOCAL_W * )pl_w->tlocal ;

	ew->routine_0 ++ ;
	lw->hurricane_flag = 0 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl02_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl02_01.c" End of File	*/
