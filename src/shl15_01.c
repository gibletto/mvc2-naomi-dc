/* Amingo's effect 01 (character 15's shot 01): the hopping seed of Daichi no Kodomo (type 0 the one
   that seizes the opponent, type 1 the one that strikes him). It flies out, can be struck down, and
   is counted in his tlocal while it lives. */
#include "player.h"
#include "pl_char.h"
#include "pl15_def.h"

static void shl15_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl15_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *ew ;

	if( lw->shot_cnt >= 2 ) return 0 ;
	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x1501 ;
		ew->work_func = shl15_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		lw->shot_cnt ++ ;
	}
	return ew ;
}

static void shl15_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl15_01_main( PLAYER_WORK *ew )
{
	static void ( *shl15_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl15_01_init, shl15_01_move, shl15_01_end,
	} ;

	shl15_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl15_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

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
	ew->be_flag = 1 ;
	ew->prio = 0 ;
	ew->atck_no = pl_w->atck_pk + 48 ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	ew->pos = pl_w->pos ;
	x = -140.0f ;
	if( ew->disp_dir ) x = 140.0f ;
	ew->pos.x += x ;
	pl_char_set( ew, Char_Effect, 4 ) ;
	pl_shadow_set( ew ) ;
}

static void shl15_01_gone( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_t0_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_t0_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_t1_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl15_01_t1_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Off the screen it goes								*/
/*==============================================================*/
static void shl15_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl15_01_move_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl15_01_open, shl15_01_t0_hop, shl15_01_t0_jump, shl15_01_hold,
	} ;
	static void ( *shl15_01_move_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl15_01_open, shl15_01_t1_hop, shl15_01_t1_atck,
	} ;

	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 4 ;
		shl15_01_gone( ew, pl_w ) ;
		return ;
	}
	if( ew->type ) shl15_01_move_jmp1[ew->routine_1]( ew, pl_w ) ;
	else shl15_01_move_jmp0[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			The opening: it turns, then flies slowly forward	*/
/*==============================================================*/
static void shl15_01_open( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = ew->atck_level ? 9 : 6 ;
		if( ew->type ) ew->spd.x = -DOT_X( 4.0f ) ;
		else ew->spd.x = -DOT_X( 2.0f ) ;
		ew->acc.x = 0 ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		pl_char_set( ew, Char_Effect, 6 ) ;
	}
}

static Sint32 shl15_01_hit_ck( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl15_01_t0_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	VEC3 tpos ;
	Float range ;

	if( pl_char_move( ew ) < 0 ) ew->timer0 -- ;
	if( shl15_01_hit_ck( ew ) ) return ;
	if( ew->cg_type ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
	}
	else {
		em_w = pl_w->target_w ;
		range = DOT_X( 64.0f ) ;
		if( ( em_w->pos.x - range < ew->pos.x && em_w->pos.x + range > ew->pos.x && em_w->pl_stance != STANCE_AIR ) || ew->timer0 == 1 ){
			ew->routine_1 ++ ;
			pl_voice_req( ew, 52 ) ;
			tpos.x = em_w->pos.x ;
			if( ew->timer0 == 1 ){
				ew->spd.x = -DOT_X( 4.0f ) ;
				ew->acc.x = 0 ;
				if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
			}
			else {
				ew->spd.x = ( tpos.x - ew->pos.x ) / 20.0f ;
				ew->acc.x = 0 ;
			}
			ew->timer0 = 20 ;
			ew->spd.y = DOT_Y( 5.0f ) ;
			ew->acc.y = -DOT_Y( 0.5f ) ;
			pl_char_set( ew, Char_Effect, 7 ) ;
		}
		else {
			ew->spd.y = DOT_Y( 3.0f ) ;
			ew->acc.y = -DOT_Y( 0.5f ) ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl15_01_t0_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	VEC3 dd, ad ;

	if( !shl15_01_hit_ck( ew ) ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 = 2 ;
			ew->routine_1 = 2 ;
			ew->spd.x = 0 ;
			ew->spd.y = 0 ;
			ew->acc.x = 0 ;
			ew->acc.y = 0 ;
			pl_char_set( ew, Char_Effect, 12 ) ;
			return ;
		}
		if( ew->cg_type ){
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
		}
		if( ew->hit_flag ){
			if( ew->hit_flag & HR_GUARD ){
				ew->routine_0 = 2 ;
				ew->routine_1 = 1 ;
				ew->spd.x = -ew->spd.x / 2.0f ;
				ew->spd.y = DOT_Y( 2.0f ) ;
				pl_char_set( ew, Char_Effect, 13 ) ;
			}
			else if( pl_sp_catch_ck( ew ) ){
				em_w = ew->em_w ;
				if( em_w->pl_stance == STANCE_AIR ){
					ew->routine_0 = 2 ;
					ew->routine_1 = 1 ;
					ew->spd.x = -ew->spd.x / 2.0f ;
					ew->spd.y = DOT_Y( 2.0f ) ;
					pl_char_set( ew, Char_Effect, 13 ) ;
				}
				else {
					ew->routine_1 ++ ;
					ew->routine_2 = 0 ;
					ew->timer0 = 30 ;
					pl_voice_req( ew, 48 ) ;
					ad.x = dd.x = ew->pos.x - em_w->pos.x ;
					if( dd.x < 0 ) ad.x = -dd.x ;
					if( ad.x > DOT_X( 16.0f ) ){
						if( ew->disp_dir ) ew->pos.x = em_w->pos.x - DOT_X( 16.0f ) ;
						else ew->pos.x = em_w->pos.x + DOT_X( 16.0f ) ;
					}
					pl_char_set( ew, 15, 1 ) ;
					pl_w->catch_no = 0xc1 ;
					pl_ef_catch_set( ew, em_w ) ;
					ew->spd.z = ew->pos.x - em_w->pos.x ;
					if( ew->catch_pair.w->pl_type != PL_SERVBOT ) ew->spd.y = ew->pos.y - em_w->pos.y ;
					else ew->spd.y = 0 ;
				}
			}
			else {
				ew->routine_0 = 2 ;
				ew->routine_1 = 1 ;
				pl_char_set( ew, Char_Effect, 13 ) ;
			}
		}
		else hit_work_set( ew ) ;
	}
}

/*==============================================================*/
/*			It holds the opponent								*/
/*==============================================================*/
static void shl15_01_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->catch_pair.w ;

	if( em_w->catch_pair.w != ew ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 4 ;
		shl15_01_gone( ew, pl_w ) ;
		return ;
	}
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 1 ;
		em_w->catch_type = 1 ;
		em_w->atck_no = pl15_at_daichi_catch ;
		ew->spd.x = -ew->spd.x / 2.0f ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		pl_char_set( ew, Char_Effect, 13 ) ;
		return ;
	}
	pl_w->catch_flag = 1 ;
	pl_char_move( ew ) ;
	ew->pos.x = em_w->pos.x + ew->spd.z ;
	ew->pos.y = em_w->pos.y + ew->spd.y ;
}

/*--------------------------------------------------------------*/
static void shl15_01_t1_hop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;
	VEC3 tpos ;
	Float range ;
	Uint8 no ;

	if( pl_char_move( ew ) < 0 ) ew->timer0 -- ;
	if( shl15_01_hit_ck( ew ) ) return ;
	if( ew->cg_type ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
	}
	else {
		em_w = pl_w->target_w ;
		range = 80.0f ;
		if( ( em_w->pos.x - range < ew->pos.x && em_w->pos.x + range > ew->pos.x && em_w->pl_stance != STANCE_AIR ) || ew->timer0 == 1 ){
			ew->routine_1 ++ ;
			if( !pl_w->atck_level ) no = 8 ;
			else no = 9 ;
			pl_char_set( ew, Char_Effect, no ) ;
		}
		else {
			ew->spd.y = DOT_Y( 2.25f ) ;
			ew->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl15_01_t1_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 3 ;
		ew->spd.x = 0 ;
		ew->spd.y = 0 ;
		ew->acc.x = 0 ;
		ew->acc.y = 0 ;
		pl_char_set( ew, Char_Effect, 12 ) ;
		return ;
	}
	if( ew->hit_flag ){
		if( ew->hit_flag & HR_GUARD ){
			ew->routine_0 = 2 ;
			ew->routine_1 = 1 ;
			ew->spd.x = -( ew->spd.x * 2.0f ) ;
			ew->spd.y = DOT_Y( 3.0f ) ;
			ew->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( ew, Char_Effect, 13 ) ;
		}
		else {
			em_w = ew->em_w ;
			if( !em_w->work_kind && em_w->routine_1 == PL_DAMAGE && em_w->vital_new ){
				ew->routine_0 = 2 ;
				ew->routine_1 = 5 ;
				ew->spd.x = 0 ;
				ew->spd.y = 0 ;
				ew->acc.x = 0 ;
				ew->acc.y = 0 ;
				ew->pos.y = pl_w->ground_y ;
			}
		}
	}
	else hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl15_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	switch( ew->routine_1 ){
	case 0:
		if( ew->hit_stop ){
			ew->hit_stop -- ;
			pl_char_move( ew ) ;
			break ;
		}
	case 5:
		if( ew->hit_stop ){
			ew->hit_stop -- ;
			break ;
		}
	case 1:
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y <= pl_w->ground_y ){
			ew->routine_1 = 3 ;
			ew->pos.y = pl_w->ground_y ;
			pl_char_set( ew, Char_Effect, 12 ) ;
		}
		break ;
	case 2:
	case 3:
		if( pl_char_move( ew ) < 0 ){
			ew->routine_1 = 4 ;
			shl15_01_gone( ew, pl_w ) ;
		}
		break ;
	case 4:
		shl15_01_gone( ew, pl_w ) ;
		ew->be_flag = 0 ;
		work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*			Struck: it falls back with a sound and a mark		*/
/*==============================================================*/
static Sint32 shl15_01_hit_ck( PLAYER_WORK *ew )
{
	VEC3 ofs ;

	if( ew->hit_box_no ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
		ew->spd.x = -ew->spd.x / 2.0f ;
		pl_char_set( ew, Char_Effect, 11 ) ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 16.0f ) ;
		ofs.z = 0 ;
		hit_mark_set( ew, &ofs, 0 ) ;
		pl_voice_req( ew, 37 ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			One less of Amingo's seeds is out					*/
/*==============================================================*/
static void shl15_01_gone( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL15_LOCAL_W *lw = ( PL15_LOCAL_W * )pl_w->tlocal ;

	if( ew->disp_flag ){
		ew->disp_flag = 0 ;
		lw->shot_cnt -- ;
		if( lw->shot_cnt < 0 ) lw->shot_cnt = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl15_01.c" End of File	*/
