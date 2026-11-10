/* Spiral (character 31): command checks, the swords and the glow, normal attacks, dashes,
   appearance, win poses, special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl31_def.h"

void pl31_main( PLAYER_WORK *pl_w );
void pl31_com_ck( PLAYER_WORK *pl_w );
void pl31_atck_set( PLAYER_WORK *pl_w );
void pl31_atck( PLAYER_WORK *pl_w );
void pl31_dash( PLAYER_WORK *pl_w );
void pl31_noki( PLAYER_WORK *pl_w );
void pl31_appear( PLAYER_WORK *pl_w );
void pl31_win( PLAYER_WORK *pl_w );
void pl31_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl31_catch_ck( PLAYER_WORK *pl_w );
void pl31_catch_set( PLAYER_WORK *pl_w );
void pl31_catch( PLAYER_WORK *pl_w );
void pl31_hold( PLAYER_WORK *pl_w );
int pl31_delay_ck( PLAYER_WORK *pl_w );
void pl31_t_kick( PLAYER_WORK *pl_w );
void pl31_upper( PLAYER_WORK *pl_w );
void pl31_sp_unison( PLAYER_WORK *pl_w );
void pl31_sp_gattai( PLAYER_WORK *pl_w );
void pl31_sp_assist( PLAYER_WORK *pl_w );
void pl31_sp_zero( PLAYER_WORK *pl_w );

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
static Sint8 sp_atck_tbl[4] = { -1, -1, -1, -1 } ;	/* the attacks while sped up */

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_sword[] = { 5, 0, pl31_sp_sword, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Dancing Sword */
static Uint16 com_tobashi[] = { 3, 0, pl31_sp_tobashi, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_ATCK } ;	/* Tsurugi Tobashi */
static Uint16 com_tele[] = { 3, 0, pl31_sp_tele, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Teleport Dance */
static Uint16 com_grapple[] = { 3, 0, pl31_sp_grapple, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Six-hand Grapple */
static Uint16 com_metamor[] = { 3, COM_HYPER | COM_ALL_SW, pl31_sp_metamor, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Metamorphose (level 3 hyper): the speed up, then a punch's hyper throw */
static Uint16 com_power[] = { 3, COM_HYPER | COM_ALL_SW, pl31_sp_power, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Power Dance (hyper) */
static Uint16 com_speed[] = { 3, COM_HYPER | COM_ALL_SW, pl31_sp_power, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Speed Dance (hyper); speed_ck sets pl31_sp_speed */
static Uint16 com_hmetamor[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl31_sp_hmetamor, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hyper Metamorphose: the gamma type's team hyper (this command is not checked) */
static Uint16 com_stampede[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl31_sp_stampede, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Stampede Sword (hyper) */

static void ( *pl31_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl31_main,
	pl31_com_ck,
	pl31_atck_set,
	pl31_atck,
	pl31_dash,
	pl31_noki,
	pl31_appear,
	pl31_win,
	pl31_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl31_catch_ck,	/* returns a value */
	pl31_catch_set,
	pl31_catch,
	pl31_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl31_delay_ck,
	pl31_t_kick,
	pl31_upper,
	pl31_sp_unison,
	pl31_sp_gattai,
	pl31_sp_assist,
	pl31_sp_zero,
} ;

static Uint8 metamor_ck( PLAYER_WORK *pl_w );
static Uint8 stampede_ck( PLAYER_WORK *pl_w );
static Uint8 power_ck( PLAYER_WORK *pl_w );
static Uint8 speed_ck( PLAYER_WORK *pl_w );
static Uint8 sp_sword_ck( PLAYER_WORK *pl_w );
static Uint8 tobashi_ck( PLAYER_WORK *pl_w );
static Uint8 tele_ck( PLAYER_WORK *pl_w );
static Uint8 grapple_ck( PLAYER_WORK *pl_w );
static Uint8 snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int stampede_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl31_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl31_tbl[i] ;
}

/****************************************************************/
/*			Commands; sped up, the moves that make the most		*/
/*			of it												*/
/****************************************************************/
void pl31_com_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( !lw->maxspeed_flag ){
		if( pl_v_combi_ck( pl_w ) ) return ;
	}
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( metamor_ck( pl_w ) ) return ;
	if( stampede_ck( pl_w ) ) return ;
	if( power_ck( pl_w ) ) return ;
	if( speed_ck( pl_w ) ) return ;
	if( sp_sword_ck( pl_w ) ) return ;
	if( tobashi_ck( pl_w ) ) return ;
	if( tele_ck( pl_w ) ) return ;
	if( grapple_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Not with all six swords out; once a jump unless		*/
/*			super jumping										*/
/*==============================================================*/
static Uint8 sp_sword_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;
	int i, n ;

	if( !com_nml_ck( pl_w, com_sword, &pl_w->com_w0 ) ) return 0 ;
	n = 0 ;
	for( i = 0 ; i < 6 ; i++ ) if( lw->sword_flag[i] ) n ++ ;
	if( n == 6 ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_sword ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			With a sword out; a kick makes it stronger			*/
/*==============================================================*/
static Uint8 tobashi_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;
	int i, n ;

	if( !com_nml_ck( pl_w, com_tobashi, &pl_w->com_w1 ) ) return 0 ;
	n = 0 ;
	for( i = 0 ; i < 6 ; i++ ) if( !lw->sword_flag[i] ) n ++ ;
	if( n == 6 ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->atck_level += pl_w->atck_pk * 2 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_tobashi ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground, not with one kick					*/
/*==============================================================*/
static Uint8 tele_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tele, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance != STANCE_AIR && ( pl_w->com_w2.sw & 0x0020 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->atck_level ;
	pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_tele ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 grapple_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_grapple, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_grapple ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A level 3 hyper combo, not while sped up			*/
/*==============================================================*/
static Uint8 metamor_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ) return 0 ;
	if( !com_nml_ck( pl_w, com_metamor, &pl_w->com_w4 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_metamor ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 power_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ) return 0 ;
	if( !com_nml_ck( pl_w, com_power, &pl_w->com_w5 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_power ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 speed_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ) return 0 ;
	if( !com_nml_ck( pl_w, com_speed, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_speed ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 stampede_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ) return 0 ;
	if( !com_nml_ck( pl_w, com_stampede, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl31_sp_stampede ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl31_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 6 ) ) return 0 ;
	pl_w->pl_sp_type = pl31_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			A hyper combo entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl31_delay_ck( PLAYER_WORK *pl_w )
{
	if( stampede_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int stampede_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_stampede, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl31_sp_stampede ;
	return 1 ;
}

static void sword_ck( PLAYER_WORK *pl_w );
static void glow_ck( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/****************************************************************/
/*			The swords and the glow								*/
/****************************************************************/
void pl31_main( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	sword_ck( pl_w ) ;
	glow_ck( pl_w, lw ) ;
}

/*==============================================================*/
/*			The button fires the first sword out				*/
/*==============================================================*/
static void sword_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;
	int i ;

	if( pl_w->routine_1 == PL_NORM && pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER
	 && pl_w->act_st != N_GUARD && pl_w->act_st != N_AIR_GUARD && ( pl_w->pl_sw_now & KEY_LP ) ){
		for( i = 0 ; i < 6 ; i++ ){
			if( lw->sword_flag[i] ){
				lw->fire_flag[i] = 1 ;
				return ;
			}
		}
	}
}

/* the glow's colours, from 2 on a loop */
static Uint8 glow_power_tbl[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 0xff } ;
static Uint8 glow_speed_tbl[15] = { 0, 1, 8, 9, 10, 11, 12, 13, 13, 12, 11, 10, 9, 8, 0xff } ;
static Uint8 glow_maxspeed_tbl[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0xff } ;

/*==============================================================*/
/*			Not while a hit stops the action					*/
/*==============================================================*/
static void glow_power( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( lw->glow_timer ){
		if( !pl_w->hit_stop ) lw->glow_timer -- ;
		if( !lw->col_timer -- ){
			lw->col_timer = 4 ;
			lw->glow_col ++ ;
			if( glow_power_tbl[lw->glow_col] == 0xff ) lw->glow_col = 2 ;
			ef_color_set( pl_w, 0, glow_power_tbl[lw->glow_col] + pl_w->pl_color * 28 + 34, 1 ) ;
		}
	} else {
		pl_color_set( pl_w, PC_DEFAULT ) ;
		lw->glow = 0 ;
	}
}

/*==============================================================*/
static void glow_speed( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( lw->glow_timer ){
		if( !pl_w->hit_stop ) lw->glow_timer -- ;
		if( !lw->col_timer -- ){
			lw->col_timer = 4 ;
			lw->glow_col ++ ;
			if( glow_speed_tbl[lw->glow_col] == 0xff ) lw->glow_col = 2 ;
			ef_color_set( pl_w, 0, glow_speed_tbl[lw->glow_col] + pl_w->pl_color * 28 + 34, 1 ) ;
		}
	} else {
		pl_color_set( pl_w, PC_DEFAULT ) ;
		lw->glow = 0 ;
	}
}

/*==============================================================*/
/*			The speed up's glow; when it ends she slows down	*/
/*==============================================================*/
static void glow_maxspeed( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( lw->glow_timer ){
		lw->glow_timer -- ;
		if( !lw->col_timer -- ){
			lw->col_timer = 4 ;
			lw->glow_col ++ ;
			if( glow_maxspeed_tbl[lw->glow_col] == 0xff ) lw->glow_col = 2 ;
			ef_color_set( pl_w, 0, glow_maxspeed_tbl[lw->glow_col] + pl_w->pl_color * 28 + 48, 1 ) ;
		}
	} else {
		pl_color_set( pl_w, PC_DEFAULT ) ;
		lw->glow = 0 ;
		lw->maxspeed_flag = 0 ;
	}
}

/*==============================================================*/
static void glow_none( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
}

/*==============================================================*/
/*			The glow ends with the player's vitality			*/
/*==============================================================*/
static void glow_ck( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *glow_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		glow_none, glow_power, glow_speed, glow_maxspeed,
	} ;

	if( !pl_w->vital_new ) lw->glow_timer = 0 ;
	glow_jmp[lw->glow]( pl_w, lw ) ;
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
static void sp_stand_punch( PLAYER_WORK *pl_w );
static void sp_crouch_punch( PLAYER_WORK *pl_w );
static void sp_jump_punch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl31_atck_set( PLAYER_WORK *pl_w )
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
/*			Sped up, the punches are others						*/
/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ){
		sp_stand_punch( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ){
		sp_crouch_punch( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		pl_se_req( pl_w, SE_SWING_M ) ;
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
/*			The medium and heavy punches aim down with the		*/
/*			stick												*/
/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ){
		sp_jump_punch( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl31_at_jdown_mp ;
		}
		break ;
	case 2:
		pl_w->char_no.b.grp = 11 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_w->char_no.b.grp = 11 ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_DOWN ){
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 7 ;
			pl_w->atck_no = pl31_at_jdown_hp ;
		}
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
/*			The medium kick aims up with the stick				*/
/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.grp = 12 ;
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.grp = 12 ;
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.grp = 20 ;
			pl_w->char_no.b.no = 8 ;
			pl_w->atck_no = pl31_at_jup_mk ;
		}
		break ;
	case 2:
		pl_w->char_no.b.grp = 12 ;
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
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
static void sp_punch_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl31_atck( PLAYER_WORK *pl_w )
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
/*			The heavy punch hits twice							*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ){
		sp_punch_move( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_w->atck_no = pl31_at_stand_hp_2 ;
			HIT_RESET( pl_w ) ;
		}
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			pl_w->atck_no = ATCK_STAND_HP ;
			HIT_RESET( pl_w ) ;
		}
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ){
		sp_punch_move( pl_w ) ;
		return ;
	}
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
	switch( pl_w->nml_no ){
	case 0:
	case 1:
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
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	if( lw->maxspeed_flag ){
		sp_punch_move( pl_w ) ;
		return ;
	}
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
void pl31_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->atck_no = pl31_at_upper ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		HIT_RESET( pl_w ) ;
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
void pl31_dash( PLAYER_WORK *pl_w )
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
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
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
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
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
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl31_noki( PLAYER_WORK *pl_w )
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
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.375f ) : -DOT_X( 0.375f ) ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
	}
}

/*==============================================================*/
/*			On landing it slides to a stop						*/
/*==============================================================*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->acc.x = -( pl_w->spd.x / 8.0f ) ;
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

static void appear_init( PLAYER_WORK *pl_w );
static void appear_drop( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl31_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_drop, appear_wait, appear_fall, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			She comes down from above, unless the round's		*/
/*			start skips it										*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st < VS_APPEAR ){
		pl_w->disp_flag = 0 ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->pos.x -= pl_w->disp_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	pl_w->pos.y -= DOT_Y( 88.0f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 1.5f ) ;
	pl_w->acc.y = 0.0502232127f ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	pl_voice_req( pl_w, 32 ) ;
	pls31_00_set( pl_w, 0, 0 ) ;
	pls31_00_set( pl_w, 0, 1 ) ;
}

/*--------------------------------------------------------------*/
static void appear_drop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = -DOT_Y( 2.25f ) ;
		pl_w->acc.y = -0.133928567f ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void win_time( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );
static void speed_clear( PLAYER_WORK *pl_w );

/****************************************************************/
void pl31_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
		speed_clear( pl_w ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls31_00_set( pl_w, 1, 0 ) ;
		}
	}
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
		speed_clear( pl_w ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 1 ) ;
		speed_clear( pl_w ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 2 ) ;
		speed_clear( pl_w ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The speed up and the glow end						*/
/*==============================================================*/
static void speed_clear( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;

	lw->maxspeed_flag = 0 ;
	lw->glow = 0 ;
	lw->glow_timer = 0 ;
	lw->col_timer = 0 ;
	lw->glow_col = 0 ;
	pl_w->speed_up_flag = 0 ;
	pl_w->speed_plus_timer = 0 ;
	pl_w->atck_plus_timer = 0 ;
	pl_w->atck_plus = 0 ;
}

static void sp_sword( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_tobashi( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_tele( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_grapple( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_metamor( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_snapback( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_swords( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_stand_hp( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_crouch_hp( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_power( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_speed( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_hmetamor( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void sp_stampede( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/****************************************************************/
void pl31_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		sp_sword, sp_tobashi, sp_tele, sp_grapple, sp_metamor, sp_snapback, sp_chouhatsu,
		sp_swords, sp_stand_hp, sp_crouch_hp, sp_power, sp_speed, sp_hmetamor, sp_stampede,
	} ;

	sp_jmp[pl_w->pl_sp_type]( pl_w, ( PL31_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
/*			Slows a move begun in the air						*/
/*==============================================================*/
static void air_slow( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= 8.0f ;
		pl_w->acc.x /= 8.0f ;
		pl_w->spd.y /= 8.0f ;
		pl_w->acc.y /= 8.0f ;
	}
}

/*==============================================================*/
static void air_move( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_land_ck( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_end( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->jump_dir = -1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 1.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_sjump_ret( pl_w ) ;
	} else pl_stand_ret( pl_w ) ;
}

static void sword_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The swords come out									*/
/*==============================================================*/
static void sword_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	int i ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	air_slow( pl_w ) ;
	for( i = 0 ; i < 6 ; i ++ ) lw->fire_flag[i] = 0 ;
	lw->fire_level = 0 ;
	sword_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void sword_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	int i ;

	if( pl_char_move( pl_w ) < 0 ){
		sp_end( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		for( i = 0 ; i < 6 ; i ++ ){
			if( !lw->sword_flag[i] ){
				lw->sword_flag[i] = 1 ;
				shl31_00_set( pl_w, 0, i ) ;
			}
		}
		pl_voice_req( pl_w, 34 ) ;
	}
	air_move( pl_w ) ;
}

/*==============================================================*/
static void sp_sword( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *sword_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		sword_init, sword_move,
	} ;

	sword_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void tobashi_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The swords are fired								*/
/*==============================================================*/
static void tobashi_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl31_at_tobashi ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
	air_slow( pl_w ) ;
	tobashi_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void tobashi_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		sp_end( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		lw->fire_level = pl_w->atck_level + 1 ;
	}
	air_move( pl_w ) ;
}

/*==============================================================*/
static void sp_tobashi( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *tobashi_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		tobashi_init, tobashi_move,
	} ;

	tobashi_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void tele_tele( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void tele_land( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
static void sp_tele( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *sp_tele_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		tele_tele, tele_land,
	} ;

	sp_tele_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void tele_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void tele_out( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void tele_wait( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void tele_in( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The teleport										*/
/*==============================================================*/
static void tele_tele( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *tele_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		tele_init, tele_out, tele_wait, tele_in,
	} ;

	tele_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void tele_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_color_set( pl_w, 0 ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
	tele_out( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void tele_out( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 0 ;
		pl_w->timer0 = 16 ;
		pl_w->timer1 = 0 ;
	}
}

/*==============================================================*/
/*			The buttons held pick where to come back			*/
/*==============================================================*/
static void tele_wait( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static const Sint16 tele_pos_tbl[20] = {	/* x and y, by the buttons */
		0, 128, -128, 192, 128, 192, -256, 0, 256, 0,
		0, 128, -64, 128, 64, 128, -128, 0, 128, 0
	} ;

	if( pl_w->timer0 -- ) return ;
	/* the original's test: always true */
	if( !pl_w->cpu_flag && ( pl_w->pl_new_lvbt & KEY_PUNCH | 0x0060 ) ){
		if( pl_w->pl_new_lvbt & KEY_LP ) pl_w->timer1 = 1 ;
		if( pl_w->pl_new_lvbt & KEY_HP ) pl_w->timer1 = 2 ;
		if( pl_w->pl_new_lvbt & KEY_LK ) pl_w->timer1 = 3 ;
		if( pl_w->pl_new_lvbt & KEY_HK ) pl_w->timer1 = 4 ;
		if( pl_w->pl_new_lvbt & KEY_DOWN ) pl_w->timer1 += 5 ;
	}
	pl_w->routine_3 ++ ;
	pl_w->pos.x = tele_pos_tbl[pl_w->timer1 * 2] * DOT_X( 1.0f ) + pl_w->target_w->pos.x ;
	pl_w->pos.y = *( tele_pos_tbl + pl_w->timer1 * 2 + 1 ) * DOT_Y( 1.0f ) + pl_w->target_w->pos.y ;
	pl_dir_set( pl_w ) ;
	pl_w->disp_flag = 1 ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
}

/*--------------------------------------------------------------*/
static void tele_in( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->ground_y >= pl_w->pos.y ) pl_w->pl_stance = STANCE_STAND ;
		else {
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->jump_atk_cnt = JATK_ONE ;
		}
		sp_end( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void tele_land( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_dir_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_color_set( pl_w, 0 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			pl_w->jump_dir = -1 ;
			if( pl_w->sjump_flag ){
				pl_w->sjump_flag = 0 ;
				pl_w->jump_com_cnt = 0 ;
				pl_w->jump_dash_cnt = 0 ;
				pl_w->jump_atk_cnt = JATK_ONE ;
				pl_w->air_jump_cnt = 0 ;
			}
			pl_jump_ret( pl_w ) ;
			pl_w->acc.y = -DOT_Y( 1.5f ) ;
			return ;
		}
		if( pl_w->cg_type ) pl_w->cg_type = 0 ;
		if( pl_w->ground_y >= pl_w->pos.y ){
			pl_w->pos.y = pl_w->ground_y ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_land_ef_set( pl_w ) ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			The glow: red										*/
/*==============================================================*/
static void sp_power( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
		air_slow( pl_w ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			sp_end( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			ofs.x = -DOT_X( 4.0f ) ;
			ofs.y = DOT_Y( 95.0f ) ;
			pl_hyper_flash( pl_w, &ofs, 1 ) ;
			pl_color_set( pl_w, 0 ) ;
			lw->glow = 1 ;
			lw->glow_timer = 600 ;
			lw->col_timer = 0 ;
			lw->glow_col = 0 ;
			atck_plus_set( pl_w, lw->glow_timer, 40 ) ;
			Timer_Gauge_Set( pl_w, &lw->glow_timer, 0 ) ;
			pl_w->speed_up_flag = 0 ;
			pl_w->speed_up_timer = 0 ;
		}
		air_move( pl_w ) ;
	}
}

/*==============================================================*/
/*			The glow: blue										*/
/*==============================================================*/
static void sp_speed( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
		air_slow( pl_w ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			sp_end( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			ofs.x = -DOT_X( 13.0f ) ;
			ofs.y = DOT_Y( 83.0f ) ;
			pl_hyper_flash( pl_w, &ofs, 1 ) ;
			pl_color_set( pl_w, 0 ) ;
			lw->glow_timer = 600 ;
			lw->glow = 2 ;
			lw->col_timer = 0 ;
			lw->glow_col = 0 ;
			pl_w->speed_up_timer = lw->glow_timer ;
			Timer_Gauge_Set( pl_w, &lw->glow_timer, 2 ) ;
			pl_w->atck_plus_timer = 0 ;
			pl_w->atck_plus = 0 ;
		}
		air_move( pl_w ) ;
	}
}

static void grapple_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The air throw										*/
/*==============================================================*/
static void grapple_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
	air_slow( pl_w ) ;
	pl_w->timer0 = 90 ;
	grapple_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void grapple_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 9 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		if( ( em = pl_catch_ck( pl_w ) ) && em->pl_stance == STANCE_AIR ){
			pl_w->catch_no = 3 ;
			pl_w->catch_no |= CATCH_ESC_NG ;
			pl_w->catch_no |= CATCH_ESC2_NG ;
			pl_catch_set( pl_w, em ) ;
			pl_w->routine_3 = 0 ;
			pl_w->routine_2 = 0 ;
			return ;
		}
		if( ( em = pl_catch_ck2( pl_w ) ) && em->pl_stance == STANCE_AIR ){
			pl_w->catch_no = 3 ;
			pl_w->catch_no |= CATCH_ESC_NG ;
			pl_w->catch_no |= CATCH_ESC2_NG ;
			pl_catch_set( pl_w, em ) ;
			pl_w->routine_3 = 0 ;
			pl_w->routine_2 = 0 ;
			return ;
		}
	}
	air_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void grapple_end( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) sp_end( pl_w ) ;
}

/*==============================================================*/
static void sp_grapple( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *grapple_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		grapple_init, grapple_move, grapple_end,
	} ;

	grapple_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void metamor_maxspeed_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The speed up										*/
/*==============================================================*/
static void metamor_maxspeed_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl31_at_metamor ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	air_slow( pl_w ) ;
	metamor_maxspeed_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void metamor_maxspeed_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_3 ++ ;
		pls31_00_set2( pl_w, 2, 0 ) ;
		ofs.x = DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_hyper_flash( pl_w, &ofs, 3 ) ;
		pl_color_set( pl_w, 0 ) ;
		lw->maxspeed_flag = 1 ;
		lw->glow = 3 ;
		lw->glow_timer = 420 ;
		lw->col_timer = 0 ;
		lw->glow_col = 0 ;
		Timer_Gauge_Set( pl_w, &lw->glow_timer, 6 ) ;
		pl_w->speed_up_flag = 0 ;
		pl_w->speed_up_timer = 0 ;
		pl_w->atck_plus_timer = 0 ;
		pl_w->atck_plus = 0 ;
	}
	air_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void metamor_maxspeed_end( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) sp_end( pl_w ) ;
	else air_move( pl_w ) ;
}

/*==============================================================*/
static void metamor_maxspeed( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *metamor_maxspeed_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		metamor_maxspeed_init, metamor_maxspeed_move, metamor_maxspeed_end,
	} ;

	metamor_maxspeed_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

static void metamor_throw_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The hyper combo throw a sped-up punch starts		*/
/*==============================================================*/
static void metamor_throw_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->delay_flag = 0xff ;
	pl_w->delay_timer = 16 ;
	pl_w->hyper_bg = 1 ;
	vs_sys_w->hyper_bg_req = ( pl_w->pl_no << 4 ) | 1 ;
	pl_w->hyper_bg_tm = 16 ;
	pl_w->routine_3 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	switch( pl_w->pl_stance ){
	case STANCE_STAND:
		pl_w->char_no.b.no = 7 ;
		break ;
	case STANCE_CROUCH:
		pl_w->char_no.b.no = 8 ;
		break ;
	default:
		pl_w->char_no.b.no = 9 ;
		break ;
	}
	pl_char_set( pl_w, Char_Catch, pl_w->char_no.b.no ) ;
	speed_clear( pl_w ) ;
	nage_se_req( pl_w ) ;
	metamor_throw_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void metamor_throw_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->catch_flag = 0 ;
		lw->glow = 0 ;
		pl_w->atck_no = pl31_at_metamor_1 ;
		HIT_RESET( pl_w ) ;
		switch( pl_w->pl_stance ){
		case STANCE_STAND:
		case STANCE_CROUCH:
			pl_w->char_no.b.no = 7 ;
			break ;
		default:
			pl_w->char_no.b.no = 8 ;
			break ;
		}
		pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
		em = pl_w->catch_w ;
		em->catch_type = 6 ;
	}
}

/*--------------------------------------------------------------*/
static void metamor_throw_end( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_ng_timer = 2 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_color_set( pl_w, 0 ) ;
		sp_end( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		ef_color_set( pl_w, 0, pl_w->cg_prm, 2 ) ;
		switch( pl_w->cg_type ){
		case 1:
			pl_w->atck_no = pl31_at_metamor_1 ;
			HIT_RESET( pl_w ) ;
			break ;
		case 2:
			pl_w->atck_no = pl31_at_metamor_2 ;
			HIT_RESET( pl_w ) ;
			break ;
		case 3:
			pl_w->atck_no = pl31_at_metamor_3 ;
			HIT_RESET( pl_w ) ;
			lw->maxspeed_flag = 0 ;
			break ;
		}
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void metamor_throw( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *metamor_throw_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		metamor_throw_init, metamor_throw_move, metamor_throw_end,
	} ;

	metamor_throw_jmp[pl_w->routine_3]( pl_w, lw ) ;
}

/*==============================================================*/
static void sp_metamor( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *metamor_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		metamor_maxspeed, metamor_throw,
	} ;

	metamor_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
/*			The punches while sped up							*/
/*==============================================================*/
static void sp_stand_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->chain_tbl = sp_atck_tbl ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->chain_tbl = sp_atck_tbl ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		break ;
	}
	pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void sp_crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		pl_w->char_no.b.no = 3 ;
		pl_w->chain_tbl = sp_atck_tbl ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 4 ;
		pl_w->chain_tbl = sp_atck_tbl ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		break ;
	}
	pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void sp_jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		pl_w->char_no.b.no = 5 ;
		pl_w->chain_tbl = sp_atck_tbl ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		break ;
	case 2:
		pl_w->char_no.b.no = 6 ;
		pl_w->chain_tbl = sp_atck_tbl ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		break ;
	}
	pl_char_set( pl_w, Char_Super, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -- ;
}

static void sp_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*==============================================================*/
/*			A punch while sped up throws on its hit				*/
/*==============================================================*/
static void sp_punch_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			sp_catch_set( pl_w, em ) ;
			return ;
		}
		if( ( em = pl_catch_ck2( pl_w ) ) ) sp_catch_set( pl_w, em ) ;
	}
}

/*==============================================================*/
static void sp_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 1 ;
	pl_w->routine_3 = 0 ;
	pl_w->catch_no = 4 ;
	pl_w->catch_no |= CATCH_ESC_NG ;
	pl_w->catch_no |= CATCH_ESC2_NG ;
	pl_sp_catch_set( pl_w, em ) ;
	pl_w->pl_sp_type = pl31_sp_metamor ;
	act_st_set( pl_w, N_SUPER ) ;
	em->routine_2 = 0 ;
	em->spd.x = em->spd.y = em->acc.x = em->acc.y = 0.0f ;
}

static void snapback_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );
static void snapback_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
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
	pl_w->atck_no = pl31_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 31.0f ) ;
		ofs.y = 120.0f ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void swords_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The partner's swords: out and fired in one move		*/
/*==============================================================*/
static void swords_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	int i ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl31_at_tobashi ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
	for( i = 0 ; i < 6 ; i ++ ) lw->fire_flag[i] = 0 ;
	lw->fire_level = 0 ;
	swords_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void swords_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	int i ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, 11 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		for( i = 0 ; i < 6 ; i ++ ){
			if( !lw->sword_flag[i] ){
				lw->sword_flag[i] = 1 ;
				shl31_00_set( pl_w, 0, i ) ;
			}
		}
		pl_voice_req( pl_w, 34 ) ;
	}
}

/*--------------------------------------------------------------*/
static void swords_end( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		lw->fire_level = pl_w->atck_level + 1 ;
	}
}

/*==============================================================*/
static void sp_swords( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *swords_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		swords_init, swords_move, swords_end,
	} ;

	swords_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void stand_hp_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The partner's two hits								*/
/*==============================================================*/
static void stand_hp_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = ATCK_STAND_HP ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, 2 ) ;
	stand_hp_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stand_hp_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl31_at_stand_hp_2 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->cg_prm ){
		pl_w->cg_prm = 0 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		HIT_RESET( pl_w ) ;
	}
}

/*==============================================================*/
static void sp_stand_hp( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *stand_hp_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		stand_hp_init, stand_hp_move,
	} ;

	stand_hp_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void crouch_hp_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
static void crouch_hp_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = ATCK_CROUCH_HP ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, 2 ) ;
	crouch_hp_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void crouch_hp_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_crouch_hp( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *crouch_hp_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		crouch_hp_init, crouch_hp_move,
	} ;

	crouch_hp_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void hmetamor_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The gamma type's team hyper combo: the glow ends	*/
/*			in a dash											*/
/*==============================================================*/
static void hmetamor_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl31_at_hmetamor_1 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
	lw->maxspeed_flag = 0 ;
	hmetamor_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void hmetamor_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pls31_00_set2( pl_w, 2, 0 ) ;
		ofs.x = DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		lw->glow = 0 ;
		lw->glow_timer = 0 ;
		lw->col_timer = 0 ;
		lw->glow_col = 0 ;
		pl_w->speed_up_flag = 0 ;
		pl_w->speed_up_timer = 0 ;
		pl_w->atck_plus_timer = 0 ;
		pl_w->atck_plus = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void hmetamor_dash( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl31_at_hmetamor_1 ;
		HIT_RESET( pl_w ) ;
		pl_w->spd.x = -DOT_X( 4.0f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void hmetamor_hit( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_color_set( pl_w, 0 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		ef_color_set( pl_w, 0, pl_w->cg_prm, 2 ) ;
		switch( pl_w->cg_type ){
		case 1:
			pl_w->atck_no = pl31_at_hmetamor_1 ;
			HIT_RESET( pl_w ) ;
			break ;
		case 2:
			pl_w->atck_no = pl31_at_hmetamor_2 ;
			HIT_RESET( pl_w ) ;
			break ;
		case 3:
			pl_w->atck_no = pl31_at_hmetamor_3 ;
			HIT_RESET( pl_w ) ;
			break ;
		}
		pl_w->cg_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void hmetamor_end( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_hmetamor( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *hmetamor_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		hmetamor_init, hmetamor_move, hmetamor_dash, hmetamor_hit, hmetamor_end,
	} ;

	hmetamor_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void stampede_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw );

/*==============================================================*/
/*			The hyper combo, and the alpha and beta types'		*/
/*			team hyper: three volleys of swords					*/
/*==============================================================*/
static void stampede_init( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl31_at_stampede ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 11 ) ;
	stampede_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stampede_move( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_w->timer0 = 10 ;
		pl_w->timer1 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void stampede_end( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	int i ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->timer1 && !pl_w->timer0 -- ){
		pl_w->timer0 = 30 ;
		pl_w->timer1 -- ;
		for( i = 0 ; i < 6 ; i ++ ) shl31_01_set( pl_w, 0, i ) ;
		pl_voice_req( pl_w, 34 ) ;
	}
}

/*==============================================================*/
static void sp_stampede( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw )
{
	static void ( *stampede_jmp[])( PLAYER_WORK *pl_w, PL31_LOCAL_W *lw ) = {
		stampede_init, stampede_move, stampede_end,
	} ;

	stampede_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl31_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_end,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			The partner's kick: a leap through the air			*/
/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, 0 ) ;
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = 30.0f ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 2.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl31_at_t_kick ;
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
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_color_set( pl_w, 0 ) ;
		pl_stop_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( lw->maxspeed_flag != 0 ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *lw = ( PL31_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( lw->maxspeed_flag ) return 0 ;
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	if( pl_w->atck_pk == 1 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
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

/****************************************************************/
/*			Can a throw start? not while sped up				*/
/****************************************************************/
PLAYER_WORK *pl31_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, crouch_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 25.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	hold_renda_set( pl_w->catch_w, 16 ) ;
	catch_renda_set( pl_w, 1 ) ;
	pl_w->timer0 = 48 ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 25.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 5 ) ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 25.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/*==============================================================*/
/*			The air throw of sp_grapple								*/
/*==============================================================*/
static void grapple_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 25.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
}

/****************************************************************/
/*			Start the throw that pl31_catch_ck chose			*/
/****************************************************************/
void pl31_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, jump_p_set, jump_k_set, grapple_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	em->pos.y = em->ground_y ;
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	if( hold_renda_ck( pl_w->catch_w ) ) pl_w->timer0 = 0 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 26.0f ) ;
		ofs.y = DOT_Y( 13.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		hit_se_req( em, 2 ) ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl31_at_throw_p ;
		pl_w->atck_no = pl31_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		catch_damage( pl_w ) ;
		pl_w->hit_stop = 8 ;
	}
	if( !pl_w->timer0 -- ){
		em->dm_hit_w = pl_w ;
		em->atck_no = pl31_at_throw_p ;
		pl_w->atck_no = pl31_at_throw_p ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_move,
	} ;

	nml_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_p_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->timer0 = 20 ;
	}
}

/*==============================================================*/
/*			Up, down or neither picks the throw					*/
/*==============================================================*/
static void jump_p_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		if( pl_w->pl_sw_new & KEY_UP ) pl_w->char_no.b.no = 2 ;
		else if( pl_w->pl_sw_new & KEY_DOWN ) pl_w->char_no.b.no = 3 ;
		else pl_w->char_no.b.no = 1 ;
		pl_char_set( pl_w, Char_Catch, pl_w->char_no.b.no ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	Uint8 no ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em->dm_hit_w = pl_w ;
		if( pl_w->char_no.b.no == 2 ) no = 34 ;
		else if( pl_w->char_no.b.no == 3 ) no = 35 ;
		else no = 33 ;
		em->atck_no = no ;
		pl_w->atck_no = no ;
		em->catch_type = 1 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
	}
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_p_init, jump_p_move, jump_p_end,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_k_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*==============================================================*/
/*			Falls until the ground, and slams the opponent		*/
/*			there												*/
/*==============================================================*/
static void jump_k_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y + -120.0f ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		em->dm_hit_w = pl_w ;
		em->atck_no = pl31_at_air_throw_k ;
		pl_w->atck_no = pl31_at_air_throw_k ;
		em->catch_type = 2 ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		pl_w->routine_2 ++ ;
		pl_char_move( pl_w ) ;
		catch_se_req( em ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_k_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_k_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_k_init, jump_k_move, jump_k_end,
	} ;

	jump_k_jmp[pl_w->routine_2]( pl_w ) ;
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl31_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, jump_p_catch, jump_k_catch, jump_p_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_hold( PLAYER_WORK *pl_w );
static void jump_hold( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The opponent's side of each throw					*/
/****************************************************************/
void pl31_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_hold, jump_hold, jump_hold, jump_hold, jump_hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	catch_em_pos_set1( em, pl_w ) ;
}

/*==============================================================*/
static void jump_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
void pl31_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl31_sp_stampede ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl31_sp_stampede ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl31_sp_hmetamor ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl31_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl31_sp_stampede ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl31_sp_stampede ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl31_sp_hmetamor ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl31_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl31_sp_swords ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl31_sp_swords ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl31_sp_stand_hp ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl31_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl31_sp_swords ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl31_sp_swords ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl31_sp_crouch_hp ;
		pl_w->atck_level = 0 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl31.c" End of File	*/
