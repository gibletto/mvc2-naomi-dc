/* Dhalsim (character 25): command checks, normal attacks, dashes, appearance, win and lose poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl25_def.h"

/* the local work of the player doing a special move, for its effects */
static PL25_LOCAL_W *pl25_local_w;

void pl25_main( PLAYER_WORK *pl_w );
void pl25_com_ck( PLAYER_WORK *pl_w );
void pl25_atck_set( PLAYER_WORK *pl_w );
void pl25_atck( PLAYER_WORK *pl_w );
void pl25_dash( PLAYER_WORK *pl_w );
void pl25_noki( PLAYER_WORK *pl_w );
void pl25_appear( PLAYER_WORK *pl_w );
void pl25_win( PLAYER_WORK *pl_w );
void pl25_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl25_catch_ck( PLAYER_WORK *pl_w );
void pl25_catch_set( PLAYER_WORK *pl_w );
void pl25_catch( PLAYER_WORK *pl_w );
void pl25_hold( PLAYER_WORK *pl_w );
int pl25_delay_ck( PLAYER_WORK *pl_w );
void pl25_t_kick( PLAYER_WORK *pl_w );
void pl25_upper( PLAYER_WORK *pl_w );
void pl25_sp_unison( PLAYER_WORK *pl_w );
void pl25_sp_gattai( PLAYER_WORK *pl_w );
void pl25_sp_assist( PLAYER_WORK *pl_w );
void pl25_sp_zero( PLAYER_WORK *pl_w );
void pl25_dm_color( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2, -1,  2 } ;
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
static Uint16 com_fire[] = { 3, 0, pl25_sp_fire, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Yoga Fire */
static Uint16 com_frei[] = { 5, COM_GROUND, pl25_sp_frei, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Yoga Flame */
static Uint16 com_brast[] = { 5, COM_GROUND, pl25_sp_brast, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Yoga Blast */
static Uint16 com_teleport_0[] = { 3, COM_ALL_SW, pl25_sp_teleport, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Yoga Teleport */
static Uint16 com_teleport_1[] = { 3, COM_ALL_SW, pl25_sp_teleport, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_PUNCH } ;	/* Yoga Teleport */
static Uint16 com_teleport_2[] = { 3, COM_ALL_SW, pl25_sp_teleport, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Yoga Teleport */
static Uint16 com_teleport_3[] = { 3, COM_ALL_SW, pl25_sp_teleport, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_KICK } ;	/* Yoga Teleport */
static Uint16 com_inferno[] = { 3, COM_HYPER | COM_ALL_SW, pl25_sp_inferno, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Yoga Inferno (hyper) */
static Uint16 com_strike[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl25_sp_strike, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Yoga Strike (hyper) */
static Uint16 com_float[] = { 3, COM_ALL_SW, pl25_sp_float, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Yoga Float */
static Uint16 com_catch[] = { 5, COM_GROUND, 0, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_HP } ;	/* the command throw */

static void ( *pl25_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl25_main,
	pl25_com_ck,
	pl25_atck_set,
	pl25_atck,
	pl25_dash,
	pl25_noki,
	pl25_appear,
	pl25_win,
	pl25_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl25_catch_ck,	/* returns a value */
	pl25_catch_set,
	pl25_catch,
	pl25_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl25_delay_ck,
	pl25_t_kick,
	pl25_upper,
	pl25_sp_unison,
	pl25_sp_gattai,
	pl25_sp_assist,
	pl25_sp_zero,
	0, 0, 0, 0,
	pl25_dm_color,
} ;

static Uint8 fire_ck( PLAYER_WORK *pl_w );
static Uint8 frei_ck( PLAYER_WORK *pl_w );
static Uint8 brast_ck( PLAYER_WORK *pl_w );
static Uint8 teleport_0_ck( PLAYER_WORK *pl_w );
static Uint8 teleport_1_ck( PLAYER_WORK *pl_w );
static Uint8 teleport_2_ck( PLAYER_WORK *pl_w );
static Uint8 teleport_3_ck( PLAYER_WORK *pl_w );
static Uint8 inferno_ck( PLAYER_WORK *pl_w );
static Uint8 strike_ck( PLAYER_WORK *pl_w );
static Uint8 float_ck( PLAYER_WORK *pl_w );
static Uint8 catch_com_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int inferno_delay_ck( PLAYER_WORK *pl_w );
static int strike_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl25_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl25_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl25_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( teleport_0_ck( pl_w ) ) return ;
	if( teleport_1_ck( pl_w ) ) return ;
	if( teleport_2_ck( pl_w ) ) return ;
	if( teleport_3_ck( pl_w ) ) return ;
	if( inferno_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( catch_com_ck( pl_w ) ) return ;
	if( float_ck( pl_w ) ) return ;
	if( fire_ck( pl_w ) ) return ;
	if( frei_ck( pl_w ) ) return ;
	if( brast_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( pl_air_dash_sw_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			In the air, once a jump unless it is a super		*/
/*			jump, and not while one is on the screen			*/
/*==============================================================*/
static Uint8 fire_ck( PLAYER_WORK *pl_w )
{
	PL25_LOCAL_W *lw = ( PL25_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_fire, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_fire ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 frei_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_frei, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_frei ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 brast_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_brast, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_brast ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Yoga Teleport in four ways, by atck_level;			*/
/*			it clears the hyper combos' commands				*/
/*==============================================================*/
static Uint8 teleport_0_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_teleport_0, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_teleport ;
	act_st_set( pl_w, N_SUPER ) ;
	memset( &pl_w->com_w7, 0, 8 ) ;
	memset( &pl_w->com_w8, 0, 8 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 teleport_1_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_teleport_1, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_teleport ;
	act_st_set( pl_w, N_SUPER ) ;
	memset( &pl_w->com_w7, 0, 8 ) ;
	memset( &pl_w->com_w8, 0, 8 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 teleport_2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_teleport_2, &pl_w->com_w5 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 2 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_teleport ;
	act_st_set( pl_w, N_SUPER ) ;
	memset( &pl_w->com_w7, 0, 8 ) ;
	memset( &pl_w->com_w8, 0, 8 ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 teleport_3_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_teleport_3, &pl_w->com_w6 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->atck_level = 3 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_teleport ;
	act_st_set( pl_w, N_SUPER ) ;
	memset( &pl_w->com_w7, 0, 8 ) ;
	memset( &pl_w->com_w8, 0, 8 ) ;
	return 1 ;
}

/*==============================================================*/
/*			Hyper combos										*/
/*==============================================================*/
static Uint8 inferno_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_inferno, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->pl_sp_type = pl25_sp_jinferno ;
	else pl_w->pl_sp_type = pl25_sp_inferno ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_strike ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 float_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_float, &pl_w->com_w9 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w9 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl25_sp_float ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A throw done by a command, when the opponent is		*/
/*			in reach											*/
/*==============================================================*/
static Uint8 catch_com_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;	/* unused */

	if( !com_nml_ck( pl_w, com_catch, &pl_w->com_w10 ) ) return 0 ;
	if( !( em = pl_catch_ck( pl_w ) ) ) return 0 ;
	pl_w->catch_no = 0xc3 ;
	pl_catch_set( pl_w, em ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 9 ) ) return 0 ;
	pl_w->pl_sp_type = pl25_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl25_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl25_delay_ck( PLAYER_WORK *pl_w )
{
	if( inferno_delay_ck( pl_w ) || strike_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int inferno_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_inferno, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl25_sp_inferno ;
	return 1 ;
}

/*==============================================================*/
static int strike_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_strike, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl25_sp_strike ;
	return 1 ;
}

static void float_end( PLAYER_WORK *pl_w );
static void float_off( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The float runs out									*/
/****************************************************************/
void pl25_main( PLAYER_WORK *pl_w )
{
	PL25_LOCAL_W *lw = ( PL25_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->fly_flag && !pl_w->routine_1 ){
		if( -- lw->float_timer <= 0 ){
			if( pl_w->act_st != N_SUPER && pl_w->act_st != N_SPECIAL ) float_end( pl_w ) ;
		}
	}
}

/* how close the other player must be for each close attack: standing and crouching punches and
   kicks, then the jumping ones, then how close in height for two jumping punches */
static Float near_range[20] = {
	DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ),
	DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ),
	DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ), DOT_X( 112.0f ),
	DOT_Y( 80.0f ), DOT_Y( 96.0f ),
} ;

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
void pl25_atck_set( PLAYER_WORK *pl_w )
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
		if( pl_far_ck( pl_w, near_range[0] ) ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl25_at_far_lp ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_STAND_LP ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_w->unk_2a2 = 1 ;
		break ;
	case 1:
		if( pl_far_ck( pl_w, near_range[1] ) ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl25_at_far_mp ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_STAND_MP ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_ck( pl_w, near_range[2] ) ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl25_at_far_hp ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HP ;
		}
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
		if( pl_far_ck( pl_w, near_range[3] ) ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl25_at_far_crouch_lp ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_CROUCH_LP ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_far_ck( pl_w, near_range[4] ) ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl25_at_far_crouch_mp ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_CROUCH_MP ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_ck( pl_w, near_range[5] ) ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl25_at_far_crouch_hp ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_CROUCH_HP ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_far_ck( pl_w, near_range[6] ) ){
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl25_at_far_lk ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_STAND_LK ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_far_ck( pl_w, near_range[7] ) ){
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl25_at_far_mk ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_STAND_MK ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_ck( pl_w, near_range[8] ) ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl25_at_far_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_STAND_HK ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The close ones go on to a second part				*/
/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_far_ck( pl_w, near_range[9] ) ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl25_at_far_crouch_lk ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_CROUCH_LK ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_far_ck( pl_w, near_range[10] ) ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 4 ;
			pl_w->atck_no = pl25_at_far_crouch_mk ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_CROUCH_MK ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_ck( pl_w, near_range[11] ) ){
			pl_w->routine_2 ++ ;
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl25_at_far_crouch_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_CROUCH_HK ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag ) jump_atck( pl_w ) ;
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
		if( pl_far_xy_ck( pl_w, near_range[12], near_range[18] ) ){
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl25_at_far_jump_lp ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_JUMP_LP ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_far_ck( pl_w, near_range[13] ) ){
			pl_w->char_no.b.no = 7 ;
			pl_w->atck_no = pl25_at_far_jump_mp ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MP ;
		}
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_xy_ck( pl_w, near_range[14], near_range[19] ) ){
			pl_w->char_no.b.no = 8 ;
			pl_w->atck_no = pl25_at_far_jump_hp ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HP ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		}
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 9 ;
			pl_w->nml_no = 0x60 ;
			pl_w->atck_no = pl25_at_jdown_hp ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_far_ck( pl_w, near_range[15] ) ){
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl25_at_far_jump_lk ;
		} else {
			pl_w->char_no.b.no = 0 ;
			pl_w->atck_no = ATCK_JUMP_LK ;
		}
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 9 ;
			pl_w->nml_no = 0x61 ;
			pl_w->atck_no = pl25_at_jdown_lk ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		if( pl_far_ck( pl_w, near_range[16] ) ){
			pl_w->char_no.b.no = 7 ;
			pl_w->atck_no = pl25_at_far_jump_mk ;
			pl_se_req( pl_w, SE_SWING_M ) ;
		} else {
			pl_w->char_no.b.no = 1 ;
			pl_w->atck_no = ATCK_JUMP_MK ;
			pl_se_req( pl_w, SE_SWING_M ) ;
		}
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 10 ;
			pl_w->nml_no = 0x62 ;
			pl_w->atck_no = pl25_at_jdown_mk ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		if( pl_far_ck( pl_w, near_range[17] ) ){
			pl_w->char_no.b.no = 8 ;
			pl_w->atck_no = pl25_at_far_jump_hk ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_JUMP_HK ;
			pl_se_req( pl_w, SE_SWING_H ) ;
		}
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.no = 11 ;
			pl_w->nml_no = 0x63 ;
			pl_w->atck_no = pl25_at_jdown_hk ;
		}
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

static void nml_move( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_slide( PLAYER_WORK *pl_w );
static void crouch_kick_slide_start( PLAYER_WORK *pl_w );
static void crouch_kick_slide_move( PLAYER_WORK *pl_w );
static void crouch_kick_slide_end( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void lever_atck( PLAYER_WORK *pl_w );
static void lever_atck0( PLAYER_WORK *pl_w );
static void lever_atck0_start( PLAYER_WORK *pl_w );
static void lever_atck0_move( PLAYER_WORK *pl_w );
static void lever_atck0_end( PLAYER_WORK *pl_w );
static void lever_atck1( PLAYER_WORK *pl_w );
static void lever_atck1_start( PLAYER_WORK *pl_w );
static void lever_atck1_move( PLAYER_WORK *pl_w );
static void lever_atck1_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl25_atck( PLAYER_WORK *pl_w )
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
	static void ( *crouch_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_kick_slide, crouch_kick_slide, crouch_kick_slide,
	} ;

	crouch_kick_jmp[pl_w->atck_level]( pl_w ) ;
}

/*==============================================================*/
/*			The close crouching kicks slide forward				*/
/*==============================================================*/
static void crouch_kick_slide( PLAYER_WORK *pl_w )
{
	static void ( *crouch_kick_slide_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_kick_slide_start, crouch_kick_slide_move, crouch_kick_slide_end,
	} ;

	if( !pl_w->routine_2 ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		crouch_kick_slide_jmp[pl_w->routine_3]( pl_w ) ;
	}
}

/*==============================================================*/
static void crouch_kick_slide_start( PLAYER_WORK *pl_w )
{
	/* the speed and its slowing for each strength */
	static Float slide_spd[3][2] = {
		{ -10.0f, DOT_X( 0.5f ) },
		{ -DOT_X( 8.0f ), DOT_X( 0.5f ) },
		{ -DOT_X( 10.0f ), DOT_X( 0.5f ) },
	} ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = slide_spd[pl_w->atck_level][0] ;
		pl_w->acc.x = slide_spd[pl_w->atck_level][1] ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void crouch_kick_slide_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->spd.x ){
		pl_w->routine_3 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_char_set( pl_w, Char_Crouch_K, pl_w->atck_level + 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_kick_slide_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->fly_flag ){
		pl_jump_hit_ck( pl_w ) ;
		pl_sjump_ctrl( pl_w ) ;
	}
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->fly_flag ) pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->nml_no == 0x60 ){
		pl_w->timer0 = 0 ;
		lever_atck( pl_w ) ;
		return ;
	}
	if( pl_w->nml_no >= 0x61 && pl_w->nml_no <= 0x63 ){
		pl_w->timer0 = 1 ;
		lever_atck( pl_w ) ;
		return ;
	}
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

/*==============================================================*/
/*			The jumping attacks done with the stick held		*/
/*			down: a dive										*/
/*==============================================================*/
static void lever_atck( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_atck0, lever_atck1,
	} ;

	lever_atck_jmp[pl_w->timer0]( pl_w ) ;
}

/*==============================================================*/
static void lever_atck0( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck0_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_atck0_start, lever_atck0_move, lever_atck0_end,
	} ;

	lever_atck0_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lever_atck0_start( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->fly_flag ) float_off( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = DOT_X( 7.0f ) ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 3.0f ) ;
		pl_w->acc.y = 0.0f ;
		pl_w->sjump_flag = 0 ;
		pl_w->timer1 = 40 ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck0_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
	if( -- pl_w->timer1 <= 0 || pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump_P, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck0_end( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void lever_atck1( PLAYER_WORK *pl_w )
{
	static void ( *lever_atck1_jmp[])( PLAYER_WORK *pl_w ) = {
		lever_atck1_start, lever_atck1_move, lever_atck1_end,
	} ;

	lever_atck1_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lever_atck1_start( PLAYER_WORK *pl_w )
{
	/* how long the dive lasts for each strength */
	static Float dive_time[3] = { 88.0f, 96.0f, 128.0f } ;

	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		pl_land_ef_set( pl_w ) ;
		return ;
	}
	if( pl_w->fly_flag ) float_off( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->sjump_flag = 0 ;
		if( pl_w->nml_no == 0x61 ){
			pl_w->spd.x = DOT_X( 6.5f ) ;
			pl_w->spd.y = -DOT_Y( 3.0f ) ;
			pl_w->timer1 = dive_time[0] ;
		} else if( pl_w->nml_no == 0x62 ){
			pl_w->spd.x = DOT_X( 4.0f ) ;
			pl_w->spd.y = -DOT_Y( 4.0f ) ;
			pl_w->timer1 = dive_time[1] ;
		} else {
			pl_w->spd.x = 2.5f ;
			pl_w->spd.y = -DOT_Y( 5.5f ) ;
			pl_w->timer1 = dive_time[2] ;
		}
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->sjump_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void lever_atck1_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		if( pl_w->nml_no == 0x61 ) pl_se_req( pl_w, SE_SWING_L ) ;
		else if( pl_w->nml_no == 0x62 ) pl_se_req( pl_w, SE_SWING_M ) ;
		else pl_se_req( pl_w, SE_SWING_H ) ;
	}
	pl_char_move( pl_w ) ;
	pl_w->timer1 -- ;
	if( pl_w->timer1 == 0 || pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->pl_stance == STANCE_AIR ){
			if( pl_w->nml_no == 0x61 ) pl_char_set( pl_w, Char_Jump_K, 15 ) ;
			else if( pl_w->nml_no == 0x62 ) pl_char_set( pl_w, Char_Jump_K, 16 ) ;
			else pl_char_set( pl_w, Char_Jump_K, 17 ) ;
		} else {
			if( pl_w->nml_no == 0x61 ) pl_char_set( pl_w, Char_Jump_K, 12 ) ;
			else if( pl_w->nml_no == 0x62 ) pl_char_set( pl_w, Char_Jump_K, 13 ) ;
			else pl_char_set( pl_w, Char_Jump_K, 14 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void lever_atck1_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl25_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_voice_req( pl_w, 20 ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl25_at_upper ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 16 ) ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		HIT_RESET( pl_w ) ;
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

static void dash_start( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Dashes: a slide that slows to a stop				*/
/****************************************************************/
void pl25_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
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
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl25_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_start, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.3125f : -0.3125f ;
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
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl25_appear( PLAYER_WORK *pl_w )
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
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl25_02_set( pl_w ) ;
	}
}

static void win_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_end( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl25_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_time_ko, win_ko, lose_time_ko, draw_time,
	} ;

	pl_w->pos.y = pl_w->ground_y ;
	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move, win_ko_end,
	} ;

	win_ko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			One of three poses, or the floating one for a		*/
/*			win on time											*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->type == 2 ){
		pl_char_set( pl_w, Char_Win, 3 ) ;
		return ;
	}
	pl_w->timer0 = Rnd() & 3 ;
	switch( pl_w->timer0 ){
	case 0:
		pl_char_set( pl_w, Char_Win, 0 ) ;
		break ;
	case 1:
		pl_char_set( pl_w, Char_Win, 1 ) ;
		break ;
	case 2:
		pl_char_set( pl_w, Char_Win, 1 ) ;
		break ;
	case 3:
		pl_char_set( pl_w, Char_Win, 2 ) ;
		break ;
	}
}

/*==============================================================*/
/*			Floats up and down									*/
/*==============================================================*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( !pl_w->type ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 6 ;
			pl_w->sub_type = 0 ;
		}
		if( !pl_w->sub_type ) pl_w->pos.y += 2.142857f ;
		else pl_w->pos.y -= 2.142857f ;
		if( -- pl_w->timer0 == 0 ){
			pl_w->timer0 = 6 ;
			pl_w->sub_type = pl_w->sub_type ^ 1 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void win_ko_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
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
		pl_char_set( pl_w, Char_Win, 5 ) ;
	} else pl_char_move( pl_w ) ;
}

static void sp_fire( PLAYER_WORK *pl_w );
static void sp_frei( PLAYER_WORK *pl_w );
static void sp_brast( PLAYER_WORK *pl_w );
static void sp_teleport( PLAYER_WORK *pl_w );
static void sp_float( PLAYER_WORK *pl_w );
static void sp_inferno( PLAYER_WORK *pl_w );
static void sp_strike( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_jinferno( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl25_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_fire, sp_frei, sp_brast, sp_teleport, sp_float, sp_inferno, sp_strike,
		sp_fire, sp_fire, sp_chouhatsu, sp_fire, sp_fire, sp_jinferno, sp_snapback,
	} ;

	pl25_local_w = ( PL25_LOCAL_W * )pl_w->tlocal ;
	sp_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			Stand on the ground, still							*/
/*==============================================================*/
static void sp_land( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
}

static void sp_fire_init( PLAYER_WORK *pl_w );
static void sp_fire_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The fireball, also in the air						*/
/*==============================================================*/
static void sp_fire( PLAYER_WORK *pl_w )
{
	static void ( *sp_fire_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_fire_init, sp_fire_move,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			if( !pl_w->sub_type ){
				pl_land_ef_set( pl_w ) ;
				pl_w->sub_type = 1 ;
			}
		}
	}
	sp_fire_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sp_fire_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl25_at_fire ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		sp_land( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		if( pl_w->pl_sp_type == 7 ) pl_w->char_no.b.no = 11 ;
		else pl_w->char_no.b.no = 0 ;
	} else {
		pl_w->char_no.b.no = 7 ;
		pl_w->spd.x /= 16.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y /= 64.0f ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void sp_fire_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_stand_ret( pl_w ) ;
		else {
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_jump_ret( pl_w ) ;
		}
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl25_local_w->unk_03 = 0 ;
		shl25_00_set( pl_w ) ;
	}
}

static void sp_frei_init( PLAYER_WORK *pl_w );
static void sp_frei_start( PLAYER_WORK *pl_w );
static void sp_frei_shot( PLAYER_WORK *pl_w );
static void sp_frei_wait( PLAYER_WORK *pl_w );
static void sp_frei_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The flame in front									*/
/*==============================================================*/
static void sp_frei( PLAYER_WORK *pl_w )
{
	static void ( *sp_frei_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_frei_init, sp_frei_start, sp_frei_shot, sp_frei_wait, sp_frei_end,
	} ;

	sp_frei_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sp_frei_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_sp_ef_set( pl_w ) ;
	sp_land( pl_w ) ;
	pl_voice_req( pl_w, 22 ) ;
	if( !pl_w->atck_level ){
		pl_w->atck_no = pl25_at_frei_l ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl25_at_frei_h ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level + 1 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void sp_frei_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void sp_frei_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl25_local_w->ef_end_flag = 0 ;
		shl25_01_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			Until the effect is done							*/
/*==============================================================*/
static void sp_frei_wait( PLAYER_WORK *pl_w )
{
	if( pl25_local_w->ef_end_flag ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void sp_frei_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void sp_brast_init( PLAYER_WORK *pl_w );
static void sp_brast_shot( PLAYER_WORK *pl_w );
static void sp_brast_wait( PLAYER_WORK *pl_w );
static void sp_brast_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The flame up high									*/
/*==============================================================*/
static void sp_brast( PLAYER_WORK *pl_w )
{
	static void ( *sp_brast_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_brast_init, sp_brast_shot, sp_brast_wait, sp_brast_end,
	} ;

	sp_brast_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sp_brast_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_sp_ef_set( pl_w ) ;
	sp_land( pl_w ) ;
	pl_voice_req( pl_w, 23 ) ;
	if( !pl_w->atck_level ){
		pl_w->atck_no = pl25_at_brast_l ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl25_at_brast_h ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->char_no.b.no = pl_w->atck_level + 3 ;
	pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void sp_brast_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl25_local_w->ef_end_flag = 0 ;
		shl25_03_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_brast_wait( PLAYER_WORK *pl_w )
{
	if( pl25_local_w->ef_end_flag ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void sp_brast_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void teleport_init( PLAYER_WORK *pl_w );
static void teleport_out( PLAYER_WORK *pl_w );
static void teleport_move( PLAYER_WORK *pl_w );
static void teleport_in( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The teleport: it vanishes and comes out by the		*/
/*			opponent, in front or behind, near or far			*/
/*==============================================================*/
static void sp_teleport( PLAYER_WORK *pl_w )
{
	static void ( *teleport_jmp[])( PLAYER_WORK *pl_w ) = {
		teleport_init, teleport_out, teleport_move, teleport_in,
	} ;

	pl_w->wall_wide_timer = 3 ;
	teleport_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void teleport_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->fly_flag ) float_off( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		sp_land( pl_w ) ;
	} else {
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_dash_cnt = 0 ;
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_w->air_jump_cnt = 0 ;
	}
	pl_char_set( pl_w, Char_Special, 5 ) ;
}

/*--------------------------------------------------------------*/
static void teleport_out( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float x ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
		pl_w->push_ng_timer = 1 ;
		pl_w->disp_flag = 0 ;
		pl_w->cg_shadow = 0xff ;
		em = pl_w->target_w ;
		if( !pl_w->atck_level ) x = 160.0f ;
		if( pl_w->atck_level == 1 ) x = -160.0f ;
		if( pl_w->atck_level == 2 ) x = DOT_X( 256.0f ) ;
		if( pl_w->atck_level == 3 ) x = -DOT_X( 256.0f ) ;
		if( !pl_w->pl_dir ) x = -x ;
		pl_w->spd.x = ( em->pos.x + x - pl_w->pos.x ) / 16.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	pl25_dm_color( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void teleport_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_w->push_ng_timer = 1 ;
	pl_w->cg_shadow = 0xff ;
	if( -- pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
		pl_dir_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void teleport_in( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->wall_wide_timer = 0 ;
			pl_stand_ret( pl_w ) ;
		} else pl_jump_ret( pl_w ) ;
	}
	pl25_dm_color( pl_w ) ;
}

static void inferno_init( PLAYER_WORK *pl_w );
static void sp_flash( PLAYER_WORK *pl_w );
static void inferno_start( PLAYER_WORK *pl_w );
static void inferno_move( PLAYER_WORK *pl_w );
static void inferno_turn_set( PLAYER_WORK *pl_w );
static void inferno_flame( PLAYER_WORK *pl_w );
static void inferno_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a long flame the stick turns up		*/
/*			and down											*/
/*==============================================================*/
static void sp_inferno( PLAYER_WORK *pl_w )
{
	static void ( *inferno_jmp[])( PLAYER_WORK *pl_w ) = {
		inferno_init, sp_flash, inferno_start, inferno_move, inferno_end,
	} ;

	inferno_jmp[pl_w->routine_2]( pl_w ) ;
}

/* the flame's height for each of its ten directions */
static Sint8 flame_tbl[10] = { 0x12, 0x10, 0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00 } ;

/*==============================================================*/
static void inferno_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_sp_ef_set( pl_w ) ;
	sp_land( pl_w ) ;
	pl_w->atck_no = pl25_at_fire ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->timer0 = 3 ;
	pl_w->timer1 = 0 ;
	pl_w->type = 0x60 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void inferno_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl25_local_w->turn_timer = 2 ;
		if( pl_w->cpu_flag || pl_w->team_type == 4 || pl_w->team_type == 5 ){
			pl_w->dir = 7 ;
			pl25_local_w->turn_dir = -1 ;
		} else pl_w->dir = 5 ;
	}
}

/*==============================================================*/
/*			The computer's flame sweeps by itself; the			*/
/*			player's turns with the stick						*/
/*==============================================================*/
static void inferno_move( PLAYER_WORK *pl_w )
{
	PL25_LOCAL_W *lw ;
	Sint8 turn ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw = pl25_local_w ;
	if( pl_w->cpu_flag || pl_w->team_type == 4 || pl_w->team_type == 5 ){
		if( -- lw->turn_timer ){
			inferno_flame( pl_w ) ;
			return ;
		}
		turn = pl25_local_w->turn_dir ;
		pl_w->dir += turn ;
		if( pl_w->dir > 7 || pl_w->dir < 3 ) pl25_local_w->turn_dir *= -1 ;
	} else {
		Uint16 lever = pl_w->pl_sw_new & pl_w->pl_sw_old ;

		if( lever & ( KEY_UP | KEY_DOWN ) ){
			if( -- lw->turn_timer ){
				inferno_flame( pl_w ) ;
				return ;
			} else if( pl_w->pl_sw_new & KEY_UP ){
				if( pl_w->dir ) pl_w->dir -- ;
			} else if( pl_w->dir < 9 ) pl_w->dir ++ ;
		}
	}
	inferno_turn_set( pl_w ) ;
}

/*==============================================================*/
/*			The flame turns again in two frames					*/
/*==============================================================*/
static void inferno_turn_set( PLAYER_WORK *pl_w )
{
	pl25_local_w->turn_timer = 2 ;
	inferno_flame( pl_w ) ;
}

/*==============================================================*/
static void inferno_flame( PLAYER_WORK *pl_w )
{
	Sint8 height ;

	if( !( pl25_local_w->flame_cnt = ( ++ pl25_local_w->flame_cnt + 4 ) % 4 ) ){
		height = flame_tbl[pl_w->dir] ;
		if( shl25_04_set( pl_w, height, pl25_local_w->flame_no ) ){
			pl25_local_w->flame_no = ++ pl25_local_w->flame_no & 3 ;
		}
	}
	pl_char_move( pl_w ) ;
	if( -- pl_w->type == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, pl_w->pl_stance != STANCE_AIR ? 2 : 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void inferno_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static Sint8 unk_tbl[4] = { 0x12, 0x0c, 0x08, 0x02 } ;	/* not used */

static void jinferno_init( PLAYER_WORK *pl_w );
static void jinferno_flash( PLAYER_WORK *pl_w );
static void jinferno_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hyper flame from the air						*/
/*==============================================================*/
static void sp_jinferno( PLAYER_WORK *pl_w )
{
	static void ( *jinferno_jmp[])( PLAYER_WORK *pl_w ) = {
		jinferno_init, jinferno_flash, inferno_start, inferno_move, jinferno_end,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		if( !pl_w->sub_type ){
			pl_land_ef_set( pl_w ) ;
			pl_w->sub_type = 1 ;
		}
	}
	jinferno_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jinferno_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->atck_no = pl25_at_fire ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->timer0 = 3 ;
	pl_w->type = 0x60 ;
	pl_w->spd.x /= 16.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
}

/*--------------------------------------------------------------*/
static void jinferno_flash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs = pl_w->pos ;
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 90.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jinferno_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_jump_ret( pl_w ) ;
	}
}

static void strike_init( PLAYER_WORK *pl_w );
static void strike_start( PLAYER_WORK *pl_w );
static void strike_jump( PLAYER_WORK *pl_w );
static void strike_miss( PLAYER_WORK *pl_w );
static void strike_catch( PLAYER_WORK *pl_w );
static void strike_up( PLAYER_WORK *pl_w );
static void strike_air( PLAYER_WORK *pl_w );
static void strike_fall( PLAYER_WORK *pl_w );
static void strike_slam( PLAYER_WORK *pl_w );
static void strike_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: it leaps, and if it catches the		*/
/*			opponent it slams them down							*/
/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	static void ( *strike_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_init, sp_flash, strike_start, strike_jump, strike_miss, strike_catch, strike_up,
		strike_air, strike_fall, strike_slam, strike_end,
	} ;

	strike_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void strike_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_sp_ef_set( pl_w ) ;
	sp_land( pl_w ) ;
	pl25_local_w->turn_timer = pl_w->atck_level ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
}

/*--------------------------------------------------------------*/
static void sp_flash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs = pl_w->pos ;
		ofs.x = DOT_X( 2.0f ) ;
		ofs.y = 150.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = 10.0f ;
		pl_w->acc.y = -0.3125f ;
		pl_w->spd.x = pl_w->pl_dir ? 1.25f : -1.25f ;
	}
}

/*--------------------------------------------------------------*/
static void strike_jump( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	} else {
		if( pl_w->cg_flag1 ){	/* nothing is done here */
		}
		if( pl_w->cg_type ){
			if( !( em = pl_catch_ck( pl_w ) ) ) return ;
			pl_w->catch_no = 0xc4 ;
			pl_w->routine_2 += 2 ;
			pl_sp_catch_set( pl_w, em ) ;
			ofs.x = -80.0f ;
			ofs.y = DOT_Y( 72.0f ) ;
			nage_hit_set( pl_w, &ofs ) ;
			pl_w->hit_stop = 10 ;
			catch_gauge_add( pl_w ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->timer0 = 2 ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.5f ) ;
			pl_char_set( pl_w, Char_Catch, 2 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void strike_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void strike_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->routine_2 ++ ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = DOT_X( 2.0f ) ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_char_set( pl_w, Char_Catch, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_up( PLAYER_WORK *pl_w )
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
	if( pl_w->spd.y > DOT_Y( 0.25f ) ) return ;
	pl_w->routine_2 ++ ;
	pl_w->timer1 = 16 ;
	pl_w->spd.y = 2.142857f ;
	pl_w->acc.y = -0.13392857f ;
}

/*--------------------------------------------------------------*/
static void strike_air( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( -- pl_w->timer1 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -DOT_Y( 1.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
	}
}

/*==============================================================*/
/*			The slam: lets the opponent go, or shakes the		*/
/*			screen and leaves the dust							*/
/*==============================================================*/
static void strike_slam( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		/* the animation has not marked a frame */
	} else if( pl_w->cg_type > 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_STAND ;
		SCR_EXCEP_SET ;
		em->atck_no = pl25_at_strike_release ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -0.4017857f ;
		pl_w->spd.x = 1.25f ;
		if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	} else {
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl25_at_strike_slam ;
		ofs.x = pl_w->pos.x + -DOT_X( 80.0f ) ;
		if( !pl_w->pl_dir ) ofs.x = pl_w->pos.x + DOT_X( 80.0f ) ;
		ofs.y = pl_w->pos.y ;
		hitmark_set( &ofs, em->disp_dir, 1, 16 ) ;
		catch_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ) pl_stand_ret( pl_w ) ;
}

static void float_init( PLAYER_WORK *pl_w );
static void float_up( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The float: it rises and stays in the air for a		*/
/*			while												*/
/*==============================================================*/
static void sp_float( PLAYER_WORK *pl_w )
{
	static void ( *float_jmp[])( PLAYER_WORK *pl_w ) = {
		float_init, float_up,
	} ;

	float_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void float_init( PLAYER_WORK *pl_w )
{
	PL25_LOCAL_W *lw = ( PL25_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_2 ++ ;
	if( pl_w->fly_flag ){
		float_end( pl_w ) ;
		return ;
	}
	if( pl_w->sjump_flag ) pl_w->sjump_flag = 0 ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		sp_land( pl_w ) ;
	}
	lw->float_timer = 600 ;
	pl_voice_req( pl_w, 4 ) ;
	pl_w->fly_flag = 1 ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = DOT_Y( 0.125f ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_char_set( pl_w, Char_Fly, 0 ) ;
}

/*--------------------------------------------------------------*/
static void float_up( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The float ends, and it falls						*/
/*==============================================================*/
static void float_end( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The float ends, for a move done from it				*/
/*==============================================================*/
static void float_off( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
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
	pl_char_set( pl_w, Char_Extra, 15 ) ;
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
	sp_land( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl25_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
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
		ofs.x = -DOT_X( 4.0f ) ;
		ofs.y = DOT_Y( 24.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl25_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl25_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 13 ) ;
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
		pl_char_set( pl_w, Char_Extra, 14 ) ;
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
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl25_catch_ck( PLAYER_WORK *pl_w )
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
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
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
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void com_catch_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl25_catch_ck or the			*/
/*			command chose										*/
/****************************************************************/
void	pl25_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_p_set, com_catch_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 1 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -90.0f ;
	ofs.y = DOT_Y( 76.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -90.0f ;
	ofs.y = DOT_Y( 76.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !( pl_w->dir & 2 ) ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	scr_excep_set( pl_w, 5, 5 ) ;
	ofs.x = -DOT_X( 64.0f ) ;
	ofs.y = DOT_Y( 88.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 5 ) ;
}

/*==============================================================*/
/*			The command throw									*/
/*==============================================================*/
static void com_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	ofs.x = -90.0f ;
	ofs.y = DOT_Y( 76.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->timer0 = 0x88 ;
	pl_w->timer1 = 0 ;
	hold_renda_set( pl_w->catch_w, 20 ) ;
	catch_renda_set( pl_w, 3 ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void com_catch_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl25_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_p_catch, com_catch_move,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_dir ^= 1 ;
		pl_stand_ret( pl_w ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_STAND ;
		SCR_EXCEP_SET ;
		em->atck_no = pl25_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
	}
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		if( pl_w->cg_type < 0 ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl25_at_throw_k_hit ;
			catch_damage( pl_w ) ;
			ofs.y = pl_w->pos.y + DOT_Y( 57.0f ) ;
			ofs.x = pl_w->disp_dir ? 95.0f : -95.0f ;
			ofs.x += pl_w->pos.x ;
			ofs.z = pl_w->pos.z ;
			hitmark_set( &ofs, pl_w->disp_dir, 1, 0 ) ;
		} else {
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_stance = STANCE_STAND ;
			SCR_EXCEP_SET ;
			em->atck_no = pl25_at_throw_k ;
		}
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		if( pl_w->cg_type < 0 ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->atck_no = pl25_at_air_throw_hit ;
			catch_damage( pl_w ) ;
			ofs.y = pl_w->pos.y + DOT_Y( 57.0f ) ;
			ofs.x = pl_w->disp_dir ? 95.0f : -95.0f ;
			ofs.x += pl_w->pos.x ;
			ofs.z = pl_w->pos.z ;
			hitmark_set( &ofs, pl_w->disp_dir, 1, 0 ) ;
		} else {
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_stance = STANCE_AIR ;
			SCR_EXCEP_SET ;
			em->atck_no = pl25_at_air_throw ;
		}
	}
}

static void com_catch_hit( PLAYER_WORK *pl_w );
static void com_catch_throw( PLAYER_WORK *pl_w );
static void com_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void com_catch_move( PLAYER_WORK *pl_w )
{
	static void ( *com_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		com_catch_hit, com_catch_throw, com_catch_end,
	} ;

	pl_w->catch_flag = 1 ;
	com_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The opponent can mash out of it until the time		*/
/*			runs out											*/
/*==============================================================*/
static void com_catch_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	pl_w->timer0 -- ;
	if( pl_w->cg_flag1 ){
		if( ( !pl_w->timer1 && hold_renda_ck( pl_w->catch_w ) )
		 || ( pl_w->timer0 <= 0 && !pl_w->timer1 ) ){
			pl_w->timer1 = 1 ;
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Catch, 7 ) ;
		}
	} else if( pl_w->cg_type ){
		if( pl_w->cg_type > 0 ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->atck_no = pl25_at_com_throw_hit ;
			catch_damage( pl_w ) ;
			if( !em->pl_dir ) em->pos.x += -DOT_X( 32.0f ) ;
			else em->pos.x += DOT_X( 32.0f ) ;
			em->pos.y += -DOT_Y( 80.0f ) ;
			ofs.y = pl_w->pos.y + DOT_Y( 57.0f ) ;
			ofs.x = pl_w->disp_dir ? 95.0f : -95.0f ;
			ofs.x += pl_w->pos.x ;
			ofs.z = pl_w->pos.z ;
			hitmark_set( &ofs, pl_w->disp_dir, 1, 0 ) ;
			pl_se_req( pl_w, 0 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void com_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_STAND ;
		SCR_EXCEP_SET ;
		em->atck_no = pl25_at_com_throw ;
		em->pl_dir = pl_w->pl_dir ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void com_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void hold( PLAYER_WORK *pl_w );
static void com_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl25_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold, hold, com_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void com_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl25_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl25_sp_inferno ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl25_sp_inferno ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl25_sp_inferno ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl25_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl25_sp_inferno ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl25_sp_inferno ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl25_sp_inferno ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl25_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl25_sp_fire ;
		pl_w->atck_level = ATCK_H ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl25_sp_frei ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl25_sp_brast ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			The zero counter									*/
/****************************************************************/
void	pl25_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl25_sp_fire ;
		pl_w->atck_level = ATCK_H ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl25_sp_frei ;
		pl_w->atck_level = ATCK_H ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl25_sp_brast ;
		pl_w->atck_level = ATCK_H ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			The colours while getting up						*/
/****************************************************************/
void	pl25_dm_color( PLAYER_WORK *pl_w )
{
	ef_color_set( pl_w, 3, 4, 1 ) ;
	if( pl_w->cg_flag1 == 0 ){
		return ;
	} else {
		if( ( Sint8 )pl_w->cg_flag1 > 0 ){
			pl_w->timer0 = pl_w->cg_flag1 ;
			pl_w->timer0 -= 2 ;
			pl_w->timer0 /= 2 ;
			pl_w->cg_flag1 = 0 ;
			ef_color_set( pl_w, 0, pl_w->pl_color * 5 + pl_w->timer0, 1 ) ;
		} else {
			pl_w->cg_flag1 = 0 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			return ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl25.c" End of File	*/
