/* Wolverine (character 07): command checks, normal attacks, appearance, win poses, special moves
   and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl07_def.h"

void pl07_main( PLAYER_WORK *pl_w );
void pl07_com_ck( PLAYER_WORK *pl_w );
void pl07_atck_set( PLAYER_WORK *pl_w );
void pl07_atck( PLAYER_WORK *pl_w );
void pl07_dash( PLAYER_WORK *pl_w );
void pl07_noki( PLAYER_WORK *pl_w );
void pl07_appear( PLAYER_WORK *pl_w );
void pl07_win( PLAYER_WORK *pl_w );
void pl07_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl07_catch_ck( PLAYER_WORK *pl_w );
void pl07_catch_set( PLAYER_WORK *pl_w );
void pl07_catch( PLAYER_WORK *pl_w );
void pl07_hold( PLAYER_WORK *pl_w );
int pl07_delay_ck( PLAYER_WORK *pl_w );
void pl07_t_kick( PLAYER_WORK *pl_w );
void pl07_upper( PLAYER_WORK *pl_w );
void pl07_sp_unison( PLAYER_WORK *pl_w );
void pl07_sp_gattai( PLAYER_WORK *pl_w );
void pl07_sp_assist( PLAYER_WORK *pl_w );
void pl07_sp_zero( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_tornado[] = { 3, COM_GROUND, pl07_sp_tornado, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Tornado Claw */
static Uint16 com_barrage[] = { 3, COM_GROUND, pl07_sp_barrage, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Berserker Barrage */
static Uint16 com_slash[] = { 3, COM_GROUND, pl07_sp_slash, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Berserker Slash */
static Uint16 com_weapon_x[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl07_sp_weapon_x, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Weapon X */
static Uint16 com_barrage_x[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl07_sp_barrage_x, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Berserker Barrage X */
static Uint16 com_fatal[] = { 3, COM_HYPER | COM_ALL_SW, pl07_sp_fatal, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Fatal Claw */
static Uint16 com_charge[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl07_sp_charge, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Berserker Charge */

/* each attack's table */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;

static Sint32 tornado_spd_tbl0[8] = {
	-0x40000, 0x4000, 0x80000, 0x3000, -0xc0000, 0x4000, 0x80000, 0x3000,
} ;
static Sint32 tornado_spd_tbl1[8] = {
	0, 0x8000, 0x58000, -0x8000, 0, 0x20000, 0xd8000, -0x10000,
} ;
static Sint32 *tornado_spd_tbl[2] = { tornado_spd_tbl0, tornado_spd_tbl1 } ;

/* the claw dash's direction for each stick position, on the ground and in the air */
static Uint8 drill_stick_tbl[32] = {
	0, 3, 7, 0, 0, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0,
	0, 3, 7, 0, 5, 4, 6, 0, 1, 2, 8, 0, 0, 0, 0, 0,
} ;
static Uint8 drill_next_tbl[16] = { 0, 3, 7, 0, 0, 0, 0, 0, 1, 2, 8, 0, 0, 0, 0, 0 } ;
static Uint8 drill_air_next_tbl[32] = {
	0, 3, 0, 0, 5, 4, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0,
	0, 0, 7, 0, 5, 0, 6, 0, 1, 0, 8, 0, 0, 0, 0, 0,
} ;
static Uint8 drill_dir_tbl[8] = { 0, 1, 1, 1, 0, 0, 0, 0 } ;
static Sint32 slash_spd_tbl[2] = { -0x140000, -0x340000 } ;
static Sint32 fatal_spd_tbl[4] = { -0x90000, 0xa0000, -0x30000, 0x30000 } ;
static Sint16 unk_tbl7[4] = { -72, -52, -80, -16 } ;

static void ( *pl07_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl07_main,
	pl07_com_ck,
	pl07_atck_set,
	pl07_atck,
	pl07_dash,
	pl07_noki,
	pl07_appear,
	pl07_win,
	pl07_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl07_catch_ck,	/* returns a value */
	pl07_catch_set,
	pl07_catch,
	pl07_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl07_delay_ck,
	pl07_t_kick,
	pl07_upper,
	pl07_sp_unison,
	pl07_sp_gattai,
	pl07_sp_assist,
	pl07_sp_zero,
} ;

static Uint8 tornado_ck( PLAYER_WORK *pl_w );
static Uint8 drill_ck( PLAYER_WORK *pl_w );
static Uint8 barrage_ck( PLAYER_WORK *pl_w );
static Uint8 slash_ck( PLAYER_WORK *pl_w );
static Uint8 weapon_x_ck( PLAYER_WORK *pl_w );
static Uint8 barrage_x_ck( PLAYER_WORK *pl_w );
static Uint8 fatal_ck( PLAYER_WORK *pl_w );
static Uint8 charge_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int weapon_x_delay_ck( PLAYER_WORK *pl_w );
static int barrage_x_delay_ck( PLAYER_WORK *pl_w );
static int fatal_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl07_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl07_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl07_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( weapon_x_ck( pl_w ) ) return ;
	if( barrage_x_ck( pl_w ) ) return ;
	if( fatal_ck( pl_w ) ) return ;
	if( tornado_ck( pl_w ) ) return ;
	if( barrage_ck( pl_w ) ) return ;
	if( drill_ck( pl_w ) ) return ;
	if( charge_ck( pl_w ) ) return ;
	if( slash_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 tornado_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tornado, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_tornado ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A dash in one of eight directions, chosen by the	*/
/*			stick												*/
/*==============================================================*/
static Uint8 drill_ck( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;
	int dir ;

	if( lw->dash_ng_flag ) return 0 ;
	if( !sw_now_ck( pl_w, 0x0140, &sw, 1 ) ) return 0 ;
	if( !sp_ok_ck( pl_w ) ) return 0 ;
	if( pl_w->cpu_flag ){
		sw = pl_w->atck_lever ;
		if( pl_w->pl_dir ){
			if( sw & 0x0c00 ) sw ^= 0x0c00 ;
		}
	} else sw = pl_w->pl_new_lvbt ;
	sw = ( sw & 0x3c00 ) >> 10 ;
	if( !sw ) sw = 2 - pl_w->pl_dir ;
	if( pl_w->pl_stance == STANCE_AIR ) sw += 16 ;
	dir = drill_stick_tbl[sw] ;
	if( !dir ) return 0 ;
	if( pl_w->pl_stance != STANCE_AIR ){
		if( sw & ( 1 << pl_w->pl_dir ) ) return 0 ;
	}
	pl_w->dir = dir - 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->atck_level = 1 ;
	pl_w->pl_sp_type = pl07_sp_drill ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 barrage_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_barrage, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_barrage ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 slash_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_slash, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_slash ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 weapon_x_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_weapon_x, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_weapon_x ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 barrage_x_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_barrage_x, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_barrage_x ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground, or in a jump with no air special		*/
/*			done yet, or in a super jump						*/
/*==============================================================*/
static Uint8 fatal_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fatal, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag && pl_w->jump_com_cnt ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_fatal ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 charge_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_charge, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->speed_up_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_charge ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_snapback ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl07_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl07_delay_ck( PLAYER_WORK *pl_w )
{
	if( weapon_x_delay_ck( pl_w ) || barrage_x_delay_ck( pl_w ) || fatal_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int weapon_x_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_weapon_x, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int barrage_x_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_barrage_x, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int fatal_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fatal, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

/****************************************************************/
void pl07_main( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->speed_up_flag ) pl_w->atck_pow = 24 ;
	else pl_w->atck_pow = 0 ;
	if( lw->dash_ng_flag && pl_w->pl_stance != STANCE_AIR ) lw->dash_ng_flag = 0 ;
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
void pl07_atck_set( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 0 ){
		if( pl_w->pl_stance == STANCE_STAND ) stand_punch( pl_w ) ;
		else crouch_punch( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_STAND ) stand_kick( pl_w ) ;
		else crouch_kick( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	int no, atck, voice ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 0 ;
		no = 0 ;
		voice = 30 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 1:
		atck = 1 ;
		no = 1 ;
		voice = 31 ;
		pl_w->type = 0 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 2:
		atck = 2 ;
		no = 2 ;
		voice = 32 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = no ;
		break ;
	}
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_voice_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Stand_P, no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	int no, atck, voice ;

	pl_w->routine_2 = 0 ;
	switch( pl_w->nml_no ){
	case 0:
		atck = 6 ;
		no = 0 ;
		voice = 30 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 1:
		atck = 7 ;
		no = 1 ;
		voice = 31 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 2:
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_2 = 1 ;
			atck = 18 ;
			no = 5 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		} else {
			atck = 8 ;
			no = 2 ;
			pl_w->spd.x = -5.0f ;
			pl_w->acc.x = 0.0f ;
			voice = 32 ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		}
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Crouch_P, no ) ;
	if( !pl_w->routine_2 ) pl_voice_req( pl_w, voice ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	int no, atck, voice ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 3 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 1:
		atck = 4 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 2:
		atck = 5 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = no ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Stand_K, no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	int no, atck, voice ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 9 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 1:
		atck = 10 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 2:
		atck = 11 ;
		no = 2 ;
		voice = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = no ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Crouch_K, no ) ;
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) ){
		jump_atck( pl_w ) ;
		return ;
	}
	if( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk ) jump_punch( pl_w ) ;
	else jump_kick( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	int no, atck, voice ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 12 ;
		no = 0 ;
		voice = 30 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 1:
		atck = 13 ;
		no = 1 ;
		voice = 31 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 2:
		atck = 14 ;
		no = 2 ;
		voice = 32 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = no ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_voice_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Jump_P, no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -- ;
}

/*==============================================================*/
/*			The heavy kick, held high in the air, starts the	*/
/*			dive												*/
/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	int no, atck, voice ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 15 ;
		no = 0 ;
		voice = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 1:
		atck = 16 ;
		no = 1 ;
		voice = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = no ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_DOWN ){
			if( pl_w->pos.y - pl_w->ground_y > 240.0f ){
				atck = 19 ;
				no = 5 ;
				voice = 22 ;
				pl_w->routine_2 = 1 ;
				pl_w->jump_atk_cnt &= JATK_P_MASK ;
				pl_w->sjump_flag = 0 ;
				pl_w->chain_tbl = hk_tbl ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				break ;
			}
		}
		atck = 17 ;
		no = 2 ;
		voice = 22 ;
		pl_w->routine_2 = 0 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = no ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, voice ) ;
	pl_char_set( pl_w, Char_Jump_K, no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 16 ;
}

static void atck_nml_dash( PLAYER_WORK *pl_w );
static void atck_nml( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void atck_sjump( PLAYER_WORK *pl_w );
static void atck_jump( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void dive_nml( PLAYER_WORK *pl_w );
static void dive_wait( PLAYER_WORK *pl_w );
static void dive_fall( PLAYER_WORK *pl_w );
static void dive_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl07_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		atck_nml, atck_jump, atck_sjump, atck_nml_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void atck_nml_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	atck_nml( pl_w ) ;
}

/*==============================================================*/
static void atck_nml( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->atck_pk == 0 ){
		if( pl_w->pl_stance == STANCE_STAND ) stand_punch_move( pl_w ) ;
		else crouch_punch_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_STAND ) stand_kick_move( pl_w ) ;
		else crouch_kick_move( pl_w ) ;
	}
}

/*==============================================================*/
/*			The medium punch chains into a second hit with		*/
/*			the button held										*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		if( !pl_w->cg_type ) break ;
		if( pl_turn_ck( pl_w ) ) break ;
		if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x0200 ){
			pl_w->pl_stop_now = 0 ;
			pl_w->atck_no = pl07_at_stand_mp_2 ;
			HIT_RESET( pl_w ) ;
			pl_voice_req( pl_w, 31 ) ;
			pl_atck_gauge_add( pl_w ) ;
			pl_char_set( pl_w, Char_Stand_P, 4 ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls07_00_set( pl_w, 1 ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*			The heavy one slides forward, or with the stick		*/
/*			toward, steps through								*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	Float ofs ;

	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls07_00_set( pl_w, 2 ) ;
		}
		break ;
	case 2:
		if( !pl_w->routine_2 ){
			if( !( pl_w->cg_type & 2 ) ){
				pl_w->pos.x += pl_w->spd.x ;
				pl_w->spd.x += pl_w->acc.x ;
			}
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			else if( pl_w->cg_type & 1 ){
				pl_w->spd.x = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_voice_req( pl_w, 32 ) ;
				pls07_00_set( pl_w, 3 ) ;
			}
		} else {
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			else {
				if( pl_w->cg_type & 1 ){
					pl_w->cg_type ^= 1 ;
					pl_w->spd.x = -DOT_X( 4.75f ) ;
					pl_w->acc.x = 0.15625f ;
					if( pl_w->pl_dir ){
						pl_w->spd.x = -pl_w->spd.x ;
						pl_w->acc.x = -pl_w->acc.x ;
					}
					pl_se_req( pl_w, 41 ) ;
				}
				pl_w->pos.x += pl_w->spd.x ;
				pl_w->spd.x += pl_w->acc.x ;
				if( pl_w->cg_type & 2 ){
					pl_w->cg_type ^= 2 ;
					pl_w->spd.x = 0.0f ;
					pl_w->acc.x = 0.0f ;
					ofs = -DOT_X( 32.0f ) ;
					if( pl_w->pl_dir ) ofs = DOT_X( 32.0f ) ;
					pl_w->pos.x += ofs ;
				}
			}
		}
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		if( !pl_w->cg_type ) break ;
		if( pl_turn_ck( pl_w ) ) break ;
		if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x0040 ){
			pl_w->pl_stop_now = 0 ;
			pl_w->atck_no = pl07_at_stand_mk_2 ;
			HIT_RESET( pl_w ) ;
			pl_atck_gauge_add( pl_w ) ;
			pl_se_req( pl_w, SE_SWING_M ) ;
			pl_char_set( pl_w, Char_Stand_K, 4 ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else {
			spd = DOT_X( pl_w->cg_prm ) ;
			if( !pl_w->pl_dir ) spd = -spd ;
			pl_w->pos.x += spd ;
		}
		break ;
	}
}

/*==============================================================*/
static void atck_sjump( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	atck_jump( pl_w ) ;
}

/*==============================================================*/
static void atck_jump( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 0 ) jump_punch_move( pl_w ) ;
	else jump_kick_move( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls07_00_set( pl_w, 4 ) ;
		}
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			break ;
		}
		if( !pl_w->cg_type ) break ;
		if( ( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x0200 ) && pl_w->sjump_flag ){
			pl_w->pl_stop_now = 0 ;
			pl_w->atck_no = pl07_at_sjump_mp_2 ;
			HIT_RESET( pl_w ) ;
			pl_atck_gauge_add( pl_w ) ;
			pl_voice_req( pl_w, 31 ) ;
			pl_char_set( pl_w, Char_Jump_P, 4 ) ;
		}
		break ;
	}
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w ) = {
		dive_nml, dive_wait, dive_fall, dive_land,
	} ;

	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		break ;
	case 2:
		dive_jmp[pl_w->routine_2]( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick with the stick down: a dive to		*/
/*			the ground											*/
/*==============================================================*/
static void dive_nml( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dive_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->spd.y = -DOT_Y( 10.0f ) ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->sjump_flag ) pl_w->spd.y = -DOT_Y( 13.0f ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
}

/*==============================================================*/
/*			Bounces off a hit, or lands							*/
/*==============================================================*/
static void dive_fall( PLAYER_WORK *pl_w )
{
	Uint8 dir ;

	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		pl_w->spd.x = DOT_X( 3.5f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 15.0f ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->jump_dir = -1 ;
		dir = pl_w->pl_dir ;
		pl_jump_ret( pl_w ) ;
		pl_w->pl_dir = dir ;
		pl_w->disp_dir = dir ;
	} else if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dive_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl07_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl07_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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

/****************************************************************/
void pl07_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->type = pl_w->speed_up_flag ;
	pl_w->timer0 = 24 ;
	pl_w->spd.x = -DOT_X( 10.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->type ) pl_w->spd.x = -DOT_X( 16.0f ) ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	dash_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		if( pl_w->type && !pl_w->speed_up_flag ){
			pl_w->type = 0 ;
			pl_w->spd.x = -DOT_X( 10.0f ) ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( --pl_w->timer0 <= 0 ) pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl07_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_jump, noki_land,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( !pl_w->speed_up_flag ){
			pl_w->spd.x = 20.0f ;
			pl_w->acc.x = -DOT_Y( 0.375f ) ;
			pl_w->spd.y = 5.0f ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
		} else {
			pl_w->spd.x = 25.0f ;
			pl_w->acc.x = -DOT_Y( 0.375f ) ;
			pl_w->spd.y = DOT_X( 4.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		noki_land( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_wait( PLAYER_WORK *pl_w );
static void appear_jump( PLAYER_WORK *pl_w );
static void appear_land( PLAYER_WORK *pl_w );
static void appear_jump_init( PLAYER_WORK *pl_w );
static void appear_jump_start( PLAYER_WORK *pl_w );
static void appear_jump_air( PLAYER_WORK *pl_w );

/****************************************************************/
void pl07_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_wait, appear_jump, appear_land,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st < VS_APPEAR ) pl_w->disp_flag = 0 ;
	else {
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		appear_jump( pl_w ) ;
	}
}

/*==============================================================*/
/*			Jumps in from off the screen						*/
/*==============================================================*/
static void appear_jump( PLAYER_WORK *pl_w )
{
	static void ( *appear_jump_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_jump_init, appear_jump_start, appear_jump_air,
	} ;

	appear_jump_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_jump_init( PLAYER_WORK *pl_w )
{
	Float x, spd ;

	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->timer0 = 32 ;
	pl_w->spd.y = DOT_Y( 10.0f ) ;
	pl_w->acc.y = -0.6696428f ;
	x = -DOT_X( 128.0f ) ;
	spd = DOT_X( 2.0f ) ;
	if( pl_w->pl_no ){
		x = DOT_X( 128.0f ) ;
		spd = -DOT_X( 2.0f ) ;
	}
	pl_w->pos.x += x ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	appear_jump_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_jump_start( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		if( pl_land_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->pos.x -= pl_w->spd.x ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Appear, 2 ) ;
			pl_land_ef_set( pl_w ) ;
		}
	} else {
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_jump_air( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pos.x -= pl_w->spd.x ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl07_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	switch( pl_w->type ){
	case 0:
	case 2:
		pl_w->sub_type = Sys->game_timer0 & 1 ;
		pl_char_set( pl_w, Char_Win, pl_w->sub_type ) ;
		break ;
	case 1:
	case 3:
	case 4:
		pl_char_set( pl_w, Char_Win, 2 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_w->act_st == N_WIN && pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else {
		switch( pl_w->type ){
		case 0:
		case 2:
		case 4:
			if( !pl_w->sub_type ){
				pl_char_move( pl_w ) ;
				if( pl_w->cg_type ){
					pl_w->cg_type = 0 ;
					spd = pl_w->cg_prm ;
					if( pl_w->pl_dir ) spd = -spd ;
					pl_w->pos.x += spd ;
				}
			} else {
				if( pl_char_move( pl_w ) >= 0 ){
					if( pl_w->cg_type ){
						pl_w->cg_type = 0 ;
						pls07_00_set( pl_w, 6 ) ;
					}
				} else {
					pl_w->type = 4 ;
					pl_char_set( pl_w, Char_Stand, 0 ) ;
				}
			}
			break ;
		case 1:
		case 3:
			pl_char_move( pl_w ) ;
			break ;
		}
	}
}

static void sp_tornado( PLAYER_WORK *pl_w );
static void sp_drill( PLAYER_WORK *pl_w );
static void sp_barrage( PLAYER_WORK *pl_w );
static void sp_slash( PLAYER_WORK *pl_w );
static void sp_weapon_x( PLAYER_WORK *pl_w );
static void sp_barrage_x( PLAYER_WORK *pl_w );
static void sp_fatal( PLAYER_WORK *pl_w );
static void sp_charge( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl07_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_tornado, sp_drill, sp_barrage, sp_slash, sp_weapon_x, sp_barrage_x, sp_fatal,
		sp_charge, sp_snapback, sp_chouhatsu,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void tornado_init( PLAYER_WORK *pl_w );
static void tornado_move( PLAYER_WORK *pl_w );
static void tornado_fall( PLAYER_WORK *pl_w );
static void tornado_start( PLAYER_WORK *pl_w );
static void tornado_up( PLAYER_WORK *pl_w );
static void tornado_keep( PLAYER_WORK *pl_w );
static void tornado_last( PLAYER_WORK *pl_w );
static void tornado_spd_set( PLAYER_WORK *pl_w, int no );
static void tornado_hit_ck( PLAYER_WORK *pl_w );
static void tornado_sw_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A spinning slash, kept going with the buttons		*/
/*==============================================================*/
static void sp_tornado( PLAYER_WORK *pl_w )
{
	static void ( *tornado_jmp[])( PLAYER_WORK *pl_w ) = {
		tornado_init, tornado_move, tornado_fall,
	} ;

	tornado_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tornado_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->jump_com_cnt = 1 ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->catch_ng_timer = 4 ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = pl_w->atck_level * 2 + 3 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = ( pl_w->atck_level << 1 ) + 76 ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void tornado_move( PLAYER_WORK *pl_w )
{
	static void ( *tornado_move_jmp[])( PLAYER_WORK *pl_w ) = {
		tornado_start, tornado_up, tornado_keep, tornado_last,
	} ;

	tornado_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void tornado_start( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		tornado_spd_set( pl_w, 0 ) ;
		pl_voice_req( pl_w, 31 ) ;
	}
	tornado_hit_ck( pl_w ) ;
	tornado_sw_ck( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void tornado_up( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !( pl_w->spd.x * pl_w->acc.x < 0.0f ) ) RATE_CLR_X( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		tornado_spd_set( pl_w, 1 ) ;
		pl_voice_req( pl_w, 31 ) ;
		if( !pl_w->timer0 ){
			pl_w->routine_3 = 3 ;
			pl_w->atck_no = pl07_at_tornado_end ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 3 ) ;
			return ;
		}
		pl_w->type = 1 ;
		pl_w->timer0 = 0 ;
		pl_w->timer1 -- ;
		pl_char_set( pl_w, Char_Special, 1 ) ;
	} else {
		tornado_hit_ck( pl_w ) ;
		tornado_sw_ck( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tornado_keep( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !( pl_w->spd.x * pl_w->acc.x < 0.0f ) ) RATE_CLR_X( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_voice_req( pl_w, 31 ) ;
		if( pl_w->timer0 ){
			pl_w->timer0 = 0 ;
			if( --pl_w->timer1 > 0 ){
				pl_w->spd.y += DOT_Y( 5.0f ) ;
				pl_char_set( pl_w, Char_Special, ( pl_w->type ^= 1 ) + 1 ) ;
				return ;
			}
		}
		pl_w->routine_3 = 3 ;
		pl_w->atck_no = pl07_at_tornado_end ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	} else {
		tornado_hit_ck( pl_w ) ;
		tornado_sw_ck( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tornado_last( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !( pl_w->spd.x * pl_w->acc.x < 0.0f ) ) RATE_CLR_X( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->hit_flag = HR_GUARD ;
		pl_w->acc.y = -1.2053571f ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void tornado_fall( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_3 ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_3 ++ ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_char_set( pl_w, Char_Special, pl_w->atck_level + 5 ) ;
			pl_land_ef_set( pl_w ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The speeds for each strength, in 16.16 fixed		*/
/*			point												*/
/*==============================================================*/
static void tornado_spd_set( PLAYER_WORK *pl_w, int no )
{
	Sint32 *tbl ;

	tbl = tornado_spd_tbl[no] ;
	tbl += pl_w->atck_level * 4 ;
	pl_w->spd.x = *tbl++ * 1.6666666f / 65536.0f ;
	pl_w->acc.x = *tbl++ * 1.6666666f / 65536.0f ;
	pl_w->spd.y = *tbl++ * 2.142857f / 65536.0f ;
	pl_w->acc.y = *tbl * 2.142857f / 65536.0f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*==============================================================*/
static void tornado_hit_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = ( pl_w->atck_level << 1 ) + pl_w->cg_flag1 + 75 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*==============================================================*/
/*			A button pressed in time keeps the move going		*/
/*==============================================================*/
static void tornado_sw_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( pl_w->cg_prm ){
		if( !pl_w->cpu_flag ){
			sw = pl_w->pl_now_lvbt | pl_w->pl_stop_now ;
			if( sw & KEY_PUNCH ){
				pl_w->timer0 = 1 ;
				pl_w->pl_stop_now = 0 ;
			}
		} else if( !pl_w->team_no ) pl_w->timer0 = 1 ;
	}
}

static void drill_init( PLAYER_WORK *pl_w );
static void drill_move( PLAYER_WORK *pl_w );
static void drill_fall( PLAYER_WORK *pl_w );
static void drill_fall_combo( PLAYER_WORK *pl_w );
static void drill_fly( PLAYER_WORK *pl_w );
static void drill_land( PLAYER_WORK *pl_w );
static void drill_wall( PLAYER_WORK *pl_w );
static void drill_hit_set( PLAYER_WORK *pl_w );
static void drill_spd_set( PLAYER_WORK *pl_w );
static void drill_dir_set( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The claw dash, in one of eight directions			*/
/*==============================================================*/
static void sp_drill( PLAYER_WORK *pl_w )
{
	static void ( *drill_jmp[])( PLAYER_WORK *pl_w ) = {
		drill_init, drill_move, drill_fall, drill_fall_combo,
	} ;

	drill_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void drill_init( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	int no ;

	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->atck_level = 1 ;
		lw->dash_ng_flag = 0 ;
		pl_gauge_add( pl_w, 5 ) ;
		drill_hit_set( pl_w ) ;
		no = 9 ;
		if( pl_w->pl_stance != STANCE_AIR ){
			no = 8 ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_sp_ef_set( pl_w ) ;
		}
		pl_char_set( pl_w, Char_Special, no ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		lw->dash_ng_flag = 1 ;
		pl_air_set( pl_w ) ;
		drill_hit_set( pl_w ) ;
		drill_spd_set( pl_w ) ;
		drill_dir_set( pl_w ) ;
		dm00_01_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void drill_move( PLAYER_WORK *pl_w )
{
	static void ( *drill_move_jmp[])( PLAYER_WORK *pl_w ) = {
		drill_fly, drill_land, drill_wall,
	} ;

	drill_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
/*			In the air: hit, against a wall, landed, or at		*/
/*			the animation's end									*/
/*==============================================================*/
static void drill_fly( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	int wall ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->hit_flag ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.y = DOT_Y( 8.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x /= 8.0f ;
		if( pl_w->hit_flag & HR_GUARD ) pl_w->spd.x /= 8.0f ;
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_dir_set( pl_w ) ;
		if( pl_w->air_combo_cnt != lw->air_combo_old ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Jump, 9 ) ;
		} else {
			pl_char_set( pl_w, Char_Special, 19 ) ;
		}
		return ;
	}
	wall = pl_w->wall_flag ;
	if( wall ){
		if( !( wall & ( 1 << pl_w->pl_dir ) ) && pl_w->dir && pl_w->dir != 4 ){
			pl_w->routine_3 = 2 ;
			pl_char_set( pl_w, Char_Special, 20 ) ;
			return ;
		}
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 = 1 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_dir_set( pl_w ) ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Special, 21 ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->pl_stance = STANCE_AIR ;
		lw->dash_ng_flag = 1 ;
		pl_jump_move( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no + 1 ) ;
	}
}

/*==============================================================*/
/*			Landed: a stick direction and a button chain		*/
/*			another dash										*/
/*==============================================================*/
static void drill_land( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;
	int dir ;

	if( pl_char_move( pl_w ) < 0 ){
		if( lw->atck_flag && ( sw = pl_w->pl_new_lvbt & KEY_LEVER ) ){
			sw >>= 10 ;
			dir = drill_next_tbl[sw] ;
			if( dir ){
				if( !( sw & ( 1 << pl_w->pl_dir ) ) ){
					pl_w->dir = dir - 1 ;
					pl_w->pl_stance = STANCE_AIR ;
					pl_w->routine_3 = 0 ;
					lw->dash_ng_flag = 1 ;
					lw->atck_flag = 0 ;
					drill_hit_set( pl_w ) ;
					drill_spd_set( pl_w ) ;
					drill_dir_set( pl_w ) ;
					pl_sp_ef_set( pl_w ) ;
					return ;
				}
			}
		}
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->sjump_flag = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_stand_ret( pl_w ) ;
	} else if( sw = pl_w->pl_new_lvbt & KEY_ATCK ) lw->atck_flag = 1 ;
}

/*==============================================================*/
/*			Hit a wall: the same, in the air					*/
/*==============================================================*/
static void drill_wall( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;
	int dir ;

	if( pl_char_move( pl_w ) < 0 ){
		if( lw->atck_flag && ( sw = pl_w->pl_new_lvbt & KEY_LEVER ) ){
			sw >>= 10 ;
			if( pl_w->pl_dir ) sw += 16 ;
			dir = drill_air_next_tbl[sw] ;
			if( dir ){
				pl_w->dir = dir - 1 ;
				pl_w->pl_stance = STANCE_AIR ;
				pl_w->routine_3 = 0 ;
				lw->dash_ng_flag = 1 ;
				lw->atck_flag = 0 ;
				drill_hit_set( pl_w ) ;
				drill_spd_set( pl_w ) ;
				drill_dir_set( pl_w ) ;
				return ;
			}
		}
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->jump_dash_cnt = 1 ;
		pl_w->jump_dir = -1 ;
		pl_jump_ret( pl_w ) ;
	} else if( sw = pl_w->pl_new_lvbt & KEY_ATCK ) lw->atck_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void drill_fall( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_set( pl_w ) ;
	} else if( !pl_w->routine_3 ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->pl_stance = STANCE_AIR ;
			lw->dash_ng_flag = 1 ;
			if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) ){
				pl_w->jump_com_cnt = 0 ;
				pl_w->jump_atk_cnt = JATK_ONE ;
				pl_w->jump_dir = -1 ;
				pl_jump_ret( pl_w ) ;
			} else {
				pl_w->routine_3 ++ ;
				pl_w->jump_com_cnt = 1 ;
				pl_w->jump_dash_cnt = 1 ;
				pl_w->sjump_flag = 0 ;
				pl_w->jump_atk_cnt = 0 ;
			}
		}
	} else pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drill_fall_combo( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_set( pl_w ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void drill_hit_set( PLAYER_WORK *pl_w )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;

	pl_w->atck_no = pl_w->dir + 82 ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	lw->air_combo_old = pl_w->air_combo_cnt ;
}

/*==============================================================*/
/*			The dash's speed along its direction, an eighth		*/
/*			of a turn for each step of dir						*/
/*==============================================================*/
static void drill_spd_set( PLAYER_WORK *pl_w )
{
	Sint16 ang ;
	Float spd ;

	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd = 166400.0f ;
	ang = ( ( 40 - pl_w->dir * 4 ) & 31 ) << 11 ;
	pl_w->spd.x = spd * nlCos( ang ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	pl_w->spd.y = spd * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
	spd = 307200.0f ;
	pl_w->acc.x = -( spd * nlCos( ang ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ) / 256.0f ;
	pl_w->acc.y = -( spd * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ) / 256.0f ;
}

/*==============================================================*/
static void drill_dir_set( PLAYER_WORK *pl_w )
{
	int no ;

	pl_w->pl_dir = drill_dir_tbl[pl_w->dir] ;
	pl_w->disp_dir = pl_w->pl_dir ;
	no = pl_w->dir ;
	if( no >= 5 ) no = 8 - no ;
	pl_char_set( pl_w, Char_Special, no * 2 + 10 ) ;
	pl_voice_req( pl_w, 32 ) ;
}

static void barrage_init( PLAYER_WORK *pl_w );
static void barrage_start( PLAYER_WORK *pl_w );
static void barrage_move( PLAYER_WORK *pl_w );
static void barrage_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A flurry of slashes, kept going with the buttons	*/
/*==============================================================*/
static void sp_barrage( PLAYER_WORK *pl_w )
{
	static void ( *barrage_jmp[])( PLAYER_WORK *pl_w ) = {
		barrage_init, barrage_start, barrage_move, barrage_end,
	} ;

	barrage_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void barrage_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->timer0 = pl_w->atck_level * 4 + 4 ;
	pl_w->timer1 = 0 ;
	pl_w->type = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl07_at_barrage ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 22 ) ;
}

/*--------------------------------------------------------------*/
static void barrage_start( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pls07_00_set( pl_w, 5 ) ;
		shl07_00_set( pl_w, 0 ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 25 ) ;
		pl_voice_req( pl_w, 31 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->spd.x = -DOT_X( 6.5f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void barrage_move( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	int no ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->wall_flag ){
		if( pl_w->wall_flag != 1 << pl_w->pl_dir ) goto finish ;
	}
	if( pl_char_move( pl_w ) >= 0 ){
		if( ( Sint8 )pl_w->type <= 0 && pl_w->timer0 > 1 ){ /* read signed */
			if( !pl_w->cpu_flag ){
				sw = pl_w->pl_now_lvbt | pl_w->pl_stop_now ;
				if( sw & KEY_PUNCH ){
					pl_w->type = 0xff ;
					pl_w->pl_stop_now = 0 ;
				}
			} else if( !pl_w->team_no ) pl_w->type = 0xff ;
		}
		return ;
	}
	if( --pl_w->timer0 ){
		if( pl_w->timer1 ^= 1 ){
			if( pl_w->type ) pl_w->type = 0 ;
			else {
				pl_w->type = 1 ;
				pl_w->timer0 = 1 ;
			}
			if( pl_w->timer0 == 1 ) pl_w->type = 1 ;
		}
		shl07_00_set( pl_w, pl_w->timer1 ) ;
		no = pl_w->timer1 * 3 + pl_w->atck_level + 25 ;
		pl_char_set( pl_w, Char_Special, no ) ;
		pl_voice_req( pl_w, 31 ) ;
		return ;
	}
finish:
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 31 ) ;
}

/*--------------------------------------------------------------*/
static void barrage_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void slash_init( PLAYER_WORK *pl_w );
static void slash_move( PLAYER_WORK *pl_w );
static void slash_end( PLAYER_WORK *pl_w );
static void slash_start( PLAYER_WORK *pl_w );
static void slash_dash( PLAYER_WORK *pl_w );
static void slash_stop( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A lunging slash along the ground					*/
/*==============================================================*/
static void sp_slash( PLAYER_WORK *pl_w )
{
	static void ( *slash_jmp[])( PLAYER_WORK *pl_w ) = {
		slash_init, slash_move, slash_end,
	} ;

	slash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void slash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl_w->atck_level + 90 ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 36 ) ;
}

/*--------------------------------------------------------------*/
static void slash_move( PLAYER_WORK *pl_w )
{
	static void ( *slash_move_jmp[])( PLAYER_WORK *pl_w ) = {
		slash_start, slash_dash, slash_stop,
	} ;

	slash_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void slash_start( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 4 ;
		pl_w->sp_ng_timer = 3 ;
		pl_w->spd.x = slash_spd_tbl[pl_w->atck_level] * 1.6666666f / 65536.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Special, 39 ) ;
	}
}

/*--------------------------------------------------------------*/
static void slash_dash( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_level ){
		if( Sys->game_timer0 & 1 ) pl_smoke_set( pl_w ) ;
	}
	pl_w->sp_ng_timer = 3 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->push_ng_timer = 0 ;
		pl_w->acc.x = -pl_w->spd.x / 8.0f ;
		pl_w->spd.x /= 2.0f ;
		pl_char_set( pl_w, Char_Special, 40 ) ;
		pl_voice_req( pl_w, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void slash_stop( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.x != 0.0f ){
		if( Sys->game_timer0 & 1 ) pl_smoke_set( pl_w ) ;
		pl_w->sp_ng_timer = 3 ;
	}
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ){
		pl_w->sp_ng_timer = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void slash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void weapon_x_start( PLAYER_WORK *pl_w );
static void weapon_x_move( PLAYER_WORK *pl_w );
static void weapon_x_end( PLAYER_WORK *pl_w );
static void weapon_x_init( PLAYER_WORK *pl_w );
static void weapon_x_wait( PLAYER_WORK *pl_w );
static void weapon_x_dash_wait( PLAYER_WORK *pl_w );
static void weapon_x_dash( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw );
static void weapon_x_catch( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw );
static void weapon_x_float_init( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw );
static void weapon_x_float( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw );
static void weapon_x_fall( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw );

/*==============================================================*/
/*			A hyper combo: a rush that catches the opponent		*/
/*			and carries them up									*/
/*==============================================================*/
static void sp_weapon_x( PLAYER_WORK *pl_w )
{
	static void ( *weapon_x_jmp[])( PLAYER_WORK *pl_w ) = {
		weapon_x_start, weapon_x_move, weapon_x_end,
	} ;

	weapon_x_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void weapon_x_start( PLAYER_WORK *pl_w )
{
	static void ( *weapon_x_start_jmp[])( PLAYER_WORK *pl_w ) = {
		weapon_x_init, weapon_x_wait, weapon_x_dash_wait,
	} ;

	weapon_x_start_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void weapon_x_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl07_at_weapon_x ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void weapon_x_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 2.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_x_dash_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x = -40.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pls07_00_set( pl_w, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_x_move( PLAYER_WORK *pl_w )
{
	static void ( *weapon_x_move_jmp[])( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw ) = {
		weapon_x_dash, weapon_x_catch, weapon_x_float_init, weapon_x_float, weapon_x_fall,
	} ;

	weapon_x_move_jmp[pl_w->routine_3]( pl_w, ( PL07_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			The rush: a wall or a miss ends it, a hit			*/
/*			catches the opponent								*/
/*==============================================================*/
static void weapon_x_dash( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( pl_w->wall_flag ){
		if( pl_w->wall_flag != 1 << pl_w->pl_dir ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_char_set( pl_w, Char_Super, 3 ) ;
			return ;
		}
	}
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			em = pl_w->em_w ;
			pl_w->routine_3 ++ ;
			lw->em_w = em ;
			lw->catch_timer = 0 ;
			ofs = -100.0f ;
			if( pl_w->pl_dir ) ofs = 100.0f ;
			em->pos.x = pl_w->pos.x + ofs ;
			em->pos.y = pl_w->pos.y ;
			em->pl_stance = STANCE_STAND ;
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
			scr_excep_set( pl_w, 8, 8 ) ;
			pl_char_set( pl_w, Char_Super, 1 ) ;
		} else {
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Super, 4 ) ;
		}
	}
}

/*==============================================================*/
/*			The slashes on the caught opponent					*/
/*==============================================================*/
static void weapon_x_catch( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw->catch_timer ++ ;
	pl_w->push_ng_timer = 2 ;
	if( !pl_w->hit_flag ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->atck_no = pl_w->cg_type ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_type = 0 ;
			if( pl_w->cg_prm ){
				pl_voice_req( pl_w, pl_w->cg_prm + 29 ) ;
				pl_w->cg_prm = 0 ;
			}
			em = lw->em_w ;
			if( !em->armor_flag ) em->pos.y += DOT_Y( 2.0f ) ;
		}
	} else {
		pl_w->routine_3 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg_tm = 5 ;
		SCR_EXCEP_SET ;
		shl07_01_set( pl_w ) ;
		ofs = -80.0f ;
		if( pl_w->pl_dir ) ofs = 80.0f ;
		pl_w->pos.x += ofs ;
		pl_w->pos.y += DOT_Y( 32.0f ) ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_x_float_init( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Float ofs ;

	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 ++ ;
	em = lw->em_w ;
	em->ukemi_flag = 0 ;
	pl_w->timer0 = 50 ;
	ofs = -80.0f ;
	pl_w->spd.x = -DOT_X( 0.5f ) ;
	if( pl_w->pl_dir ){
		ofs = 80.0f ;
		pl_w->spd.x = -pl_w->spd.x ;
	}
	pl_w->pos.x += ofs ;
	pl_w->pos.y += DOT_Y( 24.0f ) ;
	pl_w->spd.y = DOT_Y( 0.5f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	weapon_x_float( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void weapon_x_float( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -DOT_X( 2.0f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_char_set2( pl_w, Char_Jump, 1, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_x_fall( PLAYER_WORK *pl_w, PL07_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void weapon_x_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void barrage_x_start( PLAYER_WORK *pl_w );
static void barrage_x_move( PLAYER_WORK *pl_w );
static void barrage_x_end( PLAYER_WORK *pl_w );
static void barrage_x_init( PLAYER_WORK *pl_w );
static void barrage_x_wait( PLAYER_WORK *pl_w );
static void barrage_x_move_init( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush of slashes along the			*/
/*			ground												*/
/*==============================================================*/
static void sp_barrage_x( PLAYER_WORK *pl_w )
{
	static void ( *barrage_x_jmp[])( PLAYER_WORK *pl_w ) = {
		barrage_x_start, barrage_x_move, barrage_x_end,
	} ;

	barrage_x_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void barrage_x_start( PLAYER_WORK *pl_w )
{
	static void ( *barrage_x_start_jmp[])( PLAYER_WORK *pl_w ) = {
		barrage_x_init, barrage_x_wait, barrage_x_move_init,
	} ;

	barrage_x_start_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void barrage_x_init( PLAYER_WORK *pl_w )
{
	int no ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl07_at_barrage_x ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	no = 5 ;
	if( pl_w->team_type == 4 ) no = 6 ;
	if( pl_w->team_type == 5 ) no = 6 ;
	pl_char_set( pl_w, Char_Super, no ) ;
}

/*--------------------------------------------------------------*/
static void barrage_x_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		if( pl_w->team_type != 5 ){
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			ofs.x = -80.0f ;
			ofs.y = DOT_Y( 16.0f ) ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void barrage_x_move_init( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->timer0 = 17 ;
	pl_w->timer1 = 0 ;
	pl_w->atck_level = 0 ;
	pl_w->spd.x = -DOT_X( 5.0f ) ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pls07_00_set( pl_w, 5 ) ;
	shl07_00_set( pl_w, 0 ) ;
	pl_voice_req( pl_w, 31 ) ;
	pl_char_set( pl_w, Char_Super, 7 ) ;
}

/*--------------------------------------------------------------*/
static void barrage_x_move( PLAYER_WORK *pl_w )
{
	int no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->wall_flag ){
		if( pl_char_move( pl_w ) < 0 ){
			if( --pl_w->timer0 == 0 ) goto finish ;
			pl_w->timer1 ^= 1 ;
			shl07_00_set( pl_w, pl_w->timer1 ) ;
			pl_voice_req( pl_w, 31 ) ;
			pl_w->atck_no = pl_w->timer0 + 65 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Super, pl_w->timer1 + 7 ) ;
		}
	} else if( pl_w->wall_flag != 1 << pl_w->pl_dir ) goto finish ;
	return ;
finish:
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	no = 67 ;
	if( pl_w->team_type == 4 || pl_w->team_type == 5 ) no = 98 ;
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
}

/*--------------------------------------------------------------*/
static void barrage_x_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void fatal_start( PLAYER_WORK *pl_w );
static void fatal_move( PLAYER_WORK *pl_w );
static void fatal_land( PLAYER_WORK *pl_w );
static void fatal_init( PLAYER_WORK *pl_w );
static void fatal_wait( PLAYER_WORK *pl_w );
static void fatal_jump( PLAYER_WORK *pl_w );
static void fatal_slash( PLAYER_WORK *pl_w );
static void fatal_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a leaping slash that throws out		*/
/*			claws												*/
/*==============================================================*/
static void sp_fatal( PLAYER_WORK *pl_w )
{
	static void ( *fatal_jmp[])( PLAYER_WORK *pl_w ) = {
		fatal_start, fatal_move, fatal_land,
	} ;

	fatal_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fatal_start( PLAYER_WORK *pl_w )
{
	static void ( *fatal_start_jmp[])( PLAYER_WORK *pl_w ) = {
		fatal_init, fatal_wait,
	} ;

	fatal_start_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void fatal_init( PLAYER_WORK *pl_w )
{
	int no ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl07_at_fatal ;
	HIT_RESET( pl_w ) ;
	no = 11 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		no ++ ;
	} else pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, no ) ;
}

/*--------------------------------------------------------------*/
static void fatal_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Sint32 *tbl ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->cg_type ^= 2 ;
		pl_w->timer0 = 8 ;
		pl_w->timer1 = 0 ;
		tbl = fatal_spd_tbl ;
		if( pl_w->pl_stance == STANCE_AIR ) tbl += 2 ;
		pl_w->spd.x = *tbl++ * 1.6666666f / 65536.0f ;
		pl_w->spd.y = *tbl * 2.142857f / 65536.0f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->pl_stance = STANCE_AIR ;
	} else if( pl_w->cg_type & 1 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type ^= 1 ;
		if( pl_w->pl_stance == STANCE_AIR ){
			ofs.x = -DOT_X( 16.0f ) ;
			ofs.y = DOT_Y( 80.0f ) ;
		} else {
			ofs.x = -120.0f ;
			ofs.y = DOT_Y( 52.0f ) ;
		}
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fatal_move( PLAYER_WORK *pl_w )
{
	static void ( *fatal_move_jmp[])( PLAYER_WORK *pl_w ) = {
		fatal_jump, fatal_slash, fatal_fall,
	} ;

	pl_w->push_ng_timer = 1 ;
	fatal_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void fatal_jump( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->spd.y = 0.0f ;
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 16.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		ef_color_set( pl_w, 2, 0, 1 ) ;
		shl07_02_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fatal_slash( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_w->spd.y = 0.0f ;
	if( pl_w->timer1 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -DOT_X( 2.5f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.5f ) ;
		pl_w->acc.y = -0.9375f ;
		pl_char_set2( pl_w, Char_Jump, 1, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fatal_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fatal_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void charge_init( PLAYER_WORK *pl_w );
static void charge_wait( PLAYER_WORK *pl_w );
static void charge_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Berserker Charge (hyper): a time of speed-up		*/
/*==============================================================*/
static void sp_charge( PLAYER_WORK *pl_w )
{
	static void ( *charge_jmp[])( PLAYER_WORK *pl_w ) = {
		charge_init, charge_wait, charge_end,
	} ;

	charge_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void charge_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl07_at_charge ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 14 ) ;
}

/*--------------------------------------------------------------*/
static void charge_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type ^= 1 ;
		ofs.x = 10.0f ;
		ofs.y = DOT_Y( 68.0f ) ;
		pl_hyper_flash( pl_w, &ofs, 1 ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type ^= 1 ;
		pl_w->speed_up_flag = 1 ;
		pl_w->speed_up_timer = 480 ;
		Timer_Gauge_Set( pl_w, &pl_w->speed_up_timer, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void charge_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type & 2 ) pls07_00_set( pl_w, 7 ) ;
	} else {
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A crouching slash									*/
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
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl07_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 44 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type ^= 1 ;
			pl_voice_req( pl_w, 32 ) ;
			pls07_00_set( pl_w, 1 ) ;
		}
		if( pl_w->cg_type & 2 ){
			pl_w->cg_type ^= 2 ;
			ofs.x = -DOT_X( 64.0f ) ;
			ofs.y = DOT_Y( 48.0f ) ;
			pl_snap_flash( pl_w, &ofs ) ;
		}
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_end,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls07_00_set( pl_w, 6 ) ;
	}
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl07_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl07_sp_barrage_x ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl07_sp_fatal ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl07_sp_fatal ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl07_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl07_sp_barrage_x ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl07_sp_fatal ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl07_sp_fatal ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl07_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_3 = pl_w->routine_2 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl07_sp_barrage ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl07_sp_tornado ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl07_sp_drill ;
		pl_w->atck_level = 1 ;
		pl_w->dir = 2 ;
		if( !pl_w->pl_dir ) pl_w->dir = 6 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl07_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_3 = pl_w->routine_2 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl07_sp_barrage ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl07_sp_tornado ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl07_sp_drill ;
		pl_w->atck_level = 1 ;
		pl_w->dir = 2 ;
		if( !pl_w->pl_dir ) pl_w->dir = 6 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void pl07_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl07_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 34 ) ;
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
		pl_char_set( pl_w, Char_Special, 35 ) ;
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
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl07_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_w->speed_up_flag ) return 0 ;
	if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( pl_w->atck_pk ){
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( em = pl_catch_ck( pl_w ) ) pl_w->catch_no = 1 ;
	} else if( pl_w->pl_stance == STANCE_AIR ){
		if( em = pl_catch_ck( pl_w ) ) pl_w->catch_no = 2 ;
	} else {
		if( em = pl_catch_ck( pl_w ) ) pl_w->catch_no = 0 ;
	}
	return em ;
}

static void nml_p_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/****************************************************************/
/*			Start the throw that pl07_catch_ck chose			*/
/****************************************************************/
void pl07_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		nml_p_set, nml_k_set, jump_p_set,
	} ;
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = pl_w->catch_w ;

	catch_gauge_add( pl_w ) ;
	pl_w->catch_ng_timer = 3 ;
	lw->hold_st = 0 ;
	em->disp_dir = pl_w->disp_dir ;
	em->disp_dir ^= 1 ;
	em->pl_dir = em->disp_dir ;
	catch_set_jmp[pl_w->catch_no]( pl_w, em ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	hold_renda_set( em, 56 ) ;
	catch_renda_set( pl_w, 2 ) ;
	scr_excep_set( pl_w, 6, 6 ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	hold_renda_set( em, 56 ) ;
	catch_renda_set( pl_w, 2 ) ;
	em->timer0 = 0 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	hold_renda_set( em, 56 ) ;
	catch_renda_set( pl_w, 2 ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The throws											*/
/****************************************************************/
void pl07_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 3 ;
	catch_jmp[pl_w->catch_no]( pl_w ) ;
}

static void nml_p_grip( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_follow( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_throw( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_chase( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_glide_init( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_glide( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_grip_ground( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_p_hold_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*==============================================================*/
/*			The punch throw: the opponent is held while			*/
/*			the buttons are mashed								*/
/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		nml_p_grip, nml_p_follow, nml_p_throw, nml_p_dummy, nml_p_chase,
		nml_p_glide_init, nml_p_glide, nml_p_grip_ground, nml_p_hold_hit,
	} ;
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = pl_w->catch_w ;

	if( pl_char_move( pl_w ) < 0 && lw->hold_st <= 0 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		pl_w->dm_flag = -128 ;
		pl_w->catch_ng_timer = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( lw->hold_st > 0 ){
		if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
		em->renda_timer -- ;
		if( hold_renda_ck( em ) ){
			lw->hold_st = -1 ;
			pl_char_set( pl_w, Char_Catch, 1 ) ;
		}
	}
	nml_p_catch_jmp[pl_w->cg_type >> 1]( pl_w, em ) ;
}

/*==============================================================*/
static void nml_p_grip( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_pos_set( pl_w, em ) ;
}

/*==============================================================*/
static void nml_p_follow( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_em_pos_set1( pl_w, em ) ;
}

/*==============================================================*/
static void nml_p_throw( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->cg_type = 6 ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->pl_dir = pl_w->pl_dir ;
	em->atck_no = pl07_at_throw_p ;
	em->pl_stance = STANCE_AIR ;
	scr_excep_clr() ;
}

/*==============================================================*/
static void nml_p_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
}

/*==============================================================*/
/*			Follow the opponent halfway while it moves far		*/
/*==============================================================*/
static void nml_p_chase( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;
	Float dx, dy ;

	scr_excep_set( pl_w, 6, 6 ) ;
	pos = pl_w->pos ;
	em->pos.y = em->ground_y ;
	catch_em_pos_set1( pl_w, em ) ;
	dx = ( pl_w->pos.x - pos.x ) / 2.0f ;
	dy = ( pl_w->pos.y - pos.y ) / 2.0f ;
	if( ( Uint32 )( dx + 1.0f ) > 2 || ( Uint32 )( dy + 1.0f ) > 2 ){
		pl_w->pos.x = pos.x + dx ;
		pl_w->pos.y = pos.y + dy ;
		pl_w->cg_ctr = 8 ;
	}
}

/*==============================================================*/
/*			Glide to the opponent over 16 frames				*/
/*==============================================================*/
static void nml_p_glide_init( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;

	pl_w->cg_type = 12 ;
	pos = pl_w->pos ;
	catch_em_pos_set1( pl_w, em ) ;
	pl_w->spd.x = ( pl_w->pos.x - pos.x ) / 16.0f ;
	pl_w->spd.y = ( pl_w->pos.y - pos.y ) / 16.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos = pos ;
	pl_w->timer0 = 16 ;
	nml_p_glide( pl_w, em ) ;
}

/*==============================================================*/
static void nml_p_glide( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( --pl_w->timer0 < 0 ){
		pl_w->cg_ctr = 1 ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 32.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->hit_stop = 10 ;
		scr_excep_set( pl_w, 1, 1 ) ;
	}
}

/*==============================================================*/
static void nml_p_grip_ground( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	catch_pos_set( pl_w, em ) ;
}

/*==============================================================*/
/*			A hit of the mashed hold							*/
/*==============================================================*/
static void nml_p_hold_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->cg_type = 0 ;
	lw->hold_st = 1 ;
	catch_pos_set( pl_w, em ) ;
	em->dm_hit_w = pl_w ;
	em->atck_no = pl07_at_throw_p_hold ;
	catch_damage( pl_w ) ;
	ofs.x = 80.0f ;
	ofs.y = DOT_Y( 16.0f ) ;
	hit_mark_set( pl_w, &ofs, 2 ) ;
	pl_se_req( pl_w, 5 ) ;
}

static void nml_k_grip( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_follow( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_jump( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_slam( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_leap_init( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_leap( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void nml_k_hold_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*==============================================================*/
/*			The kick throw										*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		nml_k_grip, nml_k_follow, nml_k_jump, nml_k_dummy, nml_k_slam,
		nml_k_leap_init, nml_k_leap, nml_k_jump, nml_k_leap, nml_k_hold_hit,
	} ;
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = pl_w->catch_w ;

	if( ( pl_w->cg_type != 8 || pl_w->cg_ctr == 1 ) && lw->unk_0d ){
		pl_w->cg_type = 8 ;
		pl_w->cg_ctr = 4 ;
	}
	lw->unk_0d = 0 ;
	if( pl_char_move( pl_w ) < 0 && lw->hold_st < 0 ){
		pl_w->dm_flag = -128 ;
		pl_w->catch_ng_timer = 0 ;
		pl_land_set( pl_w ) ;
		return ;
	}
	if( lw->hold_st > 0 ){
		if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
		em->renda_timer -- ;
		if( hold_renda_ck( em ) ){
			lw->hold_st = -1 ;
			pl_char_set( pl_w, Char_Catch, 3 ) ;
		}
	}
	nml_k_catch_jmp[pl_w->cg_type >> 1]( pl_w, em ) ;
}

/*==============================================================*/
static void nml_k_grip( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_pos_set( pl_w, em ) ;
}

/*==============================================================*/
static void nml_k_follow( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	em->timer0 ^= 1 ;
	catch_em_pos_set1( pl_w, em ) ;
}

/*==============================================================*/
/*			Jump up with the opponent							*/
/*==============================================================*/
static void nml_k_jump( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	scr_excep_clr() ;
	pl_w->cg_type = 16 ;
	pl_w->spd.x = DOT_X( 8.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 2 ;
	em->pl_dir = pl_w->pl_dir ^ 1 ;
	em->atck_no = pl07_at_throw_k ;
	pl_se_req( pl_w, 2 ) ;
	pl_voice_req( pl_w, 4 ) ;
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 16.0f ) ;
	hit_mark_set( pl_w, &ofs, 2 ) ;
	catch_se_req( pl_w ) ;
}

/*==============================================================*/
static void nml_k_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
}

/*==============================================================*/
/*			Slam the opponent down								*/
/*==============================================================*/
static void nml_k_slam( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 6, 6 ) ;
	pl_w->cg_type = 6 ;
	em->timer0 = 1 ;
	em->pos.y = em->ground_y ;
	pl_char_set( em, Char_Damage, 9 ) ;
	if( FLAG_CK( em->pl_bit, PLB0( PL_ANAKARIS ) | PLB0( PL_SPIDERMAN ) | PLB0( PL_MEGAMAN ) | PLB0( PL_BB_HOOD ), PLB1( PL_FELICIA ) ) ){
		em->pl_dir ^= 1 ;
		em->disp_dir = em->pl_dir ;
	}
	em->dm_hit_w = pl_w ;
	em->atck_no = pl07_at_throw_k_slam ;
	catch_damage( pl_w ) ;
	pl_se_req( pl_w, 5 ) ;
	pl_voice_req( pl_w, 3 ) ;
	ofs.x = -160.0f ;
	ofs.y = DOT_Y( 16.0f ) ;
	hit_mark_set( pl_w, &ofs, 2 ) ;
}

/*==============================================================*/
/*			Leap at the opponent								*/
/*==============================================================*/
static void nml_k_leap_init( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->cg_type = 12 ;
	pl_w->spd.x = ( em->pos.x - pl_w->pos.x ) / 32.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_w->pos.x -= pl_w->spd.x * 4.0f ;
	nml_k_leap( pl_w, em ) ;
}

/*==============================================================*/
static void nml_k_leap( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	pl_w->prio = em->prio - 1 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->cg_ctr = 1 ;
		if( pl_w->cg_type == 12 ){
			scr_excep_set( pl_w, 1, 1 ) ;
			ofs.x = 0.0f ;
			ofs.y = 0.0f ;
			nage_hit_set( pl_w, &ofs ) ;
			pl_w->hit_stop = 10 ;
			pl_voice_req( pl_w, 3 ) ;
		}
	}
}

/*==============================================================*/
/*			A hit of the mashed hold							*/
/*==============================================================*/
static void nml_k_hold_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->cg_type = 0 ;
	lw->hold_st = 1 ;
	catch_pos_set( pl_w, em ) ;
	em->dm_hit_w = pl_w ;
	em->atck_no = pl07_at_throw_k_hold ;
	catch_damage( pl_w ) ;
	pl_voice_req( pl_w, 3 ) ;
	pl_se_req( pl_w, 2 ) ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 8.0f ) ;
	hit_mark_set( pl_w, &ofs, 2 ) ;
}

static void jump_p_grip( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_follow( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_leap_init( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_chase( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_leap( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_hold_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_leap_start( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void jump_p_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*==============================================================*/
/*			The air throw										*/
/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		jump_p_grip, jump_p_follow, jump_p_leap_init, jump_p_dummy, jump_p_chase,
		jump_p_leap, jump_p_hold_hit, jump_p_leap_start, jump_p_hit,
	} ;
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em = pl_w->catch_w ;

	if( pl_char_move( pl_w ) < 0 && lw->hold_st <= 0 ){
		pl_w->dm_flag = -128 ;
		pl_w->catch_ng_timer = 0 ;
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( lw->hold_st > 0 ){
		if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
		em->renda_timer -- ;
		if( hold_renda_ck( em ) ){
			lw->hold_st = -1 ;
			pl_char_set( pl_w, Char_Catch, 5 ) ;
		}
	}
	jump_p_catch_jmp[pl_w->cg_type >> 1]( pl_w, em ) ;
}

/*==============================================================*/
static void jump_p_grip( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	scr_excep_set( pl_w, 1, 1 ) ;
	catch_pos_set( pl_w, em ) ;
}

/*==============================================================*/
static void jump_p_follow( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_em_pos_set1( pl_w, em ) ;
}

/*==============================================================*/
/*			Leap up with the opponent							*/
/*==============================================================*/
static void jump_p_leap_init( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->cg_type = 14 ;
	pl_w->spd.x = 5.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 5.0f ) ;
	pl_w->acc.y = -0.6696428f ;
	pls07_00_set( pl_w, 0 ) ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->pl_dir = pl_w->pl_dir ^ 1 ;
	em->atck_no = pl07_at_air_throw ;
	em->pl_stance = STANCE_AIR ;
}

/*==============================================================*/
static void jump_p_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
}

/*==============================================================*/
/*			Follow the opponent halfway while it moves			*/
/*==============================================================*/
static void jump_p_chase( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos ;

	scr_excep_set( pl_w, 6, 6 ) ;
	pos = pl_w->pos ;
	catch_em_pos_set1( pl_w, em ) ;
	if( ( int )pl_w->pos.x != ( int )pos.x || ( int )pl_w->pos.y != ( int )pos.y ){
		pl_w->pos.x = pos.x + ( pl_w->pos.x - pos.x ) / 2.0f ;
		pl_w->pos.y = pos.y + ( pl_w->pos.y - pos.y ) / 2.0f ;
		pl_w->cg_ctr = 8 ;
	}
}

/*==============================================================*/
static void jump_p_leap( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*==============================================================*/
/*			A hit of the mashed hold							*/
/*==============================================================*/
static void jump_p_hold_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	PL07_LOCAL_W *lw = ( PL07_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->cg_type = 0 ;
	lw->hold_st = 1 ;
	catch_pos_set( pl_w, em ) ;
	em->dm_hit_w = pl_w ;
	em->atck_no = pl07_at_air_throw_hold ;
	catch_damage( pl_w ) ;
	pl_se_req( pl_w, 5 ) ;
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 8.0f ) ;
	hit_mark_set( pl_w, &ofs, 2 ) ;
}

/*==============================================================*/
static void jump_p_leap_start( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->cg_type = 10 ;
	pl_se_req( pl_w, 5 ) ;
	scr_excep_clr() ;
	jump_p_leap( pl_w, em ) ;
}

/*==============================================================*/
static void jump_p_hit( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	pl_w->cg_type = 2 ;
	ofs.x = -DOT_X( 16.0f ) ;
	ofs.y = DOT_Y( 32.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_em_pos_set1( pl_w, em ) ;
}

/****************************************************************/
/*			Held: the kick throw lets its animation run			*/
/****************************************************************/
void pl07_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_no == 1 && pl_w->timer0 ) pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl07.c" End of File	*/
