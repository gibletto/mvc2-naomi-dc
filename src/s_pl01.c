/* Zangief (character 01): command checks, normal attacks, dashes, appearance and win poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "burst_ef_old.h"
#include "pl_act.h"
#include "pl01_def.h"

void pl01_main( PLAYER_WORK *pl_w );
void pl01_com_ck( PLAYER_WORK *pl_w );
void pl01_atck_set( PLAYER_WORK *pl_w );
void pl01_atck( PLAYER_WORK *pl_w );
void pl01_dash( PLAYER_WORK *pl_w );
void pl01_noki( PLAYER_WORK *pl_w );
void pl01_appear( PLAYER_WORK *pl_w );
void pl01_win( PLAYER_WORK *pl_w );
void pl01_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl01_catch_ck( PLAYER_WORK *pl_w );
void pl01_catch_set( PLAYER_WORK *pl_w );
void pl01_catch( PLAYER_WORK *pl_w );
void pl01_hold( PLAYER_WORK *pl_w );
int pl01_delay_ck( PLAYER_WORK *pl_w );
void pl01_t_kick( PLAYER_WORK *pl_w );
void pl01_upper( PLAYER_WORK *pl_w );
void pl01_sp_unison( PLAYER_WORK *pl_w );
void pl01_sp_gattai( PLAYER_WORK *pl_w );
void pl01_sp_assist( PLAYER_WORK *pl_w );
void pl01_sp_zero( PLAYER_WORK *pl_w );
int pl01_catch_atck_ck( PLAYER_WORK *pl_w );

/* palettes for ef_color_set(): a flash that grows with a count (lw->mash), and the base for each
   of the six colours, as Zangief and as Mecha Zangief (armor_flag) */
static Sint32 fab_flash_color[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2 } ;
static Sint32 pl_color_tbl[6] = { 0x0e, 0x11, 0x14, 0x17, 0x1a, 0x1d } ;
static Sint32 mecha_color_tbl[6] = { 0x20, 0x23, 0x26, 0x29, 0x2c, 0x2f } ;
static Sint32 uab_flash_color[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2 } ;

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons; the turns of the
   stick read by com_rot_ck give only their count and the buttons */
static Uint16 com_banish[] = { 3, COM_GROUND, pl01_sp_banish, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Banishing Flat */
static Uint16 com_screw[] = { 4, 0, pl01_sp_screw, 0, KEY_PUNCH } ;	/* Spinning Pile Driver (ground and air) */
static Uint16 com_powerbomb[] = { 5, COM_GROUND, pl01_sp_powerbomb, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Flying Powerbomb (Atomic Suplex up close) */
static Uint16 com_fab[] = { 4, COM_GROUND | COM_HYPER | COM_ALL_SW, pl01_sp_fab, 0, KEY_PUNCH } ;	/* Final Atomic Buster */
static Uint16 com_russian[] = { 3, COM_GROUND, pl01_sp_russian, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Aerial Russian Slam */
static Uint16 com_uab[] = { 4, COM_GROUND | COM_HYPER | COM_ALL_SW, pl01_sp_uab, 0, KEY_KICK } ;	/* Ultra Final Atomic Buster (hyper) */
static Uint16 com_iron[] = { 3, COM_GROUND | COM_HYPER, pl01_sp_iron, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_LK } ;	/* Iron Body */
static Uint16 com_vodka[] = { 3, COM_GROUND, pl01_sp_vodka, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Vodka Fire */
static Uint16 com_blizzard[] = { 4, COM_GROUND | COM_HYPER | COM_ALL_SW, pl01_sp_blizzard, 0, KEY_KICK } ;	/* Siberian Blizzard */
static Uint16 com_catch[] = { 3, COM_GROUND, 0, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_LP } ;	/* A command throw */

static void ( *pl01_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl01_main,
	pl01_com_ck,
	pl01_atck_set,
	pl01_atck,
	pl01_dash,
	pl01_noki,
	pl01_appear,
	pl01_win,
	pl01_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl01_catch_ck,	/* returns a value */
	pl01_catch_set,
	pl01_catch,
	pl01_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl01_delay_ck,
	pl01_t_kick,
	pl01_upper,
	pl01_sp_unison,
	pl01_sp_gattai,
	pl01_sp_assist,
	pl01_sp_zero,
	0,
	0,
	0,
	0,
	( void ( * )( PLAYER_WORK *pl_w ) )pl01_catch_atck_ck,
} ;

static Uint8 lariat_ck( PLAYER_WORK *pl_w );
static Uint8 qlariat_ck( PLAYER_WORK *pl_w );
static Uint8 mecha_lariat_ck( PLAYER_WORK *pl_w );
static Uint8 mecha_qlariat_ck( PLAYER_WORK *pl_w );
static Uint8 banish_ck( PLAYER_WORK *pl_w );
static Uint8 screw_ck( PLAYER_WORK *pl_w );
static Uint8 powerbomb_ck( PLAYER_WORK *pl_w );
static Uint8 fab_ck( PLAYER_WORK *pl_w );
static Uint8 russian_ck( PLAYER_WORK *pl_w );
static Uint8 uab_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 iron_ck( PLAYER_WORK *pl_w );
static Uint8 mecha_iron_ck( PLAYER_WORK *pl_w );
static Uint8 vodka_ck( PLAYER_WORK *pl_w );
static Uint8 blizzard_ck( PLAYER_WORK *pl_w );
static Uint8 catch_com_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl01_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl01_tbl[i] ;
}

/****************************************************************/
/*			Commands; Mecha Zangief (armor_flag) has his own	*/
/*			set													*/
/****************************************************************/
void pl01_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( !pl_w->armor_flag ){
		if( uab_ck( pl_w ) ) return ;
		if( fab_ck( pl_w ) ) return ;
		if( lariat_ck( pl_w ) ) return ;
		if( qlariat_ck( pl_w ) ) return ;
		if( screw_ck( pl_w ) ) return ;
		if( iron_ck( pl_w ) ) return ;
		if( powerbomb_ck( pl_w ) ) return ;
		if( banish_ck( pl_w ) ) return ;
		if( russian_ck( pl_w ) ) return ;
	} else {
		if( blizzard_ck( pl_w ) ) return ;
		if( fab_ck( pl_w ) ) return ;
		if( mecha_lariat_ck( pl_w ) ) return ;
		if( mecha_qlariat_ck( pl_w ) ) return ;
		if( screw_ck( pl_w ) ) return ;
		if( mecha_iron_ck( pl_w ) ) return ;
		if( powerbomb_ck( pl_w ) ) return ;
		if( russian_ck( pl_w ) ) return ;
		if( vodka_ck( pl_w ) ) return ;
	}
	if( catch_com_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			A turn of the stick and a punch, also in a jump		*/
/*			(once, unless super jumping); not with the stick	*/
/*			up on the ground									*/
/*==============================================================*/
static Uint8 lariat_ck( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( !sw_now_ck( pl_w, 0x0300, &sw, 0 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag ){
			if( pl_w->jump_com_cnt ) return 0 ;
			else pl_w->jump_com_cnt = 1 ;
		}
	} else if( pl_w->pl_sw_new & KEY_UP ) return 0 ;
	lw->lever = pl_w->atck_lever ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 ++ ;
	pl_w->pl_sp_type = pl01_sp_lariat ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			The same with a kick								*/
/*==============================================================*/
static Uint8 qlariat_ck( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( !sw_now_ck( pl_w, 0x0060, &sw, 1 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag ){
			if( pl_w->jump_com_cnt ) return 0 ;
			pl_w->jump_com_cnt = 1 ;
		}
	} else if( pl_w->pl_sw_new & KEY_UP ) return 0 ;
	lw->lever = pl_w->atck_lever ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 ++ ;
	pl_w->pl_sp_type = pl01_sp_qlariat ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Mecha Zangief's: on the ground only					*/
/*==============================================================*/
static Uint8 mecha_lariat_ck( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( !sw_now_ck( pl_w, 0x0300, &sw, 0 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	else if( pl_w->atck_lever & KEY_UP ) return 0 ;
	lw->lever = pl_w->atck_lever ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_lariat ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 mecha_qlariat_ck( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( !sw_now_ck( pl_w, 0x0060, &sw, 1 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	else if( pl_w->atck_lever & KEY_UP ) return 0 ;
	lw->lever = pl_w->atck_lever ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_qlariat ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 banish_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_banish, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_banish ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Also in a jump: once, unless super jumping			*/
/*==============================================================*/
static Uint8 screw_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_screw, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		else pl_w->jump_com_cnt = 1 ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	else pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_screw ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 powerbomb_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_powerbomb, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_powerbomb ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fab_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_fab, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_fab ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 russian_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_russian, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_russian ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A level 3 hyper combo								*/
/*==============================================================*/
static Uint8 uab_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_uab, &pl_w->com_w5 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_uab ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 iron_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_iron, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_iron ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 mecha_iron_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_iron, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_iron ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 vodka_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_vodka, &pl_w->com_w7 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_vodka ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blizzard_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_blizzard, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl01_sp_blizzard ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			A throw done by a command, when the opponent is		*/
/*			in reach											*/
/*==============================================================*/
static Uint8 catch_com_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !com_nml_ck( pl_w, com_catch, &pl_w->com_w9 ) ) return 0 ;
	if( !( em = pl_catch_ck( pl_w ) ) ) return 0 ;
	pl_w->catch_no = 0xc9 ;
	pl_catch_set( pl_w, em ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl01_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int fab_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_fab, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl01_sp_fab ;
	return 1 ;
}

/*==============================================================*/
static int uab_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_uab, &pl_w->com_w5 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->delay_sp_type = pl01_sp_uab ;
	return 1 ;
}

/*==============================================================*/
static int blizzard_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_blizzard, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl01_sp_blizzard ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl01_delay_ck( PLAYER_WORK *pl_w )
{
	if( fab_delay_ck( pl_w ) ) return 1 ;
	if( !pl_w->armor_flag ){
		if( uab_delay_ck( pl_w ) ) return 1 ;
	} else {
		if( blizzard_delay_ck( pl_w ) ) return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			Clears atck_pow except during a special move or		*/
/*			a hyper combo										*/
/****************************************************************/
void pl01_main( PLAYER_WORK *pl_w )
{
	if( pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER ) pl_w->atck_pow = 0 ;
}

/*==============================================================*/
/*			The light punch with the stick back is a step		*/
/*			and a punch (routine_2 set)							*/
/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl01_at_back_lp ;
			pl_se_req( pl_w, SE_SWING_L ) ;
			pl_w->nml_level = 1 ;
			pl_w->chain_tbl = hp_tbl ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_STAND_LP ;
			pl_se_req( pl_w, SE_SWING_L ) ;
			pl_w->chain_tbl = lp_tbl ;
			pl_w->nml_level = 0 ;
			pl_w->unk_2a2 = 1 ;
		}
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
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
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
/*			With the stick up or down some punches become		*/
/*			other moves, which end on landing					*/
/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->routine_2 = 2 ;
			pl_w->routine_3 = 0 ;
			pl_w->jump_atk_cnt &= JATK_P_MASK ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->char_no.b.no = 8 ;
			pl_w->atck_no = pl01_at_jdown_lp ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_w->nml_level = 1 ;
			pl_w->sjump_flag = 0 ;
			if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
			else pl_w->chain_tbl = sj_hp_tbl ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_JUMP_LP ;
			pl_se_req( pl_w, SE_SWING_L ) ;
			if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
			else pl_w->chain_tbl = sj_lp_tbl ;
			pl_w->nml_level = 0 ;
		}
		break ;
	case 1:
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->routine_2 = 1 ;
			pl_w->routine_3 = 0 ;
			pl_w->jump_atk_cnt &= JATK_P_MASK ;
			if( pl_w->pos.y < pl_w->ground_y ){
				pl_w->pos.y = pl_w->ground_y ;
				pl_land_set( pl_w ) ;
				break ;
			}
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl01_at_jup_mp ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_w->nml_level = 1 ;
			if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
			else pl_w->chain_tbl = sj_hp_tbl ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MP ;
			pl_se_req( pl_w, SE_SWING_M ) ;
			if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
			else pl_w->chain_tbl = sj_mp_tbl ;
			pl_w->nml_level = 1 ;
		}
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_FORWARD ){
			if( pl_w->pos.y < pl_w->ground_y ){
				pl_w->pos.y = pl_w->ground_y ;
				pl_land_set( pl_w ) ;
				break ;
			}
			pl_w->atck_no = pl01_at_jfwd_hp ;
			pl_w->char_no.b.no = 5 ;
		} else if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->routine_2 = 1 ;
			pl_w->routine_3 = 0 ;
			pl_w->jump_atk_cnt &= JATK_P_MASK ;
			pl_w->atck_no = pl01_at_jdown_hp ;
			if( pl_w->pos.y < pl_w->ground_y ){
				pl_w->pos.y = pl_w->ground_y ;
				pl_land_set( pl_w ) ;
				break ;
			}
			pl_w->char_no.b.no = 7 ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HP ;
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
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->routine_2 = 1 ;
			pl_w->routine_3 = 0 ;
			pl_w->jump_atk_cnt &= JATK_P_MASK ;
			pl_w->atck_no = pl01_at_jdown_lk ;
			if( pl_w->pos.y < pl_w->ground_y ){
				pl_w->pos.y = pl_w->ground_y ;
				pl_land_set( pl_w ) ;
				break ;
			}
			pl_w->char_no.b.no = 6 ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
			else pl_w->chain_tbl = sj_mk_tbl ;
			pl_w->nml_level = 1 ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
			pl_se_req( pl_w, SE_SWING_L ) ;
			if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
			else pl_w->chain_tbl = sj_lk_tbl ;
			pl_w->nml_level = 0 ;
		}
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
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
static void nml_atck_dash( PLAYER_WORK *pl_w )
{
	nml_atck( pl_w ) ;
}

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl01_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck_dash,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The stepping punch shakes the screen on the			*/
/*			animation's mark and turns round on another			*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		switch( pl_w->routine_2 ){
		case 0:
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			break ;
		case 1:
			if( pl_char_move( pl_w ) < 0 ){
				pl_stand_ret( pl_w ) ;
				return ;
			}
			if( HI_BYTE( pl_w->cg_rival ) ){
				HI_BYTE( pl_w->cg_rival ) = 0 ;
				scr_work.quake_level = 1 ;
				scr_work.quake_flag = 1 ;
				pl_se_req( pl_w, 44 ) ;
			}
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->disp_dir ^= 1 ;
			}
			break ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			if( pl_w->pl_dir ) pl_w->pos.x += DOT_X( 4.0f ) ;
			else pl_w->pos.x -= DOT_X( 4.0f ) ;
		}
		if( pl_w->cg_flag1 ){
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->nml_no == 2 && pl_w->cg_flag1 ){
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*==============================================================*/
/*			The heavy kick plays a sound once it hits			*/
/*			(lw->kick_hit), and hits again on the mark			*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		if( !lw->kick_hit && pl_w->hit_flag ){
			lw->kick_hit = 1 ;
			pl_voice_req( pl_w, 4 ) ;
		}
		if( HI_BYTE( pl_w->cg_rival ) ){
			HI_BYTE( pl_w->cg_rival ) = 0 ;
			pl_w->atck_no = pl01_at_stand_hk_2 ;
			pl_w->hit_flag = pl_w->atck_attr = 0 ;
			pl_w->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->nml_no == 2 && pl_w->cg_flag1 ){
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->cg_flag1 = 0 ;
	}
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
/*			The light punch with the stick down: a dive to		*/
/*			the floor on the animation's mark					*/
/*==============================================================*/
static void lever_atck0_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_w->routine_3 ++ ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->spd.x = DOT_X( 8.5f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*==============================================================*/
/*			Shakes the screen when it lands						*/
/*==============================================================*/
static void lever_atck0_dive( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 44 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump_P, 9 ) ;
	}
}

/*==============================================================*/
static void lever_atck0_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->disp_dir ^= 1 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->disp_dir ^= 1 ;
	}
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		switch( pl_w->routine_2 ){
		case 0:
			if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
			if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
			break ;
		case 1:
			if( pl_char_move( pl_w ) < 0 ){
				pl_jump_ret( pl_w ) ;
				pl_char_set( pl_w, Char_Jump, 12 ) ;
			}
			if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
			break ;
		case 2:
			switch( pl_w->routine_3 ){
			case 0:
				lever_atck0_start( pl_w ) ;
				break ;
			case 1:
				lever_atck0_dive( pl_w ) ;
				break ;
			case 2:
				lever_atck0_land( pl_w ) ;
				break ;
			}
			break ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		break ;
	case 2:
		switch( pl_w->routine_2 ){
		case 0:
			if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
			break ;
		case 1:
			pl_char_move( pl_w ) ;
			break ;
		}
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_w->routine_2 ) pl_char_move( pl_w ) ;
		else if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		if( pl_w->nml_no == 2 && pl_w->cg_flag1 ){
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	}
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl01_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			Thrown up by an upper attack						*/
/****************************************************************/
void pl01_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		if( pl_w->atck_pk == 0 ){
			pl_w->atck_no = pl01_at_upper ;
			pl_w->pl_stance = STANCE_CROUCH ;
			pl_char_set( pl_w, Char_Extra, 3 ) ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_start( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_stop( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash: a throw is tried all the way				*/
/****************************************************************/
void pl01_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_stop,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Mecha Zangief dashes slower							*/
/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 30 ;
		if( !pl_w->armor_flag ){
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 6.5f ) : -DOT_X( 6.5f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
		} else {
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 3.5f ) : -DOT_X( 3.5f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.104166664f : 0.104166664f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( ( em = pl_catch_ck( pl_w ) ) != 0 ){
		pl_w->routine_3 = 0 ;
		pl_w->routine_2 = 0 ;
		pl_w->catch_no = 7 ;
		pl_catch_set( pl_w, em ) ;
		return ;
	}
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			There is no back dash								*/
/****************************************************************/
void pl01_noki( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void lose_ko_init( PLAYER_WORK *pl_w );
static void lose_ko_move( PLAYER_WORK *pl_w );
static void lose_time_init( PLAYER_WORK *pl_w );
static void lose_time_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl01_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init,
		appear_move,
	} ;

	appear_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
	}
}

/*==============================================================*/
/*			One of two poses, on the frame count				*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( Sys->game_timer0 & 1 ) pl_char_set( pl_w, Char_Win, 0 ) ;
	else pl_char_set( pl_w, Char_Win, 1 ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init,
		win_ko_move,
	} ;

	win_ko_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void lose_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*--------------------------------------------------------------*/
static void lose_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_ko( PLAYER_WORK *pl_w )
{
	static void ( *lose_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		lose_ko_init,
		lose_ko_move,
	} ;

	lose_ko_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void lose_time_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*--------------------------------------------------------------*/
static void lose_time_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time( PLAYER_WORK *pl_w )
{
	static void ( *lose_time_jmp[])( PLAYER_WORK *pl_w ) = {
		lose_time_init,
		lose_time_move,
	} ;

	lose_time_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/****************************************************************/
void pl01_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_ko,
		win_time_ko,
		lose_time,
		lose_ko,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	win_jmp[ pl_w->type ]( pl_w ) ;
}

static void sp_lariat( PLAYER_WORK *pl_w );
static void sp_qlariat( PLAYER_WORK *pl_w );
static void sp_banish( PLAYER_WORK *pl_w );
static void sp_screw( PLAYER_WORK *pl_w );
static void sp_powerbomb( PLAYER_WORK *pl_w );
static void sp_fab( PLAYER_WORK *pl_w );
static void sp_russian( PLAYER_WORK *pl_w );
static void sp_team( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_uab( PLAYER_WORK *pl_w );
static void sp_iron( PLAYER_WORK *pl_w );
static void sp_vodka( PLAYER_WORK *pl_w );
static void sp_blizzard( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl01_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_lariat,					/* 00 Double Lariat */
		sp_qlariat,					/* 01 Quick Double Lariat */
		sp_banish,					/* 02 Banishing Flat */
		sp_screw,					/* 03 Spinning Pile Driver (ground and air) */
		sp_powerbomb,					/* 04 Flying Powerbomb (Atomic Suplex up close) */
		sp_fab,					/* 05 Final Atomic Buster (hyper) */
		sp_russian,					/* 06 Aerial Russian Slam */
		sp_russian,					/* 07 */
		sp_team,					/* 08 His part in a team hyper combo */
		sp_chouhatsu,					/* 09 the taunt */
		sp_russian,					/* 0a */
		sp_russian,					/* 0b */
		sp_russian,					/* 0c */
		sp_russian,					/* 0d */
		sp_russian,					/* 0e */
		sp_russian,					/* 0f */
		sp_russian,					/* 10 */
		sp_uab,					/* 11 Ultra Final Atomic Buster (hyper) */
		sp_iron,					/* 12 Iron Body: Mecha Zangief and back (hyper) */
		sp_vodka,					/* 13 Vodka Fire */
		sp_blizzard,					/* 14 Siberian Blizzard (Mecha Zangief's hyper) */
		sp_snapback,					/* 15 the snapback */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

/*==============================================================*/
/*			Every lariat starts on the ground, standing			*/
/*			still												*/
/*==============================================================*/
static void sp_start( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
}

static void lariat_ground( PLAYER_WORK *pl_w );
static void lariat_init( PLAYER_WORK *pl_w );
static void lariat_move( PLAYER_WORK *pl_w );
static void lariat_air( PLAYER_WORK *pl_w );
static void lariat_j_init( PLAYER_WORK *pl_w );
static void lariat_j_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The lariat with the punches: on the ground it		*/
/*			can be steered										*/
/*==============================================================*/
static void sp_lariat( PLAYER_WORK *pl_w )
{
	static void ( *lariat_jmp[])( PLAYER_WORK *pl_w ) = {
		lariat_ground, lariat_air,
	} ;

	lariat_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lariat_ground( PLAYER_WORK *pl_w )
{
	static void ( *lariat_g_jmp[])( PLAYER_WORK *pl_w ) = {
		lariat_init, lariat_move,
	} ;

	lariat_g_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lariat_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl01_at_lariat ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	sp_start( pl_w ) ;
	pl_voice_req( pl_w, 3 ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void lariat_move( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_prm ){
		pl_w->atck_no = pl_w->cg_prm ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_prm = 0 ;
	}
	if( pl_w->cg_type == 0 ){
		pl_w->spd.x = 0.0f ;
	} else if( pl_w->cpu_flag ){
		if( lw->lever & 0x0800 ) pl_w->spd.x = -DOT_X( 2.0f ) ;
		else if( lw->lever & 0x0400 ) pl_w->spd.x = DOT_X( 2.0f ) ;
		else pl_w->spd.x = 0.0f ;
	} else if( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ){
		if( pl_w->pl_new_lvbt & KEY_FORWARD ) pl_w->spd.x = -DOT_X( 2.0f ) ;
		else if( pl_w->pl_new_lvbt & KEY_BACK ) pl_w->spd.x = DOT_X( 2.0f ) ;
	} else {
		pl_w->spd.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void lariat_air( PLAYER_WORK *pl_w )
{
	static void ( *lariat_a_jmp[])( PLAYER_WORK *pl_w ) = {
		lariat_j_init, lariat_j_move,
	} ;

	lariat_a_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			In the air it goes the way the stick is held		*/
/*==============================================================*/
static void lariat_j_init( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl01_at_jlariat ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.y = DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	pl_w->spd.x = DOT_X( 2.0f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		if( pl_w->cpu_flag ){
			if( !( lw->lever & 0x0800 ) ) pl_w->spd.x = -pl_w->spd.x ;
		} else if( !( pl_w->pl_new_lvbt & KEY_FORWARD ) ) pl_w->spd.x = -pl_w->spd.x ;
	} else {
		if( pl_w->cpu_flag ){
			if( !( lw->lever & 0x0400 ) ) pl_w->spd.x = -pl_w->spd.x ;
		} else if( !( pl_w->pl_new_lvbt & KEY_BACK ) ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void lariat_j_move( PLAYER_WORK *pl_w )
{
	if( pl_land_ck( pl_w ) ){
		pl_land_set( pl_w ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

static void qlariat_ground( PLAYER_WORK *pl_w );
static void qlariat_init( PLAYER_WORK *pl_w );
static void qlariat_move( PLAYER_WORK *pl_w );
static void qlariat_air( PLAYER_WORK *pl_w );
static void qlariat_j_init( PLAYER_WORK *pl_w );
static void qlariat_j_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The lariat with the kicks, slower					*/
/*==============================================================*/
static void sp_qlariat( PLAYER_WORK *pl_w )
{
	static void ( *qlariat_jmp[])( PLAYER_WORK *pl_w ) = {
		qlariat_ground, qlariat_air,
	} ;

	qlariat_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void qlariat_ground( PLAYER_WORK *pl_w )
{
	static void ( *qlariat_g_jmp[])( PLAYER_WORK *pl_w ) = {
		qlariat_init, qlariat_move,
	} ;

	qlariat_g_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void qlariat_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl01_at_qlariat ;
	HIT_RESET( pl_w ) ;
	sp_start( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_voice_req( pl_w, 3 ) ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
}

/*--------------------------------------------------------------*/
static void qlariat_move( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 0 ){
		pl_w->spd.x = 0.0f ;
	} else if( pl_w->cpu_flag ){
		if( lw->lever & 0x0800 ) pl_w->spd.x = -2.5f ;
		else if( lw->lever & 0x0400 ) pl_w->spd.x = 2.5f ;
		else pl_w->spd.x = 0.0f ;
	} else if( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ){
		if( pl_w->pl_new_lvbt & KEY_FORWARD ) pl_w->spd.x = -2.5f ;
		else if( pl_w->pl_new_lvbt & KEY_BACK ) pl_w->spd.x = 2.5f ;
	} else {
		pl_w->spd.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void qlariat_air( PLAYER_WORK *pl_w )
{
	static void ( *qlariat_a_jmp[])( PLAYER_WORK *pl_w ) = {
		qlariat_j_init, qlariat_j_move,
	} ;

	qlariat_a_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void qlariat_j_init( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl01_at_jqlariat ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.y = DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	pl_w->spd.x = 2.5f ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		if( pl_w->cpu_flag ){
			if( !( lw->lever & 0x0800 ) ) pl_w->spd.x = -pl_w->spd.x ;
		} else if( !( pl_w->pl_new_lvbt & KEY_FORWARD ) ) pl_w->spd.x = -pl_w->spd.x ;
	} else {
		if( pl_w->cpu_flag ){
			if( !( lw->lever & 0x0400 ) ) pl_w->spd.x = -pl_w->spd.x ;
		} else if( !( pl_w->pl_new_lvbt & KEY_BACK ) ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void qlariat_j_move( PLAYER_WORK *pl_w )
{
	if( pl_land_ck( pl_w ) ){
		pl_land_set( pl_w ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}
static void em_voice( PLAYER_WORK *pl_w );

static void banish_init( PLAYER_WORK *pl_w );
static void banish_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Steps forward on the animation's marks				*/
/*==============================================================*/
static void sp_banish( PLAYER_WORK *pl_w )
{
	static void ( *banish_jmp[])( PLAYER_WORK *pl_w ) = {
		banish_init, banish_move,
	} ;

	banish_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void banish_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl01_at_banish ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 6 ) ;
	sp_start( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 4 ) ;
}

/*--------------------------------------------------------------*/
static void banish_move( PLAYER_WORK *pl_w )
{
	static Sint16 banish_spd[][2] = {
		{ 0x1000, 0x1400 },
		{ 0x1400, 0x1c00 },
		{ 0x1800, 0x3800 },
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->spd.x = 0.0f ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_w->spd.x = banish_spd[HI_BYTE( pl_w->cg_rival ) - 1][pl_w->atck_level] * 1.6666666f / 256.0f ;
		if( !pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		if( HI_BYTE( pl_w->cg_rival ) == 2 ) shl01_00_set( pl_w ) ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
	}
}

static void screw_ground( PLAYER_WORK *pl_w );
static void screw_air( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A throw that jumps with the opponent and drops		*/
/*			on them												*/
/*==============================================================*/
static void sp_screw( PLAYER_WORK *pl_w )
{
	static void ( *screw_jmp[])( PLAYER_WORK *pl_w ) = {
		screw_ground, screw_air,
	} ;

	screw_jmp[pl_w->routine_2]( pl_w ) ;
}

static void screw_init( PLAYER_WORK *pl_w );
static void screw_catch( PLAYER_WORK *pl_w );
static void screw_hold( PLAYER_WORK *pl_w );
static void screw_jump( PLAYER_WORK *pl_w );
static void screw_rise( PLAYER_WORK *pl_w );
static void screw_bound( PLAYER_WORK *pl_w );
static void screw_slam( PLAYER_WORK *pl_w );
static void screw_fall( PLAYER_WORK *pl_w );
static void screw_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void screw_ground( PLAYER_WORK *pl_w )
{
	static void ( *screw_g_jmp[])( PLAYER_WORK *pl_w ) = {
		screw_init, screw_catch, screw_hold, screw_jump, screw_rise,
		screw_bound, screw_slam, screw_fall, screw_end,
	} ;

	screw_g_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void screw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 10 ) ;
	sp_start( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 8 ) ;
}

/*==============================================================*/
/*			Misses when the animation ends						*/
/*==============================================================*/
static void screw_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 = 8 ;
		pl_char_set( pl_w, Char_Catch, 9 ) ;
	} else if( ( em = pl_catch_ck2( pl_w ) ) != 0 ){
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_voice_req( pl_w, 5 ) ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 10 ) ;
		pl_w->catch_no = 0xc3 ;
		pl_sp_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_hold( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
		pl_char_set( pl_w, Char_Catch, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_jump( PLAYER_WORK *pl_w )
{
	static Sint32 screw_spd[][3] = {
		{ 0x00058000, 0x000e8000, -0x0000c000 },
		{ 0x00038000, 0x00108000, -0x00008000 },
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = screw_spd[pl_w->atck_level][0] * 1.6666666f / 65536.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = screw_spd[pl_w->atck_level][1] * 2.142857f / 65536.0f ;
		pl_w->acc.y = screw_spd[pl_w->atck_level][2] * 2.142857f / 65536.0f ;
		if( pl_w->pos.x >= 0.0f ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Catch, 12 ) ;
	}
}

/*==============================================================*/
/*			Mashing the buttons makes the slam stronger			*/
/*			(lw->mash)											*/
/*==============================================================*/
static void screw_rise( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( lever_gacha_ck( pl_w ) ){
		if( pl_w->cpu_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				if( pl_w->cg_ctr != 1 ) pl_w->cg_ctr -- ;
			}
		} else {
			pl_w->cg_ctr = 1 ;
		}
		lw->mash ++ ;
		if( lw->mash > 20 ) lw->mash = 20 ;
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_bound( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 5.0f ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 14 ) ;
	}
}

/*==============================================================*/
/*			The hit: the stronger for the mashing				*/
/*==============================================================*/
static void screw_slam( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 33 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl_w->atck_level + 37 ;
		pl_w->atck_no = pl_w->atck_level + 37 ;
		pl_w->atck_pow = lw->mash / 2 + 32 ;
	}
}

/*--------------------------------------------------------------*/
static void screw_fall( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->atck_pow = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void screw_j_init( PLAYER_WORK *pl_w );
static void screw_j_catch( PLAYER_WORK *pl_w );
static void screw_j_hold( PLAYER_WORK *pl_w );
static void screw_j_jump( PLAYER_WORK *pl_w );
static void screw_j_rise( PLAYER_WORK *pl_w );
static void screw_j_bound( PLAYER_WORK *pl_w );
static void screw_j_slam( PLAYER_WORK *pl_w );
static void screw_j_fall( PLAYER_WORK *pl_w );
static void screw_j_end( PLAYER_WORK *pl_w );
static void screw_j_miss( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void screw_air( PLAYER_WORK *pl_w )
{
	static void ( *screw_a_jmp[])( PLAYER_WORK *pl_w ) = {
		screw_j_init, screw_j_catch, screw_j_hold, screw_j_jump, screw_j_rise,
		screw_j_bound, screw_j_slam, screw_j_fall, screw_j_end, screw_j_miss,
	} ;

	screw_a_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void screw_j_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl01_at_jscrew ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 15 ) ;
}

/*==============================================================*/
/*			A miss slows the fall								*/
/*==============================================================*/
static void screw_j_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 = 9 ;
		pl_w->spd.x = pl_w->spd.x / 16.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = pl_w->spd.y / 8.0f ;
		pl_w->acc.y = pl_w->acc.y / 64.0f ;
		pl_char_set( pl_w, Char_Catch, 16 ) ;
	} else if( ( em = pl_catch_ck2( pl_w ) ) != 0 ){
		pl_voice_req( pl_w, 5 ) ;
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 5, 5 ) ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 17 ) ;
		pl_w->catch_no = 0xc4 ;
		pl_sp_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_hold( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
		pl_char_set( pl_w, Char_Catch, 18 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_jump( PLAYER_WORK *pl_w )
{
	static Sint32 screw_j_spd[][3] = {
		{ 0x00040000, 0x000d8000, -0x0000c000 },
		{ 0x00028000, 0x000e8000, -0x00008000 },
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = screw_j_spd[pl_w->atck_level][0] * 1.6666666f / 65536.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = screw_j_spd[pl_w->atck_level][1] * 2.142857f / 65536.0f ;
		pl_w->acc.y = screw_j_spd[pl_w->atck_level][2] * 2.142857f / 65536.0f ;
		if( pl_w->pos.x >= 0.0f ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Catch, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_rise( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( lever_gacha_ck( pl_w ) ){
		if( pl_w->cpu_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				if( pl_w->cg_ctr != 1 ) pl_w->cg_ctr -- ;
			}
		} else {
			pl_w->cg_ctr = 1 ;
		}
		lw->mash ++ ;
		if( lw->mash > 20 ) lw->mash = 20 ;
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_bound( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 2.5f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Catch, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_slam( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 33 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->catch_type = 2 ;
		em->atck_no = pl_w->atck_level + 61 ;
		pl_w->atck_no = pl_w->atck_level + 61 ;
		pl_w->atck_pow = lw->mash / 2 + 32 ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_fall( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->atck_pow = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_j_miss( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->atck_pow = 0 ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
}

static void powerbomb_init( PLAYER_WORK *pl_w );
static void powerbomb_dash( PLAYER_WORK *pl_w );
static void powerbomb_run( PLAYER_WORK *pl_w );
static void powerbomb_stop( PLAYER_WORK *pl_w );
static void powerbomb_grab( PLAYER_WORK *pl_w );
static void powerbomb_run_grab( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A running throw: it grabs on the way, or at the		*/
/*			end of the run										*/
/*==============================================================*/
static void sp_powerbomb( PLAYER_WORK *pl_w )
{
	static void ( *powerbomb_jmp[])( PLAYER_WORK *pl_w ) = {
		powerbomb_init, powerbomb_dash, powerbomb_run, powerbomb_stop, powerbomb_grab, powerbomb_run_grab,
	} ;

	powerbomb_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void powerbomb_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	if( pl_w->team_type == 8 ) pl_char_set( pl_w, Char_Catch, 61 ) ;
	else pl_char_set( pl_w, Char_Catch, 22 ) ;
	armor_set( pl_w, 10, 30, 60 ) ;
}

/*==============================================================*/
/*			Mecha Zangief runs slower, and longer				*/
/*==============================================================*/
static void powerbomb_dash( PLAYER_WORK *pl_w )
{
	static Sint16 powerbomb_time[][2] = {
		{ 0x20, 0x30 },
		{ 0x30, 0x70 },
	} ;
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->armor_timer = 2 ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_char_move( pl_w ) ;
		if( ( em = pl_catch_ck( pl_w ) ) != 0 ){
			pl_w->routine_2 = 4 ;
			pl_w->routine_3 = 0 ;
			ofs.x = -DOT_X( 88.0f ) ;
			ofs.y = DOT_Y( 80.0f ) ;
			nage_hit_set( pl_w, &ofs ) ;
			scr_excep_set( pl_w, 5, 5 ) ;
			pl_w->catch_no = 0xc5 ;
			pl_char_set( pl_w, Char_Catch, 24 ) ;
			pl_sp_catch_set( pl_w, em ) ;
			return ;
		}
	} else {
		pl_w->routine_2 ++ ;
		if( !pl_w->armor_flag ){
			if( pl_w->team_type == 8 ){
				pl_w->spd.x = 5.0f ;
				pl_w->acc.x = 0.0f ;
			} else {
				pl_w->spd.x = DOT_X( 3.5f ) ;
				pl_w->acc.x = 0.013020833f ;
			}
			pl_w->timer0 = powerbomb_time[0][pl_w->atck_level] ;
		} else {
			pl_w->spd.x = DOT_X( 2.0f ) ;
			pl_w->acc.x = 0.013020833f ;
			pl_w->timer0 = powerbomb_time[1][pl_w->atck_level] ;
		}
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*==============================================================*/
/*			Stops on the timer, or when the stick is let go		*/
/*==============================================================*/
static void powerbomb_run( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->armor_timer = 2 ;
	pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( ( em = pl_catch_ck( pl_w ) ) != 0 ){
		pl_w->routine_2 = 5 ;
		pl_w->routine_3 = 0 ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_w->catch_no = 0xc6 ;
		pl_char_set( pl_w, Char_Catch, 29 ) ;
		pl_sp_catch_set( pl_w, em ) ;
		return ;
	}
	pl_w->timer1 = 0 ;
	if( pl_w->pl_dir ){
		if( pl_w->wall_flag & WALL_RIGHT ) pl_w->timer1 = 1 ;
	} else {
		if( pl_w->wall_flag & WALL_LEFT ) pl_w->timer1 = 1 ;
	}
	if( pl_w->timer1 || -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 23 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void powerbomb_g_hold( PLAYER_WORK *pl_w )
{
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_g_lift( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		if( pl_w->pl_dir ) pl_w->pos.x += 160.0f ;
		else pl_w->pos.x -= 160.0f ;
		pl_char_set( pl_w, Char_Catch, 26 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( pl_w ) ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl01_at_powerbomb_h ;
			em->atck_no = pl01_at_powerbomb_h ;
		} else {
			pl_w->atck_no = pl_w->atck_level + 63 ;
			em->atck_no = pl_w->atck_level + 63 ;
		}
		catch_damage( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_g_jump( PLAYER_WORK *pl_w )
{
	static Sint32 powerbomb_g_spd[][4] = {
		{ 0x00038000, 0x00000000, 0x000c8000, -0x0000e000 },
		{ 0x00050000, 0x00000000, 0x000d8000, -0x0000a000 },
	} ;

	pl_w->sp_ng_timer = 3 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = powerbomb_g_spd[pl_w->atck_level][0] * 1.6666666f / 65536.0f ;
		pl_w->acc.x = powerbomb_g_spd[pl_w->atck_level][1] * 1.6666666f / 65536.0f ;
		pl_w->spd.y = powerbomb_g_spd[pl_w->atck_level][2] * 2.142857f / 65536.0f ;
		pl_w->acc.y = powerbomb_g_spd[pl_w->atck_level][3] * 2.142857f / 65536.0f ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*==============================================================*/
/*			At the top of the jump it goes forward				*/
/*==============================================================*/
static void powerbomb_g_turn( PLAYER_WORK *pl_w )
{
	static Sint16 powerbomb_g_turn_spd[] = { 0x0280, 0x0480 } ;

	pl_w->sp_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y > 0.0f ) return ;
	pl_w->routine_3 ++ ;
	pl_w->spd.x = powerbomb_g_turn_spd[pl_w->atck_level] * 1.6666666f / 256.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
}

/*--------------------------------------------------------------*/
static void powerbomb_g_land( PLAYER_WORK *pl_w )
{
	pl_w->sp_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_g_bound( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 3.5f ) ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 33 ) ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		if( pl_w->team_type == 3 ){
			em->atck_no = pl01_at_powerbomb_slam_h ;
			pl_w->atck_no = pl01_at_powerbomb_slam_h ;
		} else {
			em->atck_no = pl_w->atck_level + 65 ;
			pl_w->atck_no = pl_w->atck_level + 65 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_g_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_g_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void powerbomb_grab( PLAYER_WORK *pl_w )
{
	static void ( *powerbomb_g_jmp[])( PLAYER_WORK *pl_w ) = {
		powerbomb_g_hold, powerbomb_g_lift, powerbomb_g_jump, powerbomb_g_turn,
		powerbomb_g_land, powerbomb_g_bound, powerbomb_g_fall, powerbomb_g_end,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	powerbomb_g_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void powerbomb_r_hold( PLAYER_WORK *pl_w )
{
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_r_jump( PLAYER_WORK *pl_w )
{
	static Sint32 powerbomb_r_spd[][4] = {
		{ 0x00038000, 0x00000000, 0x000c8000, -0x0000c000 },
		{ 0x00050000, 0x00000000, 0x000d0000, -0x0000a000 },
	} ;

	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = powerbomb_r_spd[pl_w->atck_level][0] * 1.6666666f / 65536.0f ;
		pl_w->acc.x = powerbomb_r_spd[pl_w->atck_level][1] * 1.6666666f / 65536.0f ;
		pl_w->spd.y = powerbomb_r_spd[pl_w->atck_level][2] * 2.142857f / 65536.0f ;
		pl_w->acc.y = powerbomb_r_spd[pl_w->atck_level][3] * 2.142857f / 65536.0f ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_r_land( PLAYER_WORK *pl_w )
{
	pl_w->sp_ng_timer = 3 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_r_bound( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 3.5f ) ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 33 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		if( pl_w->team_type == 3 ){
			em->atck_no = pl01_at_powerbomb_run_h ;
			pl_w->atck_no = pl01_at_powerbomb_run_h ;
		} else {
			em->atck_no = pl_w->atck_level + 67 ;
			pl_w->atck_no = pl_w->atck_level + 67 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_r_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void powerbomb_r_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void powerbomb_run_grab( PLAYER_WORK *pl_w )
{
	static void ( *powerbomb_r_jmp[])( PLAYER_WORK *pl_w ) = {
		powerbomb_r_hold, powerbomb_r_jump, powerbomb_r_land, powerbomb_r_bound, powerbomb_r_fall, powerbomb_r_end,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	powerbomb_r_jmp[pl_w->routine_3]( pl_w ) ;
}

static void fab_init( PLAYER_WORK *pl_w );
static void fab_run( PLAYER_WORK *pl_w );
static void fab_grab( PLAYER_WORK *pl_w );
static void fab_stop( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a running throw that slams the		*/
/*			opponent again and again							*/
/*==============================================================*/
static void sp_fab( PLAYER_WORK *pl_w )
{
	static void ( *fab_jmp[])( PLAYER_WORK *pl_w ) = {
		fab_init, fab_run, fab_grab, fab_stop,
	} ;

	fab_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fab_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_w->spd.x = DOT_X( 4.0f ) ;
	pl_w->acc.x = -0.104166664f ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->timer0 = 40 ;
	pl_char_set( pl_w, Char_Catch, 35 ) ;
	armor_set( pl_w, 10, 50, 60 ) ;
}

/*--------------------------------------------------------------*/
static void fab_run( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->armor_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type > 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 72.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		return ;
	}
	if( pl_w->cg_type != 0 ){
		pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	}
	if( !HI_BYTE( pl_w->cg_rival ) ){
		pl_smoke_set( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
	pl_char_move( pl_w ) ;
	if( ( em = pl_catch_ck( pl_w ) ) != 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_w->catch_no = 0xca ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 37 ) ;
		pl_sp_catch_set( pl_w, em ) ;
	} else {
		if( pl_w->pl_dir ){
			if( pl_w->wall_flag & WALL_RIGHT ){
				pl_w->routine_2 = 3 ;
				pl_char_set( pl_w, Char_Catch, 36 ) ;
				return ;
			}
		} else if( pl_w->wall_flag & WALL_LEFT ){
			pl_w->routine_2 = 3 ;
			pl_char_set( pl_w, Char_Catch, 36 ) ;
			return ;
		}
		if( -- pl_w->timer0 == 0 ){
			pl_w->routine_2 = 3 ;
			pl_char_set( pl_w, Char_Catch, 36 ) ;
		}
	}
}

/*==============================================================*/
static void fab_g_hold( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
		pl_char_set( pl_w, Char_Catch, 38 ) ;
	}
}

/*==============================================================*/
/*			Mecha Zangief's slams shake the floor				*/
/*==============================================================*/
static void fab_g_lift( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		if( pl_w->pl_dir ) pl_w->pos.x += 160.0f ;
		else pl_w->pos.x -= 160.0f ;
		pl_char_set( pl_w, Char_Catch, 39 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( pl_w ) ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = pl01_at_fab_1 ;
		em->atck_no = pl01_at_fab_1 ;
		if( !pl_w->armor_flag ){
			ofs.x = -160.0f ;
			ofs.y = 0.0f ;
			hit_mark_set( pl_w, &ofs, 2 ) ;
		} else {
			ofs.x = em->pos.x ;
			ofs.y = pl_w->ground_y ;
			burst_pos_set( &ofs, 1 ) ;
			pl_se_req( pl_w, 73 ) ;
		}
		catch_damage( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_lift2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		if( pl_w->pl_dir ) pl_w->pos.x += 160.0f ;
		else pl_w->pos.x -= 160.0f ;
		pl_char_set( pl_w, Char_Catch, 40 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( pl_w ) ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = pl01_at_fab_2 ;
		em->atck_no = pl01_at_fab_2 ;
		if( !pl_w->armor_flag ){
			ofs.x = -160.0f ;
			ofs.y = 0.0f ;
			hit_mark_set( pl_w, &ofs, 2 ) ;
		} else {
			ofs.x = em->pos.x ;
			ofs.y = pl_w->ground_y ;
			burst_pos_set( &ofs, 1 ) ;
			pl_se_req( pl_w, 73 ) ;
		}
		catch_damage( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_jump( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 3.5f ) ;
		pl_w->acc.x = -0.052083332f ;
		pl_w->spd.y = DOT_Y( 14.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_rise( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( lever_gacha_ck( pl_w ) ){
		if( pl_w->cpu_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				if( pl_w->cg_ctr != 1 ) pl_w->cg_ctr -- ;
			}
		} else {
			pl_w->cg_ctr = 1 ;
		}
		lw->mash ++ ;
		if( lw->mash > 15 ) lw->mash = 15 ;
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 42 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_slam( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = lw->mash / 4 + 72 ;
		em->atck_no = lw->mash / 4 + 72 ;
		catch_damage( pl_w ) ;
		pl_w->atck_pow = 0 ;
		em = pl_w->catch_w ;
		if( !pl_w->armor_flag ){
			ofs.x = -DOT_X( 64.0f ) ;
			ofs.y = 0.0f ;
			hit_mark_set( pl_w, &ofs, 2 ) ;
		} else {
			ofs.x = em->pos.x ;
			ofs.y = pl_w->ground_y ;
			burst_pos_set( &ofs, 1 ) ;
			pl_se_req( pl_w, 73 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void fab_g_up( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 14.5f ) ;
		pl_w->acc.y = -0.6696428f ;
		pl_char_set( pl_w, Char_Catch, 43 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = pl01_at_fab_3 ;
		em->atck_no = pl01_at_fab_3 ;
		em->catch_type = 17 ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_wait( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			Catches the opponent again as they fall past		*/
/*==============================================================*/
static void fab_g_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	em = pl_w->catch_w ;
	if( em->pos.y - pl_w->pos.y + -DOT_Y( 8.0f ) < -DOT_Y( 32.0f ) ){
		pl_w->routine_3 ++ ;
		pl_w->catch_no = 0xca ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_voice_req( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Catch, 17 ) ;
		pl_sp_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_air_hold( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 44 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_dive( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -1.2053571f ;
		pl_char_set( pl_w, Char_Catch, 45 ) ;
	}
}

/*==============================================================*/
/*			Flashes the more the buttons are mashed				*/
/*==============================================================*/
static void fab_g_drop( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( lever_gacha_ck( pl_w ) ){
		if( pl_w->cpu_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				if( pl_w->cg_ctr != 1 ) pl_w->cg_ctr -- ;
			}
		} else {
			pl_w->cg_ctr = 1 ;
		}
		lw->mash ++ ;
		if( lw->mash > 15 ) lw->mash = 15 ;
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 46 ) ;
		return ;
	}
	if( lw->mash > 3 ){
		if( !pl_w->armor_flag ) ef_color_set( pl_w, 0, pl_color_tbl[pl_w->pl_color] + fab_flash_color[lw->mash], 1 ) ;
		else ef_color_set( pl_w, 0, mecha_color_tbl[pl_w->pl_color] + fab_flash_color[lw->mash], 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_shake( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		ofs.x = em->pos.x ;
		ofs.y = pl_w->ground_y ;
		if( !pl_w->armor_flag ){
			burst_pos_set( &ofs, 1 ) ;
			pl_se_req( pl_w, 73 ) ;
		} else {
			burst_pos_set( &ofs, 1 ) ;
			burst_set( pl_w, &ofs, 4, 126 ) ;
			pl_se_req( pl_w, 74 ) ;
		}
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 14 ) ;
	}
}

/*==============================================================*/
/*			The last hit: its strength from the mashing			*/
/*==============================================================*/
static void fab_g_bound( PLAYER_WORK *pl_w )
{
	static Uint8 fab_hit[][2] = {
		{ 2, 0x4c }, { 2, 0x4d }, { 2, 0x4e }, { 9, 0x4f },
	} ;
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->spd.y = DOT_Y( 9.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 33 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->catch_type = fab_hit[lw->mash / 4][0] ;
		em->atck_no = fab_hit[lw->mash / 4][1] ;
		pl_w->atck_no = fab_hit[lw->mash / 4][1] ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->atck_pow = lw->mash / 2 + 32 ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_g_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->atck_pow = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fab_grab( PLAYER_WORK *pl_w )
{
	static void ( *fab_g_jmp[])( PLAYER_WORK *pl_w ) = {
		fab_g_hold, fab_g_lift, fab_g_lift2, fab_g_jump,
		fab_g_rise, fab_g_slam, fab_g_up, fab_g_wait,
		fab_g_catch, fab_g_air_hold, fab_g_dive, fab_g_drop,
		fab_g_shake, fab_g_bound, fab_g_fall, fab_g_end,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	fab_g_jmp[pl_w->routine_3]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fab_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void russian_init( PLAYER_WORK *pl_w );
static void russian_jump( PLAYER_WORK *pl_w );
static void russian_air( PLAYER_WORK *pl_w );
static void russian_slam( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A jump that catches the opponent in the air			*/
/*==============================================================*/
static void sp_russian( PLAYER_WORK *pl_w )
{
	static void ( *russian_jmp[])( PLAYER_WORK *pl_w ) = {
		russian_init, russian_jump, russian_air, russian_slam,
	} ;

	russian_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void russian_init( PLAYER_WORK *pl_w )
{
	static Sint32 russian_spd[][2][2] = {
		{ { 0x00040000, -0x00001000 }, { 0x00080000, -0x00001000 } },
		{ { 0x00010000, -0x00001000 }, { 0x00030000, -0x00001000 } },
	} ;
	static Sint16 russian_time[] = { 0x10, 0x20 } ;

	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_gauge_add( pl_w, 12 ) ;
	pl_w->timer0 = russian_time[pl_w->atck_level] ;
	pl_w->spd.x = russian_spd[pl_w->armor_flag >> 7][pl_w->atck_level][0] * 1.6666666f / 65536.0f ;
	pl_w->acc.x = russian_spd[pl_w->armor_flag >> 7][pl_w->atck_level][1] * 1.6666666f / 65536.0f ;
	pl_w->spd.y = DOT_Y( 4.5f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Catch, 47 ) ;
}

/*--------------------------------------------------------------*/
static void russian_jump( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_air_set( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 48 ) ;
	}
}

/*==============================================================*/
/*			Turns both players round when it catches			*/
/*==============================================================*/
static void russian_air( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( -- pl_w->timer0 == 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->timer0 < 8 ){
		if( ( em = pl_catch_ck( pl_w ) ) != 0 ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->catch_no = 0xcb ;
			scr_excep_set( pl_w, 5, 5 ) ;
			ofs.x = -DOT_X( 80.0f ) ;
			ofs.y = DOT_Y( 104.0f ) ;
			nage_hit_set( pl_w, &ofs ) ;
			pl_w->disp_dir ^= 1 ;
			pl_w->pl_dir = pl_w->disp_dir ;
			em->disp_dir ^= 1 ;
			em->pl_dir = em->disp_dir ;
			pl_char_set( pl_w, Char_Catch, 49 ) ;
			pl_sp_catch_set( pl_w, em ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void russian_slam( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		if( pl_w->team_type == 3 ){
			em->atck_no = pl01_at_russian_h ;
			pl_w->atck_no = pl01_at_russian_h ;
		} else {
			em->atck_no = pl_w->atck_level + 59 ;
			pl_w->atck_no = pl_w->atck_level + 59 ;
		}
	}
}

static void team_init( PLAYER_WORK *pl_w );
static void team_move( PLAYER_WORK *pl_w );
static void team_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A short charge that hits twice						*/
/*==============================================================*/
static void sp_team( PLAYER_WORK *pl_w )
{
	static void ( *team_jmp[])( PLAYER_WORK *pl_w ) = {
		team_init, team_move, team_end,
	} ;

	team_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void team_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl01_at_team_1 ;
	HIT_RESET( pl_w ) ;
	sp_start( pl_w ) ;
	pl_w->spd.x = DOT_X( 2.5f ) ;
	pl_w->acc.x = 0.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_char_set( pl_w, Char_Special, 8 ) ;
}

/*--------------------------------------------------------------*/
static void team_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( HI_BYTE( pl_w->cg_rival ) ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 72.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		return ;
	}
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl01_at_team_1 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl01_at_team_2 ;
		HIT_RESET( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Change, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void team_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A pose that stands still							*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void uab_init( PLAYER_WORK *pl_w );
static void uab_start( PLAYER_WORK *pl_w );
static void uab_run( PLAYER_WORK *pl_w );
static void uab_grab( PLAYER_WORK *pl_w );
static void uab_stop( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Mecha Zangief's hyper throw: the same slams, and	*/
/*			a fall that shakes the floor						*/
/*==============================================================*/
static void sp_uab( PLAYER_WORK *pl_w )
{
	static void ( *uab_jmp[])( PLAYER_WORK *pl_w ) = {
		uab_init, uab_start, uab_run, uab_grab, uab_stop,
	} ;

	uab_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void uab_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 53 ) ;
	armor_set( pl_w, 10, 60, 60 ) ;
}

/*--------------------------------------------------------------*/
static void uab_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->armor_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 72.0f ) ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
		pl_char_set( pl_w, Char_Catch, 55 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_run( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->armor_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 4 ;
		pl_char_set( pl_w, Char_Catch, 54 ) ;
		return ;
	}
	if( ( em = pl_catch_ck( pl_w ) ) != 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_w->catch_no = 0xcd ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 29 ) ;
		pl_sp_catch_set( pl_w, em ) ;
	}
}

/*==============================================================*/
static void uab_g_hold( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_jump( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 5.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 12.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 57 ) ;
	}
}

/*==============================================================*/
/*			Drifts back once it starts to fall					*/
/*==============================================================*/
static void uab_g_top( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_3 ++ ;
		pl_w->acc.x = 7.5f ;
		if( !pl_w->pl_dir ) pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_land( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 20 ) ;
		em_voice( pl_w ) ;
		em = pl_w->catch_w ;
		em->atck_no = pl01_at_uab_1 ;
		pl_w->atck_no = pl01_at_uab_1 ;
		ofs.x = em->pos.x ;
		ofs.y = pl_w->ground_y ;
		burst_pos_set( &ofs, 1 ) ;
		pl_se_req( pl_w, 73 ) ;
		catch_damage( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_turn( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		pl_char_set( pl_w, Char_Catch, 58 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_lift( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		if( pl_w->disp_dir ) pl_w->pos.x += 160.0f ;
		else pl_w->pos.x -= 160.0f ;
		pl_char_set( pl_w, Char_Catch, 59 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( pl_w ) ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = pl01_at_uab_2 ;
		em->atck_no = pl01_at_uab_2 ;
		ofs.x = em->pos.x ;
		ofs.y = pl_w->ground_y ;
		burst_pos_set( &ofs, 1 ) ;
		pl_se_req( pl_w, 73 ) ;
		catch_damage( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_jump2( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 5.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 13.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.625f ) ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void uab_g_top2( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_3 ++ ;
		pl_w->acc.x = 7.5f ;
		if( !pl_w->pl_dir ) pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_land2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		pl_voice_req( pl_w, 21 ) ;
		em_voice( pl_w ) ;
		em = pl_w->catch_w ;
		em->atck_no = pl01_at_uab_3 ;
		pl_w->atck_no = pl01_at_uab_3 ;
		ofs.x = em->pos.x ;
		ofs.y = pl_w->ground_y ;
		burst_pos_set( &ofs, 1 ) ;
		pl_se_req( pl_w, 73 ) ;
		catch_damage( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 56 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_turn2( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 60 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( pl_w->disp_dir ) pl_w->pos.x += 160.0f ;
		else pl_w->pos.x -= 160.0f ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_up( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 20.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( pl_w, Char_Catch, 43 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->atck_no = pl01_at_uab_4 ;
		em->atck_no = pl01_at_uab_4 ;
		em->catch_type = 17 ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_wait( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void uab_g_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	em = pl_w->catch_w ;
	if( em->pos.y - pl_w->pos.y + -DOT_Y( 8.0f ) < -DOT_Y( 32.0f ) ){
		pl_w->routine_3 ++ ;
		pl_w->catch_no = 0xcd ;
		ofs.x = -DOT_X( 88.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_voice_req( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Catch, 29 ) ;
		pl_ef_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_air_hold( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->mash = 0 ;
		pl_char_set( pl_w, Char_Catch, 44 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_dive( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = -1.2053571f ;
		pl_char_set( pl_w, Char_Catch, 45 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_drop( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( lever_gacha_ck( pl_w ) ){
		if( pl_w->cpu_flag ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				if( pl_w->cg_ctr != 1 ) pl_w->cg_ctr -- ;
			}
		} else {
			pl_w->cg_ctr = 1 ;
		}
		lw->mash ++ ;
		if( lw->mash > 16 ) lw->mash = 16 ;
	}
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		em_voice( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 46 ) ;
		return ;
	}
	if( lw->mash > 5 ){
		if( !pl_w->armor_flag ) ef_color_set( pl_w, 0, pl_color_tbl[pl_w->pl_color] + uab_flash_color[lw->mash], 1 ) ;
		else ef_color_set( pl_w, 0, mecha_color_tbl[pl_w->pl_color] + uab_flash_color[lw->mash], 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_shake( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		ofs.x = em->pos.x ;
		ofs.y = pl_w->ground_y ;
		burst_pos_set( &ofs, 1 ) ;
		burst_set( pl_w, &ofs, 4, 126 ) ;
		pl_se_req( pl_w, 74 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_bound( PLAYER_WORK *pl_w )
{
	static Uint8 uab_hit[][2] = {
		{ 2, 0x54 }, { 2, 0x54 }, { 2, 0x54 }, { 2, 0x54 }, { 2, 0x54 }, { 2, 0x54 },
		{ 2, 0x55 }, { 2, 0x55 }, { 2, 0x55 }, { 2, 0x55 }, { 2, 0x55 },
		{ 2, 0x56 }, { 2, 0x56 }, { 2, 0x56 }, { 2, 0x56 }, { 2, 0x56 },
		{ 9, 0x57 },
	} ;
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = -0.104166664f ;
		pl_w->spd.y = DOT_Y( 9.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Catch, 33 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->catch_type = uab_hit[lw->mash][0] ;
		em->atck_no = uab_hit[lw->mash][1] ;
		pl_w->atck_no = uab_hit[lw->mash][1] ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->atck_pow = lw->mash / 2 + 32 ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_g_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->atck_pow = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void uab_grab( PLAYER_WORK *pl_w )
{
	static void ( *uab_g_jmp[])( PLAYER_WORK *pl_w ) = {
		uab_g_hold, uab_g_jump, uab_g_top, uab_g_land, uab_g_turn,
		uab_g_lift, uab_g_jump2, uab_g_top2, uab_g_land2, uab_g_turn2,
		uab_g_up, uab_g_wait, uab_g_catch, uab_g_air_hold, uab_g_dive,
		uab_g_drop, uab_g_shake, uab_g_bound, uab_g_fall, uab_g_end,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	uab_g_jmp[pl_w->routine_3]( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void uab_stop( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void iron_init( PLAYER_WORK *pl_w );
static void iron_move( PLAYER_WORK *pl_w );
static void iron_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Turns into Mecha Zangief, or back					*/
/*==============================================================*/
static void sp_iron( PLAYER_WORK *pl_w )
{
	static void ( *iron_jmp[])( PLAYER_WORK *pl_w ) = {
		iron_init, iron_move, iron_end,
	} ;

	iron_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void iron_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 14 ) ;
}

/*--------------------------------------------------------------*/
static void iron_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( !pl_w->armor_flag ) pl_w->armor_flag = ARMOR_HEAVY ;
		else pl_w->armor_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
		ofs.x = -DOT_X( 64.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void iron_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void vodka_init( PLAYER_WORK *pl_w );
static void vodka_move( PLAYER_WORK *pl_w );
static void vodka_wait( PLAYER_WORK *pl_w );
static void vodka_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Mecha Zangief's breath								*/
/*==============================================================*/
static void sp_vodka( PLAYER_WORK *pl_w )
{
	static void ( *vodka_jmp[])( PLAYER_WORK *pl_w ) = {
		vodka_init, vodka_move, vodka_wait, vodka_end,
	} ;

	vodka_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void vodka_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_w->atck_no = pl01_at_vodka ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
}

/*==============================================================*/
/*			The breath: lw->breath_end is set when it is		*/
/*			done												*/
/*==============================================================*/
static void vodka_move( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		lw->breath_end = 0 ;
		if( !shl01_01_set( pl_w ) ) pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void vodka_wait( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;

	if( !lw->breath_end ){
		pl_char_move( pl_w ) ;
	} else {
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vodka_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void blizzard_init( PLAYER_WORK *pl_w );
static void blizzard_start( PLAYER_WORK *pl_w );
static void blizzard_rise( PLAYER_WORK *pl_w );
static void blizzard_fall( PLAYER_WORK *pl_w );
static void blizzard_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Mecha Zangief's hyper combo: a rising lariat		*/
/*==============================================================*/
static void sp_blizzard( PLAYER_WORK *pl_w )
{
	static void ( *blizzard_jmp[])( PLAYER_WORK *pl_w ) = {
		blizzard_init, blizzard_start, blizzard_rise, blizzard_fall, blizzard_end,
	} ;

	blizzard_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void blizzard_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	sp_start( pl_w ) ;
	pl_w->atck_no = pl01_at_blizzard ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
}

/*--------------------------------------------------------------*/
static void blizzard_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl01_at_blizzard ;
		HIT_RESET( pl_w ) ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 12 ) ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 72.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			It hits again on each of the animation's marks		*/
/*==============================================================*/
static void blizzard_rise( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->cg_flag1 ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.y = -DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( pl_w, Char_Special, 13 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->atck_no = pl_w->cg_type ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void blizzard_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void blizzard_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback: a flash on the mark					*/
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
	pl_w->atck_no = pl01_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 16 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		ofs.x = DOT_X( 40.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_flag1 ){
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A jumping kick that the partner comes in with		*/
/****************************************************************/
void pl01_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = DOT_X( 8.5f ) ;
	pl_w->acc.x = DOT_X( 0.5f ) ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl01_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 6 ) ;
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
		pl_char_set( pl_w, Char_Extra, 0 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			The throws: an air throw when jumping, a			*/
/*			crouching kick throw, and the standing ones			*/
/****************************************************************/
PLAYER_WORK *pl01_catch_ck( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	switch( pl_w->pl_stance ){
	case STANCE_AIR :
		if( !( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
		if( pl_w->atck_pk != 0 ) return 0 ;
		if( !pl_w->atck_level ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 8 ;
			lw->lever = pl_w->atck_lever ;
			return em ;
		}
	case STANCE_CROUCH :
		if( !( pl_w->atck_lever & KEY_FORWARD ) ) return 0 ;
		if( !pl_w->atck_pk ) return 0 ;
		if( !pl_w->atck_level ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			lw->lever = pl_w->atck_lever ;
			return em ;
		}
	default :
		if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
		if( pl_w->atck_level ){
			if( !pl_w->atck_pk ){
				if( ( em = pl_catch_ck( pl_w ) ) ){
					pl_w->catch_no = 0 ;
					lw->lever = pl_w->atck_lever ;
					return em ;
				}
			} else {
				if( ( em = pl_catch_ck( pl_w ) ) ){
					pl_w->catch_no = 1 ;
					lw->lever = pl_w->atck_lever ;
					return em ;
				}
			}
		}
		break ;
	}
	return 0 ;
}

/*==============================================================*/
/*			Each throw's start: where both players stand,		*/
/*			and the animation									*/
/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( lw->lever & 0x400 ){
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		em = pl_w->catch_w ;
		em->disp_dir = em->disp_dir ^ 1 ;
		em->pl_dir = em->disp_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_w->routine_3 = 0 ;
	pl_w->routine_2 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 88.0f ) ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	pl_w->routine_3 = 0 ;
	pl_w->routine_2 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -160.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void crouch_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	pl_w->routine_3 = 0 ;
	pl_w->routine_2 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 104.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
/*			The running grab: the opponent can be shaken for	*/
/*			a while												*/
/*==============================================================*/
static void dash_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->routine_3 = 0 ;
	pl_w->routine_2 = 0 ;
	pl_w->timer0 = 60 ;
	pl_w->timer1 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -160.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	PL01_LOCAL_W *lw = ( PL01_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( lw->lever & 0x800 ){
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		em = pl_w->catch_w ;
		em->disp_dir = em->disp_dir ^ 1 ;
		em->pl_dir = em->disp_dir ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->hit_stop = 10 ;
	pl_w->routine_3 = 0 ;
	pl_w->routine_2 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 80.0f ) ;
	ofs.y = DOT_Y( 104.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

/*==============================================================*/
/*			The command throw (catch_com_ck)					*/
/*==============================================================*/
static void com_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -120.0f ;
	ofs.y = DOT_Y( 48.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/****************************************************************/
/*			Start the throw that pl01_catch_ck chose			*/
/****************************************************************/
void pl01_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, crouch_k_set, nml_p_set,
		nml_p_set, nml_p_set, nml_p_set, dash_set,
		jump_p_set, com_p_set, nml_p_set, nml_p_set,
		nml_p_set, nml_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 2 ;
		em->atck_no = pl01_at_throw_p ;
		pl_w->atck_no = pl01_at_throw_p ;
	}
}

/*==============================================================*/
/*			The kick throw: hold the opponent, which can be		*/
/*			mashed out of, then throw							*/
/*==============================================================*/
static void nml_k_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 64.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		pl_se_req( pl_w, 12 ) ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl01_at_throw_k ;
		pl_w->atck_no = pl01_at_throw_k ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 50 ) ;
		return ;
	}
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 5 ;
	if( !hold_renda_ck( pl_w->catch_w ) ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl01_at_throw_k ;
			pl_w->atck_no = pl01_at_throw_k ;
			catch_damage( pl_w ) ;
			ofs.x = -DOT_X( 64.0f ) ;
			ofs.y = DOT_Y( 96.0f ) ;
			catch_hit_mark_set( pl_w, &ofs ) ;
			pl_se_req( pl_w, 12 ) ;
		}
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Catch, 50 ) ;
}

/*==============================================================*/
static void nml_k_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	hold_renda_set( pl_w->catch_w, 20 ) ;
	catch_renda_set( pl_w, 3 ) ;
	pl_w->timer0 = 64 ;
	nml_k_hold( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_init, nml_k_hold, nml_k_throw,
	} ;

	nml_k_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			The crouching kick throw works the same way			*/
/*==============================================================*/
static void crouch_k_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 112.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		pl_voice_req( pl_w, 32 ) ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl01_at_crouch_throw_k ;
		pl_w->atck_no = pl01_at_crouch_throw_k ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_k_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Catch, 51 ) ;
		return ;
	}
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 5 ;
	if( !hold_renda_ck( pl_w->catch_w ) ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl01_at_crouch_throw_k ;
			pl_w->atck_no = pl01_at_crouch_throw_k ;
			catch_damage( pl_w ) ;
			ofs.x = -DOT_X( 112.0f ) ;
			ofs.y = DOT_Y( 64.0f ) ;
			catch_hit_mark_set( pl_w, &ofs ) ;
			pl_voice_req( pl_w, 32 ) ;
		}
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Catch, 51 ) ;
}

/*==============================================================*/
static void crouch_k_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	hold_renda_set( pl_w->catch_w, 20 ) ;
	catch_renda_set( pl_w, 3 ) ;
	pl_w->timer0 = 64 ;
	crouch_k_hold( pl_w ) ;
}

/*==============================================================*/
static void crouch_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *crouch_k_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_k_init, crouch_k_hold, crouch_k_throw,
	} ;

	crouch_k_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			The running grab: a punch (turning with the			*/
/*			stick) ends it with a blow, a kick goes into the	*/
/*			kick throw. The computer picks at random.			*/
/*==============================================================*/
static void dash_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int next ;

	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 7 ;
		em->atck_no = pl01_at_dash_throw ;
		pl_w->atck_no = pl01_at_dash_throw ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cpu_flag ){
		if( !sw_renda_ck( pl_w ) ) return ;
		if( pl_w->timer1 ++ > 10 ) return ;
		if( nlRand() & 1 ) next = 0 ;
		else next = 1 ;
	} else {
		if( !( pl_w->pl_sw_new & ( KEY_HP | KEY_HK ) ) ) return ;
		if( pl_w->pl_sw_new & KEY_HP ){
			next = 0 ;
			if( pl_w->pl_sw_new & KEY_BACK ){
				pl_w->disp_dir ^= 1 ;
				pl_w->pl_dir = pl_w->disp_dir ;
				em = pl_w->catch_w ;
				em->disp_dir ^= 1 ;
				em->pl_dir = em->disp_dir ;
			}
		} else {
			next = 1 ;
		}
	}
	switch( next ){
	case 0 :
		pl_w->routine_2 = 1 ;
		pl_char_set( pl_w, Char_Catch, 52 ) ;
		break ;
	case 1 :
		pl_w->routine_2 = 2 ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Catch, 7 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->catch_type = 2 ;
		em->atck_no = pl01_at_throw_p ;
		pl_w->atck_no = pl01_at_throw_p ;
	}
}

/*==============================================================*/
static void dash_catch( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_hold, dash_end, nml_k_catch,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		SCR_EXCEP_SET ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->atck_no = pl01_at_air_throw ;
		pl_w->atck_no = pl01_at_air_throw ;
	}
}

/*==============================================================*/
static void com_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		if( !pl_w->cpu_flag ){
			if( pl_w->pl_sw_new & KEY_UP ){
				pl_w->jump_dir = -1 ;
				pl_act_set( pl_w, N_SJUMP ) ;
			} else {
				pl_stand_ret( pl_w ) ;
			}
		} else {
			pl_stand_ret( pl_w ) ;
		}
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 5 ) ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		hit_mark_set( pl_w, &ofs, 7 ) ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->atck_no = pl01_at_com_throw ;
		pl_w->atck_no = pl01_at_com_throw ;
	}
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl01_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, crouch_k_catch, nml_p_catch,
		nml_p_catch, nml_p_catch, nml_p_catch, dash_catch,
		jump_p_catch, com_p_catch, nml_p_catch, nml_p_catch,
		nml_p_catch, nml_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void hold( PLAYER_WORK *pl_w );
static void hold_ck( PLAYER_WORK *pl_w );
static void hold_buster_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The thrown player, while it is held					*/
/****************************************************************/
void pl01_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold_ck, hold_ck, hold_ck,
		hold_ck, hold, hold_ck, hold_ck,
		hold, hold, hold_buster_ck, hold,
		hold, hold_buster_ck,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
/*			Only once the animation has marked it				*/
/*==============================================================*/
static void hold_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( HI_BYTE( em->cg_rival ) ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_buster_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( HI_BYTE( em->cg_rival ) ) catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl01_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl01_sp_team ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl01_sp_team ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl01_sp_team ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl01_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl01_sp_team ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl01_sp_team ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl01_sp_team ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl01_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl01_sp_lariat ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl01_sp_powerbomb ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl01_sp_russian ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl01_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl01_sp_lariat ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl01_sp_powerbomb ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl01_sp_russian ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Whether the thrown player is in one of the			*/
/*			attacks that can be held							*/
/****************************************************************/
int pl01_catch_atck_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	int atck ;

	if( pl_w->pl_type == PL_ZANGIEF ){
		atck = em->atck_no ;
		if( atck == 37 || atck == 38 || atck == 61 || atck == 62
		 || atck == 65 || atck == 66 || atck == 67 || atck == 68
		 || atck == 76 || atck == 77 || atck == 78 || atck == 79
		 || atck == 84 || atck == 85 || atck == 86 || atck == 87 ) return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The thrown player's cry								*/
/*==============================================================*/
static void em_voice( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_se_req( em, 49 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl01.c" End of File	*/
