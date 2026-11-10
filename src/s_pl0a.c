/* Rogue (character 0a): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, the powers she takes from the opponent, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl0a_def.h"

/* what she reads of other characters' own work when she takes their power */
typedef struct PL0B_LOCAL_W {		/* Captain America */
	Uint8	unk_00[0x0e];
	Uint8	shield_st;
} PL0B_LOCAL_W;

typedef struct PL12_LOCAL_W {		/* Hayato */
	Sint16	hyper_timer;
} PL12_LOCAL_W;

typedef struct PL29_LOCAL_W {		/* Juggernaut */
	Uint8	unk_00[0x05];
	Uint8	pw_set;
} PL29_LOCAL_W;

typedef struct PL2D_LOCAL_W {		/* Shuma-Gorath */
	Uint8	unk_00[0x09];
	Sint8	power_step;			/* a power-up is on */
	Uint8	unk_0a[0x10 - 0x0a];
	Uint8	dive_flag;
} PL2D_LOCAL_W;

typedef struct PL2F_LOCAL_W {		/* Silver Samurai */
	Uint8	unk_00[0x1c];
	Sint32	elem;			/* the element */
} PL2F_LOCAL_W;

typedef struct PL31_LOCAL_W {		/* Spiral */
	Uint8	unk_00[0x0e];
	Sint16	glow_timer;
	Uint8	unk_10[0x16 - 0x10];
	Uint8	maxspeed_flag;
} PL31_LOCAL_W;

typedef struct PL37_LOCAL_W {		/* Jin */
	Uint8	unk_00[0x08];
	Uint8	burning_flag;
} PL37_LOCAL_W;

void pl0a_main( PLAYER_WORK *pl_w );
void pl0a_com_ck( PLAYER_WORK *pl_w );
void pl0a_atck_set( PLAYER_WORK *pl_w );
void pl0a_atck( PLAYER_WORK *pl_w );
void pl0a_dash( PLAYER_WORK *pl_w );
void pl0a_noki( PLAYER_WORK *pl_w );
void pl0a_appear( PLAYER_WORK *pl_w );
void pl0a_win( PLAYER_WORK *pl_w );
void pl0a_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl0a_catch_ck( PLAYER_WORK *pl_w );
void pl0a_catch_set( PLAYER_WORK *pl_w );
void pl0a_catch( PLAYER_WORK *pl_w );
void pl0a_hold( PLAYER_WORK *pl_w );
int pl0a_delay_ck( PLAYER_WORK *pl_w );
void pl0a_t_kick( PLAYER_WORK *pl_w );
void pl0a_upper( PLAYER_WORK *pl_w );
void pl0a_sp_unison( PLAYER_WORK *pl_w );
void pl0a_sp_gattai( PLAYER_WORK *pl_w );
void pl0a_sp_assist( PLAYER_WORK *pl_w );
void pl0a_sp_zero( PLAYER_WORK *pl_w );
void pl0a_tlocal_init( PLAYER_WORK *pl_w );
void pl0a_wait_set( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1,  2 } ;
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

static Sint8 *atck_tbl[18] = {
	lp_tbl, mp_tbl, hp_tbl, lk_tbl, mk_tbl, hk_tbl,
	j_lp_tbl, j_mp_tbl, j_hp_tbl, j_lk_tbl, j_mk_tbl, j_hk_tbl,
	sj_lp_tbl, sj_mp_tbl, sj_hp_tbl, sj_lk_tbl, sj_mk_tbl, sj_hk_tbl,
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_repeat[] = { 3, 0, pl0a_sp_repeat, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Repeating Punches */
static Uint16 com_rising[] = { 3, COM_GROUND, pl0a_sp_rising, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Rising Repeating Punches */
static Uint16 com_dive[] = { 3, COM_GROUND, pl0a_sp_dive, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ;	/* Dive Punch */
static Uint16 com_drain[] = { 3, 0, pl0a_sp_drain, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Power Drain */
static Uint16 com_sugah[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl0a_sp_sugah, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Goodnight Sugah (hyper) */
static Uint16 com_screw[] = { 4, COM_GROUND, pl0a_sp_screw, 0, KEY_PUNCH } ;	/* Screw Piledriver */
static Uint16 com_leap[] = { 3, COM_GROUND, pl0a_sp_leap, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Ken's leap */
static Uint16 com_shungoku[] = { 4, COM_GROUND | COM_HYPER, pl0a_sp_shungoku, 0, KEY_LP, KEY_LP, KEY_FORWARD, KEY_LK, KEY_HP } ;	/* Shun Goku Satsu */

static void ( *pl0a_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl0a_main,
	pl0a_com_ck,
	pl0a_atck_set,
	pl0a_atck,
	pl0a_dash,
	pl0a_noki,
	pl0a_appear,
	pl0a_win,
	pl0a_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl0a_catch_ck,	/* returns a value */
	pl0a_catch_set,
	pl0a_catch,
	pl0a_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl0a_delay_ck,
	pl0a_t_kick,
	pl0a_upper,
	pl0a_sp_unison,
	pl0a_sp_gattai,
	pl0a_sp_assist,
	pl0a_sp_zero,
	0,
	0,
	0,
	pl0a_tlocal_init,
	pl0a_wait_set,
} ;

static int shungoku_ck( PLAYER_WORK *pl_w );
static int sugah_ck( PLAYER_WORK *pl_w );
static int screw_ck( PLAYER_WORK *pl_w );
static int rising_ck( PLAYER_WORK *pl_w );
static int dive_ck( PLAYER_WORK *pl_w );
static int drain_ck( PLAYER_WORK *pl_w );
static int repeat_ck( PLAYER_WORK *pl_w );
static int leap_ck( PLAYER_WORK *pl_w );
static int chouhatsu_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static int sugah_delay_ck( PLAYER_WORK *pl_w );
static void power_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void stand_ret( PLAYER_WORK *pl_w );
static void sp_stand_set( PLAYER_WORK *pl_w );
static void land_set( PLAYER_WORK *pl_w );
static void power_effect( PLAYER_WORK *pl_w );
static void power_wait( PLAYER_WORK *pl_w );
static void hit_set( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0a_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl0a_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl0a_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( shungoku_ck( pl_w ) ) return ;
	if( sugah_ck( pl_w ) ) return ;
	if( screw_ck( pl_w ) ) return ;
	if( rising_ck( pl_w ) ) return ;
	if( dive_ck( pl_w ) ) return ;
	if( drain_ck( pl_w ) ) return ;
	if( repeat_ck( pl_w ) ) return ;
	if( leap_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	if( pl_air_dash_ck( pl_w, &pl_w->com_w5 ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static int repeat_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_repeat, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->pl_sp_type = pl0a_sp_repeat ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A move of the character whose power she has:		*/
/*			Ken's and Wolverine's, or Cammy's					*/
/*==============================================================*/
static int leap_ck( PLAYER_WORK *pl_w )
{
	Uint8 sp ;

	if( !com_nml_ck( pl_w, com_leap, &pl_w->com_w7 )
	 || ( PL0A_LW( pl_w )->power_type != 0x27 && PL0A_LW( pl_w )->power_type != 0x07
	   && PL0A_LW( pl_w )->power_type != 0x39 && PL0A_LW( pl_w )->power_type != 0x24 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	sp = pl0a_sp_leap ;
	if( PL0A_LW( pl_w )->power_type == 0x07 ) sp = pl0a_sp_barrage ;
	if( PL0A_LW( pl_w )->power_type == 0x39 ) sp = pl0a_sp_barrage ;
	if( PL0A_LW( pl_w )->power_type == 0x24 ) sp = pl0a_sp_spike ;
	pl_w->pl_sp_type = sp ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int rising_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_rising, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->pl_sp_type = pl0a_sp_rising ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int dive_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dive, &pl_w->com_w4 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->pl_sp_type = pl0a_sp_dive ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int drain_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_drain, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->pl_sp_type = pl0a_sp_drain ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, pl0a_sp_chouhatsu ) ) return 0 ;
	pl_w->pl_sp_type = pl0a_sp_chouhatsu ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int sugah_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sugah, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl0a_sp_sugah ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Akuma's, with three levels of the gauge				*/
/*==============================================================*/
static int shungoku_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_shungoku, &pl_w->com_w8 ) || PL0A_LW( pl_w )->power_type != 0x1e ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->pl_sp_type = pl0a_sp_shungoku ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl0a_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			Zangief's											*/
/*==============================================================*/
static int screw_ck( PLAYER_WORK *pl_w )
{
	if( !com_rot_ck( pl_w, com_screw, &pl_w->com_w6 ) || PL0A_LW( pl_w )->power_type != 0x01 ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->pl_sp_type = pl0a_sp_screw ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl0a_delay_ck( PLAYER_WORK *pl_w )
{
	if( sugah_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int sugah_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_sugah, &pl_w->com_w3 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->delay_sp_type = pl0a_sp_sugah ;
	return 1 ;
}

/****************************************************************/
/*			Counts the powers' timers down, and gives her		*/
/*			own back when one runs out							*/
/****************************************************************/
void pl0a_main( PLAYER_WORK *pl_w )
{
	int old ;
	int no ;
	int act ;

	if( !pl_w->hit_stop ){
		if( PL0A_LW( pl_w )->glow_timer ) PL0A_LW( pl_w )->glow_timer -- ;
		if( pl_w->tlocal[TM_WAIT] > 0 ){
			pl_w->tlocal[TM_WAIT] -- ;
			return ;
		}
		if( pl_w->tlocal[TM_POWER_1] > 0 ){
			if( -- pl_w->tlocal[TM_POWER_1] <= 0 ) goto lose ;
		}
		if( pl_w->tlocal[TM_POWER_2] > 0 ){
			if( -- pl_w->tlocal[TM_POWER_2] <= 0 ) goto lose ;
		}
		if( pl_w->tlocal[TM_POWER_3] > 0 ){
			pl_w->tlocal[TM_POWER_3] -- ;
			pl_w->speed_up_timer = 3 ;
		}
		if( pl_w->tlocal[TM_POWER_5] > 0 && !pl_w->ko_flag ){
			pl_w->tlocal[TM_POWER_5] -- ;
			if( !( pl_w->tlocal[TM_POWER_5] & 7 ) ) vital_add_dmg( pl_w, 1 ) ;
		}
		if( pl_w->tlocal[TM_POWER_4] > 0 ){
			pl_w->tlocal[TM_POWER_4] -- ;
			pl_w->armor_timer = 3 ;
		}
		if( pl_w->tlocal[TM_POWER_9] > 0 ){
			pl_w->tlocal[TM_POWER_9] -- ;
			pl_w->armor_timer = 3 ;
		}
		if( pl_w->tlocal[TM_POWER_8] > 0 ){
			if( -- pl_w->tlocal[TM_POWER_8] <= 0 ) goto lose ;
		}
		if( pl_w->tlocal[TM_POWER_7] > 0 ){
			if( -- pl_w->tlocal[TM_POWER_7] <= 0 ) goto lose ;
		}
		return ;
	lose:
		PL0A_LW( pl_w )->power_type = pl_w->pl_type ;
		power_set( pl_w, pl_w ) ;
		if( !pl_w->char_no.b.grp ){
			act = pl_w->char_no.b.no ;
			if( act == 8 || act == 9 || act == 1 ){
				no = HI_BYTE( pl_w->cg_rival ) - 64 ;
				if( no < 0 ) no = 0 ;
				old = pl_w->cg_ctr ;
				pl_char_set( pl_w, Char_Stand, no ) ;
				if( pl_w->cg_ctr > old ) pl_w->cg_ctr = old ;
			}
		}
	} else if( !pl_w->tlocal[TM_WAIT] ){
		if( pl_w->tlocal[TM_POWER_4] > 0 ) pl_w->armor_timer = 3 ;
	}
}

static void nml_atck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Normal attacks: nml_no is the strength, then		*/
/*			kicks and crouching ones follow						*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	static Uint8 atck_no[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } ;
	static Uint8 atck_level[3] = { 0, 1, 2 } ;
	static Uint8 char_no[12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 } ;
	static Uint8 char_group[12] = { Char_Stand_P, Char_Stand_P, Char_Stand_P, Char_Stand_K, Char_Stand_K, Char_Stand_K, Char_Crouch_P, Char_Crouch_P, Char_Crouch_P, Char_Crouch_K, Char_Crouch_K, Char_Crouch_K } ;
	static Uint8 atck_move[12] = { 0, 0, 0, 0, 3, 4, 0, 1, 2, 0, 0, 0 } ;
	Uint8 no ;

	pl_brake_set( pl_w ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->pl_stance == STANCE_CROUCH ) no += 6 ;
	if( pl_w->atck_pk ) no += 3 ;
	PL0A_LW( pl_w )->nml_move = atck_move[no] ;
	pl_w->chain_tbl = atck_tbl[no % 6] ;
	pl_w->nml_level = atck_level[pl_w->nml_no] ;
	pl_w->atck_no = atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, char_group[no], char_no[no] ) ;
	/* Juggernaut's: the heavy punch held forward */
	if( no == 2 ){
		if( PL0A_LW( pl_w )->power_type == 0x29 ){
			if( pl_w->pl_sw_new & KEY_FORWARD ){
				PL0A_LW( pl_w )->nml_move = 5 ;
				pl_w->timer0 = 60 ;
				pl_char_set( pl_w, Char_Stand_P, 3 ) ;
			}
		}
	}
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	static Uint8 atck_no[6] = { 12, 13, 14, 15, 16, 17 } ;
	static Uint8 atck_level[3] = { 0, 1, 2 } ;
	static Uint8 char_no[6] = { 0, 1, 2, 0, 1, 2 } ;
	static Uint8 char_group[6] = { Char_Jump_P, Char_Jump_P, Char_Jump_P, Char_Jump_K, Char_Jump_K, Char_Jump_K } ;
	Uint8 no ;

	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	no = pl_w->nml_no ;
	if( pl_w->atck_pk ) no += 3 ;
	PL0A_LW( pl_w )->nml_move = 0 ;
	pl_w->nml_level = atck_level[pl_w->nml_no] ;
	pl_w->atck_no = atck_no[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, char_group[no], char_no[no] ) ;
	/* the heavy kick held down: a dive */
	if( no == 5 ){
		if( pl_w->atck_lever & KEY_DOWN ){
			PL0A_LW( pl_w )->nml_move = 1 ;
			pl_w->jump_atk_cnt &= JATK_P_MASK ;
			pl_char_set( pl_w, Char_Jump_K, 5 ) ;
		}
	}
	if( pl_w->sjump_flag ) no += 6 ;
	pl_w->chain_tbl = atck_tbl[6 + no] ;
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
	}
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->atck_pk ){
		if( pl_w->jump_atk_cnt & JATK_P_MASK ) jump_atck( pl_w ) ;
	} else {
		if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
	}
}

/****************************************************************/
void pl0a_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

static void jump_move( PLAYER_WORK *pl_w );
static void jump_dive( PLAYER_WORK *pl_w );
static void jump_dive_start( PLAYER_WORK *pl_w );
static void jump_dive_fall( PLAYER_WORK *pl_w );
static void jump_dive_end( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void nml_move( PLAYER_WORK *pl_w );
static void nml_move_none( PLAYER_WORK *pl_w );
static void nml_move_slide( PLAYER_WORK *pl_w );
static void nml_move_step( PLAYER_WORK *pl_w );
static void nml_move_step_mk( PLAYER_WORK *pl_w );
static void nml_move_steps( PLAYER_WORK *pl_w );
static void nml_move_charge( PLAYER_WORK *pl_w );
static void nml_move_dash( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( !PL0A_LW( pl_w )->nml_move ){
		if( pl_char_move( pl_w ) < 0 ){
			pl_jump_ret( pl_w ) ;
			return ;
		}
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
	} else jump_dive( pl_w ) ;
}

/*==============================================================*/
/*			The heavy kick held down: a dive to the ground		*/
/*==============================================================*/
static void jump_dive( PLAYER_WORK *pl_w )
{
	static void ( *jump_dive_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_dive_start, jump_dive_fall, jump_dive_end,
	} ;

	jump_dive_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void jump_dive_start( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->sjump_flag = 0 ;
		spd = -DOT_X( 8.5f ) ;
		if( pl_w->pl_dir ) spd = DOT_X( 8.5f ) ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
	}
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jump_dive_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		sp_stand_set( pl_w ) ;
		pl_char_set( pl_w, Char_Jump, 3 ) ;
		land_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_dive_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	static void ( *nml_move_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move_none, nml_move_slide, nml_move_step, nml_move_step_mk, nml_move_steps,
		nml_move_charge,
	} ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	nml_move_jmp[PL0A_LW( pl_w )->nml_move]( pl_w ) ;
}

/*==============================================================*/
static void nml_move_none( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Slides forward when the animation says, and			*/
/*			stops at its second mark							*/
/*==============================================================*/
static void nml_move_slide( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			spd = DOT_X( 16.0f ) ;
			acc = -DOT_X( 0.5f ) ;
			if( !pl_w->pl_dir ){
				spd = -DOT_X( 16.0f ) ;
				acc = DOT_X( 0.5f ) ;
			}
			pl_w->spd.x = spd ;
			pl_w->acc.x = acc ;
		}
	} else {
		if( pl_w->cg_type == 2 ){
			pl_w->cg_type = 0 ;
			RATE_CLR_X( pl_w ) ;
		}
		if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			A step forward at the animation's mark				*/
/*==============================================================*/
static void nml_move_step( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		spd = DOT_X( 32.0f ) ;
		if( !pl_w->pl_dir ) spd = -DOT_X( 32.0f ) ;
		pl_w->pos.x += spd ;
	}
}

/*==============================================================*/
static void nml_move_step_mk( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		spd = DOT_X( 32.0f ) ;
		if( !pl_w->pl_dir ) spd = -DOT_X( 32.0f ) ;
		pl_w->pos.x += spd ;
	}
}

/*==============================================================*/
/*			Steps of the lengths the animation picks			*/
/*==============================================================*/
static void nml_move_steps( PLAYER_WORK *pl_w )
{
	static Float step_tbl[] = { 0.0f, -DOT_X( 8.0f ), -DOT_X( 8.0f ), -DOT_X( 16.0f ), -80.0f } ;
	Float spd ;

	if( pl_char_move( pl_w ) < 0 ){
		stand_ret( pl_w ) ;
		return ;
	}
	spd = pl_w->cg_type ;
	if( spd > 0.0f ){
		pl_w->cg_type = 0 ;
		spd = step_tbl[( int )spd] ;
		if( !pl_w->pl_dir ) spd = -spd ;
		pl_w->pos.x += spd ;
	} else RATE_CLR_X( pl_w ) ;
}

/*==============================================================*/
/*			Juggernaut's charge: one hit, then held for its		*/
/*			time												*/
/*==============================================================*/
static void nml_move_charge( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		if( pl_w->cg_prm > 0 ){
			pl_w->cg_prm = 0 ;
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
			pl_se_req( pl_w, 48 ) ;
		}
		if( -- pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Stand_P, 4 ) ;
		}
	} else if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/****************************************************************/
void pl0a_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
/*			The launcher										*/
/****************************************************************/
void pl0a_upper( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_brake_set( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->atck_no = pl0a_at_upper ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 23 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( pl_w->cg_prm > 0 ){
		spd = DOT_X( pl_w->cg_prm ) ;
		if( pl_w->disp_dir ) spd = -spd ;
		pl_w->pos.x += spd ;
		pl_w->cg_prm = 0 ;
	}
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_move( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0a_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_move, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 24 ;
		spd = -DOT_X( 9.5f ) ;
		acc = 0.3125f ;
		if( pl_w->pl_dir ){
			spd = DOT_X( 9.5f ) ;
			acc = -0.3125f ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
	}
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	Float acc ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		acc = 0.5208333f ;
		if( pl_w->pl_dir ) acc = -0.5208333f ;
		pl_w->acc.x = acc ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0a_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 24 ;
		spd = DOT_X( 9.5f ) ;
		acc = -0.3125f ;
		if( pl_w->pl_dir ){
			spd = -DOT_X( 9.5f ) ;
			acc = 0.3125f ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
	}
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	Float acc ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 < 0 ){
		pl_w->routine_2 ++ ;
		acc = -0.5208333f ;
		if( pl_w->pl_dir ) acc = 0.5208333f ;
		pl_w->acc.x = acc ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
	else if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_fly( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
/*			She flies in from off the screen					*/
/****************************************************************/
void pl0a_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_fly, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	Float ofs, spd ;

	pl_w->disp_flag = 0 ;
	pl_w->cg_shadow = 0xff ;
	if( vs_sys_work.vs_st >= VS_APPEAR ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->timer0 = 32 ;
		pl_w->spd.z = pl_w->pos.x ;
		ofs = -640.0f ;
		spd = 20.0f ;
		if( pl_w->disp_dir ){
			ofs = 640.0f ;
			spd = -20.0f ;
		}
		pl_w->pos.x += ofs ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->pos.y = pl_w->ground_y + DOT_Y( 256.0f ) ;
		pl_w->spd.y = -DOT_Y( 8.0f ) ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Appear, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_fly( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.x = pl_w->spd.z ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_se_req( pl_w, 49 ) ;
		pls0a_00_set( pl_w, 0 ) ;
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pls0a_00_set( pl_w, 1 ) ;
	}
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0a_win( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ) win_init( pl_w ) ;
	else win_move( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	static Uint8 ko_no[4] = { 0, 1, 2, 3 } ;
	static Uint8 win_no[5] = { 9, 9, 2, 9, 9 } ;
	Uint8 no ;

	pl_w->routine_2 ++ ;
	no = win_no[pl_w->type] ;
	if( !pl_w->type ){
		no = ko_no[Rnd() & 3] ;
		PL0A_LW( pl_w )->win_no = no ;
	}
	pl_char_set( pl_w, Char_Win, no ) ;
}

/*==============================================================*/
/*			After a win by a knockout, pose 3 is a jump that	*/
/*			lands in one of five poses							*/
/*==============================================================*/
static void win_move( PLAYER_WORK *pl_w )
{
	static Uint8 land_no[8] = { 4, 5, 6, 7, 8, 5, 6, 7 } ;
	Float spd ;

	if( pl_w->type ){
		if( pl_w->type == 2 ){
			if( pl_win_end_ck( pl_w ) ){
				stand_ret( pl_w ) ;
				return ;
			}
		}
		pl_char_move( pl_w ) ;
	} else if( PL0A_LW( pl_w )->win_no != 3 ){
		if( pl_win_end_ck( pl_w ) ) stand_ret( pl_w ) ;
		else {
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				shl0a_00_set( pl_w, -34.0f, 95.0f, 1 ) ;
			}
			pl_char_move( pl_w ) ;
		}
	} else if( !pl_w->routine_3 ){
		pl_w->routine_3 ++ ;
		spd = 5.0f ;
		if( pl_w->disp_dir ) spd = -5.0f ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 11.0f ) ;
		pl_w->acc.y = -1.0044643f ;
	} else if( pl_w->routine_3 == 1 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->pos.y > pl_w->ground_y ) pl_char_move( pl_w ) ;
		else {
			pl_w->routine_3 ++ ;
			pl_w->pos.y = pl_w->ground_y ;
			ef_color_set( pl_w, 0, pl_w->pl_no + 20, 1 ) ;
			pl_char_set( pl_w, Char_Win, land_no[Rnd() & 7] ) ;
			land_set( pl_w ) ;
			pl_voice_req( pl_w, 16 ) ;
		}
	} else if( pl_win_end_ck( pl_w ) ) stand_ret( pl_w ) ;
	else pl_char_move( pl_w ) ;
}

static void sp_00( PLAYER_WORK *pl_w );
static void sp_01( PLAYER_WORK *pl_w );
static void sp_02( PLAYER_WORK *pl_w );
static void sp_repeat( PLAYER_WORK *pl_w );
static void sp_04( PLAYER_WORK *pl_w );
static void sp_05( PLAYER_WORK *pl_w );
static void sp_06( PLAYER_WORK *pl_w );
static void sp_07( PLAYER_WORK *pl_w );
static void sp_leap( PLAYER_WORK *pl_w );
static void sp_09( PLAYER_WORK *pl_w );
static void sp_10( PLAYER_WORK *pl_w );
static void sp_drain( PLAYER_WORK *pl_w );
static void sp_screw( PLAYER_WORK *pl_w );
static void sp_rising( PLAYER_WORK *pl_w );
static void sp_14( PLAYER_WORK *pl_w );
static void sp_15( PLAYER_WORK *pl_w );
static void sp_16( PLAYER_WORK *pl_w );
static void sp_spike( PLAYER_WORK *pl_w );
static void sp_barrage( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_sugah( PLAYER_WORK *pl_w );
static void sp_21( PLAYER_WORK *pl_w );
static void sp_dive( PLAYER_WORK *pl_w );
static void sp_shungoku( PLAYER_WORK *pl_w );
static void sp_team( PLAYER_WORK *pl_w );
static void sp_25( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_barragex( PLAYER_WORK *pl_w );
static void sp_reppa( PLAYER_WORK *pl_w );

/****************************************************************/
void pl0a_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_00, sp_01, sp_02, sp_repeat, sp_04, sp_05, sp_06, sp_07, sp_leap, sp_09,
		sp_10, sp_drain, sp_screw, sp_rising, sp_14, sp_15, sp_16, sp_spike, sp_barrage, sp_chouhatsu,
		sp_sugah, sp_21, sp_dive, sp_shungoku, sp_team, sp_25, sp_snapback, sp_barragex, sp_reppa,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			The special moves she doesn't have					*/
/*==============================================================*/
static void sp_00( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_01( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_02( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void repeat_init( PLAYER_WORK *pl_w );
static void repeat_wait( PLAYER_WORK *pl_w );
static void repeat_move( PLAYER_WORK *pl_w );
static void repeat_fall( PLAYER_WORK *pl_w );
static void repeat_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A dash forward that rises into a jump				*/
/*==============================================================*/
static void sp_repeat( PLAYER_WORK *pl_w )
{
	static void ( *repeat_jmp[])( PLAYER_WORK *pl_w ) = {
		repeat_init, repeat_wait, repeat_move, repeat_fall, repeat_end,
	} ;

	repeat_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void repeat_init( PLAYER_WORK *pl_w )
{
	static Float repeat_spd[3 * 2] = {
		-10.0f, DOT_X( 0.125f ),
		-DOT_X( 8.0f ), DOT_X( 0.125f ),
		-DOT_X( 10.0f ), DOT_X( 0.125f ),
	} ;
	Float *tbl, spd, acc ;

	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	pl_gauge_add( pl_w, 16 ) ;
	tbl = &repeat_spd[pl_w->atck_level * 2] ;
	spd = *tbl++ ;
	acc = *tbl ;
	if( pl_w->disp_dir ){
		spd = -spd ;
		acc = -acc ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level << 1 ) ;
}

/*--------------------------------------------------------------*/
static void repeat_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ) pl_w->routine_2 ++ ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void repeat_move( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_stance = STANCE_AIR ;
		spd = -10.0f ;
		acc = DOT_X( 0.375f ) ;
		if( pl_w->disp_dir ){
			spd = 10.0f ;
			acc = -DOT_X( 0.375f ) ;
		}
		pl_w->spd.x = spd ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.x = acc ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void repeat_fall( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y < 0.0f ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 3 ) ;
	land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void repeat_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_04( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_05( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_06( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_07( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void leap_init( PLAYER_WORK *pl_w );
static void leap_wait( PLAYER_WORK *pl_w );
static void leap_fly( PLAYER_WORK *pl_w );
static void leap_end( PLAYER_WORK *pl_w );
static void leap_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A jump forward and down, from Ken's or				*/
/*			Wolverine's power									*/
/*==============================================================*/
static void sp_leap( PLAYER_WORK *pl_w )
{
	static void ( *leap_jmp[])( PLAYER_WORK *pl_w ) = {
		leap_init, leap_wait, leap_fly, leap_end,
	} ;

	leap_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void leap_init( PLAYER_WORK *pl_w )
{
	static Float leap_spd[2][3] = {
		{ -10.0f, -DOT_X( 8.0f ), -DOT_X( 10.0f ) },
		{ DOT_Y( 8.0f ), DOT_Y( 12.0f ), DOT_Y( 16.0f ) },
	} ;
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_sp_ef_set( pl_w ) ;
	sp_stand_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	spd = leap_spd[0][pl_w->atck_level * 2] ;
	acc = 0.7291666f ;
	if( pl_w->disp_dir ){
		spd = -spd ;
		acc = -0.7291666f ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = leap_spd[1][pl_w->atck_level * 2] ;
	pl_w->acc.y = -DOT_Y( 0.5f ) ;
	pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 18 ) ;
}

/*--------------------------------------------------------------*/
static void leap_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void leap_fly( PLAYER_WORK *pl_w )
{
	leap_move( pl_w ) ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		sp_stand_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void leap_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Sideways only while slowing down, and falling		*/
/*			faster once past the top							*/
/*==============================================================*/
static void leap_move( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.x * pl_w->acc.x < 0.0f ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
	}
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ) pl_w->acc.y = -2.142857f ;
}

/*==============================================================*/
static void sp_09( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_10( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void drain_init( PLAYER_WORK *pl_w );
static void drain_wait( PLAYER_WORK *pl_w );
static void drain_move( PLAYER_WORK *pl_w );
static void sp_drain_end( PLAYER_WORK *pl_w );
static void drain_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A lunge that takes the power of whoever it			*/
/*			catches												*/
/*==============================================================*/
static void sp_drain( PLAYER_WORK *pl_w )
{
	static void ( *sp_drain_jmp[])( PLAYER_WORK *pl_w ) = {
		drain_init, drain_wait, drain_move, sp_drain_end, drain_land,
	} ;

	sp_drain_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void drain_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pl_stance = STANCE_STAND ;
		pl_sp_ef_set( pl_w ) ;
	}
	PL0A_LW( pl_w )->super_flag = 0 ;
	pl_gauge_add( pl_w, 1 ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd = -DOT_X( 10.0f ) ;
	acc = 0.3125f ;
	if( pl_w->pl_dir ){
		spd = DOT_X( 10.0f ) ;
		acc = -0.3125f ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_char_set( pl_w, Char_Special, 22 ) ;
}

/*--------------------------------------------------------------*/
static void drain_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 16 ;
		PL0A_LW( pl_w )->glow_timer = 4 ;
		pls0a_02_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void drain_move( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float acc ;

	PL0A_LW( pl_w )->glow_timer = 4 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		acc = 0.5208333f ;
		if( pl_w->pl_dir ) acc = -0.5208333f ;
		pl_w->acc.x = acc ;
		pl_w->routine_2 ++ ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->routine_2 = 4 ;
			PL0A_LW( pl_w )->glow_timer = 0 ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( pl_w, Char_Jump, 9 ) ;
		} else pl_char_set( pl_w, Char_Special, 23 ) ;
	} else if( ( em = pl_catch_ck( pl_w ) ) ){
		PL0A_LW( pl_w )->glow_timer = 16 ;
		if( !pl_w->team_no ){
			PL0A_LW( pl_w )->power_type = em->pl_type ;
			power_set( pl_w, em ) ;
		} else pl_w->obj_flag = 0 ;
		pl_w->catch_no = 0xc0 ;
		pl_catch_set( pl_w, em ) ;
	}
}

/*--------------------------------------------------------------*/
static void sp_drain_end( PLAYER_WORK *pl_w )
{
	PL0A_LW( pl_w )->glow_timer = 4 ;
	if( pl_char_move( pl_w ) >= 0 ){
		if( !pl_w->cg_type ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
	} else {
		PL0A_LW( pl_w )->glow_timer = 0 ;
		stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void drain_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	land_set( pl_w ) ;
	stand_ret( pl_w ) ;
}

static void screw_init( PLAYER_WORK *pl_w );
static void screw_catch( PLAYER_WORK *pl_w );
static void screw_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Zangief's throw										*/
/*==============================================================*/
static void sp_screw( PLAYER_WORK *pl_w )
{
	static void ( *screw_jmp[])( PLAYER_WORK *pl_w ) = {
		screw_init, screw_catch, screw_end,
	} ;

	screw_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void screw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Special, 26 ) ;
}

/*--------------------------------------------------------------*/
static void screw_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0xc1 ;
			pl_catch_set( pl_w, em ) ;
		} else {
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 27 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void screw_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void rising_init( PLAYER_WORK *pl_w );
static void rising_wait( PLAYER_WORK *pl_w );
static void rising_move( PLAYER_WORK *pl_w );
static void rising_rise( PLAYER_WORK *pl_w );
static void rising_land( PLAYER_WORK *pl_w );
static void rising_jump( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hops of kicks, more while the button is pressed		*/
/*			again												*/
/*==============================================================*/
static void sp_rising( PLAYER_WORK *pl_w )
{
	static void ( *rising_jmp[])( PLAYER_WORK *pl_w ) = {
		rising_init, rising_wait, rising_move, rising_rise, rising_land,
	} ;

	rising_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void rising_init( PLAYER_WORK *pl_w )
{
	static Uint8 rising_cnt[4] = { 1, 2, 3, 3 } ;

	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_sp_ef_set( pl_w ) ;
	sp_stand_set( pl_w ) ;
	rising_jump( pl_w ) ;
	pl_gauge_add( pl_w, 16 ) ;
	pl_w->timer0 = rising_cnt[pl_w->atck_level * 2] ;
	pl_w->timer1 = 0 ;
	pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 50 ) ;
}

/*--------------------------------------------------------------*/
static void rising_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void rising_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->timer0 && pl_w->timer1 ){
			pl_w->timer0 -- ;
			pl_w->timer1 = 0 ;
			pl_char_set( pl_w, Char_Special, pl_w->cg_prm + 7 ) ;
		} else {
			pl_w->routine_2 = 3 ;
			pl_char_set( pl_w, Char_Special, ( pl_w->atck_level << 1 ) + 9 ) ;
		}
	} else if( pl_w->cg_type > 0 ){
		pl_w->cg_type = 0 ;
		rising_jump( pl_w ) ;
	} else if( pl_w->cg_type < 0 && sw_renda_ck( pl_w ) ) pl_w->timer1 = 0xff ;
}

/*--------------------------------------------------------------*/
static void rising_rise( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void rising_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	land_set( pl_w ) ;
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void rising_jump( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	spd = -DOT_X( 8.0f ) ;
	acc = DOT_X( 0.25f ) ;
	if( pl_w->disp_dir ){
		spd = DOT_X( 8.0f ) ;
		acc = -DOT_X( 0.25f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_w->acc.x = acc ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
}

/*==============================================================*/
static void sp_14( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_15( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_16( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void spike_init( PLAYER_WORK *pl_w );
static void spike_wait( PLAYER_WORK *pl_w );
static void spike_rise( PLAYER_WORK *pl_w );
static void spike_move( PLAYER_WORK *pl_w );
static void spike_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Cammy's: a rising kick that carries on forward		*/
/*==============================================================*/
static void sp_spike( PLAYER_WORK *pl_w )
{
	static void ( *spike_jmp[])( PLAYER_WORK *pl_w ) = {
		spike_init, spike_wait, spike_rise, spike_move, spike_end,
	} ;

	spike_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void spike_init( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	no = 33 ;
	if( pl_w->atck_level ) no = 48 ;
	pl_char_set( pl_w, Char_Special, no ) ;
}

/*--------------------------------------------------------------*/
static void spike_wait( PLAYER_WORK *pl_w )
{
	static Float spike_spd[2][3] = {
		{ -DOT_X( 8.0f ), -20.0f, -DOT_X( 10.0f ) },
		{ -DOT_Y( 0.25f ), -0.13392857f, -DOT_Y( 0.125f ) },
	} ;
	Float spd ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_w->pos.y += DOT_Y( 16.0f ) ;
		spd = spike_spd[0][pl_w->atck_level] ;
		if( pl_w->disp_dir ) spd = -spd ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = pl_w->spd.y = 0.0f ;
		pl_w->acc.y = spike_spd[1][pl_w->atck_level] ;
	}
}

/*--------------------------------------------------------------*/
static void spike_rise( PLAYER_WORK *pl_w )
{
	static Float spike_acc[3] = { 1.5625f, 2.5f, 1.6666666f } ;
	Float acc ;
	Uint8 no ;

	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		acc = spike_acc[pl_w->atck_level] ;
		if( pl_w->disp_dir ) acc = -acc ;
		pl_w->acc.x = acc ;
		no = 34 ;
		if( pl_w->atck_level ) no = 49 ;
		pl_char_set( pl_w, Char_Special, no ) ;
	}
}

/*==============================================================*/
/*			Until it stops										*/
/*==============================================================*/
static void spike_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x > 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void spike_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void barrage_init( PLAYER_WORK *pl_w );
static void barrage_move( PLAYER_WORK *pl_w );
static void barrage_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Wolverine's slashes forward, more while a punch		*/
/*			is pressed											*/
/*==============================================================*/
static void sp_barrage( PLAYER_WORK *pl_w )
{
	static void ( *barrage_jmp[])( PLAYER_WORK *pl_w ) = {
		barrage_init, barrage_move, barrage_end,
	} ;

	barrage_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void barrage_init( PLAYER_WORK *pl_w )
{
	static Float barrage_spd[3] = { -DOT_X( 6.5f ), -DOT_X( 6.5f ), -DOT_X( 6.5f ) } ;
	static Sint16 barrage_cnt[3] = { 2, 6, 4 } ;
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	sp_stand_set( pl_w ) ;
	pl_w->timer0 = 2 ;
	pl_w->timer1 = barrage_cnt[pl_w->atck_level] ;
	pl_w->type = 0 ;
	pl_w->sub_type = 0 ;
	pl_w->atck_no = pl0a_at_barrage ;
	HIT_RESET( pl_w ) ;
	spd = barrage_spd[pl_w->atck_level] ;
	if( pl_w->disp_dir ) spd = -spd ;
	pl_w->spd.x = spd ;
	pl_char_set( pl_w, Char_Special, 37 ) ;
}

/*--------------------------------------------------------------*/
static void barrage_move( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->type ){
			if( -- pl_w->timer1 >= 0 ) pl_w->timer0 ++ ;
			pl_w->type = 0 ;
			pl_w->pl_stop_now = 0 ;
		}
		if( -- pl_w->timer0 <= 0 ){
			pl_w->routine_2 ++ ;
			no = 53 ;
		} else {
			pl_w->sub_type ^= 1 ;
			no = pl_w->sub_type + 37 ;
			if( pl_w->timer0 == 1 ) no += 3 ;
		}
		pl_char_set( pl_w, Char_Special, no ) ;
	} else {
		if( ( pl_w->pl_now_lvbt | pl_w->pl_stop_now ) & 0x60 ) pl_w->type = 1 ;
		if( pl_w->cg_type ){
			pls0a_03_set( pl_w, pl_w->cg_type, pl_w->cg_flag1 ) ;
			pls0a_03_set( pl_w, pl_w->cg_type - 1, pl_w->cg_flag1 ) ;
			pl_w->cg_type = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void barrage_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The taunt											*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->timer0 = 60 ;
		pl_char_set( pl_w, Char_Extra, 22 ) ;
	} else {
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			shl0a_00_set( pl_w, -34.0f, 95.0f, 0 ) ;
		}
		if( !-- pl_w->timer0 ) stand_ret( pl_w ) ;
	}
	hit_set( pl_w ) ;
}

static void sugah_init( PLAYER_WORK *pl_w );
static void sugah_wait( PLAYER_WORK *pl_w );
static void sugah_dash_init( PLAYER_WORK *pl_w );
static void sugah_dash( PLAYER_WORK *pl_w );
static void sugah_catch( PLAYER_WORK *pl_w );
static void sugah_rise( PLAYER_WORK *pl_w );
static void sugah_miss( PLAYER_WORK *pl_w );
static void sugah_hold( PLAYER_WORK *pl_w );
static void sugah_drain( PLAYER_WORK *pl_w );
static void sugah_drop( PLAYER_WORK *pl_w );
static void sugah_fall( PLAYER_WORK *pl_w );
static void sugah_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A hyper combo: a dash that grabs, carries the		*/
/*			opponent up and takes their power					*/
/*==============================================================*/
static void sp_sugah( PLAYER_WORK *pl_w )
{
	static void ( *sugah_jmp[])( PLAYER_WORK *pl_w ) = {
		sugah_init, sugah_wait, sugah_dash_init, sugah_dash, sugah_catch, sugah_rise,
		sugah_miss, sugah_hold, sugah_drain, sugah_drop, sugah_fall, sugah_end,
	} ;

	sugah_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void sugah_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	PL0A_LW( pl_w )->super_flag = 0xff ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Super, 0 ) ;
}

/*--------------------------------------------------------------*/
static void sugah_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		ofs.x = -DOT_X( 17.0f ) ;
		ofs.y = DOT_Y( 81.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sugah_dash_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
	pl_w->timer0 = 24 ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd = -DOT_X( 16.0f ) ;
	acc = DOT_X( 0.25f ) ;
	if( pl_w->pl_dir ){
		spd = DOT_X( 16.0f ) ;
		acc = -DOT_X( 0.25f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
}

/*--------------------------------------------------------------*/
static void sugah_dash( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float spd, ofs, acc ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( !pl_w->hit_flag ){
		if( -- pl_w->timer0 == 0 ){ } else return ;
	} else {
		em = pl_w->em_w ;
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			spd = -DOT_X( 2.0f ) ;
			ofs = DOT_X( 64.0f ) ;
			if( pl_w->pl_dir ){
				spd = DOT_X( 2.0f ) ;
				ofs = -DOT_X( 64.0f ) ;
			}
			pl_w->spd.x = spd ;
			pl_w->acc.x = 0.0f ;
			em->pl_stance = STANCE_STAND ;
			em->pos.y = pl_w->ground_y ;
			em->pos.x = pl_w->pos.x - ofs ;
			pl_char_set( pl_w, Char_Super, 3 ) ;
			return ;
		}
	}
	pl_w->routine_2 = 6 ;
	spd = -DOT_X( 10.0f ) ;
	acc = 0.5208333f ;
	if( pl_w->pl_dir ){
		spd = DOT_X( 10.0f ) ;
		acc = -0.5208333f ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_char_set( pl_w, Char_Super, 2 ) ;
}

/*--------------------------------------------------------------*/
static void sugah_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_sp_catch_ck( pl_w ) ){
			pl_w->routine_2 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->spd.y = DOT_Y( 16.0f ) ;
			pl_w->acc.y = 0.0f ;
			pl_w->timer0 = 18 ;
			pl_char_set( pl_w, Char_Super, 4 ) ;
		} else {
			pl_w->routine_2 = 6 ;
			pl_char_set( pl_w, Char_Super, 2 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sugah_rise( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 = 7 ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_AIR ;
		em = pl_w->em_w ;
		PL0A_LW( pl_w )->power_type = em->pl_type ;
		power_set( pl_w, em ) ;
		pl_w->catch_no = 0xc0 ;
		pl_sp_catch_set( pl_w, em ) ;
		pl_se_req( pl_w, 35 ) ;
		scr_excep_set( pl_w, 5, 5 ) ;
		pl_w->hit_stop = 10 ;
		em = pl_w->catch_w ;
		em->pos.y += DOT_Y( 24.0f ) ;
		pl_w->timer0 = 64 ;
		pl_char_set( pl_w, Char_Catch, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sugah_miss( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) >= 0 ){
		if( !pl_w->cg_type ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
		}
	} else stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sugah_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	power_wait( pl_w ) ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		pl_w->catch_no = em->catch_no = 0xc3 ;
	}
}

/*--------------------------------------------------------------*/
static void sugah_drain( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	power_wait( pl_w ) ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	} else {
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			shl0a_00_set( pl_w, 29.0f, 100.0f, 2 ) ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			pl_voice_req( pl_w, 22 ) ;
		}
	}
}

/*==============================================================*/
/*			Lets the opponent go, falling, and turns round		*/
/*==============================================================*/
static void sugah_drop( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	power_wait( pl_w ) ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->pl_dir = pl_w->pl_dir ;
		em->atck_no = pl0a_at_sugah ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir ^= 1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		scr_excep_clr() ;
		power_effect( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void sugah_fall( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	power_wait( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
	land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void sugah_end( PLAYER_WORK *pl_w )
{
	power_wait( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_21( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void dive_init( PLAYER_WORK *pl_w );
static void dive_wait( PLAYER_WORK *pl_w );
static void dive_rise( PLAYER_WORK *pl_w );
static void dive_turn( PLAYER_WORK *pl_w );
static void dive_dive( PLAYER_WORK *pl_w );
static void dive_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Rises while the button is held, then dives down		*/
/*			at an angle											*/
/*==============================================================*/
static void sp_dive( PLAYER_WORK *pl_w )
{
	static void ( *dive_jmp[])( PLAYER_WORK *pl_w ) = {
		dive_init, dive_wait, dive_rise, dive_turn, dive_dive, dive_end,
	} ;

	dive_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void dive_init( PLAYER_WORK *pl_w )
{
	static Float dive_spd[6] = {
		DOT_Y( 6.0f ), -DOT_Y( 0.125f ), DOT_Y( 8.0f ), -0.3348214f, DOT_Y( 10.0f ), -0.4017857f,
	} ;
	static Uint16 dive_btn[3] = { 0x40, 0x20, 0x20 } ;
	static Sint16 dive_cnt[3] = { 48, 64, 80 } ;
	Float *tbl ;
	int lv ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl0a_at_dive ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 16 ) ;
	lv = pl_w->atck_level * 2 ;
	PL0A_LW( pl_w )->rise_sw = dive_btn[lv] ;
	pl_w->timer1 = dive_cnt[lv] ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	tbl = &dive_spd[pl_w->atck_level * 4] ;
	pl_w->spd.y = *tbl++ ;
	pl_w->acc.y = *tbl ;
	pl_char_set( pl_w, Char_Special, 44 ) ;
}

/*--------------------------------------------------------------*/
static void dive_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pl_stance = STANCE_AIR ;
	}
}

/*==============================================================*/
/*			Until the top, or until she is high enough and		*/
/*			the button is let go								*/
/*==============================================================*/
static void dive_rise( PLAYER_WORK *pl_w )
{
	static Sint16 dive_time[3] = { 8, 10, 12 } ;

	pl_char_move( pl_w ) ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !( pl_w->pos.y > pl_w->ground_y + DOT_Y( pl_w->timer1 )
		&& ( !pl_w->cpu_flag ? pl_w->pl_sw_now & PL0A_LW( pl_w )->rise_sw : sw_renda_ck( pl_w ) ) )
		&& pl_w->spd.y > 0.0f ) return ;
	pl_w->routine_2 ++ ;
	pl_w->timer0 = dive_time[pl_w->atck_level * 2] ;
	pl_char_set( pl_w, Char_Special, 45 ) ;
}

/*--------------------------------------------------------------*/
static void dive_turn( PLAYER_WORK *pl_w )
{
	static Float dive_dive_spd[6] = {
		DOT_Y( 9.0f ), -DOT_Y( 6.0f ), 22.5f, -15.0f, DOT_Y( 12.0f ), -DOT_Y( 8.0f ),
	} ;
	Float *tbl ;
	Float spd ;

	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		tbl = &dive_dive_spd[pl_w->atck_level * 4] ;
		spd = *tbl++ ;
		if( !pl_w->disp_dir ) spd = -spd ;
		pl_w->spd.x = spd ;
		pl_w->spd.y = *tbl ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Special, 46 ) ;
		PL0A_LW( pl_w )->glow_timer = 4 ;
		pls0a_02_set( pl_w, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dive_dive( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	PL0A_LW( pl_w )->glow_timer = 4 ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Special, 47 ) ;
	land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dive_end( PLAYER_WORK *pl_w )
{
	PL0A_LW( pl_w )->glow_timer = 4 ;
	if( pl_char_move( pl_w ) < 0 ){
		PL0A_LW( pl_w )->glow_timer = 0 ;
		stand_ret( pl_w ) ;
	}
}

static void shungoku_init( PLAYER_WORK *pl_w );
static void shungoku_wait( PLAYER_WORK *pl_w );
static void shungoku_dash( PLAYER_WORK *pl_w );
static void shungoku_hold( PLAYER_WORK *pl_w );
static void shungoku_drain( PLAYER_WORK *pl_w );
static void shungoku_end( PLAYER_WORK *pl_w );
static void shungoku_catch_set( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Akuma's hyper combo: a dash that catches the		*/
/*			opponent, then blow after blow in the dark			*/
/*==============================================================*/
static void sp_shungoku( PLAYER_WORK *pl_w )
{
	static void ( *shungoku_jmp[])( PLAYER_WORK *pl_w ) = {
		shungoku_init, shungoku_wait, shungoku_dash, shungoku_hold, shungoku_drain, shungoku_end,
	} ;

	shungoku_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shungoku_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 32 ;
	pl_w->timer1 = 6 ;
	spd = -DOT_X( 3.5f ) ;
	if( pl_w->disp_dir ) spd = DOT_X( 3.5f ) ;
	pl_w->spd.x = spd ;
	pl_char_set( pl_w, Char_Super, 5 ) ;
}

/*--------------------------------------------------------------*/
static void shungoku_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
	else {
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 7.0f ) ;
		ofs.y = DOT_Y( 92.0f ) ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
	}
}

/*==============================================================*/
/*			Until she reaches the opponent, a wall or the		*/
/*			end of the dash										*/
/*==============================================================*/
static void shungoku_dash( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->wall_flag & ( 1 << ( pl_w->pl_dir ^ 1 ) ) || -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		stand_ret( pl_w ) ;
	} else if( pl_w->timer1 ) pl_w->timer1 -- ;
	else if( ( em = pl_catch_ck( pl_w ) ) ){
		pl_w->routine_2 ++ ;
		pl_w->catch_no = 0xc4 ;
		pl_sp_catch_set( pl_w, em ) ;
		shungoku_catch_set( pl_w ) ;
		ofs.x = -DOT_X( 50.0f ) ;
		ofs.y = DOT_Y( 74.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_hold( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 64 ;
		pl_w->timer1 = 0 ;
		flash_dark_long_set() ;
		flash_work.timer = 2 ;
		pl_char_set( pl_w, Char_Catch, 10 ) ;
	}
}

/*==============================================================*/
/*			Sparks fly about the opponent until she lets go		*/
/*==============================================================*/
static void shungoku_drain( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	flash_work.timer = 2 ;
	if( !( pl_w->timer1 += 0x4000 ) ){
		pos = pl_w->pos ;
		pos.x += ( Sint16 )( ( Rnd() & 0xff ) - 128 ) * 1.6666666f ;
		pos.y += ( Sint16 )( ( Rnd() & 0x7f ) + 32 ) * 2.142857f ;
		hitmark_set1( &pos, pl_w->disp_dir, 1, 0 ) ;
		if( Rnd() & 1 ) pl_se_req( pl_w, 0 ) ;
		else pl_se_req( pl_w, 1 ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 64 ;
		em = pl_w->catch_w ;
		combo_add( em, pl_w, 14 ) ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 8 ;
		em->pl_stance = STANCE_STAND ;
		em->atck_no = pl_w->atck_no = pl0a_at_shungoku ;
		scr_excep_clr() ;
		flash_work.timer = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->target_w->catch_ng_timer = 48 ;
		stand_ret( pl_w ) ;
	}
}

static void team_init( PLAYER_WORK *pl_w );
static void team_wait( PLAYER_WORK *pl_w );
static void team_move( PLAYER_WORK *pl_w );
static void team_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The partner hyper combo: she flies forward			*/
/*==============================================================*/
static void sp_team( PLAYER_WORK *pl_w )
{
	static void ( *team_jmp[])( PLAYER_WORK *pl_w ) = {
		team_init, team_wait, team_move, team_end,
	} ;

	team_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void team_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	spd = -20.0f ;
	acc = DOT_X( 0.125f ) ;
	if( pl_w->pl_dir ){
		spd = 20.0f ;
		acc = -DOT_X( 0.125f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->acc.x = acc ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 6 ) ;
}

/*--------------------------------------------------------------*/
static void team_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 26.0f ) ;
		ofs.y = 120.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void team_move( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->cg_prm ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void team_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
static void sp_25( PLAYER_WORK *pl_w )
{
	stand_ret( pl_w ) ;
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_wait( PLAYER_WORK *pl_w );
static void snapback_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The snapback										*/
/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_wait, snapback_end,
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
	pl_w->atck_no = pl0a_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = 45.0f ;
		ofs.y = DOT_Y( 96.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void snapback_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void barragex_init( PLAYER_WORK *pl_w );
static void barragex_wait( PLAYER_WORK *pl_w );
static void barragex_start( PLAYER_WORK *pl_w );
static void barragex_move( PLAYER_WORK *pl_w );
static void barragex_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The partner hyper combo with Wolverine: slashes		*/
/*			forward												*/
/*==============================================================*/
static void sp_barragex( PLAYER_WORK *pl_w )
{
	static void ( *barragex_jmp[])( PLAYER_WORK *pl_w ) = {
		barragex_init, barragex_wait, barragex_start, barragex_move, barragex_end,
	} ;

	barragex_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void barragex_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl0a_at_barrage ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
}

/*--------------------------------------------------------------*/
static void barragex_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		ofs.x = DOT_X( 26.0f ) ;
		ofs.y = 120.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void barragex_start( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 18 ;
		pl_w->sub_type = 0 ;
		spd = -DOT_X( 5.0f ) ;
		if( pl_w->disp_dir ) spd = DOT_X( 5.0f ) ;
		pl_w->spd.x = spd ;
		pl_char_set( pl_w, Char_Super, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void barragex_move( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_char_move( pl_w ) < 0 ){
		if( -- pl_w->timer0 <= 0 ){
			pl_w->delay_flag = 0 ;
			pl_w->delay_timer = 0 ;
			pl_w->hyper_bg = 0 ;
			pl_w->hyper_bg_tm = 0 ;
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Special, 53 ) ;
		} else {
			pl_w->sub_type ^= 1 ;
			no = pl_w->sub_type + 11 ;
			if( pl_w->timer0 == 1 ){
				no += 2 ;
				pl_w->spd.x = 0.0f ;
			}
			pl_char_set( pl_w, Char_Super, no ) ;
		}
	} else {
		if( pl_w->cg_type ){
			pls0a_03_set( pl_w, pl_w->cg_type, pl_w->cg_flag1 ) ;
			pls0a_03_set( pl_w, pl_w->cg_type - 1, pl_w->cg_flag1 ) ;
			pl_w->cg_type = 0 ;
		}
		if( pl_w->cg_prm ){
			pls0a_03_set( pl_w, 0x80, pl_w->cg_flag1 ) ;
			pls0a_03_set( pl_w, 0x81, pl_w->cg_flag1 ) ;
			pls0a_03_set( pl_w, 0x82, pl_w->cg_flag1 ) ;
			pls0a_03_set( pl_w, 0x83, pl_w->cg_flag1 ) ;
			pl_w->cg_prm = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void barragex_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void reppa_init( PLAYER_WORK *pl_w );
static void reppa_wait( PLAYER_WORK *pl_w );
static void reppa_dash( PLAYER_WORK *pl_w );
static void reppa_jump( PLAYER_WORK *pl_w );
static void reppa_brake( PLAYER_WORK *pl_w );
static void reppa_fall( PLAYER_WORK *pl_w );
static void reppa_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The partner hyper combo with Ken: a dash into a		*/
/*			rising punch										*/
/*==============================================================*/
static void sp_reppa( PLAYER_WORK *pl_w )
{
	static void ( *reppa_jmp[])( PLAYER_WORK *pl_w ) = {
		reppa_init, reppa_wait, reppa_dash, reppa_jump, reppa_brake, reppa_fall, reppa_end,
	} ;

	reppa_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void reppa_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl0a_at_dive ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 15 ) ;
}

/*--------------------------------------------------------------*/
static void reppa_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;
	Float spd, acc ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		spd = -DOT_X( 10.0f ) ;
		acc = DOT_X( 0.375f ) ;
		if( pl_w->disp_dir ){
			spd = DOT_X( 10.0f ) ;
			acc = -DOT_X( 0.375f ) ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		ofs.x = -40.0f ;
		ofs.y = DOT_Y( 24.0f ) ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void reppa_dash( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->spd.x * pl_w->acc.x >= 0.0f ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
	if( pl_w->cg_type == 2 ){
		pl_w->cg_type = 0 ;
		spd = -DOT_X( 10.0f ) ;
		acc = DOT_X( 0.375f ) ;
		if( pl_w->disp_dir ){
			spd = DOT_X( 10.0f ) ;
			acc = -DOT_X( 0.375f ) ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
	}
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		spd = -20.0f ;
		acc = DOT_X( 0.375f ) ;
		if( pl_w->disp_dir ){
			spd = 20.0f ;
			acc = -DOT_X( 0.375f ) ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		pl_w->spd.y = DOT_Y( 16.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	}
}

/*--------------------------------------------------------------*/
static void reppa_jump( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void reppa_brake( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( pl_w->acc.x * pl_w->spd.x >= 0.0f ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = pl_w->acc.x = 0.0f ;
	}
	reppa_fall( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void reppa_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ) pl_w->acc.y = -1.2053571f ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_land_ef_set( pl_w ) ;
	} else if( !pl_w->cg_type ) pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void reppa_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl0a_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->pos.y += DOT_Y( 64.0f ) ;
	spd = -30.0f ;
	if( pl_w->pl_dir ) spd = 30.0f ;
	pl_w->spd.x = spd ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = -DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl0a_at_dive ;
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
		land_set( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Her throw: a punch throw on the ground or in the	*/
/*			air													*/
/****************************************************************/
PLAYER_WORK *pl0a_catch_ck( PLAYER_WORK *pl_w )
{
	static Sint32 catch_ok[4] = { 1, 0, 1, 0 } ;
	PLAYER_WORK *em ;
	Uint32 sw ;

	em = 0 ;
	if( catch_ok[pl_w->pl_stance] ){
		if( ( sw = pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) ){
			pl_w->dir = sw >> 10 ;
			if( !pl_w->atck_pk && pl_w->atck_level == 1 ){
				if( ( em = pl_catch_ck( pl_w ) ) ) pl_w->catch_no = 2 ;
			}
		}
	}
	return em ;
}

static void drain_set( PLAYER_WORK *pl_w );
static void screw_catch_set( PLAYER_WORK *pl_w );
static void nml_catch_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl0a_catch_ck or a special		*/
/*			move chose											*/
/****************************************************************/
void	pl0a_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		drain_set, screw_catch_set, nml_catch_set, nml_catch_set, shungoku_catch_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			The power drain: she lifts the opponent by their	*/
/*			height												*/
/*==============================================================*/
static void drain_set( PLAYER_WORK *pl_w )
{
	static Uint16 drain_y[64] = {
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	} ;
	PLAYER_WORK *em ;
	VEC3 ofs ;

	ofs.x = -110.0f ;
	ofs.y = DOT_Y( 80.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	if( !pl_w->team_no ) scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	em = pl_w->catch_w ;
	em->pos.y += DOT_Y( drain_y[em->pl_type] ) ;
	pl_w->timer0 = 64 ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
static void screw_catch_set( PLAYER_WORK *pl_w )
{
	pl_char_set( pl_w, Char_Catch, 3 ) ;
}

/*==============================================================*/
static void nml_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
		pl_w->pl_dir ^= 1 ;
	}
	ofs.x = -DOT_X( 34.0f ) ;
	ofs.y = DOT_Y( 85.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 7 ) ;
	catch_gauge_add( pl_w ) ;
}

/*==============================================================*/
/*			Akuma's hyper combo, once she has the opponent		*/
/*==============================================================*/
static void shungoku_catch_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = -DOT_X( 50.0f ) ;
	ofs.y = DOT_Y( 74.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->timer0 = 16 ;
	scr_excep_set( pl_w, 5, 5 ) ;
	pl_w->hit_stop = 10 ;
	pl_char_set( pl_w, Char_Catch, 9 ) ;
}

static void drain_hold( PLAYER_WORK *pl_w );
static void screw_throw( PLAYER_WORK *pl_w );
static void nml_catch( PLAYER_WORK *pl_w );
static void drain_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl0a_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		drain_hold, screw_throw, nml_catch, drain_catch, drain_hold,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
/*			Until the animation starts the drain				*/
/*==============================================================*/
static void drain_hold( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	power_wait( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = pl_w->routine_2 = 0 ;
		pl_w->catch_no = 0xc3 ;
		em = pl_w->catch_w ;
		em->catch_no = pl_w->catch_no ;
	}
}

static void screw_throw_hold( PLAYER_WORK *pl_w );
static void screw_throw_jump( PLAYER_WORK *pl_w );
static void screw_throw_fly( PLAYER_WORK *pl_w );
static void screw_throw_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			She leaps with the opponent and slams them down		*/
/*==============================================================*/
static void screw_throw( PLAYER_WORK *pl_w )
{
	static void ( *screw_throw_jmp[])( PLAYER_WORK *pl_w ) = {
		screw_throw_hold, screw_throw_jump, screw_throw_fly, screw_throw_end,
	} ;

	screw_throw_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void screw_throw_hold( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 4 ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = -DOT_X( 68.0f ) ;
		ofs.y = DOT_Y( 76.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->hit_stop = 10 ;
		scr_excep_set( pl_w, 5, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_throw_jump( PLAYER_WORK *pl_w )
{
	static Float screw_throw_spd[12] = {
		DOT_X( 5.5f ), 0.0f, DOT_Y( 14.5f ), -DOT_Y( 0.75f ),
		DOT_X( 3.5f ), 0.0f, DOT_Y( 16.5f ), -DOT_Y( 0.5f ),
		DOT_X( 3.5f ), 0.0f, DOT_Y( 16.5f ), -DOT_Y( 0.5f ),
	} ;
	Float spd, acc ;
	int i ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		i = pl_w->atck_level * 4 ;
		spd = screw_throw_spd[i] ;
		acc = screw_throw_spd[i + 1] ;
		pl_w->spd.y = screw_throw_spd[i + 2] ;
		pl_w->acc.y = screw_throw_spd[i + 3] ;
		if( pl_w->pos.x > 0.0f ){
			spd = -spd ;
			acc = -acc ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		pl_char_set( pl_w, Char_Catch, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_throw_fly( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		spd = 5.0f ;
		acc = 0.104166664f ;
		if( pl_w->pl_dir ){
			spd = -5.0f ;
			acc = -0.104166664f ;
		}
		pl_w->spd.x = spd ;
		pl_w->acc.x = acc ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -0.6696428f ;
		pl_char_set( pl_w, Char_Catch, 6 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		catch_se_req( pl_w->catch_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void screw_throw_end( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else {
		if( pl_w->cg_prm ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			em = pl_w->catch_w ;
			em->dm_hit_w = pl_w ;
			em->catch_type = 2 ;
			em->atck_no = pl0a_at_screw ;
			em->pl_dir = pl_w->pl_dir ^ 1 ;
			scr_excep_clr() ;
		}
	}
}

static void nml_catch_jump( PLAYER_WORK *pl_w );
static void nml_catch_rise( PLAYER_WORK *pl_w );
static void nml_catch_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			She jumps with the opponent and throws them			*/
/*			behind her											*/
/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_jump, nml_catch_rise, nml_catch_fall,
	} ;

	nml_catch_jmp[pl_w->routine_2]( pl_w ) ;
	hit_set( pl_w ) ;
}

/*==============================================================*/
static void nml_catch_jump( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->pos.y += 60.0f ;
		spd = -DOT_X( 2.0f ) ;
		if( pl_w->pl_dir ) spd = DOT_X( 2.0f ) ;
		pl_w->spd.x = spd ;
		pl_w->acc.x = 0.0f ;
		spd = DOT_Y( 8.0f ) ;
		acc = -DOT_Y( 0.125f ) ;
		if( pl_w->pl_stance == STANCE_AIR ){
			spd = DOT_Y( 2.0f ) ;
			acc = -0.06696428f ;
		}
		pl_w->spd.y = spd ;
		pl_w->acc.y = acc ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_rise( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_fall( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
	else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl0a_at_throw_p ;
		em->disp_dir = em->pl_dir = pl_w->pl_dir ^ 1 ;
		scr_excep_clr() ;
	}
}

static void drain_main( PLAYER_WORK *pl_w );
static void drain_drop( PLAYER_WORK *pl_w );
static void drain_fall( PLAYER_WORK *pl_w );
static void drain_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The power drain: she holds the opponent, takes		*/
/*			their power and lets them fall						*/
/*==============================================================*/
static void drain_catch( PLAYER_WORK *pl_w )
{
	static void ( *drain_jmp[])( PLAYER_WORK *pl_w ) = {
		drain_main, drain_drop, drain_fall, drain_end,
	} ;

	power_wait( pl_w ) ;
	drain_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void drain_main( PLAYER_WORK *pl_w )
{
	Sint8 no ;

	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	} else {
		if( pl_w->cg_prm ){
			pl_w->cg_prm = 0 ;
			shl0a_00_set( pl_w, 29.0f, 100.0f, 2 ) ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			no = 20 ;
			if( PL0A_LW( pl_w )->super_flag ) no = 22 ;
			pl_voice_req( pl_w, no ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void drain_drop( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Uint8 no ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->pl_dir = pl_w->pl_dir ;
		no = 32 ;
		if( PL0A_LW( pl_w )->super_flag ) no = 36 ;
		em->atck_no = no ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir ^= 1 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		scr_excep_clr() ;
		power_effect( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void drain_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_char_move( pl_w ) ;
		return ;
	}
	pl_w->routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Catch, 2 ) ;
	land_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drain_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Takes the power of em's character: no picks it,		*/
/*			and some characters give another one while their	*/
/*			own power-up is on									*/
/*==============================================================*/
static void power_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	static Sint8 power_no[64] = {
		1, 2, 2, 3, 5, 3, 1, 3, 3, 2, 0, 2, 3, 1, 1, 2,
		2, 5, 3, 2, 3, 1, 3, 1, 0, 0, 0, 3, 1, 2, 1, 5,
		3, 1, 2, 5, 3, 5, 1, 1, 3, 2, 3, 1, 2, 5, 2, 2,
		2, 2, 2, 2, 2, 1, 1, 2, 1, 3, 5,
	} ;
	static Sint16 power_time[10] = { 0, 900, 900, 600, 900, 0, 0, 900, 900, 900 } ;
	Sint32 *tm ;
	int disp ;
	int no ;

	pl_w->armor_flag = pl_w->tlocal[TM_WAIT] = pl_w->tlocal[TM_POWER_1]
		= pl_w->tlocal[TM_POWER_2] = pl_w->tlocal[TM_POWER_3] = pl_w->tlocal[TM_POWER_5]
		= pl_w->tlocal[TM_POWER_8] = pl_w->tlocal[TM_POWER_4] = pl_w->tlocal[TM_POWER_9]
		= pl_w->tlocal[TM_POWER_7] = pl_w->atck_pow = pl_w->def_pow = 0 ;
	no = power_no[PL0A_LW( pl_w )->power_type] ;
	if( pl_w != em && em->armor_timer_flag ) no = 4 ;
	if( em->armor_flag ){
		if( em->armor_flag & ARMOR_HEAVY ) no = 7 ;
		else no = 8 ;
	}
	switch( PL0A_LW( pl_w )->power_type ){
	case 0x0b :
		if( ( ( PL0B_LOCAL_W * )em->tlocal )->shield_st == 0 ) no = 1 ;
		break ;
	case 0x12 :
		if( ( ( PL12_LOCAL_W * )em->tlocal )->hyper_timer > 0 ) no = 1 ;
		break ;
	case 0x22 :
		if( em->obj_flag ) no = 1 ;
		break ;
	case 0x29 :
		if( ( ( PL29_LOCAL_W * )em->tlocal )->pw_set ) no = 1 ;
		break ;
	case 0x2d :
		if( ( ( PL2D_LOCAL_W * )em->tlocal )->power_step ) no = 1 ;
		if( ( ( PL2D_LOCAL_W * )em->tlocal )->dive_flag ) no = 2 ;
		break ;
	case 0x2f :
		if( ( ( PL2F_LOCAL_W * )em->tlocal )->elem == 3 ) no = 1 ;
		if( ( ( PL2F_LOCAL_W * )em->tlocal )->elem == 1 ) no = 3 ;
		break ;
	case 0x31 :
		if( ( ( PL31_LOCAL_W * )em->tlocal )->glow_timer > 0 ){
			if( em->atck_plus ) no = 1 ;
			if( em->speed_up_flag ) no = 3 ;
		}
		if( ( ( PL31_LOCAL_W * )em->tlocal )->maxspeed_flag ) no = 5 ;
		break ;
	case 0x37 :
		if( ( ( PL37_LOCAL_W * )em->tlocal )->burning_flag ) no = 9 ;
		break ;
	}
	PL0A_LW( pl_w )->power_type = pl_w->pl_type ;
	pl_w->obj_flag = no ;
	switch( no ){
	case 0 :
		break ;
	case 1 :
		pl_w->tlocal[TM_POWER_1] = 900 ;
		pl_w->atck_pow = 40 ;
		tm = &pl_w->tlocal[TM_POWER_1] ;
		disp = 0 ;
		break ;
	case 2 :
		pl_w->tlocal[TM_POWER_2] = 900 ;
		pl_w->def_pow = 16 ;
		tm = &pl_w->tlocal[TM_POWER_2] ;
		disp = 1 ;
		break ;
	case 3 :
		pl_w->tlocal[TM_POWER_3] = 600 ;
		pl_w->speed_up_flag = 1 ;
		tm = &pl_w->tlocal[TM_POWER_3] ;
		disp = 2 ;
		break ;
	case 4 :
		pl_w->tlocal[TM_POWER_4] = 900 ;
		armor_set( pl_w, 900, 0, 0 ) ;
		tm = &pl_w->tlocal[TM_POWER_4] ;
		disp = 3 ;
		break ;
	case 5 :
		if( ( pl_w->tlocal[TM_POWER_5] = ( pl_w->vital_red - pl_w->vital_new ) * 8 ) != 0 ){
			tm = &pl_w->tlocal[TM_POWER_5] ;
			disp = 4 ;
		} else pl_w->obj_flag = no = 0 ;
		break ;
	case 6 :
		pl_gauge_add( pl_w, 0x90 ) ;
		disp = 5 ;
		break ;
	case 7 :
		pl_w->tlocal[TM_POWER_7] = 900 ;
		pl_w->armor_flag |= ARMOR_HEAVY ;
		tm = &pl_w->tlocal[TM_POWER_7] ;
		disp = 8 ;
		break ;
	case 8 :
		pl_w->tlocal[TM_POWER_8] = 900 ;
		pl_w->armor_flag = ARMOR_ON ;
		tm = &pl_w->tlocal[TM_POWER_8] ;
		disp = 7 ;
		break ;
	case 9 :
		pl_w->tlocal[TM_POWER_9] = 900 ;
		pl_w->atck_pow = 40 ;
		pl_w->def_pow = 40 ;
		armor_set( pl_w, 900, 0, 0 ) ;
		tm = &pl_w->tlocal[TM_POWER_9] ;
		disp = 9 ;
		break ;
	}
	if( no == 0 ) return ;
	else {
		pl_w->tlocal[TM_WAIT] = 10 ;
		Timer_Gauge_Set( pl_w, tm, disp ) ;
	}
}

/*==============================================================*/
/*			The power's effect, from what the animation asks	*/
/*			for													*/
/*==============================================================*/
static void power_effect( PLAYER_WORK *pl_w )
{
	static Sint8 effect_no[10] = { 0, 0, 1, 2, 3, 4, 5, 3, 3, 3 } ;

	if( pl_w->obj_flag && !pl_w->team_no ) puff_ef_set( pl_w, effect_no[pl_w->obj_flag] ) ;
}

/*==============================================================*/
/*			Holds the powers' timers for a few frames			*/
/*==============================================================*/
static void power_wait( PLAYER_WORK *pl_w )
{
	pl_w->tlocal[TM_WAIT] = 4 ;
}

/*==============================================================*/
static void stand_ret( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
	else {
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Now and then a different pose while standing,		*/
/*			unless she has Sentinel's flight					*/
/****************************************************************/
void pl0a_wait_set( PLAYER_WORK *pl_w )
{
	Uint32 r ;

	if( pl_w->armor_flag == ARMOR_HEAVY ) return ;
	r = Rnd() ;
	if( r & 3 ) return ;
	if( r & 0x80 ) pl_w->char_no.b.no = 10 ;
	else pl_w->char_no.b.no = 11 ;
	pl_char_set( pl_w, Char_Stand, pl_w->char_no.b.no ) ;
}

/****************************************************************/
void pl0a_tlocal_init( PLAYER_WORK *pl_w )
{
	pl_w->armor_flag = 0 ;
	pl_w->atck_pow = 0 ;
	pl_w->def_pow = 0 ;
	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
}

/*==============================================================*/
static void land_set( PLAYER_WORK *pl_w )
{
	pl_land_ef_set( pl_w ) ;
	if( pl_w->armor_flag == ARMOR_HEAVY ){
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
}

/*==============================================================*/
static void sp_stand_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_dir_set( pl_w ) ;
}

/*==============================================================*/
/*			A hit on each of the animation's marks				*/
/*==============================================================*/
static void hit_set( PLAYER_WORK *pl_w )
{
	int no ;

	if( ( no = pl_w->cg_flag1 ) ){
		pl_w->cg_flag1 = 0 ;
		pl_w->atck_no = no ;
		HIT_RESET( pl_w ) ;
	}
}

/****************************************************************/
void pl0a_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: Ken's or				*/
/*			Wolverine's hyper combo while she has their			*/
/*			power												*/
/****************************************************************/
void pl0a_sp_unison( PLAYER_WORK *pl_w )
{
	static Uint8 unison_tbl[3] = { pl0a_sp_team, pl0a_sp_team, pl0a_sp_team } ;
	Uint8 sp ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	sp = unison_tbl[pl_w->pl_personal] ;
	if( PL0A_LW( pl_w )->power_type == 0x27 ) sp = pl0a_sp_reppa ;
	if( PL0A_LW( pl_w )->power_type == 0x07 ) sp = pl0a_sp_barragex ;
	pl_w->pl_sp_type = sp ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void pl0a_sp_gattai( PLAYER_WORK *pl_w )
{
	static Uint8 gattai_tbl[3] = { pl0a_sp_team, pl0a_sp_team, pl0a_sp_team } ;
	Uint8 sp ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	sp = gattai_tbl[pl_w->pl_personal] ;
	if( PL0A_LW( pl_w )->power_type == 0x27 ) sp = pl0a_sp_reppa ;
	if( PL0A_LW( pl_w )->power_type == 0x07 ) sp = pl0a_sp_barragex ;
	pl_w->pl_sp_type = sp ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
/*			The special move and its strength for each			*/
/*			assist												*/
/****************************************************************/
void pl0a_sp_assist( PLAYER_WORK *pl_w )
{
	static Uint8 assist_tbl[3][2] = {
		{ pl0a_sp_rising, 1 }, { pl0a_sp_repeat, 1 }, { pl0a_sp_drain, 1 },
	} ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = assist_tbl[pl_w->pl_personal][0] ;
	pl_w->atck_level = assist_tbl[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void pl0a_sp_zero( PLAYER_WORK *pl_w )
{
	static Uint8 zero_tbl[3][2] = {
		{ pl0a_sp_rising, 1 }, { pl0a_sp_repeat, 1 }, { pl0a_sp_drain, 1 },
	} ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = zero_tbl[pl_w->pl_personal][0] ;
	pl_w->atck_level = zero_tbl[pl_w->pl_personal][1] ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl0a.c" End of File	*/
