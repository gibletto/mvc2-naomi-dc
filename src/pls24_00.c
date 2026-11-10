/* Cammy's player effect 00: two pictures, by type: the cloak she throws off as she
   appears, which flies off and fades out blinking, and Bison in a win pose, who comes up beside
   her and bobs there; he tells her when the pose may go on (win_flag of her tlocal work). */
#include "player.h"
#include "ef_char.h"
#include "pl24_def.h"

static void pls24_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Made with pls24_00_set() in the character's			*/
/*			module												*/
/****************************************************************/
PLAYER_WORK *pls24_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls24_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2400 ;
	}
	return ew ;
}

/* the player's own work, which the effects of both kinds read */
static PL24_LOCAL_W *pls24_00_lw ;

static void pls24_00_cloak( PLAYER_WORK *ew );
static void pls24_00_bison( PLAYER_WORK *ew );

/*==============================================================*/
static void pls24_00_main( PLAYER_WORK *ew )
{
	static void ( *pls24_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls24_00_cloak, pls24_00_bison,
	} ;

	pls24_00_lw = PL24_LW( ew->master_w ) ;
	pls24_00_jmp[ew->type]( ew ) ;
}

static void pls24_00_cloak_init( PLAYER_WORK *ew );
static void pls24_00_cloak_move( PLAYER_WORK *ew );
static void pls24_00_end( PLAYER_WORK *ew );
static void pls24_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls24_00_cloak( PLAYER_WORK *ew )
{
	static void ( *pls24_00_cloak_jmp[] )( PLAYER_WORK *ew ) = {
		pls24_00_cloak_init, pls24_00_cloak_move, pls24_00_end, pls24_00_free,
	} ;

	pls24_00_cloak_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls24_00_cloak_init( PLAYER_WORK *ew )
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
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 0 ) ;
}

/*==============================================================*/
/*			Waits for the player's animation, then flies off	*/
/*			and blinks out										*/
/*==============================================================*/
static void pls24_00_cloak_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		if( !ew->master_w->cg_type ){
			ew->disp_flag = 1 ;
			return ;
		}
		ew->routine_1 ++ ;
		ew->timer1 = 60 ;
		if( ew->disp_dir ){
			ew->spd.x = -DOT_X( 2.0f ) ;
		} else {
			ew->spd.x = DOT_X( 2.0f ) ;
		}
		ew->acc.x = 0 ;
		ew->spd.y = DOT_Y( 6.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
	}
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer1 < 0 ){
		pls24_00_end( ew ) ;
		return ;
	}
	if( ew->timer1 > 40 ){
		ew->disp_flag = 1 ;
		return ;
	}
	ew->disp_flag = ( Sys->game_timer0 & 1 ) ^ ew->pl_no ;
}

static void pls24_00_bison_init( PLAYER_WORK *ew );
static void pls24_00_bison_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls24_00_bison( PLAYER_WORK *ew )
{
	static void ( *pls24_00_bison_jmp[] )( PLAYER_WORK *ew ) = {
		pls24_00_bison_init, pls24_00_bison_move, pls24_00_end, pls24_00_free,
	} ;

	pls24_00_bison_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls24_00_bison_init( PLAYER_WORK *ew )
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
	ew->disp_dir = ew->disp_dir ^ 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.x += ew->disp_dir ? -DOT_X( 128.0f ) : DOT_X( 128.0f ) ;
	ew->pos.y = ew->master_w->ground_y ;
	ew->pos.y += DOT_Y( 16.0f ) ;
	ew->disp_flag = 0 ;
	ef_char_set( ew, 27, 1 ) ;
}

/*==============================================================*/
/*			Plays its animation, then bobs on a sine of dir		*/
/*==============================================================*/
static void pls24_00_bison_move( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	if( !ew->routine_1 ){
		if( ef_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			pls24_00_lw->win_flag = 1 ;
			ew->timer1 = 1 ;
			ew->timer0 = 1 ;
			ew->dir = 0 ;
		} else if( !ew->cg_type ){
			return ;
		}
	} else {
		ef_char_move( ew ) ;
		ew->pos.y += nlSin( ( ( 40 - ew->dir ) & 31 ) << 11 ) * DOT_Y( 2.0f ) ;
		if( !-- ew->timer0 ){
			ew->dir ++ ;
			ew->timer0 = ew->timer1 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls24_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	pls24_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls24_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls24_00.c" End of File	*/
