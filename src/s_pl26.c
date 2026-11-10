/* M. Bison (character 26): command checks, flight, normal attacks, appearance, win poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl26_def.h"

/* speeds for a jump */
typedef struct JUMP_SPD {
	Float	spd_x, acc_x, spd_y, acc_y;
} JUMP_SPD;

void pl26_main( PLAYER_WORK *pl_w );
void pl26_com_ck( PLAYER_WORK *pl_w );
void pl26_atck_set( PLAYER_WORK *pl_w );
void pl26_atck( PLAYER_WORK *pl_w );
void pl26_dash( PLAYER_WORK *pl_w );
void pl26_noki( PLAYER_WORK *pl_w );
void pl26_appear( PLAYER_WORK *pl_w );
void pl26_win( PLAYER_WORK *pl_w );
void pl26_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl26_catch_ck( PLAYER_WORK *pl_w );
void pl26_catch_set( PLAYER_WORK *pl_w );
void pl26_catch( PLAYER_WORK *pl_w );
void pl26_hold( PLAYER_WORK *pl_w );
int pl26_delay_ck( PLAYER_WORK *pl_w );
void pl26_t_kick( PLAYER_WORK *pl_w );
void pl26_upper( PLAYER_WORK *pl_w );
void pl26_sp_unison( PLAYER_WORK *pl_w );
void pl26_sp_gattai( PLAYER_WORK *pl_w );
void pl26_sp_assist( PLAYER_WORK *pl_w );
void pl26_sp_zero( PLAYER_WORK *pl_w );

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
static Uint16 com_shot[] = { 5, COM_GROUND, pl26_sp_shot, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Psycho Shot */
static Uint16 com_field[] = { 5, COM_GROUND, pl26_sp_field, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Psycho Field */
static Uint16 com_knee[] = { 5, 0, pl26_sp_knee, 0, KEY_BACK, KEY_DOWN | KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Double Knee Press */
static Uint16 com_warp[] = { 3, 0, pl26_sp_warp, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_ATCK } ;	/* Bison Warp */
static Uint16 com_head[] = { 32, 0, pl26_sp_head, 0, KEY_DOWN, KEY_UP, KEY_KICK } ;	/* Head Press */
static Uint16 com_skull[] = { 32, 0, pl26_sp_skull, 0, KEY_DOWN, KEY_UP, KEY_PUNCH } ;	/* Somersault Skull Diver */
static Uint16 com_crusher[] = { 3, COM_HYPER | COM_ALL_SW, pl26_sp_crusher, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Psycho Crusher (hyper) */
static Uint16 com_nightmare[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl26_sp_nightmare, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Knee Press Nightmare (hyper) */
static Uint16 com_fly[] = { 3, COM_ALL_SW, pl26_sp_fly, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Flight */
static Uint16 com_explosion[] = { 5, COM_GROUND | COM_HYPER | COM_ALL_SW, pl26_sp_explosion, 0, KEY_FORWARD, KEY_DOWN | KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Psycho Explosion (hyper) */

static void ( *pl26_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl26_main,
	pl26_com_ck,
	pl26_atck_set,
	pl26_atck,
	pl26_dash,
	pl26_noki,
	pl26_appear,
	pl26_win,
	pl26_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl26_catch_ck,	/* returns a value */
	pl26_catch_set,
	pl26_catch,
	pl26_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl26_delay_ck,
	pl26_t_kick,
	pl26_upper,
	pl26_sp_unison,
	pl26_sp_gattai,
	pl26_sp_assist,
	pl26_sp_zero,
} ;

static Uint8 shot_ck( PLAYER_WORK *pl_w );
static Uint8 field_ck( PLAYER_WORK *pl_w );
static Uint8 knee_ck( PLAYER_WORK *pl_w );
static Uint8 warp_ck( PLAYER_WORK *pl_w );
static Uint8 head_ck( PLAYER_WORK *pl_w );
static Uint8 skull_ck( PLAYER_WORK *pl_w );
static Uint8 crusher_ck( PLAYER_WORK *pl_w );
static Uint8 nightmare_ck( PLAYER_WORK *pl_w );
static Uint8 fly_ck( PLAYER_WORK *pl_w );
static Uint8 explosion_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int crusher_delay_ck( PLAYER_WORK *pl_w );
static int nightmare_delay_ck( PLAYER_WORK *pl_w );
static int explosion_delay_ck( PLAYER_WORK *pl_w );
static void fly_end( PLAYER_WORK *pl_w );
static void fly_stop( PLAYER_WORK *pl_w );

/****************************************************************/
void pl26_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl26_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl26_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( explosion_ck( pl_w ) ) return ;
	if( fly_ck( pl_w ) ) return ;
	if( crusher_ck( pl_w ) ) return ;
	if( shot_ck( pl_w ) ) return ;
	if( nightmare_ck( pl_w ) ) return ;
	if( knee_ck( pl_w ) ) return ;
	if( warp_ck( pl_w ) ) return ;
	if( head_ck( pl_w ) ) return ;
	if( skull_ck( pl_w ) ) return ;
	if( field_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 shot_ck( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_shot, &pl_w->com_w0 ) ) return 0 ;
	if( lw->shot_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_shot ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 field_ck( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	if( !com_nml_ck( pl_w, com_field, &pl_w->com_w1 ) ) return 0 ;
	if( lw->field_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_field ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			In the air once a jump, but freely in a super		*/
/*			jump												*/
/*==============================================================*/
static Uint8 knee_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_knee, &pl_w->com_w2 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_knee ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 warp_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_warp, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_warp ;
	act_st_set( pl_w, N_SUPER ) ;
	if( pl_w->cpu_flag ) pl_w->routine_3 = pl_w->atck_pk * 2 ;
	else pl_w->routine_3 = pl_w->atck_level + pl_w->atck_pk * 2 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 head_ck( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	if( !com_tame_ck( pl_w, com_head, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( pl_w->cpu_flag && pl_w->fly_flag ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_head ;
	act_st_set( pl_w, N_SPECIAL ) ;
	lw->air = pl_w->pl_stance == STANCE_AIR ;
	return 1 ;
}

/*==============================================================*/
static Uint8 skull_ck( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	if( !com_tame_ck( pl_w, com_skull, &pl_w->com_w5 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( pl_w->cpu_flag && pl_w->fly_flag ) return 0 ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->atck_level = 1 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_skull ;
	act_st_set( pl_w, N_SPECIAL ) ;
	lw->air = pl_w->pl_stance == STANCE_AIR ;
	return 1 ;
}

/*==============================================================*/
static Uint8 crusher_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crusher, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_crusher ;
	act_st_set( pl_w, N_SUPER ) ;
	if( pl_w->pl_stance == STANCE_AIR ) pl_w->routine_2 = 1 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 nightmare_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_nightmare, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	if( pl_w->fly_flag ) fly_stop( pl_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_nightmare ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 fly_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_fly, &pl_w->com_w8 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_fly ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 explosion_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_explosion, &pl_w->com_w9 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w9 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl26_sp_explosion ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl26_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 11 ) ) return 0 ;
	pl_w->pl_sp_type = pl26_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SPECIAL ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl26_delay_ck( PLAYER_WORK *pl_w )
{
	if( explosion_delay_ck( pl_w ) || crusher_delay_ck( pl_w ) || nightmare_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int crusher_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_crusher, &pl_w->com_w6 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	if( pl_w->fly_flag ) return 0 ;
	pl_w->delay_sp_type = pl26_sp_crusher ;
	return 1 ;
}

/*==============================================================*/
static int nightmare_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_nightmare, &pl_w->com_w7 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl26_sp_nightmare ;
	return 1 ;
}

/*==============================================================*/
static int explosion_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_explosion, &pl_w->com_w9 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl26_sp_explosion ;
	return 1 ;
}

/****************************************************************/
/*			A Head Press stomp (sp_head) that hits a guarding	*/
/*			opponent goes straight on to its bounce; the		*/
/*			flight counts down									*/
/****************************************************************/
void pl26_main( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->atck_no == pl26_at_head && pl_w->hit_stop && pl_w->hit_flag && pl_w->routine_1 == PL_NORM
	 && !pl_w->em_w->work_kind ){
		PLAYER_WORK *em = pl_w->em_w ;

		if( em->act_st == N_GUARD || em->routine_1 == PL_DAMAGE ){
			pl_w->hit_stop = 0 ;
			pl_char_set( pl_w, Char_Special, 9 ) ;
			return ;
		}
	}
	if( pl_w->fly_flag && pl_w->routine_1 == PL_NORM && -- lw->fly_timer < 0 ){
		if( pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER ) fly_end( pl_w ) ;
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
void pl26_atck_set( PLAYER_WORK *pl_w )
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
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

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
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		lw->hit_voice = 0 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

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
		lw->hit_voice = 0 ;
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
		pl_w->nml_level = 1 ;
		pl_voice_req( pl_w, 3 ) ;
		pl_se_req( pl_w, 41 ) ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			In the air only while that button's jump attacks	*/
/*			are left, but freely while flying					*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->fly_flag ){
		jump_atck( pl_w ) ;
		return ;
	}
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
static void crouch_hk( PLAYER_WORK *pl_w );
static void crouch_hk_slide( PLAYER_WORK *pl_w );
static void crouch_hk_end( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl26_atck( PLAYER_WORK *pl_w )
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
/*			The middle punch fires a shot						*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls26_00_set( pl_w, 0 ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy punch steps in once its hit lands			*/
/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls26_00_set( pl_w, 2 ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		if( !lw->hit_voice && pl_w->hit_flag ){
			lw->hit_voice = 1 ;
			pl_voice_req( pl_w, 3 ) ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls26_00_set( pl_w, 3 ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick hits a second time					*/
/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			break ;
		}
		if( !lw->hit_voice && pl_w->hit_flag ){
			lw->hit_voice = 1 ;
			pl_voice_req( pl_w, 3 ) ;
		}
		if( HI_BYTE( pl_w->cg_rival ) ){
			HI_BYTE( pl_w->cg_rival ) = 0 ;
			pl_w->atck_no = pl26_at_stand_hk_2 ;
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
	switch( pl_w->nml_no ){
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	case 2:
		crouch_hk( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick slides								*/
/*==============================================================*/
static void crouch_hk( PLAYER_WORK *pl_w )
{
	static void ( *crouch_hk_jmp[])( PLAYER_WORK *pl_w ) = {
		crouch_hk_slide, crouch_hk_end,
	} ;

	crouch_hk_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crouch_hk_slide( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.5f ) : -DOT_X( 8.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.7291666f : 0.7291666f ;
	}
}

/*--------------------------------------------------------------*/
static void crouch_hk_end( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) == 2 ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		RATE_CLR_X( pl_w ) ;
	}
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
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		break ;
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls26_00_set( pl_w, 4 ) ;
		}
		break ;
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		else if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls26_00_set( pl_w, 5 ) ;
		}
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
void pl26_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 5 ) ;
		pl_w->atck_no = pl26_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_gauge_add( pl_w, 5 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls26_00_set( pl_w, 1 ) ;
	}
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl26_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	pl_smoke_set( pl_w ) ;
	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 28 ;
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
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 1.6666666f : -1.6666666f ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl26_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.3125f : -0.3125f ;
		pl_w->timer0 = 28 ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void appear_start( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void appear_fall( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void appear_fall_low( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void appear_land( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/****************************************************************/
/*			The appearance: he drops in from above				*/
/****************************************************************/
void pl26_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		appear_init, appear_start, appear_fall, appear_fall_low, appear_land,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_voice_req( pl_w, 22 ) ;
	pl_w->disp_flag = 0 ;
	lw->unk_04 = 1 ;
	pl_w->pos.y = pl_w->ground_y + DOT_Y( 80.0f ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_start( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->disp_flag = 1 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.y = -DOT_Y( 0.875f ) ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		lw->unk_04 = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y + DOT_Y( 24.0f ) ){
		pl_w->routine_2 ++ ;
		pls26_00_set( pl_w, 13 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fall_low( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_land( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_time_ko( PLAYER_WORK *pl_w );
static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );
static void win_ko_move_e( PLAYER_WORK *pl_w );
static void win_ko_move_2( PLAYER_WORK *pl_w );
static void win_ko_vanish( PLAYER_WORK *pl_w );
static void win_ko_wait( PLAYER_WORK *pl_w );
static void win_ko_appear( PLAYER_WORK *pl_w );
static void win_ko_float( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void draw_time( PLAYER_WORK *pl_w );

/****************************************************************/
void pl26_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko, lose_time_ko, win_time_ko, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move, win_ko_move_e, win_ko_move_2, win_ko_vanish, win_ko_wait, win_ko_appear, win_ko_float,
	} ;

	win_ko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
/*			Won: one of three poses, picked at random			*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	static Uint8 win_pose0[8] = { 0, 0, 0, 1, 1, 1, 2, 2 } ;
	static Uint8 win_pose1[8] = { 0, 0, 0, 0, 1, 1, 1, 1 } ;

	if( vs_sys_work.vs_end_flag ) pl_w->timer0 = win_pose0[Sys->game_timer0 & 7] ;
	else pl_w->timer0 = win_pose1[Sys->game_timer0 & 7] ;
	switch( pl_w->timer0 ){
	case 0:
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 60 ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
		break ;
	case 1:
		pl_w->routine_2 = 3 ;
		pl_char_set( pl_w, Char_Win, 1 ) ;
		break ;
	case 2:
		pl_w->routine_2 = 4 ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	if( !-- pl_w->timer1 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 60 ;
		pl_char_set( pl_w, Char_Win, 4 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move_e( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move_2( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The third pose: he vanishes,						*/
/*==============================================================*/
static void win_ko_vanish( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	pl_w->win_wait = 0xff ;
	if( HI_BYTE( pl_w->cg_rival ) ) lw->unk_04 = 1 ;
	if( pl_w->cg_type ) pl_w->cg_type = 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 10 ;
		pl_w->disp_flag = 0 ;
		pl_w->push_ng_timer = 1 ;
	}
}

/*==============================================================*/
/*			Comes back high up,									*/
/*==============================================================*/
static void win_ko_wait( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_w->disp_flag = 0 ;
	pl_w->push_ng_timer = 1 ;
	if( -- pl_w->timer1 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->pos.y = pl_w->ground_y + DOT_Y( 64.0f ) ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_ko_appear( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	pl_w->win_wait = 0xff ;
	if( HI_BYTE( pl_w->cg_rival ) ) lw->unk_04 = 1 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 16 ;
		pl_w->spd.y = 2.142857f ;
		pl_voice_req( pl_w, 15 ) ;
		pl_char_set( pl_w, Char_Win, 4 ) ;
	}
}

/*==============================================================*/
/*			And floats up and down								*/
/*==============================================================*/
static void win_ko_float( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	if( !-- pl_w->timer1 ){
		pl_w->timer1 = 16 ;
		pl_w->spd.y = -pl_w->spd.y ;
	}
	pl_w->pos.y += pl_w->spd.y ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 3 ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 3 ) ;
	} else pl_char_move( pl_w ) ;
}

static void sp_shot( PLAYER_WORK *pl_w );
static void sp_knee( PLAYER_WORK *pl_w );
static void sp_warp( PLAYER_WORK *pl_w );
static void sp_head( PLAYER_WORK *pl_w );
static void sp_skull( PLAYER_WORK *pl_w );
static void sp_nightmare( PLAYER_WORK *pl_w );
static void sp_crusher( PLAYER_WORK *pl_w );
static void sp_field( PLAYER_WORK *pl_w );
static void sp_fly( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_explosion( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Special moves and hyper combos						*/
/****************************************************************/
void pl26_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_shot, sp_knee, sp_warp, sp_head, sp_skull, sp_nightmare, sp_crusher, sp_field,
		sp_shot, sp_fly, sp_shot, sp_chouhatsu, sp_shot, sp_shot, sp_snapback, sp_explosion,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void shot_init( PLAYER_WORK *pl_w );
static void shot_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_shot( PLAYER_WORK *pl_w )
{
	static void ( *shot_jmp[])( PLAYER_WORK *pl_w ) = {
		shot_init, shot_move,
	} ;

	shot_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shot_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level ? 56 : 54 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shot_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		HI_BYTE( pl_w->cg_rival ) &= 0xfe ;
		pls26_00_set( pl_w, 6 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl26_00_set( pl_w, 0 ) ;
	}
}

static void knee_ground( PLAYER_WORK *pl_w );
static void knee_g_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void knee_g_jump( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void knee_g_move( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void knee_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void knee_air( PLAYER_WORK *pl_w );
static void knee_j_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void knee_j_jump( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void knee_j_move( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			A leaping attack, from the ground or the air		*/
/*==============================================================*/
static void sp_knee( PLAYER_WORK *pl_w )
{
	static void ( *knee_jmp[])( PLAYER_WORK *pl_w ) = {
		knee_ground, knee_air,
	} ;

	knee_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void knee_ground( PLAYER_WORK *pl_w )
{
	static void ( *knee_g_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		knee_g_init, knee_g_jump, knee_g_move, knee_end,
	} ;

	knee_g_jmp[pl_w->routine_3]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void knee_g_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->team_type != 3 ){
		pl_w->atck_no = pl_w->atck_level ? 52 : 48 ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->atck_no = pl26_at_knee_vc_1 ;
		HIT_RESET( pl_w ) ;
	}
	pl_sp_ef_set( pl_w ) ;
	lw->hit_voice = 0 ;
	pl_char_set( pl_w, Char_Special, 1 ) ;
}

/*--------------------------------------------------------------*/
static void knee_g_jump( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	static JUMP_SPD knee_g_spd[2] = {
		{ 12.5f, -0.3125f, DOT_Y( 2.0f ), -DOT_Y( 0.375f ) },
		{ 20.0f, -0.3125f, DOT_Y( 3.0f ), -DOT_Y( 0.375f ) },
	} ;

	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_w->routine_3 ++ ;
		pl_air_set( pl_w ) ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? knee_g_spd[pl_w->atck_level].spd_x : -knee_g_spd[pl_w->atck_level].spd_x ;
		pl_w->acc.x = pl_w->pl_dir ? knee_g_spd[pl_w->atck_level].acc_x : -knee_g_spd[pl_w->atck_level].acc_x ;
		pl_w->spd.y = knee_g_spd[pl_w->atck_level].spd_y ;
		pl_w->acc.y = knee_g_spd[pl_w->atck_level].acc_y ;
	}
}

/*--------------------------------------------------------------*/
static void knee_g_move( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !lw->hit_voice && pl_w->hit_flag ){
		lw->hit_voice = 1 ;
		pl_voice_req( pl_w, 3 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( pl_w->team_type != 3 ){
			pl_w->atck_no = pl_w->atck_level ? 53 : 49 ;
			HIT_RESET( pl_w ) ;
		} else {
			pl_w->atck_no = pl26_at_knee_vc_2 ;
			HIT_RESET( pl_w ) ;
		}
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void knee_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void knee_air( PLAYER_WORK *pl_w )
{
	static void ( *knee_j_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		knee_j_init, knee_j_jump, knee_j_move, knee_end,
	} ;

	knee_j_jmp[pl_w->routine_3]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void knee_j_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->routine_3 ++ ;
	pl_w->atck_no = pl26_at_knee_l_1 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	lw->hit_voice = 0 ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
}

/*--------------------------------------------------------------*/
static void knee_j_jump( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	static JUMP_SPD knee_j_spd[2] = {
		{ 12.5f, -0.3125f, DOT_Y( 0.5f ), -DOT_Y( 0.5f ) },
		{ 20.0f, -0.3125f, DOT_Y( 0.5f ), -DOT_Y( 0.5f ) },
	} ;

	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_w->routine_3 ++ ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? knee_j_spd[pl_w->atck_level].spd_x : -knee_j_spd[pl_w->atck_level].spd_x ;
		pl_w->acc.x = pl_w->pl_dir ? knee_j_spd[pl_w->atck_level].acc_x : -knee_j_spd[pl_w->atck_level].acc_x ;
		pl_w->spd.y = knee_j_spd[pl_w->atck_level].spd_y ;
		pl_w->acc.y = knee_j_spd[pl_w->atck_level].acc_y ;
	}
}

/*--------------------------------------------------------------*/
static void knee_j_move( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !lw->hit_voice && pl_w->hit_flag ){
		lw->hit_voice = 1 ;
		pl_voice_req( pl_w, 3 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl_w->atck_level ? 43 : 41 ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	}
}

/* where the teleport ends: behind or in front of the opponent, high or low */
static Float warp_pos[4][2] = {
	{ -DOT_X( 128.0f ), DOT_Y( 96.0f ) },
	{ DOT_X( 608.0f ), DOT_Y( 96.0f ) },
	{ -DOT_X( 128.0f ), 0.0f },
	{ DOT_X( 608.0f ), 0.0f },
} ;

static void warp_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void warp_vanish( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void warp_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void warp_appear( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			The teleport										*/
/*==============================================================*/
static void sp_warp( PLAYER_WORK *pl_w )
{
	static void ( *warp_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		warp_init, warp_vanish, warp_wait, warp_appear,
	} ;

	warp_jmp[pl_w->routine_2]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void warp_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 2 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
}

/*--------------------------------------------------------------*/
static void warp_vanish( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( HI_BYTE( pl_w->cg_rival ) ) lw->unk_04 = 1 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		lw->unk_04 = 1 ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		lw->unk_04 = 1 ;
		pl_w->disp_flag = 0 ;
		pl_w->push_ng_timer = 1 ;
		pl_voice_req( pl_w, 27 ) ;
		pls26_00_warp_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void warp_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->disp_flag = 0 ;
	pl_w->push_ng_timer = 1 ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->pos.x = scr_work.eye[0].x + warp_pos[pl_w->routine_3][0] + -320.0f ;
		pl_w->pos.y = pl_w->target_w->pos.y + warp_pos[pl_w->routine_3][1] ;
		if( pl_w->pos.y > pl_w->ground_y ){
			pl_w->pl_stance = STANCE_AIR ;
			pl_w->jump_com_cnt = 0 ;
			pl_w->jump_dash_cnt = 0 ;
			pl_w->jump_atk_cnt = JATK_ONE ;
			pl_w->air_jump_cnt = 0 ;
		} else pl_w->pl_stance = STANCE_STAND ;
		pl_voice_req( pl_w, 28 ) ;
		pl_w->pl_dir = pl_w->disp_dir = ( pl_w->routine_3 & 1 ) ? 0 : 1 ;
		pl_char_set( pl_w, Char_Special, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void warp_appear( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( HI_BYTE( pl_w->cg_rival ) ) lw->unk_04 = 1 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else {
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

static void head_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_rise( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_fall( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_bound( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_air( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_dive_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_dive( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void head_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			A jump onto the opponent's head, a bounce off it	*/
/*			and a dive											*/
/*==============================================================*/
static void sp_head( PLAYER_WORK *pl_w )
{
	static void ( *head_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		head_init, head_wait, head_rise, head_fall, head_bound, head_air, head_dive_wait, head_dive,
		head_end,
	} ;

	head_jmp[pl_w->routine_2]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void head_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	Float h ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->target_w->pos.x - pl_w->pos.x ;
	pl_w->spd.x /= 32.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -1.2053571f ;
	pl_w->timer0 = 0 ;
	if( lw->air ){
		pl_w->timer0 = 5 ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
	h = DOT_Y( 96.0f ) ;
	if( pl_w->target_w->pl_stance == STANCE_AIR ) h = DOT_Y( 128.0f ) ;
	if( pl_w->target_w->pl_stance == STANCE_CROUCH ) h = DOT_Y( 48.0f ) ;
	h = h + pl_w->target_w->pos.y - pl_w->pos.y ;
	if( h > 0.0f && h < DOT_Y( 144.0f ) ) h = DOT_Y( 144.0f ) ;
	pl_w->spd.y += h / 32.0f ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 7 ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void head_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void head_rise( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl26_at_head ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 8 ) ;
	}
}

/*==============================================================*/
/*			Coming down: a hit bounces him off, the floor		*/
/*			ends it												*/
/*==============================================================*/
static void head_fall( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		if( pl_w->hit_flag ){
			pl_w->routine_2 ++ ;
			return ;
		}
	} else {
		pl_w->routine_2 = 8 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 12 ) ;
	}
}

/*==============================================================*/
/*			The bounce, steered away from the stick's side		*/
/*==============================================================*/
static void head_bound( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		if( !pl_w->cpu_flag ){
			if( pl_w->pl_new_lvbt & KEY_FORWARD ) pl_w->pl_dir = pl_w->disp_dir = 0 ;
			else if( pl_w->pl_new_lvbt & KEY_BACK ) pl_w->pl_dir = pl_w->disp_dir = 1 ;
		}
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.125f ) : -DOT_X( 0.125f ) ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 10 ) ;
	}
}

/*==============================================================*/
/*			High enough and falling, a button starts the		*/
/*			dive												*/
/*==============================================================*/
static void head_air( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_w->pos.y - pl_w->ground_y > DOT_Y( 64.0f ) && pl_w->spd.y < 0.0f ){
		if( ( pl_w->cpu_flag && pl_w->atck_pk ) || ( !pl_w->cpu_flag && ( pl_w->pl_new_lvbt & KEY_PUNCH ) ) ){
			pl_w->routine_2 ++ ;
			pl_w->timer0 = 10 ;
			return ;
		}
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		if( !pl_w->cpu_flag ){
			if( pl_w->pl_new_lvbt & KEY_FORWARD ) pl_w->spd.x = -5.0f ;
			else if( pl_w->pl_new_lvbt & KEY_BACK ) pl_w->spd.x = 5.0f ;
		}
	} else {
		pl_w->routine_2 = 8 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void head_dive_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( -- pl_w->timer0 ) pl_char_move( pl_w ) ;
	else {
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl26_at_head_dive ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void head_dive( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		if( HI_BYTE( pl_w->cg_rival ) ){
			HI_BYTE( pl_w->cg_rival ) = 0 ;
			pls26_00_set( pl_w, 7 ) ;
		}
	} else {
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void head_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void skull_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void skull_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void skull_jump( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void skull_dive( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void skull_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			The skull crusher: a jump at the enemy, then a		*/
/*			dive on a button									*/
/*==============================================================*/
static void sp_skull( PLAYER_WORK *pl_w )
{
	static void ( *skull_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		skull_init, skull_wait, skull_jump, skull_dive, skull_end,
	} ;

	skull_jmp[pl_w->routine_2]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void skull_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	Float h ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = pl_w->target_w->pos.x - pl_w->pos.x ;
	pl_w->spd.x += pl_w->spd.x < 0.0f ? -20.0f : 20.0f ;
	pl_w->spd.x /= 32.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -0.9375f ;
	pl_w->timer0 = lw->air ? 5 : 0 ;
	h = DOT_Y( 96.0f ) ;
	if( pl_w->target_w->pl_stance == STANCE_AIR ) h = DOT_Y( 128.0f ) ;
	if( pl_w->target_w->pl_stance == STANCE_CROUCH ) h = DOT_Y( 48.0f ) ;
	h = h + pl_w->target_w->pos.y - pl_w->pos.y ;
	if( h > 0.0f && h < DOT_Y( 144.0f ) ) h = DOT_Y( 144.0f ) ;
	pl_w->spd.y += h / 32.0f ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
}

/*--------------------------------------------------------------*/
static void skull_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void skull_jump( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y - pl_w->ground_y > DOT_Y( 64.0f ) ){
		if( ( pl_w->cpu_flag && pl_w->atck_pk ) || ( !pl_w->cpu_flag && ( pl_w->pl_new_lvbt & KEY_PUNCH ) ) ){
			pl_w->routine_2 ++ ;
			pl_w->atck_no = pl26_at_skull ;
			HIT_RESET( pl_w ) ;
			pl_dir_set( pl_w ) ;
			pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
			pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.375f ) : -DOT_X( 0.375f ) ;
			pl_w->spd.y = DOT_Y( 6.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( pl_w, Char_Special, 14 ) ;
		}
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 = 4 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void skull_dive( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		if( HI_BYTE( pl_w->cg_rival ) ){
			HI_BYTE( pl_w->cg_rival ) = 0 ;
			pls26_00_set( pl_w, 7 ) ;
		}
		if( !pl_w->cpu_flag ){
			if( pl_w->pl_new_lvbt & KEY_FORWARD ) pl_w->spd.x = -DOT_X( 4.0f ) ;
			else if( pl_w->pl_new_lvbt & KEY_BACK ) pl_w->spd.x = DOT_X( 4.0f ) ;
		}
	} else {
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void skull_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void nightmare_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_flash( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_flash_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_ready( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_kick_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_kick_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_kick1( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_kick2( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_crush_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void nightmare_crush( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			The knee press nightmare: two jumping kicks,		*/
/*			then the psycho crusher								*/
/*==============================================================*/
static void sp_nightmare( PLAYER_WORK *pl_w )
{
	static void ( *nightmare_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		nightmare_init, nightmare_flash, nightmare_flash_wait, nightmare_ready, nightmare_kick_init, nightmare_kick_wait,
		nightmare_kick1, nightmare_kick2, nightmare_crush_wait, nightmare_crush,
	} ;

	nightmare_jmp[pl_w->routine_2]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void nightmare_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 255 ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_voice_req( pl_w, 22 ) ;
	pl_sp_ef_set( pl_w ) ;
	lw->start_flag = 0 ;
	pl_char_set( pl_w, Char_Special, 16 ) ;
}

/*--------------------------------------------------------------*/
static void nightmare_flash( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( HI_BYTE( pl_w->cg_rival ) ) lw->unk_04 = 1 ;
	if( pl_w->cg_type == 2 ) pl_w->cg_type = 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 2 ;
		pl_w->disp_flag = 0 ;
		pl_w->push_ng_timer = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void nightmare_flash_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_w->disp_flag = 0 ;
	pl_w->push_ng_timer = 1 ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		shl26_01_set( pl_w ) ;
		pl_dir_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 17 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nightmare_ready( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( HI_BYTE( pl_w->cg_rival ) ) lw->unk_04 = 1 ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 56.0f ) ;
		ofs.y = DOT_Y( 80.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_w->catch_ng_timer = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void nightmare_kick_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = 4 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl26_at_nightmare_kick ;
	HIT_RESET( pl_w ) ;
	lw->start_flag = 1 ;
	pl_char_set( pl_w, Char_Special, 18 ) ;
}

/*--------------------------------------------------------------*/
static void nightmare_kick_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( -- pl_w->timer0 < 0 ){
		pl_char_move( pl_w ) ;
		if( HI_BYTE( pl_w->cg_rival ) ){
			pl_w->routine_2 ++ ;
			pl_w->pl_stance = STANCE_AIR ;
			lw->hit_voice = 0 ;
			pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
			pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
			pl_w->spd.y = DOT_Y( 3.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void nightmare_kick1( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl26_at_nightmare_kick ;
		HIT_RESET( pl_w ) ;
		lw->hit_timer = 2 ;
	}
	if( ! -- lw->hit_timer && pl_w->hit_flag ){
		pl_w->atck_no = pl26_at_nightmare_kick ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		lw->hit_voice = 0 ;
		pl_w->atck_no = pl26_at_nightmare_kick ;
		HIT_RESET( pl_w ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 13.0f ) : -DOT_X( 13.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -0.3125f : 0.3125f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 19 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nightmare_kick2( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->atck_no = pl26_at_nightmare_kick ;
		HIT_RESET( pl_w ) ;
		lw->hit_timer = 2 ;
	}
	if( ! -- lw->hit_timer && pl_w->hit_flag ){
		pl_w->atck_no = pl26_at_nightmare_kick ;
		HIT_RESET( pl_w ) ;
	}
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nightmare_crush_wait( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) ){
		pl_w->routine_2 ++ ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->atck_no = pl26_at_nightmare_kick ;
		HIT_RESET( pl_w ) ;
		lw->hit_timer = 2 ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 14.0f ) : -DOT_X( 14.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_se_req( pl_w, 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nightmare_crush( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( HI_BYTE( pl_w->cg_rival ) == 2 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( ! -- lw->hit_timer && pl_w->hit_flag ){
		pl_w->atck_no = pl26_at_nightmare_crush ;
		HIT_RESET( pl_w ) ;
	}
}

static void crusher_ground( PLAYER_WORK *pl_w );
static void crusher_g_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_g_flash( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_g_start( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_g_fly( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_g_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_air( PLAYER_WORK *pl_w );
static void crusher_j_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_j_flash( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_j_start( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_j_fly( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void crusher_j_fall( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			The psycho crusher hyper, from the ground or the	*/
/*			air: pressing buttons adds hits						*/
/*==============================================================*/
static void sp_crusher( PLAYER_WORK *pl_w )
{
	static void ( *crusher_jmp[])( PLAYER_WORK *pl_w ) = {
		crusher_ground, crusher_air,
	} ;

	crusher_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void crusher_ground( PLAYER_WORK *pl_w )
{
	static void ( *crusher_g_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		crusher_g_init, crusher_g_flash, crusher_g_start, crusher_g_fly, crusher_g_end,
	} ;

	crusher_g_jmp[pl_w->routine_3]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void crusher_g_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 255 ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	lw->hit_cnt = 0 ;
	lw->hit_full = 0 ;
	lw->hold_timer = 0 ;
	lw->press_cnt = 0 ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 21 ) ;
}

/*--------------------------------------------------------------*/
static void crusher_g_flash( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 40.0f ) ;
		ofs.y = DOT_Y( 72.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_char_set( pl_w, Char_Special, 40 ) ;
		pls26_00_crusher_flash_set( pl_w, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crusher_g_start( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 15.0f : -15.0f ;
		pl_w->atck_no = pl26_at_crusher ;
		HIT_RESET( pl_w ) ;
		lw->hit_timer = 2 ;
		pl_w->timer0 = 96 ;
		pl_w->timer1 = 7 ;
		pl_w->ef_w = 0 ;
		ef_color_set( pl_w, 0, pl_w->pl_color * 2, 1 ) ;
		pls26_00_set( pl_w, 9 ) ;
		shl26_02_set( pl_w, 0 ) ;
		pl_char_set( pl_w, Char_Special, 22 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crusher_g_fly( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	PLAYER_WORK	*ew ;
	Uint8	need ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	ew = pl_w->ef_w ;
	if( ew && ew->hit_box_no ){
		pl_w->spd.x -= pl_w->pl_dir ? -1.25f : 1.25f ;
		lw->hold_timer = 6 ;
	}
	if( pl_w->cg_type ) lw->unk_04 = 1 ;
	pl_w->push_ng_timer = 1 ;
	if( ( !pl_w->pl_dir && !( pl_w->wall_flag & WALL_LEFT ) ) || ( pl_w->pl_dir && !( pl_w->wall_flag & WALL_RIGHT ) ) ){
		if( lw->hold_timer ){
			if( ! -- lw->hold_timer ){
				pl_w->acc.x += pl_w->pl_dir ? 0.104166664f : -0.104166664f ;
				pl_w->pos.x += pl_w->spd.x ;
				pl_w->spd.x += pl_w->acc.x ;
				pl_w->pos.y += pl_w->spd.y ;
				pl_w->spd.y += pl_w->acc.y ;
			}
		} else {
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
		if( sw_renda_ck( pl_w ) ){
			pl_w->cg_ctr = 1 ;
			lw->press_cnt ++ ;
			if( pl_w->cpu_flag ) need = 6 ;
			else need = 2 ;
			if( lw->press_cnt >= need ){
				lw->press_cnt = 0 ;
				if( pl_w->timer1 >= 10 ) lw->hit_full = 1 ;
				if( !lw->hit_full ) pl_w->timer1 ++ ;
			}
		}
		pl_char_move( pl_w ) ;
		if( -- pl_w->timer0 >= 0 ){
			if( pl_w->hit_flag ){
				if( ! -- lw->hit_timer && ++ lw->hit_cnt < 10 ){
					if( -- pl_w->timer1 ){
						pl_w->atck_no = pl26_at_crusher ;
						HIT_RESET( pl_w ) ;
						lw->hit_timer = 2 ;
					}
				}
			}
			return ;
		}
	}
	pl_w->routine_3 ++ ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_land_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 23 ) ;
}

/*--------------------------------------------------------------*/
static void crusher_g_end( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( HI_BYTE( pl_w->cg_rival ) ){
			pl_w->pl_stance = STANCE_STAND ;
			HI_BYTE( pl_w->cg_rival ) = 0 ;
		}
		return ;
	}
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void crusher_air( PLAYER_WORK *pl_w )
{
	static void ( *crusher_j_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		crusher_j_init, crusher_j_flash, crusher_j_start, crusher_j_fly, crusher_j_fall,
	} ;

	crusher_j_jmp[pl_w->routine_3]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void crusher_j_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 255 ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_3 ++ ;
	lw->hit_cnt = 0 ;
	lw->hit_full = 0 ;
	lw->hold_timer = 0 ;
	lw->press_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 24 ) ;
}

/*--------------------------------------------------------------*/
static void crusher_j_flash( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 104.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
		pl_char_set( pl_w, Char_Special, 41 ) ;
		pls26_00_crusher_flash_set( pl_w, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crusher_j_start( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? 15.0f : -15.0f ;
		pl_w->atck_no = pl26_at_crusher ;
		HIT_RESET( pl_w ) ;
		lw->hit_timer = 2 ;
		pl_w->timer0 = 96 ;
		pl_w->timer1 = 7 ;
		pl_w->ef_w = 0 ;
		pls26_00_set( pl_w, 11 ) ;
		shl26_02_set( pl_w, 0 ) ;
		pl_char_set( pl_w, Char_Special, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void crusher_j_fly( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	PLAYER_WORK	*ew ;
	Uint8	need ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	ew = pl_w->ef_w ;
	if( ew && ew->hit_box_no ){
		pl_w->spd.x -= pl_w->pl_dir ? -1.25f : 1.25f ;
		lw->hold_timer = 6 ;
	}
	if( pl_w->cg_type ) lw->unk_04 = 1 ;
	pl_w->push_ng_timer = 1 ;
	if( pl_w->target_w->vital_new > 0 || vs_sys_work.ko_cnt[pl_w->target_w->pl_no] > 2 ){
		if( ( !pl_w->pl_dir && !( pl_w->wall_flag & WALL_LEFT ) ) || ( pl_w->pl_dir && !( pl_w->wall_flag & WALL_RIGHT ) ) ){
			if( lw->hold_timer ){
				if( ! -- lw->hold_timer ){
					pl_w->acc.x += pl_w->pl_dir ? 0.104166664f : -0.104166664f ;
					pl_w->pos.x += pl_w->spd.x ;
					pl_w->spd.x += pl_w->acc.x ;
					pl_w->pos.y += pl_w->spd.y ;
					pl_w->spd.y += pl_w->acc.y ;
				}
			} else {
				pl_w->pos.x += pl_w->spd.x ;
				pl_w->spd.x += pl_w->acc.x ;
				pl_w->pos.y += pl_w->spd.y ;
				pl_w->spd.y += pl_w->acc.y ;
			}
			if( sw_renda_ck( pl_w ) ){
				pl_w->cg_ctr = 1 ;
				lw->press_cnt ++ ;
				if( pl_w->cpu_flag ) need = 6 ;
				else need = 2 ;
				if( lw->press_cnt >= need ){
					lw->press_cnt = 0 ;
					if( pl_w->timer1 >= 10 ) lw->hit_full = 1 ;
					if( !lw->hit_full ) pl_w->timer1 ++ ;
				}
			}
			pl_char_move( pl_w ) ;
			if( -- pl_w->timer0 >= 0 ){
				if( pl_w->hit_flag ){
					if( ! -- lw->hit_timer && ++ lw->hit_cnt < 10 ){
						if( -- pl_w->timer1 ){
							pl_w->atck_no = pl26_at_crusher ;
							HIT_RESET( pl_w ) ;
							lw->hit_timer = 2 ;
						}
					}
				}
				return ;
			}
		}
	}
	pl_w->routine_3 ++ ;
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Special, 48 ) ;
}

/*--------------------------------------------------------------*/
static void crusher_j_fall( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y || pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void field_init( PLAYER_WORK *pl_w );
static void field_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_field( PLAYER_WORK *pl_w )
{
	static void ( *field_jmp[])( PLAYER_WORK *pl_w ) = {
		field_init, field_move,
	} ;

	field_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void field_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl26_at_field ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_voice_req( pl_w, 24 ) ;
	pl_char_set( pl_w, Char_Special, 26 ) ;
}

/*--------------------------------------------------------------*/
static void field_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( HI_BYTE( pl_w->cg_rival ) ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		shl26_00_field_set( pl_w, 0 ) ;
	}
}

static void fly_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );
static void fly_rise( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw );

/*==============================================================*/
/*			Flight: pressed again while flying, it ends			*/
/*==============================================================*/
static void sp_fly( PLAYER_WORK *pl_w )
{
	static void ( *fly_jmp[])( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw ) = {
		fly_init, fly_rise,
	} ;

	fly_jmp[pl_w->routine_2]( pl_w, ( PL26_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void fly_init( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	if( pl_w->fly_flag ){
		fly_end( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		pl_air_set( pl_w ) ;
	}
	pl_gauge_add( pl_w, 2 ) ;
	pl_w->fly_flag = 1 ;
	pl_w->sjump_flag = 0 ;
	lw->fly_timer = 600 ;
	pl_w->timer0 = 16 ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.125f ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Fly, 0 ) ;
}

/*--------------------------------------------------------------*/
static void fly_rise( PLAYER_WORK *pl_w, PL26_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The flight ends: he falls							*/
/*==============================================================*/
static void fly_end( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
/*			The flight ends for a special move					*/
/*==============================================================*/
static void fly_stop( PLAYER_WORK *pl_w )
{
	pl_w->fly_flag = 0 ;
	pl_w->spd.x = pl_w->acc.x = pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
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
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->atck_no = pl26_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 47 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type = 0 ;
		ofs.x = 1.6666666f ;
		ofs.y = DOT_Y( 23.0f ) ;
		ofs.z = 0.0f ;
		pl_snap_flash( pl_w, &ofs ) ;
	} else if( pl_w->cg_type & 2 ){
		pl_w->cg_type = 0 ;
		pl_se_req( pl_w, SE_SWING_H ) ;
	}
}

/*==============================================================*/
/*			A pose held for a second							*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_char_set( pl_w, Char_Special, 29 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( -- pl_w->timer0 == 0 ){
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_stand_ret( pl_w ) ;
		}
	}
}

static void explosion_init( PLAYER_WORK *pl_w );
static void explosion_flash( PLAYER_WORK *pl_w );
static void explosion_shot( PLAYER_WORK *pl_w );
static void explosion_wait( PLAYER_WORK *pl_w );
static void explosion_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo that sends out an effect, and			*/
/*			waits for it to finish								*/
/*==============================================================*/
static void sp_explosion( PLAYER_WORK *pl_w )
{
	static void ( *explosion_jmp[])( PLAYER_WORK *pl_w ) = {
		explosion_init, explosion_flash, explosion_shot, explosion_wait, explosion_end,
	} ;

	explosion_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void explosion_init( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 255 ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl26_at_explosion ;
	HIT_RESET( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	lw->ef_end_flag = 0 ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void explosion_flash( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( HI_BYTE( pl_w->cg_rival ) & 1 ){
		HI_BYTE( pl_w->cg_rival ) &= ~1 ;
		pls26_00_set( pl_w, 6 ) ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 49.0f ) ;
		ofs.y = DOT_Y( 69.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void explosion_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 0 ;
		if( !shl26_00_set( pl_w, 2 ) ) pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void explosion_wait( PLAYER_WORK *pl_w )
{
	PL26_LOCAL_W *lw = ( PL26_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( !pl_w->timer0 && pl_w->hit_flag ){
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->cg_type = 0 ;
		pl_w->timer0 = 1 ;
	}
	if( lw->ef_end_flag ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void explosion_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl26_t_kick( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl26_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 4 ) ;
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

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl26_catch_ck( PLAYER_WORK *pl_w )
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

static void nml_set( PLAYER_WORK *pl_w );
static void jump_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl26_catch_ck chose			*/
/****************************************************************/
void pl26_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_set, jump_set,
	} ;
	PLAYER_WORK *em ;

	em = pl_w->catch_w ;
	em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	catch_gauge_add( pl_w ) ;
	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	if( pl_w->dir & 2 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	ofs.x = -120.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void jump_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	if( pl_w->dir & 1 ){
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
		em = pl_w->catch_w ;
		em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	ofs.x = DOT_X( 32.0f ) ;
	ofs.y = DOT_Y( 128.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

static void nml_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void pl26_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch, jump_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( HI_BYTE( pl_w->cg_rival ) ){
		HI_BYTE( pl_w->cg_rival ) = 0 ;
		pl_w->pl_dir = pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
	}
	if( pl_char_move( pl_w ) >= 0 ){
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 1 ;
			em->atck_no = pl26_at_throw ;
		}
		return ;
	}
	pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		pls26_00_set( pl_w, 5 ) ;
	}
	if( pl_w->cg_type == 1 ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->atck_no = pl26_at_air_throw ;
	}
}

/****************************************************************/
/*			The opponent's side of each throw					*/
/****************************************************************/
void pl26_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
void pl26_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl26_sp_crusher ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl26_sp_crusher ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl26_sp_nightmare ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl26_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl26_sp_crusher ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl26_sp_crusher ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl26_sp_nightmare ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl26_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl26_sp_shot ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl26_sp_field ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl26_sp_knee ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl26_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl26_sp_shot ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl26_sp_field ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl26_sp_knee ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl26.c" End of File	*/
