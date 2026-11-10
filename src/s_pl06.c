/* Cyclops (character 06): command checks, normal attacks, appearance, win poses, special moves
   and hyper combos, throws and the partner moves. */
#define UNK_1C4_OLD
#include "player.h"
#include "pl_char.h"
#include "pl_color_old.h"
#include "o_sub1.h"
#include "pl06_def.h"

void pl06_main( PLAYER_WORK *pl_w );
void pl06_com_ck( PLAYER_WORK *pl_w );
void pl06_atck_set( PLAYER_WORK *pl_w );
void pl06_atck( PLAYER_WORK *pl_w );
void pl06_dash( PLAYER_WORK *pl_w );
void pl06_noki( PLAYER_WORK *pl_w );
void pl06_appear( PLAYER_WORK *pl_w );
void pl06_win( PLAYER_WORK *pl_w );
void pl06_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl06_catch_ck( PLAYER_WORK *pl_w );
void pl06_catch_set( PLAYER_WORK *pl_w );
void pl06_catch( PLAYER_WORK *pl_w );
void pl06_hold( PLAYER_WORK *pl_w );
int pl06_delay_ck( PLAYER_WORK *pl_w );
void pl06_t_kick( PLAYER_WORK *pl_w );
void pl06_upper( PLAYER_WORK *pl_w );
void pl06_sp_unison( PLAYER_WORK *pl_w );
void pl06_sp_gattai( PLAYER_WORK *pl_w );
void pl06_sp_assist( PLAYER_WORK *pl_w );
void pl06_sp_zero( PLAYER_WORK *pl_w );
void pl06_tlocal_init( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_mega[] = { 3, COM_HYPER | COM_ALL_SW, pl06_sp_mega, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Mega Optic Blast (hyper) */
static Uint16 com_super[] = { 3, COM_HYPER | COM_ALL_SW, pl06_sp_super, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Super Optic Blast (hyper) */
static Uint16 com_blast[] = { 3, 0, pl06_sp_blast, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Optic Blast */
static Uint16 com_sweep[] = { 3, COM_GROUND, pl06_sp_sweep, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_PUNCH } ;	/* Optic Sweep */
static Uint16 com_cyclone[] = { 3, COM_GROUND, pl06_sp_cyclone, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Cyclone Kick */
static Uint16 com_gene[] = { 3, COM_GROUND, pl06_sp_gene, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Gene Splice */
static Uint16 com_neckbreaker[] = { 50, COM_GROUND, pl06_sp_neckbreaker, 0, KEY_BACK, KEY_FORWARD, KEY_KICK } ;	/* Running Neckbreaker */
static Uint16 com_rapid[] = { 60, COM_GROUND, pl06_sp_rapid, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Rapid Punches */
static Uint16 com_catch[] = { 5, COM_GROUND, 0x000d, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* A command throw */

static void ( *pl06_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl06_main,
	pl06_com_ck,
	pl06_atck_set,
	pl06_atck,
	pl06_dash,
	pl06_noki,
	pl06_appear,
	pl06_win,
	pl06_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl06_catch_ck,	/* returns a value */
	pl06_catch_set,
	pl06_catch,
	pl06_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl06_delay_ck,
	pl06_t_kick,
	pl06_upper,
	pl06_sp_unison,
	pl06_sp_gattai,
	pl06_sp_assist,
	pl06_sp_zero,
} ;

static Uint8 mega_ck( PLAYER_WORK *pl_w );
static Uint8 super_ck( PLAYER_WORK *pl_w );
static Uint8 blast_ck( PLAYER_WORK *pl_w );
static Uint8 gene_ck( PLAYER_WORK *pl_w );
static int catch_com_ck( PLAYER_WORK *pl_w );
static Uint8 neckbreaker_ck( PLAYER_WORK *pl_w );
static Uint8 rapid_ck( PLAYER_WORK *pl_w );
static Uint8 sweep_ck( PLAYER_WORK *pl_w );
static Uint8 cyclone_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int mega_delay_ck( PLAYER_WORK *pl_w );
static int super_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl06_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl06_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl06_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( mega_ck( pl_w ) ) return ;
	if( super_ck( pl_w ) ) return ;
	if( gene_ck( pl_w ) ) return ;
	if( catch_com_ck( pl_w ) ) return ;
	if( sweep_ck( pl_w ) ) return ;
	if( blast_ck( pl_w ) ) return ;
	if( cyclone_ck( pl_w ) ) return ;
	if( neckbreaker_ck( pl_w ) ) return ;
	if( rapid_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			The hyper combos: in the air, once a jump and		*/
/*			not in a super jump									*/
/*==============================================================*/
static Uint8 mega_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_mega, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl06_sp_mega ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 super_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_super, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl06_sp_super ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blast_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blast, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl06_sp_blast ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 gene_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gene, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl06_sp_gene ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			A throw done by a command, when the opponent is		*/
/*			in reach											*/
/*==============================================================*/
static int catch_com_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !com_nml_ck( pl_w, com_catch, &pl_w->com_w7 ) ) return 0 ;
	if( !( em = pl_catch_ck( pl_w ) ) ) return 0 ;
	pl_w->catch_no = 0x43 ;
	pl_catch_set( pl_w, em ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 neckbreaker_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_neckbreaker, &pl_w->com_w3 ) ) return 0 ;
	pl_w->pl_sp_type = pl06_sp_neckbreaker ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rapid_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_rapid, &pl_w->com_w5 ) ) return 0 ;
	pl_w->pl_sp_type = pl06_sp_rapid ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 sweep_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sweep, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl06_sp_sweep ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cyclone_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cyclone, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->pl_sp_type = pl06_sp_cyclone ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl06_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 11 ) ) return 0 ;
	pl_w->pl_sp_type = pl06_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl06_delay_ck( PLAYER_WORK *pl_w )
{
	if( mega_delay_ck( pl_w ) || super_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int mega_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_mega, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 2 ;
	return 1 ;
}

/*==============================================================*/
static int super_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_super, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	pl_w->delay_sp_type = 1 ;
	return 1 ;
}

/****************************************************************/
void pl06_main( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_stop == 0 || pl_w->routine_1 != PL_NORM ) PL06_LW( pl_w )->beam_flag = 0 ;
}

static void nml_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl06_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick( pl_w ) ;
		else stand_kick( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch( pl_w ) ;
		else stand_punch( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	Uint8 se ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LK ;
		se = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MK ;
		se = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		se = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_se_req( pl_w, se ) ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	Uint8 se ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LK ;
		se = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MK ;
		se = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		se = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_se_req( pl_w, se ) ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl06_at_jdown_mp ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl06_at_jdown_hp ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl06_at_jdown_lk ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl06_at_jdown_mk ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl06_at_jdown_hk ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

static void nml_move_dash( PLAYER_WORK *pl_w );
static void nml_move( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl06_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick_move( pl_w ) ;
		else stand_kick_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch_move( pl_w ) ;
		else stand_punch_move( pl_w ) ;
	}
}

/*==============================================================*/
/*			The heavy punch fires an optic blast				*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->nml_no == 2 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl06_00_set( pl_w, 0 ) ;
		pls06_01_set( pl_w, 0 ) ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->nml_no == 2 && pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl06_00_set( pl_w, 1 ) ;
		pls06_01_set( pl_w, 1 ) ;
	}
}

/*==============================================================*/
/*			The heavy kick goes on into a second kick with		*/
/*			the button still held								*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type && ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x20 ) ){
			pl_atck_gauge_add( pl_w ) ;
			pl_w->pl_stop_now = 0 ;
			pl_w->atck_no = pl06_at_stand_hk_2 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Stand_K, 4 ) ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher, standing or crouching					*/
/****************************************************************/
void pl06_upper( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_type != PL_CYCLOPS ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		if( pl_w->atck_pk == 0 ){
			pl_w->atck_no = pl06_at_upper_p ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Extra, 17 ) ;
		} else {
			pl_w->atck_no = pl06_at_upper_k ;
			pl_w->pl_stance = STANCE_CROUCH ;
			pl_char_set( pl_w, Char_Extra, 18 ) ;
		}
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_gauge_add( pl_w, 5 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl06_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl06_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_fade_in( PLAYER_WORK *pl_w );
static void appear_fade_out( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );
static void color_fade_in( PLAYER_WORK *pl_w );
static void color_fade_out( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void win_ko_move_2( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl06_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_wait,
		appear_fade_in,
		appear_fade_out,
		appear_end,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w -> routine_2 ++ ;
	pl_w -> disp_flag = 1 ;
	pls06_01_set( pl_w, 2 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;

		pl_w->cg_prm = 0 ;
		pl_w->routine_2 ++ ;
		lw->col_step = lw->flash_timer = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fade_in( PLAYER_WORK *pl_w )
{
	color_fade_in( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;

		pl_w->cg_prm = 0 ;
		pl_w->routine_2 ++ ;
		lw->col_step = lw->flash_timer = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fade_out( PLAYER_WORK *pl_w )
{
	color_fade_out( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		PL06_LW( pl_w )->col_step = PL06_LW( pl_w )->flash_timer = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*==============================================================*/
/*			Eight steps through the entrance's colours, one		*/
/*			a frame												*/
/*==============================================================*/
static void color_fade_in( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;
	int col ;

	if( lw->col_step < 8 ){
		col = pl_w->pl_color * 8 + lw->col_step + 32 ;
		lw->col_step ++ ;
		ef_color_set( pl_w, 0, col, 1 ) ;
	}
}

/*==============================================================*/
static void color_fade_out( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;
	int col ;

	if( lw->col_step < 8 ){
		col = pl_w->pl_color * 8 - lw->col_step + 39 ;
		lw->col_step ++ ;
		ef_color_set( pl_w, 0, col, 1 ) ;
	}
}

/****************************************************************/
void pl06_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time,
		lose_time_ko,
		draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_voice_req( pl_w, 43 ) ;
		pl_stand_ret( pl_w ) ;
	}else{
		win_jmp[ pl_w->type ]( pl_w ) ;
	}
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		win_ko_move,
		win_ko_move_e,
		win_ko_move_2,
	} ;

	win_ko_jmp[ pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO: one of two poses							*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	if( !( Sys->game_timer0 & 1 ) || Sys->ending_flag ){
		pl_w->routine_2 = 3 ;
		pl_w->char_no.b.no = CH06_WIN_KO1 ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->char_no.b.no = CH06_WIN_KO0 ;
	}
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;

		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		lw->beam_st = 64 ;
		lw->beam_len = 4 ;
		lw->beam_dir = 0 ;
		shl06_01_set( pl_w, 1, 0, 0 ) ;
		lw->beam_flag = 0xff ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	PL06_LW( pl_w )->beam_flag = 0xff ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move_2( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls06_01_set( pl_w, 4 ) ;
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH06_LOSE ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH06_WIN_TIME ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w -> routine_2 ){
		pl_w -> routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, CH06_TM_DRAW ) ;
	}else{
		pl_char_move( pl_w ) ;
	}
}

static void sp_blast( PLAYER_WORK *pl_w );
static void sp_super( PLAYER_WORK *pl_w );
static void sp_mega( PLAYER_WORK *pl_w );
static void sp_gene( PLAYER_WORK *pl_w );
static void sp_neckbreaker( PLAYER_WORK *pl_w );
static void sp_cyclone( PLAYER_WORK *pl_w );
static void sp_sweep( PLAYER_WORK *pl_w );
static void sp_rapid( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl06_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_blast, sp_super, sp_mega, sp_gene, sp_neckbreaker, sp_blast, sp_blast, sp_cyclone,
		sp_sweep, sp_rapid, sp_blast, sp_chouhatsu, sp_snapback, sp_blast,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void blast_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void blast_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void blast_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void blast_stop( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void blast_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void beam_flash( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );

/*==============================================================*/
/*			Optic Blast											*/
/*==============================================================*/
static void sp_blast( PLAYER_WORK *pl_w )
{
	static void ( *blast_jmp[])( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw ) = {
		blast_init, blast_start, blast_fire, blast_stop, blast_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	blast_jmp[pl_w->routine_2]( pl_w, ( PL06_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void blast_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	Uint8 no ;

	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->timer0 = 10 ;
		lw->beam_dir = 12 ;
		pl_w->atck_no = pl06_at_jblast ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	}else{
		pl_brake_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		no = pl_w->atck_level + 0x31 ;
		if( pl_w->team_type == 3 ) no = 0x41 ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level * 2 ) ;
		if( !pl_w->atck_level ){
			lw->beam_dir = 12 ;
			pl_w->timer0 = 12 ;
		}else{
			lw->beam_dir = 14 ;
			pl_w->timer0 = 10 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void blast_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		lw->beam_st = 32 ;
		lw->beam_len = 13 ;
		shl06_01_set( pl_w, 0, 0, 0 ) ;
		lw->beam_flag = 0xff ;
	}
}

/*--------------------------------------------------------------*/
static void blast_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	lw->beam_flag = 0xff ;
	beam_flash( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		lw->beam_st = 0 ;
		pl_w->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void blast_stop( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	lw->beam_flag = 0xff ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		lw->beam_flag = 0 ;
		pl_w->char_no.b.no = 7 ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_w->char_no.b.no = !pl_w->atck_level ? 4 : 6 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void blast_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 43 ) ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_jump_ret( pl_w ) ;
		}else{
			pl_stand_ret( pl_w ) ;
		}
	}
}

static void super_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void super_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void super_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void super_stop( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void super_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void super_char_set( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void super_aim( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );

/*==============================================================*/
/*			Super Optic Blast: the beam the stick aims			*/
/*==============================================================*/
static void sp_super( PLAYER_WORK *pl_w )
{
	static void ( *super_jmp[])( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw ) = {
		super_init, super_start, super_fire, super_stop, super_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	super_jmp[pl_w->routine_2]( pl_w, ( PL06_LOCAL_W * )pl_w->tlocal ) ;
}

/* the beam's aim for each stick direction, or -1 */
static Sint8 aim_tbl[16] = {
	-1,  4, 12, -1,  8,  6, 10, -1,  0,  2, 14, -1, -1, -1, -1, -1,
} ;

/*==============================================================*/
static void super_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	}else{
		pl_brake_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
	}
	pl_w->atck_no = pl06_at_super ;
	HIT_RESET( pl_w ) ;
	lw->col_step = lw->flash_timer = 0 ;
}

/*--------------------------------------------------------------*/
static void super_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	VEC3 pos[2] ;
	int dir ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			if( pl_w->pl_stance == STANCE_AIR ){
				pos[0].x = -DOT_X( 4.0f ) ;
				pos[0].y = DOT_Y( 104.0f ) ;
			}else{
				pos[0].x = -DOT_X( 28.0f ) ;
				pos[0].y = 180.0f ;
			}
			pos[0].z = 0.0f ;
			pl_hyper_start( pl_w, &pos[0], 1 ) ;
		}
	}else{
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 50 ;
		lw->beam_flag = 0xff ;
		lw->renda_cnt = 48 ;
		lw->beam_st = 64 ;
		lw->beam_len = 13 ;
		pos[0] = pl_w->pos ;
		pos[0].y += 102.0f ;
		pos[1] = pl_w->target_w->pos ;
		if( pl_w->target_w->pl_stance == STANCE_STAND || pl_w->target_w->pl_stance == STANCE_CROUCH ){
			pos[1].y = pl_w->target_w->ground_y + 102.0f ;
		}else{
			pos[1].y += 56.0f ;
		}
		dir = ( caldir_pos_256( &pos[0], &pos[1] ) / 16 ) & 15 ;
		if( pl_w->disp_dir ) dir = ( 16 - dir ) & 15 ;
		lw->beam_dir = dir ;
		super_char_set( pl_w, lw ) ;
		shl06_01_set( pl_w, 0, 0, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void super_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw->beam_flag = 0xff ;
	beam_flash( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ) super_aim( pl_w, lw ) ;
	if( lw->renda_cnt && sw_renda_ck( pl_w ) ){
		lw->renda_cnt -- ;
		pl_w->timer0 ++ ;
	}
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 8 ;
		lw->beam_st = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void super_stop( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw->beam_flag = 0xff ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		lw->beam_flag = 0 ;
		pl_w->char_no.b.no = ( pl_w->pl_stance == STANCE_AIR ) ? 44 : 26 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*--------------------------------------------------------------*/
static void super_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 43 ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_jump_ret( pl_w ) ;
		}else{
			pl_stand_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			The beam's animation for its aim					*/
/*==============================================================*/
static void super_char_set( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->char_no.b.no = lw->beam_dir + 28 ;
	}else{
		pl_w->char_no.b.no = lw->beam_dir + 10 ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The beam's colours flicker							*/
/*==============================================================*/
static void beam_flash( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( -- lw->flash_timer < 0 ){
		int col ;

		lw->flash_timer = 1 ;
		col = pl_w->pl_color * 2 + 80 ;
		if( lw->col_step ^= 1 ) col ++ ;
		ef_color_set( pl_w, 0, col, 1 ) ;
	}
}

/*==============================================================*/
/*			The beam turns one step towards the stick			*/
/*==============================================================*/
static void super_aim( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	Sint8 lev = ( pl_w->pl_sw_new & KEY_LEVER ) >> 10 ;
	Sint8 aim = aim_tbl[lev] ;

	if( aim >= 0 && aim != lw->beam_dir ){
		if( ( ( aim - lw->beam_dir ) & 15 ) < 8 ){
			lw->beam_dir ++ ;
		}else{
			lw->beam_dir -- ;
		}
		lw->beam_dir &= 15 ;
	}
	super_char_set( pl_w, lw ) ;
}

static void mega_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void mega_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void mega_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void mega_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );

/*==============================================================*/
/*			Mega Optic Blast: the wide beam						*/
/*==============================================================*/
static void sp_mega( PLAYER_WORK *pl_w )
{
	static void ( *mega_jmp[])( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw ) = {
		mega_init, mega_start, mega_fire, mega_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	mega_jmp[pl_w->routine_2]( pl_w, ( PL06_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void mega_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	Uint8 no ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 64.0f ;
		pl_w->acc.x = 0.0f ;
		no = 9 ;
	}else{
		pl_brake_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		no = 0 ;
		if( pl_w->team_type == 5 || pl_w->team_type == 4 ) no = 8 ;
	}
	pl_w->atck_no = pl06_at_mega ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, no ) ;
}

/*--------------------------------------------------------------*/
static void mega_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	VEC3 pos ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			if( pl_w->pl_stance == STANCE_AIR ){
				pos.x = -DOT_X( 4.0f ) ;
				pos.y = DOT_Y( 104.0f ) ;
			}else{
				pos.x = -DOT_X( 28.0f ) ;
				pos.y = 180.0f ;
			}
			pos.z = 0.0f ;
			pl_hyper_start( pl_w, &pos, 1 ) ;
		}
	}else{
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		lw->beam_st = 0 ;
		shl06_02_set( pl_w, 0 ) ;
		lw->beam_flag = 0 ;
		pl_w->timer0 = 60 ;
		lw->renda_cnt = 48 ;
		pl_se_req( pl_w, 72 ) ;
	}
}

/*--------------------------------------------------------------*/
static void mega_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw->beam_flag = 0xff ;
	Rnd( ) ;
	pl_char_move( pl_w ) ;
	beam_flash( pl_w, lw ) ;
	if( lw->renda_cnt && sw_renda_ck( pl_w ) ){
		lw->renda_cnt -- ;
		pl_w->timer0 ++ ;
	}
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->char_no.b.no = 1 ;
		if( pl_w->pl_stance == STANCE_AIR ) pl_w->char_no.b.no = 10 ;
		pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
		lw->beam_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*--------------------------------------------------------------*/
static void mega_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_jump_ret( pl_w ) ;
		}else{
			pl_stand_ret( pl_w ) ;
		}
	}
}

static void kick_init( PLAYER_WORK *pl_w );
static void kick_rise( PLAYER_WORK *pl_w );
static void kick_fall( PLAYER_WORK *pl_w );
static void kick_land( PLAYER_WORK *pl_w );
static void kick_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Gene Splice: each punch pressed while it rises		*/
/*			adds a hit											*/
/*==============================================================*/
static void sp_gene( PLAYER_WORK *pl_w )
{
	static void ( *gene_jmp[])( PLAYER_WORK *pl_w ) = {
		kick_init, kick_rise, kick_fall, kick_land, kick_end,
	} ;

	gene_jmp[pl_w->routine_2]( pl_w ) ;
}

/* the kick's speeds for each strength: forward and up */
static Float kick_spd[2][2] = {
	{ DOT_X( 4.0f ), 15.0f },
	{ 20.0f, DOT_Y( 9.0f ) },
} ;

/*==============================================================*/
static void kick_init( PLAYER_WORK *pl_w )
{
	Float spd[2] ;
	Uint8 no ;

	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->timer0 = pl_w->atck_level * 2 + 1 ;
	pl_w->pl_stop_now = 0 ;
	spd[0] = kick_spd[pl_w->atck_level][0] ;
	spd[1] = -DOT_X( 0.5f ) ;
	pl_w->spd.y = kick_spd[pl_w->atck_level][1] ;
	pl_w->acc.y = -1.2053571f ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x -= spd[0] ;
		if( pl_w->acc.x >= 0.0f ){
			pl_w->acc.x -= spd[1] ;
		}else{
			pl_w->acc.x = spd[1] ;
		}
	}else{
		pl_w->spd.x += spd[0] ;
		if( pl_w->acc.x < 0.0f ){
			pl_w->acc.x += spd[1] ;
		}else{
			pl_w->acc.x = spd[1] ;
		}
	}
	no = 0x30 ;
	if( pl_w->team_type == 3 ) no = 0x48 ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void kick_rise( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type != 15 ){
		pl_air_set( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_brake_stop( pl_w ) ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		no = 0x30 ;
		if( pl_w->team_type == 3 ) no = 0x48 ;
		pl_w->atck_no = no ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		if( pl_w->timer0 && ( pl_w->pl_stop_now & KEY_PUNCH ) ){
			pl_w->pl_stop_now = 0 ;
			pl_w->spd.y += DOT_Y( 8.0f ) ;
			pl_w->timer0 -- ;
			no = 0x30 ;
			if( pl_w->team_type == 3 ) no = 0x48 ;
			pl_w->atck_no = no ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
			pl_w->char_no.b.no = ( pl_w->timer0 & 1 ) ? 6 : 5 ;
			pl_char_set( pl_w, Char_Extra, pl_w->char_no.b.no ) ;
			return ;
		}
	}
	if( pl_w->cg_type == 3 ){
		pl_w->cg_type = 0 ;
		shl06_00_set( pl_w, 2 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void kick_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->spd.y > 0.0f ) return ;
	pl_w->routine_2 ++ ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
}

/*--------------------------------------------------------------*/
static void kick_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void kick_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void grab_init( PLAYER_WORK *pl_w );
static void grab_run( PLAYER_WORK *pl_w );
static void grab_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A run that throws the opponent it reaches			*/
/*==============================================================*/
static void sp_neckbreaker( PLAYER_WORK *pl_w )
{
	static void ( *neckbreaker_jmp[])( PLAYER_WORK *pl_w ) = {
		grab_init, grab_run, grab_move,
	} ;

	neckbreaker_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void grab_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 28 ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = DOT_X( 7.0f ) ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 10 ) ;
	grab_run( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void grab_run( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 <= 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void grab_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
	}else if( !pl_w->cg_type ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0xc0 ;
			pl_catch_set( pl_w, em ) ;
		}
	}
}

static void hop_init( PLAYER_WORK *pl_w );
static void hop_move( PLAYER_WORK *pl_w );
static void hop_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hop forwards that hits again as it lands			*/
/*==============================================================*/
static void sp_cyclone( PLAYER_WORK *pl_w )
{
	static void ( *cyclone_jmp[])( PLAYER_WORK *pl_w ) = {
		hop_init, hop_move, hop_end,
	} ;

	cyclone_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hop_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->routine_2 ++ ;
	pl_air_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->spd.x = -7.5f ;
	pl_w->acc.x = 0.0f ;
	no = 63 ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	if( pl_w->team_type == 3 ) no = 70 ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 45 ) ;
}

/*--------------------------------------------------------------*/
static void hop_move( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	no = 64 ;
	if( pl_w->team_type == 3 ) no = 71 ;
	pl_w->atck_no = no ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Special, 46 ) ;
}

/*--------------------------------------------------------------*/
static void hop_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void sweep_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void sweep_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void sweep_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void sweep_stop( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );
static void sweep_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw );

/*==============================================================*/
/*			Optic Sweep: the beam along the ground				*/
/*==============================================================*/
static void sp_sweep( PLAYER_WORK *pl_w )
{
	static void ( *sweep_jmp[])( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw ) = {
		sweep_init, sweep_start, sweep_fire, sweep_stop, sweep_end,
	} ;

	sweep_jmp[pl_w->routine_2]( pl_w, ( PL06_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void sweep_init( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->atck_no = pl06_at_sweep ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*--------------------------------------------------------------*/
static void sweep_start( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		lw->beam_dir = 11 ;
		lw->beam_st = 32 ;
		lw->beam_len = 3 ;
		shl06_01_set( pl_w, 0, 0, 0 ) ;
		lw->beam_flag = 0xff ;
		pl_w->timer0 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void sweep_fire( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	lw->beam_flag = 0xff ;
	beam_flash( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		lw->beam_st = 0 ;
		pl_w->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void sweep_stop( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	lw->beam_flag = 0xff ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		lw->beam_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sweep_end( PLAYER_WORK *pl_w, PL06_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 43 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_stand_ret( pl_w ) ;
	}
}

static void rush_init( PLAYER_WORK *pl_w );
static void rush_hit1( PLAYER_WORK *pl_w );
static void rush_hit2( PLAYER_WORK *pl_w );
static void rush_hit3( PLAYER_WORK *pl_w );
static void rush_last( PLAYER_WORK *pl_w );
static void rush_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A rush of hits, each one carried on by a button		*/
/*==============================================================*/
static void sp_rapid( PLAYER_WORK *pl_w )
{
	static void ( *rapid_jmp[])( PLAYER_WORK *pl_w ) = {
		rush_init, rush_hit1, rush_hit2, rush_hit3, rush_last, rush_end,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_brake_stop( pl_w ) ;
	rapid_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Each hit of the rush starts it forward again		*/
/*==============================================================*/
static void rush_spd_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = DOT_X( 8.0f ) ;
	pl_w->acc.x = -DOT_X( 0.25f ) ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->pl_stop_new = 0 ;
	pl_w->pl_stop_now = 0 ;
	pl_w->timer0 = 0 ;
}

/*==============================================================*/
static void rush_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	rush_spd_set( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->atck_no = pl06_at_rapid ;
	HIT_RESET( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 9 ) ;
}

/*--------------------------------------------------------------*/
static void rush_hit1( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			pl_w->atck_no = pl06_at_rapid ;
			HIT_RESET( pl_w ) ;
		}
		if( pl_w->cg_type ){
			if( sw_renda_ck( pl_w ) || ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x360 ) ) pl_w->timer0 = 1 ;
		}
	}else if( !pl_w->timer0 ){
		pl_w->routine_2 = 5 ;
		pl_char_set( pl_w, Char_Extra, 16 ) ;
	}else{
		pl_w->routine_2 ++ ;
		rush_spd_set( pl_w ) ;
		pl_w->atck_no = pl06_at_rapid_2 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_hit2( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			pl_w->atck_no = pl06_at_rapid ;
			HIT_RESET( pl_w ) ;
		}
		if( pl_w->cg_type ){
			if( sw_renda_ck( pl_w ) || ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x360 ) ) pl_w->timer0 = 1 ;
		}
	}else if( !pl_w->timer0 ){
		pl_w->routine_2 = 5 ;
		pl_char_set( pl_w, Char_Extra, 16 ) ;
	}else{
		pl_w->routine_2 ++ ;
		rush_spd_set( pl_w ) ;
		pl_w->atck_no = pl06_at_rapid_3 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_hit3( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			if( sw_renda_ck( pl_w ) || ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x360 ) ) pl_w->timer0 = 1 ;
		}
	}else if( !pl_w->timer0 ){
		pl_w->routine_2 = 5 ;
		pl_char_set( pl_w, Char_Extra, 16 ) ;
	}else{
		pl_w->routine_2 ++ ;
		rush_spd_set( pl_w ) ;
		pl_w->atck_no = pl06_at_rapid_4 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_last( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
	}else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl06_00_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl06_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 47 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 37.0f ) ;
		ofs.y = DOT_Y( 46.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		ef_color_set( pl_w, 1, 25, 1 ) ;
		pl_char_set( pl_w, Char_Win, 4 ) ;
	}else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl06_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl06_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 7 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 8 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls06_01_set( pl_w, 7 ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl06_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, nml_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 5 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > pl_w->ground_y + DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0x84 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void neckbreaker_set( PLAYER_WORK *pl_w );
static void crouch_k_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void beam_throw_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl06_catch_ck chose			*/
/****************************************************************/
void pl06_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		neckbreaker_set, crouch_k_set, nml_p_set, beam_throw_set, jump_p_set, nml_k_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void neckbreaker_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void crouch_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void beam_throw_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 16.0f ) ;
	ofs.y = DOT_Y( 48.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 2.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 8 ) ;
}

static void neckbreaker_catch( PLAYER_WORK *pl_w );
static void crouch_k_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );
static void beam_throw_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl06_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		neckbreaker_catch, crouch_k_catch, nml_p_catch, beam_throw_catch, jump_p_catch, nml_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void neckbreaker_catch_init( PLAYER_WORK *pl_w );
static void neckbreaker_catch_jump( PLAYER_WORK *pl_w );
static void neckbreaker_catch_throw( PLAYER_WORK *pl_w );
static void neckbreaker_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The run's throw: a leap that carries the			*/
/*			opponent											*/
/*==============================================================*/
static void neckbreaker_catch( PLAYER_WORK *pl_w )
{
	static void ( *neckbreaker_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		neckbreaker_catch_init, neckbreaker_catch_jump, neckbreaker_catch_throw, neckbreaker_catch_end,
	} ;

	neckbreaker_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/* where the opponent is moved by each step of the leap */
static Sint16 neckbreaker_catch_ofs[] = { 0, 0, 32, 0, 48, -48, -64, -32, 0, 0, 0, 0 } ;

/*==============================================================*/
static void neckbreaker_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 0 ;
	pl_w->spd.x = DOT_X( 10.0f ) ;
	pl_w->acc.x = -DOT_X( 0.25f ) ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	neckbreaker_catch_jump( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void neckbreaker_catch_jump( PLAYER_WORK *pl_w )
{
	Float x, y ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 24 ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		return ;
	}
	if( pl_w->timer0 > 12 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->cg_type ){
		pl_w->timer0 = pl_w->cg_type ;
		pl_w->cg_type = 0 ;
		x = DOT_X( neckbreaker_catch_ofs[pl_w->timer0 / 2] ) ;
		if( !pl_w->pl_dir ) x = -x ;
		pl_w->pos.x += x ;
		y = neckbreaker_catch_ofs[pl_w->timer0 / 2 + 1] * DOT_Y( 1.0f ) ;
		pl_w->pos.y -= y ;
	}
}

/*--------------------------------------------------------------*/
static void neckbreaker_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl06_at_neckbreaker ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_w->down_timer = 0 ;
		em->disp_dir = em->pl_dir = pl_w->disp_dir ^ 1 ;
		catch_se_req( em ) ;
	}else if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Get_Up, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void neckbreaker_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void crouch_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl06_at_crouch_throw_k ;
	}
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		if( pl_w->dir & 2 ){
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
		}
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl06_at_throw_p ;
		em->catch_type = 11 ;
		catch_se_req( em ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The beam throw: the stick aims the opponent			*/
/*==============================================================*/
static void beam_throw_catch( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	Uint16 lever ;
	Float x, y ;

	if( !pl_w->routine_2 ){
		if( pl_char_move( pl_w ) >= 0 ){
			if( pl_w->cg_type != 2 ) return ;
			pl_w->cg_type = 1 ;
			lever = pl_w->pl_sw_new ;
			if( pl_w->cpu_flag ){
				lw->beam_dir = 0 ;
				pl_w->dir = 0 ;
				pl_w->timer0 = 5 ;
			}else if( lever & KEY_FORWARD ){
				lw->beam_dir = 14 ;
				pl_w->dir = 28 ;
				pl_w->timer0 = 4 ;
			}else if( !( lever & KEY_BACK ) ){
				lw->beam_dir = 0 ;
				pl_w->dir = 0 ;
				pl_w->timer0 = 5 ;
			}else{
				lw->beam_dir = 14 ;
				pl_w->dir = 28 ;
				pl_w->timer0 = 4 ;
				pl_w->disp_dir = pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
			}
			goto end ;
		}
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, pl_w->timer0 ) ;
		lw->beam_st = 64 ;
		lw->beam_len = 13 ;
		lw->beam_flag = 0xff ;
		pl_w->timer0 = 30 ;
		pl_w->timer1 = 1 ;
		shl06_01_set( pl_w, 1, 2, 0 ) ;
		em = pl_w->catch_w ;
		em->pos = pl_w->pos ;
		x = 0.0f ;
		if( pl_w->dir ){
			x = -160.0f ;
			if( pl_w->pl_dir ) x = 160.0f ;
		}
		em->pos.x += x ;
		em->pos.y += DOT_Y( 96.0f ) ;
		return ;
	}
	lw->beam_flag = 0xff ;
	em = pl_w->catch_w ;
	if( em->wall_flag ) lw->beam_len = 0 ;
	if( -- pl_w->timer0 >= 0 ){
		beam_flash( pl_w, lw ) ;
		pl_char_move( pl_w ) ;
		if( !lw->beam_len ) return ;
		if( -- pl_w->timer1 ) return ;
		pl_w->timer1 = 1 ;
		x = 0.0f ;
		y = DOT_Y( 9.0f ) ;
		if( pl_w->dir ){
			x = -10.0f ;
			y = DOT_Y( 6.0f ) ;
			if( pl_w->pl_dir ) x = 10.0f ;
		}
		em->pos.x += x ;
		em->pos.y += y ;
		return ;
	}
	lw->beam_len = 0 ;
	lw->beam_flag = 0 ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->atck_no = pl06_at_beam_throw | ATCK_DIR ;
	if( pl_w->pl_dir ) pl_w->dir = ( 32 - pl_w->dir ) & 31 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_voice_req( pl_w, 43 ) ;
	pl_stand_ret( pl_w ) ;
	return ;
end:
	;
}

static void jump_p_catch_init( PLAYER_WORK *pl_w );
static void jump_p_catch_rise( PLAYER_WORK *pl_w );
static void jump_p_catch_fire( PLAYER_WORK *pl_w );
static void jump_p_catch_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The air throw: up with the opponent, then a beam	*/
/*			down at them										*/
/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_catch_init, jump_p_catch_rise, jump_p_catch_fire, jump_p_catch_land,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_rise( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw ;
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y + DOT_Y( 128.0f ) ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 10 ;
		pl_w->acc.y = DOT_Y( 0.25f ) ;
		pl_char_move( pl_w ) ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl06_at_air_throw ;
		lw = ( PL06_LOCAL_W * )pl_w->tlocal ;
		lw->beam_dir = 8 ;
		lw->beam_st = 64 ;
		lw->beam_len = 5 ;
		lw->beam_flag = 0xff ;
		shl06_01_set( pl_w, 1, 0, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_fire( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;

	lw->beam_flag = 0xff ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y + -DOT_Y( 1.0f ) ;
	}else{
		beam_flash( pl_w, lw ) ;
		if( -- pl_w->timer0 >= 0 ) return ;
	}
	pl_w->routine_2 ++ ;
	lw->beam_flag = 0 ;
	pl_w->spd.x = DOT_X( 2.0f ) ;
	if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 10.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Catch, 7 ) ;
}

/*--------------------------------------------------------------*/
static void jump_p_catch_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_jump_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_voice_req( pl_w, 43 ) ;
		pl_land_set( pl_w ) ;
	}
}

static void nml_k_catch_init( PLAYER_WORK *pl_w );
static void nml_k_catch_fire( PLAYER_WORK *pl_w );
static void nml_k_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The kick throw: a beam at point blank				*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch_init, nml_k_catch_fire, nml_k_catch_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;

		pl_w->routine_2 ++ ;
		lw->beam_st = 64 ;
		lw->beam_len = 0 ;
		lw->beam_flag = 0xff ;
		lw->beam_dir = 12 ;
		shl06_01_set( pl_w, 1, 2, 0 ) ;
		pl_w->timer0 = 32 ;
		lw->col_step = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_fire( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;

	lw->beam_flag = 0xff ;
	beam_flash( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_end( PLAYER_WORK *pl_w )
{
	PL06_LOCAL_W *lw = ( PL06_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	lw->beam_flag = 0xff ;
	if( !pl_w->routine_3 ) beam_flash( pl_w, lw ) ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type != 2 ){
			pl_w->cg_type = 0 ;
			lw->beam_flag = 0 ;
			pl_w->routine_3 ++ ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_dir = pl_w->pl_dir ^ 1 ;
			em->atck_no = pl06_at_throw_k ;
		}
	}else{
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_voice_req( pl_w, 43 ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void hold( PLAYER_WORK *pl_w );
static void hold_hit( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl06_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold_hit, hold, hold_hit, hold_hit, hold_hit,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
/*			Held only while the opponent's animation says so	*/
/*==============================================================*/
static void hold_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->cg_type ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl06_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl06_sp_mega ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl06_sp_mega ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl06_sp_mega ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl06_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl06_sp_mega ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl06_sp_mega ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl06_sp_mega ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl06_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl06_sp_blast ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl06_sp_gene ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl06_sp_cyclone ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl06_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl06_sp_blast ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl06_sp_gene ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl06_sp_cyclone ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Clear the character's work							*/
/****************************************************************/
void pl06_tlocal_init( PLAYER_WORK *pl_w )
{
	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl06.c" End of File	*/
