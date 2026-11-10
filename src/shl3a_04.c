/* Servbot's effect 04, of two types: the piece a servbot of shl3a_01 carries (type 0), and the
   pieces that fly off where he came in, fall, bounce and fade (type 1). */
#include "player.h"
#include "pl_char.h"
#include "pl3a_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL3A_04_LOCAL_W {
	Uint32	char_no;	/* 000: the master's animation when it was put out */
} SHL3A_04_LOCAL_W;
#define SHL3A_04_LW( w )	( ( SHL3A_04_LOCAL_W * )&( w )->obj_flag )

static void shl3a_04_main( PLAYER_WORK *ew );

/****************************************************************/
/*			No: type in the low byte, then sub_type and			*/
/*			sub_no; a negative one is not tied to pl_w			*/
/****************************************************************/
PLAYER_WORK *shl3a_04_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( no < 0 ) ew = work_get( 0, W_SHL, LIST_END ) ;
	else ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ;
	if( ew ){
		ew->work_func = shl3a_04_main ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w->master_w ;
		SHL3A_04_LW( ew )->char_no = pl_w->char_no.w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3a04 ;
		ew->type = no ;
		ew->sub_type = no >> 8 ;
		ew->sub_no = no >> 16 ;
	}
	return ew ;
}

static void shl3a_04_carry_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_04_item_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_04_main( PLAYER_WORK *ew )
{
	if( !ew->type ) shl3a_04_carry_main( ew, ew->master_w ) ;
	else shl3a_04_item_main( ew, ew->master_w ) ;
}

static void shl3a_04_carry_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_04_carry_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_04_carry_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl3a_04_carry_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_04_carry_init, shl3a_04_carry_move, shl3a_04_end, shl3a_04_free,
	} ;

	shl3a_04_carry_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl3a_04_carry_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->master_w->pl_no ;
	ew->pl_type = pl_w->master_w->pl_type ;
	ew->scale.x = pl_w->master_w->scale.x ;
	ew->scale.y = pl_w->master_w->scale.y ;
	ew->atck_level = pl_w->master_w->atck_level ;
	ew->work_no = pl_w->master_w->work_no ;
	ew->col_cnt = pl_w->master_w->col_cnt ;
	ew->scale = pl_w->master_w->scale ;
	ew->prio = pl_w->master_w->prio ;
	ew->sub_prio = -1 ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
	ew->dm_flag = HIT_PL_ONLY ;
	ew->atck_no = pl3a_at_fire ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 52 ) ;
	shl3a_04_carry_move( ew, pl_w ) ;
}

static void shl3a_04_item_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl3a_04_carry_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( pl_w->routine_0 > 1 ){
		ew->alpha = 0.0f ;
		shl3a_04_end( ew, pl_w ) ;
		return ;
	}
	if( pl_w->char_no.w != SHL3A_04_LW( ew )->char_no ){
		ew->alpha = 0.0f ;
		shl3a_04_end( ew, pl_w ) ;
		return ;
	}
	ew->prio = pl_w->prio ;
	x = DOT_X( pl_w->cg_type ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( pl_w->cg_prm ) ;
	pl_char_move( ew ) ;
	shl3a_04_item_hit( ew, pl_w ) ;
}

static void shl3a_04_item_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_04_item_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_04_item_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl3a_04_item_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_04_item_init, shl3a_04_item_move, shl3a_04_end, shl3a_04_free,
	} ;

	shl3a_04_item_jmp[ew->routine_0]( ew, pl_w ) ;
}

/* the sideways speed of each piece, by sub_type */
static Float shl3a_04_spd_tbl[] = { -DOT_X( 2.0f ), -2.5f, -1.6666666f, -DOT_X( 0.5f ), DOT_X( 0.5f ), 1.6666666f, 2.5f, DOT_X( 2.0f ) } ;

/*==============================================================*/
static void shl3a_04_item_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->ef_w->pl_no ;
	ew->pl_type = ew->ef_w->pl_type ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->atck_level = ew->ef_w->atck_level ;
	ew->work_no = ew->ef_w->work_no ;
	ew->col_cnt = ew->ef_w->col_cnt ;
	ew->scale = ew->ef_w->scale ;
	ew->prio = ew->ef_w->prio ;
	ew->prio = 8 ;
	ew->sub_no = 0 ;
	ew->pos = ew->ef_w->pos ;
	ew->timer0 = ( Rnd() & 31 ) + 180 ;
	x = shl3a_04_spd_tbl[ew->sub_type] ;
	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	ew->acc.y = -DOT_Y( 0.125f ) ;
	ew->spd.x += ( Sint16 )( ( Rnd() & 255 ) - 128 ) * 1.6666666f / 256.0f ;
	ew->spd.y += ( Sint16 )( ( Rnd() & 255 ) - 128 ) * 2.142857f / 256.0f ;
	ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
	ew->dm_flag = HIT_PL_ONLY ;
	ew->atck_no = pl3a_at_fire ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, ( Rnd() & 3 ) + 52 ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_04_item_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *mw = ew->ef_w ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < mw->ground_y ){
		if( !ew->routine_1 ){
			ew->routine_1 ++ ;
			ew->spd.x /= 4.0f ;
			ew->spd.y = -ew->spd.y / 2.0f ;
		} else {
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.y = 0.0f ;
		}
		ew->pos.y = mw->ground_y ;
		ew->sub_no = 1 ;
	}
	if( ew->timer0 <= 60 ){
		ew->disp_flag = 0 ;
		if( ( ew->sub_type + Sys->game_timer0 ) & 1 ) ew->disp_flag = 1 ;
	}
	if( -- ew->timer0 <= 0 ){
		ew->alpha = 0.0f ;
		shl3a_04_end( ew, pl_w ) ;
	} else shl3a_04_item_hit( ew, pl_w ) ;
}

/*==============================================================*/
/*			A piece that has landed can hit the opponent,		*/
/*			and is spent on it									*/
/*==============================================================*/
static void shl3a_04_item_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( !ew->sub_no ) return ;
	if( ew->hit_flag ){
		em_w = ew->em_w ;
		if( !em_w->team_no ){
			ew->routine_0 = 2 ;
			ew->spd.y = em_w->size_top ;
			ew->acc.y = DOT_Y( 5.0f ) ;
			if( em_w->vital_red > em_w->vital_new ){
				ew->acc.y = 15.0f ;
				vital_add_dmg( em_w, 1 ) ;
				puff_ef_set( em_w, 4 ) ;
			}
			pl_se_req( ew, 42 ) ;
			shl3a_04_end( ew, pl_w ) ;
			return ;
		} else {
			ew->hit_flag = 0 ;
		}
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			It follows the opponent's body, rises and fades		*/
/*==============================================================*/
static void shl3a_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;

	if( em_w ){
		ew->pos.x = em_w->pos.x ;
		ew->pos.y = em_w->pos.y + ew->spd.y ;
	}
	ew->spd.y += ew->acc.y ;
	ew->acc.y -= 2.142857f ;
	if( ew->acc.y < 0.0f ) ew->acc.y = 0.0f ;
	if( ( ew->alpha -= 0.02f ) < 0.0f ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		ew->alpha = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void shl3a_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl3a_04.c" End of File	*/
