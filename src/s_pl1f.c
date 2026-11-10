/* B.B. Hood (character 1f): command checks, normal attacks, appearance, win poses, special moves
   and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl1f_def.h"

void pl1f_main( PLAYER_WORK *pl_w );
void pl1f_com_ck( PLAYER_WORK *pl_w );
void pl1f_atck_set( PLAYER_WORK *pl_w );
void pl1f_atck( PLAYER_WORK *pl_w );
void pl1f_dash( PLAYER_WORK *pl_w );
void pl1f_noki( PLAYER_WORK *pl_w );
void pl1f_appear( PLAYER_WORK *pl_w );
void pl1f_win( PLAYER_WORK *pl_w );
void pl1f_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl1f_catch_ck( PLAYER_WORK *pl_w );
void pl1f_catch_set( PLAYER_WORK *pl_w );
void pl1f_catch( PLAYER_WORK *pl_w );
void pl1f_hold( PLAYER_WORK *pl_w );
int pl1f_delay_ck( PLAYER_WORK *pl_w );
void pl1f_t_kick( PLAYER_WORK *pl_w );
void pl1f_upper( PLAYER_WORK *pl_w );
void pl1f_sp_unison( PLAYER_WORK *pl_w );
void pl1f_sp_gattai( PLAYER_WORK *pl_w );
void pl1f_sp_assist( PLAYER_WORK *pl_w );
void pl1f_sp_zero( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_missile[] = { 40, COM_GROUND, pl1f_sp_missile, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ; /* Smile & Missile */
static Uint16 com_happy[] = { 40, COM_GROUND, pl1f_sp_happy, 0, KEY_BACK, KEY_FORWARD, KEY_KICK } ; /* Smile & Missile (low; Happy & Missile) */
static Uint16 com_missile_j[] = { 40, COM_GROUND, pl1f_sp_missile_j, 0, KEY_DOWN, KEY_UP, KEY_PUNCH } ; /* Smile & Missile (jumping) */
static Uint16 com_cheer[] = { 3, COM_GROUND, pl1f_sp_cheer, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ; /* Cheer & Fire (angled) */
static Uint16 com_cheer_k[] = { 3, 0, pl1f_sp_cheer_k, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ; /* Cheer & Fire (horizontal, also in the air) */
static Uint16 com_shyness[] = { 3, COM_GROUND, pl1f_sp_shyness, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ; /* Shyness & Strike */
static Uint16 com_sp02[] = { 5, COM_GROUND, pl1f_sp_02, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;
static Uint16 com_hunting[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1f_sp_hunting, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Cool Hunting */
static Uint16 com_memory[] = { 5, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1f_sp_memory, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Beautiful Memory */
static Uint16 com_apple[] = { 5, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1f_sp_apple, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* Apple for You */
static Uint16 com_tellme[] = { 3, COM_GROUND | COM_ALL_SW, pl1f_sp_tellme, 0, KEY_DOWN, 0x0000, KEY_DOWN, KEY_KICK } ; /* Tell Me Why */

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

static void ( *pl1f_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl1f_main,
	pl1f_com_ck,
	pl1f_atck_set,
	pl1f_atck,
	pl1f_dash,
	pl1f_noki,
	pl1f_appear,
	pl1f_win,
	pl1f_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl1f_catch_ck,	/* returns a value */
	pl1f_catch_set,
	pl1f_catch,
	pl1f_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl1f_delay_ck,
	pl1f_t_kick,
	pl1f_upper,
	pl1f_sp_unison,
	pl1f_sp_gattai,
	pl1f_sp_assist,
	pl1f_sp_zero,
} ;

static Uint8 memory_ck( PLAYER_WORK *pl_w );
static Uint8 hunting_ck( PLAYER_WORK *pl_w );
static Uint8 apple_ck( PLAYER_WORK *pl_w );
static Uint8 shyness_ck( PLAYER_WORK *pl_w );
static Uint8 cheer_ck( PLAYER_WORK *pl_w );
static Uint8 cheer_k_ck( PLAYER_WORK *pl_w );
static Uint8 missile_ck( PLAYER_WORK *pl_w );
static Uint8 happy_ck( PLAYER_WORK *pl_w );
static Uint8 missile_j_ck( PLAYER_WORK *pl_w );
static Uint8 tellme_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int hunting_delay_ck( PLAYER_WORK *pl_w );
static int memory_delay_ck( PLAYER_WORK *pl_w );
static int apple_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1f_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl1f_tbl[i] ;
}

/****************************************************************/
/*			Stop dead and stand									*/
/****************************************************************/
void rate_clr_stand( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl1f_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( memory_ck( pl_w ) ) return ;
	if( hunting_ck( pl_w ) ) return ;
	if( apple_ck( pl_w ) ) return ;
	if( shyness_ck( pl_w ) ) return ;
	if( cheer_ck( pl_w ) ) return ;
	if( cheer_k_ck( pl_w ) ) return ;
	if( missile_ck( pl_w ) ) return ;
	if( happy_ck( pl_w ) ) return ;
	if( missile_j_ck( pl_w ) ) return ;
	if( tellme_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 memory_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_memory, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_memory ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hunting_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hunting, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_hunting ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 apple_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_apple, &pl_w->com_w1 ) ) return 0 ;
	if( *pl_w->gauge_level < 1 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_apple ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shyness_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_shyness, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_shyness ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 cheer_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cheer, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_cheer ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Once a jump											*/
/*==============================================================*/
static Uint8 cheer_k_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_cheer_k, &pl_w->com_w10 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w10 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_cheer_k ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 missile_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_missile, &pl_w->com_w6 ) ) return 0 ;
	if( PL1F_LW( pl_w )->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_missile ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 happy_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_happy, &pl_w->com_w7 ) ) return 0 ;
	if( PL1F_LW( pl_w )->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_happy ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 missile_j_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_missile_j, &pl_w->com_w8 ) ) return 0 ;
	if( PL1F_LW( pl_w )->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_missile_j ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 tellme_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tellme, &pl_w->com_w9 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1f_sp_tellme ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl1f_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 11 ) ) return 0 ;
	pl_w->pl_sp_type = pl1f_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl1f_delay_ck( PLAYER_WORK *pl_w )
{
	if( hunting_delay_ck( pl_w ) || memory_delay_ck( pl_w ) || apple_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Reads its command into the work memory_ck uses		*/
/*==============================================================*/
static int hunting_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hunting, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl1f_sp_hunting ;
	return 1 ;
}

/*==============================================================*/
static int memory_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_memory, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl1f_sp_memory ;
	return 1 ;
}

/*==============================================================*/
static int apple_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_apple, &pl_w->com_w1 ) ) return 0 ;
	if( *pl_w->gauge_level < 1 ) return 0 ;
	pl_w->delay_sp_type = pl1f_sp_apple ;
	return 1 ;
}

/****************************************************************/
void pl1f_main( PLAYER_WORK *pl_w )
{
}

static void nml_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void nml_atck_dash( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl1f_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck_dash,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The standing heavy punch with the stick forward		*/
/*			or back is its own									*/
/*==============================================================*/
static void nml_atck_dash( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( !pl_w->atck_pk && !pl_w->pl_stance && pl_w->nml_no == 2 && ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ){
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->acc.x *= 2.0f ;
		pl_w->nml_level = 2 ;
		pl_w->chain_tbl = 0 ;
		pl_w->routine_2 = 2 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->atck_no = pl1f_at_dash_hp ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
	} else nml_atck( pl_w ) ;
}

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
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
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
	switch( pl_w->nml_no ){
	case 0:
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ){
			pl_w->chain_tbl = 0 ;
			pl_w->routine_2 = 1 ;
			pl_w->atck_no = pl1f_at_fwd_hp ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Stand_P, 6 ) ;
			return ;
		}
		break ;
	}
	pl_w->atck_no = pl_w->nml_no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->nml_no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
	pl_w->atck_no = pl_w->nml_no + 6 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->nml_no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	Sint16 no = pl_w->nml_no ;
	Sint16 atck = pl_w->nml_no + 3 ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			no = 6 ;
			pl_w->routine_2 = 1 ;
			atck = 19 ;
			pls1f_03_set( pl_w, 22 ) ;
		} else shl1f_00_set( pl_w, 0 ) ;
		break ;
	}
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	Sint16 no = pl_w->nml_no ;
	Sint16 atck = pl_w->nml_no + 9 ;

	switch( pl_w->nml_no ){
	case 0:
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_BACK ){
			no = 3 ;
			atck = 20 ;
			shl1f_00_set( pl_w, 1 ) ;
		}
		break ;
	}
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Crouch_K, no ) ;
}

static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	int no ;

	switch( pl_w->nml_no ){
	case 0:
		no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Jump_P, no + 3 ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	int no ;

	switch( pl_w->nml_no ){
	case 0:
		no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		shl1f_00_set( pl_w, 2 ) ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Jump_K, no + 3 ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

static void nml_move( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1f_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
/*			The heavy punch's dash stops when it would turn		*/
/*			back												*/
/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_smoke_set( pl_w ) ;
	spd = pl_w->spd.x ;
	if( pl_w->routine_2 == 2 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->spd.x ){
			if( pl_w->spd.x * spd < 0.0f ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
		}
	} else nml_move( pl_w ) ;
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
/*			The heavy punch hits twice, or jumps				*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( !pl_w->routine_2 ){
			if( pl_w->cg_type & 1 ){
				pl_w->cg_type = 0 ;
				pl_w->atck_no = ATCK_STAND_HP ;
				HIT_RESET( pl_w ) ;
			}
			if( pl_w->cg_type & 2 ){
				pl_w->cg_type = 0 ;
				pl_w->atck_no = pl1f_at_stand_hp_2 ;
				HIT_RESET( pl_w ) ;
			}
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			break ;
		} else {
			pl_char_move( pl_w ) ;
			if( !pl_w->routine_3 ){
				if( !pl_w->cg_type ) break ;
				pl_w->routine_3 ++ ;
				pl_w->spd.y = DOT_Y( 3.0f ) ;
				pl_w->acc.y = -DOT_Y( 0.25f ) ;
				pl_w->spd.x = DOT_X( 5.0f ) ;
				pl_w->acc.x = -0.15625f ;
				if( !( pl_w->atck_lever & KEY_FORWARD ) ){
					pl_w->spd.x = -pl_w->spd.x ;
					pl_w->acc.x = -pl_w->acc.x ;
				}
				if( pl_w->disp_dir ) break ;
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			} else if( !pl_w->cg_type ){
				pl_w->routine_3 = pl_w->routine_2 = 0 ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
			}
			break ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = ATCK_CROUCH_HP ;
			HIT_RESET( pl_w ) ;
		}
		if( pl_w->cg_type & 2 ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl1f_at_crouch_hp_2 ;
			HIT_RESET( pl_w ) ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick with the stick forward slides,		*/
/*			then hops											*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->routine_2 != 0 ){
			pl_char_move( pl_w ) ;
			if( !pl_w->routine_3 ){
				if( !pl_w->cg_type ) break ;
				pl_w->routine_3 ++ ;
				pl_w->spd.x = DOT_X( 8.0f ) ;
				pl_w->acc.x = -0.78125f ;
				if( pl_w->disp_dir ) break ;
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			} else if( !pl_w->cg_type ){
				pl_w->routine_2 = 0 ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
			} else if( pl_w->cg_type == 2 ){
				pl_w->cg_type = 1 ;
				pl_w->spd.y = DOT_Y( 4.0f ) ;
				pl_w->acc.y = -DOT_Y( 0.5f ) ;
			}
			break ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
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
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = ATCK_JUMP_HP ;
			HIT_RESET( pl_w ) ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl1f_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl1f_at_upper ;
		pl_w->pl_stance = STANCE_STAND ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
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
/*			The forward dash									*/
/****************************************************************/
void pl1f_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 24 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
	pl_w->acc.x = pl_w->pl_dir ? -0.078125f : 0.078125f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.5f ) : -DOT_X( 2.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.2864583f : 0.2864583f ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl1f_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 24 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 5.0f ) : DOT_X( 5.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 0.065104164f : -0.065104164f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 == 0 ){
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.5f ) : DOT_X( 2.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.2864583f : -0.2864583f ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Walks in from off to one side; acc.x keeps the		*/
/*			place to stop										*/
/****************************************************************/
void pl1f_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->acc.x = pl_w->pos.x ;
	if( pl_w->disp_dir ){
		pl_w->pos.x -= DOT_X( 128.0f ) ;
		pl_w->spd.x = 5.0f ;
	} else {
		pl_w->pos.x += DOT_X( 128.0f ) ;
		pl_w->spd.x = -5.0f ;
	}
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_char_move( pl_w ) ;
	if( fabs( pl_w->pos.x - pl_w->acc.x ) < DOT_X( 5.0f ) ){
		pl_w->routine_1 ++ ;
		pl_w->pos.x = pl_w->acc.x ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

static void win_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1f_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko,
		lose_time_ko,
		win_time,
		lose_time_ko,
		draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/* the KO win poses, by the buttons held, or at random */
static Uint8 win_tbl[8] = { 0, 0, 1, 0, 3, 3, 1, 3 } ;

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	Uint8 no ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( !vs_sys_work.vs_end_flag ) pl_w->routine_3 = 0 ;
		else if( sw = pl_w->pad_sw.now & KEY_ATCK ){
			no = 0 ;
			if( sw & KEY_LP ) no = 0 ;
			if( sw & KEY_HP ) no = 1 ;
			if( ( sw & KEY_PUNCH ) == 0x0300 ) no = 3 ;
			if( sw & KEY_LK ) no = 0 ;
			if( sw & KEY_HK ) no = 1 ;
			if( ( sw & KEY_KICK ) == 0x0060 ) no = 3 ;
			pl_w->routine_3 = no ;
		} else pl_w->routine_3 = win_tbl[Rnd() & 7] ;
		pl_char_set( pl_w, Char_Win, pl_w->routine_3 ) ;
		pl_w->timer0 = 0 ;
		ef_color_set( pl_w, 3, 11, 1 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->routine_3 == 1 ){
			if( pl_w->timer0 && --pl_w->timer0 <= 0 ){
				pl_voice_req( pl_w, 19 ) ;
				pl_w->timer0 = 0 ;
			}
			pl_w->win_wait = 0xff ;
			if( pl_w->cg_type ){
				pl_w->timer0 = 60 ;
				pl_w->cg_type = 0 ;
				pls1f_03_set( pl_w, 14 ) ;
			}
		}
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 5 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 4 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 6 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			A hyper combo starts standing, still				*/
/*==============================================================*/
static void super_ground_set( PLAYER_WORK *pl_w )
{
	pl_voice_req( pl_w, 43 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
}

static void sp_memory( PLAYER_WORK *pl_w );
static void sp_hunting( PLAYER_WORK *pl_w );
static void sp02( PLAYER_WORK *pl_w );
static void sp_apple( PLAYER_WORK *pl_w );
static void sp_shyness( PLAYER_WORK *pl_w );
static void sp_cheer( PLAYER_WORK *pl_w );
static void sp_missile( PLAYER_WORK *pl_w );
static void sp_happy( PLAYER_WORK *pl_w );
static void sp_missile_j( PLAYER_WORK *pl_w );
static void sp_tellme( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl1f_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_memory, sp_hunting, sp02, sp_apple, sp_shyness, sp_cheer, sp_missile, sp_happy, sp_missile_j, sp_tellme, sp_snapback, sp_chouhatsu, sp_cheer,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void memory_init( PLAYER_WORK *pl_w );
static void memory_flash( PLAYER_WORK *pl_w );
static void memory_wait( PLAYER_WORK *pl_w );
static void memory_dash( PLAYER_WORK *pl_w );
static void memory_back( PLAYER_WORK *pl_w );
static void memory_miss( PLAYER_WORK *pl_w );
static void memory_catch( PLAYER_WORK *pl_w );
static void memory_jump( PLAYER_WORK *pl_w );
static void memory_fall( PLAYER_WORK *pl_w );
static void memory_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that grabs the opponent on its		*/
/*			first hit											*/
/*==============================================================*/
static void sp_memory( PLAYER_WORK *pl_w )
{
	static void ( *memory_jmp[])( PLAYER_WORK *pl_w ) = {
		memory_init, memory_flash, memory_wait, memory_dash, memory_back, memory_miss, memory_catch, memory_jump, memory_fall, memory_end,
	} ;

	memory_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void memory_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 15 ;
	super_ground_set( pl_w ) ;
	pl_w->atck_no = pl1f_at_memory ;
	HIT_RESET( pl_w ) ;
	pl_w->atck_attr |= ATR_SHOT_HOLD ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void memory_flash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_w->routine_2 ++ ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	ofs.x = 0.0f ;
	ofs.y = DOT_Y( 76.0f ) ;
	ofs.z = 0.0f ;
	pl_hyper_start( pl_w, &ofs, 1 ) ;
}

/*--------------------------------------------------------------*/
static void memory_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer0 == 15 ){
		pls1f_03_set( pl_w, 4 ) ;
		ef_color_set( pl_w, 3, 2, 1 ) ;
	}
	if( --pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 15 ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = -DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.104166664f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void memory_dash( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag ){
		if( pl_sp_catch_ck( pl_w ) ){
			scr_excep_set( pl_w, 13, 7 ) ;
			pl_w->routine_2 = 6 ;
			pl_w->timer0 = 47 ;
			pl_w->timer1 = 0 ;
			pl_char_set( pl_w, Char_Super, 4 ) ;
			pl_w->catch_no = 0xc2 ;
			pl_sp_catch_set( pl_w, pl_w->em_w ) ;
		} else {
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->routine_2 = 4 ;
			pl_w->spd.x = -1.6666666f ;
			pl_w->acc.x = 0.0065104165f ;
			pl_w->spd.y = DOT_Y( 6.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
			if( !pl_w->disp_dir ){
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			}
			pl_char_set( pl_w, Char_Super, 2 ) ;
		}
		pl_w->hit_stop = 10 ;
	} else if( --pl_w->timer0 < 0 ){
		pl_w->routine_2 = 5 ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*==============================================================*/
/*			Missed: falls back to the ground					*/
/*==============================================================*/
static void memory_back( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void memory_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) rate_clr_stand( pl_w ) ;
}

/*==============================================================*/
/*			The opponent caught: two rounds of hits				*/
/*==============================================================*/
static void memory_catch( PLAYER_WORK *pl_w )
{
	Uint8 rnd ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ){
		if( !pl_w->timer1 ){
			pl_w->timer0 = 63 ;
			pl_w->timer1 ++ ;
			pl_char_set( pl_w, Char_Super, 5 ) ;
			pls1f_03_set( pl_w, 3 ) ;
			PL1F_LW( pl_w )->ef_char_no = 33 ;
		} else {
			PL1F_LW( pl_w )->ef_char_no = 34 ;
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 63 ;
			pl_char_set( pl_w, Char_Super, 6 ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 3.0f ) ;
			pl_w->acc.y = -0.06696428f ;
		}
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		if( !pl_w->timer1 ) pl_w->catch_w->atck_no = pl1f_at_memory_hit_1 ;
		else pl_w->catch_w->atck_no = pl1f_at_memory_hit_2 ;
		catch_damage( pl_w ) ;
		combo_add( pl_w->catch_w, pl_w, 1 ) ;
		if( pl_w->timer1 ){
			ofs.x = -60.0f ;
			ofs.y = 120.0f ;
			hit_mark_set( pl_w, &ofs, 3 ) ;
		} else {
			rnd = Rnd() & 7 ;
			ofs.x = -DOT_X( 74.0f ) ;
			ofs.y = 120.0f ;
			hit_mark_set( pl_w, &ofs, rnd + 9 ) ;
		}
	}
	catch_pos_set( pl_w, pl_w->catch_w ) ;
}

/*==============================================================*/
/*			Leaps away from the opponent, who is pulled			*/
/*			along												*/
/*==============================================================*/
static void memory_jump( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->cg_type < 0 ){
			PL1F_LW( pl_w )->ef_char_no = 35 ;
			pl_w->cg_type = 0 ;
			pl_w->routine_2 ++ ;
			pl_w->spd.x = -DOT_X( 2.0f ) ;
			pl_w->acc.x = 0.052083332f ;
			pl_w->spd.y = DOT_Y( 2.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.125f ) ;
			em->spd.x = 0.0f ;
			em->spd.y = 0.0f ;
			em->acc.x = 0.0f ;
			em->acc.y = 0.0f ;
			em->spd.x = ( pl_w->pos.x - em->pos.x ) / 16.0f ;
			em->spd.y = ( em->ground_y - em->pos.y ) / 8.0f ;
			if( !pl_w->disp_dir ){
				pl_w->spd.x = -pl_w->spd.x ;
				pl_w->acc.x = -pl_w->acc.x ;
			}
			return ;
		}
	}
	catch_pos_set( pl_w, pl_w->catch_w ) ;
}

/*--------------------------------------------------------------*/
static void memory_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_w->catch_w->pos.x += pl_w->catch_w->spd.x ;
	pl_w->catch_w->spd.x += pl_w->catch_w->acc.x ;
	pl_w->catch_w->pos.y += pl_w->catch_w->spd.y ;
	pl_w->catch_w->spd.y += pl_w->catch_w->acc.y ;
	if( pl_w->catch_w->ground_y > pl_w->catch_w->pos.y ){
		pl_w->catch_w->spd.x = 0.0f ;
		pl_w->catch_w->spd.y = 0.0f ;
		pl_w->catch_w->acc.x = 0.0f ;
		pl_w->catch_w->acc.y = 0.0f ;
		pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		pl_w->catch_w->disp_flag = 0 ;
	}
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 36 ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
		combo_add( pl_w->catch_w, pl_w, 1 ) ;
		pl_w->catch_w->catch_type = 16 ;
		pl_w->atck_no = pl_w->catch_w->atck_no = pl1f_at_memory_last ;
		pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 24.0f ) ;
		pl_w->catch_w->disp_flag = 1 ;
		catch_damage( pl_w ) ;
		PL1F_LW( pl_w )->ef_char_no = 36 ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void memory_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ) pl_stand_ret( pl_w ) ;
}

static void hunting_init( PLAYER_WORK *pl_w );
static void hunting_flash( PLAYER_WORK *pl_w );
static void hunting_wait( PLAYER_WORK *pl_w );
static void hunting_shot( PLAYER_WORK *pl_w );
static void hunting_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_hunting( PLAYER_WORK *pl_w )
{
	static void ( *hunting_jmp[])( PLAYER_WORK *pl_w ) = {
		hunting_init, hunting_flash, hunting_wait, hunting_shot, hunting_end,
	} ;

	hunting_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hunting_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 128 ;
	super_ground_set( pl_w ) ;
	ef_color_set( pl_w, 7, pl_w->pl_color + 3, 1 ) ;
	pl_w->atck_no = pl1f_at_hunting ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
	pls1f_00_set( pl_w, 5, 0 ) ;
}

/*--------------------------------------------------------------*/
static void hunting_flash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_w->routine_2 ++ ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
	ofs.x = 0.0f ;
	ofs.y = DOT_Y( 76.0f ) ;
	ofs.z = 0.0f ;
	pl_hyper_start( pl_w, &ofs, 1 ) ;
}

/*--------------------------------------------------------------*/
static void hunting_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->timer0 -- ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pls1f_00_set( pl_w, 6, 0 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hunting_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( --pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 9 ) ;
		pls1f_00_set( pl_w, 7, 0 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	} else {
		if( pl_w->cg_type == 1 ){
			pl_w->cg_type = 0 ;
			shl1f_02_set( pl_w, 0 ) ;
			shl1f_02_set( pl_w, 3 ) ;
		}
		if( pl_w->cg_type == 2 ){
			pl_w->cg_type = 0 ;
			shl1f_02_set( pl_w, 1 ) ;
			shl1f_02_set( pl_w, 2 ) ;
		}
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hunting_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Nothing of its own: back to standing or jumping		*/
/*==============================================================*/
static void sp02( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else rate_clr_stand( pl_w ) ;
}

/*==============================================================*/
/*			The hyper combo that throws: the dash, the			*/
/*			catch, and the throw's own steps					*/
/*==============================================================*/
static void apple_init( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !pl_w->routine_3 ){
		if( pl_w->team_type == 6 ){
			pl_w->stop_move_flag = 0xff ;
			pl_w->stop_move_timer = 16 ;
		}
		pl_w->routine_3 ++ ;
		super_ground_set( pl_w ) ;
		pl_w->spd.x = -5.0f ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->atck_no = pl1f_at_apple ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	} else {
		pl_w->delay_timer = 2 ;
		pl_w->hyper_bg_tm = 5 ;
		pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->cg_type = 0 ;
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			ofs.x = 0.0f ;
			ofs.y = DOT_Y( 76.0f ) ;
			ofs.z = 0.0f ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void apple_dash( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 24 ;
		pl_w->acc.x = DOT_X( 0.125f ) ;
		if( pl_w->disp_dir ) pl_w->acc.x = -pl_w->acc.x ;
		pl_char_set( pl_w, Char_Super, 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void apple_grab( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;
	Float spd, x, y ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	spd = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * spd < 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( --pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 = 7 ;
		pl_w->timer0 = 4 ;
	} else if( ( em = pl_catch_ck( pl_w ) ) ){
		scr_excep_set( pl_w, 13, 1 ) ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer1 = 0 ;
		em->catch_no = pl_w->catch_no = 0xc3 ;
		pl_sp_catch_set( pl_w, em ) ;
		PL1F_LW( pl_w )->dish_hit = 0 ;
		PL1F_LW( pl_w )->unk_44 = 0 ;
		pl_w->hit_stop = 10 ;
		pos.x = -DOT_X( 80.0f ) ;
		if( pl_w->disp_dir ) pos.x = -pos.x ;
		pos.x += pl_w->pos.x ;
		pos.y = pl_w->pos.y + DOT_Y( 8.0f ) ;
		hitmark_set( &pos, pl_w->pl_dir, 2, 0 ) ;
		x = pl_w->pos.x ;
		y = pl_w->pos.y ;
		pl_w->catch_grip_old = -1 ;
		LO_BYTE( pl_w->cg_rival ) = 32 ;
		catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
		PL1F_LW( pl_w )->hold_x = pl_w->pos.x ;
		PL1F_LW( pl_w )->hold_y = pl_w->pos.y ;
		pl_w->spd.x = ( pl_w->pos.x - x ) / 40.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = ( pl_w->pos.y - y ) / 40.0f + DOT_Y( 7.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->timer0 = 40 ;
		pl_char_set( pl_w, Char_Super, 16 ) ;
		pl_w->catch_grip_old = -1 ;
		LO_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->pos.x = x ;
		pl_w->pos.y = y ;
		pl_w->catch_w->timer0 = 1 ;
		pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		pl_char_set( pl_w->catch_w, Char_Damage, 9 ) ;
		if( FLAG_CK( pl_w->catch_w->pl_bit, PLB0( PL_ANAKARIS ) | PLB0( PL_SPIDERMAN ) | PLB0( PL_MEGAMAN ) | PLB0( PL_BB_HOOD ), PLB1( PL_FELICIA ) ) ){
			pl_w->catch_w->pl_dir ^= 1 ;
			pl_w->catch_w->disp_dir = pl_w->catch_w->pl_dir ;
		}
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl1f_at_apple ;
		catch_damage( pl_w ) ;
		hit_se_req( pl_w->catch_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void apple_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
	}
}

/*==============================================================*/
static void apple_hit_jump( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 > 0 ) return ;
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Super, 11 ) ;
	pl_w->catch_grip_old = -1 ;
	LO_BYTE( pl_w->cg_rival ) = 34 ;
	catch_em_pos_set0( pl_w, pl_w->catch_w ) ;
	pl_w->catch_grip_old = -1 ;
	LO_BYTE( pl_w->cg_rival ) = 0 ;
	em->dm_hit_w = pl_w ;
	em->atck_no = pl1f_at_apple_hit ;
	catch_damage( pl_w ) ;
	combo_add( em, pl_w, 1 ) ;
	hit_se_req( em, 3 ) ;
	ofs.x = -DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 16.0f ) ;
	hit_mark_set( pl_w, &ofs, 3 ) ;
}

/*--------------------------------------------------------------*/
static void apple_hit_hop( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y < 0.0f && PL1F_LW( pl_w )->hold_y > pl_w->pos.y ){
		pl_w->pos.y = PL1F_LW( pl_w )->hold_y ;
		pl_w->timer0 = 96 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.875f ) ;
		pl_char_set( pl_w, Char_Super, 15 ) ;
		pl_char_move( pl_w ) ;
		pl_w->routine_3 ++ ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl1f_at_apple_hit ;
		catch_damage( pl_w ) ;
		hit_se_req( em, 1 ) ;
		combo_add( em, pl_w, 1 ) ;
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 16.0f ) ;
		hit_mark_set( pl_w, &ofs, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void apple_hit_bounce( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;
	Float x, y ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		LO_BYTE( pl_w->cg_rival ) = 33 ;
	} else if( pl_w->cg_type >= 0 ){
		if( pl_w->cg_type > 0 ){
			pl_w->cg_type = 0 ;
			pl_w->spd.y += DOT_Y( 5.0f ) ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl1f_at_apple_hit ;
			catch_damage( pl_w ) ;
			hit_se_req( em, 1 ) ;
			combo_add( em, pl_w, 1 ) ;
			ofs.x = 0.0f ;
			ofs.y = DOT_Y( 24.0f ) ;
			hit_mark_set( em, &ofs, 3 ) ;
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	x = em->pos.x ;
	y = em->pos.y ;
	catch_pos_set( pl_w, em ) ;
	em->pos.x = x ;
	em->pos.y = y ;
}

/*--------------------------------------------------------------*/
static void apple_hit_fall( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;	/* unused */

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( PL1F_LW( pl_w )->hold_y > pl_w->pos.y ){
		pl_w->pos.y = PL1F_LW( pl_w )->hold_y ;
		LO_BYTE( pl_w->cg_rival ) = 33 ;
		pl_w->catch_grip_old = -1 ;
		catch_em_pos_set1( pl_w, em ) ;
		LO_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->catch_grip_old = -1 ;
		pl_w->timer0 = 96 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 12 ) ;
		pl_char_move( pl_w ) ;
		pl_w->routine_3 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void apple_hit_keep( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 ofs ;
	Float x, dx, lim_r, lim_l, gy ;

	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		scr_excep_clr() ;
		pl_voice_req( pl_w, 43 ) ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->timer1 = 0 ;
		LO_BYTE( pl_w->cg_rival ) = 33 ;
		x = em->pos.x ;
		catch_pos_set( pl_w, em ) ;
		em->pos.x = x ;
		em->pos.y = em->ground_y ;
		dx = DOT_X( 152.0f ) ;
		if( pl_w->disp_dir ){
			lim_r = scr_work.stage_right + -160.0f ;
			if( pl_w->pos.x > lim_r ){
				dx = -DOT_X( 152.0f ) ;
				pl_w->disp_dir = pl_w->pl_dir = 1 ;
			} else pl_w->disp_dir = pl_w->pl_dir = 0 ;
		} else {
			lim_l = scr_work.stage_left + 160.0f ;
			dx = -DOT_X( 152.0f ) ;
			if( lim_l > pl_w->pos.x ){
				dx = DOT_X( 152.0f ) ;
				pl_w->disp_dir = pl_w->pl_dir = 0 ;
			} else pl_w->disp_dir = pl_w->pl_dir = 1 ;
		}
		dx = em->pos.x + dx ;
		gy = pl_w->ground_y ;
		pl_w->spd.x = ( dx - pl_w->pos.x ) / 48.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = ( pl_w->pos.y - gy ) / 48.0f + DOT_Y( 9.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->timer0 = 24 ;
	} else {
		catch_pos_set( pl_w, em ) ;
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type = 0 ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl1f_at_apple_hit ;
			catch_damage( pl_w ) ;
			hit_se_req( em, 1 ) ;
			combo_add( em, pl_w, 1 ) ;
			ofs.x = -DOT_X( 32.0f ) ;
			ofs.y = DOT_Y( 16.0f ) ;
			hit_mark_set( pl_w, &ofs, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void apple_hit( PLAYER_WORK *pl_w )
{
	static void ( *apple_hit_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		apple_hit_jump, apple_hit_hop, apple_hit_bounce, apple_hit_fall, apple_hit_keep,
	} ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	apple_hit_jmp[pl_w->routine_3]( pl_w, pl_w->catch_w ) ;
}

/*--------------------------------------------------------------*/
static void apple_dish( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 14 ) ;
	}
	if( pl_w->timer1 < 5 ){
		if( --pl_w->timer0 < 0 ){
			pl_w->timer0 = 2 ;
			shl1f_03_set2( pl_w, 0, pl_w->timer1 ) ;
			pl_w->timer1 ++ ;
		}
	}
	if( PL1F_LW( pl_w )->dish_hit ){
		if( PL1F_LW( pl_w )->dish_hit < 0 ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->timer0 = 32 ;
			pl_char_set( pl_w, Char_Super, 7 ) ;
			scr_work.quake_level = 2 ;
			scr_work.quake_flag = 1 ;
			em->catch_type = 16 ;
			em->atck_no = pl1f_at_apple_last ;
		} else {
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
			em->atck_no = pl1f_at_apple_dish ;
		}
		PL1F_LW( pl_w )->dish_hit = 0 ;
		em->dm_hit_w = pl_w ;
		catch_damage( pl_w ) ;
		combo_add( em, pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void apple_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void apple_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 && --pl_w->timer0 < 0 ){
		pl_w->routine_2 = 6 ;
		pl_char_set( pl_w, Char_Super, 18 ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		shl1f_03_set( pl_w, 1 ) ;
	}
}

/*==============================================================*/
static void sp_apple( PLAYER_WORK *pl_w )
{
	static void ( *apple_jmp[])( PLAYER_WORK *pl_w ) = {
		apple_init, apple_dash, apple_grab, apple_catch, apple_hit, apple_dish, apple_end, apple_miss,
	} ;

	apple_jmp[pl_w->routine_2]( pl_w ) ;
}

static void shyness_init( PLAYER_WORK *pl_w );
static void shyness_start( PLAYER_WORK *pl_w );
static void shyness_dash( PLAYER_WORK *pl_w );
static void shyness_3( PLAYER_WORK *pl_w );
static void shyness_4( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_shyness( PLAYER_WORK *pl_w )
{
	static void ( *shyness_jmp[])( PLAYER_WORK *pl_w ) = {
		shyness_init, shyness_start, shyness_dash, shyness_3, shyness_4,
	} ;

	shyness_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The dash's speed and slowing for each strength		*/
/*==============================================================*/
static void shyness_spd_set( PLAYER_WORK *pl_w )
{
	static Float shyness_spd[4] = {
		-10.0f, DOT_Y( 0.75f ),
		-DOT_X( 10.0f ), DOT_Y( 1.25f ),
	} ;

	pl_char_set( pl_w, Char_Special, 10 ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = shyness_spd[pl_w->atck_level * 2] ;
	pl_w->acc.x = shyness_spd[pl_w->atck_level * 2 + 1] ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*==============================================================*/
static void shyness_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	super_ground_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1f_at_shyness ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	pl_se_req( pl_w, 23 ) ;
}

/*--------------------------------------------------------------*/
static void shyness_start( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		shyness_spd_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shyness_dash( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) rate_clr_stand( pl_w ) ;
	else {
		if( pl_w->cg_type ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void shyness_3( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shyness_4( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

static void cheer_init( PLAYER_WORK *pl_w );
static void cheer_wait( PLAYER_WORK *pl_w );
static void cheer_keep( PLAYER_WORK *pl_w );
static void cheer_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A special move that also works in the air			*/
/*==============================================================*/
static void sp_cheer( PLAYER_WORK *pl_w )
{
	static void ( *cheer_jmp[])( PLAYER_WORK *pl_w ) = {
		cheer_init, cheer_wait, cheer_keep, cheer_end,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	cheer_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void cheer_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 5 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( pl_w->pl_sp_type == pl1f_sp_cheer_k ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.x /= 16.0f ;
			pl_w->spd.y /= 8.0f ;
			pl_w->acc.y /= 64.0f ;
			pl_w->acc.x = 0.0f ;
			pl_voice_req( pl_w, 43 ) ;
			pl_dir_set( pl_w ) ;
		} else super_ground_set( pl_w ) ;
		pl_w->atck_no = pl1f_at_cheer_k ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 13 ) ;
	} else {
		super_ground_set( pl_w ) ;
		pl_w->atck_no = pl1f_at_cheer ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cheer_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
		if( pl_w->pl_sp_type == pl1f_sp_cheer_k ) shl1f_00_spark_set( pl_w, 1, 0 ) ;
		else shl1f_00_spark_set( pl_w, 0, 0 ) ;
		PL1F_LW( pl_w )->spark_end_flag = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void cheer_keep( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( PL1F_LW( pl_w )->spark_end_flag ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_sp_type == pl1f_sp_cheer_k ) pl_char_set( pl_w, Char_Special, pl_w->atck_level + 17 ) ;
		else pl_char_set( pl_w, Char_Special, pl_w->atck_level + 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void cheer_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else rate_clr_stand( pl_w ) ;
	}
}

/*==============================================================*/
/*			The animation asks for an effect					*/
/*==============================================================*/
static void ef_ck( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	if( ( no = pl_w->cg_type ) ){
		pl_w->cg_type = 0 ;
		pls1f_00_set( pl_w, 1, no & 0x7f ) ;
	}
}

/*==============================================================*/
static void sp_missile( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = pl1f_at_missile ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 0 ) ;
		super_ground_set( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 0x80 ){
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
		pl_w->cg_type &= 0x7f ;
		shl1f_01_set( pl_w, pl_w->atck_level ) ;
	}
	ef_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) rate_clr_stand( pl_w ) ;
}

/*==============================================================*/
static void sp_happy( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = pl1f_at_missile ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 1 ) ;
		super_ground_set( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 0x80 ){
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
		pl_w->cg_type &= 0x7f ;
		shl1f_01_set( pl_w, pl_w->atck_level + 2 ) ;
	}
	ef_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) rate_clr_stand( pl_w ) ;
}

static void missile_j_init( PLAYER_WORK *pl_w );
static void missile_j_start( PLAYER_WORK *pl_w );
static void missile_j_up( PLAYER_WORK *pl_w );
static void missile_j_shot( PLAYER_WORK *pl_w );
static void missile_j_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_missile_j( PLAYER_WORK *pl_w )
{
	static void ( *missile_j_jmp[])( PLAYER_WORK *pl_w ) = {
		missile_j_init, missile_j_start, missile_j_up, missile_j_shot, missile_j_fall,
	} ;

	missile_j_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void missile_j_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1f_at_missile ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
	super_ground_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void missile_j_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
	}
}

/*--------------------------------------------------------------*/
static void missile_j_up( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void missile_j_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 0x80 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type &= 0x7f ;
		pl_w->spd.x = 1.6666666f ;
		pl_w->acc.x = -0.026041666f ;
		pl_w->spd.y = 7.5f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		shl1f_01_set( pl_w, pl_w->atck_level + 4 ) ;
	}
	ef_ck( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void missile_j_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	ef_ck( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

static void tellme_init( PLAYER_WORK *pl_w );
static void tellme_move( PLAYER_WORK *pl_w );
static void tellme_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_tellme( PLAYER_WORK *pl_w )
{
	static void ( *tellme_jmp[])( PLAYER_WORK *pl_w ) = {
		tellme_init, tellme_move, tellme_end,
	} ;

	tellme_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void tellme_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 21 ;
	pl_voice_req( pl_w, 43 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = -DOT_X( 5.0f ) ;
	if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
}

/*--------------------------------------------------------------*/
static void tellme_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 ){
		pl_w->push_ng_timer = 2 ;
		if( pl_w->cg_type ) return ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
}

/*--------------------------------------------------------------*/
static void tellme_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) rate_clr_stand( pl_w ) ;
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
	pl_voice_req( pl_w, 43 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = -10.0f ;
	pl_w->acc.x = 0.15625f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_dir_set( pl_w ) ;
	pl_w->atck_no = pl1f_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ){
		rate_clr_stand( pl_w ) ;
		return ;
	}
	spd = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -30.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
		pl_sp_ef_set( pl_w ) ;
	}
	if( pl_w->spd.x * spd < 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
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
		pl_voice_req( pl_w, 43 ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) rate_clr_stand( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The air kick that tags out							*/
/****************************************************************/
void pl1f_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl1f_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
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
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
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
/*			The throw: from the ground with the punch, or in	*/
/*			the air												*/
/****************************************************************/
PLAYER_WORK *pl1f_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	em = 0 ;
	if( ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) && !pl_w->atck_pk && pl_w->atck_level ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			if( pl_w->pl_stance == STANCE_AIR ) pl_w->catch_no = 1 ;
			else if( !pl_w->pl_stance ) pl_w->catch_no = 0 ;
			else em = 0 ;
		}
	}
	return em ;
}

/****************************************************************/
void pl1f_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !pl_w->catch_no ){
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 82.0f ) ;
	} else {
		ofs.x = -DOT_X( 37.0f ) ;
		ofs.y = DOT_Y( 51.0f ) ;
	}
	if( !( pl_w->atck_lever & KEY_FORWARD ) ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->disp_dir ^ 1 ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	if( PL_CK( pl_w->catch_w, PL_SERVBOT ) && !pl_w->catch_no ){
		pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 48.0f ) ;
	}
	scr_excep_set( pl_w, 6, 6 ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void catch_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pls1f_03_set( pl_w, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_throw( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		scr_excep_set( pl_w, 0, 0 ) ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = DOT_X( 1.75f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 5.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		if( !pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->atck_no = pl1f_at_throw ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = 45.0f ;
		ofs.y = DOT_Y( 68.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, 6 ) ;
		ofs.x = 45.0f ;
		ofs.y = DOT_Y( 72.0f ) ;
		hit_mark_set( pl_w, &ofs, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f && pl_w->ground_y > pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		fall_se_req( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
void pl1f_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		catch_wait, catch_throw, catch_fall,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_em_pos_set1( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_wait( PLAYER_WORK *pl_w )
{
	if( pl_w->timer0 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_end_flag & CG_END ) pl_w->timer0 = 0 ;
	}
}

/*==============================================================*/
static void hold_none( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			Being held											*/
/****************************************************************/
void pl1f_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold_none, hold_wait, hold_none, hold_none,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl1f_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1f_sp_hunting ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1f_sp_hunting ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1f_sp_hunting ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl1f_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1f_sp_hunting ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1f_sp_hunting ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1f_sp_hunting ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl1f_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1f_sp_missile ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1f_sp_cheer ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1f_sp_shyness ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl1f_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1f_sp_missile ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1f_sp_cheer ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1f_sp_shyness ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl1f.c" End of File	*/
