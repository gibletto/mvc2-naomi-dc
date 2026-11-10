/* Anakaris's coffin (character 04's shot 00): it falls from the top of the screen, shakes the
   screen where it lands and bounces back up out of sight. */
#include "player.h"
#include "pl_char.h"
#include "pl04_def.h"

static void shl04_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl04_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl04_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x400 ;
	}
	return ew ;
}

static void shl04_00_init( PLAYER_WORK *ew );
static void shl04_00_move( PLAYER_WORK *ew );
static void shl04_00_end( PLAYER_WORK *ew );
static void shl04_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_00_main( PLAYER_WORK *ew )
{
	static void ( *shl04_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_00_init, shl04_00_move, shl04_00_end, shl04_00_free,
	} ;

	shl04_00_jmp[ew->routine_0]( ew ) ;
}

static void shl04_00_coffin_init( PLAYER_WORK *ew );
static void shl04_00_coffins_init( PLAYER_WORK *ew );
static void shl04_00_magic_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_00_init( PLAYER_WORK *ew )
{
	static void ( *shl04_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_00_coffin_init, shl04_00_coffins_init, shl04_00_magic_init,
	} ;

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
	ew->disp_flag = 1 ;
	ew->pos.y = scr_work.scr_top ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -DOT_Y( 8.0f ) ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->prio = 11 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	shl04_00_init_jmp[ew->type]( ew ) ;
	shl04_00_move( ew ) ;
}

/*==============================================================*/
static void shl04_00_coffin_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->team_type == 3 ){
		ew->atck_no = pl04_at_jcoffin ;
		HIT_RESET( ew ) ;
	}
	else {
		ew->atck_no = pl04_at_coffin ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 21, 18 ) ;
}

/*==============================================================*/
static void shl04_00_coffins_init( PLAYER_WORK *ew )
{
	ew->atck_no = pl04_at_coffins ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 22, 10 ) ;
}

/*==============================================================*/
static void shl04_00_magic_init( PLAYER_WORK *ew )
{
	ew->disp_dir ^= 1 ;
	ew->atck_no = pl04_at_magic_coffin ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 22, 14 ) ;
}

static void shl04_00_fall( PLAYER_WORK *ew );
static void shl04_00_land( PLAYER_WORK *ew );
static void shl04_00_leave( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl04_00_move( PLAYER_WORK *ew )
{
	static void ( *shl04_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_00_fall, shl04_00_land, shl04_00_leave,
	} ;

	shl04_00_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl04_00_fall( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_char_move( ew ) ;
		pl_se_req( ew, ew->type != 2 ? 47 : 49 ) ;
		return ;
	}
	if( ew->type != 2 && ew->hit_flag ){
		ew->routine_1 ++ ;
		pl_char_move( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_00_land( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.y = DOT_Y( 8.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl04_00_leave( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > scr_work.scr_top ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl04_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl04_00.c" End of File	*/
