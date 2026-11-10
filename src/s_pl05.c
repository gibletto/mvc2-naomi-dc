/* Strider Hiryu (character 05): command checks, normal attacks, appearance, win poses, special
   moves and hyper combos, the wall cling, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl05_def.h"

void pl05_main( PLAYER_WORK *pl_w );
void pl05_com_ck( PLAYER_WORK *pl_w );
void pl05_atck_set( PLAYER_WORK *pl_w );
void pl05_atck( PLAYER_WORK *pl_w );
void pl05_dash( PLAYER_WORK *pl_w );
void pl05_noki( PLAYER_WORK *pl_w );
void pl05_appear( PLAYER_WORK *pl_w );
void pl05_win( PLAYER_WORK *pl_w );
void pl05_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl05_catch_ck( PLAYER_WORK *pl_w );
void pl05_catch_set( PLAYER_WORK *pl_w );
void pl05_catch( PLAYER_WORK *pl_w );
void pl05_hold( PLAYER_WORK *pl_w );
int pl05_delay_ck( PLAYER_WORK *pl_w );
void pl05_t_kick( PLAYER_WORK *pl_w );
void pl05_upper( PLAYER_WORK *pl_w );
void pl05_sp_unison( PLAYER_WORK *pl_w );
void pl05_sp_gattai( PLAYER_WORK *pl_w );
void pl05_sp_assist( PLAYER_WORK *pl_w );
void pl05_sp_zero( PLAYER_WORK *pl_w );
void pl05_tlocal_init( PLAYER_WORK *pl_w );

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
static Uint16 com_murakumo[] = { 3, COM_GROUND, pl05_sp_murakumo, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Ame no Murakumo */
static Uint16 com_jexcalibur[] = { 3, COM_AIR, pl05_sp_jexcalibur, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_ATCK } ;	/* Excalibur (in the air) */
static Uint16 com_gram[] = { 3, COM_GROUND, pl05_sp_gram, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Gram */
static Uint16 com_lgram[] = { 3, COM_GROUND, pl05_sp_lgram, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Low Gram */
static Uint16 com_jgram[] = { 3, COM_AIR, pl05_sp_jgram, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Gram (in the air) */
static Uint16 com_vajra[] = { 3, COM_GROUND, pl05_sp_vajra, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Vajra */
static Uint16 com_warp[] = { 3, COM_GROUND, pl05_sp_warp, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_ATCK } ;	/* Warp */
static Uint16 com_formation_a[] = { 3, COM_GROUND, pl05_sp_formation_a, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Formation A */
static Uint16 com_formation_b[] = { 32, 0, pl05_sp_formation_b, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH } ;	/* Formation B */
static Uint16 com_formation_c[] = { 32, COM_GROUND, pl05_sp_formation_c, 0, KEY_BACK, KEY_FORWARD, KEY_KICK } ;	/* Formation C */
static Uint16 com_kabe[] = { 3, COM_GROUND, pl05_sp_kabe, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Kabe Hari Tsuki */
static Uint16 com_ragnarok[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl05_sp_ragnarok, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Ragnarok */
static Uint16 com_legion[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl05_sp_legion, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Legion */
static Uint16 com_ouroboros[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl05_sp_ouroboros, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Ouroboros */

static void ( *pl05_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl05_main,
	pl05_com_ck,
	pl05_atck_set,
	pl05_atck,
	pl05_dash,
	pl05_noki,
	pl05_appear,
	pl05_win,
	pl05_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl05_catch_ck,	/* returns a value */
	pl05_catch_set,
	pl05_catch,
	pl05_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl05_delay_ck,
	pl05_t_kick,
	pl05_upper,
	pl05_sp_unison,
	pl05_sp_gattai,
	pl05_sp_assist,
	pl05_sp_zero,
	0,
	0,
	0,
	pl05_tlocal_init,
} ;

static Uint8 ragnarok_ck( PLAYER_WORK *pl_w );
static Uint8 legion_ck( PLAYER_WORK *pl_w );
static Uint8 ouroboros_ck( PLAYER_WORK *pl_w );
static Uint8 gram_ck( PLAYER_WORK *pl_w );
static Uint8 lgram_ck( PLAYER_WORK *pl_w );
static Uint8 jgram_ck( PLAYER_WORK *pl_w );
static Uint8 warp_ck( PLAYER_WORK *pl_w );
static Uint8 kabe_ck( PLAYER_WORK *pl_w );
static Uint8 murakumo_ck( PLAYER_WORK *pl_w );
static Uint8 formation_a_ck( PLAYER_WORK *pl_w );
static Uint8 jexcalibur_ck( PLAYER_WORK *pl_w );
static Uint8 vajra_ck( PLAYER_WORK *pl_w );
static Uint8 formation_c_ck( PLAYER_WORK *pl_w );
static Uint8 formation_b_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int ragnarok_delay_ck( PLAYER_WORK *pl_w );
static int legion_delay_ck( PLAYER_WORK *pl_w );
static int ouroboros_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl05_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl05_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl05_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( ragnarok_ck( pl_w ) ) return ;
	if( legion_ck( pl_w ) ) return ;
	if( ouroboros_ck( pl_w ) ) return ;
	if( gram_ck( pl_w ) ) return ;
	if( lgram_ck( pl_w ) ) return ;
	if( jgram_ck( pl_w ) ) return ;
	if( warp_ck( pl_w ) ) return ;
	if( kabe_ck( pl_w ) ) return ;
	if( murakumo_ck( pl_w ) ) return ;
	if( formation_a_ck( pl_w ) ) return ;
	if( jexcalibur_ck( pl_w ) ) return ;
	if( vajra_ck( pl_w ) ) return ;
	if( formation_c_ck( pl_w ) ) return ;
	if( formation_b_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 ragnarok_ck( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_ragnarok, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->option_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_ragnarok ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 legion_ck( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_legion, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->option_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_legion ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ouroboros_ck( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_ouroboros, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->option_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_ouroboros ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 gram_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_gram, &pl_w->com_w3 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_gram ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 lgram_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_lgram, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_lgram ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
/*==============================================================*/
static Uint8 jgram_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jgram, &pl_w->com_w5 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_jgram ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 warp_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_warp, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_warp ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 kabe_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_kabe, &pl_w->com_w7 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_kabe ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 murakumo_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_murakumo, &pl_w->com_w8 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_murakumo ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 formation_a_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_formation_a, &pl_w->com_w9 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w9 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_formation_a ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
/*==============================================================*/
static Uint8 jexcalibur_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_jexcalibur, &pl_w->com_w10 ) ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w10 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_jexcalibur ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 vajra_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_vajra, &pl_w->com_w11 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w11 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_vajra ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 formation_c_ck( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( !com_tame_ck( pl_w, com_formation_c, &pl_w->com_w12 ) ) return 0 ;
	if( lw->bird_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w12 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl05_sp_formation_c ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 formation_b_ck( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( !com_tame_ck( pl_w, com_formation_b, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !lw->panther_flag ) return 0 ;
	if( !lw->panther_flag ) pl_w->pl_sp_type = pl05_sp_formation_b ;
	else {
		if( lw->panther_go_flag ) return 0 ;
		pl_w->pl_sp_type = pl05_sp_formation_b_shot ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl05_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 17 ) ) return 0 ;
	pl_w->pl_sp_type = pl05_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl05_delay_ck( PLAYER_WORK *pl_w )
{
	if( ragnarok_delay_ck( pl_w ) || legion_delay_ck( pl_w ) || ouroboros_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int ragnarok_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_ragnarok, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl05_sp_ragnarok ;
	return 1 ;
}

/*==============================================================*/
static int legion_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_legion, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl05_sp_legion ;
	return 1 ;
}

/*==============================================================*/
static int ouroboros_delay_ck( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_ouroboros, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->option_flag ) return 0 ;
	pl_w->delay_sp_type = pl05_sp_ouroboros ;
	return 1 ;
}

/****************************************************************/
void pl05_main( PLAYER_WORK *pl_w )
{
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
void pl05_atck_set( PLAYER_WORK *pl_w )
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
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_voice_req( pl_w, 30 ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_voice_req( pl_w, 30 ) ;
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
		pl_voice_req( pl_w, 30 ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_voice_req( pl_w, 30 ) ;
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
		pl_voice_req( pl_w, 30 ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_voice_req( pl_w, 30 ) ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			The heavy one slides, or with the stick forward		*/
/*			is an overhead										*/
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
		pl_voice_req( pl_w, 30 ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->dir = 0 ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 1 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->dir = 1 ;
			pl_w->char_no.b.no = 3 ;
			pl_w->atck_no = pl05_at_dfwd_hk ;
		} else {
			pl_w->char_no.b.no = 2 ;
			pl_w->atck_no = ATCK_CROUCH_HK ;
			pl_voice_req( pl_w, 30 ) ;
		}
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
		pl_voice_req( pl_w, 30 ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_voice_req( pl_w, 30 ) ;
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
		pl_voice_req( pl_w, 30 ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		pl_voice_req( pl_w, 30 ) ;
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
static void slide_move( PLAYER_WORK *pl_w );
static void slide_start( PLAYER_WORK *pl_w );
static void slide_slide( PLAYER_WORK *pl_w );
static void slide_end( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl05_atck( PLAYER_WORK *pl_w )
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
/*			The heavy one steps forward as the animation		*/
/*			says												*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	Float step ;

	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		step = pl_w->cg_type ;
		if( !step ) break ;
		pl_w->cg_type = 0 ;
		step *= pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
		pl_w->pos.x += step ;
		break ;
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
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	Float step ;

	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		step = pl_w->cg_type ;
		if( !step ) break ;
		pl_w->cg_type = 0 ;
		step *= pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
		pl_w->pos.x += step ;
		break ;
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
	case 2:
		if( pl_w->dir ){
			slide_move( pl_w ) ;
			break ;
		}
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick with the stick forward: a slide		*/
/*==============================================================*/
static void slide_move( PLAYER_WORK *pl_w )
{
	static void ( *slide_jmp[])( PLAYER_WORK *pl_w ) = {
		slide_start, slide_slide, slide_end,
	} ;

	slide_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void slide_start( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 32 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
}

/*--------------------------------------------------------------*/
static void slide_slide( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) pl_smoke_set2( pl_w, 3 ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Crouch_K, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void slide_end( PLAYER_WORK *pl_w )
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
/*			The launcher										*/
/****************************************************************/
void pl05_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 17 ) ;
		pl_w->atck_no = pl05_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_voice_req( pl_w, 30 ) ;
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
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl05_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 9.5f ) : -DOT_X( 9.5f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
	pl_w->timer0 = 14 ;
	pl_voice_req( pl_w, 6 ) ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
		pl_w->char_no.b.grp = 2 ;
		pl_w->char_no.b.no = 2 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_smoke_set2( pl_w, 3 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl05_noki( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 10.0f ) : DOT_X( 10.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? 0.3125f : -0.3125f ;
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
		pl_w->char_no.b.grp = 2 ;
		pl_w->char_no.b.no = 3 ;
		pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear0( PLAYER_WORK *pl_w );
static void appear0_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear0_fly( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear0_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear0_fall( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear0_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear1( PLAYER_WORK *pl_w );
static void appear1_init( PLAYER_WORK *pl_w );
static void appear1_wait( PLAYER_WORK *pl_w );
static void appear1_end( PLAYER_WORK *pl_w );
static void appear2( PLAYER_WORK *pl_w );
static void appear2_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear2_ride( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear2_out( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear2_in( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void appear2_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/****************************************************************/
/*			The entrance, one of three picked at random			*/
/****************************************************************/
void pl05_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	static Sint16 appear_type[4] = { 0, 0, 1, 2 } ;

	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->timer1 = appear_type[nlRand() & 3] ;
	appear_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	static void ( *appear_move_jmp[])( PLAYER_WORK *pl_w ) = {
		appear0, appear1, appear2,
	} ;

	appear_move_jmp[pl_w->timer1]( pl_w ) ;
}

/*==============================================================*/
/*			Drops in from above and behind						*/
/*==============================================================*/
static void appear0( PLAYER_WORK *pl_w )
{
	static void ( *appear0_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		appear0_init, appear0_fly, appear0_jump, appear0_fall, appear0_land,
	} ;

	appear0_jmp[pl_w->routine_3]( pl_w, ( PL05_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void appear0_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	lw->end_x = pl_w->pos.x ;
	lw->end_y = pl_w->pos.y ;
	pl_w->pos.x += pl_w->pl_dir ? -320.0f : 320.0f ;
	pl_w->pos.y += DOT_Y( 80.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	pl_w->spd.y = -DOT_Y( 0.5f ) ;
	pl_w->timer0 = 32 ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear0_fly( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = ( lw->end_x - pl_w->pos.x ) / 32.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		pl_w->timer0 = 32 ;
		shl05_03_set( pl_w, 1 ) ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear0_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_w->pos.x = lw->end_x ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void appear0_fall( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear0_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/*==============================================================*/
static void appear1( PLAYER_WORK *pl_w )
{
	static void ( *appear1_jmp[])( PLAYER_WORK *pl_w ) = {
		appear1_init, appear1_wait, appear1_end,
	} ;

	appear1_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear1_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	shl05_03_set( pl_w, 3 ) ;
	pl_w->timer0 = 24 ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear1_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Appear, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear1_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

/*==============================================================*/
/*			Comes in from the front, facing away				*/
/*==============================================================*/
static void appear2( PLAYER_WORK *pl_w )
{
	static void ( *appear2_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		appear2_init, appear2_ride, appear2_out, appear2_in, appear2_land,
	} ;

	appear2_jmp[pl_w->routine_3]( pl_w, ( PL05_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void appear2_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	lw->end_x = pl_w->pos.x ;
	lw->end_y = pl_w->pos.y ;
	pl_w->pos.x += pl_w->pl_dir ? 640.0f : -640.0f ;
	pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	lw->away_flag = 1 ;
	shl05_02_set( pl_w, 5 ) ;
	pl_w->timer0 = 48 ;
	pl_char_set( pl_w, Char_Crouch_K, 1 ) ;
}

/*--------------------------------------------------------------*/
static void appear2_ride( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( !--pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear2_out( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		lw->away_flag = 0 ;
		pl_w->pos.x = lw->end_x ;
		pl_w->pos.y = lw->end_y ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear2_in( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear2_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void lose_init( PLAYER_WORK *pl_w );
static void win_time_init( PLAYER_WORK *pl_w );
static void lose_time_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move0( PLAYER_WORK *pl_w );
static void win_ko_move3( PLAYER_WORK *pl_w );
static void win_ko3_start( PLAYER_WORK *pl_w );
static void win_ko3_up( PLAYER_WORK *pl_w );
static void win_ko3_turn( PLAYER_WORK *pl_w );
static void win_ko3_end( PLAYER_WORK *pl_w );
static void win_time_move( PLAYER_WORK *pl_w );
static void lose_time_move( PLAYER_WORK *pl_w );
static void lose_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The win and lose poses								*/
/****************************************************************/
void pl05_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, lose_init, win_time_init, lose_time_init, lose_init,
	} ;

	pl_w->routine_2 ++ ;
	win_init_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO: the buttons held pick the pose, or		*/
/*			else it is random									*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	static Uint8 win_pose0[4] = { 0, 1, 2, 3 } ;
	static Uint8 win_pose1[4] = { 3, 1, 2, 3 } ;

	if( !pl_w->cpu_flag && ( pl_w->pl_new_lvbt & KEY_ATCK ) ){
		if( pl_w->pl_new_lvbt & KEY_LP ) pl_w->timer0 = 0 ;
		else if( pl_w->pl_new_lvbt & KEY_HP ) pl_w->timer0 = 1 ;
		else if( pl_w->pl_new_lvbt & KEY_LK ) pl_w->timer0 = 2 ;
		else pl_w->timer0 = 3 ;
	} else pl_w->timer0 = Rnd() & 3 ;
	if( vs_sys_work.vs_end_flag ) pl_w->timer0 = win_pose0[pl_w->timer0] ;
	else pl_w->timer0 = win_pose1[pl_w->timer0] ;
	if( pl_be_cnt( pl_w ) >= 2 ) pl_w->timer0 = 1 ;
	switch( pl_w->timer0 ){
	case 0:
		shl05_03_set( pl_w, 6 ) ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
		break ;
	case 1:
		pl_char_set( pl_w, Char_Win, 1 ) ;
		break ;
	case 2:
		pl_char_set( pl_w, Char_Win, 2 ) ;
		break ;
	case 3:
		pl_char_set( pl_w, Char_Win, 4 ) ;
		break ;
	default:
		pl_char_set( pl_w, Char_Win, 1 ) ;
		break ;
	}
}

/*==============================================================*/
static void win_time_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*==============================================================*/
static void lose_time_init( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	pls05_01_set( pl_w ) ;
	pl_char_set( pl_w, Char_Win, 3 ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	pls05_01_set( pl_w ) ;
	pl_char_set( pl_w, Char_Win, 3 ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_move, lose_move, win_time_move, lose_time_move, lose_move,
	} ;

	win_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_move0, win_ko_move0, win_ko_move0, win_ko_move3,
	} ;

	win_ko_move_jmp[pl_w->timer0]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_move0( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The fourth pose: a leap off the top of the			*/
/*			screen, and back down								*/
/*==============================================================*/
static void win_ko_move3( PLAYER_WORK *pl_w )
{
	static void ( *win_ko3_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko3_start, win_ko3_up, win_ko3_turn, win_ko3_end,
	} ;

	win_ko3_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_ko3_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->routine_3 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void win_ko3_up( PLAYER_WORK *pl_w )
{
	Sint32 *rot ;

	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		rot = &pl_w->rot[2] ;
		*rot = ( *rot + 0x8000 ) & 0xffff ;	/* a half turn */
		pl_w->pos.y += DOT_Y( 136.0f ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko3_turn( PLAYER_WORK *pl_w )
{
	Sint32 *rot ;

	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y + DOT_Y( 572.0f ) ) return ;
	pl_w->routine_3 ++ ;
	rot = &pl_w->rot[2] ;
	*rot = ( *rot + 0x8000 ) & 0xffff ;
	pl_w->pos.y = pl_w->ground_y + DOT_Y( 482.0f ) ;
	shl05_03_set( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Win, 5 ) ;
}

/*--------------------------------------------------------------*/
static void win_ko3_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_time_move( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void lose_move( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	pl_char_move( pl_w ) ;
}

static void sp_murakumo( PLAYER_WORK *pl_w );
static void sp_warp( PLAYER_WORK *pl_w );
static void sp_ouroboros( PLAYER_WORK *pl_w );
static void sp_gram( PLAYER_WORK *pl_w );
static void sp_lgram( PLAYER_WORK *pl_w );
static void sp_jgram( PLAYER_WORK *pl_w );
static void sp_kabe( PLAYER_WORK *pl_w );
static void sp_formation_a( PLAYER_WORK *pl_w );
static void sp_legion( PLAYER_WORK *pl_w );
static void sp_jexcalibur( PLAYER_WORK *pl_w );
static void sp_ragnarok( PLAYER_WORK *pl_w );
static void sp_vajra( PLAYER_WORK *pl_w );
static void sp_formation_c( PLAYER_WORK *pl_w );
static void sp_formation_b( PLAYER_WORK *pl_w );
static void sp_formation_b_shot( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void shadow_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl05_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_murakumo, sp_murakumo, sp_murakumo, sp_warp, sp_ouroboros, sp_gram, sp_lgram, sp_jgram,
		sp_kabe, sp_formation_a, sp_legion, sp_jexcalibur, sp_ragnarok, sp_vajra, sp_formation_c, sp_formation_b,
		sp_formation_b_shot, sp_chouhatsu, sp_murakumo, sp_murakumo, sp_murakumo, sp_murakumo, sp_snapback,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void murakumo_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void murakumo_dash( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void murakumo_slash( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			A dash along the ground, leaving afterimages		*/
/*==============================================================*/
static void sp_murakumo( PLAYER_WORK *pl_w )
{
	static void ( *murakumo_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		murakumo_init, murakumo_dash, murakumo_slash,
	} ;

	murakumo_jmp[pl_w->routine_2]( pl_w, ( PL05_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void murakumo_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl05_at_vc_murakumo ;
		HIT_RESET( pl_w ) ;
	} else if( pl_w->pl_sp_type ){
		pl_w->atck_no = pl05_at_murakumo_2 ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level ? 0x32 : 0x30 ;
		HIT_RESET( pl_w ) ;
	}
	lw->zanzo_timer = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
	pl_w->timer0 = pl_w->atck_level ? 12 : 1 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void murakumo_dash( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	shadow_set( pl_w ) ;
	if( --pl_w->timer0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
}

/*--------------------------------------------------------------*/
static void murakumo_slash( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		shadow_set( pl_w ) ;
	}
}

static void warp_init( PLAYER_WORK *pl_w );
static void warp_out( PLAYER_WORK *pl_w );
static void warp_in( PLAYER_WORK *pl_w );
static void warp_fall( PLAYER_WORK *pl_w );
static void warp_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A warp to the far side, or overhead with the		*/
/*			kick												*/
/*==============================================================*/
static void sp_warp( PLAYER_WORK *pl_w )
{
	static void ( *warp_jmp[])( PLAYER_WORK *pl_w ) = {
		warp_init, warp_out, warp_in, warp_fall, warp_land,
	} ;

	warp_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void warp_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 20 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
}

/*--------------------------------------------------------------*/
static void warp_out( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_air_set( pl_w ) ;
		pl_w->pos.y = pl_w->pos.y + ( pl_w->atck_pk ? DOT_Y( 32.0f ) : DOT_Y( 128.0f ) ) ;
		pl_w->pos.x = scr_work.eye[0].x - 320.0f ;
		pl_w->pos.x = pl_w->pos.x + ( pl_w->atck_level ? DOT_X( 448.0f ) : 0.0f ) ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void warp_in( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( !pl_w->atck_pk ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void warp_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_land_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Jump, 3 ) ;
}

/*--------------------------------------------------------------*/
static void warp_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void ouroboros_init( PLAYER_WORK *pl_w );
static void ouroboros_wait( PLAYER_WORK *pl_w );
static void ouroboros_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_ouroboros( PLAYER_WORK *pl_w )
{
	static void ( *ouroboros_jmp[])( PLAYER_WORK *pl_w ) = {
		ouroboros_init, ouroboros_wait, ouroboros_move,
	} ;

	ouroboros_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ouroboros_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl05_at_ouroboros ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void ouroboros_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 35.0f ) ;
		ofs.y = DOT_Y( 69.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ouroboros_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->cg_type = 0 ;
		shl05_00_set( pl_w ) ;
	}
}

static void gram_init( PLAYER_WORK *pl_w );
static void gram_start( PLAYER_WORK *pl_w );
static void gram_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_gram( PLAYER_WORK *pl_w )
{
	static void ( *gram_jmp[])( PLAYER_WORK *pl_w ) = {
		gram_init, gram_start, gram_move,
	} ;

	gram_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void gram_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 0x38 : 0x36 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 15 ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
}

/*--------------------------------------------------------------*/
static void gram_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void gram_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void lgram_init( PLAYER_WORK *pl_w );
static void lgram_start( PLAYER_WORK *pl_w );
static void lgram_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_lgram( PLAYER_WORK *pl_w )
{
	static void ( *lgram_jmp[])( PLAYER_WORK *pl_w ) = {
		lgram_init, lgram_start, lgram_move,
	} ;

	lgram_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lgram_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 0x3b : 0x39 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 13 ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
}

/*--------------------------------------------------------------*/
static void lgram_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void lgram_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jgram_init( PLAYER_WORK *pl_w );
static void jgram_start( PLAYER_WORK *pl_w );
static void jgram_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The same in the air, slowing the jump				*/
/*==============================================================*/
static void sp_jgram( PLAYER_WORK *pl_w )
{
	static void ( *jgram_jmp[])( PLAYER_WORK *pl_w ) = {
		jgram_init, jgram_start, jgram_move,
	} ;

	jgram_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jgram_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 0x3b : 0x39 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x /= 4.0f ;
	pl_w->acc.x /= 4.0f ;
	pl_w->spd.y /= 4.0f ;
	pl_w->acc.y /= 4.0f ;
	pl_w->timer0 = 14 ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
}

/*--------------------------------------------------------------*/
static void jgram_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jgram_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void kabe_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void kabe_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void kabe_stick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void sp_stick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void sp_stick_chg( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw, Uint8 no );
static void sp_stick_end( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static Uint8 pl05_kdash_ck( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			The leap to the wall, and the moves made			*/
/*			clinging to it										*/
/*==============================================================*/
static void sp_kabe( PLAYER_WORK *pl_w )
{
	static void ( *kabe_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		kabe_init, kabe_jump, kabe_stick, sp_stick,
	} ;

	kabe_jmp[pl_w->routine_2]( pl_w, ( PL05_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void kabe_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	Float x ;
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_sp_ef_set( pl_w ) ;
	scr_excep_set( pl_w, 1, 13 ) ;
	pl_w->timer0 = 32 ;
	lw->pl05_wall_flag = 0 ;
	lw->pl05_wall_flag1 = 0 ;
	lw->wall_cnt = 2 ;
	x = pl_w->pl_dir ? scr_work.eye[0].x + -333.33334f : scr_work.eye[0].x + 333.33334f ;
	pl_w->spd.x = ( x - pl_w->pos.x ) / 32.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = pl_w->atck_level ? DOT_Y( 10.0f ) : DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_char_set( pl_w, Char_Jump, 8 ) ;
}

/*--------------------------------------------------------------*/
static void kabe_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_air_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( --pl_w->timer0 >= 0 ) return ;
	if( !( pl_w->wall_flag & ( pl_w->pl_dir + 1 ) ) ) return ;
	pl_w->routine_2 ++ ;
	dm00_00_set( pl_w ) ;
	SCR_EXCEP_SET ;
	if( pl_w->target_w->pl_type == PL_ABYSS3 ) scr_excep_set( pl_w, 1, 13 ) ;
	lw->pl05_wall_flag1 = 1 ;
	lw->pl05_com_st = 0 ;
	lw->com_timer = 0 ;
	pl_char_set( pl_w, Char_Extra, 6 ) ;
}

/*--------------------------------------------------------------*/
static void kabe_stick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl05_kdash_ck( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 300 ;
		pl_w->timer1 = 0 ;
		pl_w->routine_3 = 0 ;
	}
}

static void stick_wait( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_up( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_down( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_punch( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_kick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_dash( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_slide( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_off( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			On the wall: lets go after a while, or when the		*/
/*			round ends											*/
/*==============================================================*/
static void sp_stick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_wait, stick_up, stick_down, stick_punch, stick_kick, stick_dash, stick_slide, stick_off,
	} ;

	if( vs_sys_work.vs_st >= VS_END_WAIT ){
		sp_stick_end( pl_w, lw ) ;
		return ;
	}
	if( !lw->pl05_wall_flag && !--pl_w->timer0 ) lw->pl05_wall_flag = 1 ;
	stick_jmp[pl_w->timer1]( pl_w, lw ) ;
}

static void stick_wait_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_wait_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			Hanging still										*/
/*==============================================================*/
static void stick_wait( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_wait_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_wait_init, stick_wait_move,
	} ;

	if( lw->pl05_wall_flag ){
		sp_stick_end( pl_w, lw ) ;
		return ;
	}
	stick_wait_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_wait_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_char_set( pl_w, Char_Extra, 3 ) ;
	stick_wait_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_wait_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl05_kdash_ck( pl_w, lw ) ){
		sp_stick_chg( pl_w, lw, 5 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_PUNCH ){
		pl_w->atck_level = ( pl_w->pl_sw_new & KEY_HP ) ? 1 : 0 ;
		sp_stick_chg( pl_w, lw, 3 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_KICK ){
		if( pl_w->pl_sw_new & KEY_DOWN ) sp_stick_chg( pl_w, lw, 6 ) ;
		else {
			pl_w->atck_level = ( pl_w->pl_sw_new & KEY_HK ) ? 1 : 0 ;
			sp_stick_chg( pl_w, lw, 4 ) ;
		}
		return ;
	}
	if( pl_w->pl_sw_new & KEY_UP ){
		sp_stick_chg( pl_w, lw, 1 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_DOWN ){
		sp_stick_chg( pl_w, lw, 2 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_FORWARD ){
		sp_stick_chg( pl_w, lw, 7 ) ;
		return ;
	}
}

static void stick_up_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_up_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			Climbing up											*/
/*==============================================================*/
static void stick_up( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_up_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_up_init, stick_up_move,
	} ;

	if( lw->pl05_wall_flag ){
		sp_stick_end( pl_w, lw ) ;
		return ;
	}
	stick_up_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_up_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_char_set( pl_w, Char_Extra, 4 ) ;
	stick_up_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_up_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl05_kdash_ck( pl_w, lw ) ){
		sp_stick_chg( pl_w, lw, 5 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_PUNCH ){
		pl_w->atck_level = ( pl_w->pl_sw_new & KEY_HP ) ? 1 : 0 ;
		sp_stick_chg( pl_w, lw, 3 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_KICK ){
		if( pl_w->pl_sw_new & KEY_DOWN ) sp_stick_chg( pl_w, lw, 6 ) ;
		else {
			pl_w->atck_level = ( pl_w->pl_sw_new & KEY_HK ) ? 1 : 0 ;
			sp_stick_chg( pl_w, lw, 4 ) ;
		}
		return ;
	}
	if( pl_w->pl_sw_new & KEY_FORWARD ){
		sp_stick_chg( pl_w, lw, 2 ) ;
		return ;
	}
	if( !( pl_w->pl_sw_new & KEY_UP ) ){
		sp_stick_chg( pl_w, lw, 0 ) ;
		return ;
	}
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y >= pl_w->ground_y + DOT_Y( 368.0f ) ){
		pl_w->pos.y = pl_w->ground_y + DOT_Y( 368.0f ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	pl_char_move( pl_w ) ;
}

static void stick_down_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_down_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			Climbing down, kicking up dust now and then			*/
/*==============================================================*/
static void stick_down( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_down_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_down_init, stick_down_move,
	} ;

	if( lw->pl05_wall_flag ){
		sp_stick_end( pl_w, lw ) ;
		return ;
	}
	stick_down_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_down_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = -DOT_Y( 3.0f ) ;
	pl_char_set( pl_w, Char_Extra, 5 ) ;
	stick_down_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_down_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl05_kdash_ck( pl_w, lw ) ){
		sp_stick_chg( pl_w, lw, 5 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_PUNCH ){
		pl_w->atck_level = ( pl_w->pl_sw_new & KEY_HP ) ? 1 : 0 ;
		sp_stick_chg( pl_w, lw, 3 ) ;
		return ;
	}
	if( pl_w->pl_sw_new & KEY_KICK ){
		if( pl_w->pl_sw_new & KEY_DOWN ) sp_stick_chg( pl_w, lw, 6 ) ;
		else {
			pl_w->atck_level = ( pl_w->pl_sw_new & KEY_HK ) ? 1 : 0 ;
			sp_stick_chg( pl_w, lw, 4 ) ;
		}
		return ;
	}
	if( pl_w->pl_sw_new & KEY_FORWARD ){
		sp_stick_chg( pl_w, lw, 7 ) ;
		return ;
	}
	if( !( pl_w->pl_sw_new & KEY_DOWN ) ){
		sp_stick_chg( pl_w, lw, 0 ) ;
		return ;
	}
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( !( Rnd() & 15 ) ){
		ofs.x = -35.0f ;
		ofs.y = DOT_Y( 120.0f ) ;
		ofs.z = 0.0f ;
		dust_ofs_set( pl_w, &ofs, 1.0f, 1.0f ) ;
		dust_ofs_set( pl_w, &ofs, 1.0f, 1.0f ) ;
		dust_ofs_set( pl_w, &ofs, 1.0f, 1.0f ) ;
	}
	pl_char_move( pl_w ) ;
}

static void stick_punch_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_punch_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			A punch from the wall								*/
/*==============================================================*/
static void stick_punch( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_punch_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_punch_init, stick_punch_move,
	} ;

	stick_punch_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_punch_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl05_at_kabe_p ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 9 ) ;
	pl_voice_req( pl_w, 30 ) ;
	stick_punch_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_punch_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl05_kdash_ck( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ) sp_stick_chg( pl_w, lw, 0 ) ;
}

/* the speed and slowing of a jump off the wall */
typedef struct KICK_SPD {
	Float	spd_x;
	Float	acc_x;
} KICK_SPD;

static void stick_kick_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_kick_start( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_kick_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_kick_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			A kick that leaps off the wall						*/
/*==============================================================*/
static void stick_kick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_kick_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_kick_init, stick_kick_start, stick_kick_jump, stick_kick_land,
	} ;

	stick_kick_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_kick_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static KICK_SPD kick_spd[2] = {
		{ 10.0f, -0.0f }, { DOT_X( 13.0f ), -0.0f },
	} ;

	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 0x42 : 0x40 ;
	HIT_RESET( pl_w ) ;
	lw->pl05_wall_flag = 1 ;
	pl_w->spd.x = pl_w->pl_dir ? kick_spd[pl_w->atck_level].spd_x : -kick_spd[pl_w->atck_level].spd_x ;
	pl_w->acc.x = pl_w->pl_dir ? -kick_spd[pl_w->atck_level].acc_x : kick_spd[pl_w->atck_level].acc_x ;
	pl_w->spd.y = -DOT_Y( 9.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	pl_char_set( pl_w, Char_Extra, ( pl_w->atck_level << 1 ) + 10 ) ;
	stick_kick_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_kick_start( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		lw->pl05_wall_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void stick_kick_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Extra, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stick_kick_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void stick_dash_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_dash_start( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_dash_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_dash_stick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			The wall dash: a leap to the other wall				*/
/*==============================================================*/
static void stick_dash( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_dash_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_dash_init, stick_dash_start, stick_dash_jump, stick_dash_stick,
	} ;

	stick_dash_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_dash_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	lw->wall_cnt -- ;
	pl_char_set( pl_w, Char_Extra, 7 ) ;
	stick_dash_start( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_dash_start( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	Float x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		scr_excep_set( pl_w, 1, 1 ) ;
		lw->pl05_wall_flag1 = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		x = pl_w->pl_dir ? scr_work.eye[0].x + 333.33334f : scr_work.eye[0].x + -333.33334f ;
		pl_w->spd.x = ( x - pl_w->pos.x ) / 32.0f ;
		lw->leap_timer = 32 ;
		pl_voice_req( pl_w, 6 ) ;
		pl_char_set( pl_w, Char_Extra, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stick_dash_jump( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_w->push_ng_timer = 1 ;
	pl_char_move( pl_w ) ;
	if( --lw->leap_timer >= 0 ) return ;
	if( !( pl_w->wall_flag & ( ( pl_w->pl_dir ^ 1 ) + 1 ) ) ) return ;
	pl_w->routine_3 ++ ;
	pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	dm00_00_set( pl_w ) ;
	SCR_EXCEP_SET ;
	if( pl_w->target_w->pl_type == PL_ABYSS3 ) scr_excep_set( pl_w, 1, 1 ) ;
	lw->pl05_wall_flag1 = 1 ;
	pl_char_set( pl_w, Char_Extra, 6 ) ;
}

/*--------------------------------------------------------------*/
static void stick_dash_stick( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		lw->pl05_wall_flag = 0 ;
		pl_w->timer0 = 240 ;
		sp_stick_chg( pl_w, lw, 0 ) ;
	}
}

static void stick_slide_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_slide_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_slide_end( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			A kick sliding down the wall						*/
/*==============================================================*/
static void stick_slide( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_slide_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_slide_init, stick_slide_move, stick_slide_end,
	} ;

	stick_slide_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_slide_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl05_at_kabe_slide_k ;
	HIT_RESET( pl_w ) ;
	lw->leap_timer = 28 ;
	pl_w->spd.y = -DOT_Y( 6.0f ) ;
	pl_w->acc.y = DOT_Y( 0.125f ) ;
	pl_char_set( pl_w, Char_Extra, 14 ) ;
	stick_slide_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_slide_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl05_kdash_ck( pl_w, lw ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y <= pl_w->ground_y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
	}
	if( !--lw->leap_timer ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Extra, 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stick_slide_end( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl05_kdash_ck( pl_w, lw ) ;
	if( pl_char_move( pl_w ) < 0 ) sp_stick_chg( pl_w, lw, 0 ) ;
}

static void stick_off_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_off_fall( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void stick_off_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			Letting go, back to the ground						*/
/*==============================================================*/
static void stick_off( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static void ( *stick_off_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		stick_off_init, stick_off_fall, stick_off_land,
	} ;

	stick_off_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void stick_off_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	pl_char_set( pl_w, Char_Jump, 1 ) ;
	stick_off_fall( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stick_off_fall( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		SCR_EXCEP_SET ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void stick_off_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			To another move on the wall, unless it is time		*/
/*			to let go											*/
/*==============================================================*/
static void sp_stick_chg( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw, Uint8 no )
{
	if( lw->pl05_wall_flag || !lw->wall_cnt ) sp_stick_end( pl_w, lw ) ;
	else {
		pl_w->timer1 = no ;
		pl_w->routine_3 = 0 ;
	}
}

/*==============================================================*/
/*			The end of the wall cling							*/
/*==============================================================*/
static void sp_stick_end( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	lw->pl05_wall_flag = 0 ;
	lw->pl05_wall_flag1 = 0 ;

	pl_w->jump_atk_cnt = 0 ;
	pl_w->jump_com_cnt = 1 ;

	SCR_EXCEP_SET ;

	RATE_CLR_X( pl_w ) ;

	pl_jump_ret( pl_w ) ;
}

static Uint8 pl05_kdash_ck_back( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static Uint8 pl05_kdash_ck_forward( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			The wall dash command: back, then forward in		*/
/*			time												*/
/*==============================================================*/
static Uint8 pl05_kdash_ck( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static Uint8 ( *pl05_kdash_ck_tbl[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		pl05_kdash_ck_back,
		pl05_kdash_ck_forward,
	} ;

	return pl05_kdash_ck_tbl[lw->pl05_com_st]( pl_w, lw ) ;
}

/*==============================================================*/
static Uint8 pl05_kdash_ck_back( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( !( pl_w->pl_sw_now & BACK ) ) return 0 ;
	lw->pl05_com_st ++ ;
	lw->com_timer = 15 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 pl05_kdash_ck_forward( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( !--lw->com_timer ){
		lw->pl05_com_st = 0 ;
		return 0 ;
	} else {
		if( !( pl_w->pl_sw_now & KEY_FORWARD ) ) return 0 ;
		lw->pl05_com_st = 0 ;
		return 1 ;
	}
}

static void formation_a_init( PLAYER_WORK *pl_w );
static void formation_a_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_formation_a( PLAYER_WORK *pl_w )
{
	static void ( *formation_a_jmp[])( PLAYER_WORK *pl_w ) = {
		formation_a_init, formation_a_move,
	} ;

	formation_a_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void formation_a_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl05_at_vc_formation_a ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl_w->atck_level ? 0x5d : 0x44 ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 17 ) ;
}

/*--------------------------------------------------------------*/
static void formation_a_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( pl_w->atck_level ) shl05_02_set( pl_w, 6 ) ;
		else shl05_02_set( pl_w, 0 ) ;
	}
}

static void legion_init( PLAYER_WORK *pl_w );
static void legion_wait( PLAYER_WORK *pl_w );
static void legion_move( PLAYER_WORK *pl_w );
static void legion_keep( PLAYER_WORK *pl_w );
static void legion_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_legion( PLAYER_WORK *pl_w )
{
	static void ( *legion_jmp[])( PLAYER_WORK *pl_w ) = {
		legion_init, legion_wait, legion_move, legion_keep, legion_end,
	} ;

	legion_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void legion_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl05_at_legion ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void legion_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 111 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = 25.0f ;
		ofs.y = DOT_Y( 93.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void legion_move( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->timer0 = 40 ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl05_02_legion_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void legion_keep( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( --pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 33 ) ;
	}
}

/*--------------------------------------------------------------*/
static void legion_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/* the speeds of the air rush: punch or kick, then the strength */
typedef struct RUSH_SPD {
	Float	spd_x;
	Float	spd_y;
} RUSH_SPD;

static void jexcalibur_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void jexcalibur_start( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void jexcalibur_rush( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void jexcalibur_fall( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void jexcalibur_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void jexcalibur_hit( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			A rush through the air, hitting up to five times	*/
/*==============================================================*/
static void sp_jexcalibur( PLAYER_WORK *pl_w )
{
	static void ( *jexcalibur_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		jexcalibur_init, jexcalibur_start, jexcalibur_rush, jexcalibur_fall, jexcalibur_land,
	} ;

	jexcalibur_jmp[pl_w->routine_2]( pl_w, ( PL05_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void jexcalibur_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	int no ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_AIR ;
	lw->rush_cnt = 5 ;
	no = pl_w->atck_pk ? 0x4f : 0x4c ;
	pl_w->atck_no = no + ( pl_w->atck_level << 1 ) ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 18 ) ;
}

/*--------------------------------------------------------------*/
static void jexcalibur_start( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	static RUSH_SPD rush_spd[4] = {
		{ 10.0f, DOT_Y( 12.0f ) }, { DOT_X( 14.0f ), DOT_Y( 2.0f ) },
		{ 10.0f, -DOT_Y( 12.0f ) }, { DOT_X( 14.0f ), -DOT_Y( 2.0f ) },
	} ;
	Sint32 no ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->timer0 = 32 ;
		no = pl_w->atck_pk << 1 ;
		pl_w->spd.x = pl_w->pl_dir ? rush_spd[no + pl_w->atck_level].spd_x : -rush_spd[no + pl_w->atck_level].spd_x ;
		pl_w->spd.y = rush_spd[no + pl_w->atck_level].spd_y ;
		pl_voice_req( pl_w, 6 ) ;
		pl_w->char_no.b.no = pl_w->atck_pk ? 21 : 24 ;
		pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void jexcalibur_rush( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	shadow_set( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		jexcalibur_hit( pl_w, lw ) ;
	}
	if( !pl_w->hit_flag || ( !( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) && !pl_w->em_w->armor_flag ) ){
		if( !( pl_w->wall_flag & ( WALL_RIGHT | WALL_LEFT ) ) || ( ( pl_w->pl_dir + 1 ) & pl_w->wall_flag & ( WALL_RIGHT | WALL_LEFT ) ) ){
			if( pl_w->pos.y <= pl_w->ground_y ){
				pl_w->routine_2 = 4 ;
				pl_w->pos.y = pl_w->ground_y ;
				pl_w->pl_stance = STANCE_STAND ;
				pl_w->spd.x = 0.0f ;
				pl_w->spd.y = 0.0f ;
				pl_w->acc.x = 0.0f ;
				pl_w->acc.y = 0.0f ;
				pl_land_ef_set( pl_w ) ;
				pl_char_set( pl_w, Char_Special, 27 ) ;
				return ;
			}
			if( --pl_w->timer0 ) return ;
		}
	}
	pl_w->routine_2 ++ ;
	pl_w->spd.x /= 8.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Special, 53 ) ;
}

/*--------------------------------------------------------------*/
static void jexcalibur_fall( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
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
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jexcalibur_land( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Each hit of the rush counts as a new attack			*/
/*==============================================================*/
static void jexcalibur_hit( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	int no ;

	if( lw->rush_cnt ){
		lw->rush_cnt -- ;
		pl_w->push_ng_timer = 16 ;
		no = pl_w->atck_pk ? 0x4f : 0x4c ;
		pl_w->atck_no = no + ( pl_w->atck_level << 1 ) ;
		HIT_RESET( pl_w ) ;
	}
}

static void ragnarok_init( PLAYER_WORK *pl_w );
static void ragnarok_wait( PLAYER_WORK *pl_w );
static void ragnarok_start( PLAYER_WORK *pl_w );
static void ragnarok_dash( PLAYER_WORK *pl_w );
static void ragnarok_miss( PLAYER_WORK *pl_w );
static void ragnarok_catch( PLAYER_WORK *pl_w );
static void ragnarok_up( PLAYER_WORK *pl_w );
static void ragnarok_hide( PLAYER_WORK *pl_w );
static void ragnarok_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a rush that throws the opponent		*/
/*			if it connects										*/
/*==============================================================*/
static void sp_ragnarok( PLAYER_WORK *pl_w )
{
	static void ( *ragnarok_jmp[])( PLAYER_WORK *pl_w ) = {
		ragnarok_init, ragnarok_wait, ragnarok_start, ragnarok_dash, ragnarok_miss,
		ragnarok_catch, ragnarok_up, ragnarok_hide, ragnarok_end,
	} ;

	ragnarok_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void ragnarok_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl05_at_ragnarok ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 4 ) ;
}

/*--------------------------------------------------------------*/
static void ragnarok_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -30.0f ;
		ofs.y = DOT_Y( 82.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ragnarok_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->timer0 = 24 ;
		pl_char_set( pl_w, Char_Super, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ragnarok_dash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->hit_flag ){
		pl_w->timer0 = 1 ;
		if( pl_sp_catch_ck( pl_w ) ){
			scr_excep_set( pl_w, 8, 8 ) ;
			pl_w->routine_2 += 2 ;
			pl_w->catch_no = 0xc2 ;
			pl_sp_catch_set( pl_w, pl_w->em_w ) ;
			pl_w->hit_stop = 10 ;
			ofs.x = -DOT_X( 58.0f ) ;
			ofs.y = DOT_Y( 60.0f ) ;
			ofs.z = 0.0f ;
			nage_hit_set( pl_w, &ofs ) ;
			pl_voice_req( pl_w, 5 ) ;
			pl_char_set( pl_w, Char_Catch, 5 ) ;
			return ;
		}
	}
	if( --pl_w->timer0 ) return ;
	pl_w->routine_2 ++ ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 7.0f ) : -DOT_X( 7.0f ) ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
	pl_char_set( pl_w, Char_Super, 31 ) ;
}

/*--------------------------------------------------------------*/
static void ragnarok_miss( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_smoke_set2( pl_w, 3 ) ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ragnarok_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->timer0 = 12 ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -0.4017857f ;
	}
}

/*--------------------------------------------------------------*/
static void ragnarok_up( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

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
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 13 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl05_at_ragnarok_throw ;
		SCR_EXCEP_SET ;
		shl05_04_set( pl_w ) ;
		pl_w->disp_flag = 0 ;
		pl_w->timer0 = 192 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void ragnarok_hide( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_w->disp_flag = 0 ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		scr_excep_set( pl_w, 7, 7 ) ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		if( !Sys->voice_type ) pl_voice_req( pl_w, 15 ) ;
		pl_char_set( pl_w, Char_Super, 32 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ragnarok_end( PLAYER_WORK *pl_w )
{
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		SCR_EXCEP_SET ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void vajra_init( PLAYER_WORK *pl_w );
static void vajra_start( PLAYER_WORK *pl_w );
static void vajra_jump( PLAYER_WORK *pl_w );
static void vajra_out( PLAYER_WORK *pl_w );
static void vajra_in( PLAYER_WORK *pl_w );
static void vajra_dive( PLAYER_WORK *pl_w );
static void vajra_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Vajra: a leap back, a warp above the far side,		*/
/*			and a dive back down								*/
/*==============================================================*/
static void sp_vajra( PLAYER_WORK *pl_w )
{
	static void ( *vajra_jmp[])( PLAYER_WORK *pl_w ) = {
		vajra_init, vajra_start, vajra_jump, vajra_out, vajra_in, vajra_dive, vajra_land,
	} ;

	vajra_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void vajra_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl05_at_vc_vajra ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl05_at_vajra ;
		HIT_RESET( pl_w ) ;
	}
	pl_gauge_add( pl_w, 13 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 15 ;
	pl_char_set( pl_w, Char_Special, 28 ) ;
}

/*--------------------------------------------------------------*/
static void vajra_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->push_ng_timer = 2 ;
		pl_w->wall_ng_timer = 2 ;
		pl_w->timer1 = 8 ;
		shl05_05_set( pl_w, 0 ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_voice_req( pl_w, 6 ) ;
		pl_se_req( pl_w, 50 ) ;
		pl_char_set( pl_w, Char_Special, 31 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vajra_jump( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 2 ;
	pl_w->wall_ng_timer = 2 ;
	if( --pl_w->timer1 ){
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 35 ) ;
}

/*--------------------------------------------------------------*/
static void vajra_out( PLAYER_WORK *pl_w )
{
	pl_w->wall_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->team_type == 8 || pl_w->team_type == 3 ) pl_w->pos.x = pl_w->target_w->pos.x ;
		else pl_w->pos.x = scr_work.eye[0].x + ( pl_w->atck_level ? 640.0f : DOT_X( 128.0f ) ) + -320.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		shl05_05_set( pl_w, 1 ) ;
		pl_w->pos.x += pl_w->pl_dir ? -DOT_X( 80.0f ) : DOT_X( 80.0f ) ;
		pl_w->pos.y += DOT_Y( 160.0f ) ;
		pl_char_set( pl_w, Char_Special, 47 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vajra_in( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->spd.y = -DOT_Y( 10.0f ) ;
		pl_voice_req( pl_w, 4 ) ;
		pl_se_req( pl_w, 75 ) ;
		pl_char_set( pl_w, Char_Special, 39 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vajra_dive( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void vajra_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void formation_c_init( PLAYER_WORK *pl_w );
static void formation_c_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Formation C: calls the bird that drops a bomb		*/
/*==============================================================*/
static void sp_formation_c( PLAYER_WORK *pl_w )
{
	static void ( *formation_c_jmp[])( PLAYER_WORK *pl_w ) = {
		formation_c_init, formation_c_move,
	} ;

	formation_c_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void formation_c_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl05_at_formation_c ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 49 ) ;
}

/*--------------------------------------------------------------*/
static void formation_c_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl05_03_set( pl_w, 0 ) ;
	}
}

static void formation_b_init( PLAYER_WORK *pl_w );
static void formation_b_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Formation B: calls the panther that waits by him	*/
/*==============================================================*/
static void sp_formation_b( PLAYER_WORK *pl_w )
{
	static void ( *formation_b_jmp[])( PLAYER_WORK *pl_w ) = {
		formation_b_init, formation_b_move,
	} ;

	formation_b_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void formation_b_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl05_at_formation_b ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 50 ) ;
}

/*--------------------------------------------------------------*/
static void formation_b_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl05_02_set( pl_w, 4 ) ;
	}
}

static void formation_b_shot_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );
static void formation_b_shot_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw );

/*==============================================================*/
/*			Formation B again: sends the waiting panther off	*/
/*==============================================================*/
static void sp_formation_b_shot( PLAYER_WORK *pl_w )
{
	static void ( *formation_b_shot_jmp[])( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw ) = {
		formation_b_shot_init, formation_b_shot_move,
	} ;

	formation_b_shot_jmp[pl_w->routine_2]( pl_w, ( PL05_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void formation_b_shot_init( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl05_at_formation_b ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->pl_stance == STANCE_AIR ? 52 : 51 ) ;
}

/*--------------------------------------------------------------*/
static void formation_b_shot_move( PLAYER_WORK *pl_w, PL05_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_jump_ret( pl_w ) ;
		} else {
			pl_w->pl_stance = STANCE_STAND ;
			pl_stand_ret( pl_w ) ;
		}
		return ;
	}
	if( !pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		lw->panther_go_flag = 1 ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
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
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl05_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 54 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3	ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_flag1 & 1 ){
		pl_w->cg_flag1 = 0 ;
		ofs.x = -DOT_X( 13.0f ) ;
		ofs.y = DOT_Y( 92.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	} else if( pl_w->cg_flag1 & 2 ){
		pl_w->cg_flag1 = 0 ;
		pl_voice_req( pl_w, 30 ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl05_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl05_at_t_kick ;
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
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stop_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
	}
}

/*==============================================================*/
/*			The taunt											*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 16 ) ;
	} else if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Leaves an afterimage every eight frames				*/
/*==============================================================*/
static void shadow_set( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;

	if( -- lw->zanzo_timer < 0 ){
		lw->zanzo_timer = 8 ;
		pls05_02_set( pl_w, 1, 12 ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl05_catch_ck( PLAYER_WORK *pl_w )
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
			pl_w->catch_no = 3 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
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

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_p_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl05_catch_ck chose			*/
/****************************************************************/
void	pl05_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_set, jump_p_set, nml_k_set, nml_p_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	scr_excep_set( pl_w, 5, 5 ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	ofs.x = -DOT_X( 43.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
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
	pl_w->hit_stop = 10 ;
	ofs.x = -DOT_X( 58.0f ) ;
	ofs.y = DOT_Y( 60.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_voice_req( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Catch, 5 ) ;
}

static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_p_catch( PLAYER_WORK *pl_w );
static void nml_p_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl05_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch, jump_p_catch, nml_k_catch, nml_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_k_catch_init( PLAYER_WORK *pl_w );
static void nml_k_catch_start( PLAYER_WORK *pl_w );
static void nml_k_catch_jump( PLAYER_WORK *pl_w );
static void nml_k_catch_throw( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The kick throw: leaps up with the opponent and		*/
/*			throws him on landing								*/
/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_catch_init, nml_k_catch_start, nml_k_catch_jump, nml_k_catch_throw,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos[3] ;	/* the move to the throw's place, the start; the frame has a third, unused */

	pl_w->routine_2 ++ ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	em = pl_w->catch_w ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	em->spd.x = 0.0f ;
	em->spd.y = 0.0f ;
	em->acc.x = 0.0f ;
	em->acc.y = 0.0f ;
	pos[0] = pos[1] = pl_w->pos ;
	catch_em_pos_set0( pl_w, em ) ;
	pos[0].x = pl_w->pos.x - pos[0].x ;
	pos[0].y = pl_w->pos.y - pos[0].y ;
	pl_w->pos = pos[1] ;
	pl_w->spd.x = pos[0].x / 16.0f ;
	pl_w->spd.y = pos[0].y / 16.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y += DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_w->timer0 = 16 ;
}

/*--------------------------------------------------------------*/
static void nml_k_catch_start( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !--pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->hit_stop = 10 ;
		ofs.x = 20.0f ;
		ofs.y = DOT_Y( 54.0f ) ;
		ofs.z = 0.0f ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->pl_stance = STANCE_AIR ;
			SCR_EXCEP_SET ;
			em->atck_no = pl05_at_throw_k ;
		}
		return ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/* x, y pairs that no code reads */
static Float unk_xy_tbl[59][2] = {
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 40.0f ), DOT_Y( 24.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ 60.0f, DOT_Y( 5.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 28.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ 30.0f, DOT_Y( 2.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 16.0f ) },
	{ DOT_X( 22.0f ), DOT_Y( 2.0f ) },
	{ DOT_X( 20.0f ), DOT_Y( 24.0f ) },
	{ DOT_X( 8.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ 20.0f, DOT_Y( 3.0f ) },
	{ 40.0f, -DOT_Y( 5.0f ) },
	{ DOT_X( 22.0f ), -DOT_Y( 16.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ 30.0f, DOT_Y( 8.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 16.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 32.0f ), DOT_Y( 6.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 8.0f ) },
	{ 30.0f, DOT_Y( 2.0f ) },
	{ DOT_X( 16.0f ), DOT_Y( 6.0f ) },
} ;

static void jump_p_catch_start( PLAYER_WORK *pl_w );
static void jump_p_catch_fall( PLAYER_WORK *pl_w );
static void jump_p_catch_land( PLAYER_WORK *pl_w );
static void jump_p_catch_back( PLAYER_WORK *pl_w );
static void jump_p_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The air throw: drops with the opponent and kicks	*/
/*			him away											*/
/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_catch_start, jump_p_catch_fall, jump_p_catch_land, jump_p_catch_back, jump_p_catch_end,
	} ;

	jump_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_fall( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pos = pl_w->pos ;
		pos.x += pl_w->pl_dir ? -8 : 8 ;
		land_dust_set( &pos, -1 ) ;
		catch_se_req( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_land( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		SCR_EXCEP_SET ;
		em->atck_no = pl05_at_air_throw_p ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.125f ) : -DOT_X( 0.125f ) ;
		pl_char_set( pl_w, Char_Dash, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_catch_back( PLAYER_WORK *pl_w )
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
static void jump_p_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void nml_p_catch_start( PLAYER_WORK *pl_w );
static void nml_p_catch_up( PLAYER_WORK *pl_w );
static void nml_p_catch_air( PLAYER_WORK *pl_w );
static void nml_p_catch_fall( PLAYER_WORK *pl_w );
static void nml_p_catch_land( PLAYER_WORK *pl_w );
static void nml_p_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The punch throw: leaps up with the opponent and		*/
/*			slams him down										*/
/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch_start, nml_p_catch_up, nml_p_catch_air, nml_p_catch_fall, nml_p_catch_land,
		nml_p_catch_end,
	} ;

	nml_p_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_up( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y <= 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_air( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 10.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_fall( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pos = pl_w->pos ;
		pos.x += pl_w->pl_dir ? -8 : 8 ;
		land_dust_set( &pos, -1 ) ;
		catch_se_req( pl_w ) ;
		pl_char_set( pl_w, Char_Catch, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_land( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl05_at_throw_p ;
		SCR_EXCEP_SET ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.125f ) : -DOT_X( 0.125f ) ;
		pl_char_set( pl_w, Char_Catch, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_catch_end( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void nml_k_hold( PLAYER_WORK *pl_w );
static void jump_p_hold( PLAYER_WORK *pl_w );
static void nml_p_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The opponent's side of a throw						*/
/****************************************************************/
void	pl05_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_hold, jump_p_hold, nml_p_hold, nml_p_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_hold( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->routine_2 <= 1 ) return ;
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void jump_p_hold( PLAYER_WORK *pl_w )
{
	if( !pl_w->catch_w->routine_2 ) catch_em_pos_set1( pl_w->catch_w, pl_w ) ;
	else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void nml_p_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl05_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl05_sp_legion ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl05_sp_legion ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl05_sp_legion ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl05_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl05_sp_legion ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl05_sp_legion ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl05_sp_legion ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl05_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl05_sp_murakumo ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl05_sp_vajra ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl05_sp_formation_a ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl05_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl05_sp_murakumo ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl05_sp_vajra ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl05_sp_formation_a ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			Clears the character's own work, keeping			*/
/*			panther_flag once the round is under way			*/
/****************************************************************/
void	pl05_tlocal_init( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *lw = ( PL05_LOCAL_W * )pl_w->tlocal ;
	Uint8 keep = vs_sys_work.vs_st >= VS_END_WAIT ? lw->panther_flag : 0 ;

	memset( lw, 0, sizeof( pl_w->tlocal ) ) ;
	lw->panther_flag = keep ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl05.c" End of File	*/
