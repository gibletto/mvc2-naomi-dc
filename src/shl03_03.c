/* Morrigan's effect 03 (character 03's shot 03): her double in the throw of a hyper combo, which
   appears on the far side of the opponent she caught, mirrors her and flies off when it is done. */
#include "player.h"
#include "pl_char.h"
#include "pl03_def.h"

static void shl03_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl03_03_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl03_03_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->ef_no = 0x303 ;
	}
	return ew ;
}

static void shl03_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_03_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It ends as soon as Morrigan leaves the hyper		*/
/*			combo												*/
/*==============================================================*/
static void shl03_03_main( PLAYER_WORK *ew )
{
	static void ( *shl03_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_03_init, shl03_03_move, shl03_03_end, shl03_03_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 <= 1 && pl_w->act_st != N_SUPER ) ew->routine_0 = 2 ;
	shl03_03_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_03_illusion_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->double_w = ew ;
	BODY_SIZE_SET( ew, 0x80403030 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->disp_dir = ew->disp_dir ^ 1 ;
	shl03_03_pos_set( ew, pl_w ) ;
	pl_char_set( ew, 22, 8 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	shl03_03_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_03_jillusion_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->double_w = ew ;
	BODY_SIZE_SET( ew, 0x80403030 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->disp_dir = ew->disp_dir ^ 1 ;
	shl03_03_pos_set( ew, pl_w ) ;
	pl_char_set( ew, 22, 9 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	shl03_03_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_03_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_03_illusion_init, shl03_03_jillusion_init,
	} ;

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
	shl03_03_init_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_03_illusion_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	shl03_03_pos_set( ew, pl_w ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 1 ;
		lw->double_flag = 1 ;
		pl_char_set( ew, 22, 16 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_illusion_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !--ew->timer0 ) ew->routine_1 ++ ;
	shl03_03_pos_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_03_illusion_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl03_03_pos_set( ew, pl_w ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 16.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( ew, 22, 13 ) ;
		return ;
	}
	if( ew->cg_type ){
		if( ew->cg_type == 62 ){
			pls03_04_double_set( ew, 1 ) ;
			ew->cg_type = 0 ;
		}
		ew->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_illusion_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > pl_w->ground_y + DOT_Y( 256.0f ) ){
		ew->pos.y = pl_w->ground_y + DOT_Y( 256.0f ) ;
		ew->routine_1 ++ ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			Until Morrigan says									*/
/*==============================================================*/
static void shl03_03_illusion_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( lw->drop_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 17 ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_03_illusion_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		if( ew->cg_type == 63 ){
			ew->routine_1 ++ ;
			pls03_04_double_set( ew, 3 ) ;
			pl_char_set( ew, 22, 18 ) ;
		}
		else ew->cg_type = 0 ;
	}
}

/*==============================================================*/
/* differs from shl03_03_jillusion_land in its first store, acc.y where that one clears acc.x */
static void shl03_03_illusion_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->timer0 = 30 ;
		ew->routine_1 ++ ;
		ew->spd.x = DOT_X( 2.0f ) ;
		ew->acc.y = 0.0f ;
		ew->spd.y = -DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.5f ) ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		pl_char_set( ew, 22, 19 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_illusion_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl03_03_illusion_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_03_illusion_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_03_illusion_appear, shl03_03_illusion_wait, shl03_03_illusion_hit, shl03_03_illusion_rise,
		shl03_03_illusion_hold, shl03_03_illusion_drop, shl03_03_illusion_land, shl03_03_illusion_leave,
	} ;

	shl03_03_illusion_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_03_jillusion_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 1 ;
		lw->double_flag = 1 ;
		pl_char_set( ew, 22, 22 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_jillusion_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !--ew->timer0 ) ew->routine_1 ++ ;
	shl03_03_pos_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_03_jillusion_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer1 = ew->timer0 = 5 ;
		pl_char_set( ew, 22, 23 ) ;
		return ;
	}
	if( ew->cg_type ){
		if( ew->cg_type == 66 ){
			ew->cg_type = 0 ;
			pls03_04_double_set( ew, 1 ) ;
		}
		ew->cg_type = 0 ;
		shl03_03_pos_set( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_jillusion_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl03_03_pos_set( ew, pl_w ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->disp_dir = ew->disp_dir ^ 1 ;
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 24 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_jillusion_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = DOT_X( 2.0f ) ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.5f ) ;
		if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		pl_char_set( ew, 22, 19 ) ;
	}
	else if( ew->cg_type ){
		ew->cg_type = 0 ;
		pls03_04_double_set( ew, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl03_03_jillusion_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void shl03_03_jillusion_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_03_jillusion_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_03_jillusion_appear, shl03_03_jillusion_wait, shl03_03_jillusion_hit, shl03_03_jillusion_turn,
		shl03_03_jillusion_land, shl03_03_jillusion_leave,
	} ;

	shl03_03_jillusion_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_03_illusion_move, shl03_03_jillusion_move,
	} ;

	if( ew->hit_stop ){
		ew->hit_stop -- ;
		return ;
	}
	shl03_03_move_jmp[ew->type]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl03_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	lw->double_flag = 1 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			On the far side of the opponent from Morrigan,		*/
/*			at her height										*/
/*==============================================================*/
static void shl03_03_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = lw->em_w ;

	ew->pos.x = em->pos.x - pl_w->pos.x + em->pos.x ;
	ew->pos.y = pl_w->pos.y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl03_03.c" End of File	*/
