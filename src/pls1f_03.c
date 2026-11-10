/* B.B. Hood's player effect 03: the small things that go with her moves (kinds by the setter's no:
   2, 3, 4, 5, 14 and 22), each a few animation frames at her side or on the ground. */
#include "player.h"
#include "ef_char.h"
#include "pl1f_def.h"

static void pls1f_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1f_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_03_main ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1f03 ;
		ew->work1 = pl_w->char_no.w ;
	}
	return ew ;
}

/*==============================================================*/
/*			What every kind sets up first: her look, and its	*/
/*			own													*/
/*==============================================================*/
static void pls1f_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
}

/*==============================================================*/
static void pls1f_03_none( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls1f_03_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
		pls1f_03_init( ew, pl_w ) ;
		ef_char_set( ew, 27, 24 ) ;
	}
	ew->prio = 11 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls1f_03_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
		pls1f_03_init( ew, pl_w ) ;
		ef_char_set( ew, 27, 33 ) ;
		ew->obj_flag = -1 ;
		ew->sub_prio = -1 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->ground_y ;
		ew->prio = 11 ;
	}
	if( !ew->routine_1 ){
		if( pl_w->team_no ){
			ew->routine_1 = 1 ;
			ef_char_set( ew, 27, 36 ) ;
		} else {
			if( pl_w->routine_1 == PL_DAMAGE ){
				work_free( ew ) ;
				return ;
			} else if( PL1F_LW( pl_w )->ef_char_no != ew->obj_flag ){
				if( PL1F_LW( pl_w )->ef_char_no == 36 ) ew->routine_1 ++ ;
				ew->obj_flag = PL1F_LW( pl_w )->ef_char_no ;
				ef_char_set( ew, 27, ew->obj_flag ) ;
			} else {
				if( ew->cg_type ){
					ew->cg_type = 0 ;
					pl_voice_req( pl_w, 35 ) ;
				}
				ef_char_move( ew ) ;
				return ;
			}
		}
	} else {
		if( ef_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*==============================================================*/
static void pls1f_03_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
		pls1f_03_init( ew, pl_w ) ;
		ef_char_set( ew, 27, 37 ) ;
		ew->timer0 = 2 ;
		ew->sub_prio = -1 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->ground_y ;
	}
	if( ef_char_move( ew ) < 0 ){
		work_free( ew ) ;
	} else if( ew->cg_type ){
		ew->cg_type = 0 ;
		if( --ew->timer0 < 0 ){
			ew->timer0 = 2 ;
			pls1f_03_set( pl_w, 5 ) ;
		}
	}
}

/*==============================================================*/
static void pls1f_03_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Float pls1f_03_spd_tbl[4] = { DOT_Y( 2.0f ), DOT_Y( 1.5f ), DOT_Y( 1.25f ), 2.142857f } ;
	Float x ;
	Uint32 no ;

	if( !ew->routine_0 ){
		pls1f_03_init( ew, pl_w ) ;
		ef_char_set( ew, 27, 38 ) ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		x = ( ( Rnd() & 127 ) - 96 ) * 1.6666666f ;
		if( ( ew->disp_dir = pl_w->disp_dir ) ) x = -x ;
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->ground_y + ( ( Rnd() & 63 ) + 80 ) * 2.142857f ;
		no = Rnd() & 3 ;
		ew->spd.y = pls1f_03_spd_tbl[no] ;
		ew->acc.y = -0.13392857f ;
		ew->sub_prio = -1 ;
	}
	if( pl_w->routine_1 == PL_DAMAGE || ef_char_move( ew ) < 0 ){
		work_free( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*==============================================================*/
static void pls1f_03_t14( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

	if( ew->work1 != pl_w->char_no.w ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ew->be_flag = 0 ;
		return ;
	}
	if( !ew->routine_0 ){
		ew->be_flag = 1 ;
		pls1f_03_init( ew, pl_w ) ;
		ofs = DOT_X( 64.0f ) ;
		if( ( ew->disp_dir = pl_w->disp_dir ) ) ofs = -DOT_X( 64.0f ) ;
		ew->pos.x = pl_w->pos.x + ofs ;
		ew->pos.y = pl_w->ground_y ;
		ef_char_set( ew, 27, 53 ) ;
		ew->cg_shadow = 0xe0 ;
		pl_shadow_set( ew ) ;
	}
	ef_char_move( ew ) ;
	ew->cg_shadow = 0xe0 ;
}

/*==============================================================*/
static void pls1f_03_t22( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
		pls1f_03_init( ew, pl_w ) ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->ground_y ;
		ew->disp_dir = pl_w->disp_dir ;
		ef_char_set( ew, 27, 99 ) ;
	}
	if( ef_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls1f_03_main( PLAYER_WORK *ew )
{
	static void ( *pls1f_03_jmp[23] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1f_03_none, pls1f_03_none, pls1f_03_t2, pls1f_03_t3, pls1f_03_t4, pls1f_03_t5,
		pls1f_03_none, pls1f_03_none, pls1f_03_none, pls1f_03_none, pls1f_03_none, pls1f_03_none,
		pls1f_03_none, pls1f_03_none, pls1f_03_t14, pls1f_03_none, pls1f_03_none, pls1f_03_none,
		pls1f_03_none, pls1f_03_none, pls1f_03_none, pls1f_03_none, pls1f_03_t22,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ){
		work_free( ew ) ;
	} else {
		pls1f_03_jmp[ew->type]( ew, pl_w ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1f_03.c" End of File	*/
