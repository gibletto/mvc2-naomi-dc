/* Ruby Heart (character 13): command checks, normal attacks, appearance, win and lose poses,
   special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "o_sub1.h"
#include "pl13_def.h"

void pl13_main( PLAYER_WORK *pl_w );
void pl13_com_ck( PLAYER_WORK *pl_w );
void pl13_atck_set( PLAYER_WORK *pl_w );
void pl13_atck( PLAYER_WORK *pl_w );
void pl13_dash( PLAYER_WORK *pl_w );
void pl13_noki( PLAYER_WORK *pl_w );
void pl13_appear( PLAYER_WORK *pl_w );
void pl13_win( PLAYER_WORK *pl_w );
void pl13_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl13_catch_ck( PLAYER_WORK *pl_w );
void pl13_catch_set( PLAYER_WORK *pl_w );
void pl13_catch( PLAYER_WORK *pl_w );
void pl13_hold( PLAYER_WORK *pl_w );
int pl13_delay_ck( PLAYER_WORK *pl_w );
void pl13_t_kick( PLAYER_WORK *pl_w );
void pl13_upper( PLAYER_WORK *pl_w );
void pl13_sp_unison( PLAYER_WORK *pl_w );
void pl13_sp_gattai( PLAYER_WORK *pl_w );
void pl13_sp_assist( PLAYER_WORK *pl_w );
void pl13_sp_zero( PLAYER_WORK *pl_w );

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

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_schwarz[] = { 3, 0, pl13_sp_schwarz, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Schwarzaile */
static Uint16 com_fantome[] = { 3, COM_GROUND, pl13_sp_fantome, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Fantome */
static Uint16 com_sublimation[] = { 3, COM_GROUND, pl13_sp_sublimation, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Sublimation */
static Uint16 com_flammeile[] = { 3, COM_HYPER | COM_ALL_SW, pl13_sp_flammeile, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Flammeile (hyper) */
static Uint16 com_mille[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl13_sp_mille, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Mille Fantomes (hyper) */
static Uint16 com_paretonnre[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl13_sp_paretonnre, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Pare-tonn're (hyper) */
static Uint16 com_magie[] = { 4, COM_GROUND | COM_HYPER, pl13_sp_magie, 0, KEY_HK, KEY_LP, KEY_DOWN, KEY_LK, KEY_HP } ;	/* Tour De Magie (hyper) */
static Uint16 com_rafale[] = { 5, COM_GROUND, pl13_sp_rafale, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Rafale Cannon */

/* the flights, by the stick: spd.x, acc.x, spd.y, acc.y, then whether it goes, the way to face
   (or below 0 to keep it) and the animation */
static Float schwarz_spd[16 * 7] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	17.5f, -0.0065104165f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	-17.5f, 0.0065104165f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -22.5f, 0.0f, 1.0f, -1.0f, 2.0f,
	DOT_X( 7.0f ), 0.0f, -15.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-DOT_X( 7.0f ), 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 22.5f, -0.008370535f, 1.0f, -1.0f, 4.0f,
	DOT_X( 7.0f ), -0.0065104165f, 15.0f, -0.008370535f, 1.0f, 1.0f, 3.0f,
	-DOT_X( 7.0f ), 0.0065104165f, 15.0f, -0.008370535f, 1.0f, 0.0f, 3.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
} ;
static Float flammeile_spd[16 * 7] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	22.5f, -0.0065104165f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	-22.5f, 0.0065104165f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -DOT_Y( 13.5f ), 0.0f, 1.0f, -1.0f, 2.0f,
	15.0f, 0.0f, -DOT_Y( 9.0f ), 0.0f, 1.0f, 1.0f, 1.0f,
	-15.0f, 0.0f, -DOT_Y( 9.0f ), 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, DOT_Y( 13.5f ), -0.008370535f, 1.0f, -1.0f, 4.0f,
	15.0f, -0.0065104165f, DOT_Y( 9.0f ), -0.008370535f, 1.0f, 1.0f, 3.0f,
	-15.0f, 0.0065104165f, DOT_Y( 9.0f ), -0.008370535f, 1.0f, 0.0f, 3.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
} ;
static Float flammeile_auto_tbl[48] = {
	8.0f, 8.0f, 9.0f, 9.0f, 9.0f, 9.0f,
	9.0f, 1.0f, 1.0f, 1.0f, 5.0f, 5.0f,
	5.0f, 5.0f, 5.0f, 4.0f, 4.0f, 4.0f,
	6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 2.0f,
	2.0f, 2.0f, 10.0f, 10.0f, 10.0f, 10.0f,
	10.0f, 8.0f, 50.0f, DOT_X( 20.0f ), DOT_X( 10.0f ), 0.0f,
	DOT_X( 35.0f ), DOT_X( 25.0f ), 25.0f, DOT_X( 5.0f ), -50.0f, -DOT_X( 20.0f ),
	-DOT_X( 10.0f ), 0.0f, -DOT_X( 35.0f ), -DOT_X( 25.0f ), -25.0f, -DOT_X( 5.0f ),
} ;

static void ( *pl13_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl13_main,
	pl13_com_ck,
	pl13_atck_set,
	pl13_atck,
	pl13_dash,
	pl13_noki,
	pl13_appear,
	pl13_win,
	pl13_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl13_catch_ck,	/* returns a value */
	pl13_catch_set,
	pl13_catch,
	pl13_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl13_delay_ck,
	pl13_t_kick,
	pl13_upper,
	pl13_sp_unison,
	pl13_sp_gattai,
	pl13_sp_assist,
	pl13_sp_zero,
} ;

static Uint8 schwarz_ck( PLAYER_WORK *pl_w );
static Uint8 fantome_ck( PLAYER_WORK *pl_w );
static Uint8 sublimation_ck( PLAYER_WORK *pl_w );
static Uint8 flammeile_ck( PLAYER_WORK *pl_w );
static Uint8 mille_ck( PLAYER_WORK *pl_w );
static Uint8 paretonnre_ck( PLAYER_WORK *pl_w );
static Uint8 rafale_ck( PLAYER_WORK *pl_w );
static Uint8 magie_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int paretonnre_delay_ck( PLAYER_WORK *pl_w );
static int flammeile_delay_ck( PLAYER_WORK *pl_w );
static int magie_delay_ck( PLAYER_WORK *pl_w );
static int mille_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl13_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl13_tbl[i] ;
}

/*==============================================================*/
/*			Once a jump, unless super jumping					*/
/*==============================================================*/
static Uint8 jump_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance != STANCE_AIR ) return 1 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	return 1 ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl13_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( rafale_ck( pl_w ) ) return ;
	if( paretonnre_ck( pl_w ) ) return ;
	if( magie_ck( pl_w ) ) return ;
	if( mille_ck( pl_w ) ) return ;
	if( flammeile_ck( pl_w ) ) return ;
	if( sublimation_ck( pl_w ) ) return ;
	if( schwarz_ck( pl_w ) ) return ;
	if( fantome_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_chouhatsu_ck( pl_w, 11 ) ){
		chouhatsu_ck( pl_w ) ;
		return ;
	}
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			In the air, once a jump								*/
/*==============================================================*/
static Uint8 schwarz_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_schwarz, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_schwarz ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fantome_ck( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_fantome, &pl_w->com_w0 ) ) return 0 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->fantome_shot_flag ) return 0 ;
	if( lw->mille_shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_fantome ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 sublimation_ck( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_sublimation, &pl_w->com_w1 ) ) return 0 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->sublimation_shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_sublimation ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 flammeile_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_flammeile, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_flammeile ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 mille_ck( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_mille, &pl_w->com_w3 ) ) return 0 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->mille_shot_flag ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_mille ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 paretonnre_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_paretonnre, &pl_w->com_w4 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_paretonnre ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 rafale_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rafale, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_rafale ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 magie_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_magie, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl13_sp_magie ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 11 ) ) return 0 ;
	pl_w->pl_sp_type = pl13_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl13_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl13_delay_ck( PLAYER_WORK *pl_w )
{
	if( paretonnre_delay_ck( pl_w ) || flammeile_delay_ck( pl_w ) || magie_delay_ck( pl_w ) || mille_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int paretonnre_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_paretonnre, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/*==============================================================*/
static int flammeile_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_flammeile, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 3 ;
	return 1 ;
}

/*==============================================================*/
static int magie_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_magie, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 9 ;
	return 1 ;
}

/*==============================================================*/
static int mille_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_mille, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/****************************************************************/
void pl13_main( PLAYER_WORK *pl_w )
{
}

static void nml_atck_dash( PLAYER_WORK *pl_w );
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
void pl13_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck_dash,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_atck_dash( PLAYER_WORK *pl_w )
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
		pl_se_req( pl_w, 26 ) ;
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
		pl_se_req( pl_w, 26 ) ;
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
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, 26 ) ;
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
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
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
static void stand_punch_move_3( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );
static void stand_mk_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl13_atck( PLAYER_WORK *pl_w )
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
		else stand_punch_move_3( pl_w ) ;
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
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
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
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The medium kick hits twice							*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 1:
		stand_mk_move( pl_w ) ;
		return ;
	case 0:
	case 2:
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
static void stand_punch_move_3( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
	case 0:
	case 1:
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
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void stand_mk_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl13_at_stand_mk_2 ;
		HIT_RESET( pl_w ) ;
	}
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl13_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		if( pl_w->atck_pk == 1 ){
			pl_gauge_add( pl_w, 5 ) ;
			pl_w->atck_no = pl13_at_upper ;
			HIT_RESET( pl_w ) ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_char_set( pl_w, Char_Extra, 3 ) ;
		}
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
static void dash_jump( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash, a low hop									*/
/****************************************************************/
void pl13_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_jump, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_w->timer0 = 18 ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x += pl_w->pl_dir ? -0.3125f : 0.3125f ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
	pl_w->timer0 -- ;
}

/*--------------------------------------------------------------*/
static void dash_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop back							*/
/****************************************************************/
void pl13_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_jump, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.3125f : -0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_w->timer0 = 18 ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x += pl_w->pl_dir ? 0.3125f : -0.3125f ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
	pl_w->timer0 -- ;
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl13_appear( PLAYER_WORK *pl_w )
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
	pl_w->disp_flag = 1 ;
	pls13_00_set( pl_w, 13 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = 80.0f ;
		ofs.y = DOT_Y( 16.0f ) ;
		hit_mark_set( pl_w, &ofs, 2 ) ;
		pl_se_req( pl_w, 2 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_main( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void lose_init( PLAYER_WORK *pl_w );
static void lose_move( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_scene( PLAYER_WORK *pl_w );
static void win_scene_init( PLAYER_WORK *pl_w );
static void win_scene_move( PLAYER_WORK *pl_w );
static void win_type_set( PLAYER_WORK *pl_w );

/****************************************************************/
void pl13_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init,
		win_main,
	} ;

	win_jmp[ pl_w->routine_2 ]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( !pl_w->type ) win_type_set( pl_w ) ;
	win_main( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_main( PLAYER_WORK *pl_w )
{
	static void ( *win_type_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko,
		lose_time_ko,
		win_time_ko,
		lose_time_ko,
		lose_time_ko,
		win_time_ko,
		win_time_ko,
		win_time_ko,
		win_scene,
		win_scene,
		win_scene,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
	}else{
		win_type_jmp[ pl_w->type ]( pl_w ) ;
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *lose_jmp[])( PLAYER_WORK *pl_w ) = {
		lose_init,
		lose_move,
	} ;

	lose_jmp[ pl_w->routine_3 ]( pl_w ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*--------------------------------------------------------------*/
static void lose_move( PLAYER_WORK *pl_w )
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

	win_ko_jmp[ pl_w->routine_3 ]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Win, 0 ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_scene( PLAYER_WORK *pl_w )
{
	static void ( *win_scene_jmp[])( PLAYER_WORK *pl_w ) = {
		win_scene_init,
		win_scene_move,
	} ;

	win_scene_jmp[ pl_w->routine_3 ]( pl_w ) ;
}

/*==============================================================*/
static void win_scene_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Win, 1 ) ;
}

/*--------------------------------------------------------------*/
static void win_scene_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		pls13_00_set( pl_w, 14 ) ;
		pls13_00_set( pl_w, 10 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The win pose: by the button held, or else at		*/
/*			random												*/
/*==============================================================*/
static void win_type_set( PLAYER_WORK *pl_w )
{
	static Uint16 win_type_tbl[16] = {
		5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 10, 10, 10,
	} ;

	if( pl_w->pl_new_lvbt & KEY_BUTTON ){
		if( pl_w->pl_new_lvbt & KEY_LP ) pl_w->type = 5 ;
		else if( pl_w->pl_new_lvbt & KEY_HP ) pl_w->type = 6 ;
		else if( pl_w->pl_new_lvbt & KEY_A1 ) pl_w->type = 7 ;
		else if( pl_w->pl_new_lvbt & KEY_LK ) pl_w->type = 8 ;
		else if( pl_w->pl_new_lvbt & KEY_HK ) pl_w->type = 9 ;
		else pl_w->type = 10 ;
	} else {
		pl_w->type = win_type_tbl[Rnd() & 15] ;
	}
	if( pl_be_cnt( pl_w ) > 1 ) pl_w->type = 5 ;
}

static void sp_schwarz( PLAYER_WORK *pl_w );
static void sp_fantome( PLAYER_WORK *pl_w );
static void sp_sublimation( PLAYER_WORK *pl_w );
static void sp_flammeile( PLAYER_WORK *pl_w );
static void sp_mille( PLAYER_WORK *pl_w );
static void sp_paretonnre( PLAYER_WORK *pl_w );
static void sp_rafale( PLAYER_WORK *pl_w );
static void sp_throw( PLAYER_WORK *pl_w );
static void sp_power( PLAYER_WORK *pl_w );
static void sp_magie( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_flammeile_team( PLAYER_WORK *pl_w );

/****************************************************************/
void	pl13_sp_atck( PLAYER_WORK *pl_w )
{
	static void (*pl_sp_tbl[])( PLAYER_WORK *pl_w ) ={
		sp_schwarz,					/* 00 Schwarzaile */
		sp_fantome,					/* 01 Fantome */
		sp_sublimation,					/* 02 Sublimation */
		sp_flammeile,					/* 03 Flammeile (hyper) */
		sp_mille,					/* 04 Mille Fantomes (hyper) */
		sp_paretonnre,					/* 05 Pare-tonn're (hyper) */
		sp_rafale,					/* 06 Rafale Cannon */
		sp_throw,					/* 07 a throw that carries the opponent (assist) */
		sp_power,					/* 08 a power-up for the partner (assist) */
		sp_magie,					/* 09 Tour De Magie (hyper) */
		sp_snapback,					/* 0a the snapback */
		sp_chouhatsu,					/* 0b the taunt */
		sp_flammeile_team,					/* 0c Flammeile as her part of a team hyper */
	} ;

	pl_sp_tbl[ pl_w->pl_sp_type ]( pl_w ) ;
}

static void schwarz_init( PLAYER_WORK *pl_w );
static void schwarz_start( PLAYER_WORK *pl_w );
static void schwarz_fly( PLAYER_WORK *pl_w );
static void schwarz_turn( PLAYER_WORK *pl_w );
static void schwarz_fall( PLAYER_WORK *pl_w );
static void schwarz_land( PLAYER_WORK *pl_w );
static void schwarz_next( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A flight that hits as it goes, steered by the		*/
/*			stick												*/
/*==============================================================*/
static void sp_schwarz( PLAYER_WORK *pl_w )
{
	static void ( *schwarz_jmp[])( PLAYER_WORK *pl_w ) = {
		schwarz_init, schwarz_start, schwarz_fly, schwarz_turn, schwarz_fall, schwarz_land,
	} ;

	schwarz_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void schwarz_init( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;
	Float *spd ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	spd = schwarz_spd ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	if( pl_w->atck_level && pl_w->pl_stance != STANCE_AIR ){
		pl_w->sub_no = pl_w->disp_dir ? 9 : 10 ;
		spd += pl_w->disp_dir ? 9 * 7 : 10 * 7 ;
	} else {
		pl_w->sub_no = pl_w->disp_dir ? 1 : 2 ;
		spd += pl_w->disp_dir ? 1 * 7 : 2 * 7 ;
	}
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = spd[0] ;
	pl_w->acc.x = spd[1] ;
	pl_w->spd.y = spd[2] ;
	pl_w->acc.y = spd[3] ;
	pl_w->timer0 = 30 ;
	pl_w->timer1 = 0 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->hit_cnt = 0 ;
	lw->hit_time = 0 ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl13_at_schwarz_vc ;
		HIT_RESET( pl_w ) ;
		lw->hit_cnt = 3 ;
		pl_w->timer1 = 3 ;
	} else {
		pl_w->atck_no = pl13_at_schwarz ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void schwarz_start( PLAYER_WORK *pl_w )
{
	pl_w->jump_com_cnt = 1 ;
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		ef_color_set( pl_w, 0, 1, 1 ) ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
		pl_w->routine_3 = 0 ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_air_set( pl_w ) ;
			pl_w->pos.y = pl_w->ground_y + DOT_Y( 10.0f ) ;
			if( pl_w->atck_level ){
				pl_char_set( pl_w, Char_Special, 6 ) ;
				pl_w->routine_3 = 3 ;
				pl_w->pos.x += pl_w->disp_dir ? DOT_X( 80.0f ) : -DOT_X( 80.0f ) ;
			}
		}
		pl_w->pl_stop_now = 0 ;
		pl_se_req( pl_w, 75 ) ;
		shl13_00_set( pl_w, 10, 19, 10 ) ;
		shl13_00_set( pl_w, 9, 19, 9 ) ;
		shl13_00_set( pl_w, 11, 19, 11 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void schwarz_fly( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	if( pl_w->pos.y > scr_work.stage_top - DOT_Y( 48.0f ) ) pl_w->pos.y = scr_work.stage_top - DOT_Y( 48.0f ) ;
	pl_w->push_ng_timer = 2 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->hit_time-- <= 0 ){
		lw->hit_time = 0 ;
		if( pl_w->hit_flag ){
			if( lw->hit_cnt < 2 ){
				if( lw->hit_cnt == 1 ){
					pl_w->atck_no = pl13_at_schwarz_3 ;
					HIT_RESET( pl_w ) ;
				} else {
					pl_w->atck_no = pl13_at_schwarz_2 ;
					HIT_RESET( pl_w ) ;
				}
				lw->hit_cnt ++ ;
				lw->hit_time = 4 ;
			}
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_2 = 5 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Special, 8 ) ;
			return ;
		}
		if( !--pl_w->timer0 ){
			pl_w->routine_2 = 4 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_w->acc.y = -1.0f ;
			if( pl_w->pos.y > pl_w->ground_y + DOT_X( 40.0f ) ){
				pl_char_set( pl_w, Char_Special, 2 ) ;
				return ;
			}
			pl_char_set( pl_w, Char_Special, 32 ) ;
			return ;
		}
		if( pl_w->cg_type != 1 ){
			if( ( pl_w->pl_now_lvbt & KEY_PUNCH ) || ( pl_w->pl_stop_now & KEY_PUNCH ) ) schwarz_next( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void schwarz_turn( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	pl_w->push_ng_timer = 2 ;
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 = 2 ;
		pl_w->pl_stop_now = 0 ;
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->hit_cnt = 0 ;
		lw->hit_time = 0 ;
		pl_w->atck_no = pl13_at_schwarz ;
		HIT_RESET( pl_w ) ;
		shl13_00_set( pl_w, 10, 19, 10 ) ;
		shl13_00_set( pl_w, 9, 19, 9 ) ;
		shl13_00_set( pl_w, 11, 19, 11 ) ;
		pl_se_req( pl_w, 75 ) ;
		pl_char_set( pl_w, Char_Special, pl_w->routine_3 + 3 ) ;
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void schwarz_fall( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 8 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void schwarz_land( PLAYER_WORK *pl_w )
{
	pl_w->pl_stop_now = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->rot[2] = 0 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A button turns the flight the way the stick			*/
/*			points, twice at most								*/
/*==============================================================*/
static void schwarz_next( PLAYER_WORK *pl_w )
{
	Float *spd ;
	Uint8 no ;

	if( pl_w->timer1 < 2 ){
	spd = schwarz_spd ;
	pl_w->sub_no = no = pl_w->pl_new_lvbt >> 10 ;
	spd += no * 7 ;
	if( spd[4] ){
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 30 ;
	pl_w->timer1 ++ ;
	pl_w->spd.x = spd[0] ;
	pl_w->acc.x = spd[1] ;
	pl_w->spd.y = spd[2] ;
	pl_w->acc.y = spd[3] ;
	pl_w->routine_3 = spd[6] ;
	pl_w->disp_dir = spd[5] >= 0.0f ? spd[5] : pl_w->disp_dir ;
	pls13_00_set( pl_w, 0 ) ;
	pls13_00_set( pl_w, 1 ) ;
	pl_w->rot[2] = 0 ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	}
	}
}

static void fantome_init( PLAYER_WORK *pl_w );
static void fantome_move( PLAYER_WORK *pl_w );
static void fantome_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_fantome( PLAYER_WORK *pl_w )
{
	static void ( *fantome_jmp[])( PLAYER_WORK *pl_w ) = {
		fantome_init, fantome_move, fantome_end,
	} ;

	fantome_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void fantome_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_fantome ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
}

/*--------------------------------------------------------------*/
static void fantome_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type < 1 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl13_00_set( pl_w, 0, 19, 0 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fantome_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void sublimation_init( PLAYER_WORK *pl_w );
static void sublimation_shot( PLAYER_WORK *pl_w );
static void sublimation_wait( PLAYER_WORK *pl_w );
static void sublimation_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_sublimation( PLAYER_WORK *pl_w )
{
	static void ( *sublimation_jmp[])( PLAYER_WORK *pl_w ) = {
		sublimation_init, sublimation_shot, sublimation_wait, sublimation_end,
	} ;

	sublimation_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sublimation_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_sublimation ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->sub_no = 0 ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
}

/*--------------------------------------------------------------*/
static void sublimation_shot( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		shl13_00_set( pl_w, 2, 19, 2 ) ;
		pls13_00_set( pl_w, 4 ) ;
		pl_w->cg_type = 0 ;
		if( pl_w->atck_level ) pl_w->timer0 = 16 ;
		else pl_w->timer0 = 16 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sublimation_wait( PLAYER_WORK *pl_w )
{
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 11 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sublimation_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void rafale_init( PLAYER_WORK *pl_w );
static void rafale_shot( PLAYER_WORK *pl_w );
static void rafale_back( PLAYER_WORK *pl_w );
static void rafale_catch( PLAYER_WORK *pl_w );
static void rafale_hit( PLAYER_WORK *pl_w );
static void rafale_end( PLAYER_WORK *pl_w );
static void rafale_throw_init( PLAYER_WORK *pl_w );
static void throw_wait( PLAYER_WORK *pl_w );
static void throw_shot( PLAYER_WORK *pl_w );
static void throw_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_rafale( PLAYER_WORK *pl_w )
{
	static void ( *rafale_jmp[])( PLAYER_WORK *pl_w ) = {
		rafale_init, rafale_shot, rafale_back, rafale_catch, rafale_hit, rafale_end,
		rafale_throw_init, throw_wait, throw_shot, throw_end,
	} ;

	rafale_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void rafale_init( PLAYER_WORK *pl_w )
{
	pl_gauge_add( pl_w, 5 ) ;
	if( !pl_w->atck_level ) pl_w->routine_2 ++ ;
	else {
		pl_w->routine_2 = 6 ;
		rafale_throw_init( pl_w ) ;
		return ;
	}
	pl_w->sub_no = 0 ;
	pl_w->timer0 = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 14 ) ;
}

/*--------------------------------------------------------------*/
static void rafale_shot( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 112 ){
		pl_w->cg_type = 0 ;
		shl13_00_set( pl_w, 6, 19, 6 ) ;
	}
	if( pl_w->sub_no > 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->sub_no + 15 ) ;
		return ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rafale_back( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_end_flag < 0 ){
		if( pl_w->sub_no > 0 && !pl_w->timer0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 26 ) ;
		} else {
			pl_w->routine_2 = 5 ;
			pl_char_set( pl_w, Char_Special, 29 ) ;
		}
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rafale_catch( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rafale_hit( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type == 14 ){
		pl_w->cg_type = 0 ;
		shl13_00_set( pl_w, 8, 19, 8 ) ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 28 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rafale_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rafale_throw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_throw ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->timer0 = 24 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
}

static void throw_init( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_throw( PLAYER_WORK *pl_w )
{
	static void ( *throw_jmp[])( PLAYER_WORK *pl_w ) = {
		throw_init, throw_wait, throw_shot, throw_end,
	} ;

	throw_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void throw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_throw ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->timer0 = 30 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
}

/*--------------------------------------------------------------*/
static void throw_wait( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 13 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void throw_shot( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		shl13_00_set( pl_w, 4, 19, 4 ) ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void throw_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void power_init( PLAYER_WORK *pl_w );
static void power_move( PLAYER_WORK *pl_w );
static void power_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_power( PLAYER_WORK *pl_w )
{
	static void ( *power_jmp[])( PLAYER_WORK *pl_w ) = {
		power_init, power_move, power_end,
	} ;

	power_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void power_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_sublimation ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 30 ) ;
}

/*--------------------------------------------------------------*/
static void power_move( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		shl13_00_set( pl_w, 5, 19, 5 ) ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void power_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void flammeile_init( PLAYER_WORK *pl_w );
static void flammeile_start( PLAYER_WORK *pl_w );
static void flammeile_fly( PLAYER_WORK *pl_w );
static void flammeile_turn( PLAYER_WORK *pl_w );
static void flammeile_fall( PLAYER_WORK *pl_w );
static void flammeile_land( PLAYER_WORK *pl_w );
static void flammeile_next( PLAYER_WORK *pl_w );
static void flammeile_auto( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The flight in the air: it hits as it goes,			*/
/*			steered by the stick or towards the enemy			*/
/*==============================================================*/
static void sp_flammeile( PLAYER_WORK *pl_w )
{
	static void ( *flammeile_jmp[])( PLAYER_WORK *pl_w ) = {
		flammeile_init, flammeile_start, flammeile_fly, flammeile_turn, flammeile_fall, flammeile_land,
	} ;

	flammeile_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void flammeile_init( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;
	Float *spd ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_flammeile ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	spd = flammeile_spd ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_w->sub_no = pl_w->disp_dir ? 1 : 2 ;
	spd += pl_w->disp_dir ? 1 * 7 : 2 * 7 ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	ef_color_set( pl_w, 4, 7, 1 ) ;
	pl_w->spd.x = spd[0] ;
	pl_w->acc.x = spd[1] ;
	pl_w->spd.y = spd[2] ;
	pl_w->acc.y = spd[3] ;
	pl_w->timer0 = 30 ;
	pl_w->timer1 = 0 ;
	pl_w->sub_type = 0 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->air_hit_cnt = 0 ;
	lw->air_hit_time = 0 ;
}

/*--------------------------------------------------------------*/
static void flammeile_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) >= 0 && pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->pos.y = pl_w->ground_y + DOT_Y( 10.0f ) ;
		}
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
		pl_w->pl_stop_now = 0 ;
		shl13_00_set( pl_w, 10, 19, 10 ) ;
		shl13_00_set( pl_w, 9, 19, 9 ) ;
		shl13_00_set( pl_w, 11, 19, 11 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flammeile_fly( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	if( pl_w->pos.y > scr_work.stage_top - DOT_Y( 48.0f ) ) pl_w->pos.y = scr_work.stage_top - DOT_Y( 48.0f ) ;
	if( pl_w->pos.y < pl_w->ground_y + 2.142857f ) pl_w->pos.y = pl_w->ground_y + DOT_Y( 8.0f ) ;
	pl_w->push_ng_timer = 2 ;
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		shl13_00_set( pl_w, 21, 19, 21 ) ;
	}
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->air_hit_time-- <= 0 ){
		lw->air_hit_time = 0 ;
		if( pl_w->hit_flag ){
			if( lw->air_hit_cnt < 2 ){
				if( lw->air_hit_cnt == 1 ){
					pl_w->atck_no = pl13_at_flammeile_3 ;
					HIT_RESET( pl_w ) ;
				} else {
					pl_w->atck_no = pl13_at_flammeile_2 ;
					HIT_RESET( pl_w ) ;
				}
				lw->air_hit_cnt ++ ;
				lw->air_hit_time = 4 ;
			}
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ){
			pl_w->routine_2 = 5 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_land_ef_set( pl_w ) ;
			pl_char_set( pl_w, Char_Super, 8 ) ;
			return ;
		}
		if( !--pl_w->timer0 ){
			pl_w->routine_2 = 4 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_w->acc.y = -1.0f ;
			if( pl_w->pos.y > pl_w->ground_y + DOT_X( 40.0f ) ){
				pl_char_set( pl_w, Char_Super, 2 ) ;
				return ;
			}
			pl_char_set( pl_w, Char_Super, 17 ) ;
			return ;
		} else if( pl_w->cpu_flag ){
			if( pl_w->cg_type != 1 ) flammeile_auto( pl_w ) ;
		} else if( pl_w->cg_type != 1 ){
			if( ( pl_w->pl_now_lvbt & KEY_PUNCH ) || ( pl_w->pl_stop_now & KEY_PUNCH ) ) flammeile_next( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void flammeile_turn( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_end_flag < 0 ){
		pl_w->pl_stop_now = 0 ;
		pl_w->routine_2 = 2 ;
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->air_hit_cnt = 0 ;
		lw->air_hit_time = 0 ;
		pl_w->atck_no = pl13_at_flammeile ;
		HIT_RESET( pl_w ) ;
		pl_voice_req( pl_w, 4 ) ;
		ef_color_set( pl_w, 4, 7, 1 ) ;
		shl13_00_set( pl_w, 10, 19, 10 ) ;
		shl13_00_set( pl_w, 9, 19, 9 ) ;
		shl13_00_set( pl_w, 11, 19, 11 ) ;
		pl_char_set( pl_w, Char_Super, pl_w->routine_3 + 3 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flammeile_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flammeile_land( PLAYER_WORK *pl_w )
{
	pl_w->pl_stop_now = 0 ;
	pl_w->rot[2] = 0 ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			A button turns the flight the way the stick			*/
/*			points, four times at most							*/
/*==============================================================*/
static void flammeile_next( PLAYER_WORK *pl_w )
{
	Float *spd ;
	Uint16 no ;

	if( pl_w->timer1 < 4 ){
		spd = flammeile_spd ;
		no = pl_w->pl_new_lvbt >> 10 ;
		pl_w->sub_no = no ;
		pl_w->sub_type = pl_w->sub_no ;
		spd += no * 7 ;
		if( spd[4] ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 30 ;
			pl_w->timer1 ++ ;
			pl_w->spd.x = spd[0] ;
			pl_w->acc.x = spd[1] ;
			pl_w->spd.y = spd[2] ;
			pl_w->acc.y = spd[3] ;
			pl_w->routine_3 = spd[6] ;
			pl_w->disp_dir = spd[5] >= 0.0f ? spd[5] : pl_w->disp_dir ;
			pls13_00_set( pl_w, 0 ) ;
			pls13_00_set( pl_w, 1 ) ;
			pl_w->rot[2] = 0 ;
			pl_char_set( pl_w, Char_Super, 1 ) ;
		}
	}
}

/*==============================================================*/
/*			The computer's turn: towards the enemy				*/
/*==============================================================*/
static void flammeile_auto( PLAYER_WORK *pl_w )
{
	Float *spd ;
	Float *dir ;
	Uint16 no ;

	dir = flammeile_auto_tbl ;
	if( pl_w->timer1 < 4 ){
		pl_w->sub_no = caldir_pos_256( &pl_w->pos, &pl_w->target_w->pos ) >> 3 ;
		spd = flammeile_spd ;
		pl_w->sub_no = dir[pl_w->sub_no] ;
		no = pl_w->sub_no ;
		spd += no * 7 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 30 ;
		pl_w->timer1 ++ ;
		pl_w->spd.x = spd[0] ;
		pl_w->acc.x = spd[1] ;
		pl_w->spd.y = spd[2] ;
		pl_w->acc.y = spd[3] ;
		pl_w->routine_3 = spd[6] ;
		pl_w->disp_dir = spd[5] >= 0.0f ? spd[5] : pl_w->disp_dir ;
		pls13_00_set( pl_w, 0 ) ;
		pls13_00_set( pl_w, 1 ) ;
		pl_w->rot[2] = 0 ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
	}
}

static void mille_init( PLAYER_WORK *pl_w );
static void mille_shot( PLAYER_WORK *pl_w );
static void mille_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_mille( PLAYER_WORK *pl_w )
{
	static void ( *mille_jmp[])( PLAYER_WORK *pl_w ) = {
		mille_init, mille_shot, mille_end,
	} ;

	mille_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void mille_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_fantome ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*--------------------------------------------------------------*/
static void mille_shot( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type == 1 ){
		if( pl_w->cg_type == 1 ){
			pl_w->cg_type = 0 ;
			ofs.x = DOT_X( 8.0f ) ;
			ofs.y = DOT_Y( 88.0f ) ;
			pl_hyper_start( pl_w, &ofs, 1 ) ;
		}
	}
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->atck_level = 0 ;
		shl13_00_set( pl_w, 22, 19, 0 ) ;
		pl_w->cg_type = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void mille_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
	}
}

static void paretonnre_init( PLAYER_WORK *pl_w );
static void paretonnre_shot( PLAYER_WORK *pl_w );
static void paretonnre_wait( PLAYER_WORK *pl_w );
static void paretonnre_fall( PLAYER_WORK *pl_w );
static void paretonnre_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_paretonnre( PLAYER_WORK *pl_w )
{
	static void ( *paretonnre_jmp[])( PLAYER_WORK *pl_w ) = {
		paretonnre_init, paretonnre_shot, paretonnre_wait, paretonnre_fall, paretonnre_end,
	} ;

	paretonnre_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void paretonnre_init( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_paretonnre ;
	HIT_RESET( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_w->sub_no = 0 ;
	pl_w->timer1 = 0 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->shot_on = 1 ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
	}
	scr_excep_set( pl_w, 1, 13 ) ;
	pl_char_set( pl_w, Char_Super, 14 ) ;
}

/*--------------------------------------------------------------*/
static void paretonnre_shot( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		shl13_00_set( pl_w, 16, 19, 16 ) ;
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->shot_on = 1 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void paretonnre_wait( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !lw->shot_on ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_w->routine_2 = 4 ;
		else {
			pl_w->routine_2 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( pl_w, Char_Super, 15 ) ;
			return ;
		}
	}
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void paretonnre_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 16 ) ;
	}
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
}

/*--------------------------------------------------------------*/
static void paretonnre_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	if( pl_w->cg_end_flag < 0 ){
		scr_excep_set( pl_w, 0, 13 ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

static void magie_init( PLAYER_WORK *pl_w );
static void magie_shot( PLAYER_WORK *pl_w );
static void magie_wait( PLAYER_WORK *pl_w );
static void magie_catch( PLAYER_WORK *pl_w );
static void magie_pick( PLAYER_WORK *pl_w );
static void magie_pick_move( PLAYER_WORK *pl_w );
static void magie_strike( PLAYER_WORK *pl_w );
static void magie_result( PLAYER_WORK *pl_w );
static void magie_keep( PLAYER_WORK *pl_w );
static void magie_round_end( PLAYER_WORK *pl_w );
static void magie_next( PLAYER_WORK *pl_w );
static void magie_miss( PLAYER_WORK *pl_w );
static void magie_end_wait( PLAYER_WORK *pl_w );
static void magie_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo										*/
/*==============================================================*/
static void sp_magie( PLAYER_WORK *pl_w )
{
	static void ( *magie_jmp[])( PLAYER_WORK *pl_w ) = {
		magie_init, magie_shot, magie_wait, magie_catch, magie_pick, magie_pick_move, magie_strike, magie_result,
		magie_keep, magie_round_end, magie_next, magie_miss, magie_end_wait, magie_end,
	} ;

	magie_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void magie_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_paretonnre ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_sp_ef_set( pl_w ) ;
	pl_w->sub_no = 0 ;
	pl_w->sub_type = 0 ;
	pl_w->timer0 = 0 ;
	pl_w->timer1 = 0 ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
}

/*--------------------------------------------------------------*/
static void magie_shot( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 128.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		shl13_00_set( pl_w, 14, 19, 14 ) ;
		pl_w->timer0 = 180 ;
	}
	if( pl_w->cg_end_flag < 0 ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 120 ;
	}
}

/*--------------------------------------------------------------*/
static void magie_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	scr_excep_set( pl_w, 1, 13 ) ;
	pl_w->pos.x = pl_w->disp_dir ? scr_work.scr_left + 80.0f : scr_work.scr_right + -80.0f ;
	pl_w->timer0 = 180 ;
	shl13_00_set( pl_w, 15, 19, 15 ) ;
	pl_w->routine_3 = Rnd() ;
	pl_w->routine_3 &= 1 ;
	pl_voice_req( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Super, 11 ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_pick( PLAYER_WORK *pl_w )
{
	Uint8 rnd ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( flash_work.flag ) return ;
	if( pl_w->cpu_flag && pl_w->timer0 != 30 ){
		rnd = Rnd() ;
		rnd &= 1 ;
		if( rnd ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 16 ;
			shl13_00_set( pl_w, 12, 19, 12 ) ;
		} else {
			pl_w->routine_2 ++ ;
			pl_w->routine_3 ^= 1 ;
			pl_w->timer0 = 16 ;
			shl13_00_set( pl_w, 13, 19, 13 ) ;
		}
		if( cpu_level_work.level >= 24 ) pl_w->routine_3 = 1 ;
		return ;
	}
	if( pl_w->pl_now_lvbt & KEY_LP ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
		shl13_00_set( pl_w, 12, 19, 12 ) ;
		return ;
	}
	if( pl_w->pl_now_lvbt & KEY_LK ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 ^= 1 ;
		pl_w->timer0 = 16 ;
		shl13_00_set( pl_w, 13, 19, 13 ) ;
		return ;
	}
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 = 12 ;
		pl_w->timer0 = 10 ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void magie_pick_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 10 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_strike( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 10 ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_result( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;
	VEC3 pos ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0 == 10 ){
		if( pl_w->routine_3 ){
			pl_w->sub_no = 1 ;
			pl_w->sub_type ++ ;
			pos.x += pl_w->disp_dir ? 400.0f : -400.0f ;
			pos.y = DOT_Y( 128.0f ) ;
			pos.x = pl_w->pos.x + pos.x ;
			pos.y = pl_w->pos.y + pos.y ;
			burst_set( pl_w, &pos, 1, 0xc9 ) ;
			pl_se_req( pl_w, 73 ) ;
			vital_add_dmg2( em, -( pl_w->sub_type * 4 ) ) ;
		} else {
			pl_w->routine_2 = 12 ;
			pl_char_set( pl_w, Char_Super, 13 ) ;
			return ;
		}
	}
	if( pl_w->timer0 <= 0 ){
		if( pl_w->timer1 == 3 ){
			pl_w->routine_2 = 9 ;
			return ;
		}
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 23 ;
		pl_w->sub_no = 0 ;
		pl_voice_req( pl_w, 16 ) ;
		return ;
	}
	pl_w->timer0 -- ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0-- == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		return ;
	}
	if( !pl_w->sub_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_round_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_end_flag < 0 ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_next( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_3 = Rnd() ;
	pl_w->routine_3 &= 1 ;
	pl_w->timer0 = 120 ;
	if( pl_w->timer1 <= 2 ){
		pl_w->routine_2 = 4 ;
		pl_w->timer1 ++ ;
		if( pl_w->timer1 == 3 ) pl_char_set( pl_w, Char_Super, 12 ) ;
		else pl_char_set( pl_w, Char_Super, 11 ) ;
	} else {
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void magie_miss( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 10 ;
		pl_char_set( pl_w, Char_Super, 13 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void magie_end_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->timer0-- == 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void magie_end( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_voice_req( pl_w, pl_w->sub_type == 4 ? 15 : 12 ) ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		scr_excep_set( pl_w, 0, 13 ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

static void flammeile_team_init( PLAYER_WORK *pl_w );
static void flammeile_team_start( PLAYER_WORK *pl_w );
static void flammeile_team_fly( PLAYER_WORK *pl_w );
static void flammeile_team_none( PLAYER_WORK *pl_w );
static void flammeile_team_fall( PLAYER_WORK *pl_w );
static void flammeile_team_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flammeile in a team hyper: nine hits, not steered	*/
/*==============================================================*/
static void sp_flammeile_team( PLAYER_WORK *pl_w )
{
	static void ( *flammeile_team_jmp[])( PLAYER_WORK *pl_w ) = {
		flammeile_team_init, flammeile_team_start, flammeile_team_fly, flammeile_team_none, flammeile_team_fall, flammeile_team_land,
	} ;

	flammeile_team_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void flammeile_team_init( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;
	Float *spd ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl13_at_flammeile_team ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	spd = flammeile_spd ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_w->sub_no = pl_w->disp_dir ? 1 : 2 ;
	spd += pl_w->disp_dir ? 1 * 7 : 2 * 7 ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	ef_color_set( pl_w, 4, 7, 1 ) ;
	pl_w->spd.x = spd[0] ;
	pl_w->acc.x = spd[1] ;
	pl_w->spd.y = spd[2] ;
	pl_w->acc.y = spd[3] ;
	pl_w->timer0 = 90 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->air_hit_cnt = 0 ;
	lw->air_hit_time = 0 ;
}

/*--------------------------------------------------------------*/
static void flammeile_team_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) >= 0 && pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
	if( pl_w->cg_end_flag < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ){
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->pos.y = pl_w->ground_y + DOT_Y( 10.0f ) ;
		}
		pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
		shl13_00_set( pl_w, 10, 19, 10 ) ;
		shl13_00_set( pl_w, 9, 19, 9 ) ;
		shl13_00_set( pl_w, 11, 19, 11 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flammeile_team_fly( PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw ;

	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->pos.y > scr_work.stage_top - DOT_Y( 48.0f ) ) pl_w->pos.y = scr_work.stage_top - DOT_Y( 48.0f ) ;
	if( pl_w->pos.y < pl_w->ground_y + 2.142857f ) pl_w->pos.y = pl_w->ground_y + DOT_Y( 8.0f ) ;
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		shl13_00_set( pl_w, 21, 19, 21 ) ;
	}
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->air_hit_time-- <= 0 ){
		lw->air_hit_time = 0 ;
		if( pl_w->hit_flag ){
			if( lw->air_hit_cnt < 9 ){
				pl_w->atck_no = pl13_at_flammeile_team ;
				HIT_RESET( pl_w ) ;
				lw->air_hit_cnt ++ ;
				lw->air_hit_time = 4 ;
			}
		}
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
		if( !--pl_w->timer0 ){
			pl_w->push_ng_timer = 2 ;
			pl_w->routine_2 = 4 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_w->acc.y = -1.0f ;
			pl_char_set( pl_w, Char_Super, pl_w->pos.y > pl_w->ground_y + DOT_X( 40.0f ) ? 2 : 17 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void flammeile_team_none( PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void flammeile_team_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		RATE_CLR_X( pl_w ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flammeile_team_land( PLAYER_WORK *pl_w )
{
	pl_w->rot[2] = 0 ;
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 31 ) ;
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
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl13_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, 26 ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl13_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl13_at_t_kick ;
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
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl13_catch_ck( PLAYER_WORK *pl_w )
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
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_p_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl13_catch_ck chose			*/
/****************************************************************/
void	pl13_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	pl_w->hit_stop = 10 ;
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ){
		pl_w->pl_dir = pl_w->pl_dir ^ 1 ;
		pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	}
	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	RATE_CLR_X( pl_w ) ;
	pl_w->spd.y /= 2.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl13_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->cg_type == 1 ){
		ofs.x = -160.0f ;
		ofs.y = DOT_Y( 72.0f ) ;
		hit_mark_set( pl_w, &ofs, 2 ) ;
		pl_se_req( pl_w, 2 ) ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		pl_w->pl_dir = pl_w->disp_dir ;
		em->disp_dir = pl_w->pl_dir ^ 1 ;
		em->pl_dir = em->disp_dir ;
		em->atck_no = pl13_at_throw_p ;
		em->catch_type = 1 ;
		SCR_EXCEP_SET ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jump_p_catch_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_catch_throw,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	if( pl_w->cg_type == 1 ){
		ofs.x = -DOT_X( 64.0f ) ;
		ofs.y = DOT_Y( 48.0f ) ;
		hit_mark_set( pl_w, &ofs, 2 ) ;
		pl_se_req( pl_w, 2 ) ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl13_at_air_throw ;
		em->catch_type = 1 ;
		SCR_EXCEP_SET ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void hold( PLAYER_WORK *pl_w );
static void hold_none( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl13_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold, hold_none,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_none( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl13_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl13_sp_paretonnre ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl13_sp_mille ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl13_sp_flammeile_team ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl13_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl13_sp_paretonnre ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl13_sp_mille ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl13_sp_flammeile_team ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl13_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl13_sp_sublimation ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl13_sp_throw ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl13_sp_power ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl13_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl13_sp_sublimation ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl13_sp_schwarz ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl13_sp_sublimation ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl13.c" End of File	*/
