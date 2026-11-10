/* The puffs on the player a shot or hit lands on (kind 7 works, three made by puff_ef_set for the
   effects of many characters): each follows the player at his chest, waits its turn, swells,
   settles, stays while the effect lives and thins away. */
#include "player.h"

/* the model of each kind of puff */
static const Sint16 puff_obj_tbl[6] = { 214, 215, 216, 217, 218, 219 } ;

static void puff_wait( PLAYER_WORK *ew );
static void puff_grow( PLAYER_WORK *ew );
static void puff_shrink( PLAYER_WORK *ew );
static void puff_stay( PLAYER_WORK *ew );
static void puff_fade( PLAYER_WORK *ew );

/*==============================================================*/
static void puff_main( PLAYER_WORK *ew )
{
	static void ( *puff_jmp[] )( PLAYER_WORK *ew ) = {
		puff_wait, puff_grow, puff_shrink, puff_stay, puff_fade,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !pl_w->be_flag ){
		work_free( ew ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_w->size_top > 80 ) ew->pos.y += ( ( pl_w->size_top - pl_w->size_bottom ) / 5 ) * pl_w->scale.y * DOT_Y( 2.0f ) ;
	else ew->pos.y += 30.0f + pl_w->size_top * pl_w->scale.y * 2.142857f ;
	puff_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The first puff makes the sound of its kind			*/
/*==============================================================*/
static void puff_wait( PLAYER_WORK *ew )
{
	if( ew->timer1 ){
		ew->timer1 -- ;
		return ;
	}
	if( !ew->sub_no ){
		switch( ew->type ){
		case 0:
			na_req( 29 ) ;
			break ;
		case 1:
			na_req( 28 ) ;
			break ;
		case 2:
			na_req( 27 ) ;
			break ;
		case 3:
			na_req( 30 ) ;
			break ;
		case 4:
			na_req( 26 ) ;
			break ;
		case 5:
			na_req( 29 ) ;
			break ;
		}
	}
	ew->disp_flag = 1 ;
	ew->timer1 = 10 ;
	ew->scale.x = 1.0f ;
	ew->routine_0 ++ ;
	puff_grow( ew ) ;
}

/*--------------------------------------------------------------*/
static void puff_grow( PLAYER_WORK *ew )
{
	if( -- ew->timer1 ) ew->scale.y += 0.15f ;
	else {
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.5f ;
		ew->routine_0 ++ ;
		ew->timer1 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void puff_shrink( PLAYER_WORK *ew )
{
	if( -- ew->timer1 ) ew->scale.y -= 0.05f ;
	else {
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->routine_0 ++ ;
		ew->timer1 = 40 ;
	}
}

/*--------------------------------------------------------------*/
static void puff_stay( PLAYER_WORK *ew )
{
	if( ew->sub_no ){
		work_free( ew ) ;
		return ;
	}
	if( !-- ew->timer1 ){
		ew->routine_0 ++ ;
		ew->timer1 = 4 ;
	}
}

/*--------------------------------------------------------------*/
static void puff_fade( PLAYER_WORK *ew )
{
	if( -- ew->timer1 ){
		ew->scale.x += 0.25f ;
		ew->scale.y -= 0.25f ;
	} else work_free( ew ) ;
}

/*==============================================================*/
static void puff_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	int i ;
	const Sint16 *tab = puff_obj_tbl ;

	for( i = 0 ; i < 3 ; i ++ ){
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) return ;
		ew->disp_flag = 0 ;
		ew->work_func = puff_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = pl_w->pos ;
		ew->obj = obj_efky->obj[tab[( Sint8 )no]] ;
		ew->master_w = pl_w ;
		ew->sub_no = i ;
		ew->timer1 = i * 2 ;
		ew->type = no ;
	}
}

/****************************************************************/
/*			Three puffs on a player, of the kind no (0 to 5)	*/
/****************************************************************/
void puff_ef_set( PLAYER_WORK *pl_w, int no )
{
	puff_set( pl_w, no ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_puff.c" End of File	*/
