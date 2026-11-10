/* Marrow's player effect 00: the manhole of her entrance. Type 0 is its lid, which is thrown up and
   back, falls, bounces once on the ground and fades; type 1 is the hole she leaps out of, which
   fades. */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

static void pls16_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x1600 ;
	}
	return ew ;
}

static void pls16_00_lid_main( PLAYER_WORK *ew );
static void pls16_00_hole_main( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_00_main( PLAYER_WORK *ew )
{
	static void ( *pls16_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_00_lid_main, pls16_00_hole_main,
	} ;

	pls16_00_jmp[ew->type]( ew ) ;
}

static void pls16_00_lid_init( PLAYER_WORK *ew );
static void pls16_00_lid_move( PLAYER_WORK *ew );
static void pls16_00_lid_fall( PLAYER_WORK *ew );
static void pls16_00_lid_land( PLAYER_WORK *ew );
static void pls16_00_lid_fade( PLAYER_WORK *ew );
static void pls16_00_end( PLAYER_WORK *ew );
static void pls16_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_00_lid_main( PLAYER_WORK *ew )
{
	static void ( *pls16_00_lid_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_00_lid_init, pls16_00_lid_move, pls16_00_lid_fall, pls16_00_lid_land,
		pls16_00_lid_fade, pls16_00_end, pls16_00_free,
	} ;

	pls16_00_lid_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_00_lid_init( PLAYER_WORK *ew )
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
	ew->prio = 7 ;
	ew->sub_prio = -2 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 4 ) ;
}

/*--------------------------------------------------------------*/
static void pls16_00_lid_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		if( ew->disp_dir ) ew->spd.x = 2.5f ;
		else ew->spd.x = -DOT_X( 2.0f ) ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 8.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
		ew->disp_dir ^= 1 ;
		ef_char_set( ew, 27, 5 ) ;
		pl_se_req( ew, 47 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_00_lid_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_0 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = DOT_Y( 4.0f ) ;
		pl_voice_req( ew, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_00_lid_land( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < pl_w->ground_y ){
		ew->routine_0 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ef_char_set( ew, 27, 6 ) ;
		pl_voice_req( ew, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_00_lid_fade( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->alpha -= 0.012f ;
	if( ew->alpha <= 0.0f ) pls16_00_end( ew ) ;
}

static void pls16_00_hole_init( PLAYER_WORK *ew );
static void pls16_00_hole_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_00_hole_main( PLAYER_WORK *ew )
{
	static void ( *pls16_00_hole_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_00_hole_init, pls16_00_hole_move, pls16_00_end, pls16_00_free,
	} ;

	pls16_00_hole_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_00_hole_init( PLAYER_WORK *ew )
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
	ew->prio = 7 ;
	ew->sub_prio = 2 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->ground_y ;
	ef_char_set( ew, 27, 19 ) ;
}

/*--------------------------------------------------------------*/
static void pls16_00_hole_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		if( ef_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			ew->alpha = 1.0f ;
		}
	} else {
		if( ( ew->alpha -= 0.05f ) <= 0.0f ) pls16_00_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_00.c" End of File	*/
