/* Captain Commando's player effect 02: the copy of the thrower that shakes beside the player he
   throws, at random spots, while the throw's hit lasts. */
#include "player.h"
#include "pl_char.h"
#include "pl38_def.h"

static void pls38_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls38_02_set( PLAYER_WORK *pl_w, Sint16 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls38_02_main ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->dm_hit_w->pl_type ;
		ew->ef_no = 0x3802 ;
	}
	return ew ;
}

/*==============================================================*/
static void pls38_02_main( PLAYER_WORK *ew )
{
	static Float pls38_02_x_tbl[8] = {
		DOT_X( 13.0f ), -1.6666666f, DOT_X( 28.0f ), DOT_X( 2.0f ), DOT_X( 2.0f ), -DOT_X( 14.0f ), DOT_X( 14.0f ), -DOT_X( 22.0f ),
	} ;
	static Float pls38_02_y_tbl[8] = {
		DOT_Y( 80.0f ), DOT_Y( 73.0f ), DOT_Y( 67.0f ), DOT_Y( 64.0f ), DOT_Y( 47.0f ), DOT_Y( 39.0f ), DOT_Y( 25.0f ), 30.0f,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = pl_w->dm_hit_w ;
	Float x ;

	if( ew->routine_0 >= 2 ){
		work_free( ew ) ;
		return ;
	}
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		CHAR_PART_COPY( ew, em ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = em->pl_no ;
		ew->pl_type = em->pl_type ;
		ew->scale.x = em->scale.x ;
		ew->scale.y = em->scale.y ;
		ew->atck_level = em->atck_level ;
		ew->work_no = em->work_no ;
		ew->col_cnt = em->col_cnt ;
		ew->scale = em->scale ;
		ew->prio = em->prio ;
		pl_char_set( ew, 25, 4 ) ;
		ew->prio = em->prio ;
		ew->sub_prio = -8 ;
	}
	ew->prio = em->prio ;
	pl_char_move( ew ) ;
	if( ew->cg_type ) hit_se_req( em, 4 ) ;
	if( pl_w->routine_1 != PL_CAUGHT || pl_w->catch_type != 19 || pl_w->routine_2 > 1 ){
		ew->routine_0 ++ ;
	} else if( !ew->routine_1 ){
		x = pls38_02_x_tbl[Rnd() & 7] ;
		if( pl_w->disp_dir ) x = -x ;
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->pos.y + pls38_02_y_tbl[Rnd() & 7] ;
		ew->routine_1 ++ ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls38_02.c" End of File	*/
