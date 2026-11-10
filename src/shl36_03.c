/* Thanos's effect 03 (character 36's shot 03): the bubble of his Death Sphere (sp_sphere). It flies
   out from his hand; if it takes the opponent it holds him inside, and ends when his mashing has
   worn it out. */
#include "player.h"
#include "pl_char.h"
#include "pl36_def.h"

/* what the effect holds, in work1 */
#define SHL36_03_EM( w )	( ( PLAYER_WORK * )( w )->work1 )

/* the grab's end: no is 0 when it flew far enough, 1 when it missed, 2 when it let go */
extern void pls36_02_set( PLAYER_WORK *w, Uint8 no );

static void shl36_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl36_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl36_03_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3603 ;
	}
	return ew ;
}

static void shl36_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_03_main( PLAYER_WORK *ew )
{
	static void ( *shl36_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_03_init, shl36_03_move, shl36_03_end, shl36_03_free,
	} ;

	shl36_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl36_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Float shl36_03_spd[3] = {
		-DOT_X( 5.0f ), -DOT_X( 7.0f ), -15.0f,
	} ;
	Sint16 x ;
	Float spd ;

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
	ew->prio = 8 ;
	ew->sub_type = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl36_at_sphere ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	PL36_LW( pl_w )->shot_timer[0] = 4 ;
	ew->obj_flag = 0 ;
	ew->size_front = ew->size_back = 32 ;
	x = -72 ;
	if( ew->disp_dir ) x = 72 ;
	ew->pos.x = pl_w->pos.x + DOT_X( x ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 64.0f ) ;
	spd = shl36_03_spd[ew->atck_level] ;
	if( ew->disp_dir ) spd = -spd ;
	ew->spd.x = spd ;
	ew->acc.x = ew->spd.y = ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 5 ) ;
}

static void shl36_03_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_03_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_03_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl36_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_03_fly, shl36_03_catch, shl36_03_hold,
	} ;

	PL36_LW( pl_w )->shot_timer[0] = 4 ;
	shl36_03_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl36_03_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It flies out; when it takes the opponent it goes	*/
/*			on to hold him, and it goes when its master acts	*/
/*			again or it is hit									*/
/*==============================================================*/
static void shl36_03_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || ew->hit_box_no ){
	} else if( ew->hit_flag ){
		if( pl_sp_catch_ck( ew ) ){
			ew->routine_1 ++ ;
			ew->prio = 8 ;
			shl36_03_pos( ew, pl_w ) ;
			pl_char_set( ew, Char_Effect, 4 ) ;
			return ;
		}
	} else {
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !( ew->sub_type += 64 ) ) pls36_02_set( ew, 0 ) ;
		if( !scr_in_x_ck( ew ) ){
			shl36_03_free( ew, pl_w ) ;
			return ;
		}
		hit_work_set( ew ) ;
		return ;
	}
	pls36_02_set( ew, 1 ) ;
	shl36_03_free( ew, pl_w ) ;
}

/*==============================================================*/
/*			The opponent is taken: when his hit stop is over	*/
/*			it takes hold of him								*/
/*==============================================================*/
static void shl36_03_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;
	Sint16 time ;

	pl_char_move( ew ) ;
	if( !em_w->hit_stop ){
		ew->routine_1 ++ ;
		pl_char_set( em_w, Char_Dizzy, 0 ) ;
		pl_ef_catch_set( ew, em_w ) ;
		ew->work1 = ( Sint32 )pl_w->catch_w ;
		time = 90 ;
		if( pl_w->team_no ) time = 60 ;
		ew->timer0 = time ;
		hold_renda_set( pl_w->catch_w, 1 ) ;
		em_w->catch_type = 6 ;
		em_w->catch_no = pl_w->catch_no = 0xc2 ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_grip_old = -1 ;
		if( em_w->pl_stance == STANCE_AIR ){
			em_w->spd.x /= 4.0f ;
			em_w->acc.x /= 4.0f ;
			if( em_w->spd.y > 0.0f ) em_w->spd.y /= 4.0f ;
			em_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/*==============================================================*/
/*			It holds him until his mashing frees him, and		*/
/*			the throw's own move goes on						*/
/*==============================================================*/
static void shl36_03_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = SHL36_03_EM( ew ) ;

	shl36_03_pos( ew, pl_w ) ;
	if( em_w->hit_box_no && em_w->routine_1 == PL_DAMAGE ){
		/* struck out of the hold: let go */
	} else if( ew == em_w->catch_pair.w ){
		if( hold_renda_move_ck( em_w ) ) ew->timer0 -= 3 ;
		if( -- ew->timer0 >= 0 ) return ;
		em_w->catch_type = 0 ;
		em_w->auto_guard_timer = 8 ;
		if( em_w->vital_new > 0 ){
			if( em_w->pl_stance != STANCE_AIR ){
				pl_stand_ret( em_w ) ;
			}
			else {
				pl_sjump_ret( em_w ) ;
			}
		}
	}
	pls36_02_set( ew, 2 ) ;
	shl36_03_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl36_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl36_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The grab keeps its place in front of the			*/
/*			opponent, and sways									*/
/*==============================================================*/
static void shl36_03_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 shl36_03_ofs[2] = {
		0, 64,
	} ;
	static Sint16 shl36_03_sway[4] = {
		-1, 0, 1, 0,
	} ;
	PLAYER_WORK *em_w = ew->em_w ;
	Sint16 x ;

	x = shl36_03_ofs[0] ;
	if( em_w->disp_dir ) x = -x ;
	ew->pos.x = em_w->pos.x + DOT_X( x ) ;
	ew->pos.y = em_w->pos.y + ( shl36_03_ofs[1] + shl36_03_sway[ew->obj_flag] ) * 2.142857f ;
	ew->obj_flag ++ ;
	ew->obj_flag = ew->obj_flag & 3 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl36_03.c" End of File	*/
