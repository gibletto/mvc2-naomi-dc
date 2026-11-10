/* Spider-Man's Web Ball (character 0c's shot 00): it flies until it catches the opponent in a web
   and holds him there for a while, or misses. */
#include "player.h"
#include "pl_char_old.h"
#include "pl0c_def.h"

extern int pls0c_00_set( PLAYER_WORK *pl_w, int no );

/* where the ball starts from Spider-Man and its speed, x and y, on the ground and in the air */
Sint16 shl0c_00_ofs[] = { -40, 46, -56, 56 } ;
Sint16 shl0c_00_spd[] = { -1620, 0, -1620, -952 } ;

/* the web's animation, by the caught character */
Uint8 shl0c_00_char_tbl[64] = {
	0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
} ;

static void shl0c_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0c_00_set( PLAYER_WORK *pl_w, int air )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0c_00_main ;
		ew->ef_no = 0x0c00 ;
		ew->type = air ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl0c_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0c_00_main( PLAYER_WORK *ew )
{
	static void ( *shl0c_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0c_00_init, shl0c_00_move, shl0c_00_end, shl0c_00_free,
	} ;

	shl0c_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl0c_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	Sint16 *dt ;
	Float ofs ;
	int no ;
	int atck ;

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
	ew->routine_0 ++ ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	lw->shot_flag = 1 ;
	no = ew->type * 2 ;
	dt = &shl0c_00_ofs[no] ;
	ofs = *dt++ * 1.6666666f ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = pl_w->pos.x + ofs ;
	ew->pos.y = pl_w->pos.y + *dt * 2.142857f ;
	dt = &shl0c_00_spd[no] ;
	ew->spd.x = *dt++ * 1.6666666f / 256.0f ;
	ew->spd.y = *dt * 2.142857f / 256.0f ;
	if( ew->disp_dir ) ew->spd.x *= -1.0f ;
	atck = 54 ;
	if( ew->type ) atck = 57 ;
	ew->atck_no = ew->atck_level + atck ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, no + 1 ) ;
}

static void shl0c_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_free_em( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0c_00_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0c_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0c_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0c_00_fly, shl0c_00_catch, shl0c_00_hold, shl0c_00_free_em, shl0c_00_vanish,
	} ;

	shl0c_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0c_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	int n ;
	int no ;
	Sint16 t ;

	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		lw->shot_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->type && !ew->hit_flag ){
		if( ew->pos.y < pl_w->ground_y + DOT_Y( 30.0f ) ){
			ew->routine_1 = 4 ;
			ew->pos.y = pl_w->ground_y ;
			lw->shot_flag = 0 ;
			pl_char_set( ew, Char_Effect, 5 ) ;
			return ;
		}
	}
	if( !ew->hit_flag && !ew->hit_box_no ){
		hit_work_set( ew ) ;
		return ;
	}
	if( !ew->hit_box_no && pl_sp_catch_ck( ew ) ){
		em = ew->em_w ;
		n = em->work_no >> 1 ;
		if( !( lw->web_flag[n] & WEB_HOLD ) ){
			ew->ef_w = em ;
			ew->sub_no = n ;
			lw->shot_flag = 0 ;
			lw->web_flag[n] |= WEB_HOLD ;
			pl_char_set( em, Char_Stand, 0 ) ;
			pl_w->catch_no = 0xc3 ;
			pl_w->catch_grip_old = -1 ;
			pl_ef_catch_set( ew, em ) ;
			em->catch_type = 6 ;
			ew->pos = em->pos ;
			if( em->pl_stance == STANCE_AIR ){
				em->spd.x /= 4.0f ;
				em->acc.x /= 4.0f ;
				em->acc.y = -DOT_Y( 0.375f ) ;
				if( em->spd.y > 0.0f ) em->spd.y /= 4.0f ;
			} else {
				em->spd.x = 0.0f ;
				em->spd.y = 0.0f ;
				em->acc.x = 0.0f ;
				em->acc.y = 0.0f ;
			}
			t = 18 ;
			if( ew->atck_level ) t = 62 ;
			if( lw->web_flag[n] & WEB_HIT ){
				lw->web_flag[n] &= ~WEB_HIT ;
				t = 0 ;
			}
			ew->timer0 = t ;
			ew->routine_1 ++ ;
			no = shl0c_00_char_tbl[em->pl_type] ;
			ew->dir = no ;
			pl_char_set( ew, Char_Effect, no + 6 ) ;
			return ;
		}
	}
	ew->routine_1 = 4 ;
	lw->shot_flag = 0 ;
	pl_char_set( ew, Char_Effect, ew->type * 2 + 2 ) ;
}

/*--------------------------------------------------------------*/
static void shl0c_00_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = ew->ef_w ;
	int n ;

	em->nml_catch_ng_timer = 2 ;
	ew->pos = em->pos ;
	if( ew->catch_pair.w->catch_pair.w != ew ){
		ew->routine_1 ++ ;
		ew->timer0 = 0 ;
	} else {
		n = ew->sub_no ;
		if( em->hit_box_no ){
			ew->routine_1 = 3 ;
			ew->timer0 = 0 ;
			lw->web_flag[n] &= ~WEB_HOLD ;
			lw->web_flag[n] |= WEB_HIT ;
			if( !em->vital_new ) em->catch_type = 7 ;
			else em->catch_type = 0 ;
			pl_voice_req( ew, 35 ) ;
			pls0c_00_set( ew, ew->dir ) ;
			return ;
		}
		pl_char_move( ew ) ;
		if( !ew->cg_type ) return ;
		ew->routine_1 ++ ;
		em->disp_flag = 0 ;
	}
	hold_renda_set( em, 8 ) ;
}

/*--------------------------------------------------------------*/
static void shl0c_00_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	int n ;

	if( ew->catch_pair.w->catch_pair.w != ew ){
		ew->routine_0 = 2 ;
		ew->timer0 = 0 ;
	} else {
		pl_char_move( ew ) ;
		n = ew->sub_no ;
		em = ew->ef_w ;
		em->nml_catch_ng_timer = 2 ;
		ew->pos = em->pos ;
		if( em->hit_box_no ){
			if( em->routine_1 != PL_DAMAGE ){
				ew->cg_ctr = 1 ;
				pl_char_move( ew ) ;
			} else {
				ew->timer0 = 0 ;
				lw->web_flag[n] |= WEB_HIT ;
			}
		}
		em->disp_flag = 0 ;
		if( hold_renda_ck( em ) ){
			ew->cg_ctr = 1 ;
			ew->timer0 -= ew->atck_level * 4 + 3 ;
		}
		if( --ew->timer0 >= 0 ) return ;
		ew->routine_1 ++ ;
		lw->web_flag[n] &= ~WEB_HOLD ;
		if( !em->vital_new ) em->catch_type = 7 ;
		else {
			em->catch_type = 0 ;
			if( !( lw->web_flag[n] & WEB_HIT ) ){
				if( ew->atck_level ) em->auto_guard_timer = 8 ;
				if( em->pl_stance != STANCE_AIR ) pl_stand_ret( em ) ;
				else pl_sjump_ret( em ) ;
			}
		}
	}
	pl_voice_req( ew, 35 ) ;
	pls0c_00_set( ew, ew->dir ) ;
}

/*--------------------------------------------------------------*/
static void shl0c_00_free_em( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = ew->ef_w ;

	em->disp_flag = 1 ;
	if( em->hit_box_no ) lw->web_flag[ew->sub_no] |= WEB_HIT ;
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0c_00_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0c_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	lw->web_flag[ew->sub_no] &= ~WEB_HOLD ;
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0c_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0c_00.c" End of File	*/
