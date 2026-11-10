/* Psylocke (character 08): command checks, normal attacks, appearance, win poses, special moves
   and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl08_def.h"

/* sp_spin: two attacks for each level, and its speeds */
const Sint8 pl08_spin_atck_tbl[6] = { 0x40, 0x41, 0x44, 0x45, 0x42, 0x43 } ;
const Float pl08_spin_spd[3][4] = {
	{ 7.5f, DOT_X( 0.125f ), DOT_Y( 0.5f ), -0.06696428f },
	{ -DOT_X( 0.5f ), -0.00040690103f, DOT_Y( 9.0f ), -DOT_Y( 0.5f ) },
	{ DOT_X( 2.0f ), 0.104166664f, DOT_Y( 6.0f ), -DOT_Y( 0.375f ) }
} ;
/* sp_thrust and sp_ninjitsu, by direction: the attack, the animation and the angle; sp_ninjitsu's other copies' levels */
const Uint8 pl08_thrust_atck_tbl[16] = { 0x38, 0x38, 0x34, 0x38, 0x36, 0x37, 0x35, 0x38, 0x32, 0x39, 0x33, 0x38, 0x38, 0x38, 0x38, 0x38 } ;
const int pl08_thrust_char_tbl[16] = { 29, 27, 23, 23, 29, 28, 30, 23, 25, 26, 24, 23, 23, 23, 23, 23 } ;
const Uint8 pl08_thrust_dir_tbl[16] = { 0x1d, 0x10, 0x00, 0x00, 0x1d, 0x1a, 0x1e, 0x00, 0x1d, 0x1a, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00 } ;
const Uint8 pl08_thrust_ang_tbl[16] = { 0x00, 0x10, 0x00, 0x00, 0x08, 0x0c, 0x04, 0x00, 0x18, 0x14, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00 } ;
const Uint8 pl08_ninjitsu_lv_tbl[16] = { 1, 2, 3, 0, 2, 3, 0, 1, 3, 0, 1, 2, 6, 6, 14, 10 } ;
const int pl08_ninjitsu_char_tbl[16] = { 40, 38, 34, 40, 40, 39, 41, 40, 36, 37, 35, 38, 38, 38, 38, 38 } ;
const Uint8 pl08_ninjitsu_atck_tbl[16] = { 0x46, 0x46, 0x48, 0x46, 0x4a, 0x4b, 0x49, 0x46, 0x46, 0x4d, 0x47, 0x46, 0x46, 0x46, 0x46, 0x46 } ;

extern PLAYER_WORK *pls08_00_set( PLAYER_WORK *pl_w, Uint8 no );

void pl08_main( PLAYER_WORK *pl_w );
void pl08_com_ck( PLAYER_WORK *pl_w );
void pl08_atck_set( PLAYER_WORK *pl_w );
void pl08_atck( PLAYER_WORK *pl_w );
void pl08_dash( PLAYER_WORK *pl_w );
void pl08_noki( PLAYER_WORK *pl_w );
void pl08_appear( PLAYER_WORK *pl_w );
void pl08_win( PLAYER_WORK *pl_w );
void pl08_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl08_catch_ck( PLAYER_WORK *pl_w );
void pl08_catch_set( PLAYER_WORK *pl_w );
void pl08_catch( PLAYER_WORK *pl_w );
void pl08_hold( PLAYER_WORK *pl_w );
int pl08_delay_ck( PLAYER_WORK *pl_w );
void pl08_t_kick( PLAYER_WORK *pl_w );
void pl08_upper( PLAYER_WORK *pl_w );
void pl08_sp_unison( PLAYER_WORK *pl_w );
void pl08_sp_gattai( PLAYER_WORK *pl_w );
void pl08_sp_assist( PLAYER_WORK *pl_w );
void pl08_sp_zero( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_butterfly[] = { 3, COM_HYPER | COM_ALL_SW, pl08_sp_butterfly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Kocho-Gakure (hyper) */
static Uint16 com_blast[] = { 3, 0, pl08_sp_blast, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Psy-Blast */
static Uint16 com_spin[] = { 3, 0, pl08_sp_spin, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Psy-Blade */
static Uint16 com_thrust[] = { 3, COM_HYPER | COM_ALL_SW, pl08_sp_thrust, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Psy-Thrust (hyper) */
static Uint16 com_ninjitsu[] = { 5, 0, pl08_sp_ninjitsu, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_ATCK } ;	/* Ninjutsu */
static Uint16 com_chouhatsu[] = { 3, COM_GROUND, pl08_sp_chouhatsu, 0, KEY_DOWN, 0x0000, KEY_DOWN, KEY_KICK } ;	/* Taunt */
static Uint16 com_maelstrom[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl08_sp_maelstrom, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Psy-Maelstrom (hyper) */
static Uint16 com_leap[] = { 2, COM_GROUND, pl08_sp_leap, 0, KEY_LP, KEY_BACK, KEY_HK } ;	/* Leaping Slash */
/* not used */
static Uint16 com_leap_2[] = { 4, COM_GROUND, pl08_sp_leap, 0, KEY_LP, KEY_LP, KEY_HP, KEY_BACK, KEY_HK } ;	/* Leaping Slash */

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

static void ( *pl08_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl08_main,
	pl08_com_ck,
	pl08_atck_set,
	pl08_atck,
	pl08_dash,
	pl08_noki,
	pl08_appear,
	pl08_win,
	pl08_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl08_catch_ck,	/* returns a value */
	pl08_catch_set,
	pl08_catch,
	pl08_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl08_delay_ck,
	pl08_t_kick,
	pl08_upper,
	pl08_sp_unison,
	pl08_sp_gattai,
	pl08_sp_assist,
	pl08_sp_zero,
} ;

static Uint8 butterfly_ck( PLAYER_WORK *pl_w );
static Uint8 maelstrom_ck( PLAYER_WORK *pl_w );
static Uint8 thrust_ck( PLAYER_WORK *pl_w );
static Uint8 blast_ck( PLAYER_WORK *pl_w );
static Uint8 spin_ck( PLAYER_WORK *pl_w );
static Uint8 ninjitsu_ck( PLAYER_WORK *pl_w );
static Uint8 leap_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 thrust_delay_ck( PLAYER_WORK *pl_w );
static Uint8 maelstrom_delay_ck( PLAYER_WORK *pl_w );
static Uint8 butterfly_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl08_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl08_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl08_com_ck( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ){
		if( pl_w->pl_personal != 1 ){
			lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
			lw->spin_cnt = 1 ;
		}
		return ;
	}
	if( butterfly_ck( pl_w ) ) return ;
	if( maelstrom_ck( pl_w ) ) return ;
	if( thrust_ck( pl_w ) ) return ;
	if( blast_ck( pl_w ) ) return ;
	if( spin_ck( pl_w ) ) return ;
	if( ninjitsu_ck( pl_w ) ) return ;
	if( leap_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 butterfly_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_butterfly, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_butterfly ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blast_ck( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw = ( PL08_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_blast, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_blast ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 spin_ck( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_spin, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_spin ;
	act_st_set( pl_w, N_SPECIAL ) ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->spin_cnt = 1 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 thrust_ck( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_thrust, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		if( !pl_w->sjump_flag ) pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_thrust ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->atck_sw = pl_w->atck_lever ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ninjitsu_ck( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;
	Uint8 level ;

	if( !com_nml_ck( pl_w, com_ninjitsu, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		if( !pl_w->sjump_flag ) pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->atck_level += pl_w->atck_pk * 2 ;
	level = pl_w->atck_level ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_ninjitsu ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->atck_sw = pl_w->atck_lever ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->atck_level = level ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 5 ) ) return 0 ;
	pl_w->pl_sp_type = pl08_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 maelstrom_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_maelstrom, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_maelstrom ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 leap_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_leap, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl08_sp_leap ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl08_delay_ck( PLAYER_WORK *pl_w )
{
	if( thrust_delay_ck( pl_w ) || maelstrom_delay_ck( pl_w ) || butterfly_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 thrust_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_thrust, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		if( !pl_w->sjump_flag ) pl_w->jump_com_cnt ++ ;
	}
	pl_w->delay_sp_type = pl08_sp_thrust ;
	return 1 ;
}

/*==============================================================*/
static Uint8 maelstrom_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_maelstrom, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl08_sp_maelstrom ;
	return 1 ;
}

/*==============================================================*/
static Uint8 butterfly_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_butterfly, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl08_sp_butterfly ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl08_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Puts the second and third colour entries in the		*/
/*			shadow colours, once								*/
/*==============================================================*/
static void shadow_col_set( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	COL_WORK *ow ;
	Sint16 i, j ;
	Uint16 col ;

	ow = &col_work[pl_w->col_no] + 1 ;
	if( ( ow->col[1] & COL_ALPHA ) == 0xc000 ) return ;
	lw->color_flag = 1 ;
	for( i = 0 ; i < 2 ; i++ ){
		ow = &col_work[pl_w->col_no + i + 1] ;
		ow->trans_flag = 1 ;
		for( j = 1 ; j < 16 ; j++ ){
			col = ow->col[j] ;
			col &= 0x0fff ;
			col |= 0xc000 ;
			ow->col[j] = col ;
		}
	}
}

/****************************************************************/
/*			A button in Psy-Blade's dive starts the next part	*/
/****************************************************************/
void pl08_main( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	shadow_col_set( pl_w ) ;
	if( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == pl08_sp_spin && pl_w->routine_2 == 2
	 && pl_w->routine_1 == PL_NORM ){
		lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
		if( lw->spin_cnt && ( pl_w->pl_sw_new & KEY_ATCK ) ){
			com_sw_set( pl_w, &pl_w->com_w1 ) ;
			if( pl_w->atck_level ){
				pl_w->routine_2 = 0 ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				lw->spin_cnt -- ;
			}
		}
	}
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
void pl08_atck_set( PLAYER_WORK *pl_w )
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
		pl_voice_req( pl_w, 3 ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_voice_req( pl_w, 4 ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_voice_req( pl_w, 32 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		pl_voice_req( pl_w, 3 ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_voice_req( pl_w, 4 ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_voice_req( pl_w, 32 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy kick: a flip back with the stick back,	*/
/*			a hop forward with it forward						*/
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
		pl_w->routine_3 = 0 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_voice_req( pl_w, 3 ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_BACK ){
			pl_w->routine_3 = 1 ;
			if( pl_w->pl_dir ){
				pl_w->spd.x = -5.0f ;
				pl_w->acc.x = 0.104166664f ;
				pl_w->pos.x += -40.0f ;
			} else {
				pl_w->spd.x = 5.0f ;
				pl_w->acc.x = -0.104166664f ;
				pl_w->pos.x += 40.0f ;
			}
			pl_w->pos.y += DOT_Y( 31.0f ) ;
			pl_w->spd.y = 15.0f ;
			pl_w->acc.y = -1.7410713f ;
			pl_w->atck_no = pl08_at_back_hk ;
			pl_w->char_no.b.no = 5 ;
		} else if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->routine_3 = 2 ;
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl08_at_fwd_hk ;
			pl_w->spd.x = -DOT_X( 3.5f ) ;
			pl_w->acc.x = 0.052083332f ;
			if( pl_w->pl_dir ){
				pl_w->spd.x = DOT_X( 3.5f ) ;
				pl_w->acc.x = -0.052083332f ;
				pl_w->spd.y = DOT_Y( 5.0f ) ;
			} else {
				pl_w->spd.x = -DOT_X( 3.5f ) ;
				pl_w->acc.x = 0.052083332f ;
				pl_w->spd.y = DOT_Y( 5.0f ) ;
			}
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		} else {
			pl_w->routine_3 = 0 ;
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
		}
		pl_voice_req( pl_w, 4 ) ;
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
		pl_se_req( pl_w, SE_SWING_H ) ;
		break ;
	}
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
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_voice_req( pl_w, 3 ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_voice_req( pl_w, 4 ) ;
		pl_voice_req( pl_w, 32 ) ;
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
/*			The heavy kick, with the stick up, is another		*/
/*			attack												*/
/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_voice_req( pl_w, 3 ) ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_voice_req( pl_w, 4 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->atck_no = pl08_at_jup_hk ;
			pl_w->char_no.b.no = 5 ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
		}
		pl_voice_req( pl_w, 4 ) ;
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
void pl08_atck( PLAYER_WORK *pl_w )
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
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
	case 0:
	case 0xff:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
	case 0:
	case 0xff:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The flip back lands and kicks again; the hop		*/
/*			forward lands										*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	int sw ;
	switch( pl_w->nml_no ){
	case 2:
		switch( pl_w->routine_3 ){
		case 0:
			if( pl_char_move( pl_w ) < 0 ){
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_stand_ret( pl_w ) ;
			}
			break ;
		case 1:
			if( pl_char_move( pl_w ) < 0 ){
				pl_w->routine_3 = 3 ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_char_set( pl_w, Char_Stand_K, 3 ) ;
				pl_voice_req( pl_w, 3 ) ;
				pl_se_req( pl_w, SE_SWING_H ) ;
			} else if( pl_w->ground_y >= pl_w->pos.y ){
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->cg_ctr = 1 ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_se_req( pl_w, 52 ) ;
			}
			break ;
		case 2:
			if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
			else if( pl_w->ground_y > pl_w->pos.y ){
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->cg_ctr = 1 ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_se_req( pl_w, 52 ) ;
			}
			break ;
		case 3:
			if( pl_char_move( pl_w ) < 0 ){
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_stand_ret( pl_w ) ;
			} else {
				if( pl_w->cg_type != 2 ) goto chk ;
				sw = pl_w->pl_sw_new ;
				if( !( sw & KEY_FORWARD ) ) goto chk ;
				if( pl_w->pl_dir ){
					pl_w->spd.x = DOT_X( 11.0f ) ;
					pl_w->acc.x = -0.5208333f ;
					pl_w->pos.x -= -120.0f ;
				} else {
					pl_w->spd.x = -DOT_X( 11.0f ) ;
					pl_w->acc.x = 0.5208333f ;
					pl_w->pos.x -= 120.0f ;
				}
				pl_se_req( pl_w, SE_SWING_H ) ;
				pl_char_set( pl_w, Char_Stand_K, 6 ) ;
				return ;
			chk :
				if( pl_w->char_no.w == 0x0806 ) pl_w->push_ng_timer = 1 ;
			}
			break ;
		}
		break ;
	case 1:
	case 0:
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
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
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
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The heavy kick hits again							*/
/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	Sint8 old ;

	if( pl_w->nml_no != 2 ){
		if( pl_char_move( pl_w ) < 0 ) goto jump_ret ;
	} else {
		old = pl_w->cg_type ;
		if( pl_char_move( pl_w ) >= 0 || old >= 0 ) goto cg_ck ;
jump_ret :
		pl_jump_ret( pl_w ) ;
		return ;
cg_ck :
		if( pl_w->cg_type == 1 ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl08_at_jump_hk_2 ;
			HIT_RESET( pl_w ) ;
		}
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl08_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		if( pl_w->atck_pk == 0 ){
			pl_char_set( pl_w, Char_Extra, 8 ) ;
			pl_w->atck_no = pl08_at_upper ;
			HIT_RESET( pl_w ) ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_voice_req( pl_w, 32 ) ;
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
/*			The dash, a run forward								*/
/****************************************************************/
void pl08_dash( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl08_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? -20.0f : 20.0f ;
		noki_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl08_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
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
	pl_w->atck_no = pl08_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ) pl_w->cg_type = 0 ;
}

static void appear_wait( PLAYER_WORK *pl_w );
static void appear_init( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The entrance: she drops in from the side of the		*/
/*			screen												*/
/****************************************************************/
void pl08_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_wait, appear_init, appear_fall, appear_land,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	shadow_col_set( pl_w ) ;
	if( vs_sys_work.vs_st < VS_APPEAR ) pl_w->disp_flag = 0 ;
	else {
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = pl_w->pos.x ;
	pl_w->pos.y = 128.0f ;
	if( pl_w->pl_no ){
		pl_w->pos.x += DOT_X( 128.0f ) ;
		pl_w->spd.x = -DOT_X( 5.0f ) ;
	} else {
		pl_w->pos.x += -DOT_X( 128.0f ) ;
		pl_w->spd.x = DOT_X( 5.0f ) ;
	}
	pl_w->spd.y = -DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->pos.x = pl_w->timer0 ;
		pl_se_req( pl_w, 52 ) ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/****************************************************************/
/*			The win poses: one of four, but the first near		*/
/*			the stage's middle									*/
/****************************************************************/
void pl08_win( PLAYER_WORK *pl_w )
{
	int no ;

	if( pl_w->routine_2 ){
		if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
		else pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	switch( pl_w->type ){
	case 0:
		no = nlRand() ;
		no &= 3 ;
		if( scr_work.eye[0].x + -DOT_X( 112.0f ) > pl_w->pos.x || scr_work.eye[0].x + DOT_X( 112.0f ) < pl_w->pos.x ) no = 1 ;
		pl_char_set( pl_w, Char_Win, no ) ;
		break ;
	case 1:
		pl_char_set( pl_w, Char_Win, 4 ) ;
		break ;
	case 2:
		pl_char_set( pl_w, Char_Win, 3 ) ;
		break ;
	case 3:
	case 4:
		pl_char_set( pl_w, Char_Win, 5 ) ;
		break ;
	}
}

static void sp_blast( PLAYER_WORK *pl_w );
static void sp_spin( PLAYER_WORK *pl_w );
static void sp_butterfly( PLAYER_WORK *pl_w );
static void sp_thrust( PLAYER_WORK *pl_w );
static void sp_ninjitsu( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_maelstrom( PLAYER_WORK *pl_w );
static void sp_leap( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl08_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_blast, sp_spin, sp_butterfly, sp_thrust, sp_ninjitsu, sp_chouhatsu, sp_maelstrom, sp_leap,
		sp_snapback,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void blast_init( PLAYER_WORK *pl_w );
static void blast_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_blast( PLAYER_WORK *pl_w )
{
	static void ( *blast_jmp[])( PLAYER_WORK *pl_w ) = {
		blast_init, blast_move,
	} ;

	blast_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void blast_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_voice_req( pl_w, 20 ) ;
	pl_voice_req( pl_w, 31 ) ;
	pl_gauge_add( pl_w, 5 ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl08_at_blast_vc ;
			HIT_RESET( pl_w ) ;
		} else if( pl_w->atck_level ){
			pl_w->atck_no = pl08_at_jblast ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl08_at_blast_h ;
			HIT_RESET( pl_w ) ;
		}
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->char_no.b.no = 1 ;
		pl_w->spd.x /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
	} else {
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl08_at_blast_vc ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl_w->atck_level + 59 ;
			HIT_RESET( pl_w ) ;
		}
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_dir_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->char_no.b.no = 0 ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	blast_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void blast_move( PLAYER_WORK *pl_w )
{
	void *ew ;

	if( pl_w->pos.y < pl_w->ground_y ){
	} else {
		pl_brake_stop( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y += 2.142857f ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
			pl_jump_ret( pl_w ) ;
		} else {
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
		}
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		/* in the air, the shot's three air kinds */
		if( pl_w->pl_stance == STANCE_AIR ) ew = shl08_00_set( pl_w, pl_w->atck_level + 3 ) ;
		else ew = shl08_00_set( pl_w, pl_w->atck_level ) ;
	}
}

static void spin_init( PLAYER_WORK *pl_w );
static void spin_voice( PLAYER_WORK *pl_w );
static void spin_move( PLAYER_WORK *pl_w );
static void spin_fall( PLAYER_WORK *pl_w );
static void spin_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_spin( PLAYER_WORK *pl_w )
{
	static void ( *spin_jmp[])( PLAYER_WORK *pl_w ) = {
		spin_init, spin_voice, spin_move, spin_fall, spin_land,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	spin_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void spin_init( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	pl_w->routine_2 ++ ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	pl_w->pl_stop_now = 0 ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
	}
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl08_at_spin_vc ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl08_spin_atck_tbl[pl_w->atck_level * 2] ;
		HIT_RESET( pl_w ) ;
	}
gauge :
	pl_gauge_add( pl_w, 5 ) ;
	lw->follow_cnt = 0 ;
	lw->hit_cnt = 10 ;
	pl_w->char_no.b.no = pl_w->atck_level + 11 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
	lw->old_cg = pl_w->cg_type ;
	pl_air_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = pl08_spin_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = pl08_spin_spd[pl_w->atck_level][1] ;
	} else {
		pl_w->spd.x = -pl08_spin_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = -pl08_spin_spd[pl_w->atck_level][1] ;
	}
	pl_w->spd.y = pl08_spin_spd[pl_w->atck_level][2] ;
	pl_w->acc.y = pl08_spin_spd[pl_w->atck_level][3] ;
	spin_voice( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void spin_voice( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	pl_w->routine_2 ++ ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	if( lw->spin_cnt == 1 ) pl_voice_req( pl_w, 21 ) ;
	pl_voice_req( pl_w, 33 ) ;
}

/*==============================================================*/
/*			The follow-ups: each sets its attack, its			*/
/*			animation and its speeds from the tables			*/
/*==============================================================*/
static void spin_set_13( PLAYER_WORK *pl_w )
{
	pl_w->atck_no = pl08_spin_atck_tbl[2] ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = pl08_spin_spd[1][0] ;
		pl_w->acc.x = pl08_spin_spd[1][1] ;
	} else {
		pl_w->spd.x = -pl08_spin_spd[1][0] ;
		pl_w->acc.x = -pl08_spin_spd[1][1] ;
	}
	pl_w->spd.y = pl08_spin_spd[1][2] ;
	pl_w->acc.y = pl08_spin_spd[1][3] ;
}

/*==============================================================*/
static void spin_set_12( PLAYER_WORK *pl_w )
{
	pl_w->atck_no = pl08_spin_atck_tbl[4] ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = pl08_spin_spd[2][0] ;
		pl_w->acc.x = pl08_spin_spd[2][1] ;
	} else {
		pl_w->spd.x = -pl08_spin_spd[2][0] ;
		pl_w->acc.x = -pl08_spin_spd[2][1] ;
	}
	pl_w->spd.y = pl08_spin_spd[2][2] ;
	pl_w->acc.y = pl08_spin_spd[2][3] ;
}

/*==============================================================*/
static void spin_set_11( PLAYER_WORK *pl_w )
{
	pl_w->atck_no = pl08_spin_atck_tbl[0] ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = pl08_spin_spd[0][0] ;
		pl_w->acc.x = pl08_spin_spd[0][1] ;
	} else {
		pl_w->spd.x = -pl08_spin_spd[0][0] ;
		pl_w->acc.x = -pl08_spin_spd[0][1] ;
	}
	pl_w->spd.y = pl08_spin_spd[0][2] ;
	pl_w->acc.y = pl08_spin_spd[0][3] ;
}

/* the dive: a button pressed in time chains the next part */

/*--------------------------------------------------------------*/
static void spin_move( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->old_cg = pl_w->cg_type ;
	pl_char_move( pl_w ) ;
	if( lw->hit_cnt ){
		if( lw->old_cg != pl_w->cg_type ){
			if( pl_w->team_type == 3 ){
				pl_w->atck_no = pl08_at_spin_vc_2 ;
				HIT_RESET( pl_w ) ;
			} else {
				pl_w->atck_no = pl08_spin_atck_tbl[pl_w->atck_level * 2 + pl_w->cg_type] ;
				HIT_RESET( pl_w ) ;
			}
		}
		if( ( pl_w->hit_flag & ( HR_GUARD | HR_HIT ) ) == 0x80 ) lw->hit_cnt -- ;
		if( lw->follow_cnt < 2 ){
		level :
			if( pl_w->atck_level == 0 ){
				switch( lw->follow_cnt ){
				case 0:
					if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x40 ){
						pl_w->pl_stop_now = 0 ;
						lw->follow_cnt ++ ;
						spin_set_12( pl_w ) ;
					}
					break ;
				case 1:
					if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x20 ){
						pl_w->pl_stop_now = 0 ;
						lw->follow_cnt ++ ;
						spin_set_13( pl_w ) ;
					}
					break ;
				}
			} else {
				switch( lw->follow_cnt ){
				case 0:
					if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x40 ){
						pl_w->pl_stop_now = 0 ;
						lw->follow_cnt ++ ;
						spin_set_12( pl_w ) ;
					}
					break ;
				case 1:
					if( ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x40 ){
						pl_w->pl_stop_now = 0 ;
						lw->follow_cnt ++ ;
						spin_set_11( pl_w ) ;
					}
					break ;
				}
			}
		}
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void spin_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->sjump_flag = 0 ;
		pl_se_req( pl_w, 52 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 14 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void spin_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void butterfly_init( PLAYER_WORK *pl_w );
static void butterfly_wait( PLAYER_WORK *pl_w );
static void butterfly_shot( PLAYER_WORK *pl_w );
static void butterfly_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_butterfly( PLAYER_WORK *pl_w )
{
	static void ( *butterfly_jmp[])( PLAYER_WORK *pl_w ) = {
		butterfly_init, butterfly_wait, butterfly_shot, butterfly_end,
	} ;

	butterfly_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void butterfly_init( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
	pl_w->atck_no = pl08_at_butterfly ;
	HIT_RESET( pl_w ) ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->blade_timer = 0 ;
	lw->blade_cnt = 0 ;
	pl_w->timer0 = 80 ;
	pl_w->timer1 = 0 ;
	if( pl_w->pl_stance == STANCE_STAND ) pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void butterfly_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, 23 ) ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 104.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The blades: four at a time, every twelve frames,	*/
/*			turning round her									*/
/*==============================================================*/
static void butterfly_shot( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *ew ;
	Float x, y ;
	Sint16 i ;
	Sint16 no ;
	Sint16 ang ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 56 ;
		pl_char_set( pl_w, Char_Special, 16 ) ;
	} else {
		pl_char_move( pl_w ) ;
		lw->blade_timer -- ;
		if( lw->blade_timer > 0 ) return ;
		lw->blade_timer = 12 ;
		x = -DOT_X( 8.0f ) ;
		y = DOT_Y( 106.0f ) ;
		if( pl_w->pl_dir ) x = DOT_X( 8.0f ) ;
		lw->blade_cnt ++ ;
		if( lw->blade_cnt & 1 ) no = 4 ;
		else no = 0 ;
		ang = 0 ;
		for( i = 0 ; i < 4 ; i ++ ){
			ew = shl08_01_set( pl_w, pl_w->timer1 / 2 ) ;
			if( !ew ) break ;
			ew->pos.x = x + pl_w->pos.x ;
			ew->pos.y = y + pl_w->pos.y ;
			ew->timer1 = no ;
			ew->dir = ( pl_w->timer1 + ang ) & 31 ;
			ang += 8 ;
		}
		if( pl_w->pl_dir ) pl_w->timer1 -= 2 ;
		else pl_w->timer1 += 2 ;
		pl_w->timer1 &= 31 ;
	}
}

/*--------------------------------------------------------------*/
static void butterfly_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->sjump_cnt = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->jump_com_cnt ++ ;
		pl_w->jump_dash_cnt ++ ;
		pl_w->air_jump_cnt = 127 ;
		pl_jump_ret( pl_w ) ;
	}
}

static void thrust_init( PLAYER_WORK *pl_w );
static void thrust_wait( PLAYER_WORK *pl_w );
static void thrust_move( PLAYER_WORK *pl_w );
static void thrust_ret( PLAYER_WORK *pl_w );
static void thrust_fall( PLAYER_WORK *pl_w );
static void thrust_land( PLAYER_WORK *pl_w );
static void thrust_dir_set( PLAYER_WORK *pl_w );
static void thrust_end( PLAYER_WORK *pl_w );
static void thrust_dash( PLAYER_WORK *pl_w );
static void sp_spd_set( PLAYER_WORK *pl_w, Sint16 spd, Float *x, Float *y );

/*==============================================================*/
static void sp_thrust( PLAYER_WORK *pl_w )
{
	static void ( *thrust_jmp[])( PLAYER_WORK *pl_w ) = {
		thrust_init, thrust_wait, thrust_move, thrust_ret, thrust_fall, thrust_land,
	} ;

	pl_w->push_ng_timer = 1 ;
	thrust_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void thrust_init( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_voice_req( pl_w, 34 ) ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->thrust_hit_cnt = 0 ;
	lw->dash_cnt = 2 ;
	lw->dash_add = 15 ;
	lw->thrust_dir = 8 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl08_at_thrust ;
	HIT_RESET( pl_w ) ;
	pl_air_set( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ) pl_char_set( pl_w, Char_Special, 31 ) ;
	else {
		pl_char_set( pl_w, Char_Special, 32 ) ;
		pl_sp_ef_set( pl_w ) ;
	}
	thrust_wait( pl_w ) ;
}

/*==============================================================*/
/*			The stick's direction, 0 to 15, from its four		*/
/*			bits												*/
/*==============================================================*/
static int lever_no( PLAYER_WORK *pl_w, Uint16 sw )
{
	int no ;

	sw &= 0x3c00 ;
	no = ( sw & 0x3000 ) / 0x1000 ;
	no |= ( sw & 0x0c00 ) / 0x100 ;
	return no ;
}

/*--------------------------------------------------------------*/
static void thrust_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	PL08_LOCAL_W *lw ;
	Uint16 sw ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->stop_move_flag = 0 ;
			pl_w->stop_move_timer = 0 ;
			if( pl_w->pos.y > pl_w->ground_y ){
				ofs.x = -DOT_X( 2.0f ) ;
				ofs.y = DOT_Y( 106.0f ) ;
			} else {
				ofs.x = -DOT_X( 28.0f ) ;
				ofs.y = 225.0f ;
			}
			ofs.z = 0.0f ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
		return ;
	}
	pl_voice_req( pl_w, 22 ) ;
	pl_voice_req( pl_w, 34 ) ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->dash_timer = lw->dash_add + 30 ;
	lw->dash_add = 0 ;
	if( pl_w->cpu_flag ) sw = lw->atck_sw ;
	else sw = pl_w->pl_sw_new ;
	sw &= 0x3c00 ;
	if( sw ){
		lw->thrust_dir = lever_no( pl_w, sw ) ;
		lw->thrust_dir &= 15 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	thrust_dir_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void thrust_move( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;
	Float y ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	y = pl_w->ground_y + 896.0f ;
	if( pl_w->pos.y > y ) pl_w->pos.y = y ;
	if( pl_w->hit_flag & HR_GUARD ){
		lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
		lw->thrust_hit_cnt ++ ;
		if( lw->dash_cnt != 2 ){
			thrust_end( pl_w ) ;
			return ;
		}
	}
	thrust_dash( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void thrust_ret( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.y = -DOT_Y( 0.375f ) ;
		pl_w->jump_dir = -1 ;
		pl_sjump_ret( pl_w ) ;
		pl_land_ef_set( pl_w ) ;
	} else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void thrust_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.x = 0.0f ;
		if( pl_land_ck( pl_w ) ){
			pl_se_req( pl_w, 52 ) ;
			pl_w->routine_2 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Special, 14 ) ;
			pl_land_ef_set( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void thrust_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A dash in the direction chosen						*/
/*==============================================================*/
static void thrust_dir_set( PLAYER_WORK *pl_w )
{
	Float vx, vy ;
	Uint8 dir ;
	PL08_LOCAL_W *lw ;

	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	pl_w->atck_no = pl08_thrust_atck_tbl[lw->thrust_dir] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl08_thrust_char_tbl[lw->thrust_dir] ) ;
	pl_w->dir = pl08_thrust_dir_tbl[lw->thrust_dir] ;
	dir = pl_w->dir ;
	pl_w->dir = pl08_thrust_ang_tbl[lw->thrust_dir] ;
	sp_spd_set( pl_w, 1000, &vx, &vy ) ;
	pl_w->dir = dir ;
	if( pl_w->pl_dir ) vx = -vx ;
	pl_w->spd.x = vx ;
	pl_w->spd.y = vy ;
}

/*==============================================================*/
static void thrust_end( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 += 2 ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -2.5f ;
		pl_w->acc.x = 0.026041666f ;
	} else {
		pl_w->spd.x = 2.5f ;
		pl_w->acc.x = -0.026041666f ;
	}
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set2( pl_w, Char_Jump, 2, 1 ) ;
}

/*==============================================================*/
static void thrust_dash( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;
	Uint32 rnd ;

	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	lw->dash_timer -- ;
	if( lw->dash_timer > 0 ){
		if( lw->dash_timer < 10 && ( pl_w->pl_sw_new & KEY_ATCK ) ) pl_w->routine_3 = 1 ;
		rnd = nlRand() ;
		rnd &= 3 ;
		if( !rnd ) pls08_01_set( pl_w, 0xff ) ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl08_thrust_atck_tbl[lw->thrust_dir] ;
			HIT_RESET( pl_w ) ;
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->ground_y < pl_w->pos.y ) return ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		switch( lw->thrust_dir ){
		case 9:
		case 1:
			lw->thrust_dir = lever_no( pl_w, 0x800 ) ;
			thrust_dir_set( pl_w ) ;
			break ;
		case 5:
			lw->thrust_dir = lever_no( pl_w, 0x400 ) ;
			thrust_dir_set( pl_w ) ;
			break ;
		}
	} else {
		if( !pl_w->routine_3 ) thrust_end( pl_w ) ;
		else {
			lw->dash_cnt -- ;
			if( !lw->dash_cnt ) thrust_end( pl_w ) ;
			else {
				pl_char_set( pl_w, Char_Special, 33 ) ;
				pl_w->atck_no = pl08_thrust_atck_tbl[lw->thrust_dir] ;
				HIT_RESET( pl_w ) ;
				pl_w->routine_2 -- ;
				thrust_wait( pl_w ) ;
			}
		}
	}
}

static void ninjitsu_init( PLAYER_WORK *pl_w );
static void ninjitsu_vanish( PLAYER_WORK *pl_w );
static void ninjitsu_copy( PLAYER_WORK *pl_w );
static void ninjitsu_wait( PLAYER_WORK *pl_w );
static void ninjitsu_appear( PLAYER_WORK *pl_w );
static void ninjitsu_dash( PLAYER_WORK *pl_w );
static void ninjitsu_jump( PLAYER_WORK *pl_w );
static void ninjitsu_land( PLAYER_WORK *pl_w );
static void ninjitsu_dir_set( PLAYER_WORK *pl_w );
static void ninjitsu_fall( PLAYER_WORK *pl_w );
static void ninjitsu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_ninjitsu( PLAYER_WORK *pl_w )
{
	static void ( *ninjitsu_jmp[])( PLAYER_WORK *pl_w ) = {
		ninjitsu_init, ninjitsu_vanish, ninjitsu_copy, ninjitsu_wait, ninjitsu_appear, ninjitsu_dash, ninjitsu_jump, ninjitsu_land,
	} ;

	ninjitsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ninjitsu_init( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw = ( PL08_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_voice_req( pl_w, 30 ) ;
	lw->warp_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pos.x ;
	pl_w->spd.y = pl_w->pos.y ;
	if( pl_w->target_w->disp_dir ) pl_w->acc.x += pl_w->target_w->pos.x + -160.0f ;
	else pl_w->acc.x += pl_w->target_w->pos.x + 160.0f ;
	pl_w->acc.y = pl_w->target_w->pos.y ;
	pl_char_set( pl_w, Char_Special, 43 ) ;
}

/*--------------------------------------------------------------*/
static void ninjitsu_vanish( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->push_ng_timer = 3 ;
		pl_w->nml_catch_ng_timer = 3 ;
		pl_w->catch_ng_timer = 3 ;
		pl_w->timer0 = 1 ;
		pl_w->disp_flag = 0 ;
		pl_gauge_add( pl_w, 5 ) ;
	}
}

/*==============================================================*/
/*			The copies: one for the level, then the other		*/
/*			three												*/
/*==============================================================*/
static void ninjitsu_copy( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	const Uint8 *tbl ;

	pl_w->push_ng_timer = 2 ;
	pl_w->nml_catch_ng_timer = 2 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 > 0 ) return ;
	pl_w->routine_2 ++ ;
	if( ( ew = pls08_00_set( pl_w, 1 ) ) ){
		ew->atck_level = pl_w->atck_level ;
		tbl = &pl08_ninjitsu_lv_tbl[pl_w->atck_level * 3] ;
		if( ( ew = pls08_00_set( pl_w, 0 ) ) ) ew->atck_level = *tbl++ ;
		if( ( ew = pls08_00_set( pl_w, 0 ) ) ) ew->atck_level = *tbl++ ;
		if( ( ew = pls08_00_set( pl_w, 0 ) ) ) ew->atck_level = *tbl ;
	}
}

/*--------------------------------------------------------------*/
static void ninjitsu_wait( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	if( lw->warp_flag ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Special, 44 ) ;
		pl_voice_req( pl_w, 35 ) ;
	} else {
		pl_w->push_ng_timer = 2 ;
		pl_w->nml_catch_ng_timer = 2 ;
		pl_w->catch_ng_timer = 2 ;
	}
}

/*--------------------------------------------------------------*/
static void ninjitsu_appear( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;
	Uint16 sw ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->pl_sw_now & KEY_ATCK ) pl_w->routine_3 = 1 ;
		return ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	if( !pl_w->cpu_flag ){
		if( !pl_w->routine_3 ){
			ninjitsu_fall( pl_w ) ;
			return ;
		}
		sw = pl_w->pl_sw_new ;
	} else sw = lw->atck_sw ;
	sw &= 0x3c00 ;
	if( sw ){
		pl_w->routine_2 ++ ;
		lw->dash_timer = 16 ;
		lw->thrust_dir = lever_no( pl_w, sw ) ;
		ninjitsu_dir_set( pl_w ) ;
		pl_voice_req( pl_w, 34 ) ;
	} else ninjitsu_fall( pl_w ) ;
}

/*==============================================================*/
static void ninjitsu_dir_set( PLAYER_WORK *pl_w )
{
	Float vx, vy ;
	Uint8 dir ;
	PL08_LOCAL_W *lw ;

	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	pl_w->atck_no = pl08_ninjitsu_atck_tbl[lw->thrust_dir] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl08_ninjitsu_char_tbl[lw->thrust_dir] ) ;
	pl_w->dir = pl08_thrust_dir_tbl[lw->thrust_dir] ;
	dir = pl_w->dir ;
	pl_w->dir = pl08_thrust_ang_tbl[lw->thrust_dir] ;
	sp_spd_set( pl_w, 1000, &vx, &vy ) ;
	pl_w->dir = dir ;
	if( pl_w->pl_dir ) vx = -vx ;
	pl_w->spd.x = vx ;
	pl_w->spd.y = vy ;
}

/*==============================================================*/
static void ninjitsu_fall( PLAYER_WORK *pl_w )
{
	pl_w->sjump_cnt = 0 ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_com_cnt ++ ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->air_jump_cnt = 0 ;
	pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void ninjitsu_end( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -DOT_X( 0.5f ) ;
		pl_w->acc.x = 0.0032552083f ;
	} else {
		pl_w->spd.x = DOT_X( 0.5f ) ;
		pl_w->acc.x = -0.0032552083f ;
	}
	pl_w->spd.y = DOT_Y( 6.5f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set2( pl_w, Char_Jump, 2, 1 ) ;
}

/*--------------------------------------------------------------*/
static void ninjitsu_dash( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;
	Float y ;

	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	y = pl_w->ground_y + 896.0f ;
	if( pl_w->pos.y > y ) pl_w->pos.y = y ;
	if( pl_w->hit_flag & HR_GUARD ){
		pl_w->routine_2 ++ ;
		ninjitsu_end( pl_w ) ;
	} else {
		lw->dash_timer -- ;
		if( lw->dash_timer <= 0 ){
			if( pl_w->hit_flag & HR_HIT ){
				pl_w->routine_2 ++ ;
				ninjitsu_end( pl_w ) ;
			} else {
				pl_w->sjump_cnt = 0 ;
				pl_w->sjump_flag = 0 ;
				pl_w->jump_com_cnt ++ ;
				pl_w->jump_dash_cnt = 1 ;
				pl_w->air_jump_cnt = 127 ;
				pl_jump_ret( pl_w ) ;
			}
		} else {
			if( !( nlRand() & 3 ) ) pls08_01_set( pl_w, 0xff ) ;
			pl_char_move( pl_w ) ;
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
			pl_air_set( pl_w ) ;
			if( pl_w->pos.y < pl_w->ground_y ){
				pl_w->routine_2 = 7 ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->pl_stance = STANCE_STAND ;
				pl_char_set( pl_w, Char_Special, 14 ) ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void ninjitsu_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->spd.x = 0.0f ;
		if( pl_land_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Special, 14 ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ninjitsu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void maelstrom_init( PLAYER_WORK *pl_w );
static void maelstrom_wait( PLAYER_WORK *pl_w );
static void maelstrom_voice( PLAYER_WORK *pl_w );
static void maelstrom_up( PLAYER_WORK *pl_w );
static void maelstrom_air( PLAYER_WORK *pl_w );
static void maelstrom_fall( PLAYER_WORK *pl_w );
static void maelstrom_land( PLAYER_WORK *pl_w );
static void maelstrom_dir_set( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_maelstrom( PLAYER_WORK *pl_w )
{
	static void ( *maelstrom_jmp[])( PLAYER_WORK *pl_w ) = {
		maelstrom_init, maelstrom_wait, maelstrom_voice, maelstrom_up, maelstrom_air, maelstrom_fall, maelstrom_land,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	maelstrom_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void maelstrom_init( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
	pl_w->routine_2 ++ ;
	pl_sp_ef_set( pl_w ) ;
	lw->hit_cnt = 100 ;
	pl_char_set( pl_w, Char_Special, 46 ) ;
	pl_air_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl08_at_maelstrom ;
	HIT_RESET( pl_w ) ;
	maelstrom_dir_set( pl_w ) ;
}

/*==============================================================*/
/*			Aim at the opponent: one of four angles				*/
/*==============================================================*/
static void maelstrom_dir_set( PLAYER_WORK *pl_w )
{
	int dir, ofs ;

	dir = -8 ;
	ofs = -4 ;
	if( !pl_w->target_w->pl_dir ){
		if( pl_w->pos.x >= pl_w->target_w->pos.x ){
			dir = 8 ;
			ofs = 4 ;
		}
	} else if( pl_w->pos.x <= pl_w->target_w->pos.x ){
		if( 1 ){
			dir = 8 ;
			ofs = 4 ;
		} else {
			dir = 8 ;
			ofs = 4 ;
		}
	}
	dir += 16 ;
	if( pl_w->target_w->pos.y <= pl_w->pos.y + DOT_Y( 48.0f ) ) dir += ofs ;
	pl_w->dir = dir ;
}

/*--------------------------------------------------------------*/
static void maelstrom_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	maelstrom_dir_set( pl_w ) ;
	pl_char_move( pl_w ) ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 12.0f ) ;
		pl_w->acc.y = -0.46875f ;
		ofs.x = -DOT_X( 4.0f ) ;
		ofs.y = 210.0f ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void maelstrom_voice( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_voice_req( pl_w, 24 ) ;
}

/*--------------------------------------------------------------*/
static void maelstrom_up( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->push_ng_timer = 1 ;
	maelstrom_dir_set( pl_w ) ;
	if( pl_w->hit_flag ){
		sw = ( pl_w->pl_sw_now | pl_w->pl_stop_now ) & 0x360 ;
		if( sw ){
			pl_w->pl_stop_new = 0 ;
			pl_w->pl_stop_now = 0 ;
			pl_w->cg_ctr = 1 ;
		}
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		/* the last hit is reset twice, so it counts as two attacks */
		if( DOT_Y( 2.0f ) >= pl_w->spd.y ){
			pl_w->atck_no = pl08_at_maelstrom_end ;
			HIT_RESET( pl_w ) ;
			pl_w->routine_2 ++ ;
		} else pl_w->atck_no = pl08_at_maelstrom ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void maelstrom_air( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y <= 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void maelstrom_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->sjump_flag = 0 ;
		pl_se_req( pl_w, 52 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void maelstrom_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void leap_init( PLAYER_WORK *pl_w );
static void leap_move( PLAYER_WORK *pl_w );
static void leap_fall( PLAYER_WORK *pl_w );
static void leap_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_leap( PLAYER_WORK *pl_w )
{
	static void ( *leap_jmp[])( PLAYER_WORK *pl_w ) = {
		leap_init, leap_move, leap_fall, leap_land,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	leap_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void leap_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_air_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl08_at_leap ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 47 ) ;
	if( pl_w->pl_dir ){
		pl_w->pos.x += 80.0f ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = -0.1171875f ;
	} else {
		pl_w->pos.x += -80.0f ;
		pl_w->spd.x = -DOT_X( 4.0f ) ;
		pl_w->acc.x = 0.1171875f ;
	}
	pl_w->spd.y = DOT_Y( 9.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
}

/*--------------------------------------------------------------*/
static void leap_move( PLAYER_WORK *pl_w )
{
	Sint8 old ;

	old = pl_w->cg_type ;
	if( pl_char_move( pl_w ) >= 0 && old < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	} else if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl08_at_leap ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void leap_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y < pl_w->pos.y ) return ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_se_req( pl_w, 52 ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 14 ) ;
	pl_land_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void leap_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl08_at_chouhatsu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_end( PLAYER_WORK *pl_w )
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
	pl_color_set( pl_w, 0 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl08_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 51 ) ;
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
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_voice_req( pl_w, 32 ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl08_catch_ck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
	} else if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void catch3_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl08_catch_ck chose			*/
/****************************************************************/
void pl08_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, nml_p_set, nml_p_set, catch3_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->disp_dir ^= 1 ;
	pl_w->pl_dir ^= 1 ;
	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -80.0f ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 16.0f ) ;
	ofs.y = DOT_Y( 48.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->routine_2 = 0 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void catch3_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = 0 ;
	pl_w->hit_stop = 10 ;
	scr_excep_set( pl_w, 5, 5 ) ;
}

static void nml_k_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );
static void catch3( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl08_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, nml_p_catch, jump_k_catch, catch3,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_k_catch_init( PLAYER_WORK *pl_w );
static void nml_k_catch_throw( PLAYER_WORK *pl_w );
static void nml_k_catch_turn( PLAYER_WORK *pl_w );
static void nml_k_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch_init, nml_k_catch_throw, nml_k_catch_turn, nml_k_catch_end,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void nml_k_catch_throw( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->pl_dir = pl_w->pl_dir ;
		pl_w->catch_w->atck_no = pl08_at_throw_k ;
		pl_w->catch_w->pl_stance = STANCE_AIR ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_turn( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_dir ^= 1 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void nml_p_catch_init( PLAYER_WORK *pl_w );
static void nml_p_catch_move( PLAYER_WORK *pl_w );
static void nml_p_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch_init, nml_p_catch_move, nml_p_catch_end,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 12.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->pl_stance = STANCE_AIR ;
}

/*--------------------------------------------------------------*/
static void nml_p_catch_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type <= 0 ) pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_jump_ret( pl_w ) ;
	} else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->atck_no = pl08_at_throw_p ;
		pl_w->catch_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jump_k_catch_init( PLAYER_WORK *pl_w );
static void jump_k_catch_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_k_catch_init, jump_k_catch_move,
	} ;

	jump_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_k_catch_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 5.0f ) : -DOT_X( 5.0f ) ;
	pl_w->acc.x = 0.0f ;
	if( pl_w->acc.y > 0.0f ){
		pl_w->acc.y /= DOT_Y( 2.0f ) ;
		pl_w->acc.y = -pl_w->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void jump_k_catch_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pl_stance == STANCE_AIR && pl_land_ck( pl_w ) ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_se_req( pl_w, 52 ) ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 2 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type < 0 ){
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->catch_w->atck_no = pl08_at_throw_p ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		scr_excep_clr() ;
	}
}

static void catch3_init( PLAYER_WORK *pl_w );
static void catch3_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
static void catch3( PLAYER_WORK *pl_w )
{
	static void ( *catch3_jmp[])( PLAYER_WORK *pl_w ) = {
		catch3_init, catch3_throw,
	} ;

	catch3_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void catch3_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void catch3_throw( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->target_w->dm_hit_w = pl_w ;
		pl_w->target_w->catch_type = 1 ;
		pl_w->target_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->target_w->atck_no = pl08_at_throw_3 ;
		scr_excep_clr() ;
		pl_w->hit_stop = pl_w->target_w->hit_stop = 8 ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = 5.0f ;
			pl_w->acc.x = 0.104166664f ;
		} else {
			pl_w->spd.x = -5.0f ;
			pl_w->acc.x = -0.104166664f ;
		}
		pl_w->spd.y = DOT_Y( 5.0f ) ;
		pl_w->acc.y = -0.50223213f ;
		pl_w->pl_stance = STANCE_AIR ;
	}
}

/*==============================================================*/
/*			A speed of spd at the angle in dir, added to the	*/
/*			acceleration										*/
/*==============================================================*/
static void sp_spd_set( PLAYER_WORK *pl_w, Sint16 spd, Float *x, Float *y )
{
	Float s ;
	Sint16 ang ;

	s = spd * 256.0f ;
	ang = ( ( 40 - pl_w->dir ) & 31 ) << 11 ;
	*x = s * nlCos( ang ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f + pl_w->acc.x ;
	*y = s * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f + pl_w->acc.y ;
}

static void hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl08_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl08_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl08_sp_maelstrom ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl08_sp_butterfly ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl08_sp_thrust ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl08_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl08_sp_maelstrom ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl08_sp_butterfly ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl08_sp_thrust ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl08_sp_assist( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl08_sp_spin ;
		lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
		lw->spin_cnt = 1 ;
		break ;
	case 1:
	case 2:
		pl_w->pl_sp_type = pl08_sp_blast ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl08_sp_zero( PLAYER_WORK *pl_w )
{
	PL08_LOCAL_W *lw ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 1:
		pl_w->pl_sp_type = pl08_sp_blast ;
		break ;
	case 0:
	case 2:
		pl_w->pl_sp_type = pl08_sp_spin ;
		lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
		lw->spin_cnt = 1 ;
		break ;
	}
	pl_w->atck_level = 1 ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl08.c" End of File	*/
