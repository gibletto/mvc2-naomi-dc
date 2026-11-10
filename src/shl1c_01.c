/* Mega Man's and Roll's charged buster shots (character 1c's shot 01): three kinds that go on
   through what they hit, a set number of hits. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

static void shl1c_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1c_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->obj_flag = lw->charge_lv ;
	}
	return ew ;
}

static PL1C_SHOT_DATA shl1c_01_data[3] = {
	{ -6 << 16, 0, -24, 46, 2, 49 },
	{ -6 << 16, 0, -24, 34, 2, 49 },
	{ -6 << 16, 0, -29, 68, 2, 61 },
} ;

static void shl1c_01_init( PLAYER_WORK *ew );
static void shl1c_01_move( PLAYER_WORK *ew );
static void shl1c_01_end( PLAYER_WORK *ew );
static void shl1c_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_01_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_01_init, shl1c_01_move, shl1c_01_end, shl1c_01_free,
	} ;

	shl1c_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1C_SHOT_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c01 ;
	ew->timer0 = -1 ;
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
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	if( pl_w->pl_type == PL_ROLL ){
		ew->col_flag = 1 ;
		ew->col_no -= 2 ;
	}
	dp = &shl1c_01_data[ew->type] ;
	ew->pos = pl_w->pos ;
	if( !ew->disp_dir ){
		ew->pos.x += DOT_X( dp->x ) ;
		ew->spd.x = dp->spd_x * 1.6666666f / 65536.0f ;
	} else {
		ew->pos.x += -DOT_X( dp->x ) ;
		ew->spd.x = -( dp->spd_x * 1.6666666f / 65536.0f ) ;
	}
	ew->pos.y += DOT_Y( dp->y ) ;
	ew->spd.y = dp->spd_y * 2.142857f / 65536.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = dp->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dp->char_no ) ;
	pl_voice_req( pl_w, 38 ) ;
	shl1c_01_move( ew ) ;
}

static void shl1c_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_01_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_01_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_01_fly, shl1c_01_hit,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->hit_stop ){
		ew->hit_stop -- ;
		return ;
	}
	shl1c_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			The hits left are in obj_flag; a hit on a body		*/
/*			that can't take it costs three						*/
/*==============================================================*/
static void shl1c_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 no ;
	PL1C_SHOT_DATA *dp ;

	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		if( -- ew->obj_flag ){
			dp = &shl1c_01_data[ew->type] ;
			no = dp->atck_no ;
			if( ew->timer0 ++ & 3 ) no ++ ;
			ew->atck_no = no ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		} else {
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 3 ) ;
			return ;
		}
	} else if( ew->hit_box_no ){
		if( !ew->hit_data_adrs || !( ew->hit_data_adrs->atck_kind & A_HYPER ) ){
			if( !-- ew->obj_flag ){
				ew->routine_1 ++ ;
				pl_char_set( ew, Char_Effect, 3 ) ;
				return ;
			}
		} else if( ( ew->obj_flag = ew->obj_flag - 3 ) <= 0 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 3 ) ;
			return ;
		}
		dp = &shl1c_01_data[ew->type] ;
		no = dp->atck_no ;
		if( ew->timer0 ++ & 3 ) no ++ ;
		ew->atck_no = no ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_01_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_01.c" End of File	*/
