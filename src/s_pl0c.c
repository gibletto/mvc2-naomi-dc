/* Spider-Man (character 0c): command checks, normal attacks, appearance, win and lose poses,
   special moves and hyper combos, the web swing, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "o_sub1.h"
#include "pl0c_def.h"

/* sp_sting's speeds in 16.16, by kind and level: x, y and their pulls */
const Sint32 pl0c_sting_spd_tbl[2][2][4] = {
	{ { -0x0006c000, 0x00001000, 0x00070000, -0x00008000 }, { -0x00104000, 0x00020000, 0x000b0000, -0x00008000 } },
	{ { -0x00030000, 0x00004000, 0x00060000, -0x00009000 }, { -0x000b4000, 0x00018000, 0x000a0000, -0x00009000 } }
} ;

void pl0c_main( PLAYER_WORK *pl_w );
void pl0c_com_ck( PLAYER_WORK *pl_w );
void pl0c_atck_set( PLAYER_WORK *pl_w );
void pl0c_atck( PLAYER_WORK *pl_w );
void pl0c_dash( PLAYER_WORK *pl_w );
void pl0c_noki( PLAYER_WORK *pl_w );
void pl0c_appear( PLAYER_WORK *pl_w );
void pl0c_win( PLAYER_WORK *pl_w );
void pl0c_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl0c_catch_ck( PLAYER_WORK *pl_w );
void pl0c_catch_set( PLAYER_WORK *pl_w );
void pl0c_catch( PLAYER_WORK *pl_w );
void pl0c_hold( PLAYER_WORK *pl_w );
int pl0c_delay_ck( PLAYER_WORK *pl_w );
void pl0c_t_kick( PLAYER_WORK *pl_w );
void pl0c_upper( PLAYER_WORK *pl_w );
void pl0c_sp_unison( PLAYER_WORK *pl_w );
void pl0c_sp_gattai( PLAYER_WORK *pl_w );
void pl0c_sp_assist( PLAYER_WORK *pl_w );
void pl0c_sp_zero( PLAYER_WORK *pl_w );
void pl0c_web_swing( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, then jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;

static Uint8 webthrow_dir_tbl[2] = { 24, 28 } ;

/* where the web leaves the hand, x then y, picked by the animation */
static Sint16 sp_ofs_tbl[72] = {
	 -76,  54,  -63,  86,  -20, 109,  -56,  67,  -36,  69,
	 -21,  75,  -50, 102,  -48,  90,  -27,  85,  -26,  85,
	 -26,  85,  -25,  86,  -65, 128,  -64, 118,  -43, 111,
	 -44, 112,  -42, 112,  -40, 113,    5,  50,   16,  57,
	  27,  79,    7,  94,  -23,  91,  -28,  71,  -13,  54,
	 -51,  45,   15,  28,   45,  49,   58,  85,   40, 120,
	 -50,  94,  -59,  35,    0,    0,  -36, 110,
} ;

/* the hyper combo sp_maximum's dash by the stick: its direction and its attack */
static Uint8 maximum_dir_tbl[8][2] = {
	{ 21, 0 }, { 16, 2 }, { 21, 0 }, { 21, 0 }, {  8, 1 }, {  8, 1 }, { 16, 2 }, { 21, 0 },
} ;
/* the edges a dash in each eighth of a turn can't reach */
static Sint8 maximum_wall_tbl[8] = { -9, -10, -2, -6, -5, -7, -3, -11 } ;
static Sint16 catch_ofs_tbl[6] = { -64, 72, -56, 8, -24, 22 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_sting[] = { 3, COM_GROUND, pl0c_sp_sting, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Spider Sting */
static Uint16 com_webball[] = { 3, 0, pl0c_sp_webball, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Web Ball */
static Uint16 com_swing[] = { 3, 0, pl0c_sp_swing, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Web Swing */
static Uint16 com_webthrow[] = { 5, COM_GROUND, pl0c_sp_webthrow, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Web Throw */
static Uint16 com_maximum[] = { 3, COM_HYPER | COM_ALL_SW, pl0c_sp_maximum, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Maximum Spider */
static Uint16 com_crawler[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0c_sp_crawler, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Crawler Assault */
static Uint16 com_hwebthrow[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0c_sp_hwebthrow, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Ultimate Web Throw */

static void ( *pl0c_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl0c_main,
	pl0c_com_ck,
	pl0c_atck_set,
	pl0c_atck,
	pl0c_dash,
	pl0c_noki,
	pl0c_appear,
	pl0c_win,
	pl0c_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl0c_catch_ck,	/* returns a value */
	pl0c_catch_set,
	pl0c_catch,
	pl0c_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0c_delay_ck,
	pl0c_t_kick,
	pl0c_upper,
	pl0c_sp_unison,
	pl0c_sp_gattai,
	pl0c_sp_assist,
	pl0c_sp_zero,
	0,
	pl0c_web_swing,
} ;

static Uint8 sting_ck( PLAYER_WORK *pl_w );
static Uint8 webball_ck( PLAYER_WORK *pl_w );
static Uint8 swing_ck( PLAYER_WORK *pl_w );
static Uint8 webthrow_ck( PLAYER_WORK *pl_w );
static Uint8 maximum_ck( PLAYER_WORK *pl_w );
static Uint8 crawler_ck( PLAYER_WORK *pl_w );
static Uint8 hwebthrow_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static int maximum_delay_ck( PLAYER_WORK *pl_w );
static int crawler_delay_ck( PLAYER_WORK *pl_w );
static int hwebthrow_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0c_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl0c_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl0c_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hwebthrow_ck( pl_w ) ) return ;
	if( webthrow_ck( pl_w ) ) return ;
	if( maximum_ck( pl_w ) ) return ;
	if( crawler_ck( pl_w ) ) return ;
	if( sting_ck( pl_w ) ) return ;
	if( webball_ck( pl_w ) ) return ;
	if( swing_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w11 ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 sting_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sting, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_sting ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump								*/
/*==============================================================*/
static Uint8 webball_ck( PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_webball, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_webball ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump; any number of times in		*/
/*			a super jump										*/
/*==============================================================*/
static Uint8 swing_ck( PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_swing, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->act_st == N_WALL_JUMP ) return 0 ;
	lw->swing_cnt = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag ){
			if( pl_w->jump_com_cnt ) return 0 ;
			pl_w->jump_com_cnt ++ ;
		} else lw->swing_cnt = 3 ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_swing ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 webthrow_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_webthrow, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_webthrow ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air only above the floor and below the		*/
/*			top of the screen									*/
/*==============================================================*/
static Uint8 maximum_ck( PLAYER_WORK *pl_w )
{
	Float y, gy ;

	if( !com_nml_ck( pl_w, com_maximum, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag && pl_w->jump_com_cnt ) return 0 ;
		if( pl_w->act_st != N_DASH && pl_w->act_st != N_BACK_DASH ){
			y = pl_w->pos.y ;
			gy = pl_w->ground_y ;
			if( y < gy + DOT_Y( 12.0f ) || y > gy + DOT_Y( 432.0f ) ) return 0 ;
		}
	}
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_maximum ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crawler_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crawler, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_crawler ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hwebthrow_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hwebthrow, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_hwebthrow ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 7 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl0c_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
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
	pl_w->pl_sp_type = pl0c_sp_snapback ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl0c_delay_ck( PLAYER_WORK *pl_w )
{
	if( maximum_delay_ck( pl_w ) || crawler_delay_ck( pl_w ) || hwebthrow_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int maximum_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_maximum, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int crawler_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crawler, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int hwebthrow_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hwebthrow, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 6 ;
	return 1 ;
}

static void em_flag_clr( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/****************************************************************/
void pl0c_main( PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	em_flag_clr( pl_w, lw ) ;
}

/*==============================================================*/
/*			Clears WEB_HIT for each of the opponent's team		*/
/*			once his routine_1 is under 2						*/
/*==============================================================*/
static void em_flag_clr( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	int i ;

	em = &pl_work[1 >> pl_w->pl_no] ;
	for( i = 0 ; i < 3 ; i++, em += 2 ){
		if( lw->web_flag[i] & WEB_HIT ){
			if( em->routine_1 < PL_CAUGHT ) lw->web_flag[i] ^= WEB_HIT ;
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
void pl0c_atck_set( PLAYER_WORK *pl_w )
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
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		no = atck = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		no = atck = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		no = atck = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Stand_P, no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 6 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 7 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 8 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Crouch_P, no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 3 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 4 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 5 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Stand_K, no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 9 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 10 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 11 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
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
	if( pl_w->atck_pk == 0 ) jump_punch( pl_w ) ;
	else jump_kick( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 12 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = j_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 13 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = j_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 14 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = j_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Jump_P, no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;
	int se ;

	switch( pl_w->nml_no ){
	case 0:
		atck = 15 ;
		no = 0 ;
		se = 20 ;
		pl_w->chain_tbl = j_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		atck = 16 ;
		no = 1 ;
		se = 21 ;
		pl_w->chain_tbl = j_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		atck = 17 ;
		no = 2 ;
		se = 22 ;
		pl_w->chain_tbl = j_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->atck_no = atck ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_se_req( pl_w, se ) ;
	pl_char_set( pl_w, Char_Jump_K, no ) ;
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
void pl0c_atck( PLAYER_WORK *pl_w )
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
	if( pl_w->atck_pk == 0 ){
		if( pl_w->pl_stance == STANCE_STAND ) stand_punch_move( pl_w ) ;
		else crouch_punch_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_STAND ) stand_kick_move( pl_w ) ;
		else crouch_kick_move( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
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
	if( !pl_w->atck_pk ) jump_punch_move( pl_w ) ;
	else jump_kick_move( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/****************************************************************/
/*			The launcher: a punch from standing, a kick from	*/
/*			a crouch											*/
/****************************************************************/
void pl0c_upper( PLAYER_WORK *pl_w )
{
	Uint8 stance, atck, no ;
	int se ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( !pl_w->atck_pk ){
			atck = 43 ;
			no = 1 ;
			se = 21 ;
			stance = 0 ;
		} else {
			atck = 42 ;
			no = 2 ;
			se = 20 ;
			stance = 1 ;
		}
		pl_w->pl_stance = stance ;
		pl_w->atck_no = atck ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_se_req( pl_w, se ) ;
		pl_char_set( pl_w, Char_Extra, no ) ;
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
/*			The dash: a low hop forward							*/
/****************************************************************/
void pl0c_dash( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = -DOT_X( 9.5f ) ;
	pl_w->acc.x = 0.3125f ;
	pl_w->spd.y = DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
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
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
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
	else if( pl_w->cg_type ) RATE_CLR_X( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash: a low hop back						*/
/****************************************************************/
void pl0c_noki( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = DOT_X( 9.5f ) ;
	pl_w->acc.x = -0.3125f ;
	pl_w->spd.y = DOT_Y( 3.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
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
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_sel( PLAYER_WORK *pl_w );
static void appear_drop( PLAYER_WORK *pl_w );
static void appear_pose( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The appearance: pl_w->type picks it, and			*/
/*			appear_sel picks the type							*/
/****************************************************************/
void pl0c_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_sel, appear_drop, appear_pose,
	} ;

	appear_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void appear_sel( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( Sys->vs_flag ){
		pl_w->type = 2 ;
		if( Sys->new_side_keep & 1 << ( pl_w->pl_index & 1 ) ) pl_w->type = 1 ;
	} else {
		pl_w->type = 2 ;
		if( pl_w->cpu_flag ) pl_w->type = 1 ;
	}
}

static void drop_wait( PLAYER_WORK *pl_w );
static void drop_move( PLAYER_WORK *pl_w );
static void drop_end( PLAYER_WORK *pl_w );
static void drop_flash( PLAYER_WORK *pl_w );
static void drop_fall( PLAYER_WORK *pl_w );
static void drop_land( PLAYER_WORK *pl_w );
static void color_fade( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Type 1: he drops in from above the screen			*/
/*==============================================================*/
static void appear_drop( PLAYER_WORK *pl_w )
{
	static void ( *drop_jmp[])( PLAYER_WORK *pl_w ) = {
		drop_wait, drop_move, drop_end,
	} ;

	drop_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void drop_wait( PLAYER_WORK *pl_w )
{
	Float ofs ;

	pl_w->disp_flag = 0 ;
	if( vs_sys_work.vs_st != VS_APPEAR ) return ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->disp_flag = 1 ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 3 ;
	ofs = DOT_X( 43.0f ) ;
	if( !pl_w->pl_dir ) ofs = -DOT_X( 43.0f ) ;
	pl_w->pos.x += ofs ;
	pl_w->spd.y = -DOT_Y( 5.75f ) ;
	pl_w->acc.y = DOT_Y( 0.125f ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void drop_move( PLAYER_WORK *pl_w )
{
	static void ( *drop_move_jmp[])( PLAYER_WORK *pl_w ) = {
		drop_flash, drop_fall, drop_land,
	} ;

	drop_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void drop_flash( PLAYER_WORK *pl_w )
{
	int col ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( --pl_w->timer1 == 0 ){
			pl_w->timer1 = 3 ;
			pl_color_set( pl_w, 1 ) ;
		} else {
			col = pl_w->pl_color << 4 ;
			col += pl_w->timer0 + 8 ;
			ef_color_set( pl_w, 1, col, 1 ) ;
		}
		pl_w->timer0 = ( pl_w->timer0 + 1 ) % 3 ;
		pl_w->disp_flag = Sys->game_timer0 & 1 ;
	} else {
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->timer0 = 0 ;
		pl_w->timer1 = 1 ;
		pl_w->pos.y += DOT_Y( 144.0f ) ;
		pls0c_03_set( pl_w, 0 ) ;
		color_fade( pl_w ) ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void drop_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) color_fade( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y * pl_w->acc.y >= 0.0f ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -1.6666666f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 5.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void drop_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_char_set( pl_w, Char_Appear, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void drop_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
		pl_w->type = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

/*==============================================================*/
/*			Steps the colour back to normal, one shade every	*/
/*			three frames										*/
/*==============================================================*/
static void color_fade( PLAYER_WORK *pl_w )
{
	int col ;

	if( --pl_w->timer1 == 0 ){
		pl_w->timer1 = 3 ;
		if( pl_w->timer0 == 8 ) pl_color_set( pl_w, 1 ) ;
		else {
			col = pl_w->pl_color << 4 ;
			ef_color_set( pl_w, 0, col + pl_w->timer0, 1 ) ;
			pl_w->timer0 ++ ;
		}
	}
}

static void pose_init( PLAYER_WORK *pl_w );
static void pose_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Type 2: a pose										*/
/*==============================================================*/
static void appear_pose( PLAYER_WORK *pl_w )
{
	static void ( *pose_jmp[])( PLAYER_WORK *pl_w ) = {
		pose_init, pose_end,
	} ;

	pose_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void pose_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Appear, 4 ) ;
}

/*--------------------------------------------------------------*/
static void pose_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
		pl_w->routine_3 = 0 ;
		pl_w->type = 0 ;
		pl_color_set( pl_w, 1 ) ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void win_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/****************************************************************/
void pl0c_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		win_init, win_move,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	win_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			A KO win picks one of two poses at random, and		*/
/*			the second swings in on a web						*/
/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	switch( pl_w->type ){
	case 0:
		if( !( pl_w->sub_type = Rnd() & 1 ) ) pl_char_set( pl_w, Char_Win, 0 ) ;
		else {
			lw->win_cut_flag = 0 ;
			pl_char_set( pl_w, Char_Win, 1 ) ;
			scr_excep_set( pl_w, 1, 13 ) ;
		}
		break ;
	case 2:
		pl_char_set( pl_w, Char_Win, 2 ) ;
		break ;
	case 1:
	case 3:
	case 4:
		pl_char_set( pl_w, Char_Win, 3 ) ;
	}
}

static void swing_drop( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_jump( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_land( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	static void ( *swing_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		swing_drop, swing_jump, swing_land, swing_end,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		if( pl_w->sub_type ){
			scr_excep_clr() ;
			lw->win_cut_flag = 1 ;
		}
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->type ){
	case 0:
		if( pl_w->sub_type ){
			swing_jmp[pl_w->routine_3]( pl_w, lw ) ;
			break ;
		}
	case 2:
	case 4:
	case 1:
	case 3:
	default:
		pl_char_move( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void swing_drop( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Float ofs ;

	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		ofs = DOT_X( 32.0f ) ;
		if( pl_w->pl_dir ) ofs = -DOT_X( 32.0f ) ;
		pl_w->pos.x += ofs ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type ^= 2 ;
		pls0c_03_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void swing_jump( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Float ofs ;

	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		ofs = -DOT_X( 32.0f ) ;
		if( pl_w->pl_dir ) ofs = DOT_X( 32.0f ) ;
		pl_w->pos.x += ofs ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = -DOT_X( 10.0f ) ;
		pl_w->acc.x = 0.5208333f ;
		pl_w->spd.y = DOT_Y( 2.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void swing_land( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void swing_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
}

static void sp_sting( PLAYER_WORK *pl_w );
static void sp_webball( PLAYER_WORK *pl_w );
static void sp_swing( PLAYER_WORK *pl_w );
static void sp_webthrow( PLAYER_WORK *pl_w );
static void sp_maximum( PLAYER_WORK *pl_w );
static void sp_crawler( PLAYER_WORK *pl_w );
static void sp_hwebthrow( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0c_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_sting, sp_webball, sp_swing, sp_webthrow, sp_maximum, sp_crawler, sp_hwebthrow, sp_chouhatsu, sp_snapback,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void sting_init( PLAYER_WORK *pl_w );
static void sting_move( PLAYER_WORK *pl_w );
static void sting_end( PLAYER_WORK *pl_w );
static void sting_spd_set( PLAYER_WORK *pl_w, int no );

/*==============================================================*/
/*			Spider Sting: a rising uppercut that hits again if	*/
/*			the first one lands and a punch is held				*/
/*==============================================================*/
static void sp_sting( PLAYER_WORK *pl_w )
{
	static void ( *sting_jmp[])( PLAYER_WORK *pl_w ) = {
		sting_init, sting_move, sting_end,
	} ;

	sting_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sting_init( PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->sjump_flag = 0 ;
	lw->hit2_flag = 0 ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl_w->atck_level + 48 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

static void sting_rise( PLAYER_WORK *pl_w );
static void sting_climb( PLAYER_WORK *pl_w );
static void sting_top( PLAYER_WORK *pl_w );
static void sting_fall( PLAYER_WORK *pl_w );
static void sting_land( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void sting_move( PLAYER_WORK *pl_w )
{
	static void ( *sting_move_jmp[])( PLAYER_WORK *pl_w ) = {
		sting_rise, sting_climb, sting_top, sting_fall, sting_land,
	} ;

	sting_move_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void sting_rise( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_air_set( pl_w ) ;
		if( !pl_w->hit_flag ){
			pl_w->atck_no = pl_w->atck_level + 100 ;
			HIT_RESET( pl_w ) ;
		}
		sting_spd_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sting_climb( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		sting_spd_set( pl_w, 1 ) ;
	}
}

/*==============================================================*/
/*			At the top: the second hit if the first landed		*/
/*			and a punch is held									*/
/*==============================================================*/
static void sting_top( PLAYER_WORK *pl_w )
{
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;
	int flag ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		flag = 0 ;
		if( pl_w->hit_flag && !( pl_w->hit_flag & HR_GUARD ) ){
			if( pl_w->cpu_flag ) flag = 1 ;
			else if( pl_w->pl_sw_now & KEY_PUNCH ) flag = 1 ;
		}
		lw->hit2_flag |= flag ;
	}
	if( pl_w->spd.y <= 0.0f ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		if( lw->hit2_flag ){
			pl_w->spd.y += DOT_Y( 5.0f ) ;
			pl_w->atck_no = pl_w->atck_level + 51 ;
			HIT_RESET( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 2 ) ;
		} else {
			pl_w->routine_3 ++ ;
			if( !pl_w->atck_level ) pl_w->spd.y = 2.142857f ;
			else pl_w->spd.y = DOT_Y( 2.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.75f ) ;
			pl_char_set( pl_w, Char_Special, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sting_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.y = -DOT_Y( 6.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sting_land( PLAYER_WORK *pl_w )
{
	int no ;

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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->hit_flag & HR_GUARD || pl_w->atck_level == 1 ) no = 6 ;
		else no = 5 ;
		pl_char_set( pl_w, Char_Special, no ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void sting_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The speeds for each part of the rise, in 16.16		*/
/*==============================================================*/
static void sting_spd_set( PLAYER_WORK *pl_w, int no )
{
	const Sint32 *spd ;

	spd = pl0c_sting_spd_tbl[no][0] ;
	spd += pl_w->atck_level * 4 ;

	pl_w->spd.x = *spd++ * 1.6666666f / 65536.0f ;
	pl_w->acc.x = *spd++ * 1.6666666f / 65536.0f ;
	pl_w->spd.y = *spd++ * 2.142857f / 65536.0f ;
	pl_w->acc.y = *spd * 2.142857f / 65536.0f ;
	if( pl_w->disp_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
}

static void webball_init( PLAYER_WORK *pl_w );
static void webball_shot( PLAYER_WORK *pl_w );
static void webball_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Web Ball, from the ground or in the air				*/
/*==============================================================*/
static void sp_webball( PLAYER_WORK *pl_w )
{
	static void ( *webball_jmp[])( PLAYER_WORK *pl_w ) = {
		webball_init, webball_shot, webball_end,
	} ;

	webball_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void webball_init( PLAYER_WORK *pl_w )
{
	Uint8 atck, no ;

	pl_w->routine_2 ++ ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->sjump_flag = 0 ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		atck = 54 ;
		no = 7 ;
	} else {
		pl_w->jump_com_cnt ++ ;
		pl_w->spd.x = 1.6666666f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		pl_w->disp_dir = pl_w->pl_dir ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		atck = 57 ;
		no = 9 ;
	}
	pl_w->atck_no = pl_w->atck_level + atck ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + no ) ;
}

/*--------------------------------------------------------------*/
static void webball_shot( PLAYER_WORK *pl_w )
{
	int air ;

	air = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_land_set( pl_w ) ;
			return ;
		}
		air = 1 ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		shl0c_00_set( pl_w, air ) ;
	}
}

/*--------------------------------------------------------------*/
static void webball_end( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance != STANCE_AIR ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_land_set( pl_w ) ;
		} else {
			if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
			if( pl_w->cg_type & 1 ){
				pl_w->cg_type = 0 ;
				pl_w->spd.y += DOT_Y( 0.375f ) ;
				pl_w->acc.y = -DOT_Y( 0.375f ) ;
			}
		}
	}
}

static void sp_swing_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void sp_swing_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void sp_swing_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void web_hand_set( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*==============================================================*/
/*			Web Swing: a kick from a web line					*/
/*			that can be done again in the air					*/
/*==============================================================*/
static void sp_swing( PLAYER_WORK *pl_w )
{
	static void ( *sp_swing_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		sp_swing_init, sp_swing_move, sp_swing_end,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	sp_swing_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_swing_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->type = 1 ;
	pl_w->dir = 28 ;
	pl_w->push_ng_timer = 2 ;
	lw->swing_st = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = DOT_X( 1.75f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->type = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_air_set( pl_w ) ;
	}
	pl_w->atck_no = pl_w->type * 3 + 60 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->type * 3 + pl_w->atck_level + 11 ) ;
}

static void swing_shot( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_pull( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_swing( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void sp_swing_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	static void ( *swing_move_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		swing_shot, swing_pull, swing_swing,
	} ;

	pl_w->push_ng_timer = 2 ;
	swing_move_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void swing_shot( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	int no ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->routine_3 ++ ;
		web_hand_set( pl_w, lw ) ;
		no = 8 ;
		if( pl_w->atck_level ) no = 10 ;
		pls0c_04_set( pl_w, no, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void swing_pull( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	web_hand_set( pl_w, lw ) ;
	if( lw->swing_st ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -3.75f ;
		pl_w->acc.x = -1.25f ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_gauge_add( pl_w, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void swing_swing( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( lw->swing_st == 1 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		web_hand_set( pl_w, lw ) ;
		if( ( pl_w->wall_flag & 1 << ( pl_w->pl_dir ^ 1 ) ) || !( 1 << pl_w->dir & 0xFC00000F ) ) lw->swing_st = 2 ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x = -5.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Special, pl_w->type * 3 + 13 ) ;
	}
}

static void swing_again( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void sp_swing_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( !pl_w->routine_3 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			if( !pl_w->type ){
				pl_land_set( pl_w ) ;
				return ;
			}
			pl_w->routine_3 ++ ;
			pl_dir_set( pl_w ) ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Jump, 3 ) ;
			return ;
		}
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ) swing_again( pl_w, lw ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			In the air, the command again starts a new swing	*/
/*			the other way										*/
/*==============================================================*/
static void swing_again( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( pl_w->type && pl_w->pl_stance == STANCE_AIR && !pl_w->cpu_flag && lw->swing_cnt ){
		pl_w->act_st = N_JUMP_AIR ;
		if( !com_nml_ck( pl_w, com_swing, &pl_w->com_w12 ) ){
			pl_w->act_st = N_SPECIAL ;
			return ;
		}
		lw->swing_cnt -- ;
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
		com_sw_set( pl_w, &pl_w->com_w12 ) ;
		pl_w->routine_2 = 0 ;
		pl_w->routine_3 = 0 ;
		pl_w->pl_sp_type = pl0c_sp_swing ;
		pl_w->act_st = N_SPECIAL ;
	}
}

static void webthrow_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void webthrow_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void webthrow_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*==============================================================*/
/*			Web Throw: a web line that pulls the				*/
/*			opponent in and swings them over					*/
/*==============================================================*/
static void sp_webthrow( PLAYER_WORK *pl_w )
{
	static void ( *webthrow_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		webthrow_init, webthrow_move, webthrow_end,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	webthrow_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void webthrow_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	lw->line_st = 0 ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->dir = webthrow_dir_tbl[pl_w->atck_level] ;
	pl_w->atck_no = pl_w->atck_level + 66 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 18 ) ;
}

static void webthrow_shot( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void webthrow_pull( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void webthrow_catch( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void webthrow_swing( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void webthrow_throw( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void webthrow_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	static void ( *webthrow_move_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		webthrow_shot, webthrow_pull, webthrow_catch, webthrow_swing, webthrow_throw,
	} ;

	webthrow_move_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void webthrow_shot( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->routine_3 ++ ;
		pl_w->cg_flag1 = 0 ;
		pl_w->unk_27b = 1 ;
		pl_w->cancel_timer = 16 ;
		pl_gauge_add( pl_w, 5 ) ;
		web_hand_set( pl_w, lw ) ;
		shl0c_01_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			The line flies until lw->line_st says it caught		*/
/*			(above 0) or missed (below 0)						*/
/*==============================================================*/
static void webthrow_pull( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->dir = pl_w->cg_prm ;
	web_hand_set( pl_w, lw ) ;
	if( lw->line_st > 0 ){
		pl_w->routine_3 ++ ;
		pl_w->catch_flag = 1 ;
		pl_w->sp_ng_timer = 3 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 21 ) ;
	} else if( lw->line_st < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void webthrow_catch( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->line_st < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->sp_ng_timer = 1 ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 25 ) ;
		return ;
	}
	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		lw->line_st = 2 ;
		lw->spin_speed = 0 ;
		lw->damage = 0 ;
		lw->spin_cnt = 2 ;
		lw->spin_max = 5 ;
		em = pl_w->catch_w ;
		em->routine_2 ++ ;
		scr_excep_set( pl_w, 5, 5 ) ;
		hold_renda_set( em, 16 ) ;
		catch_renda_set( pl_w, 10 ) ;
		pl_char_set( pl_w, Char_Catch, 3 ) ;
		pl_catch_pass( pl_w, pl_w ) ;
		pl_w->catch_grip_old = -1 ;
		pl_w->catch_no = 0xc5 ;
	} else {
		web_hand_set( pl_w, lw ) ;
		pl_w->dir = pl_w->cg_prm ;
	}
}

/*==============================================================*/
/*			Swinging the opponent round: mashing makes it do	*/
/*			more												*/
/*==============================================================*/
static void webthrow_swing( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	int dm ;

	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	em = pl_w->catch_w ;
	em->wall_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( hold_renda_ck( em ) ){
		lw->damage += 2 ;
		lw->spin_speed -- ;
		lw->spin_cnt -- ;
		hold_renda_set( em, 16 ) ;
	}
	if( catch_renda_ck( pl_w ) ){
		lw->damage -= 4 ;
		lw->spin_speed += 2 ;
		lw->spin_cnt += 2 ;
		catch_renda_set( pl_w, 10 ) ;
	}
	dm = lw->spin_speed >> 1 ;
	if( dm < 0 ){
		lw->spin_speed = 0 ;
		dm = 0 ;
	}
	if( dm >= pl_w->cg_ctr ) dm = pl_w->cg_ctr - 1 ;
	pl_w->cg_ctr -= dm ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		if( ( lw->spin_cnt -= 2 ) <= 0 || -- lw->spin_max == 0 ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Catch, 4 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void webthrow_throw( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	int dm ;

	pl_w->catch_flag = 1 ;
	pl_w->sp_ng_timer = 3 ;
	em = pl_w->catch_w ;
	em->wall_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0c_at_webthrow ;
		em->routine_2 = 0 ;
		em->wall_flag = 0 ;
		pl_w->dir = 7 ;
		if( pl_w->pl_dir ) pl_w->dir = 32 - pl_w->dir ;
		SCR_EXCEP_SET ;
		pls0c_03_set( pl_w, 3 ) ;
		hold_damage( em, lw->damage ) ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->sp_ng_timer = 1 ;
		pl_char_set( pl_w, Char_Special, 24 ) ;
	} else {
		dm = lw->spin_speed >> 1 ;
		if( dm < 0 ){
			lw->spin_speed = 0 ;
			dm = 0 ;
		}
		if( dm >= pl_w->cg_ctr ) dm = pl_w->cg_ctr - 1 ;
		pl_w->cg_ctr -= dm ;
	}
}

/*--------------------------------------------------------------*/
static void webthrow_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->catch_flag = 1 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->disp_dir ^= 1 ;
			pl_w->pl_dir ^= 1 ;
		}
	} else pl_stand_ret( pl_w ) ;
}

static void maximum_jump( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_dash( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*==============================================================*/
/*			Maximum Spider (hyper): a leap to the corner of		*/
/*			the screen and dashes off the walls					*/
/*==============================================================*/
static void sp_maximum( PLAYER_WORK *pl_w )
{
	static void ( *maximum_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		maximum_jump, maximum_dash, maximum_end,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	maximum_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void maximum_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_rise( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_wall( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*==============================================================*/
static void maximum_jump( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	static void ( *maximum_jump_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		maximum_init, maximum_rise, maximum_wall,
	} ;

	maximum_jump_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void maximum_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->jump_com_cnt = 1 ;
	pl_w->timer0 = 40 ;
	lw->maximum_dash_cnt = 10 ;
	lw->maximum_hit_cnt = 4 ;
	lw->maximum_aim_timer = 5 ;
	lw->maximum_em_w = pl_w->target_w ;
	lw->maximum_hit_flag = 0 ;
	lw->maximum_aim_flag = 0 ;
	lw->maximum_edge = 0 ;
	lw->maximum_old_edge = 0 ;
	lw->maximum_wall_flag = 0 ;
	lw->maximum_old_dir = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	scr_excep_set( pl_w, 1, pl_w->pl_no ? 4 : 3 ) ;
	pl_w->atck_no = pl0c_at_maximum ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	lw->maximum_disp_dir = pl_w->disp_dir ;
	pl_w->type = 1 ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		pl_w->type = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	pl_char_set( pl_w, Char_Super, pl_w->type ) ;
}

/*==============================================================*/
/*			The leap to the top corner on the side he faces		*/
/*==============================================================*/
static void maximum_rise( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Float x, spd ;
	int wall ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->cg_flag1 = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		x = scr_work.scr_left + DOT_X( 8.0f ) ;
		if( !pl_w->pl_dir ) x = scr_work.scr_right + -DOT_X( 8.0f ) ;
		pl_w->spd.x = ( x - pl_w->pos.x ) / 32.0f ;
		pl_w->acc.x = 0.0f ;
		spd = DOT_Y( 6.0f ) ;
		if( pl_w->type ) spd = DOT_Y( 4.0f ) ;
		pl_w->spd.y = spd ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( -- pl_w->timer0 == 0 ){
		wall = pl_w->wall_flag ;
		if( !wall ){
			wall = 1 ;
			if( pl_w->spd.x < 0.0f ) wall = 2 ;
		}
		if( wall & 1 << pl_w->pl_dir ){
			pl_w->routine_3 ++ ;
			lw->maximum_edge = wall ;
			lw->maximum_old_edge = wall ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_char_set( pl_w, Char_Super, 10 ) ;
		} else pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Clinging to the wall								*/
/*==============================================================*/
static void maximum_wall( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 20.0f ;
		ofs.y = DOT_Y( 62.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

static void maximum_dash_start( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_dash_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_dash_hit( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void maximum_dash( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	static void ( *maximum_dash_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		maximum_dash_start, maximum_dash_move, maximum_dash_hit,
	} ;

	pl_w->push_ng_timer = 2 ;
	maximum_dash_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

static int maximum_turn( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw, Uint8 mask );
static int maximum_y_ck( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static int maximum_x_ck( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void maximum_spd_set( PLAYER_WORK *pl_w, Uint8 dir );
static Uint8 maximum_dir_get( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*==============================================================*/
/*			Each dash starts toward the opponent, or the way	*/
/*			the stick points									*/
/*==============================================================*/
static void maximum_dash_start( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Uint8 dir ;
	int atck ;
	Uint8 no ;
	Uint16 sw ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 ++ ;
	pl_w->disp_dir = 0 ;
	pl_w->timer0 = 30 ;
	lw->maximum_edge = 0 ;
	no = pl_w->pl_no ? 3 : 4 ;
	scr_excep_set( pl_w, 1, no ) ;
	dm00_05_set( pl_w ) ;
	sw = pl_w->pl_new_lvbt & KEY_LEVER ;
	if( !sw ){
		lw->maximum_old_dir = pl_w->dir ;
		dir = maximum_dir_get( pl_w, pl_w->target_w ) ;
		if( dir & 32 ) dir = ( 64 - dir ) & 31 ;
		if( dir > 18 ){
			dir = 21 ;
			atck = 0 ;
		} else if( dir > 14 ){
			dir = 16 ;
			atck = 2 ;
		} else {
			dir = 8 ;
			atck = 1 ;
		}
	} else {
		sw = ( sw >> 10 & 1 ) | sw >> 11 ;
		dir = maximum_dir_tbl[sw][0] ;
		atck = maximum_dir_tbl[sw][1] ;
	}
	if( !pl_w->pl_dir ) dir = ( 64 - dir ) & 63 ;
	pl_w->dir = dir ;
	if( !pl_w->type ) atck += 69 ;
	else atck += 73 ;
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_w->atck_attr = ATR_BACK_TURN ;
	pl_w->pl_dir = 0 ;
	maximum_spd_set( pl_w, pl_w->dir ) ;
	pl_voice_req( pl_w, 20 ) ;
	pl_char_set( pl_w, Char_Super, ( pl_w->dir >> 2 ) + 12 ) ;
}

/*==============================================================*/
/*			Dashing: bounce off the stage's edges, and home		*/
/*			in once a dash has struck							*/
/*==============================================================*/
static void maximum_dash_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Uint8 d ;
	int no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !-- pl_w->timer0 ) goto stop ;
	if( lw->maximum_hit_flag && !pl_w->combo_cnt ) goto stop ;
	if( vs_sys_work.time_over ) goto stop ;
	if( !pl_w->hit_flag ) goto aim ;
	if( pl_w->hit_flag & HR_GUARD ) goto stop ;
	em = pl_w->em_w ;
	if( em->work_kind || em->routine_1 != PL_DAMAGE ) goto stop ;
	lw->maximum_hit_flag = 1 ;
	lw->maximum_em_w = em ;
	if( lw->maximum_hit_cnt ) goto aim ;
stop:
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->spd.x = -( pl_w->spd.x / 16.0f ) ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	scr_excep_clr() ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
	return ;
aim:
	if( lw->maximum_hit_flag && !pl_w->hit_flag && -- lw->maximum_aim_timer == 0 ){
		lw->maximum_aim_timer = 5 ;
		lw->maximum_old_dir = pl_w->dir ;
		d = maximum_dir_get( pl_w, lw->maximum_em_w ) ;
		d -= lw->maximum_old_dir ;
		if( d ){
			d &= 63 ;
			if( d & ~5 ) d = 0xff ;
			else d = 1 ;
			pl_w->dir = d + lw->maximum_old_dir ;
			maximum_spd_set( pl_w, pl_w->dir ) ;
		}
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( maximum_y_ck( pl_w, lw ) ){
		pl_w->routine_3 ++ ;
		lw->maximum_wall_flag = 0 ;
		no = ( ( lw->maximum_edge & 8 ) >> 2 ) + 4 ;
		if( pl_w->spd.x > 0.0f ) no ++ ;
		pl_char_set( pl_w, Char_Super, no ) ;
	} else if( maximum_x_ck( pl_w, lw ) ){
		pl_w->routine_3 ++ ;
		lw->maximum_wall_flag = 1 ;
		no = ( lw->maximum_edge & 1 ) * 2 + 8 ;
		if( pl_w->spd.y > 0.0f ) no ++ ;
		pl_char_set( pl_w, Char_Super, no ) ;
	}
}

/*==============================================================*/
/*			Against an edge: the next dash, or the fall when	*/
/*			the dashes are spent								*/
/*==============================================================*/
static void maximum_dash_hit( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Uint8 atck, no ;
	int i ;
	Uint8 wall ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !lw->maximum_wall_flag ){
		if( !maximum_x_ck( pl_w, lw ) ) lw->maximum_edge &= 0xfc ;
	} else {
		if( !maximum_y_ck( pl_w, lw ) ) lw->maximum_edge &= 0xf3 ;
	}
	if( pl_char_move( pl_w ) >= 0 ) return ;
	em = lw->maximum_em_w ;
	if( em->routine_1 != PL_DAMAGE ) goto fall ;
	if( !lw->maximum_hit_flag ) goto fall ;
	if( !-- lw->maximum_dash_cnt ) goto fall ;
	if( pl_w->hit_flag && -- lw->maximum_hit_cnt == 0 ) goto pick ;
	if( !lw->maximum_aim_flag ) goto turn ;
pick:
	lw->maximum_aim_flag = ( 0x7fff >> ( Rnd() & 15 ) ) & 1 ;
	lw->maximum_old_dir = pl_w->dir ;
	pl_w->dir = maximum_dir_get( pl_w, lw->maximum_em_w ) ;
	i = ( ( pl_w->dir + 4 ) & 63 ) >> 3 ;
	wall = maximum_wall_tbl[i] ;
	wall &= lw->maximum_edge ;
	if( wall == lw->maximum_edge ) goto set ;
	pl_w->dir = lw->maximum_old_dir ;
turn:
	if( lw->maximum_edge & 3 ){
		if( maximum_turn( pl_w, lw, 48 ) ) goto set ;
	}
	if( lw->maximum_edge & 12 ) maximum_turn( pl_w, lw, 16 ) ;
set:
	maximum_spd_set( pl_w, pl_w->dir ) ;
	atck = ( pl_w->dir >> 3 ) + 77 ;
	no = 0 ;
	if( !lw->maximum_hit_cnt ){
		atck = 93 ;
		no = 32 ;
	}
	pl_w->atck_no = atck ;
	HIT_RESET( pl_w ) ;
	pl_w->atck_attr = no ;
	pl_w->routine_3 -- ;
	pl_w->timer0 = 30 ;
	lw->maximum_aim_timer = 5 ;
	lw->maximum_old_edge = lw->maximum_edge ;
	lw->maximum_wall_flag = 0 ;
	lw->maximum_edge = 0 ;
	pl_char_set( pl_w, Char_Super, ( ( ( pl_w->dir + 2 ) & 63 ) >> 2 ) + 12 ) ;
	return ;
fall:
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->disp_dir = lw->maximum_disp_dir ;
	pl_w->pl_dir = lw->maximum_disp_dir ;
	lw->maximum_hit_cnt = 0 ;
	scr_excep_clr() ;
	if( !lw->maximum_wall_flag ){
		pl_w->spd.x /= 16.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_w->acc.y = -0.9375f ;
		if( lw->maximum_edge & 8 ) pl_w->spd.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
	} else {
		pl_w->spd.x = -( pl_w->spd.x / 16.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 5.0f ) ;
		pl_w->acc.y = -0.9375f ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*==============================================================*/
/*			The fall after the last dash						*/
/*==============================================================*/
static void maximum_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( lw->maximum_hit_flag ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_land_set( pl_w ) ;
			pl_land_ef_set( pl_w ) ;
		}
	} else if( !pl_w->routine_3 ){
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->spd.y < 0.0f ){
			if( pl_w->pos.y < pl_w->ground_y ){
				pl_w->routine_3 ++ ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->pl_stance = STANCE_STAND ;
				pl_char_set( pl_w, Char_Jump, 3 ) ;
				pl_land_ef_set( pl_w ) ;
			}
		}
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/* turn back from an edge; straight up or down turns aside */
static int maximum_turn( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw, Uint8 mask )
{
	int dir, ret = 0 ;
	Uint8 old, low ;

	lw->maximum_aim_flag = 1 ;
	lw->maximum_old_dir = pl_w->dir ;
	old = pl_w->dir ;
	if( ( dir = old ) == 16 || dir == 48 ){
		mask = 32 ;
		ret = 1 ;
	}
	/* low is never set: the arcade's code takes the stack pointer's low bits for it */
	old = ( mask ^ old ) & 48 ;
	low = ( 16 - ( low & 15 ) ) & 15 ;
	pl_w->dir = old | low ;
	return ret ;
}

/*==============================================================*/
/*			The floor and the top of the screen: 4 or 8 when	*/
/*			one is reached										*/
/*==============================================================*/
static int maximum_y_ck( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	SCR_WORK *bg = &scr_work ;
	Float ground, lim, y ;

	ground = pl_w->ground_y ;
	lim = ground ;
	if( lw->maximum_hit_flag && bg->scr_bottom > ground ) lim = bg->scr_bottom ;
	if( pl_w->pos.y + DOT_Y( 48.0f ) < lim && !( lw->maximum_old_edge & 4 ) ){
		pl_w->pos.y = lim ;
		lw->maximum_edge |= 4 ;
		return 4 ;
	}
	lim = bg->scr_top ;
	if( !lw->maximum_hit_flag ) lim = pl_w->ground_y + DOT_Y( 456.0f ) ;
	y = pl_w->pos.y ;
	if( lw->maximum_hit_flag ) y += DOT_Y( 64.0f ) ;
	if( y > lim && !( lw->maximum_old_edge & 8 ) ){
		pl_w->pos.y = lim + -240.0f ;
		lw->maximum_edge |= 8 ;
		return 8 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The stage's walls: the wall's bit when one is		*/
/*			reached												*/
/*==============================================================*/
static int maximum_x_ck( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	SCR_WORK *bg ;
	Float x ;
	Uint8 wall ;

	bg = &scr_work ;
	if( !( wall = pl_w->wall_flag ) ) return 0 ;
	if( !( lw->maximum_old_edge & wall ) ){
		x = bg->scr_left ;
		if( !( wall >> 1 ) ) x = bg->scr_right ;
		pl_w->pos.x = x ;
		lw->maximum_edge |= wall ;
		return wall ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The dash's speed along its direction, a sixty-		*/
/*			fourth of a turn for each step						*/
/*==============================================================*/
static void maximum_spd_set( PLAYER_WORK *pl_w, Uint8 dir )
{
	Sint16 ang ;
	Float spd ;

	spd = 665600.0f ;
	ang = ( ( 80 - dir ) & 63 ) << 10 ;
	pl_w->spd.x = spd * nlCos( ang ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f + pl_w->acc.x ;
	pl_w->spd.y = spd * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f + pl_w->acc.y ;
}

/*==============================================================*/
/*			The direction from his chest to the opponent's,		*/
/*			0 to 63												*/
/*==============================================================*/
static Uint8 maximum_dir_get( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	VEC3 pos, em_pos ;
	Uint8 dir ;

	pos = pl_w->pos ;
	em_pos = em->pos ;
	pos.y += DOT_Y( 64.0f ) ;
	em_pos.y += DOT_Y( 48.0f ) ;
	dir = ( caldir_pos_256( &pos, &em_pos ) + 2 ) >> 2 ;
	return dir ;
}

static void crawler_init( PLAYER_WORK *pl_w );
static void crawler_web( PLAYER_WORK *pl_w );
static void crawler_move( PLAYER_WORK *pl_w );
static void crawler_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Crawler Assault (hyper): a rush along the ground	*/
/*==============================================================*/
static void sp_crawler( PLAYER_WORK *pl_w )
{
	static void ( *crawler_jmp[])( PLAYER_WORK *pl_w ) = {
		crawler_init, crawler_web, crawler_move, crawler_end,
	} ;

	crawler_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crawler_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0c_at_crawler ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = -10.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_char_set( pl_w, Char_Super, 29 ) ;
}

/*--------------------------------------------------------------*/
static void crawler_web( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		pl_w->cg_prm = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 52.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			The rush: each hit comes from the animation			*/
/*==============================================================*/
static void crawler_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->atck_attr = ATR_DOWN_LIFT ;
			pl_w->cg_flag1 = 0 ;
		}
	} else {
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Super, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crawler_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void hwebthrow_start( PLAYER_WORK *pl_w );
static void hwebthrow_web( PLAYER_WORK *pl_w );
static void hwebthrow_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Ultimate Web Throw (hyper): a web from the air that	*/
/*			pulls the opponent in and throws them				*/
/*==============================================================*/
static void sp_hwebthrow( PLAYER_WORK *pl_w )
{
	static void ( *hwebthrow_jmp[])( PLAYER_WORK *pl_w ) = {
		hwebthrow_start, hwebthrow_web, hwebthrow_land,
	} ;

	hwebthrow_jmp[pl_w->routine_2]( pl_w ) ;
}

static void hwebthrow_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void hwebthrow_jump( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*==============================================================*/
static void hwebthrow_start( PLAYER_WORK *pl_w )
{
	static void ( *hwebthrow_start_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		hwebthrow_init, hwebthrow_jump,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	hwebthrow_start_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void hwebthrow_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 2 ;
	lw->hweb_st = 0 ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0c_at_hwebthrow ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = DOT_X( 3.125f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 10.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_char_set( pl_w, Char_Super, 32 ) ;
}

/*--------------------------------------------------------------*/
static void hwebthrow_jump( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( !( pl_w->cg_type & 1 ) ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_w->cg_type & 2 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->dir = 0 ;
		pl_w->cg_type ^= 2 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 88.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

static void hwebthrow_shot( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void hwebthrow_catch( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void hwebthrow_pull( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void hwebthrow_throw( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void hwebthrow_dir_set( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void web_hand_set( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void hwebthrow_web( PLAYER_WORK *pl_w )
{
	static void ( *hwebthrow_web_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		hwebthrow_shot, hwebthrow_catch, hwebthrow_pull, hwebthrow_throw,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	hwebthrow_web_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void hwebthrow_shot( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		shl0c_02_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			Wait for the web: it catches the opponent, or		*/
/*			misses and he falls									*/
/*==============================================================*/
static void hwebthrow_catch( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;
	Uint8 no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( lw->hweb_st > 0 ){
		pl_w->routine_3 ++ ;
		pl_w->catch_flag = 1 ;
		em = pl_w->catch_w ;
		em->wall_ng_timer = 2 ;
		no = pl_w->pl_no ? 4 : 3 ;
		scr_excep_set( pl_w, no, no ) ;
		web_hand_set( pl_w, lw ) ;
		pl_w->spd.x = -1.6666666f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -0.29296875f ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pls0c_05_set( pl_w, 8, 0 ) ;
		pl_char_set( pl_w, Char_Super, 35 ) ;
		pl_w->dir = pl_w->cg_flag1 ;
		lw->web_line = lw->web_dir = pl_w->dir ;
		hwebthrow_dir_set( pl_w, lw ) ;
	} else if( lw->hweb_st < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->spd.x /= 2.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		lw->hweb_st = 0 ;
		pl_char_set( pl_w, Char_Super, 34 ) ;
	}
}

/*==============================================================*/
/*			Reel the opponent in								*/
/*==============================================================*/
static void hwebthrow_pull( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	em = pl_w->catch_w ;
	em->wall_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 && !-- pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Super, 36 ) ;
	}
	web_hand_set( pl_w, lw ) ;
	hwebthrow_dir_set( pl_w, lw ) ;
}

/*==============================================================*/
/*			Swing the opponent round and let go					*/
/*==============================================================*/
static void hwebthrow_throw( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	em = pl_w->catch_w ;
	em->wall_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	web_hand_set( pl_w, lw ) ;
	hwebthrow_dir_set( pl_w, lw ) ;
	if( pl_w->cg_prm ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		lw->hweb_st = -1 ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0c_at_hwebthrow_2 ;
		em->routine_2 = 0 ;
		em->wall_flag = 0 ;
		scr_excep_clr() ;
		catch_se_req( pl_w ) ;
		pl_w->spd.x = DOT_X( 5.0f ) ;
		pl_w->acc.x = -0.3125f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -0.9375f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

static void hwebthrow_fall( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void hwebthrow_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void hwebthrow_land( PLAYER_WORK *pl_w )
{
	static void ( *hwebthrow_land_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		hwebthrow_fall, hwebthrow_end,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	hwebthrow_land_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void hwebthrow_fall( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	int grp, no ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) RATE_CLR_X( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		grp = 1 ;
		no = 3 ;
		if( lw->hweb_st ){
			grp = 22 ;
			no = 37 ;
		}
		pl_char_set( pl_w, grp, no ) ;
	}
}

/*--------------------------------------------------------------*/
static void hwebthrow_end( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Turn the web's line toward the animation's, over	*/
/*			cg_ctr frames										*/
/*==============================================================*/
static void hwebthrow_dir_set( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Float dir ;

	dir = pl_w->cg_flag1 & 127 ;
	if( lw->web_dir != dir ){
		lw->web_line = lw->web_dir ;
		lw->web_dir = dir ;
		lw->web_turn = lw->web_dir - lw->web_line ;
		if( lw->web_turn >= 16.0f ) lw->web_turn -= 32.0f ;
		if( lw->web_turn <= -16.0f ) lw->web_turn += 32.0f ;
		lw->web_turn /= pl_w->cg_ctr ;
	}
	if( lw->web_dir == lw->web_line ) lw->web_turn = 0.0f ;
	lw->web_line += lw->web_turn ;
	if( lw->web_line < 0.0f ) lw->web_line += 32.0f ;
	if( lw->web_line >= 32.0f ) lw->web_line -= 32.0f ;
	pl_w->dir = lw->web_line ;
}

/*==============================================================*/
/*			Where the web leaves his hand in this frame of		*/
/*			the animation										*/
/*==============================================================*/
static void web_hand_set( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Sint16 *ofs ;
	Float x, y ;

	ofs = &sp_ofs_tbl[pl_w->cg_type * 2] ;
	x = *ofs++ * 1.6666666f ;
	y = *ofs * 2.142857f ;
	if( pl_w->pl_dir ) x = -x ;
	lw->hand_x = x ;
	lw->hand_y = y ;
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The taunt: a pose that flashes him one of three		*/
/*			colours, picked by the stick						*/
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
	pl_char_set( pl_w, Char_Special, 30 ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	int no ;
	int col ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, 1 ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		sw = pl_w->pl_sw_new ;
		no = -1 ;
		if( sw & KEY_FORWARD ) no = 0 ;
		if( sw & KEY_BACK ) no = 1 ;
		if( sw & KEY_DOWN ) no = 2 ;
		if( no == -1 ) no = ( Rnd() & 15 ) % 3 ;
		pl_w->type = no ;
		pls0c_03_set( pl_w, 2 ) ;
		col = pl_w->pl_color * 16 ;
		ef_color_set( pl_w, 1, no + col + 9, 1 ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback										*/
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
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0c_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 31 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else {
		if( pl_w->cg_type & 1 ){
			pl_w->cg_type ^= 1 ;
			ofs.x = DOT_X( 16.0f ) ;
			ofs.y = DOT_Y( 78.0f ) ;
			pl_snap_flash( pl_w, &ofs ) ;
		}
		if( pl_w->cg_type & 2 ) pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl0c_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl0c_sp_crawler ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl0c_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl0c_sp_crawler ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl0c_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0c_sp_webball ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0c_sp_swing ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0c_sp_sting ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl0c_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 = pl_w->routine_2 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl0c_sp_webball ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl0c_sp_swing ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl0c_sp_sting ;
		pl_w->atck_level = 1 ;
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
void	pl0c_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = -30.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->atck_no = pl0c_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 28 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 29 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void swing_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );
static void swing_fall( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw );

/****************************************************************/
/*			Swings on a web until he meets a wall or the web	*/
/*			passes the end of its arc							*/
/****************************************************************/
void	pl0c_web_swing( PLAYER_WORK *pl_w )
{
	static void ( *swing_jmp[])( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw ) = {
		swing_init, swing_move, swing_fall,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	swing_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void swing_init( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->dir = 28 ;
	pl_w->sp_ng_timer = 12 ;
	pl_w->atck_ng_timer = 22 ;
	lw->web_swing_st = 0 ;
	pls0c_06_set( pl_w, 8, 0 ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
	web_hand_set( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void swing_move( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	Float acc = -0.9375f ;
	Float spd ;

	/* a wall behind him lets go at once */
	if( pl_w->wall_flag ){
		if( !( pl_w->wall_flag & 1 << pl_w->pl_dir ) ) goto let_go ;
	}
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	web_hand_set( pl_w, lw ) ;
	if( !pl_w->routine_3 ){
		if( pl_w->cg_prm ) return ;
		pl_w->routine_3 ++ ;
		pl_w->spd.x = -DOT_X( 3.25f ) ;
		pl_w->acc.x = -1.25f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = acc ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
	spd = pl_w->spd.x + pl_w->acc.x ;
	pl_w->spd.x = spd ;
	pl_w->pos.x += spd ;
	pl_w->pos.y += pl_w->spd.y ;
	if( !( pl_w->wall_flag & 1 << ( pl_w->pl_dir ^ 1 ) ) && 1 << pl_w->dir & 0xff00000f ) return ;
let_go:
	lw->web_swing_st = -1 ;
	pl_w->routine_2 ++ ;
	pl_w->routine_3 = 0 ;
	pl_w->cg_ctr = 1 ;
	pl_w->spd.x = -10.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = acc ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set2( pl_w, Char_Extra, 0, 3 ) ;
}

/*--------------------------------------------------------------*/
static void swing_fall( PLAYER_WORK *pl_w, PL0C_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_set( pl_w ) ;
	}
}

/****************************************************************/
PLAYER_WORK *pl0c_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	if( !pl_w->atck_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_CROUCH ) return 0 ;
	if( pl_w->atck_pk ){
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 0 ;
	} else {
		if( pl_w->pl_stance != STANCE_AIR ){
			if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 1 ;
		} else {
			if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 2 ;
		}
	}
	return em ;
}

static void throw_set( PLAYER_WORK *pl_w );
static void catch_dummy( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl0c_catch_ck chose			*/
/****************************************************************/
void	pl0c_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		throw_set, throw_set, throw_set, catch_dummy, catch_dummy, catch_dummy,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			Each throw's entry in catch_ofs_tbl is four			*/
/*			words, of which the first two are read				*/
/*==============================================================*/
static void throw_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Sint16 *tbl ;
	Uint16 mask ;

	scr_excep_set( pl_w, 5, 5 ) ;
	if( pl_w->catch_no == 0 ) mask = 0x800 ;
	else mask = 0x400 ;
	if( pl_w->atck_lever & mask ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	catch_gauge_add( pl_w ) ;
	tbl = &catch_ofs_tbl[pl_w->catch_no * 4] ;
	ofs.x = 1.6666666f * *tbl ++ ;
	ofs.y = 2.142857f * *tbl ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->routine_2 = 0 ;
	pl_w->routine_3 = 0 ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, pl_w->catch_no ) ;
}

/*==============================================================*/
static void catch_dummy( PLAYER_WORK *pl_w )
{
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl0c_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, jump_p_catch, jump_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_p_init( PLAYER_WORK *pl_w );
static void nml_p_move( PLAYER_WORK *pl_w );
static void nml_p_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_init, nml_p_move, nml_p_end,
	} ;

	nml_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_p_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void nml_p_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0c_at_throw_p ;
		scr_excep_clr() ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_dir_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
	}
}

static void nml_k_init( PLAYER_WORK *pl_w );
static void nml_k_move( PLAYER_WORK *pl_w );
static void nml_k_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_init, nml_k_move, nml_k_end,
	} ;

	nml_k_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	nml_k_move( pl_w ) ;
}

/*==============================================================*/
/*			Leaps up with the opponent, and lets them go		*/
/*			over his head										*/
/*==============================================================*/
static void nml_k_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !pl_w->routine_3 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_3 ++ ;
			pl_w->cg_type = 0 ;
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->spd.x = -DOT_X( 2.0f ) ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 5.5f ) ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
			pl_w->jump_atk_cnt = JATK_ONE ;
			pl_w->jump_com_cnt = 1 ;
			if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		}
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_flag1 ){
			pl_w->routine_2 ++ ;
			pl_w->routine_3 = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_dir = pl_w->pl_dir ;
			em->pl_stance = STANCE_AIR ;
			em->atck_no = pl0c_at_throw_k ;
			em->disp_dir ^= 1 ;
			em->pl_dir ^= 1 ;
			scr_excep_clr() ;
		}
	}
}

/*--------------------------------------------------------------*/
static void nml_k_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_set( pl_w ) ;
	}
}

static void jump_p_init( PLAYER_WORK *pl_w );
static void jump_p_move( PLAYER_WORK *pl_w );
static void jump_p_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_init, jump_p_move, jump_p_end,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	jump_p_move( pl_w ) ;
}

/*==============================================================*/
/*			Throws the opponent over his head, and drifts		*/
/*			back												*/
/*==============================================================*/
static void jump_p_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_flag1 = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0c_at_air_throw ;
		em->disp_dir ^= 1 ;
		em->pl_dir ^= 1 ;
		scr_excep_clr() ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = -DOT_X( 0.125f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->spd.y = DOT_Y( 5.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	/* the drift stops when it would turn round */
	if( !( pl_w->spd.x * pl_w->acc.x < 0.0f ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_set( pl_w ) ;
	}
}

static void hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void hold_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void sp_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void web_throw_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl0c_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		hold, hold, hold, hold_dummy, sp_hold, web_throw_hold,
	} ;
	PLAYER_WORK *em = pl_w->catch_w ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w, em ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	catch_pos_set( em, pl_w ) ;
}

/*==============================================================*/
static void hold_dummy( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
}

/*==============================================================*/
static void sp_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	if( !pl_w->routine_2 ) pl_w->wall_ng_timer = 2 ;
	catch_pos_set( em, pl_w ) ;
}

/*==============================================================*/
static void web_throw_hold( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->wall_ng_timer = 2 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl0c.c" End of File	*/
