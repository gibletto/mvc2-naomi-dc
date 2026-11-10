/* Gambit (character 28): command checks, normal attacks, appearance, win poses, special moves and
   hyper combos, throws and the partner moves. */
#define PL_GAUGE_ADD_OLD
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl28_def.h"

/* the character's own work, in tlocal */
typedef struct PL28_LOCAL_W {
	Uint8	card_ng_flag;			/* no card moves while set; only ever cleared */
	Uint8	unk_01;
	Uint16	unk_02;
	Uint8	unk_04[4];
	Uint8	ef_ng_flag;			/* the hyper combo's effect did not start */
} PL28_LOCAL_W;

#define PL28_LW( pl )	( ( PL28_LOCAL_W * )( pl )->tlocal )

typedef struct EF28_LOCAL_W {
	Uint8	unk_00[0x0d];
	Uint8	pose;
	Uint8	unk_0e[2];
	Sint16	part_kind;
} EF28_LOCAL_W;

#define EF28_LW( ew )	( ( EF28_LOCAL_W * )&( ew )->obj_mtx )	/* an effect keeps its own work at obj_mtx */

extern PLAYER_WORK *shl28_00_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *shl28_01_set( PLAYER_WORK *pl_w, Uint8 type );

void pl28_main( PLAYER_WORK *pl_w );
void pl28_com_ck( PLAYER_WORK *pl_w );
void pl28_atck_set( PLAYER_WORK *pl_w );
void pl28_atck( PLAYER_WORK *pl_w );
void pl28_dash( PLAYER_WORK *pl_w );
void pl28_noki( PLAYER_WORK *pl_w );
void pl28_appear( PLAYER_WORK *pl_w );
void pl28_win( PLAYER_WORK *pl_w );
void pl28_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl28_catch_ck( PLAYER_WORK *pl_w );
void pl28_catch_set( PLAYER_WORK *pl_w );
void pl28_catch( PLAYER_WORK *pl_w );
void pl28_hold( PLAYER_WORK *pl_w );
int pl28_delay_ck( PLAYER_WORK *pl_w );
void pl28_t_kick( PLAYER_WORK *pl_w );
void pl28_upper( PLAYER_WORK *pl_w );
void pl28_sp_unison( PLAYER_WORK *pl_w );
void pl28_sp_gattai( PLAYER_WORK *pl_w );
void pl28_sp_assist( PLAYER_WORK *pl_w );
void pl28_sp_zero( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_card[] = { 3, COM_GROUND, pl28_sp_card, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Kinetic Card */
static Uint16 com_jcard[] = { 3, COM_AIR, pl28_sp_jcard, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Kinetic Card */
static Uint16 com_trick[] = { 3, COM_GROUND, pl28_sp_trick, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Trick Card */
static Uint16 com_slash[] = { 3, COM_GROUND, pl28_sp_slash, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Cajun Slash */
static Uint16 com_slash2[] = { 3, COM_GROUND | COM_ALL_SW, pl28_sp_slash2, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_HP | KEY_LK } ;	/* Cajun Slash, two buttons */
static Uint16 com_strike[] = { 40, COM_GROUND, pl28_sp_strike, 0, KEY_DOWN, KEY_UP, KEY_KICK } ;	/* Cajun Strike */
static Uint16 com_escape[] = { 40, COM_GROUND, pl28_sp_escape, 0, KEY_DOWN, KEY_UP, KEY_PUNCH } ;	/* Cajun Escape */
static Uint16 com_royal[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl28_sp_royal, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Royal Flush */
static Uint16 com_explosion[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl28_sp_explosion, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Cajun Explosion */
static Uint16 com_explosion_f[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl28_sp_explosion_f, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Cajun Explosion, jumping forward */

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

static void ( *pl28_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl28_main,
	pl28_com_ck,
	pl28_atck_set,
	pl28_atck,
	pl28_dash,
	pl28_noki,
	pl28_appear,
	pl28_win,
	pl28_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl28_catch_ck,	/* returns a value */
	pl28_catch_set,
	pl28_catch,
	pl28_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl28_delay_ck,
	pl28_t_kick,
	pl28_upper,
	pl28_sp_unison,
	pl28_sp_gattai,
	pl28_sp_assist,
	pl28_sp_zero,
} ;

static Uint8 explosion_ck( PLAYER_WORK *pl_w );
static Uint8 explosion_f_ck( PLAYER_WORK *pl_w );
static Uint8 slash_ck( PLAYER_WORK *pl_w );
static Uint8 slash2_ck( PLAYER_WORK *pl_w );
static Uint8 trick_ck( PLAYER_WORK *pl_w );
static Uint8 royal_ck( PLAYER_WORK *pl_w );
static Uint8 sp_card_ck( PLAYER_WORK *pl_w );
static Uint8 jcard_ck( PLAYER_WORK *pl_w );
static Uint8 strike_ck( PLAYER_WORK *pl_w );
static Uint8 escape_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 explosion_delay_ck( PLAYER_WORK *pl_w );
static Uint8 explosion_f_delay_ck( PLAYER_WORK *pl_w );
static Uint8 royal_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl28_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl28_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl28_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( explosion_ck( pl_w ) ) return ;
	if( explosion_f_ck( pl_w ) ) return ;
	if( slash2_ck( pl_w ) ) return ;
	if( slash_ck( pl_w ) ) return ;
	if( trick_ck( pl_w ) ) return ;
	if( royal_ck( pl_w ) ) return ;
	if( sp_card_ck( pl_w ) ) return ;
	if( jcard_ck( pl_w ) ) return ;
	if( strike_ck( pl_w ) ) return ;
	if( escape_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( pl_chouhatsu_ck( pl_w, 6 ) ){
		pl_w->pl_sp_type = pl28_sp_chouhatsu ;
		pl_w->routine_1 = PL_NORM ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		act_st_set( pl_w, N_SPECIAL ) ;
		return ;
	}
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 explosion_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_explosion, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl28_sp_explosion ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 explosion_f_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_explosion_f, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl28_sp_explosion_f ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 slash_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_slash, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->pl_sp_type = pl28_sp_slash ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 slash2_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_slash2, &pl_w->com_w9 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w9 ) ;
	pl_w->pl_sp_type = pl28_sp_slash2 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Not while card_ng_flag is set						*/
/*==============================================================*/
static Uint8 trick_ck( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_trick, &pl_w->com_w0 ) ) return 0 ;
	lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
	if( lw->card_ng_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->pl_sp_type = pl28_sp_trick ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 royal_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_royal, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->pl_sp_type = pl28_sp_royal ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 sp_card_ck( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_card, &pl_w->com_w ) ) return 0 ;
	lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
	if( lw->card_ng_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->pl_sp_type = pl28_sp_card ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air, once a jump unless super jumping		*/
/*==============================================================*/
static Uint8 jcard_ck( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_jcard, &pl_w->com_w6 ) ) return 0 ;
	lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
	if( lw->card_ng_flag ) return 0 ;
	if( !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->pl_sp_type = pl28_sp_jcard ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 strike_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_strike, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->atck_no == ATCK_CROUCH_HP ) com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl28_sp_strike ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 escape_ck( PLAYER_WORK *pl_w )
{
	if( !com_tame_ck( pl_w, com_escape, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->pl_sp_type = pl28_sp_escape ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl28_delay_ck( PLAYER_WORK *pl_w )
{
	if( explosion_delay_ck( pl_w ) || explosion_f_delay_ck( pl_w ) || royal_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static Uint8 explosion_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_explosion, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl28_sp_explosion ;
	return 1 ;
}

/*==============================================================*/
static Uint8 explosion_f_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_explosion_f, &pl_w->com_w8 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl28_sp_explosion_f ;
	return 1 ;
}

/*==============================================================*/
static Uint8 royal_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_royal, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl28_sp_royal ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl28_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
void pl28_main( PLAYER_WORK *pl_w )
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
void pl28_atck_set( PLAYER_WORK *pl_w )
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
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		pl_se_req( pl_w, SE_SWING_H ) ;
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
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
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
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl28_atck( PLAYER_WORK *pl_w )
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
/*			The medium punch hits again							*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_w->nml_no == 1 ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->atck_no = pl28_at_stand_mp_2 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_type = 0 ;
		}
	} else {
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	}
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
void pl28_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->atck_no = pl28_at_upper ;
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

static void dash_init( PLAYER_WORK *pl_w );
static void dash_jump( PLAYER_WORK *pl_w );
static void dash_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash, a hop forward								*/
/****************************************************************/
void pl28_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_jump, dash_land,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_w->spd.x = -DOT_X( 9.5f ) ;
		pl_w->acc.x = 0.3125f ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void dash_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
		pl_se_req( pl_w, 52 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop back							*/
/****************************************************************/
void pl28_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_w->spd.x = DOT_X( 9.5f ) ;
		pl_w->acc.x = -0.3125f ;
		if( pl_w->pl_dir ){
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
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		pl_se_req( pl_w, 52 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
void pl28_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl28_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
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
		pl_char_set( pl_w, Char_Extra, 3 ) ;
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

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_jump( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl28_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_wait, appear_jump, appear_end,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			He jumps in from off the screen						*/
/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	Float ofs ;

	pl_w->disp_flag = 0 ;
	if( vs_sys_work.vs_st < VS_APPEAR ) return ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 30 ;
	pl_w->timer1 = 24 ;
	pl_w->spd.y = DOT_Y( 9.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.x = 5.0f ;
	ofs = -245.0f ;
	if( !pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		ofs = 245.0f ;
	}
	pl_w->spd.z = pl_w->pos.x ;
	pl_w->pos.x += ofs ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
	shl28_00_set( pl_w, 3 ) ;
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->disp_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->routine_3 ){
		if( -- pl_w->timer1 <= 0 ){
			pl_w->routine_3 ++ ;
			pl_char_set( pl_w, Char_Appear, 1 ) ;
		}
	} else if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pos.x = pl_w->spd.z ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Appear, 3 ) ;
		pl_se_req( pl_w, 52 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void lose_init( PLAYER_WORK *pl_w );
static void win_time_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_move_e( PLAYER_WORK *pl_w );

/****************************************************************/
void pl28_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static void ( *win_init_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, lose_init, win_time_init, lose_init, lose_init,
	} ;

	pl_w->routine_2 ++ ;
	win_init_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			Won by KO: one of two poses							*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	Sint32 rnd ;

	pl_w->routine_3 ++ ;
	rnd = nlRand() ;
	if( rnd & 1 ) pl_char_set( pl_w, Char_Win, 0 ) ;
	else pl_char_set( pl_w, Char_Win, 1 ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 3 ) ;
}

/*==============================================================*/
static void win_time_init( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Win, 2 ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_move, win_move_e, win_move_e, win_move_e, win_move_e,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			The animation picks the palette						*/
/*==============================================================*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_3 && pl_w->cg_flag1 ){
		ef_color_set( pl_w, 2, pl_w->cg_flag1 + pl_w->pl_color * 5 - 1, 1 ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void sp_t_kick( PLAYER_WORK *pl_w );
static void sp_card( PLAYER_WORK *pl_w );
static void sp_trick( PLAYER_WORK *pl_w );
static void sp_slash( PLAYER_WORK *pl_w );
static void sp_strike( PLAYER_WORK *pl_w );
static void sp_royal( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_escape( PLAYER_WORK *pl_w );
static void sp_explosion( PLAYER_WORK *pl_w );
static void sp_crouch_hp( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl28_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_t_kick, sp_card, sp_trick, sp_slash, sp_strike, sp_royal, sp_chouhatsu, sp_escape,
		sp_slash, sp_card, sp_explosion, sp_escape, sp_escape, sp_escape, sp_explosion, sp_crouch_hp,
		sp_snapback,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			Stop and stand on the ground						*/
/*==============================================================*/
static void sp_ground( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
}

static void sp_t_kick_init( PLAYER_WORK *pl_w );
static void sp_t_kick_jump( PLAYER_WORK *pl_w );
static void sp_t_kick_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *sp_t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_t_kick_init, sp_t_kick_jump, sp_t_kick_land,
	} ;

	sp_t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void sp_t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = pl_w->pl_dir ? 30.0f : -30.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl28_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
}

/*--------------------------------------------------------------*/
static void sp_t_kick_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

static void card_init( PLAYER_WORK *pl_w );
static void card_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A card thrown, on the ground or in the air			*/
/*==============================================================*/
static void sp_card( PLAYER_WORK *pl_w )
{
	static void ( *card_jmp[])( PLAYER_WORK *pl_w ) = {
		card_init, card_move,
	} ;

	if( pl_w->pl_stance == STANCE_AIR ){
		pl_jump_move( pl_w ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
	card_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void card_init( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw ;
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	pl_w->routine_2 ++ ;
	if( pl_w->pl_sp_type != pl28_sp_jcard ) sp_ground( pl_w ) ;
	lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
	lw->card_ng_flag = 0 ;
	pl_w->timer0 = 0 ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl28_at_card_vc ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl28_at_card ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->spd.x /= DOT_X( 16.0f ) ;
		pl_w->spd.y /= DOT_Y( 8.0f ) ;
		pl_w->acc.y /= DOT_Y( 64.0f ) ;
		pl_w->acc.x = 0.0f ;
		if( !pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 3 ) ;
		else pl_char_set( pl_w, Char_Special, 5 ) ;
	} else {
		if( !pl_w->atck_level ) pl_char_set( pl_w, Char_Special, 0 ) ;
		else pl_char_set( pl_w, Char_Special, 2 ) ;
	}
	ew = shl28_00_set( pl_w, 2 ) ;
	if( ew ){
		el = EF28_LW( ew ) ;
		if( pl_w->pl_stance == STANCE_AIR ){
			el->pose = 1 ;
			pl_gauge_add( pl_w, 5 ) ;
		} else {
			el->pose = 0 ;
			pl_gauge_add( pl_w, 5 ) ;
			pl_sp_ef_set( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void card_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_jump_ret( pl_w ) ;
		} else pl_stand_ret( pl_w ) ;
	} else if( pl_w->cg_type ){
		ew = shl28_00_set( pl_w, 4 ) ;
		if( ew ){
			el = EF28_LW( ew ) ;
			el->part_kind = pl_w->pl_stance == STANCE_AIR ;
			if( pl_w->pl_stance == STANCE_AIR ){ }	/* empty in the arcade build */
			else shl28_00_set( pl_w, 10 ) ;
		}
		pl_w->cg_type = 0 ;
	}
}

static void trick_init( PLAYER_WORK *pl_w );
static void trick_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_trick( PLAYER_WORK *pl_w )
{
	static void ( *trick_jmp[])( PLAYER_WORK *pl_w ) = {
		trick_init, trick_move,
	} ;

	trick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void trick_init( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	lw->card_ng_flag = 0 ;
	pl_w->timer0 = 0 ;
	pl_w->atck_no = pl28_at_card ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, !pl_w->atck_level ? 6 : 8 ) ;
	ew = shl28_00_set( pl_w, 2 ) ;
	if( ew ){
		el = EF28_LW( ew ) ;
		el->pose = 2 ;
	}
	pl_gauge_add( pl_w, 5 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void trick_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ew = shl28_00_set( pl_w, 4 ) ;
		if( ew ){
			el = EF28_LW( ew ) ;
			el->part_kind = 2 ;
		}
	}
}

static void slash_init( PLAYER_WORK *pl_w );
static void slash_start( PLAYER_WORK *pl_w );
static void slash_jump( PLAYER_WORK *pl_w );
static void slash_slide( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_slash( PLAYER_WORK *pl_w )
{
	static void ( *slash_jmp[])( PLAYER_WORK *pl_w ) = {
		slash_init, slash_start, slash_jump, slash_slide,
	} ;

	slash_jmp[pl_w->routine_2]( pl_w ) ;
}

/* the attack for each strength */
static const Uint8 slash_atck[3] = { 53, 54, 58 } ;

/*==============================================================*/
static void slash_init( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw = ( PL28_LOCAL_W * )pl_w->tlocal ;

	lw->unk_02 = 0 ;
	pl_voice_req( pl_w, 22 ) ;
	sp_ground( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->pl_sp_type == pl28_sp_slash2 ){
		pl_w->atck_no = slash_atck[2] ;
		HIT_RESET( pl_w ) ;
		pl_w->routine_2 += 3 ;
		pl_char_set( pl_w, Char_Special, 14 ) ;
	} else {
		if( pl_w->team_type == 3 ){
			pl_w->atck_no = pl28_at_slash_vc ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = slash_atck[pl_w->atck_level] ;
			HIT_RESET( pl_w ) ;
		}
		if( !pl_w->atck_level ){
			pl_w->routine_2 += 3 ;
			pl_char_set( pl_w, Char_Special, 12 ) ;
		} else {
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 10 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void slash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 5.0f : -5.0f ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -0.9375f ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void slash_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 13 ) ;
	}
}

static void slash_slide_nop( PLAYER_WORK *pl_w );
static void slash_slide_spd( PLAYER_WORK *pl_w );
static void slash_slide_acc( PLAYER_WORK *pl_w );
static void slash_slide_stop( PLAYER_WORK *pl_w );
static void slash_slide_ck( PLAYER_WORK *pl_w );

/* the attack for each step of the animation */
static const Uint8 slash_slide_atck[7] = { 53, 53, 55, 56, 58, 59, 60 } ;

/*--------------------------------------------------------------*/
static void slash_slide( PLAYER_WORK *pl_w )
{
	static void ( *slash_slide_jmp[])( PLAYER_WORK *pl_w ) = {
		slash_slide_nop, slash_slide_spd, slash_slide_acc, slash_slide_stop,
	} ;

	slash_slide_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	} else {
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = slash_slide_atck[pl_w->cg_flag1] ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		slash_slide_jmp[pl_w->cg_type]( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
		}
	}
}

/*==============================================================*/
static void slash_slide_nop( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void slash_slide_spd( PLAYER_WORK *pl_w )
{
	pl_w->cg_type = 0 ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	pl_w->acc.x = 0.0f ;
}

/*==============================================================*/
static void slash_slide_acc( PLAYER_WORK *pl_w )
{
	pl_w->cg_type = 0 ;
	pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
}

/*==============================================================*/
static void slash_slide_stop( PLAYER_WORK *pl_w )
{
	pl_w->cg_type = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
}

/*==============================================================*/
static void slash_slide_ck( PLAYER_WORK *pl_w )
{
}

static void strike_jump_set( PLAYER_WORK *pl_w, Uint16 no );
static void strike_init( PLAYER_WORK *pl_w );
static void strike_start( PLAYER_WORK *pl_w );
static void strike_jump( PLAYER_WORK *pl_w );
static void strike_wall( PLAYER_WORK *pl_w );
static void strike_fall( PLAYER_WORK *pl_w );
static void strike_end( PLAYER_WORK *pl_w );
static void strike_land_set( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A jump to the edge of the screen, then off it		*/
/*==============================================================*/
static void sp_strike( PLAYER_WORK *pl_w )
{
	static void ( *strike_jmp[])( PLAYER_WORK *pl_w ) = {
		strike_init, strike_start, strike_jump, strike_wall, strike_fall, strike_end,
	} ;

	strike_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void strike_jump_set( PLAYER_WORK *pl_w, Uint16 no )
{
	Uint16 sw ;
	Float scr_x ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, no ) ;
	sp_ground( pl_w ) ;
	pl_w->timer0 = 1 ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->cpu_flag ) sw = pl_w->atck_lever ;
	else sw = pl_w->pl_sw_new ;
	if( sw & KEY_BACK ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
	}
	pl_w->spd.y = DOT_Y( 8.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->pl_dir ) scr_x = scr_work.eye[0].x + 640.0f ;
	else scr_x = scr_work.eye[0].x - 746.6667f ;
	pl_w->spd.x = ( scr_x - pl_w->pos.x ) / 20.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->timer1 = 20 ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
}

/*==============================================================*/
static void strike_init( PLAYER_WORK *pl_w )
{
	pl_w->atck_no = pl28_at_strike ;
	HIT_RESET( pl_w ) ;
	strike_jump_set( pl_w, 0 ) ;
}

/*--------------------------------------------------------------*/
static void strike_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 = 5 ;
		pl_w->pl_stance = STANCE_STAND ;
		strike_land_set( pl_w ) ;
		return ;
	}
	pl_w->timer1 -- ;
	if( pl_w->timer1 < 0 && ( pl_w->pl_dir ? pl_w->wall_flag & WALL_RIGHT : pl_w->wall_flag & WALL_LEFT ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 17 ) ;
		scr_excep_set( pl_w, 1, 13 ) ;
		pl_se_req( pl_w, 52 ) ;
	}
}

/* the throw direction after the jump off the edge: two strengths for each side */
static const Uint8 strike_dir[4] = { 18, 20, 14, 12 } ;

/*==============================================================*/
/*			Off the edge: another jump while bounces are		*/
/*			left and the attack is held, else the throw			*/
/*==============================================================*/
static void strike_wall( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		scr_excep_x_clr() ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		if( pl_w->timer0 ){
			if( pl_w->cpu_flag ){
				if( pl_w->atck_pk ) goto fire ;
				else goto wall ;
			}
			if( !( pl_w->pl_sw_new & KEY_BACK ) ) goto wall ;
		fire :
			pl_w->timer0 -- ;
			pl_w->routine_2 = 2 ;
			pl_w->spd.y = DOT_Y( 10.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
			pl_w->spd.x = pl_w->pl_dir ? 31.601562f : -31.601562f ;
			pl_w->acc.x = 0.0f ;
			pl_char_set( pl_w, Char_Special, 18 ) ;
		} else {
		wall :
			pl_w->timer0 = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			if( pl_w->disp_dir ) pl_w->dir = strike_dir[pl_w->atck_level + 2] ;
			else pl_w->dir = strike_dir[pl_w->atck_level] ;
			pl_char_set( pl_w, Char_Special, 19 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void strike_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	dir_move( pl_w, 2000 ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		strike_land_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void strike_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void strike_land_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Jump, 4 ) ;
	pl_land_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_se_req( pl_w, 52 ) ;
}

static void royal_init( PLAYER_WORK *pl_w );
static void royal_wait( PLAYER_WORK *pl_w );
static void royal_move( PLAYER_WORK *pl_w );
static void royal_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a stream of cards					*/
/*==============================================================*/
static void sp_royal( PLAYER_WORK *pl_w )
{
	static void ( *royal_jmp[])( PLAYER_WORK *pl_w ) = {
		royal_init, royal_wait, royal_move, royal_end,
	} ;

	royal_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void royal_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	sp_ground( pl_w ) ;
	pl_w->atck_no = pl28_at_royal ;
	HIT_RESET( pl_w ) ;
	pl_w->timer0 = 0 ;
	pl_w->dir = 0 ;
	pl_w->sub_type = 0 ;
	pl_w->atck_level = 1 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void royal_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	PL28_LOCAL_W *lw ;
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( ( Sint8 )pl_w->cg_flag1 > 0 ){
		pl_w->cg_flag1 = 0 ;
		ew = shl28_00_set( pl_w, 9 ) ;
		lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
		if( !ew ) lw->ef_ng_flag = 1 ;
		else lw->ef_ng_flag = 0 ;
	}
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ew = shl28_00_set( pl_w, 2 ) ;
		if( ew ){
			el = EF28_LW( ew ) ;
			el->pose = 4 ;
			pl_w->timer1 = 3 ;
		}
		ofs.x = DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 32.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void royal_move( PLAYER_WORK *pl_w )
{
	PL28_LOCAL_W *lw ;
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->timer1 -- ;
		if( pl_w->timer1 <= 0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Super, 1 ) ;
			lw = ( PL28_LOCAL_W * )pl_w->tlocal ;
			if( lw->ef_ng_flag ) pl_w->cg_ctr = 8 ;
			return ;
		}
	}
	pl_w->timer0 -- ;
	if( pl_w->timer0 < 0 ){
		pl_w->timer0 = 1 ;
		if( !pl_w->routine_3 ){
			pl_w->routine_3 ++ ;
			ew = shl28_00_set( pl_w, 0 ) ;
			if( ew ){
				el = EF28_LW( ew ) ;
				el->pose = 0 ;
			}
			ew = shl28_00_set( pl_w, 0 ) ;
			if( ew ){
				el = EF28_LW( ew ) ;
				el->pose = 1 ;
			}
		}
		ew = shl28_00_set( pl_w, 8 ) ;
		if( ew ){
			ew->dir = pl_w->dir ;
			pl_w->dir ++ ;
			pl_w->dir &= 7 ;
			ew->sub_type = pl_w->sub_type ;
			pl_w->sub_type ++ ;
			pl_w->sub_type &= 7 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void royal_end( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 ){
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	} else {
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 4 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
}

static void escape_init( PLAYER_WORK *pl_w );
static void escape_jump( PLAYER_WORK *pl_w );
static void escape_wall( PLAYER_WORK *pl_w );
static void escape_dive( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The same jump to the edge of the screen, then a		*/
/*			dive at the enemy									*/
/*==============================================================*/
static void sp_escape( PLAYER_WORK *pl_w )
{
	static void ( *escape_jmp[])( PLAYER_WORK *pl_w ) = {
		escape_init, strike_start, escape_jump, escape_wall, escape_dive, strike_end,
	} ;

	escape_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void escape_init( PLAYER_WORK *pl_w )
{
	pl_w->atck_no = pl28_at_escape ;
	HIT_RESET( pl_w ) ;
	strike_jump_set( pl_w, 0 ) ;
}

/*--------------------------------------------------------------*/
static void escape_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 = 5 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 4 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_se_req( pl_w, 52 ) ;
	} else {
		pl_w->timer1 -- ;
		if( pl_w->timer1 < 0 ){
			if( pl_w->wall_flag ){
				if( pl_w->pl_dir ? pl_w->wall_flag & WALL_RIGHT : pl_w->wall_flag & WALL_LEFT ){
					pl_w->routine_2 ++ ;
					pl_w->spd.x = 0.0f ;
					pl_w->spd.y = 0.0f ;
					pl_w->acc.x = 0.0f ;
					pl_w->acc.y = 0.0f ;
					pl_char_set( pl_w, Char_Special, 17 ) ;
					pl_se_req( pl_w, 52 ) ;
					scr_excep_set( pl_w, 1, 13 ) ;
				}
			} else scr_excep_set( pl_w, 1, 13 ) ;
		}
	}
}

/*==============================================================*/
/*			Off the wall: another jump while bounces are		*/
/*			left and the attack is held, else a dive at the		*/
/*			target												*/
/*==============================================================*/
static void escape_wall( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		scr_excep_x_clr() ;
		pl_char_set( pl_w, Char_Special, 18 ) ;
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		if( pl_w->timer0 ){
			if( pl_w->cpu_flag ){
				if( pl_w->atck_pk ) goto fire ;
				else goto slow ;
			}
			if( !( pl_w->pl_sw_new & KEY_BACK ) ) goto slow ;
		fire :
			pl_w->timer0 -- ;
			pl_w->routine_2 = 2 ;
			pl_w->spd.y = DOT_Y( 10.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.25f ) ;
			pl_w->spd.x = pl_w->pl_dir ? 31.601562f : -31.601562f ;
			pl_w->acc.x = 0.0f ;
			return ;
		} else {
		slow :
			spd = pl_w->target_w->pos.x - pl_w->pos.x ;
			spd /= DOT_X( 16.0f ) ;
			pl_w->spd.x = spd ;
			pl_w->acc.x = -( spd / DOT_X( 32.0f ) ) ;
			pl_w->spd.y = DOT_Y( 8.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.5f ) ;
			pl_w->timer0 = 0 ;
		}
	}
}

/*==============================================================*/
/*			The dive: steered left and right, one attack on		*/
/*			the way down, then the landing						*/
/*==============================================================*/
static void escape_dive( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ){
		if( pl_w->pl_new_lvbt & KEY_BACK ) pl_w->pos.x += DOT_X( 4.0f ) ;
		else pl_w->pos.x += -DOT_X( 4.0f ) ;
	}
	if( pl_w->timer0 ) goto end ;
	if( !( pl_w->cpu_flag ? ( pl_w->target_w->pl_stance == STANCE_AIR || !( pl_w->spd.y < 0.0f ) )
			&& em_atck_hit_ck( pl_w, pl_w->target_w, 61 ) : pl_w->pl_now_lvbt & KEY_PUNCH ) ) goto end ;
	pl_w->timer0 = 1 ;
	pl_char_set( pl_w, Char_Special, 16 ) ;
end :
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 4 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_se_req( pl_w, 52 ) ;
	}
}

static void explosion_init( PLAYER_WORK *pl_w );
static void explosion_wait( PLAYER_WORK *pl_w );
static void explosion_jump( PLAYER_WORK *pl_w );
static void explosion_hit( PLAYER_WORK *pl_w );
static void explosion_fall( PLAYER_WORK *pl_w );
static void explosion_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a jump off the edge of the screen	*/
/*			and a dive back										*/
/*==============================================================*/
static void sp_explosion( PLAYER_WORK *pl_w )
{
	static void ( *explosion_jmp[])( PLAYER_WORK *pl_w ) = {
		explosion_init, explosion_wait, explosion_jump, explosion_hit, explosion_fall, explosion_end,
	} ;

	explosion_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void explosion_init( PLAYER_WORK *pl_w )
{
	Float scr_x ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_sp_ef_set( pl_w ) ;
	scr_excep_set( pl_w, 1, 13 ) ;
	sp_ground( pl_w ) ;
	pl_w->atck_no = pl28_at_explosion ;
	HIT_RESET( pl_w ) ;
	if( pl_w->pl_sp_type != pl28_sp_explosion_f ) pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->spd.y = DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	if( pl_w->pl_dir ) scr_x = scr_work.eye[0].x + 746.6667f ;
	else scr_x = scr_work.eye[0].x - 746.6667f ;
	pl_w->spd.x = ( scr_x - pl_w->pos.x ) / DOT_X( 32.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->timer0 = 33 ;
	pl_char_set( pl_w, Char_Super, pl_w->pl_sp_type == pl28_sp_explosion_f ? 6 : 15 ) ;
}

/*--------------------------------------------------------------*/
static void explosion_wait( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->push_ng_timer = 35 ;
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void explosion_jump( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 = 5 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Jump, 4 ) ;
	} else {
		pl_w->timer0 -- ;
		if( pl_w->timer0 < 0 && pl_w->wall_flag && ( pl_w->pl_dir ? pl_w->wall_flag & WALL_RIGHT : pl_w->wall_flag & WALL_LEFT ) ){
			pl_w->routine_2 ++ ;
			pl_w->pl_dir ^= 1 ;
			pl_w->disp_dir = pl_w->pl_dir ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			if( pl_w->pl_sp_type == pl28_sp_explosion_f ) pl_char_set( pl_w, Char_Super, 14 ) ;
			else pl_char_set( pl_w, Char_Super, 7 ) ;
			ew = shl28_00_set( pl_w, 2 ) ;
			if( ew ){
				el = EF28_LW( ew ) ;
				el->pose = 1 ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void explosion_hit( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->cg_type ^= 1 ;
		ofs.x = -DOT_X( 28.0f ) ;
		ofs.y = DOT_Y( 96.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void explosion_fall( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->jump_com_cnt ++ ;
		pl_jump_ret( pl_w ) ;
	} else if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		ew = shl28_01_set( pl_w, 3 ) ;
		if( ew ) ew->sub_type = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void explosion_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void crouch_hp_init( PLAYER_WORK *pl_w );
static void crouch_hp_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_crouch_hp( PLAYER_WORK *pl_w )
{
	static void ( *crouch_hp_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_hp_init, crouch_hp_move,
	} ;

	crouch_hp_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crouch_hp_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
	if( pl_w->team_type == 3 ){
		pl_w->atck_no = pl28_at_crouch_hp_vc ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = ATCK_CROUCH_HP ;
		HIT_RESET( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hp_move( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl28_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
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
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl28_catch_ck( PLAYER_WORK *pl_w )
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

static void catch_dir_set( PLAYER_WORK *pl_w );
static void nml_p_set( PLAYER_WORK *pl_w );
static void nml_k_set( PLAYER_WORK *pl_w );
static void jump_k_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl28_catch_ck chose			*/
/****************************************************************/
void pl28_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_set, nml_k_set, jump_k_set,
	} ;

	catch_gauge_add( pl_w ) ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			The one being thrown faces Gambit					*/
/*==============================================================*/
static void catch_dir_set( PLAYER_WORK *pl_w )
{
	pl_w->catch_w->disp_dir = pl_w->disp_dir ^ 1 ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	PLAYER_WORK *ew ;
	EF28_LOCAL_W *el ;

	catch_dir_set( pl_w ) ;
	if( pl_w->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		pl_w->catch_w->disp_dir ^= 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir ;
	}
	ofs.x = -110.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	ew = shl28_00_set( pl_w, 2 ) ;
	if( ew ){
		el = EF28_LW( ew ) ;
		el->pose = 3 ;
	}
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_dir_set( pl_w ) ;
	if( pl_w->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		pl_w->catch_w->disp_dir ^= 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_w->jump_com_cnt = 1 ;
	ofs.x = -DOT_X( 56.0f ) ;
	ofs.y = DOT_Y( 64.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

/*==============================================================*/
static void jump_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	catch_dir_set( pl_w ) ;
	if( pl_w->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir = pl_w->disp_dir ;
		pl_w->catch_w->disp_dir ^= 1 ;
		pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir ;
	}
	pl_w->hit_stop = 10 ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 43.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 4 ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_k_catch( PLAYER_WORK *pl_w );
static void jump_k_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl28_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_k_catch, jump_k_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
	} else {
		if( pl_w->cg_flag1 ) pl_color_set( pl_w->catch_w, 6 ) ;
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			pl_se_req( pl_w, 79 ) ;
		}
		if( pl_w->cg_type ){
			if( pl_w->cg_type > 0 ){
				pl_w->cg_type = 0 ;
				pl_w->catch_w->dm_hit_w = pl_w ;
				pl_w->catch_w->catch_type = 14 ;
				pl_w->catch_w->atck_no = pl28_at_throw_p ;
			} else {
				pl_w->cg_type = 0 ;
				ofs.x = -DOT_X( 97.0f ) ;
				ofs.y = DOT_Y( 50.0f ) ;
				ofs.z = 0.0f ;
				hit_mark_set( pl_w, &ofs, 0 ) ;
				hit_se_req( pl_w->catch_w, 0 ) ;
			}
		}
	}
}

static void nml_k_jump( PLAYER_WORK *pl_w );
static void nml_k_land( PLAYER_WORK *pl_w );
static void nml_k_hold( PLAYER_WORK *pl_w );
static void nml_k_throw( PLAYER_WORK *pl_w );
static void nml_k_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_k_jump, nml_k_land, nml_k_hold, nml_k_throw, nml_k_fall,
	} ;

	nml_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_k_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		pl_w->timer0 = 60 ;
		scr_excep_set( pl_w, 13, 6 ) ;
		hold_renda_set( pl_w->catch_w, 30 ) ;
		catch_renda_set( pl_w, 3 ) ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
	}
}

/*==============================================================*/
/*			Pinned down: the buttons add hits until the time	*/
/*			runs out or the throw ends							*/
/*==============================================================*/
static void nml_k_hold( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	pl_w->timer0 -- ;
	if( pl_w->timer0 > 0 ){
		if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
		if( hold_renda_ck( pl_w->catch_w ) ) goto hold ;
		if( !pl_w->cg_type ) return ;
		if( pl_w->cg_type < 0 ){
			pl_w->cg_type = 0 ;
			pl_w->catch_w->dm_hit_w = pl_w ;
			pl_w->catch_w->atck_no = pl28_at_throw_k_hit ;
			catch_damage( pl_w ) ;
			pos.y = pl_w->pos.y + DOT_Y( 8.0f ) ;
			pos.x = pl_w->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
			pos.x += pl_w->pos.x ;
			pos.z = pl_w->pos.z ;
			hitmark_set( &pos, pl_w->disp_dir, 1, 0 ) ;
			hit_se_req( pl_w->catch_w, 0 ) ;
			pl_voice_req( pl_w, 3 ) ;
		} else goto release ;
	} else {
hold:
		pl_w->cg_ctr = 1 ;
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type <= 0 ) goto hold ;
release:
		pl_w->routine_2 ++ ;
		scr_excep_y_clr() ;
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 3 ;
		pl_w->catch_w->atck_no = pl28_at_throw_k ;
		if( pl_w->pl_type == PL_MEGAMAN || pl_w->pl_type == PL_SPIRAL ) pl_w->catch_w->pl_dir ^= 1 ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_throw( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.y = DOT_Y( 11.0f ) ;
	pl_w->acc.y = -0.9375f ;
	pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.5f ) : DOT_X( 2.5f ) ;
	pl_w->pl_stance = STANCE_AIR ;
	scr_excep_clr() ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

/*--------------------------------------------------------------*/
static void nml_k_fall( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->jump_dir = 1 ;
		pl_jump_ret( pl_w ) ;
	} else if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

static void jump_k_start( PLAYER_WORK *pl_w );
static void jump_k_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_k_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_k_start, jump_k_fall,
	} ;

	jump_k_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_k_start( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pos.y = pl_w->pos.y + DOT_Y( 80.0f ) ;
		pos.x = pl_w->pl_dir ? DOT_X( 32.0f ) : -DOT_X( 32.0f ) ;
		pos.x += pl_w->pos.x ;
		hitmark_set( &pos, pl_w->disp_dir, 1, 0 ) ;
		hit_se_req( pl_w->catch_w, 0 ) ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->atck_no = pl28_at_air_throw ;
		pl_w->spd.y = DOT_Y( 11.0f ) ;
		pl_w->acc.y = -0.9375f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.5f ) : DOT_X( 2.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_k_fall( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
	else if( pl_char_move( pl_w ) < 0 ){
		pl_w->jump_dir = 1 ;
		pl_jump_ret( pl_w ) ;
	}
}

static void hold( PLAYER_WORK *pl_w );
static void hold_k( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void pl28_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		hold, hold_k, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/*==============================================================*/
static void hold_k( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_w->cg_flag1 ) catch_em_pos_set0( pl_w->catch_w, pl_w ) ;
	else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void pl28_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl28_sp_royal ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl28_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = pl28_sp_royal ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl28_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->atck_level = 1 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl28_sp_card ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl28_sp_slash ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl28_sp_crouch_hp ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl28_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->atck_level = 1 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl28_sp_card ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl28_sp_slash ;
		pl_w->atck_level = 0 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl28_sp_crouch_hp ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl28.c" End of File	*/
