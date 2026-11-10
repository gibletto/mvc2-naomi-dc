/* The shots of Mega Man's and Roll's Beat Plane (character 1c's shot 09): four kinds, fired by the
   arms (shot 08); the second and fourth fall to the floor and burst. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

static PL1C_SHOT_DATA shl1c_09_data[4] = {
	{ -0x68000, 0, -50, 13, 30, 78 },
	{ -0x48000, 0, -3, 0, 32, 63 },
	{ -0x68000, 0, -50, 13, 30, 74 },
	{ -0x48000, 0, -3, 0, 32, 75 },
} ;

static void shl1c_09_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1c_09_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_09_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void shl1c_09_init( PLAYER_WORK *ew );
static void shl1c_09_move( PLAYER_WORK *ew );
static void shl1c_09_end( PLAYER_WORK *ew );
static void shl1c_09_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_09_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_09_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_09_init, shl1c_09_move, shl1c_09_end, shl1c_09_free,
	} ;

	shl1c_09_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_09_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL1C_SHOT_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c09 ;
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
	if( ew->type & 1 ){
	}
	dp = &shl1c_09_data[ew->type] ;
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
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = dp->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 22, dp->char_no ) ;
	shl1c_09_move( ew ) ;
}

static void shl1c_09_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_09_bomb( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_09_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_09_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_09_shot, shl1c_09_bomb, shl1c_09_shot, shl1c_09_bomb,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1c_09_move_jmp[ew->type]( ew, pl_w ) ;
}

static void shl1c_09_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_09_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_09_shot_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_09_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_09_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_09_shot_init, shl1c_09_shot_fly, shl1c_09_shot_hit,
	} ;

	shl1c_09_shot_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_09_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->size_top = 8 ;
	ew->size_bottom = 8 ;
	ew->size_front = 8 ;
	ew->size_back = 8 ;
	pl_se_req( ew, 70 ) ;
	shl1c_09_shot_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_09_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 31 ) ;
		return ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_09_shot_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void shl1c_09_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_09_bomb_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_09_bomb( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_09_bomb_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_09_bomb_init, shl1c_09_bomb_fly,
	} ;

	shl1c_09_bomb_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_09_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->size_top = 24 ;
	ew->size_bottom = 24 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	shl1c_09_bomb_fly( ew, pl_w ) ;
}

/*==============================================================*/
/*			It bursts on a hit or on the floor					*/
/*==============================================================*/
static void shl1c_09_bomb_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		if( !ew->disp_dir ){
			pos.x = ew->pos.x + 0.0f ;
		} else {
			pos.x = ew->pos.x + -0.0f ;
		}
		pos.y = ew->pos.y + 0.0f ;
		burst_set( ew, &pos, 1, 8 ) ;
		pl_se_req( ew, 73 ) ;
		return ;
	}
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_0 ++ ;
		ew->pos.y = pl_w->ground_y ;
		if( !ew->disp_dir ){
			pos.x = ew->pos.x + 0.0f ;
		} else {
			pos.x = ew->pos.x + -0.0f ;
		}
		pos.y = ew->pos.y + 0.0f ;
		burst_set( ew, &pos, 1, 8 ) ;
		pl_se_req( ew, 73 ) ;
		return ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_09_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_09_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_09.c" End of File	*/
