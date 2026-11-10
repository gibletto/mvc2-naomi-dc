/* B.B. Hood's player effect 02: the thing that follows her about and shows how her combo goes on
   (her combo_cnt). It keeps at her side, turns when she turns, and each kind of hit (her cg_flag2
   frame code, obj_flag / 2) gives it an animation. */
#include "player.h"
#include "ef_char.h"
#include "pl1f_def.h"

static void pls1f_02_main( PLAYER_WORK *ew );
static int pls1f_02_char_set( PLAYER_WORK *ew, Uint8 grp, Uint8 no );

/****************************************************************/
PLAYER_WORK *pls1f_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_02_main ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1f02 ;
	}
	return ew ;
}

static void pls1f_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1f_02_approach( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1f_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1f_02_voice( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1f_02_flap( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1f_02_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls1f_02_char_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1f_02_main( PLAYER_WORK *ew )
{
	static void ( *pls1f_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1f_02_follow, pls1f_02_approach, pls1f_02_approach,
		pls1f_02_hold, pls1f_02_hold, pls1f_02_hold, pls1f_02_hold, pls1f_02_hold,
		pls1f_02_voice, pls1f_02_hold, pls1f_02_flap, pls1f_02_hold, pls1f_02_hold, pls1f_02_hold,
		pls1f_02_stay, pls1f_02_stay,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ){
		work_free( ew ) ;
		return ;
	} else if( !ew->routine_0 ){
		ew->be_flag = 1 ;
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
		ew->prio = 12 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->obj_flag = 0 ;
		ew->routine_1 = 0 ;
		pls1f_02_char_set( ew, 27, 80 ) ;
		pl_shadow_set( ew ) ;
		return ;
	} else if( pl_w->be_flag && pl_w->disp_flag ){
		ew->disp_flag = 1 ;
		ew->be_flag = 1 ;
		if( pl_w->cg_flag2 != ew->obj_flag ){
			ew->obj_flag = pl_w->cg_flag2 ;
			ew->routine_1 = 0 ;
			ew->cg_type = -1 ;
		}
	} else {
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
	}
	pls1f_02_jmp[ew->obj_flag >> 1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Which side of the master it is on: 1 left of her	*/
/*==============================================================*/
static int pls1f_02_side( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 side = 0 ;

	if( ew->pos.x - pl_w->pos.x < 0.0f ) side = 1 ;
	return side ;
}

/*==============================================================*/
/*			It stays still for a while, or looks about			*/
/*==============================================================*/
static void pls1f_02_wait_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 = 0 ;
	if( Rnd() & 3 ) pls1f_02_char_set( ew, 27, 80 ) ;
	else pls1f_02_char_set( ew, 27, 81 ) ;
}

/*==============================================================*/
static void pls1f_02_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 = 1 ;
	ew->disp_dir ^= 1 ;
	pls1f_02_char_set( ew, 27, 84 ) ;
}

/*==============================================================*/
/*			How far it is from its place beside the master		*/
/*==============================================================*/
static Float pls1f_02_dx( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs = DOT_X( 64.0f ) ;
	Float d ;

	if( pl_w->disp_dir ) ofs = -DOT_X( 64.0f ) ;
	d = ofs + pl_w->pos.x - ew->pos.x ;
	return d ;
}

/*==============================================================*/
/*			The animation her hit gives it						*/
/*==============================================================*/
static void pls1f_02_anm_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 pls1f_02_anm_tbl[16] = {
		80, 97, 98, 88, 89, 90, 91, 92, 93, 94, 95, 85, 86, 87, 0, 0,
	} ;

	pls1f_02_char_set( ew, 27, pls1f_02_anm_tbl[ew->obj_flag >> 1] ) ;
}

/*==============================================================*/
static void pls1f_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float lim = DOT_X( 16.0f ) ;
	Float d ;
	Sint8 neg ;

	if( !ew->routine_1 ){
		if( pls1f_02_side( ew, pl_w ) != ew->disp_dir ){
			pls1f_02_turn( ew, pl_w ) ;
			return ;
		} else if( ew->cg_type ){
			Uint8 no ;

			d = pls1f_02_dx( ew, pl_w ) ;
			neg = 0 ;
			if( d < 0.0f ){
				d = -d ;
				neg = 1 ;
			}
			if( lim > d ){
				pls1f_02_wait_set( ew, pl_w ) ;
				return ;
			}
			ew->routine_1 = 2 ;
			ew->timer0 = 0x160 ;
			if( neg ) d = -d ;
			if( ew->disp_dir ) d = -d ;
			if( d < 0.0f ){
				ew->spd.x = -DOT_X( 4.0f ) ;
				no = 82 ;
			} else {
				ew->spd.x = 5.0f ;
				no = 83 ;
			}
			if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
			pls1f_02_char_set( ew, 27, no ) ;
		}
	} else if( ew->routine_1 == 1 ){
		if( pls1f_02_char_move( ew ) < 0 ) pls1f_02_wait_set( ew, pl_w ) ;
		return ;
	} else {
		if( pls1f_02_side( ew, pl_w ) != ew->disp_dir ){
			pls1f_02_turn( ew, pl_w ) ;
			return ;
		}
		ew->pos.x += ew->spd.x ;
		if( --ew->timer0 < 0 ){
			pls1f_02_wait_set( ew, pl_w ) ;
			return ;
		}
		d = pls1f_02_dx( ew, pl_w ) ;
		if( d < 0.0f ) d = -d ;
		if( lim > d ){
			pls1f_02_wait_set( ew, pl_w ) ;
			return ;
		}
	}
	pls1f_02_char_move( ew ) ;
}

/*==============================================================*/
static void pls1f_02_approach( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float d ;

	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		ew->work1 = pl_w->combo_cnt ;
		pls1f_02_anm_set( ew, pl_w ) ;
		d = pl_w->pos.x - ew->pos.x ;
		if( d < 0.0f ) d = -d ;
		if( !( d > DOT_X( 128.0f ) ) ) pls1f_02_char_set( ew, 27, 79 ) ;
	}
	if( pl_w->combo_cnt != ew->work1 ) ew->routine_1 = 0 ;
	pls1f_02_char_move( ew ) ;
}

/*==============================================================*/
static void pls1f_02_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		pls1f_02_anm_set( ew, pl_w ) ;
	}
	pls1f_02_char_move( ew ) ;
}

/*==============================================================*/
static void pls1f_02_voice( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		pls1f_02_anm_set( ew, pl_w ) ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		pl_voice_req( pl_w, 34 ) ;
	}
	pls1f_02_char_move( ew ) ;
}

/*==============================================================*/
static void pls1f_02_flap( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		pls1f_02_char_set( ew, 27, 95 ) ;
	}
	pls1f_02_char_move( ew ) ;
}

/*==============================================================*/
static void pls1f_02_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls1f_02_char_move( ew ) ;
}

/*==============================================================*/
static int pls1f_02_char_set( PLAYER_WORK *ew, Uint8 grp, Uint8 no )
{
	int ret = ef_char_set( ew, grp, no ) ;

	ew->cg_shadow = 0xe0 ;
	return ret ;
}

/*==============================================================*/
static int pls1f_02_char_move( PLAYER_WORK *ew )
{
	int end ;

	end = ef_char_move( ew ) ;
	ew->cg_shadow = 0xe0 ;
	return end ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1f_02.c" End of File	*/
