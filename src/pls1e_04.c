/* Akuma's player effect 04: a picture at his place (Char_Effect 17), before or behind him by its
   own animation's code, while he spins: through the Tatsumaki Zankuukyaku (type 0) and the Messatsu
   Gou Rasen (type 1). */
#include "player.h"
#include "pl_char.h"

static void pls1e_04_main( PLAYER_WORK *ew );
static void pls1e_04_gou_rasen_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1e_04_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->master_w = pl_w ;
		ew->ef_no = 0x1e04 ;
		ew->work_func = pls1e_04_main ;
		if( no ){
			ew->work_func = pls1e_04_gou_rasen_main ;
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
			ew->disp_flag = 0 ;
		}
	}
	return ew ;
}

static void pls1e_04_tatsumaki_init( PLAYER_WORK *ew );
static void pls1e_04_tatsumaki_move( PLAYER_WORK *ew );
static void pls1e_04_end( PLAYER_WORK *ew );
static void pls1e_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1e_04_main( PLAYER_WORK *ew )
{
	static void ( *pls1e_04_jmp[] )( PLAYER_WORK *ew ) = {
		pls1e_04_tatsumaki_init, pls1e_04_tatsumaki_move, pls1e_04_end, pls1e_04_free,
	} ;

	pls1e_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1e_04_tatsumaki_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = 0 ;
	pl_char_set( ew, Char_Effect, 17 ) ;
	pls1e_04_tatsumaki_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_04_tatsumaki_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->cg_type < 0 ) ew->prio = 0 ;
	else ew->prio = 7 ;
	if( ew->master_w->char_no.b.grp != Char_Special ){
		ew->routine_0 ++ ;
		pls1e_04_end( ew ) ;
		return ;
	}
	switch( ew->master_w->char_no.b.no ){
	default:
		ew->routine_0 ++ ;
		pls1e_04_end( ew ) ;
		return ;
	case 6:
	case 8:
	case 9:
		pl_char_move( ew ) ;
		return ;
	}
}

static void pls1e_04_gou_rasen_init( PLAYER_WORK *ew );
static void pls1e_04_gou_rasen_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1e_04_gou_rasen_main( PLAYER_WORK *ew )
{
	static void ( *pls1e_04_gou_rasen_jmp[] )( PLAYER_WORK *ew ) = {
		pls1e_04_gou_rasen_init, pls1e_04_gou_rasen_move, pls1e_04_end, pls1e_04_free,
	} ;

	pls1e_04_gou_rasen_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1e_04_gou_rasen_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->disp_dir = 0 ;
	pl_char_set( ew, Char_Effect, 17 ) ;
	pls1e_04_gou_rasen_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_04_gou_rasen_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->cg_type < 0 ) ew->prio = 0 ;
	else ew->prio = 7 ;
	pl_w = ew->master_w ;
	if( pl_w->char_no.b.grp != Char_Super || pl_w->char_no.b.no != 10 ){
		ew->routine_0 ++ ;
		pls1e_04_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls1e_04_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1e_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1e_04.c" End of File	*/
