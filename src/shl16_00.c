/* Marrow's effect 00 (character 16's shot 00): the thrown bone of her Bone-merang, four kinds by
   her stance and strength. It flies, can strike several times and fades. Her tlocal marks that one
   is out. */
#include "player.h"
#include "pl_char.h"
#include "pl16_def.h"

static void shl16_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl16_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl16_00_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1600 ;
	}
	return ew ;
}

static void shl16_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl16_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl16_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl16_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl16_00_main( PLAYER_WORK *ew )
{
	static void ( *shl16_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl16_00_init, shl16_00_move, shl16_00_end, shl16_00_free,
	} ;

	shl16_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
/*			Type 0 low and light, 1 low and heavy, 2 high		*/
/*			and light, 3 high and heavy							*/
/*==============================================================*/
static void shl16_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )pl_w->tlocal ;

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
	ew->size_top = 64 ;
	ew->size_bottom = 64 ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	ew->prio = 11 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !ew->atck_level ){
			if( ew->disp_dir ) ew->pos.x += 95.0f ;
			else ew->pos.x -= 95.0f ;
			ew->pos.y += DOT_Y( 93.0f ) ;
			ew->spd.x = -DOT_X( 10.0f ) ;
			ew->spd.y = 0 ;
			ew->type = 2 ;
		}
		else {
			if( ew->disp_dir ) ew->pos.x += DOT_X( 40.0f ) ;
			else ew->pos.x -= DOT_X( 40.0f ) ;
			ew->pos.y += 180.0f ;
			ew->spd.x = -15.0f ;
			ew->spd.y = -DOT_Y( 6.0f ) ;
			ew->rot[2] = 0x1000 ;
			ew->type = 3 ;
		}
	}
	else {
		if( !ew->atck_level ){
			if( ew->disp_dir ) ew->pos.x += DOT_X( 31.0f ) ;
			else ew->pos.x -= DOT_X( 31.0f ) ;
			ew->pos.y += DOT_Y( 48.0f ) ;
			ew->spd.x = -DOT_X( 10.0f ) ;
			ew->spd.y = -DOT_Y( 8.0f ) ;
			ew->acc.y = DOT_Y( 1.125f ) ;
			ew->type = 0 ;
		}
		else {
			if( ew->disp_dir ) ew->pos.x += DOT_X( 58.0f ) ;
			else ew->pos.x -= DOT_X( 58.0f ) ;
			ew->pos.y += DOT_Y( 54.0f ) ;
			ew->spd.x = -15.0f ;
			ew->spd.y = -DOT_Y( 6.0f ) ;
			ew->acc.y = 2.142857f ;
			ew->rot[2] = 0xee00 ;
			ew->type = 1 ;
		}
	}
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	lw->shot_flag = 1 ;
	ew->timer0 = 16 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	ew->timer1 = 0 ;
	ew->sub_type = 3 ;
	ew->alpha = 1.0f ;
	ew->atck_no = pl16_at_bone ;
	HIT_RESET( ew ) ;
	ew->atck_attr = ATR_BACK_WORK ;
	shl16_00_move( ew, pl_w ) ;
}

static void shl16_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl16_00_strike( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl16_00_fade( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl16_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl16_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl16_00_fly, shl16_00_strike, shl16_00_fade,
	} ;
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !scr_in_x_ck( ew ) ){
		shl16_00_end( ew, pl_w ) ;
		lw->shot_flag = 0 ;
		return ;
	}
	shl16_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			The high kinds stop on the floor					*/
/*==============================================================*/
static void shl16_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )pl_w->tlocal ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->type & 2 ){
		if( ew->pos.y <= pl_w->ground_y ){
			shl16_00_end( ew, pl_w ) ;
			lw->shot_flag = 0 ;
			return ;
		}
	}
	if( -- ew->timer0 <= 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
		ew->atck_flag = HIT_ON | HIT_OBJ ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		ew->scale.x = 1.25f ;
		ew->scale.y = 1.25f ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			It strikes until it has struck three times or		*/
/*			its time is out										*/
/*==============================================================*/
static void shl16_00_strike( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( -- ew->timer0 <= 0 || ew->hit_box_no ){
		ew->routine_1 ++ ;
		ew->spd.x = 0 ;
		ew->spd.y = 0 ;
		ew->acc.x = 0 ;
		ew->acc.y = 0 ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		return ;
	}
	if( ew->hit_flag ){
		ew->timer1 ++ ;
		if( ew->timer1 >= 8 ){
			ew->timer1 = 0 ;
			if( ! -- ew->sub_type ){
				ew->routine_1 ++ ;
				ew->spd.x = 0 ;
				ew->spd.y = 0 ;
				ew->acc.x = 0 ;
				ew->acc.y = 0 ;
				ew->acc.y = -DOT_Y( 0.375f ) ;
				return ;
			}
			ew->atck_no = pl16_at_bone ;
			HIT_RESET( ew ) ;
			ew->atck_attr = ATR_BACK_WORK ;
		}
	}
	hit_work_set( ew ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl16_00_fade( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL16_LOCAL_W *lw = ( PL16_LOCAL_W * )pl_w->tlocal ;

	if( !ew->routine_2 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y <= pl_w->ground_y ) ew->routine_2 ++ ;
	}
	else {
		ew->alpha -= 0.1f ;
		if( ew->alpha <= 0 ){
			ew->routine_0 ++ ;
			lw->shot_flag = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl16_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl16_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl16_00.c" End of File	*/
