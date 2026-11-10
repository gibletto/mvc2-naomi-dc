/* Colossus (character 32): command checks, his glow, normal attacks, dashes, appearance, win
   poses, special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl32_def.h"

void pl32_main( PLAYER_WORK *pl_w );
void pl32_com_ck( PLAYER_WORK *pl_w );
void pl32_atck_set( PLAYER_WORK *pl_w );
void pl32_atck( PLAYER_WORK *pl_w );
void pl32_dash( PLAYER_WORK *pl_w );
void pl32_noki( PLAYER_WORK *pl_w );
void pl32_appear( PLAYER_WORK *pl_w );
void pl32_win( PLAYER_WORK *pl_w );
void pl32_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl32_catch_ck( PLAYER_WORK *pl_w );
void pl32_catch_set( PLAYER_WORK *pl_w );
void pl32_catch( PLAYER_WORK *pl_w );
void pl32_hold( PLAYER_WORK *pl_w );
int pl32_delay_ck( PLAYER_WORK *pl_w );
void pl32_t_kick( PLAYER_WORK *pl_w );
void pl32_upper( PLAYER_WORK *pl_w );
void pl32_sp_unison( PLAYER_WORK *pl_w );
void pl32_sp_gattai( PLAYER_WORK *pl_w );
void pl32_sp_assist( PLAYER_WORK *pl_w );
void pl32_sp_zero( PLAYER_WORK *pl_w );
int pl32_armor_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_tackle[] = { 3, 0, pl32_sp_tackle, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Power Tackle */
static Uint16 com_swing[] = { 3, 0, pl32_sp_swing, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Giant Swing */
static Uint16 com_armor[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl32_sp_armor, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Super Armor */
static Uint16 com_dive[] = { 3, COM_HYPER | COM_ALL_SW, pl32_sp_dive, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Super Dive */

static void ( *pl32_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl32_main,
	pl32_com_ck,
	pl32_atck_set,
	pl32_atck,
	pl32_dash,
	pl32_noki,
	pl32_appear,
	pl32_win,
	pl32_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl32_catch_ck,	/* returns a value */
	pl32_catch_set,
	pl32_catch,
	pl32_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl32_delay_ck,
	pl32_t_kick,
	pl32_upper,
	pl32_sp_unison,
	pl32_sp_gattai,
	pl32_sp_assist,
	pl32_sp_zero,
	0, 0, 0, 0,
	( void ( * )( PLAYER_WORK *pl_w ) )pl32_armor_ck,	/* takes the attacker too */
} ;

static Uint8 tackle_ck( PLAYER_WORK *pl_w );
static Uint8 swing_ck( PLAYER_WORK *pl_w );
static Uint8 armor_ck( PLAYER_WORK *pl_w );
static Uint8 dive_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int dive_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl32_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl32_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl32_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( dive_ck( pl_w ) ) return ;
	if( armor_ck( pl_w ) ) return ;
	if( tackle_ck( pl_w ) ) return ;
	if( swing_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			In the air only high enough, and once a jump		*/
/*			unless super jumping								*/
/*==============================================================*/
static Uint8 tackle_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_tackle, &pl_w->com_w0 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( !( pl_w->pos.y > pl_w->ground_y + DOT_Y( 48.0f ) ) ) return 0 ;
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl32_sp_tackle ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 swing_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_swing, &pl_w->com_w1 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl32_sp_swing ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Not while the armour is still on					*/
/*==============================================================*/
static Uint8 armor_ck( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !com_nml_ck( pl_w, com_armor, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( lw->armor_timer ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl32_sp_armor ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Once a jump unless super jumping					*/
/*==============================================================*/
static Uint8 dive_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dive, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl32_sp_dive ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 4 ) ) return 0 ;
	pl_w->pl_sp_type = pl32_sp_chouhatsu ;
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
	pl_w->pl_sp_type = pl32_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/****************************************************************/
/*			A hyper combo entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl32_delay_ck( PLAYER_WORK *pl_w )
{
	if( dive_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int dive_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dive, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = pl32_sp_dive ;
	return 1 ;
}

static void idle_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );
static void voice_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );
static void armor_glow( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );
static void super_glow( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );
static void armor_end_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );
static void hit_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/****************************************************************/
void pl32_main( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	idle_ck( pl_w, lw ) ;
	voice_ck( pl_w, lw ) ;
	armor_glow( pl_w, lw ) ;
	super_glow( pl_w, lw ) ;
	armor_end_ck( pl_w, lw ) ;
	hit_ck( pl_w, lw ) ;
}

/*==============================================================*/
/*			Now and then a sound while standing still			*/
/*==============================================================*/
static void voice_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( !pl_w->char_no.b.grp ){
		if( !( ++ lw->voice_cnt & 15 ) ){
			if( !( Rnd() & 7 ) ) pls32_00_set( pl_w, 0, 0 ) ;
		}
	}
}

/*==============================================================*/
/*			The armour's glow, until it runs out or he is		*/
/*			knocked out											*/
/*==============================================================*/
static void armor_glow( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_w->armor_flag ){
		if( !pl_w->vital_new ) lw->armor_timer = 0 ;
		if( lw->armor_timer ){
			lw->armor_timer -- ;
			ef_color_set( pl_w, 0, lw->armor_col + pl_w->pl_color * 32, 1 ) ;
			if( lw->armor_col < 7 ) lw->armor_col ++ ;
			else lw->armor_col = 0 ;
		} else {
			pl_color_set( pl_w, PC_DEFAULT ) ;
			pl_w->armor_flag = 0 ;
		}
	}
}

/*==============================================================*/
/*			Now and then, while standing, the glow starts		*/
/*==============================================================*/
static void idle_wait( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( !( lw->frame_cnt & 3 ) && !( Rnd() & 63 ) && !pl_w->routine_1 ){
		switch( pl_w->act_st ){
		case N_STAND: case N_WALK: case N_JUMP: case N_CROUCHING: case N_DASH: case N_BACK_DASH:
			break ;
		default:
			return ;
		}
		lw->idle_step ++ ;
		lw->glow_step = 0 ;
	}
}

/*==============================================================*/
static void idle_glow( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static Uint8 glow_tbl[] = { 3, 0, 1, 2, 3, 0xff } ;

	if( glow_tbl[lw->glow_step] != 0xff ) ef_color_set( pl_w, 0, pl_w->pl_color * 32 + glow_tbl[lw->glow_step] + 7, 1 ) ;
	else {
		pl_color_set( pl_w, PC_DEFAULT ) ;
		lw->idle_step = 0 ;
	}
	if( !( lw->frame_cnt & 3 ) ) lw->glow_step ++ ;
}

/*==============================================================*/
static void idle_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *idle_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		idle_wait, idle_glow,
	} ;

	if( pl_w->char_no.w != 0x1c000 && pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER ){
		lw->frame_cnt ++ ;
		idle_jmp[lw->idle_step]( pl_w, lw ) ;
	}
}

/*==============================================================*/
/*			The glow of a hyper combo							*/
/*==============================================================*/
static void super_glow( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( lw->dive_flag ){
		if( pl_w->act_st != N_SUPER ){
			lw->dive_flag = 0 ;
			pl_color_set( pl_w, PC_DEFAULT ) ;
		} else if( !lw->dive_timer -- ){
			lw->dive_timer = 2 ;
			ef_color_set( pl_w, 0, lw->dive_col + pl_w->pl_color * 32 + 15, 1 ) ;
			if( lw->dive_col < 7 ) lw->dive_col ++ ;
			else lw->dive_col = 0 ;
		}
	}
}

/****************************************************************/
/*			A hit while the armour is on only slows him down	*/
/****************************************************************/
int pl32_armor_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;
	Float spd ;
	int n ;

	if( pl_w->pl_type != PL_COLOSSUS || !( em->atck_flag & HIT_KIND_MASK ) || !lw->sp_armor_flag ) return 0 ;
	if( lw->sp_armor_flag > 0 ){
		if( em->hit_data_adrs->atck_kind & A_HYPER ) return 0 ;
		n = 2 ;
	} else n = 4 ;
	spd = pl_w->spd.x ;
	if( spd < 0.0f ) spd = -spd ;
	if( spd > DOT_X( 2.0f ) ) pl_w->spd.x /= n ;
	else {
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
	}
	return 1 ;
}

/*==============================================================*/
/*			The armour of a special move lasts only while it	*/
/*			does												*/
/*==============================================================*/
static void armor_end_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( !lw->sp_armor_flag ) return ;
	if( pl_w->routine_1 != PL_NORM || pl_w->act_st != N_SPECIAL ){
		lw->sp_armor_flag = 0 ;
		return ;
	}
	if( pl_w->pl_sp_type ) lw->sp_armor_flag = 0 ;
}

/*==============================================================*/
/*			Shakes the stage when a hit lands on a standing		*/
/*			opponent											*/
/*==============================================================*/
static void hit_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_w->cg_flag1 && pl_w->hit_flag ){
		pl_w->cg_flag1 = 0 ;
		em = pl_w->em_w ;
		if( !em->work_kind ){
			scr_work.quake_level = 3 ;
			scr_work.quake_flag = 1 ;
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
void pl32_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	pl_w->atck_pow = 0 ;
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
/*			The heavy punch with the stick forward is			*/
/*			another attack										*/
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
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		if( pl_w->atck_lever & KEY_FORWARD ){
			pl_w->char_no.b.no = 5 ;
			pl_w->atck_no = pl32_at_jfwd_hp ;
		}
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
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
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
void pl32_atck( PLAYER_WORK *pl_w )
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
/*			The heavy punch glows, and makes a sound			*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
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
		if( pl_w->cg_prm ) ef_color_set( pl_w, 0, pl_w->cg_prm + pl_w->pl_color * 32 + 22, 1 ) ;
		else pl_color_set( pl_w, PC_DEFAULT ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pls32_00_set( pl_w, 1, 0 ) ;
		}
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
void pl32_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		pl_w->atck_no = pl32_at_upper ;
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

/****************************************************************/
void pl32_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move,
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
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		pl_w->timer0 = 24 ;
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
	if( !-- pl_w->timer0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop								*/
/****************************************************************/
void pl32_noki( PLAYER_WORK *pl_w )
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
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->spd.x = DOT_X( 10.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 3.0f ) ;
		pl_w->acc.y = -0.60267854f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
		pl_w->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_fall( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );

/****************************************************************/
/*			He drops into the stage and lands with a shake		*/
/****************************************************************/
void pl32_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_fall, appear_wait,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_w->pos.y += 480.0f ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 4.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_se_req( pl_w, 53 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ) pl_w->routine_1 ++ ;
}

/*==============================================================*/
/*			The win poses, each ending the armour				*/
/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !( Rnd() & 1 ) ) pl_w->char_no.b.no = 0 ;
	else pl_w->char_no.b.no = 1 ;
	pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
	pl_w->armor_flag = 0 ;
	lw->armor_timer = 0 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move,
	} ;

	win_ko_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( !( Rnd() & 1 ) ) pl_w->char_no.b.no = 2 ;
		else pl_w->char_no.b.no = 3 ;
		pl_char_set( pl_w, Char_Win, pl_w->char_no.b.no ) ;
		pl_w->armor_flag = 0 ;
		lw->armor_timer = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void win_time( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
		pl_w->armor_flag = 0 ;
		lw->armor_timer = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	} else pl_char_move( pl_w ) ;
}

/*==============================================================*/
static void draw_time( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Win, 4 ) ;
		pl_w->armor_flag = 0 ;
		lw->armor_timer = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
	} else pl_char_move( pl_w ) ;
}

/****************************************************************/
void pl32_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko, lose_time_ko, win_time, lose_time_ko, draw_time,
	} ;

	if( pl_win_end_ck( pl_w ) ) pl_stand_ret( pl_w ) ;
	else win_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			The partner's jump in								*/
/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.x = -30.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl32_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
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
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
		pl_w->timer0 = 40 ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stop_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
void pl32_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

static void tackle_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			A charge that takes hits, on the ground or down		*/
/*			from the air										*/
/*==============================================================*/
static void tackle_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	/* speed and its change, x then y, in 1/65536ths, for each of sub_type's kinds */
	static Sint32 spd_tbl[16] = {
		-0x000a0000,  0x00001000,  0x00000000,  0x00000000,
		-0x00080000,  0x00000000,  0x000a0000, -0x00001000,
		-0x000a0000,  0x00001000,  0x00000000,  0x00000000,
		-0x00080000,  0x00000000, -0x00080000, -0x00001000,
	} ;

	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
		pl_w->pos.y = pl_w->ground_y ;
		if( !pl_w->atck_level ) pl_w->sub_type = 0 ;
		else pl_w->sub_type = 1 ;
	} else {
		if( !pl_w->atck_level ) pl_w->sub_type = 2 ;
		else pl_w->sub_type = 3 ;
	}
	pl_w->atck_no = pl_w->atck_level + 48 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
	pl_w->spd.x = spd_tbl[pl_w->sub_type * 4 + 0] * 1.6666666f / 65536.0f ;
	pl_w->acc.x = spd_tbl[pl_w->sub_type * 4 + 1] * 1.6666666f / 65536.0f ;
	pl_w->spd.y = spd_tbl[pl_w->sub_type * 4 + 2] * 2.142857f / 65536.0f ;
	pl_w->acc.y = spd_tbl[pl_w->sub_type * 4 + 3] * 2.142857f / 65536.0f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	lw->sp_armor_flag = 0 ;
	tackle_move( pl_w, lw ) ;
}

/*==============================================================*/
/*			The armour comes on with the animation				*/
/*==============================================================*/
static void tackle_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		lw->sp_armor_flag = 1 ;
		pl_w->timer1 = 0 ;
		if( pl_w->sub_type == 1 ) pl_air_set( pl_w ) ;
	}
}

/*==============================================================*/
/*			Ends when the animation does, or when the stick		*/
/*			is pulled back										*/
/*==============================================================*/
static void tackle_rush( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	int end ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	end = 0 ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	if( !pl_w->pl_dir ){
		if( pl_w->wall_flag == WALL_LEFT ) end = 1 ;
	} else if( pl_w->wall_flag == WALL_RIGHT ) end = 1 ;
	if( pl_char_move( pl_w ) < 0 || end == 1 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->spd.x / 4.0f ;
		pl_w->acc.x = pl_w->acc.x / 4.0f ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_w->char_no.b.no = 2 ;
		else {
			pl_w->char_no.b.no = 3 ;
			pl_w->spd.y = pl_w->spd.y / 4.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
		lw->sp_armor_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void tackle_end( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_char_move( pl_w ) ;
		if( pl_w->ground_y >= pl_w->pos.y ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_stand_ret( pl_w ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_tackle( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *tackle_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		tackle_init, tackle_move, tackle_rush, tackle_end,
	} ;

	tackle_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void swing_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			Giant Swing: a rush that grabs the opponent it		*/
/*			hits; mashing makes the throw stronger				*/
/*==============================================================*/
static void swing_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_sp_ef_set( pl_w ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
	}
	pl_w->atck_no = pl32_at_swing ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.x = -DOT_X( 8.0f ) ;
	pl_w->acc.x = 0.46875f ;
	if( pl_w->pl_dir ){
		pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = -pl_w->acc.x ;
	}
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 4 ) ;
	swing_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void swing_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = !pl_w->atck_level ? 10 : 20 ;
	}
}

/*--------------------------------------------------------------*/
static void swing_rush( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_sp_catch_ck( pl_w ) ){
		if( pl_w->pl_stance != STANCE_AIR ) pl_w->catch_no = 0 ;
		else pl_w->catch_no = 4 ;
		pl_w->catch_no |= CATCH_ESC_NG ;
		pl_w->catch_no |= CATCH_ESC2_NG ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		em = pl_w->em_w ;
		em->spd.x = em->spd.y = em->acc.x = em->acc.y = 0.0f ;
		pl_catch_set( pl_w, em ) ;
		return ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance != STANCE_AIR ) pl_w->char_no.b.no = pl_w->atck_level + 6 ;
		else pl_w->char_no.b.no = pl_w->atck_level + 9 ;
		pl_char_set( pl_w, Char_Special, pl_w->char_no.b.no ) ;
		pl_w->spd.x /= 2.0f ;
		pl_w->acc.x = pl_w->acc.x / 8.0f ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void swing_end( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_char_move( pl_w ) ;
		if( pl_w->ground_y >= pl_w->pos.y ){
			pl_w->spd.y = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_stand_ret( pl_w ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_swing( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *swing_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		swing_init, swing_move, swing_rush, swing_end,
	} ;

	swing_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void sp_armor_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			The armour, for 600 frames							*/
/*==============================================================*/
static void sp_armor_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Super, 3 ) ;
	pl_w->timer0 = 60 ;
	sp_armor_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void sp_armor_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->armor_flag = ARMOR_ON ;
		lw->armor_timer = 600 ;
		lw->armor_col = 0 ;
		Timer_Gauge_Set( pl_w, &lw->armor_timer, 7 ) ;
		ofs.x = -DOT_X( 13.0f ) ;
		ofs.y = 210.0f ;
		ofs.z = 0.0f ;
		pl_hyper_flash( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void armor_end( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_armor( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *armor_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		sp_armor_init, sp_armor_move, armor_end,
	} ;

	armor_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void dive_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			Up off the screen, then down at an angle			*/
/*==============================================================*/
static void dive_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
	}
	lw->dive_flag = 0 ;
	lw->dive_timer = 1 ;
	lw->dive_col = 0 ;
	pl_w->atck_no = pl32_at_dive ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->spd.y = DOT_Y( 12.0f ) ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
	pl_w->timer0 = 50 ;
	dive_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void dive_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		ofs.x = -DOT_X( 13.0f ) ;
		ofs.y = DOT_Y( 104.0f ) ;
		ofs.z = 0.0f ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*==============================================================*/
/*			Rises until high enough with the stick up and a		*/
/*			button held, or the time runs out					*/
/*==============================================================*/
static void dive_rise( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->timer0 == 50 ) pl_voice_req( pl_w, 31 ) ;
	if( pl_w->pos.y >= pl_w->ground_y + DOT_Y( 192.0f ) && ( pl_w->pl_sw_new & KEY_DOWN ) && ( pl_w->pl_sw_now & KEY_ATCK ) ) pl_w->timer0 = 0 ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		lw->dive_flag = 1 ;
		pl_char_set( pl_w, Char_Super, 1 ) ;
		pl_w->atck_no = pl32_at_dive_2 ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		pl_w->spd.x = -DOT_X( 8.0f ) ;
		pl_w->spd.y = -DOT_Y( 12.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_voice_req( pl_w, 43 ) ;
		pl_voice_req( pl_w, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dive_dive( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y + -DOT_Y( 32.0f ) ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		lw->dive_flag = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 2 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		pl_se_req( pl_w, 49 ) ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void dive_end( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_dive( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		dive_init, dive_move, dive_rise, dive_dive, dive_end,
	} ;

	dive_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void chouhatsu_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	chouhatsu_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void snapback_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
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
	pl_w->atck_no = pl32_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 8 ) ;
	snapback_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -5.0f ;
		ofs.y = DOT_Y( 74.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

static void stand_hk_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			The assist's kick									*/
/*==============================================================*/
static void stand_hk_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = ATCK_STAND_HK ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, 2 ) ;
	stand_hk_move( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void stand_hk_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_stand_hk( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static void ( *stand_hk_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		stand_hk_init, stand_hk_move,
	} ;

	stand_hk_jmp[pl_w->routine_2]( pl_w, lw ) ;
}

/****************************************************************/
void pl32_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		sp_tackle, sp_swing, sp_armor, sp_dive, sp_chouhatsu, sp_snapback, sp_stand_hk,
	} ;

	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_w->atck_pow = 0 ;
	pl_sp_tbl[pl_w->pl_sp_type]( pl_w, lw ) ;
}

/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 2 ;
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
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
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
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl32_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, crouch_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

static void mash_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			The rush's grab, on the ground						*/
/*==============================================================*/
static void rush_set( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
	mash_init( pl_w, lw ) ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 83.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->timer0 = 60 ;
	pls32_00_set( pl_w, 2, 0 ) ;
	pls32_00_set( pl_w, 2, 1 ) ;
	pl_w->target_w->sub_type = 1 ;
}

/*==============================================================*/
static void nml_k_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 3 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 100.0f ) ;
	ofs.y = DOT_Y( 15.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pls32_00_set( pl_w, 4, 0 ) ;
}

/*==============================================================*/
static void nml_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 2 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 6 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -DOT_X( 100.0f ) ;
	ofs.y = DOT_Y( 15.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
static void jump_p_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 7 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ofs.x = -25.0f ;
	ofs.y = DOT_Y( 88.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
/*			The rush's grab, in the air							*/
/*==============================================================*/
static void rush_air_set( PLAYER_WORK *pl_w )
{
	PL32_LOCAL_W *lw = ( PL32_LOCAL_W * )&pl_w->tlocal[0] ;
	VEC3 ofs ;

	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
	mash_init( pl_w, lw ) ;
	ofs.x = -85.0f ;
	ofs.y = DOT_Y( 83.0f ) ;
	ofs.z = 0.0f ;
	nage_hit_set( pl_w, &ofs ) ;
	catch_gauge_add( pl_w ) ;
	pl_w->timer0 = 60 ;
	pl_w->spd.y = DOT_Y( 0.5f ) ;
	pl_w->acc.y = 0.0f ;
	pls32_00_set( pl_w, 3, 0 ) ;
	pls32_00_set( pl_w, 3, 1 ) ;
}

/****************************************************************/
/*			Start the throw that pl32_catch_ck chose			*/
/****************************************************************/
void	pl32_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		rush_set, nml_k_set, nml_p_set, jump_p_set, rush_air_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static int mash_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw );

/*==============================================================*/
/*			The opponent is held while the player mashes,		*/
/*			then thrown up										*/
/*==============================================================*/
static void rush_hold( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	/* the throw's speed up, by how hard it was mashed */
	static Uint8 rise_tbl[8] = { 7, 8, 9, 10, 11, 12, 13, 14 } ;

	pl_w->target_w->sub_type = 1 ;
	if( mash_ck( pl_w, lw ) ) pl_char_set2( pl_w, Char_Catch, 0, lw->mash_flip ) ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
		pl_w->target_w->sub_type = 0 ;
		pl_w->spd.y = ( rise_tbl[lw->mash_lv] << 16 ) * 2.142857f / 65536.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pls32_00_set( pl_w, 3, 0 ) ;
		pls32_00_set( pl_w, 3, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void rush_rise( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->target_w->sub_type = 0 ;
	if( mash_ck( pl_w, lw ) ) pl_char_set2( pl_w, Char_Catch, 1, lw->mash_flip ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y * pl_w->acc.y >= 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
		pl_w->spd.y = 2.142857f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*==============================================================*/
/*			At the top the opponent is thrown down				*/
/*==============================================================*/
static void rush_top( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static Uint8 toss_tbl[8] = { 0x20, 0x20, 0x20, 0x28, 0x28, 0x28, 0x30, 0x30 } ;
	PLAYER_WORK *em ;

	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		pl_w->routine_2 ++ ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		return ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		em = pl_w->catch_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		pl_w->atck_pow = toss_tbl[lw->mash_lv] ;
		em->atck_no = pl32_at_swing_throw ;
		pl_w->atck_no = pl32_at_swing_throw ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
	}
}

/*--------------------------------------------------------------*/
static void rush_land( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		scr_excep_clr() ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
static void rush_catch( PLAYER_WORK *pl_w )
{
	static void ( *rush_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		rush_hold, rush_rise, rush_top, rush_land,
	} ;

	rush_jmp[pl_w->routine_2]( pl_w, ( PL32_LOCAL_W * )&pl_w->tlocal[0] ) ;
}

/*==============================================================*/
static void mash_init( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	lw->step_timer = 8 ;
	lw->mash_lv = 1 ;
	lw->mash_flip = 0 ;
	lw->mash_flag = 0 ;
	lw->cpu_timer = 0 ;
	lw->team_timer = 0 ;
}

/*==============================================================*/
/*			Mashing makes the throw stronger; true at each		*/
/*			step												*/
/*==============================================================*/
static int mash_ck( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	static Sint8 step_tbl[8] = { 8, 7, 6, 5, 4, 3, 2, 1 } ;
	static Sint8 com_tbl[4] = { 8, 6, 4, 2 } ;

	if( pl_w->pl_sw_now & KEY_LEVER ) lw->mash_flag = 1 ;
	if( pl_w->cpu_flag ){
		if( !lw->cpu_timer -- ){
			lw->cpu_timer = com_tbl[pl_w->atck_pk] ;
			lw->mash_flag = 1 ;
		}
	}
	if( pl_w->team_no ){
		if( !lw->team_timer -- ){
			lw->team_timer = 6 ;
			lw->mash_flag = 1 ;
		}
	}
	if( !lw->step_timer -- ){
		if( lw->mash_flag ){
			if( lw->mash_lv < 7 ) lw->mash_lv ++ ;
		} else if( lw->mash_lv > 0 ) lw->mash_lv -- ;
		lw->step_timer = step_tbl[lw->mash_lv] ;
		lw->mash_flag = 0 ;
		lw->mash_flip ++ ;
		lw->mash_flip &= 1 ;
		lw->mash_lv_copy[0] = lw->mash_lv ;
		if( lw->mash_flip ) pl_voice_req( pl_w, 30 ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void nml_k_jump( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = -5.0f ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.875f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_fall( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
		em = pl_w->catch_w ;
		em->catch_type = 2 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl32_at_throw_k ;
		pl_w->atck_no = pl32_at_throw_k ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		catch_se_req( em ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_k_end( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_k_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_k_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		nml_k_jump, nml_k_fall, nml_k_end,
	} ;

	nml_k_jmp[pl_w->routine_2]( pl_w, ( PL32_LOCAL_W * )&pl_w->tlocal[0] ) ;
}

/*==============================================================*/
static void nml_p_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->catch_type = 16 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl32_at_throw_p ;
		pl_w->atck_no = pl32_at_throw_p ;
		em->pl_dir = pl_w->pl_dir ;
	}
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_p_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		nml_p_move,
	} ;

	nml_p_jmp[pl_w->routine_2]( pl_w, ( PL32_LOCAL_W * )&pl_w->tlocal[0] ) ;
}

/*==============================================================*/
static void jump_p_move( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_fall( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 8 ) ;
		em = pl_w->catch_w ;
		em->catch_type = 2 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl32_at_air_throw ;
		pl_w->atck_no = pl32_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ;
		em->pl_dir ^= 1 ;
		catch_se_req( em ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_p_end( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_p_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_p_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		jump_p_move, jump_p_fall, jump_p_end,
	} ;

	jump_p_jmp[pl_w->routine_2]( pl_w, ( PL32_LOCAL_W * )&pl_w->tlocal[0] ) ;
}

/*==============================================================*/
static void rush_air_hold( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw )
{
	if( mash_ck( pl_w, lw ) ) pl_char_set2( pl_w, Char_Catch, 1, lw->mash_flip ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->timer0 -- ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 2 ) ;
		pl_w->spd.y = 2.142857f ;
		pl_w->acc.y = 0.0f ;
	}
}

/*==============================================================*/
static void rush_air_catch( PLAYER_WORK *pl_w )
{
	static void ( *rush_air_jmp[])( PLAYER_WORK *pl_w, PL32_LOCAL_W *lw ) = {
		rush_air_hold, rush_top, rush_land,
	} ;

	rush_air_jmp[pl_w->routine_2]( pl_w, ( PL32_LOCAL_W * )&pl_w->tlocal[0] ) ;
}

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl32_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		rush_catch, nml_k_catch, nml_p_catch, jump_p_catch, rush_air_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/* where the rush's grab holds the opponent */
typedef struct {
	Sint16	x, y ;		/* the offset from the opponent */
	Sint8	no ;		/* the animation */
	Uint8	dir ;		/* 1 to face the other way */
	Sint8	pri ;		/* above zero to be drawn in front */
	Uint8	rot ;
} HOLD_POS ;

/*==============================================================*/
/*			The rush's grab carries the opponent along			*/
/*==============================================================*/
static void rush_hold_pos( PLAYER_WORK *pl_w )
{
	static HOLD_POS hold_tbl[][3] = {
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   80,   13,  13, 0,  1, 0xc2 }, { -108,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   94,   30,  13, 0,  1, 0xc2 }, { -100,   32,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  103,   58,  13, 0,  1, 0xc2 }, { -101,   54,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  109,    9,  13, 0,  1, 0xc2 }, { -113,   20,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   96,   37,  13, 0,  1, 0xc2 }, {  -84,   27,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  101,   24,  13, 0,  1, 0xc2 }, { -100,   34,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   80,   46,  13, 0,  1, 0xc2 }, {  -94,   36,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  105,   18,  13, 0,  1, 0xc2 }, { -100,   18,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   85,   42,  13, 0,  1, 0xc2 }, { -101,   38,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  110,   46,  13, 0,  1, 0xc2 }, {  -94,   26,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   92,   24,  13, 0,  1, 0xc2 }, { -102,   30,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  104,   50,  13, 0,  1, 0xc2 }, { -104,   18,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  112,   -5,  13, 0,  1, 0xc2 }, { -102,    0,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  123,    0,  13, 0,  1, 0xc2 }, { -101,  -10,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  119,    5,  13, 0,  1, 0xc2 }, { -104,    6,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  114,   42,  13, 0,  1, 0xc2 }, { -108,   42,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   97,   40,  13, 0,  1, 0xc2 }, { -106,   36,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   98,   29,  13, 0,  1, 0xc2 }, { -102,   17,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  104,   34,  13, 0,  1, 0xc2 }, { -103,   28,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   92,   38,  13, 0,  1, 0xc2 }, {  -97,   28,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  104,   35,  13, 0,  1, 0xc2 }, {  -99,   16,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   92,   24,  13, 0,  1, 0xc2 }, { -100,   20,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   96,   16,  13, 0,  1, 0xc2 }, { -100,    7,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  101,   49,  13, 0,  1, 0xc2 }, {  -97,   58,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   97,   28,  13, 0,  1, 0xc2 }, {  -92,   34,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   83,   55,  13, 0,  1, 0xc2 }, {  -92,   48,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   91,   36,  13, 0,  1, 0xc2 }, {  -96,   50,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  100,   48,  13, 0,  1, 0xc2 }, {  -86,   52,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   74,   35,  13, 0,  1, 0xc2 }, {  -90,   13,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   96,   48,  13, 0,  1, 0xc2 }, {  -92,   44,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   92,   40,  13, 0,  1, 0xc2 }, { -100,   39,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   80,   36,  13, 0,  1, 0xc2 }, {  -92,   32,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   99,   22,  13, 0,  1, 0xc2 }, {  -99,    5,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  112,   22,  13, 0,  1, 0xc2 }, { -109,   21,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  108,    8,  13, 0,  1, 0xc2 }, { -109,    9,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  104,   27,  13, 0,  1, 0xc2 }, {  -99,   19,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  126,   -8,  13, 0,  1, 0xc2 }, { -157,  -32,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  108,   12,  13, 0,  1, 0xc2 }, { -100,   28,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   86,   22,  13, 0,  1, 0xc2 }, { -100,   12,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   87,   28,  13, 0,  1, 0xc2 }, {  -98,   34,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  107,   14,  13, 0,  1, 0xc2 }, {  -99,   13,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   93,   26,  13, 0,  1, 0xc2 }, { -104,   24,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  104,   37,  13, 0,  1, 0xc2 }, {  -98,   34,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  107,   19,  13, 0,  1, 0xc2 }, { -100,   26,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   87,   28,  13, 0,  1, 0xc2 }, {  -98,   34,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  126,   24,  13, 0,  1, 0xc2 }, { -119,    5,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  124,    1,  13, 0,  1, 0xc2 }, { -120,  -18,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  122,    4,  13, 0,  1, 0xc2 }, { -114,  -19,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  112,   34,  13, 0,  1, 0xc2 }, {  -97,   36,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {  100,   49,  13, 0,  1, 0xc2 }, {  -96,   42,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   80,   46,  13, 0,  1, 0xc2 }, {  -94,   36,  14, 1,  1, 0xe4 } },
		{ {    0,    0,   0, 0,  0, 0x00 }, {   75,   66,  13, 0,  1, 0xc2 }, {  -72,   64,  14, 1,  1, 0xe4 } },
	} ;
	HOLD_POS *hp ;
	Sint8 pri ;

	if( pl_w->sub_type ){
		hp = hold_tbl[pl_w->pl_type] ;
		hp += LO_BYTE( pl_w->catch_w->cg_rival ) % 3 ;
		if( pl_w->catch_w->disp_dir ) pl_w->pos.x = pl_w->catch_w->pos.x - hp->x * 1.6666666f ;
		else pl_w->pos.x = pl_w->catch_w->pos.x + DOT_X( hp->x ) ;
		pl_wall_ck( pl_w ) ;
		if( pl_w->wall_flag ){
			pl_w->catch_w->pos.x = pl_w->pos.x ;
			if( pl_w->catch_w->disp_dir ) pl_w->catch_w->pos.x += DOT_X( hp->x ) ;
			else pl_w->catch_w->pos.x -= DOT_X( hp->x ) ;
		}
		pl_w->pos.y = pl_w->catch_w->pos.y + DOT_Y( hp->y ) ;
		pl_w->char_no.b.no = hp->no ;
		pl_w->disp_dir = pl_w->catch_w->disp_dir ^ ( hp->dir & 1 ) ;
		if( hp->pri > 0 ){
			if( pl_w->catch_w->prio > pl_w->prio ){
				pri = pl_w->prio ;
				pl_w->prio = pl_w->catch_w->prio ;
				pl_w->catch_w->prio = pri ;
			}
		} else if( pl_w->catch_w->prio < pl_w->prio ){
			pri = pl_w->prio ;
			pl_w->prio = pl_w->catch_w->prio ;
			pl_w->catch_w->prio = pri ;
		}
		pl_w->rot[2] = pl_w->catch_w->rot[2] + ( Float )( hp->rot << 8 ) ;
		if( LO_BYTE( pl_w->catch_w->cg_rival ) != pl_w->catch_grip_old ){
			pl_char_set( pl_w, Char_Caught, pl_w->char_no.b.no ) ;
			pl_w->catch_grip_old = LO_BYTE( pl_w->catch_w->cg_rival ) ;
		}
		pl_rot_ofs_set( pl_w ) ;
	} else catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl32_hold( PLAYER_WORK *pl_w )
{
	static void ( *hold_jmp[])( PLAYER_WORK *pl_w ) = {
		rush_hold_pos, hold, hold, hold, hold,
	} ;

	hold_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl32_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl32_sp_dive ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl32_sp_dive ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl32_sp_dive ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl32_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl32_sp_dive ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl32_sp_dive ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl32_sp_dive ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl32_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl32_sp_tackle ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl32_sp_tackle ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl32_sp_stand_hk ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl32_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl32_sp_tackle ;
		pl_w->atck_level = 0 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl32_sp_tackle ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl32_sp_stand_hk ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl32.c" End of File	*/
