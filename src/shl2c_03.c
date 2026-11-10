/* Magneto's effect 03 (character 2c's shot 03): the four magnets of Hyper Grav. Each flies out from
   him, takes the opponent it hits, steers with the stick while it holds him, and then drags him
   along Magneto's side. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl2c_def.h"

/* where magnet 0 to 3 starts from Magneto, x for the front */
static Float shl2c_03_pos_tbl[] = {
	DOT_X( 32.0f ), DOT_Y( 64.0f ), -DOT_X( 32.0f ), DOT_Y( 64.0f ), DOT_X( 32.0f ), DOT_Y( 96.0f ), -DOT_X( 32.0f ), DOT_Y( 96.0f ),
} ;

/* the direction (32 to a circle) the stick picks, by its four direction bits */
static Sint8 shl2c_03_dir_tbl[] = {
	0, 8, 24, 0, 16, 12, 20, 0, 0, 4, 28, 0, 0, 0, 0, 0,
} ;

static void shl2c_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2c_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 4 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2c_03_main ;
			ew->sub_type = i ;
			ew->type = ew->work2 = ( Uint8 )i ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x2c03 ;
			PL2C_LW( pl_w )->magnet_flag |= 1 << i ;
		} else break ;
	}
	return ew ;
}

static void shl2c_03_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_03_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_03_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_03_release( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2c_03_main( PLAYER_WORK *ew )
{
	static void ( *shl2c_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2c_03_start, shl2c_03_fly, shl2c_03_hold, shl2c_03_release,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	switch( ew->routine_0 ){
	case 0 :
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
		ew->sub_prio = -8 ;
		ew->hit_box_no = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
		ew->disp_flag = 1 ;
		ew->obj_flag = 80 ;
		if( pl_w->pl_stance == STANCE_AIR ){
			ew->atck_no = pl2c_at_jgrav ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		}
		else {
			ew->atck_no = pl2c_at_grav ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		}
		ew->atck_flag = HIT_ON ;
		ew->dm_flag = HIT_ON ;
		ew->atck_attr |= ATR_SHOT_HOLD ;
		x = shl2c_03_pos_tbl[ew->type * 2] ;
		ew->pos.y = pl_w->pos.y + shl2c_03_pos_tbl[ew->type * 2 + 1] ;
		if( ew->disp_dir ) x = -x ;
		ew->pos.x = pl_w->pos.x + x ;
		pls2c_01_set( ew, 2 ) ;
		pl_char_set( ew, 21, 2 ) ;
	case 1 :
		ew->prio = pl_w->prio ;
		shl2c_03_jmp[ew->routine_1]( ew, pl_w ) ;
		if( !ew->obj_flag ){
			if( !scr_in_x_ck( ew ) ) shl2c_03_end( ew, pl_w ) ;
		}
		else ew->obj_flag -- ;
		break ;
	case 2 :
		if( pl_char_move( ew ) >= 0 ) break ;
	case 3 :
	default :
		work_free( ew ) ;
		break ;
	}
}

static Sint8 shl2c_03_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flies out; at the end of its animation it turns		*/
/*			toward the opponent									*/
/*==============================================================*/
static void shl2c_03_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 v ;

	if( !shl2c_03_ck( ew, pl_w ) ) shl2c_03_end( ew, pl_w ) ;
	else if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 32 ;
		ew->timer1 = 16 ;
		v.x = pl_w->target_w->pos.x ;
		v.y = pl_w->target_w->pos.y ;
		ew->dir = caldir_pos_256( &pl_w->pos, &v ) >> 3 ;
		ew->work1 = ew->atck_level ? 900 : 400 ;
		pl_char_set( ew, 21, 3 ) ;
	}
}

/*==============================================================*/
/*			Flies on; if it hits the opponent Magneto's side	*/
/*			is after, it holds him								*/
/*==============================================================*/
static void shl2c_03_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint8 d ;

	if( ew->hit_flag ){
		if( !pl_sp_catch_ck( ew ) ) goto end ;
		if( ew->em_w == pl_w->target_w ){
			if( pl_w->target_w->vital_new ){
				PL2C_LW( pl_w )->hold_flag = -1 ;
				pl_w->target_w->catch_no = pl_w->catch_no = 193 ;
				pl_ef_catch_set( ew, pl_w->target_w ) ;
				pl_w->target_w->catch_type = 6 ;
				pl_w->target_w->pl_stance = STANCE_AIR ;
				ew->work1 = 800 ;
				ew->timer0 = 64 ;
				ew->timer1 = 0 ;
				ew->routine_1 ++ ;
				hold_renda_set( pl_w->target_w, 16 ) ;
				return ;
			}
			goto end ;
		}
	}
	if( !shl2c_03_ck( ew, pl_w ) || -- ew->timer0 < 0 ){
end :
		shl2c_03_end( ew, pl_w ) ;
		return ;
	}
	if( -- ew->timer1 < 0 ){
		ew->timer1 = 16 ;
		if( !pl_w->cpu_flag && ( pl_w->pl_new_lvbt & KEY_LEVER ) ){
			d = shl2c_03_dir_tbl[( pl_w->pl_new_lvbt & KEY_LEVER ) >> 10] ;
			d -= ew->dir ;
			d &= 31 ;
			if( d >= 16 ) ew->dir -- ;
			else ew->dir ++ ;
			ew->dir &= 31 ;
		}
	}
	if( ( ew->work2 = ew->work2 - 1 ) < 0 ){
		ew->work2 = 3 ;
		hit_work_set( ew ) ;
	}
	dir_spd_move( ew, ew->work1 ) ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			Holds the opponent, and drags him toward Magneto	*/
/*			until he breaks free								*/
/*==============================================================*/
static void shl2c_03_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w, *tm_w ;
	VEC3 v[2] ;
	Sint16 ofs ;

	pl_w->catch_flag = 1 ;
	em_w = ew->catch_w ;
	if( ew->catch_pair.w->catch_pair.w != ew ){
		shl2c_03_end( ew, pl_w ) ;
		return ;
	}
	if( !em_w->vital_new || ( em_w->hit_box_no && em_w->routine_1 == PL_DAMAGE ) ){
		pl_w->catch_flag = 0 ;
		shl2c_03_end( ew, pl_w ) ;
		return ;
	}
	if( -- ew->timer0 >= 0 && em_w->renda_timer ){
		if( hold_renda_ck( em_w ) ) em_w->renda_timer = 0 ;
	}
	else ew->routine_1 ++ ;
	if( -- ew->timer1 < 0 ){
		ew->timer1 = 4 ;
		tm_w = vs_sys_work.team_w[pl_w->pl_no][0] ;
		ofs = 64 ;
		if( !pl_w->disp_dir ) ofs = -64 ;
		v[0].x = ew->pos.x ;
		v[0].y = ew->pos.y ;
		v[1].x = tm_w->pos.x + DOT_X( ofs ) ;
		v[1].y = tm_w->pos.y + DOT_Y( 80.0f ) ;
		ew->dir = caldir_pos_256( &v[0], &v[1] ) >> 3 ;
	}
	dir_spd_move( ew, ew->work1 ) ;
	catch_pos_set( ew, em_w ) ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			Lets the opponent go								*/
/*==============================================================*/
static void shl2c_03_release( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->catch_w ;

	pl_w->catch_flag = 0 ;
	em_w->dm_hit_w = pl_w ;
	em_w->catch_type = 1 ;
	em_w->atck_no = pl2c_at_grav_free ;
	shl2c_03_end( ew, pl_w ) ;
}

/*==============================================================*/
static Sint8 shl2c_03_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 ) return 0 ;
	if( pl_w->routine_1 == PL_CAUGHT ) return 0 ;
	if( PL2C_LW( pl_w )->magnet_flag != 15 ) return 0 ;
	if( PL2C_LW( pl_w )->hold_flag ) return 0 ;
	if( !pl_w->target_w->vital_new ) return 0 ;
	return 1 ;
}

/*==============================================================*/
static void shl2c_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2C_LW( pl_w )->magnet_flag &= ~( 1 << ew->type ) ;
	ew->routine_0 = 2 ;
	pl_char_set( ew, 21, 4 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2c_03.c" End of File	*/
