/* Blackheart's effect 04 (character 35's shot 04): the bats and fiends of his attacks. Twelve
   kinds, by type; each flies out or stays beside the opponent it was set on, and ends when its
   animation is over or the opponent is out of reach. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "o_sub1_old.h"
#include "pl35_def.h"

#define SD	static

/* the effect's own work, from obj_flag */
typedef struct SHL35_04_LOCAL_W {
	PLAYER_WORK	*em_w;		/* the opponent it was set on */
	Uint8	char_grp;
	Uint8	char_no;
	Uint8	shot_no;
	Sint8	hold_no;
	Uint8	em_norm_flag;
	Uint8	hold_flag;
	Uint8	escape_cnt;
} SHL35_04_LOCAL_W;
#define SHL35_04_LW( w )	( ( SHL35_04_LOCAL_W * )EF_WK( w )->work )

SD void shl35_04_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A new one beside his master; `type` is the kind		*/
/*			and `no` its sub_type								*/
/****************************************************************/
PLAYER_WORK *shl35_04_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	SHL35_04_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		lw = SHL35_04_LW( ew ) ;
		ew->work_func = shl35_04_main ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3504 ;
		ew->type = type ;
		ew->sub_type = no ;
		lw->em_w = pl_w->target_w ;
	}
	return ew ;
}

/****************************************************************/
/*			The eight pieces (type 9) of crouching HK's fiend	*/
/****************************************************************/
SD PLAYER_WORK *shl35_04_chk_piece_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	SHL35_04_LOCAL_W *ew_lw, *pl_lw ;
	Sint32 i ;

	for( i = 0 ; i < 8 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
			ew_lw = SHL35_04_LW( ew ) ;
			pl_lw = SHL35_04_LW( pl_w ) ;
			ew->work_func = shl35_04_main ;
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->master_w = pl_w->master_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3504 ;
			ew->type = 9 ;
			ew->sub_type = i ;
			ew_lw->em_w = pl_lw->em_w ;
			ew->ef_w = pl_w ;
		}
	}
	return ew ;
}

SD void shl35_04_dummy( PLAYER_WORK *ew );
SD void shl35_04_hp( PLAYER_WORK *ew );
SD void shl35_04_chp( PLAYER_WORK *ew );
SD void shl35_04_jhp( PLAYER_WORK *ew );
SD void shl35_04_hk( PLAYER_WORK *ew );
SD void shl35_04_chk( PLAYER_WORK *ew );
SD void shl35_04_jhk( PLAYER_WORK *ew );
SD void shl35_04_mp( PLAYER_WORK *ew );
SD void shl35_04_cmp( PLAYER_WORK *ew );
SD void shl35_04_jump_mp( PLAYER_WORK *ew );
SD void shl35_04_chk_piece( PLAYER_WORK *ew );
SD void shl35_04_judgement( PLAYER_WORK *ew );
SD void shl35_04_jjudgement( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_main( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hp, shl35_04_chp, shl35_04_jhp, shl35_04_hk,
		shl35_04_chk, shl35_04_jhk, shl35_04_mp, shl35_04_cmp,
		shl35_04_jump_mp, shl35_04_chk_piece, shl35_04_judgement, shl35_04_jjudgement,
	} ;

	shl35_04_dummy( ew ) ;
	shl35_04_jmp[ew->type]( ew ) ;
}

/* the helpers at the end of the module */
extern void dm_ko_dir_set( PLAYER_WORK *dm_w, Uint8 no, Uint8 dir ) ;	/* pl_dmg.c */
SD void shl35_04_copy( PLAYER_WORK *ew );
SD void shl35_04_atck_set( PLAYER_WORK *ew );
SD void shl35_04_spd_set( PLAYER_WORK *ew );
SD void shl35_04_link( PLAYER_WORK *ew );
SD void shl35_04_follow( PLAYER_WORK *ew );
SD void shl35_04_sway( PLAYER_WORK *ew );
SD void shl35_04_pad_move( PLAYER_WORK *ew );
SD int shl35_04_dm_ck( PLAYER_WORK *ew, PLAYER_WORK *em_w );
SD int shl35_04_em_ck( PLAYER_WORK *ew );
SD int shl35_04_pl_ck( PLAYER_WORK *ew );
SD int shl35_04_end_ck( PLAYER_WORK *ew );
SD int shl35_04_ng_ck( PLAYER_WORK *ew );
SD int shl35_04_hit_ck( PLAYER_WORK *ew );
SD void shl35_04_place( PLAYER_WORK *ew );

/* where each kind of him sits on each opponent: place, mirror, priority and animation */
typedef struct SHL35_04_PLACE {
	Sint16	x;
	Sint16	y;
	Uint8	flip;
	Sint8	prio;
	Uint8	anim;
	Uint8	unk_07;
} SHL35_04_PLACE;
static SHL35_04_PLACE shl35_04_place_tbl[59][8] ;
SD void shl35_04_hit_ok( PLAYER_WORK *ew );
SD void shl35_04_to_end1( PLAYER_WORK *ew );
SD void shl35_04_to_end0( PLAYER_WORK *ew );
SD void shl35_04_color( PLAYER_WORK *ew );
SD void shl35_04_color1( PLAYER_WORK *ew );
SD int shl35_04_nop( PLAYER_WORK *ew );
SD void shl35_04_next( PLAYER_WORK *ew );
SD void shl35_04_free( PLAYER_WORK *ew );
SD void shl35_04_step( PLAYER_WORK *ew );

/* ---- type 0: the fiends of standing HP ---- */

SD void shl35_04_hp_init( PLAYER_WORK *ew );
SD void shl35_04_hp_move( PLAYER_WORK *ew );
SD void shl35_04_hp_end( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hp( PLAYER_WORK *ew )
{
	static void ( *shl35_04_hp_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hp_init, shl35_04_hp_move, shl35_04_hp_end, shl35_04_free,
	} ;

	shl35_04_hp_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_hp_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	shl35_04_link( ew ) ;
	ew->atck_no = pl35_at_fiend_hp ;
	HIT_RESET( ew ) ;
	shl35_04_spd_set( ew ) ;
	pl_char_set( ew, 25, 0 ) ;
}

SD void shl35_04_hp_fly( PLAYER_WORK *ew );
SD void shl35_04_hp_hold( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hp_move( PLAYER_WORK *ew )
{
	static void ( *shl35_04_hp_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hp_fly, shl35_04_hp_hold,
	} ;

	if( ew->hit_box_no ) shl35_04_to_end1( ew ) ;
	else if( shl35_04_end_ck( ew ) ) shl35_04_to_end0( ew ) ;
	else shl35_04_hp_move_jmp[ew->routine_1]( ew ) ;
}

SD void shl35_04_hp_fly_out( PLAYER_WORK *ew );
SD void shl35_04_hp_fly_sway( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hp_fly( PLAYER_WORK *ew )
{
	static void ( *shl35_04_hp_fly_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hp_fly_out, shl35_04_hp_fly_sway,
	} ;
	PLAYER_WORK *em_w ;

	if( ew->hit_flag ){
		em_w = ew->em_w ;
		if( em_w->work_kind == W_PL && em_w->team_no == 0 &&
			( em_w->pl_type < PL_ABYSS1 || em_w->pl_type > PL_ABYSS3 ) ){
			if( ew->hit_flag & 127 ) shl35_04_to_end0( ew ) ;
			else shl35_04_hit_ok( ew ) ;
		}
		else shl35_04_to_end0( ew ) ;
	}
	else shl35_04_hp_fly_jmp[ew->routine_2]( ew ) ;
}

/****************************************************************/
/*			Flies on, as long as his master is in the move		*/
/****************************************************************/
SD void shl35_04_hp_fly_out( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	if( pl_w->char_no.b.grp != Char_Stand_P || pl_w->char_no.b.no != 2 ){
		shl35_04_to_end0( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	if( ew->cg_prm >= 0 ){
		ew->routine_2 ++ ;
		ew->acc.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -2.142857f ;
		x = DOT_X( 4.0f ) ;
		if( !pl_w->pl_dir ) x = -DOT_X( 4.0f ) ;
		ew->spd.x = x ;
	}
	hit_work_set( ew ) ;
}

/****************************************************************/
SD void shl35_04_hp_fly_sway( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl35_04_sway( ew ) ;
	shl35_04_pad_move( ew ) ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = ew->acc.y = 0.0f ;
	}
	if( pl_char_move( ew ) < 0 ) shl35_04_to_end0( ew ) ;
	else hit_work_set( ew ) ;
}

static Float shl35_04_pl_tbl[59][2] ;

/****************************************************************/
/*			Goes to the opponent's side and holds there			*/
/****************************************************************/
SD void shl35_04_hp_hold( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w = lw->em_w ;
	Float x, ofs, spd, dy ;

	shl35_04_color( ew ) ;
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		ofs = -DOT_X( 16.0f ) ;
		x = ew->pos.x - em_w->pos.x ;
		if( x > 0.0f ) ofs = DOT_X( 16.0f ) ;
		x += ofs ;
		spd = x ;
		if( spd < 0.0f ) spd = -spd ;
		if( spd > shl35_04_pl_tbl[em_w->pl_type][0] ) spd = shl35_04_pl_tbl[em_w->pl_type][0] ;
		if( x < 0.0f ) spd = -spd ;
		ew->spd.x = spd ;
		dy = ew->pos.y - em_w->pos.y - shl35_04_pl_tbl[em_w->pl_type][1] ;
		if( dy < 0.0f ) ew->spd.y = shl35_04_pl_tbl[em_w->pl_type][1] ;
		else ew->spd.y = dy ;
		shl35_04_follow( ew ) ;
		pl_char_set( ew, 25, 1 ) ;
	}
	else if( !pl_lw->hold_timer || pl_w->team_no ) shl35_04_to_end0( ew ) ;
	else if( shl35_04_em_ck( ew ) || shl35_04_pl_ck( ew ) ) shl35_04_to_end1( ew ) ;
	else{
		shl35_04_follow( ew ) ;
		if( pl_char_move( ew ) < 0 && !shl35_04_dm_ck( ew, em_w ) ) shl35_04_to_end0( ew ) ;
	}
}

SD void shl35_04_hp_end_0( PLAYER_WORK *ew );
SD void shl35_04_hp_end_1( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hp_end( PLAYER_WORK *ew )
{
	static void ( *shl35_04_hp_end_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hp_end_0, shl35_04_hp_end_1,
	} ;

	shl35_04_hp_end_jmp[ew->routine_1]( ew ) ;
}

/****************************************************************/
SD void shl35_04_hp_end_0( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;

	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		if( lw->hold_no >= 0 ) shl35_04_color1( ew ) ;
		pl_char_set( ew, 25, 3 ) ;
		shl35_04_nop( ew ) ;
	}
	else if( pl_char_move( ew ) < 0 ) shl35_04_next( ew ) ;
	else shl35_04_nop( ew ) ;
}

/****************************************************************/
SD void shl35_04_hp_end_1( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;

	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		if( lw->hold_no >= 0 ) shl35_04_color1( ew ) ;
		pl_char_set( ew, 25, 2 ) ;
		shl35_04_nop( ew ) ;
	}
	else if( pl_char_move( ew ) < 0 ) shl35_04_next( ew ) ;
	else shl35_04_nop( ew ) ;
}

/* ---- type 1: the fiends of crouching HP ---- */

SD void shl35_04_chp_init( PLAYER_WORK *ew );
SD void shl35_04_chp_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_chp( PLAYER_WORK *ew )
{
	static void ( *shl35_04_chp_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_chp_init, shl35_04_chp_move, shl35_04_hp_end, shl35_04_free,
	} ;

	shl35_04_chp_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_chp_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	shl35_04_link( ew ) ;
	ew->atck_no = pl35_at_fiend_chp ;
	HIT_RESET( ew ) ;
	shl35_04_spd_set( ew ) ;
	pl_char_set( ew, 25, 0 ) ;
}

SD void shl35_04_chp_fly( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_chp_move( PLAYER_WORK *ew )
{
	static void ( *shl35_04_chp_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_chp_fly, shl35_04_hp_hold,
	} ;

	if( ew->hit_box_no ) shl35_04_to_end1( ew ) ;
	else if( shl35_04_end_ck( ew ) ) shl35_04_to_end0( ew ) ;
	else shl35_04_chp_move_jmp[ew->routine_1]( ew ) ;
}

SD void shl35_04_chp_fly_out( PLAYER_WORK *ew );
SD void shl35_04_chp_fly_sway( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_chp_fly( PLAYER_WORK *ew )
{
	static void ( *shl35_04_chp_fly_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_chp_fly_out, shl35_04_chp_fly_sway,
	} ;
	PLAYER_WORK *em_w ;

	if( ew->hit_flag ){
		em_w = ew->em_w ;
		if( em_w->work_kind == W_PL && em_w->team_no == 0 &&
			( em_w->pl_type < PL_ABYSS1 || em_w->pl_type > PL_ABYSS3 ) ){
			if( ew->hit_flag & 127 ) shl35_04_to_end0( ew ) ;
			else shl35_04_hit_ok( ew ) ;
		}
		else shl35_04_to_end0( ew ) ;
	}
	else if( shl35_04_ng_ck( ew ) ) shl35_04_to_end0( ew ) ;
	else shl35_04_chp_fly_jmp[ew->routine_2]( ew ) ;
}

/****************************************************************/
SD void shl35_04_chp_fly_out( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_2 ++ ;
		pl_char_set( ew, 25, 1 ) ;
		ew->acc.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -2.142857f ;
		x = DOT_X( 4.0f ) ;
		if( !pl_w->pl_dir ) x = -DOT_X( 4.0f ) ;
		ew->spd.x = x ;
	}
	hit_work_set( ew ) ;
}

/****************************************************************/
SD void shl35_04_chp_fly_sway( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl35_04_sway( ew ) ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	if( pl_char_move( ew ) < 0 ) shl35_04_to_end0( ew ) ;
	else hit_work_set( ew ) ;
}

/* ---- type 2: the fiends of jumping HP ---- */

SD void shl35_04_jhp_init( PLAYER_WORK *ew );
SD void shl35_04_jhp_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_jhp( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jhp_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_jhp_init, shl35_04_jhp_move, shl35_04_hp_end, shl35_04_free,
	} ;

	shl35_04_jhp_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_jhp_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	shl35_04_link( ew ) ;
	ew->atck_no = pl35_at_fiend_jhp ;
	HIT_RESET( ew ) ;
	shl35_04_spd_set( ew ) ;
	pl_char_set( ew, 25, 19 ) ;
}

SD void shl35_04_jhp_fly( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_jhp_move( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jhp_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_jhp_fly, shl35_04_hp_hold,
	} ;

	if( ew->hit_box_no ) shl35_04_to_end1( ew ) ;
	else if( shl35_04_end_ck( ew ) ) shl35_04_to_end0( ew ) ;
	else shl35_04_jhp_move_jmp[ew->routine_1]( ew ) ;
}

SD void shl35_04_jhp_fly_out( PLAYER_WORK *ew );
SD void shl35_04_jhp_fly_back( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_jhp_fly( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jhp_fly_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_jhp_fly_out, shl35_04_jhp_fly_back,
	} ;
	PLAYER_WORK *em_w ;

	if( ew->hit_flag ){
		em_w = ew->em_w ;
		if( em_w->work_kind == W_PL && em_w->team_no == 0 &&
			( em_w->pl_type < PL_ABYSS1 || em_w->pl_type > PL_ABYSS3 ) ){
			if( ew->hit_flag & 127 ) shl35_04_to_end0( ew ) ;
			else shl35_04_hit_ok( ew ) ;
		}
		else shl35_04_to_end0( ew ) ;
	}
	else if( shl35_04_ng_ck( ew ) ) shl35_04_to_end0( ew ) ;
	else shl35_04_jhp_fly_jmp[ew->routine_2]( ew ) ;
}

/****************************************************************/
SD void shl35_04_jhp_fly_out( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	x = ew->spd.x ;
	if( !ew->disp_dir ) x = -x ;
	if( x > 0.0f ){
		if( pl_w->ground_y > ew->pos.y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.y = 0.0f ;
			ew->acc.y = 0.0f ;
		}
		hit_work_set( ew ) ;
	}
	else{
		ew->routine_2 ++ ;
		ew->routine_3 = 0 ;
		shl35_04_jhp_fly_back( ew ) ;
	}
}

/****************************************************************/
/*			He turns round and flies back over the opponent		*/
/****************************************************************/
SD void shl35_04_jhp_fly_back( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	VEC3 to ;
	Float x, a, r ;
	Sint16 ang ;
	int dir ;

	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		x = DOT_X( 4.0f ) ;
		a = DOT_X( 0.25f ) ;
		if( !( ew->disp_dir ^= 1 ) ){
			x = -DOT_X( 4.0f ) ;
			a = -DOT_X( 0.25f ) ;
		}
		ew->spd.x = x ;
		ew->acc.x = a ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, 25, 20 ) ;
	}
	else{
		pl_w = ew->master_w ;
		to.x = pl_w->pos.x ;
		to.y = pl_w->pos.y + DOT_Y( 128.0f ) ;
		dir = caldir_pos_256( &ew->pos, &to ) * 2 ;
		r = 230400.0f ;
		ang = ( ( 48 - ( dir & 255 ) ) & 63 ) << 10 ;
		ew->spd.y = r * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		x = pl_w->pos.x - ew->pos.x ;
		if( !ew->disp_dir ) x = -x ;
		if( x < 0.0f ){
			shl35_04_to_end0( ew ) ;
			return ;
		}
		else pl_char_move( ew ) ;
	}
	hit_work_set( ew ) ;
}

/* ---- type 3: the fiends of standing HK ---- */

SD void shl35_04_hk_init( PLAYER_WORK *ew );
SD void shl35_04_hk_move( PLAYER_WORK *ew );
SD void shl35_04_hk_end( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hk( PLAYER_WORK *ew )
{
	static void ( *shl35_04_hk_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hk_init, shl35_04_hk_move, shl35_04_hk_end, shl35_04_free,
	} ;

	shl35_04_hk_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_hk_init( PLAYER_WORK *ew )
{
	static Uint8 shl35_04_hk_anim_tbl[8] = { 4, 6, 5, 6, 4, 5, 4, 4 } ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	shl35_04_spd_set( ew ) ;
	shl35_04_link( ew ) ;
	ew->atck_no = pl35_at_fiend_hk ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 25, shl35_04_hk_anim_tbl[ew->sub_type] ) ;
}

SD void shl35_04_hk_fly( PLAYER_WORK *ew );
SD void shl35_04_hk_hold( PLAYER_WORK *ew );
SD void shl35_04_hk_to_end1( PLAYER_WORK *ew );
SD void shl35_04_hk_to_end0( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hk_move( PLAYER_WORK *ew )
{
	static void ( *shl35_04_hk_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_hk_fly, shl35_04_hk_hold,
	} ;

	if( shl35_04_hit_ck( ew ) ) shl35_04_hk_to_end1( ew ) ;
	else if( shl35_04_end_ck( ew ) ) shl35_04_to_end0( ew ) ;
	else shl35_04_hk_move_jmp[ew->routine_1]( ew ) ;
}

SD void shl35_04_hk_hit_ok( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_hk_fly( PLAYER_WORK *ew )
{
	static Float shl35_04_hk_len_tbl[8] = {
		DOT_Y( 16.0f ), DOT_Y( 8.0f ), 0.0f, DOT_Y( 16.0f ), DOT_Y( 8.0f ), 0.0f, DOT_Y( 24.0f ), DOT_Y( 24.0f ),
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w ;

	if( ew->hit_flag ){
		em_w = ew->em_w ;
		if( em_w->work_kind == W_PL && em_w->team_no == 0 &&
			( em_w->pl_type < PL_ABYSS1 || em_w->pl_type > PL_ABYSS3 ) ){
			if( ew->hit_flag & 127 ) shl35_04_hk_to_end1( ew ) ;
			else shl35_04_hk_hit_ok( ew ) ;
		}
		else shl35_04_hk_to_end1( ew ) ;
	}
	else if( shl35_04_ng_ck( ew ) ) shl35_04_hk_to_end0( ew ) ;
	else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( pl_w->ground_y > ew->pos.y - shl35_04_hk_len_tbl[ew->sub_type] ){
			ew->pos.y = pl_w->ground_y - shl35_04_hk_len_tbl[ew->sub_type] ;
			shl35_04_hk_to_end1( ew ) ;
		}
		else{
			pl_char_move( ew ) ;
			hit_work_set( ew ) ;
		}
	}
}

/****************************************************************/
/*			Sits on the opponent, at the place his animation	*/
/*			gives												*/
/****************************************************************/
SD void shl35_04_hk_hold( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em_w = lw->em_w ;
	int dm_no ;

	shl35_04_color( ew ) ;
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		ew->disp_dir = em_w->disp_dir ;
		shl35_04_place( ew ) ;
		pl_char_set( ew, 25, shl35_04_place_tbl[em_w->pl_type][ew->sub_type].anim ) ;
	}
	else{
		shl35_04_place( ew ) ;
		if( !pl_lw->hold_timer || pl_w->team_no ){
			shl35_04_hk_to_end0( ew ) ;
			return ;
		}
		if( em_w->routine_1 != PL_DAMAGE ){
			shl35_04_hk_to_end1( ew ) ;
			return ;
		}
		dm_no = em_w->dm_no ;
		if( dm_no != 9 && dm_no != 23 ){
			shl35_04_hk_to_end1( ew ) ;
			return ;
		}
		em_w->dizzy_timer = 2 ;
		em_w->recover_timer = 2 ;
		if( shl35_04_pl_ck( ew ) ) shl35_04_hk_to_end1( ew ) ;
		else pl_char_move( ew ) ;
	}
}

/****************************************************************/
SD void shl35_04_hk_hit_ok( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL35_LOCAL_W *pl_lw ;

	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	lw->em_w = ew->em_w ;
	lw->hold_flag = 255 ;
	pl_w = ew->master_w ;
	pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	if( !pl_lw->hold_cnt ) pl_lw->hold_timer = 120 ;
	lw->hold_no = pl_lw->hold_cnt ;
	lw->escape_cnt = 0 ;
	pl_lw->hold_cnt ++ ;
}

/****************************************************************/
SD void shl35_04_hk_to_end0( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	ew->disp_flag = 1 ;
}

/****************************************************************/
SD void shl35_04_hk_to_end1( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 1 ;
	ew->routine_2 = 0 ;
	ew->disp_flag = 1 ;
}

/****************************************************************/
SD void shl35_04_hk_end( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;

	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		if( lw->hold_no >= 0 ) shl35_04_color1( ew ) ;
		if( !ew->routine_1 ) pl_char_set( ew, 25, 23 ) ;
		else pl_char_set( ew, 25, 10 ) ;
		shl35_04_nop( ew ) ;
	}
	else if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else shl35_04_nop( ew ) ;
}

/* ---- type 4: the fiend of crouching HK ---- */

SD void shl35_04_chk_init( PLAYER_WORK *ew );
SD void shl35_04_chk_move( PLAYER_WORK *ew );
SD void shl35_04_chk_end( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_chk( PLAYER_WORK *ew )
{
	static void ( *shl35_04_chk_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_chk_init, shl35_04_chk_move, shl35_04_chk_end, shl35_04_free,
	} ;

	shl35_04_chk_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_chk_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x, spd ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->timer0 = 96 ;
	x = DOT_X( 64.0f ) ;
	spd = DOT_X( 2.0f ) ;
	if( !pl_w->pl_dir ){
		x = -DOT_X( 64.0f ) ;
		spd = -DOT_X( 2.0f ) ;
	}
	ew->pos.x += x ;
	ew->spd.x = spd ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	shl35_04_link( ew ) ;
	ew->atck_no = pl35_at_fiend_chk ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 25, 13 ) ;
}

SD void shl35_04_chk_fly( PLAYER_WORK *ew );
SD void shl35_04_chk_hold( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_chk_move( PLAYER_WORK *ew )
{
	static void ( *shl35_04_chk_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_chk_fly, shl35_04_chk_hold,
	} ;

	if( ew->hit_box_no || shl35_04_end_ck( ew ) ) shl35_04_hk_to_end0( ew ) ;
	else shl35_04_chk_move_jmp[ew->routine_1]( ew ) ;
}

SD void shl35_04_chk_hit_ok( PLAYER_WORK *ew );

/****************************************************************/
/*			Flies out, for 96 frames at most					*/
/****************************************************************/
SD void shl35_04_chk_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *em_w ;

	if( ew->hit_flag ){
		em_w = ew->em_w ;
		if( em_w->work_kind == W_PL && em_w->team_no == 0 &&
			( em_w->pl_type < PL_ABYSS1 || em_w->pl_type > PL_ABYSS3 ) ){
			if( ew->hit_flag & 127 ) shl35_04_hk_to_end0( ew ) ;
			else shl35_04_chk_hit_ok( ew ) ;
		}
		else shl35_04_hk_to_end0( ew ) ;
	}
	else if( !shl35_04_ng_ck( ew ) && -- ew->timer0 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	}
	else shl35_04_hk_to_end0( ew ) ;
}

/****************************************************************/
/*			Holds on the opponent, and makes its eight			*/
/*			pieces (type 9)										*/
/****************************************************************/
SD void shl35_04_chk_hold( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL35_LOCAL_W *pl_lw ;
	PLAYER_WORK *em_w ;
	int dm_no ;

	shl35_04_color( ew ) ;
	pl_w = ew->master_w ;
	pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		shl35_04_chk_piece_set( ew ) ;
	}
	if( !pl_lw->hold_timer || pl_w->team_no ){
		shl35_04_hk_to_end0( ew ) ;
		return ;
	}
	em_w = lw->em_w ;
	ew->pos.x = em_w->pos.x ;
	if( em_w->routine_1 == PL_DAMAGE ){
		dm_no = em_w->dm_no ;
		if( dm_no == 9 || dm_no == 23 ){
			em_w->dizzy_timer = 2 ;
			em_w->recover_timer = 2 ;
			if( !shl35_04_pl_ck( ew ) ){
				pl_char_move( ew ) ;
				return ;
			}
		}
	}
	shl35_04_hk_to_end1( ew ) ;
}

/****************************************************************/
SD void shl35_04_chk_hit_ok( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL35_LOCAL_W *pl_lw ;

	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	lw->em_w = ew->em_w ;
	lw->hold_flag = 255 ;
	pl_w = ew->master_w ;
	pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	ew->disp_flag = 0 ;
	if( !pl_lw->hold_cnt ) pl_lw->hold_timer = 120 ;
	lw->hold_no = pl_lw->hold_cnt ;
	pl_lw->hold_cnt ++ ;
	lw->escape_cnt = 0 ;
}

/****************************************************************/
SD void shl35_04_chk_end( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;

	if( !ew->routine_1 ){
		if( !ew->routine_2 ){
			ew->routine_2 ++ ;
			ew->disp_flag = 1 ;
			if( lw->hold_no >= 0 ) shl35_04_color1( ew ) ;
			pl_char_set( ew, 25, 17 ) ;
			shl35_04_nop( ew ) ;
		}
		else if( pl_char_move( ew ) >= 0 ) shl35_04_nop( ew ) ;
		else{
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
	else{
		if( lw->hold_no >= 0 ) shl35_04_color1( ew ) ;
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/* ---- type 5: the fiends of jumping HK ---- */

SD void shl35_04_jhk_init( PLAYER_WORK *ew );
SD void shl35_04_jhk_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_jhk( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jhk_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_jhk_init, shl35_04_jhk_move, shl35_04_hk_end, shl35_04_free,
	} ;

	shl35_04_jhk_jmp[ew->routine_0]( ew ) ;
}

/* where each of the five starts, and how it flies */
typedef struct SHL35_04_FLY {
	Float	spd_x;
	Float	acc_x;
	Float	spd_y;
	Float	acc_y;
} SHL35_04_FLY;

/****************************************************************/
SD void shl35_04_jhk_init( register PLAYER_WORK *ew )
{
	static Float shl35_04_jhk_pos_tbl[8][2] = {
		80.0f, 180.0f,
		80.0f, 180.0f,
		DOT_X( 56.0f ), DOT_Y( 88.0f ),
		DOT_X( 56.0f ), DOT_Y( 88.0f ),
		DOT_X( 64.0f ), DOT_Y( 92.0f ),
		DOT_X( 64.0f ), DOT_Y( 92.0f ),
		DOT_X( 64.0f ), DOT_Y( 47.0f ),
		DOT_X( 64.0f ), DOT_Y( 47.0f ),
	} ;
	static SHL35_04_FLY shl35_04_jhk_fly_tbl[8] = {
		DOT_X( 4.0f ), 0.0f, -7.5f, -DOT_Y( 0.125f ),
		7.5f, 0.0f, -DOT_Y( 3.0f ), -DOT_Y( 0.125f ),
		DOT_X( 4.0f ), 0.0f, -DOT_Y( 4.5f ), -DOT_Y( 0.125f ),
		7.5f, 0.0f, -DOT_Y( 4.0f ), -DOT_Y( 0.125f ),
		DOT_X( 4.0f ), 0.0f, -DOT_Y( 5.5f ), -0.4017857f,
		7.5f, 0.0f, -DOT_Y( 5.0f ), -2.5446427f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
	} ;
	static Float shl35_04_jhk_anim_tbl[8] = { 21.0f, 22.0f, 21.0f, 22.0f, 21.0f, 22.0f, 21.0f, 21.0f } ;
	PLAYER_WORK *pl_w ;
	register Float x, spd, acc ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	x = shl35_04_jhk_pos_tbl[ew->sub_type][0] ;
	if( !pl_w->pl_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + shl35_04_jhk_pos_tbl[ew->sub_type][0] ;
	ew->spd.y = shl35_04_jhk_fly_tbl[ew->sub_type].spd_y ;
	ew->acc.y = shl35_04_jhk_fly_tbl[ew->sub_type].acc_y ;
	spd = shl35_04_jhk_fly_tbl[ew->sub_type].spd_x ;
	acc = shl35_04_jhk_fly_tbl[ew->sub_type].acc_x ;
	if( !pl_w->pl_dir ){
		spd = -spd ;
		acc = -acc ;
	}
	ew->spd.x = spd ;
	ew->acc.x = acc ;
	shl35_04_link( ew ) ;
	ew->atck_no = pl35_at_fiend_jhk ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 25, shl35_04_jhk_anim_tbl[ew->sub_type] ) ;
}

/****************************************************************/
SD void shl35_04_jhk_move( PLAYER_WORK *ew )
{
	static Float shl35_04_jhk_len_tbl[8] = {
		0.0f, DOT_Y( 8.0f ), 0.0f, DOT_Y( 8.0f ), 0.0f, DOT_Y( 8.0f ), DOT_Y( 24.0f ), DOT_Y( 24.0f ),
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w ;

	if( shl35_04_hit_ck( ew ) ) shl35_04_hk_to_end1( ew ) ;
	else if( shl35_04_end_ck( ew ) ) shl35_04_hk_to_end0( ew ) ;
	else if( !ew->routine_1 ){
		if( ew->hit_flag ){
			em_w = ew->em_w ;
			if( em_w->work_kind == W_PL && em_w->team_no == 0 &&
				( em_w->pl_type < PL_ABYSS1 || em_w->pl_type > PL_ABYSS3 ) ){
				if( ew->hit_flag & 127 ) shl35_04_hk_to_end1( ew ) ;
				else shl35_04_hk_hit_ok( ew ) ;
			}
			else shl35_04_hk_to_end1( ew ) ;
		}
		else if( shl35_04_ng_ck( ew ) ) shl35_04_hk_to_end0( ew ) ;
		else{
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			if( pl_w->ground_y - shl35_04_jhk_len_tbl[ew->sub_type] > ew->pos.y ){
				ew->pos.y = pl_w->ground_y + shl35_04_jhk_len_tbl[ew->sub_type] ;
				shl35_04_hk_to_end1( ew ) ;
			}
			else{
				pl_char_move( ew ) ;
				hit_work_set( ew ) ;
			}
		}
	}
	else shl35_04_hk_hold( ew ) ;
}

/* ---- type 6: the fiend of standing MP ---- */

SD void shl35_04_mp_init( PLAYER_WORK *ew );
SD void shl35_04_mp_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_mp( PLAYER_WORK *ew )
{
	static void ( *shl35_04_mp_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_mp_init, shl35_04_mp_move, shl35_04_next, shl35_04_free,
	} ;

	shl35_04_mp_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_mp_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

/****************************************************************/
SD void shl35_04_mp_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp == Char_Stand_P && pl_w->char_no.b.no == 1 ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		if( pl_char_move( ew ) >= 0 ) return ;
	}
	shl35_04_step( ew ) ;
}

/* ---- type 7: the fiend of crouching MP, and the snapback's ---- */

SD void shl35_04_cmp_init( PLAYER_WORK *ew );
SD void shl35_04_cmp_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_cmp( PLAYER_WORK *ew )
{
	static void ( *shl35_04_cmp_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_cmp_init, shl35_04_cmp_move, shl35_04_next, shl35_04_free,
	} ;

	shl35_04_cmp_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_cmp_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	shl35_04_copy( ew ) ;
	shl35_04_atck_set( ew ) ;
	if( !ew->sub_type ){
		ew->atck_no = ATCK_CROUCH_MP ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl35_at_snapback ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, Char_Effect, 5 ) ;
}

/****************************************************************/
SD void shl35_04_cmp_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->sub_type ){
		if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Crouch_P || pl_w->char_no.b.no != 1 ){
			shl35_04_step( ew ) ;
			return ;
		}
	}
	else if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Special || pl_w->char_no.b.no != 26 ){
		shl35_04_step( ew ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_char_move( ew ) < 0 ) shl35_04_step( ew ) ;
	else hit_work_set( ew ) ;
}

/* ---- type 8: the fiend of jumping MP ---- */

SD void shl35_04_jump_mp_init( PLAYER_WORK *ew );
SD void shl35_04_jump_mp_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_jump_mp( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jump_mp_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_jump_mp_init, shl35_04_jump_mp_move, shl35_04_next, shl35_04_free,
	} ;

	shl35_04_jump_mp_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_jump_mp_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	shl35_04_copy( ew ) ;
	shl35_04_atck_set( ew ) ;
	ew->atck_no = ATCK_JUMP_MP ;
	HIT_RESET( ew ) ;
	shl35_04_jump_mp_move( ew ) ;
}

/****************************************************************/
SD void shl35_04_jump_mp_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Jump_P || pl_w->char_no.b.no != 1 ) shl35_04_step( ew ) ;
	else if( pl_w->cg_type < 0 ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_flag = 1 ;
		pl_char_set2( ew, Char_Effect, 6, pl_w->cg_type & 127 ) ;
		hit_work_set( ew ) ;
	}
	else ew->disp_flag = 0 ;
}

/* ---- type 9: the pieces of crouching HK's fiend ---- */

/****************************************************************/
/*			The eight that hang from crouching HK's fiend:		*/
/*			each has its own animation, priority and place		*/
/****************************************************************/
SD void shl35_04_chk_piece( PLAYER_WORK *ew )
{
	static Uint8 shl35_04_chk_piece_anim_tbl[8] = { 25, 25, 25, 26, 26, 26, 24, 24 } ;
	static Uint8 shl35_04_chk_piece_prio_tbl[8] = { 12, 12, 12, 11, 11, 11, 10, 10 } ;
	static Float shl35_04_chk_piece_pos_tbl[8][2] = {
		-DOT_X( 2.0f ), DOT_Y( 23.0f ),
		-5.0f, DOT_Y( 17.0f ),
		DOT_X( 38.0f ), DOT_Y( 18.0f ),
		DOT_X( 40.0f ), 0.0f,
		-DOT_X( 52.0f ), 15.0f,
		-DOT_X( 2.0f ), DOT_Y( 2.0f ),
		-DOT_X( 2.0f ), DOT_Y( 11.0f ),
		-DOT_X( 2.0f ), DOT_Y( 11.0f ),
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *ef_w, *em_w ;
	Float x ;

	if( pl_w->hyper_stop_flag ) shl35_04_free( ew ) ;
	else if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		shl35_04_copy( ew ) ;
		ew->prio = shl35_04_chk_piece_prio_tbl[ew->sub_type] ;
		pl_char_set( ew, 25, shl35_04_chk_piece_anim_tbl[ew->sub_type] ) ;
	}
	else if( ( ef_w = ew->ef_w )->routine_0 >= 2 ) shl35_04_free( ew ) ;
	else{
		ew->disp_dir = ef_w->disp_dir ;
		em_w = lw->em_w ;
		ew->pos.x = em_w->pos.x ;
		ew->pos.y = em_w->ground_y + shl35_04_chk_piece_pos_tbl[ew->sub_type][1] ;
		x = shl35_04_chk_piece_pos_tbl[ew->sub_type][0] ;
		if( !ew->disp_dir ) x = -x ;
		ew->pos.x += x ;
		pl_char_move( ew ) ;
	}
}

/* ---- type 10: Judgement Day on the ground ---- */

SD void shl35_04_judgement_init( PLAYER_WORK *ew );
SD void shl35_04_judgement_move( PLAYER_WORK *ew );
SD void shl35_04_shake( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_judgement( PLAYER_WORK *ew )
{
	static void ( *shl35_04_judgement_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_judgement_init, shl35_04_judgement_move,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->hyper_stop_flag ) shl35_04_free( ew ) ;
	else shl35_04_judgement_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_judgement_init( PLAYER_WORK *ew )
{
	static Float shl35_04_judgement_spd_tbl[8][2] = {
		DOT_X( 7.0f ), DOT_Y( 2.0f ),
		DOT_X( 9.5f ), DOT_Y( 0.5f ),
		15.0f, 3.75f,
		DOT_X( 9.5f ), DOT_Y( 0.75f ),
		DOT_X( 8.5f ), DOT_Y( 1.5f ),
		DOT_X( 10.0f ), DOT_Y( 0.125f ),
		16.25f, DOT_Y( 2.125f ),
		DOT_X( 8.75f ), 2.142857f,
	} ;
	PLAYER_WORK *pl_w ;
	Float x, y ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( Sys->game_timer0 & 1 ){
		ew->atck_no = pl35_at_judgement_1 ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl35_at_judgement_2 ;
		HIT_RESET( ew ) ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + 240.0f ;
	ew->timer0 = 48 ;
	pl_char_set( ew, 25, 27 ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	x = shl35_04_judgement_spd_tbl[Rnd() & 7][0] ;
	if( !ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	y = shl35_04_judgement_spd_tbl[Rnd() & 7][1] ;
	if( Rnd() & 1 ) y = -y ;
	ew->spd.y = y ;
	shl35_04_shake( ew ) ;
}

/****************************************************************/
SD void shl35_04_judgement_move( PLAYER_WORK *ew )
{
	if( !ew->routine_2 ){
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( -- ew->timer0 < 0 || ew->hit_box_no || ew->hit_flag ){
			ew->routine_2 ++ ;
			pl_char_set( ew, 25, 2 ) ;
		}
		hit_work_set( ew ) ;
	}
	else if( pl_char_move( ew ) < 0 ) shl35_04_free( ew ) ;
}

/* ---- type 11: Judgement Day in the air ---- */

SD void shl35_04_jjudgement_init( PLAYER_WORK *ew );
SD void shl35_04_jjudgement_move( PLAYER_WORK *ew );

/****************************************************************/
SD void shl35_04_jjudgement( PLAYER_WORK *ew )
{
	static void ( *shl35_04_jjudgement_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_04_jjudgement_init, shl35_04_jjudgement_move,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->hyper_stop_flag ) shl35_04_free( ew ) ;
	else shl35_04_jjudgement_jmp[ew->routine_0]( ew ) ;
}

/****************************************************************/
SD void shl35_04_jjudgement_init( PLAYER_WORK *ew )
{
	static Float shl35_04_jjudgement_spd_tbl[8][2] = {
		5.0f, -DOT_Y( 6.0f ),
		DOT_X( 6.5f ), -DOT_Y( 5.5f ),
		DOT_X( 5.0f ), -DOT_Y( 5.75f ),
		DOT_X( 3.5f ), -DOT_Y( 4.75f ),
		7.5f, -DOT_Y( 7.5f ),
		DOT_X( 5.125f ), -DOT_Y( 5.125f ),
		6.25f, -DOT_Y( 5.125f ),
		DOT_X( 4.75f ), -DOT_Y( 6.25f ),
	} ;
	PLAYER_WORK *pl_w ;
	Float x ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	shl35_04_copy( ew ) ;
	x = 55.0f ;
	if( !ew->disp_dir ) x = -55.0f ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 96.0f ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl35_at_jjudgement_2 ;
	HIT_RESET( ew ) ;
	if( Sys->game_timer0 & 1 ) ew->atck_no = pl35_at_jjudgement_1 ;
	else ew->atck_no = pl35_at_jjudgement_2 ;
	HIT_RESET( ew ) ;
	ew->timer0 = 96 ;
	pl_char_set( ew, 25, 21 ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	x = shl35_04_jjudgement_spd_tbl[Rnd() & 7][0] ;
	if( !ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->spd.y = shl35_04_jjudgement_spd_tbl[Rnd() & 7][1] ;
	shl35_04_shake( ew ) ;
}

/****************************************************************/
SD void shl35_04_jjudgement_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->routine_2 ){
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( pl_w->ground_y > ew->pos.y ){
			ew->routine_2 ++ ;
			ew->pos.y = pl_w->ground_y ;
			pl_char_set( ew, 25, 2 ) ;
			return ;
		}
		if( -- ew->timer0 < 0 || ew->hit_box_no || ew->hit_flag ){
			ew->routine_2 ++ ;
			pl_char_set( ew, 25, 2 ) ;
		}
		hit_work_set( ew ) ;
	}
	else if( pl_char_move( ew ) < 0 ) shl35_04_free( ew ) ;
}

/****************************************************************/
/*			A little jitter in the place						*/
/****************************************************************/
SD void shl35_04_shake( PLAYER_WORK *ew )
{
	static Float shl35_04_shake_tbl[8][2] = {
		-DOT_X( 8.0f ), -0.06696428f,
		-10.0f, -0.050223213f,
		-20.0f, -0.100446425f,
		-DOT_X( 4.0f ), -0.03348214f,
		DOT_X( 4.0f ), 0.03348214f,
		20.0f, 0.100446425f,
		0.0f, 0.0f,
		DOT_X( 2.0f ), 0.01674107f,
	} ;

	ew->pos.x += shl35_04_shake_tbl[Rnd() & 7][0] ;
	ew->pos.y += shl35_04_shake_tbl[Rnd() & 7][1] ;
}

/* ---- the helpers ---- */

/****************************************************************/
/*			The opponent holds still, and goes down when its	*/
/*			life is gone: 0, or -1 while it lasts				*/
/****************************************************************/
SD int shl35_04_dm_ck( PLAYER_WORK *ew, PLAYER_WORK *dm_w )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *em_w = lw->em_w ;

	if( em_w->routine_1 ) return -1 ;
	if( Sys->game_timer0 & 1 ) hold_damage( em_w, -1 ) ;
	if( em_w->vital_new > 0 ) return -1 ;
	else{
		dm_ko_dir_set( em_w, 11, 28 ) ;
		return 0 ;
	}
}

/****************************************************************/
/*			Stays on the opponent's place, and turns to the		*/
/*			way it goes											*/
/****************************************************************/
SD void shl35_04_follow( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *em_w = lw->em_w ;
	Float x ;

	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->pos.y ;
	x = ew->spd.x ;
	if( x > 0.0f ) ew->disp_dir = 0 ;
	else ew->disp_dir = 1 ;
	if( ew->dir != em_w->disp_dir ){
		ew->disp_dir ^= 1 ;
		x = -x ;
	}
	ew->pos.x += x ;
	ew->pos.y += ew->spd.y ;
}

/****************************************************************/
/*			0 until it is hit, and then -1 (and the opponent	*/
/*			thrown is let go)									*/
/****************************************************************/
SD int shl35_04_hit_ck( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL35_LOCAL_W *pl_lw ;
	PLAYER_WORK *em_w ;

	if( !ew->hit_box_no ) return 0 ;
	pl_w = ew->master_w ;
	pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	if( !pl_lw->hold_cnt && lw->hold_flag ){
		em_w = lw->em_w ;
		if( em_w->routine_1 == PL_DAMAGE ){
			if( em_w->dm_no == 9 ){
				em_w->dizzy_timer = 0 ;
				em_w->recover_timer = 0 ;
				return -1 ;
			}
			if( em_w->dm_no == 23 ){
				em_w->dizzy_timer = 0 ;
				em_w->recover_timer = 0 ;
			}
		}
	}
	return -1 ;
}

/****************************************************************/
SD int shl35_04_em_ck( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *em_w = lw->em_w ;

	if( !em_w->vital_new ) return -1 ;
	if( !em_w->routine_1 ){
		lw->em_norm_flag = 255 ;
		return 0 ;
	}
	if( lw->em_norm_flag ) return -1 ;
	return 0 ;
}

/****************************************************************/
/*			It rises and falls a little as it goes				*/
/****************************************************************/
SD void shl35_04_sway( PLAYER_WORK *ew )
{
	Sint16 ang ;
	Float r ;

	ew->dir = ++ ew->dir & 31 ;
	r = 38400.0f ;
	ang = ( ( 40 - ew->dir ) & 31 ) << 11 ;
	ew->pos.y = ew->pos.y + r * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
}

/****************************************************************/
/*			The buttons of the master's side push it up and		*/
/*			down												*/
/****************************************************************/
SD void shl35_04_pad_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint16 sw ;

	if( ( sw = pl_w->pl_sw_new & ( KEY_UP | KEY_DOWN ) ) ){
		if( sw == KEY_DOWN ) ew->pos.y -= DOT_Y( 0.5f ) ;
		else ew->pos.y += DOT_Y( 1.5f ) ;
	}
}

/****************************************************************/
/*			-1 when the one that was hit is let go: its			*/
/*			master has gone on to the next, or its stick		*/
/*			moves												*/
/****************************************************************/
SD int shl35_04_pl_ck( PLAYER_WORK *ew )
{
	static Sint8 shl35_04_pl_ck_tbl[32] = {
		8, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
	} ;
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *tw ;

	if( pl_lw->free_cnt == lw->hold_no ){
		tw = pl_w->target_w ;
		if( tw->pl_sw_now & KEY_LEVER ) lw->escape_cnt += 2 ;
		if( tw->pl_sw_now & KEY_ATCK ) lw->escape_cnt += 2 ;
		if( lw->escape_cnt > shl35_04_pl_ck_tbl[lw->hold_no] ){
			pl_lw->free_cnt ++ ;
			return 1 ;
		}
	}
	else if( pl_lw->free_cnt > lw->hold_no ){
		pl_lw->free_cnt ++ ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			What it follows: its master's animation, and the	*/
/*			master's own count									*/
/****************************************************************/
SD void shl35_04_link( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;

	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	lw->char_grp = pl_w->char_no.b.grp ;
	lw->char_no = pl_w->char_no.b.no ;
	lw->em_norm_flag = 0 ;
	lw->shot_no = pl_lw->shot_no ;
	lw->hold_no = -1 ;
}

/****************************************************************/
SD void shl35_04_atck_set( PLAYER_WORK *ew )
{
	ew->atck_flag = HIT_ON | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_OWNER ;
}

/****************************************************************/
/*			It starts at the place beside the opponent that		*/
/*			its kind and number give							*/
/****************************************************************/
SD void shl35_04_spd_set( PLAYER_WORK *ew )
{
	static Float shl35_04_pos_tbl[23][2] = {
		-DOT_X( 8.0f ), DOT_Y( 120.0f ),
		0.0f, 270.0f,
		DOT_X( 8.0f ), DOT_Y( 120.0f ),
		20.0f, DOT_Y( 120.0f ),
		DOT_X( 16.0f ), DOT_Y( 128.0f ),
		120.0f, DOT_Y( 32.0f ),
		DOT_X( 64.0f ), DOT_Y( 32.0f ),
		DOT_X( 56.0f ), DOT_Y( 32.0f ),
		80.0f, DOT_Y( 32.0f ),
		DOT_X( 40.0f ), DOT_Y( 32.0f ),
		120.0f, DOT_Y( 128.0f ),
		DOT_X( 64.0f ), DOT_Y( 128.0f ),
		DOT_X( 56.0f ), DOT_Y( 128.0f ),
		80.0f, DOT_Y( 128.0f ),
		DOT_X( 40.0f ), DOT_Y( 128.0f ),
		DOT_X( 82.0f ), DOT_Y( 47.0f ),
		DOT_X( 82.0f ), DOT_Y( 47.0f ),
		DOT_X( 82.0f ), DOT_Y( 47.0f ),
		DOT_X( 65.0f ), DOT_Y( 47.0f ),
		DOT_X( 65.0f ), DOT_Y( 47.0f ),
		DOT_X( 65.0f ), DOT_Y( 47.0f ),
		DOT_X( 65.0f ), DOT_Y( 47.0f ),
		DOT_X( 65.0f ), DOT_Y( 47.0f ),
	} ;
	static SHL35_04_FLY shl35_04_fly_tbl[23] = {
		DOT_X( 0.5f ), -0.11067708f, 2.142857f, -0.4017857f,
		DOT_X( 1.375f ), -0.13671875f, 0.0f, -0.3348214f,
		1.875f, -0.11067708f, 0.0f, -0.46875f,
		DOT_X( 0.5f ), -0.13671875f, 2.0089285f, -0.9375f,
		1.25f, -0.052083332f, 2.142857f, -0.8705357f,
		-0.104166664f, 0.052083332f, 4.1643414f, -0.20089285f,
		-0.104166664f, 0.078125f, 2.0758927f, -0.20089285f,
		0.3125f, 0.0f, 2.0842633f, -0.20089285f,
		0.3125f, 0.0081380205f, 4.1685266f, -0.20089285f,
		0.3125f, 0.008951822f, 2.1010044f, -0.20089285f,
		DOT_X( 4.0f ), -0.16927083f, -DOT_Y( 3.0f ), 0.0f,
		7.5f, -0.15625f, -DOT_Y( 4.0f ), 0.0f,
		DOT_X( 4.0f ), -0.234375f, -DOT_Y( 5.0f ), 0.0f,
		7.5f, -0.26041666f, -DOT_Y( 6.0f ), 0.0f,
		DOT_X( 4.0f ), -0.15625f, -15.0f, 0.0f,
		6.25f, 0.0f, DOT_Y( 8.0f ), -1.0044643f,
		DOT_X( 4.0f ), 0.0f, DOT_Y( 4.0f ), -DOT_Y( 0.5f ),
		7.5f, 0.0f, DOT_Y( 8.0f ), -1.2723213f,
		DOT_X( 3.25f ), 0.0f, DOT_Y( 8.0f ), -1.2053571f,
		DOT_X( 4.75f ), 0.052083332f, DOT_Y( 8.0f ), -DOT_Y( 0.625f ),
		DOT_X( 5.0f ), 0.052083332f, DOT_Y( 8.0f ), -1.875f,
		DOT_X( 4.0f ), 0.052083332f, DOT_Y( 8.0f ), -DOT_Y( 0.5f ),
		5.0f, 0.052083332f, DOT_Y( 8.0f ), -DOT_Y( 0.625f ),
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x, spd, acc ;

	x = shl35_04_pos_tbl[ew->type * 5 + ew->sub_type][0] ;
	if( !pl_w->pl_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += shl35_04_pos_tbl[ew->type * 5 + ew->sub_type][1] ;
	ew->spd.y = shl35_04_fly_tbl[ew->type * 5 + ew->sub_type].spd_y ;
	ew->acc.y = shl35_04_fly_tbl[ew->type * 5 + ew->sub_type].acc_y ;
	spd = shl35_04_fly_tbl[ew->type * 5 + ew->sub_type].spd_x ;
	acc = shl35_04_fly_tbl[ew->type * 5 + ew->sub_type].acc_x ;
	if( !pl_w->disp_dir ){
		spd = -spd ;
		acc = -acc ;
	}
	ew->spd.x = spd ;
	ew->acc.x = acc ;
}

static Float shl35_04_pl_tbl[59][2] = {
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 40.0f ), DOT_Y( 0.125f ),
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 40.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.20089285f,
	DOT_X( 32.0f ), DOT_Y( 0.125f ),
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	80.0f, DOT_Y( 0.125f ),
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 64.0f ), DOT_Y( 0.125f ),
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	40.0f, 0.06696428f,
	40.0f, 0.06696428f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	80.0f, 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 64.0f ), DOT_Y( 0.125f ),
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.13392857f,
	DOT_X( 32.0f ), 0.06696428f,
} ;

static SHL35_04_PLACE shl35_04_place_tbl[59][8] = {
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 0, 76, 1, 10, 8, 0 }, { 8, 40, 1, 11, 9, 0 }, { -46, 36, 0, 12, 7, 0 }, { -52, 96, 0, 11, 9, 0 }, { -30, 54, 0, 10, 8, 0 }, { 12, 88, 1, 12, 7, 0 }, { 48, 30, 1, 11, 9, 0 }, { -44, 76, 0, 10, 8, 0 } },
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 13, 48, 1, 10, 8, 0 }, { 0, 84, 1, 10, 8, 0 }, { -16, 36, 0, 12, 7, 0 }, { 32, 48, 1, 11, 9, 0 }, { -84, 96, 0, 10, 9, 0 }, { 24, 36, 1, 12, 7, 0 }, { -48, 48, 0, 11, 9, 0 }, { -32, 96, 1, 10, 8, 0 } },
	{ { 11, 28, 1, 11, 9, 0 }, { 16, 52, 1, 10, 8, 0 }, { -24, 36, 0, 12, 7, 0 }, { -16, 64, 0, 11, 9, 0 }, { -24, 48, 0, 10, 8, 0 }, { 12, 62, 1, 12, 7, 0 }, { 10, 67, 1, 11, 9, 0 }, { 37, 19, 1, 10, 8, 0 } },
	{ { -5, 66, 1, 10, 8, 0 }, { -2, 63, 0, 12, 7, 0 }, { 0, 72, 1, 12, 7, 0 }, { -31, 59, 0, 12, 7, 0 }, { 6, 43, 1, 11, 9, 0 }, { 16, 24, 1, 11, 8, 0 }, { 30, 42, 1, 11, 9, 0 }, { 6, 43, 1, 11, 9, 0 } },
	{ { 3, 28, 1, 11, 9, 0 }, { -1, 61, 1, 10, 8, 0 }, { -34, 66, 0, 12, 7, 0 }, { -42, 62, 0, 11, 9, 0 }, { 40, 29, 1, 11, 9, 0 }, { 9, 52, 1, 12, 7, 0 }, { -41, 10, 0, 10, 8, 0 }, { -34, 9, 0, 10, 8, 0 } },
	{ { -8, 56, 1, 10, 8, 0 }, { -6, 44, 1, 11, 9, 0 }, { -16, 78, 1, 12, 7, 0 }, { -56, 80, 0, 11, 9, 0 }, { -24, 28, 0, 10, 8, 0 }, { 12, 44, 1, 12, 7, 0 }, { 6, 64, 1, 11, 9, 0 }, { -6, 74, 1, 10, 8, 0 } },
	{ { 8, 64, 1, 10, 8, 0 }, { 14, 28, 1, 11, 9, 0 }, { -24, 32, 0, 12, 7, 0 }, { -40, 64, 0, 11, 9, 0 }, { -24, 20, 0, 10, 8, 0 }, { 12, 58, 1, 12, 7, 0 }, { 32, 48, 1, 10, 8, 0 }, { 46, 24, 1, 11, 9, 0 } },
	{ { -4, 56, 1, 10, 8, 0 }, { -4, 36, 1, 11, 9, 0 }, { 16, 40, 1, 12, 7, 0 }, { -48, 76, 0, 11, 9, 0 }, { 2, 72, 1, 10, 8, 0 }, { -40, 32, 0, 12, 7, 0 }, { -40, 40, 0, 11, 9, 0 }, { -46, 54, 0, 10, 8, 0 } },
	{ { 6, 42, 1, 11, 9, 0 }, { -37, 77, 0, 10, 8, 0 }, { 6, 78, 1, 12, 7, 0 }, { -12, 40, 0, 12, 7, 0 }, { -44, 62, 0, 11, 9, 0 }, { -11, 57, 0, 11, 8, 0 }, { 7, 93, 1, 11, 9, 0 }, { 26, 88, 0, 12, 7, 0 } },
	{ { -4, 17, 1, 10, 8, 0 }, { -8, 59, 1, 11, 9, 0 }, { -32, 41, 0, 12, 7, 0 }, { -28, 53, 0, 11, 9, 0 }, { -45, 53, 0, 10, 8, 0 }, { -1, 41, 1, 10, 8, 0 }, { -24, 54, 1, 11, 9, 0 }, { -35, 34, 0, 11, 8, 0 } },
	{ { 24, 72, 1, 10, 8, 0 }, { 68, 48, 1, 11, 9, 0 }, { 8, 82, 1, 12, 7, 0 }, { -48, 91, 0, 11, 9, 0 }, { -16, 32, 0, 11, 8, 0 }, { -32, 72, 0, 10, 8, 0 }, { -4, 94, 1, 11, 9, 0 }, { 28, 46, 1, 10, 8, 0 } },
	{ { 28, 48, 1, 10, 8, 0 }, { 46, 26, 1, 11, 9, 0 }, { -32, 40, 0, 12, 7, 0 }, { -24, 80, 0, 11, 9, 0 }, { 44, 84, 1, 10, 8, 0 }, { 38, 40, 1, 12, 7, 0 }, { -32, 40, 0, 11, 9, 0 }, { -16, 64, 0, 10, 8, 0 } },
	{ { 5, 54, 1, 10, 8, 0 }, { -60, 105, 0, 11, 9, 0 }, { -28, 115, 1, 12, 7, 0 }, { -39, 55, 0, 10, 8, 0 }, { 4, 73, 1, 11, 9, 0 }, { -62, 120, 0, 12, 7, 0 }, { -65, 92, 0, 10, 8, 0 }, { -40, 67, 0, 11, 9, 0 } },
	{ { 12, 56, 1, 10, 8, 0 }, { 6, 24, 1, 11, 9, 0 }, { 10, 70, 1, 12, 7, 0 }, { -42, 16, 0, 11, 9, 0 }, { -46, 62, 0, 10, 8, 0 }, { -48, 88, 0, 12, 7, 0 }, { 56, 52, 1, 11, 9, 0 }, { 14, 16, 1, 10, 8, 0 } },
	{ { 12, 60, 1, 10, 8, 0 }, { 10, 36, 1, 11, 9, 0 }, { -24, 40, 0, 12, 7, 0 }, { -42, 74, 0, 11, 9, 0 }, { -22, 24, 0, 10, 8, 0 }, { 8, 72, 1, 12, 7, 0 }, { 12, 72, 1, 11, 9, 0 }, { 24, 24, 1, 10, 8, 0 } },
	{ { 22, 56, 1, 10, 8, 0 }, { 14, 34, 1, 11, 9, 0 }, { -28, 40, 0, 12, 7, 0 }, { -30, 72, 0, 11, 9, 0 }, { -24, 30, 0, 10, 8, 0 }, { 6, 76, 1, 12, 7, 0 }, { 18, 72, 1, 11, 9, 0 }, { -22, 54, 0, 10, 8, 0 } },
	{ { 12, 64, 1, 10, 8, 0 }, { 20, 44, 1, 11, 9, 0 }, { -24, 40, 0, 12, 7, 0 }, { -32, 80, 0, 11, 9, 0 }, { -24, 68, 0, 10, 8, 0 }, { 18, 82, 1, 12, 7, 0 }, { 36, 40, 1, 11, 9, 0 }, { -32, 56, 0, 10, 8, 0 } },
	{ { 12, 42, 1, 10, 8, 0 }, { -42, 44, 0, 11, 9, 0 }, { 0, 68, 1, 12, 7, 0 }, { 36, 40, 1, 11, 9, 0 }, { -20, 34, 0, 10, 8, 0 }, { 22, 48, 1, 12, 7, 0 }, { 6, 50, 0, 11, 9, 0 }, { -10, 20, 0, 10, 8, 0 } },
	{ { 24, 52, 1, 10, 8, 0 }, { 24, 24, 1, 11, 9, 0 }, { 38, 64, 1, 12, 7, 0 }, { -64, 36, 0, 11, 9, 0 }, { -42, 80, 0, 10, 8, 0 }, { -12, 112, 0, 12, 7, 0 }, { -60, 24, 0, 11, 9, 0 }, { -16, 44, 0, 10, 8, 0 } },
	{ { 12, 42, 1, 10, 8, 0 }, { 12, 24, 1, 11, 9, 0 }, { -32, 60, 0, 12, 7, 0 }, { -32, 54, 0, 11, 9, 0 }, { -16, 20, 0, 10, 8, 0 }, { 16, 52, 1, 12, 7, 0 }, { 40, 32, 1, 11, 9, 0 }, { 18, 56, 1, 10, 8, 0 } },
	{ { 10, 80, 1, 10, 8, 0 }, { 10, 44, 1, 11, 9, 0 }, { -32, 36, 0, 12, 7, 0 }, { -28, 92, 0, 11, 9, 0 }, { 20, 26, 1, 10, 8, 0 }, { 6, 100, 1, 12, 7, 0 }, { 40, 28, 1, 11, 9, 0 }, { -34, 80, 0, 10, 8, 0 } },
	{ { 8, 120, 1, 10, 8, 0 }, { 52, 56, 1, 11, 9, 0 }, { -36, 32, 0, 10, 8, 0 }, { -72, 112, 0, 11, 9, 0 }, { -28, 72, 0, 10, 8, 0 }, { 36, 126, 1, 11, 9, 0 }, { 22, 54, 1, 10, 1, 0 }, { -46, 72, 0, 11, 9, 0 } },
	{ { 16, 72, 1, 10, 8, 0 }, { -28, 68, 0, 11, 8, 0 }, { -10, 92, 0, 12, 7, 0 }, { 24, 46, 1, 11, 9, 0 }, { -12, 32, 0, 10, 8, 0 }, { -40, 74, 0, 11, 9, 0 }, { 8, 88, 1, 11, 9, 0 }, { 8, 26, 1, 10, 8, 0 } },
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 2, 52, 1, 10, 8, 0 }, { 6, 69, 1, 11, 9, 0 }, { -14, 48, 0, 12, 7, 0 }, { -1, 33, 1, 11, 9, 0 }, { 14, 55, 1, 10, 8, 0 }, { 6, 53, 1, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 24, 32, 1, 10, 8, 0 }, { -28, 50, 0, 11, 9, 0 }, { -28, 40, 0, 12, 7, 0 }, { 10, 50, 1, 11, 9, 0 }, { -12, 26, 0, 10, 8, 0 }, { 4, 66, 1, 12, 7, 0 }, { 38, 42, 1, 11, 9, 0 }, { 28, 18, 1, 10, 8, 0 } },
	{ { 18, 38, 1, 10, 8, 0 }, { 26, 24, 1, 11, 9, 0 }, { -30, 40, 0, 12, 7, 0 }, { 18, 50, 1, 11, 9, 0 }, { -10, 18, 0, 10, 8, 0 }, { 22, 40, 1, 12, 7, 0 }, { -20, 34, 0, 11, 9, 0 }, { 22, 42, 1, 10, 8, 0 } },
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 4, 54, 1, 10, 8, 0 }, { 10, 38, 1, 11, 9, 0 }, { 0, 76, 1, 12, 7, 0 }, { -58, 64, 0, 11, 9, 0 }, { -12, 28, 0, 10, 8, 0 }, { -32, 42, 0, 12, 7, 0 }, { 30, 48, 1, 11, 9, 0 }, { -12, 64, 1, 10, 8, 0 } },
	{ { 12, 44, 1, 10, 8, 0 }, { -34, 48, 0, 11, 9, 0 }, { -16, 86, 0, 12, 7, 0 }, { 16, 72, 1, 11, 9, 0 }, { 20, 20, 1, 10, 8, 0 }, { -28, 40, 0, 12, 7, 0 }, { 30, 64, 1, 11, 9, 0 }, { -28, 44, 0, 10, 8, 0 } },
	{ { -32, 72, 0, 10, 8, 0 }, { -28, 53, 0, 11, 9, 0 }, { -20, 69, 0, 12, 7, 0 }, { 7, 73, 1, 10, 8, 0 }, { 2, 42, 1, 11, 9, 0 }, { 21, 70, 1, 11, 8, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { -35, 53, 0, 10, 8, 0 }, { -2, 59, 1, 11, 9, 0 }, { -27, 49, 0, 12, 7, 0 }, { -4, 40, 1, 11, 9, 0 }, { -35, 46, 0, 10, 8, 0 }, { -2, 51, 1, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 4, 58, 1, 10, 8, 0 }, { 6, 38, 1, 11, 9, 0 }, { -28, 40, 0, 12, 7, 0 }, { -44, 72, 0, 11, 9, 0 }, { 16, 24, 1, 10, 8, 0 }, { 16, 42, 1, 12, 7, 0 }, { 10, 68, 1, 11, 9, 0 }, { -28, 54, 0, 10, 8, 0 } },
	{ { -8, 42, 0, 10, 8, 0 }, { 6, 80, 1, 11, 9, 0 }, { 24, 25, 1, 12, 8, 0 }, { 7, 37, 1, 11, 9, 0 }, { 12, 61, 1, 10, 8, 0 }, { -27, 38, 0, 11, 8, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { -32, 72, 0, 10, 8, 0 }, { 4, 64, 1, 11, 8, 0 }, { 11, 76, 1, 12, 7, 0 }, { 5, 34, 1, 11, 9, 0 }, { -30, 47, 0, 10, 8, 0 }, { 32, 46, 1, 11, 8, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 14, 64, 1, 10, 8, 0 }, { -1, 56, 1, 11, 8, 0 }, { -21, 68, 0, 12, 7, 0 }, { 4, 30, 1, 11, 9, 0 }, { 25, 18, 1, 10, 8, 0 }, { -16, 39, 0, 12, 7, 0 }, { -54, 70, 0, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 0, 64, 1, 10, 8, 0 }, { 8, 40, 1, 11, 9, 0 }, { -40, 88, 0, 12, 7, 0 }, { -52, 78, 0, 11, 9, 0 }, { -32, 38, 0, 10, 8, 0 }, { 2, 74, 1, 12, 7, 0 }, { 22, 62, 1, 11, 9, 0 }, { 0, 76, 1, 10, 8, 0 } },
	{ { 22, 92, 1, 10, 8, 0 }, { 40, 46, 1, 11, 9, 0 }, { -48, 128, 0, 12, 7, 0 }, { -80, 90, 0, 11, 9, 0 }, { -36, 38, 0, 10, 8, 0 }, { 24, 104, 1, 12, 7, 0 }, { -62, 110, 0, 11, 9, 0 }, { 20, 68, 0, 10, 8, 0 } },
	{ { 6, 70, 1, 10, 8, 0 }, { 34, 46, 1, 11, 9, 0 }, { -28, 104, 0, 12, 7, 0 }, { -48, 70, 0, 11, 9, 0 }, { -40, 36, 0, 10, 8, 0 }, { 32, 106, 1, 12, 7, 0 }, { 4, 80, 1, 11, 9, 0 }, { -42, 32, 0, 10, 8, 0 } },
	{ { 0, 92, 1, 10, 8, 0 }, { 14, 52, 1, 11, 9, 0 }, { -36, 38, 0, 12, 7, 0 }, { -60, 112, 0, 11, 9, 0 }, { -28, 36, 0, 10, 8, 0 }, { 2, 102, 1, 12, 7, 0 }, { -30, 64, 0, 11, 9, 0 }, { 16, 32, 1, 10, 8, 0 } },
	{ { 19, 64, 1, 11, 9, 0 }, { 7, 92, 1, 10, 8, 0 }, { 0, 101, 1, 12, 7, 0 }, { -39, 93, 0, 11, 9, 0 }, { -31, 65, 0, 10, 8, 0 }, { -34, 109, 0, 12, 7, 0 }, { 9, 85, 1, 11, 9, 0 }, { 15, 71, 1, 10, 8, 0 } },
	{ { -62, 59, 0, 11, 9, 0 }, { -37, 84, 1, 12, 7, 0 }, { -22, 72, 1, 10, 8, 0 }, { -22, 38, 1, 10, 8, 0 }, { 30, 87, 1, 11, 9, 0 }, { -8, 40, 1, 12, 9, 0 }, { -11, 50, 1, 10, 8, 0 }, { -58, 34, 0, 12, 7, 0 } },
	{ { 10, 39, 1, 10, 8, 0 }, { -35, 80, 0, 11, 9, 0 }, { -9, 79, 1, 12, 7, 0 }, { -9, 76, 1, 11, 9, 0 }, { -7, 68, 1, 11, 9, 0 }, { -31, 36, 0, 10, 8, 0 }, { -25, 94, 0, 12, 7, 0 }, { 41, 18, 1, 10, 8, 0 } },
	{ { 10, 64, 1, 10, 8, 0 }, { 24, 40, 1, 11, 9, 0 }, { 16, 80, 1, 12, 7, 0 }, { -48, 78, 0, 11, 9, 0 }, { -8, 40, 0, 10, 8, 0 }, { -20, 40, 0, 12, 7, 0 }, { 52, 38, 1, 11, 9, 0 }, { -42, 64, 0, 10, 8, 0 } },
	{ { -44, 66, 0, 10, 8, 0 }, { -7, 66, 1, 11, 9, 0 }, { -12, 70, 1, 12, 7, 0 }, { -8, 38, 1, 11, 9, 0 }, { -15, 58, 1, 10, 8, 0 }, { -36, 29, 1, 10, 9, 0 }, { -24, 54, 1, 11, 9, 0 }, { -35, 34, 0, 11, 8, 0 } },
	{ { -4, 60, 1, 10, 8, 0 }, { -40, 60, 0, 11, 9, 0 }, { -16, 80, 1, 12, 7, 0 }, { 12, 44, 1, 11, 9, 0 }, { -60, 78, 0, 11, 9, 0 }, { 8, 40, 1, 12, 7, 0 }, { 4, 52, 1, 10, 8, 0 }, { 10, 64, 1, 10, 8, 0 } },
	{ { 16, 78, 1, 10, 8, 0 }, { 32, 48, 1, 11, 9, 0 }, { -16, 32, 0, 12, 7, 0 }, { -48, 96, 0, 11, 9, 0 }, { -40, 72, 0, 10, 8, 0 }, { 24, 84, 1, 12, 7, 0 }, { 56, 40, 1, 11, 9, 0 }, { -1, 59, 1, 10, 8, 0 } },
	{ { 10, 39, 1, 10, 8, 0 }, { -35, 80, 0, 11, 9, 0 }, { -9, 79, 1, 12, 7, 0 }, { -9, 76, 1, 11, 9, 0 }, { -7, 68, 1, 11, 9, 0 }, { -31, 36, 0, 10, 8, 0 }, { -25, 94, 0, 12, 7, 0 }, { 41, 18, 1, 10, 8, 0 } },
	{ { 15, 116, 1, 10, 8, 0 }, { 44, 58, 1, 11, 9, 0 }, { -56, 128, 0, 11, 9, 0 }, { -48, 80, 0, 10, 8, 0 }, { -32, 48, 0, 10, 8, 0 }, { 16, 80, 1, 11, 9, 0 }, { -8, 104, 1, 10, 8, 0 }, { 24, 36, 1, 10, 8, 0 } },
	{ { 0, 64, 1, 11, 9, 0 }, { -29, 125, 1, 12, 7, 0 }, { -42, 59, 0, 10, 8, 0 }, { -34, 118, 1, 11, 9, 0 }, { -17, 48, 1, 10, 8, 0 }, { -69, 143, 0, 12, 7, 0 }, { 25, 51, 1, 11, 9, 0 }, { -15, 106, 1, 10, 8, 0 } },
	{ { 5, 48, 1, 10, 8, 0 }, { 0, 76, 1, 11, 8, 0 }, { -19, 112, 0, 12, 7, 0 }, { 10, 84, 1, 11, 9, 0 }, { 35, 40, 1, 10, 8, 0 }, { -53, 121, 0, 12, 7, 0 }, { -45, 32, 0, 11, 9, 0 }, { -85, 79, 1, 10, 8, 0 } },
	{ { -16, 48, 1, 10, 8, 0 }, { -8, 36, 1, 11, 9, 0 }, { 12, 40, 1, 12, 7, 0 }, { -64, 64, 0, 11, 9, 0 }, { -18, 24, 0, 10, 8, 0 }, { -48, 80, 0, 12, 7, 0 }, { -16, 60, 1, 11, 9, 0 }, { 8, 30, 1, 10, 8, 0 } },
	{ { 0, 80, 1, 10, 8, 0 }, { 8, 38, 1, 11, 9, 0 }, { 12, 72, 1, 12, 7, 0 }, { -50, 80, 0, 11, 9, 0 }, { -24, 32, 0, 10, 8, 0 }, { -24, 38, 0, 12, 7, 0 }, { -18, 84, 1, 11, 9, 0 }, { -32, 32, 0, 10, 8, 0 } },
	{ { 3, 28, 1, 11, 9, 0 }, { -1, 61, 1, 10, 8, 0 }, { -34, 66, 0, 12, 7, 0 }, { -42, 62, 0, 11, 9, 0 }, { 40, 29, 1, 11, 9, 0 }, { 9, 52, 1, 12, 7, 0 }, { -41, 10, 0, 10, 8, 0 }, { -34, 9, 0, 10, 8, 0 } },
	{ { 16, 16, 0, 11, 9, 0 }, { -16, 16, 1, 11, 9, 0 }, { -16, 40, 1, 12, 7, 0 }, { -16, 32, 1, 10, 8, 0 }, { -8, 16, 1, 10, 8, 0 }, { 16, 40, 0, 12, 7, 0 }, { -8, 16, 0, 10, 8, 0 }, { -16, 32, 0, 10, 8, 0 } },
} ;

/****************************************************************/
SD void shl35_04_hit_ok( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL35_LOCAL_W *pl_lw ;

	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	lw->em_w = ew->em_w ;
	lw->hold_flag = 255 ;
	pl_w = ew->master_w ;
	pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;
	ew->dir = lw->em_w->disp_dir ;
	if( !pl_lw->hold_cnt ) pl_lw->hold_timer = 120 ;
	lw->hold_no = pl_lw->hold_cnt ;
	pl_lw->hold_cnt ++ ;
	lw->escape_cnt = 0 ;
}

/****************************************************************/
SD void shl35_04_to_end1( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 1 ;
	ew->routine_2 = 0 ;
}

/****************************************************************/
SD void shl35_04_to_end0( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
}

/****************************************************************/
/*			-1 when the master has gone on to a move that		*/
/*			lets it go, or lost its place in the count			*/
/****************************************************************/
SD int shl35_04_end_ck( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL35_LOCAL_W *pl_lw = ( PL35_LOCAL_W * )pl_w->tlocal ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SPECIAL ){
		if( pl_w->pl_sp_type < 2 ) return -1 ;
	} else {
		if( pl_lw->shot_no != lw->shot_no ) return -1 ;
		if( lw->char_grp && lw->char_no != 0 ){
			if( pl_w->char_no.b.grp == lw->char_grp && pl_w->char_no.b.no == lw->char_no ) return 0 ;
			lw->char_grp = 0 ;
			lw->char_no = 0 ;
		}
		if( pl_w->char_no.b.grp == Char_Stand_P && pl_w->char_no.b.no == 2 ) return -1 ;
		if( pl_w->char_no.b.grp == Char_Stand_K && pl_w->char_no.b.no == 2 ) return -1 ;
		if( pl_w->char_no.b.grp == Char_Crouch_P && pl_w->char_no.b.no == 2 ) return -1 ;
		if( pl_w->char_no.b.grp == Char_Crouch_K && pl_w->char_no.b.no == 2 ) return -1 ;
		if( pl_w->char_no.b.grp == Char_Jump_P && pl_w->char_no.b.no == 2 ) return -1 ;
		if( pl_w->char_no.b.grp == Char_Jump_K && pl_w->char_no.b.no == 2 ) return -1 ;
		return 0 ;
	}
}

/****************************************************************/
SD int shl35_04_ng_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 == PL_DAMAGE ) return -1 ;
	return 0 ;
}

/****************************************************************/
/*			It takes the place on the opponent that the			*/
/*			table gives for his kind, with the mirror and		*/
/*			priority											*/
/****************************************************************/
SD void shl35_04_place( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *em_w = lw->em_w ;
	Float x ;

	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->pos.y ;
	ew->disp_dir = em_w->disp_dir ;
	x = DOT_X( shl35_04_place_tbl[em_w->pl_type][ew->sub_type].x ) ;
	if( !em_w->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += 2.142857f * shl35_04_place_tbl[em_w->pl_type][ew->sub_type].y ;
	ew->disp_dir = em_w->disp_dir ;
	if( shl35_04_place_tbl[em_w->pl_type][ew->sub_type].flip ) ew->disp_dir ^= 1 ;
	ew->prio = shl35_04_place_tbl[em_w->pl_type][ew->sub_type].prio ;
}

/****************************************************************/
SD int shl35_04_nop( PLAYER_WORK *ew )
{
	return 0 ;
}

/****************************************************************/
SD void shl35_04_color( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *em_w = lw->em_w ;

	if( Sys->game_timer0 & 1 ) pl_color_set( em_w, 1 ) ;
	else pl_color_set( em_w, 8 ) ;
}

/****************************************************************/
SD void shl35_04_color1( PLAYER_WORK *ew )
{
	SHL35_04_LOCAL_W *lw = SHL35_04_LW( ew ) ;
	PLAYER_WORK *em_w = lw->em_w ;

	pl_color_set( em_w, 1 ) ;
}

/****************************************************************/
SD void shl35_04_step( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

/****************************************************************/
SD void shl35_04_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/****************************************************************/
SD void shl35_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/****************************************************************/
SD void shl35_04_copy( PLAYER_WORK *ew )
{
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
	ew->size_top = ew->size_bottom = 32 ;
	ew->size_front = ew->size_back = 36 ;
	ew->prio = 11 ;
}

/****************************************************************/
SD void shl35_04_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl35_04.c" End of File	*/
