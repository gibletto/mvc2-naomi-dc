/* Anakaris's effect 04 (character 04's shot 04): the small form of a player under his curse
   (dm_no 20). It is drawn in the player's place, follows the player's routine_2 with its own
   animations, and goes when the curse ends. */
#include "player.h"
#include "pl_char.h"
#include "pl04_def.h"

static void shl04_04_main( PLAYER_WORK *ew );

/****************************************************************/
void shl04_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl04_04_main ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w->dm_hit_w ;
		ew->ef_no = 0x404 ;
		ew->be_flag = 1 ;
		if( pl_w->dm_hit_w->pl_type == PL_ANAKARIS ){
			pl04_voice_req( pl_w, pl_w->dm_hit_w, 45 ) ;
			pl04_voice_req( pl_w, pl_w->dm_hit_w, 48 ) ;
		}
		dust_set( pl_w, 0, 0, 2.0f, 2.0f ) ;
		dust_set( pl_w, 16, 0, 2.0f, 2.0f ) ;
		dust_set( pl_w, 0xf0, 0, 2.0f, 2.0f ) ;
	}
}

static void shl04_04_init( PLAYER_WORK *ew );
static void shl04_04_move( PLAYER_WORK *ew );
static void shl04_04_end( PLAYER_WORK *ew );
static void shl04_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_04_main( PLAYER_WORK *ew )
{
	static void ( *shl04_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_04_init, shl04_04_move, shl04_04_end, shl04_04_free,
	} ;

	shl04_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It takes Anakaris's colours, and the cursed			*/
/*			player's side										*/
/*==============================================================*/
static void shl04_04_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

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
	pl_w = ew->master_w ;
	ew->pl_no = pl_w->pl_no ;
	ew->work_no = pl_w->work_no ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	pl_shadow_set( ew ) ;
	ew->routine_2 = 0xff ;
	shl04_04_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_04_move( PLAYER_WORK *ew )
{
	/* the animation for each of the player's routine_2 */
	static Uint8 shl04_04_char_tbl[] = { 4, 0, 1, 2, 5, 4, 7, 0 } ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Float spd ;

	if( pl_w->routine_1 != PL_DAMAGE || pl_w->dm_no != 20 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		dust_set( pl_w, 0, 0, 2.0f, 2.0f ) ;
		dust_set( pl_w, 16, 0, 2.0f, 2.0f ) ;
		dust_set( pl_w, 0xf0, 0, 2.0f, 2.0f ) ;
		return ;
	}
	pl_w->disp_flag = 0 ;
	ew->pos = pl_w->pos ;
	ew->disp_dir = pl_w->disp_dir ;
	if( pl_w->routine_2 != ew->routine_2 ){
		ew->routine_2 = pl_w->routine_2 ;
		ew->char_no.b.no = shl04_04_char_tbl[ew->routine_2] ;
		if( ew->routine_2 == 4 ){
			if( pl_w->pl_stance == STANCE_AIR ) ew->char_no.b.no = 6 ;
			pl04_voice_req( ew->master_w, ew->ef_w, 47 ) ;
		}
		pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	}
	else pl_char_move( ew ) ;
	if( pl_w->routine_2 == 6 && !ew->cg_type ){
		if( pl_w->jump_dir > 0 ) spd = -2.5f ;
		else spd = DOT_X( 1.75f ) ;
		if( !pl_w->pl_dir ) spd = -spd ;
		pl_w->spd.x = spd ;
		pl_w->pos.x += pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void shl04_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl04_04.c" End of File	*/
