/* Magneto's effect 00 (character 2c's shot 00): his three bolts (types 0 to 2): Magnetic Blast's,
   flying down by his attack level, the medium punch's, straight out, and EM Disruptor's spark. */
#include "player.h"
#include "pl_char.h"
#include "pl2c_def.h"

static void shl2c_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2c_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2c_00_main ;
		ew->type = type ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->ef_no = 0x2c00 ;
	}
	return ew ;
}

static void shl2c_00_jblast( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_00_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_00_disrupt( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2c_00_main( PLAYER_WORK *ew )
{
	static void ( *shl2c_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2c_00_jblast, shl2c_00_punch, shl2c_00_disrupt,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ){
		if( !ew->type ) PL2C_LW( pl_w )->shot_flag = 0 ;
		work_free( ew ) ;
	}
	else shl2c_00_jmp[ew->type]( ew, pl_w ) ;
}

/* the speed of Magnetic Blast's bolt by Magneto's attack level */
static Sint32 shl2c_00_spd_tbl[] = { 6, -7, 10, -11 } ;

/* the animation of EM Disruptor's spark by his attack level */
static Sint16 shl2c_00_char_tbl[] = { 2, 4 } ;

/*==============================================================*/
static void shl2c_00_jblast( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->dm_flag = 0 ;
		ew->size_top = ew->size_bottom = ew->size_front = 48 ;
		ew->size_back = 96 ;
		ew->acc.x = ew->acc.y = 0.0f ;
		ew->spd.x = shl2c_00_spd_tbl[ew->atck_level * 2] ;
		ew->spd.y = shl2c_00_spd_tbl[ew->atck_level * 2 + 1] ;
		if( ew->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( 64.0f ) ;
		else {
			ew->pos.x = pl_w->pos.x + -DOT_X( 64.0f ) ;
			ew->spd.x = -ew->spd.x ;
		}
		ew->pos.y = pl_w->pos.y + 180.0f ;
		ew->atck_no = pl2c_at_jblast + ( ew->atck_level << 1 ) ;
		HIT_RESET( ew ) ;
		ew->hit_stop = 0 ;
		pl_char_set( ew, 20, 0 ) ;
		ew->sub_prio = -8 ;
	}
	ew->prio = pl_w->prio ;
	if( ew->hit_flag ) ew->routine_0 = 3 ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( pl_w->ground_y > ew->pos.y || !scr_in_ck( ew ) ) ew->routine_0 = 3 ;
		else {
			pl_char_move( ew ) ;
			hit_work_set( ew ) ;
		}
	}
}

/*==============================================================*/
static void shl2c_00_punch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->dm_flag = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = -DOT_X( 5.0f ) ;
		if( ew->disp_dir ){
			ew->pos.x = pl_w->pos.x + DOT_X( 64.0f ) ;
			ew->spd.x = -ew->spd.x ;
		}
		else ew->pos.x = pl_w->pos.x + -DOT_X( 64.0f ) ;
		ew->pos.y = pl_w->pos.y ;
		ew->atck_no = ATCK_CROUCH_MP ;
		HIT_RESET( ew ) ;
		ew->hit_stop = 0 ;
		ef_color_set( pl_w, 1, 1, 1 ) ;
		ew->timer0 = 16 ;
		pl_char_set( ew, 20, 1 ) ;
		ew->sub_prio = -8 ;
	}
	ew->prio = pl_w->prio ;
	if( !( -- ew->timer0 ) ){
		if( !ew->type ) PL2C_LW( pl_w )->shot_flag = 0 ;
		work_free( ew ) ;
	}
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		if( pl_char_move( ew ) < 0 ){
			if( !ew->type ) PL2C_LW( pl_w )->shot_flag = 0 ;
			work_free( ew ) ;
		}
		else hit_work_set( ew ) ;
	}
}

/*==============================================================*/
static void shl2c_00_disrupt( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	if( !ew->routine_0 ){
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
		ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
		ew->dm_flag = 0 ;
		ew->hit_stop = 0 ;
		ew->hit_flag = 0 ;
		ew->atck_no = pl2c_at_disrupt + ( ew->atck_level << 1 ) ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 20, shl2c_00_char_tbl[ew->atck_level] ) ;
		ew->sub_prio = -8 ;
		ew->obj_flag = pl_w->char_no.w ;
	}
	ew->prio = pl_w->prio ;
	if( !pl_w->routine_1 && ew->obj_flag == pl_w->char_no.w ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		if( !ew->hit_flag && pl_char_move( ew ) >= 0 ){
			hit_work_set( ew ) ;
			return ;
		}
	}
	if( !ew->type ) PL2C_LW( pl_w )->shot_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2c_00.c" End of File	*/
