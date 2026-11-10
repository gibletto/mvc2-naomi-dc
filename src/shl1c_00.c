/* Mega Man's and Roll's buster shots (character 1c's shot 00): five kinds, picked by the setter,
   that fly straight until they hit or leave the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

static void shl1c_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1c_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static PL1C_SHOT_DATA shl1c_00_data[5] = {
	{ -6 << 16, 0, -24, 46, 0, 48 },
	{ -6 << 16, 0, -24, 34, 0, 48 },
	{ -6 << 16, 0, -29, 68, 0, 60 },
	{ -6 << 16, 0, -35, 39, 2, 49 },
	{ -6 << 16, 0, -41, 47, 2, 61 },
} ;

static void shl1c_00_init( PLAYER_WORK *ew );
static void shl1c_00_move( PLAYER_WORK *ew );
static void shl1c_00_end( PLAYER_WORK *ew );
static void shl1c_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_00_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_00_init, shl1c_00_move, shl1c_00_end, shl1c_00_free,
	} ;

	shl1c_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1C_SHOT_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c00 ;
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
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	if( pl_w->pl_type == PL_ROLL ){
		ew->col_flag = 1 ;
		ew->col_no -= 2 ;
	}
	dp = &shl1c_00_data[ew->type] ;
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
	pl_voice_req( pl_w, 37 ) ;
	shl1c_00_move( ew ) ;
}

static void shl1c_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_00_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_00_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_00_fly, shl1c_00_hit,
	} ;

	shl1c_00_move_jmp[ew->routine_1]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl1c_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, pl_w->pl_type == PL_MEGAMAN ? 1 : 3 ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_00_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_00.c" End of File	*/
