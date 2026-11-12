/* Sakura (character 22): command checks, her armour, normal attacks, dashes, appearance and win
   poses, special moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl22_def.h"

void pl22_main( PLAYER_WORK *pl_w );
void pl22_com_ck( PLAYER_WORK *pl_w );
void pl22_atck_set( PLAYER_WORK *pl_w );
void pl22_atck( PLAYER_WORK *pl_w );
void pl22_dash( PLAYER_WORK *pl_w );
void pl22_noki( PLAYER_WORK *pl_w );
void pl22_appear( PLAYER_WORK *pl_w );
void pl22_win( PLAYER_WORK *pl_w );
void pl22_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl22_catch_ck( PLAYER_WORK *pl_w );
void pl22_catch_set( PLAYER_WORK *pl_w );
void pl22_catch( PLAYER_WORK *pl_w );
void pl22_hold( PLAYER_WORK *pl_w );
int pl22_delay_ck( PLAYER_WORK *pl_w );
void pl22_t_kick( PLAYER_WORK *pl_w );
void pl22_upper( PLAYER_WORK *pl_w );
void pl22_sp_unison( PLAYER_WORK *pl_w );
void pl22_sp_gattai( PLAYER_WORK *pl_w );
void pl22_sp_assist( PLAYER_WORK *pl_w );
void pl22_sp_zero( PLAYER_WORK *pl_w );
int pl22_armor_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em );
void pl22_dash_set( PLAYER_WORK *pl_w );
void pl22_noki_set( PLAYER_WORK *pl_w );

/* each attack's table, punches then kicks: standing and crouching, jumping, super jumping */
static Sint8 nml_tbl[24] = {
	 1,  2,  1,  2,		/* lp */
	-1,  2, -1,  2,		/* mp */
	-1, -1, -1, -1,		/* hp */
	 1,  2,  1,  2,		/* lk */
	-1,  2, -1,  2,		/* mk */
	-1, -1, -1, -1,		/* hk */
} ;
static Sint8 jump_tbl[24] = {
	 1,  2,  1,  2,		/* lp */
	-1,  2, -1,  2,		/* mp */
	-1, -1, -1, -1,		/* hp */
	 1,  2,  1,  2,		/* lk */
	-1,  2, -1,  2,		/* mk */
	-1, -1, -1, -1,		/* hk */
} ;
static Sint8 sjump_tbl[24] = {
	 1,  2,  0,  2,		/* lp */
	-1,  2,  1,  2,		/* mp */
	-1, -1, -1,  2,		/* hp */
	 1,  2,  1,  2,		/* lk */
	-1,  2, -1,  2,		/* mk */
	-1, -1, -1, -1,		/* hk */
} ;

/* commands: time or steps, kind, the special move, then the stick and buttons; the second set
   is the dark form's (obj_flag set) */
static Uint16 com_shouou[] = { 3, 0, pl22_sp_shouou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shououken */
static Uint16 com_jshouou[] = { 3, 0, pl22_sp_jshouou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shououken in the air */
static Uint16 com_hadou[] = { 3, 0, pl22_sp_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Hadouken */
static Uint16 com_jhadou[] = { 3, 0, pl22_sp_jhadou, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Hadouken in the air */
static Uint16 com_shunpu[] = { 3, 0, pl22_sp_shunpu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Shunpukyaku */
static Uint16 com_jshunpu[] = { 3, 0, pl22_sp_jshunpu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Shunpukyaku in the air */
static Uint16 com_midare[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl22_sp_midare, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Midare Zakura (hyper) */
static Uint16 com_shinkuu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl22_sp_shinkuu, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ;	/* Shinkuu Hadouken (hyper) */
static Uint16 com_haru[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl22_sp_haru, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ;	/* Haru Ichiban (hyper) */
static Uint16 com_dark[] = { 3, COM_GROUND | COM_HYPER, pl22_sp_dark, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_LK } ;	/* The change to and from the dark form (level 3) */
static Uint16 com_dshouou[] = { 3, 0, pl22_sp_shouou, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shououken */
static Uint16 com_djshouou[] = { 3, 0, pl22_sp_jshouou, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ;	/* Shououken in the air */
static Uint16 com_dhadou[] = { 3, 0, pl22_sp_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hadouken */
static Uint16 com_djhadou[] = { 3, 0, pl22_sp_jhadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Hadouken in the air */
static Uint16 com_dmidare[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl22_sp_midare, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ;	/* Midare Zakura (hyper) */
static Uint16 com_dshinkuu[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl22_sp_dshinkuu, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;	/* Shinkuu Hadouken, dark form (hyper) */
static Uint16 com_ashura_f[] = { 3, COM_GROUND, pl22_sp_ashura_f, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_ATCK } ;	/* Ashura Senkuu forward */
static Uint16 com_ashura_b[] = { 3, COM_GROUND, pl22_sp_ashura_b, 0, KEY_BACK, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_ATCK } ;	/* Ashura Senkuu back */
static Uint16 com_shungoku[] = { 4, COM_GROUND | COM_HYPER, pl22_sp_shungoku, 0, KEY_LP, KEY_LP, KEY_FORWARD, KEY_LK } ;	/* Shun Goku Satsu (level 3 hyper) */

static Uint8 unk_tbl_00[] = { 0, 1 } ;	/* nothing here reads it */
static Sint16 shouou_time[] = { 16, 40 } ;
static Float shouou_spd_y[] = { DOT_Y( 8.0f ), DOT_Y( 12.0f ) } ;
static Sint16 jshouou_time[] = { 16, 24 } ;
static Float jshouou_spd_x[] = { 10.0f, DOT_X( 8.0f ) } ;
static Float jshouou_spd_y[] = { DOT_Y( 8.0f ), DOT_Y( 10.0f ) } ;
static Sint16 hadou_time[][2] = { { 50, 60 }, { 50, 60 } } ;
static Sint16 jhadou_time[][2] = { { 40, 54 }, { 40, 40 } } ;
static Float shunpu_spd[] = { DOT_X( 4.0f ), DOT_Y( 4.0f ), 10.0f, 18.75f, DOT_X( 5.0f ), DOT_Y( 6.0f ) } ;
static Float jshunpu_spd[] = { -2.5f, -DOT_X( 2.5f ) } ;
static Sint16 jshunpu_time[] = { 1, 3 } ;

static void ( *pl22_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl22_main,
	pl22_com_ck,
	pl22_atck_set,
	pl22_atck,
	pl22_dash,
	pl22_noki,
	pl22_appear,
	pl22_win,
	pl22_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl22_catch_ck,	/* returns a value */
	pl22_catch_set,
	pl22_catch,
	pl22_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl22_delay_ck,
	pl22_t_kick,
	pl22_upper,
	pl22_sp_unison,
	pl22_sp_gattai,
	pl22_sp_assist,
	pl22_sp_zero,
	0,
	0,
	0,
	0,
	( void ( * )( PLAYER_WORK *pl_w ) )pl22_armor_ck,	/* takes the attacker too */
	pl22_dash_set,
	pl22_noki_set,
} ;

/****************************************************************/
void pl22_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl22_tbl[i] ;
}

/*==============================================================*/
/*			In the air: once a jump, unless super jumping		*/
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

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 2 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl22_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl22_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 midare_ck( PLAYER_WORK *pl_w )
{
	Uint16 *com ;

	if( !pl_w->obj_flag ) com = com_midare ;
	else com = com_dmidare ;
	if( !com_nml_ck( pl_w, com, &pl_w->com_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl22_sp_midare ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			The move is the command's own						*/
/*==============================================================*/
static Uint8 shinkuu_ck( PLAYER_WORK *pl_w )
{
	Uint16 *com ;

	if( !pl_w->obj_flag ) com = com_shinkuu ;
	else com = com_dshinkuu ;
	if( !com_nml_ck( pl_w, com, &pl_w->com_w0 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = com[2] ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 haru_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_haru, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl22_sp_haru ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground or, with cpu_flag, in the air			*/
/*==============================================================*/
static Uint8 shouou_ck( PLAYER_WORK *pl_w )
{
	Uint16 *com ;

	if( !pl_w->obj_flag ){
		if( pl_w->cpu_flag && pl_w->pl_stance == STANCE_AIR ) com = com_jshouou ;
		else com = com_shouou ;
	} else {
		if( pl_w->cpu_flag && pl_w->pl_stance == STANCE_AIR ) com = com_djshouou ;
		else com = com_dshouou ;
	}
	if( !com_nml_ck( pl_w, com, &pl_w->com_w2 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_w->pl_sp_type = pl22_sp_shouou ;
	else pl_w->pl_sp_type = pl22_sp_jshouou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hadou_ck( PLAYER_WORK *pl_w )
{
	Uint16 *com ;

	if( !pl_w->obj_flag ){
		if( pl_w->cpu_flag && pl_w->pl_stance == STANCE_AIR ) com = com_jhadou ;
		else com = com_hadou ;
	} else {
		if( pl_w->cpu_flag && pl_w->pl_stance == STANCE_AIR ) com = com_djhadou ;
		else com = com_dhadou ;
	}
	if( !com_nml_ck( pl_w, com, &pl_w->com_w3 ) ) return 0 ;
	if( !jump_com_ck( pl_w ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w3 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_w->pl_sp_type = pl22_sp_hadou ;
	else pl_w->pl_sp_type = pl22_sp_jhadou ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 shunpu_ck( PLAYER_WORK *pl_w )
{
	Uint16 *com ;

	if( pl_w->cpu_flag && pl_w->pl_stance == STANCE_AIR ) com = com_jshunpu ;
	else com = com_shunpu ;
	if( !com_nml_ck( pl_w, com, &pl_w->com_w4 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w4 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_w->pl_sp_type = pl22_sp_shunpu ;
	else pl_w->pl_sp_type = pl22_sp_jshunpu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			A level 3 hyper combo; it clears three commands'	*/
/*			works												*/
/*==============================================================*/
static Uint8 dark_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_dark, &pl_w->com_w8 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	memset( &pl_w->com_w0, 0, 8 ) ;
	memset( &pl_w->com_w2, 0, 8 ) ;
	memset( &pl_w->com_w3, 0, 8 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl22_sp_dark ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ashura_f_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( !com_nml_ck( pl_w, com_ashura_f, &pl_w->com_w5 ) ) return 0 ;
	if( !sw_on_ck( pl_w, 0x0300, &sw ) && !sw_on_ck( pl_w, 0x0060, &sw ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->pl_sp_type = pl22_sp_ashura_f ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
static Uint8 ashura_b_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( !com_nml_ck( pl_w, com_ashura_b, &pl_w->com_w6 ) ) return 0 ;
	if( !sw_on_ck( pl_w, 0x0300, &sw ) && !sw_on_ck( pl_w, 0x0060, &sw ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->pl_sp_type = pl22_sp_ashura_b ;
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_SUPER ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			A level 3 hyper combo								*/
/*==============================================================*/
static Uint8 shungoku_ck( PLAYER_WORK *pl_w )
{
	if( !com_seq_ck( pl_w, com_shungoku, &pl_w->com_w7 ) ) return 0 ;
	if( *pl_w->gauge_level < 3 ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl22_sp_shungoku ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl22_delay_ck( PLAYER_WORK *pl_w )
{
	Uint16 *com ;

	if( !pl_w->obj_flag ) com = com_midare ;
	else com = com_dmidare ;
	if( com_nml_ck( pl_w, com, &pl_w->com_w ) && *pl_w->gauge_level ){
		pl_w->delay_sp_type = pl22_sp_midare ;
		/* from the European disc on, a hyper combo sets the attack level too */
#if BUILD_DATE >= 20000412
		pl_w->atck_level = 1 ;
#endif
		return 1 ;
	}
	if( !pl_w->obj_flag ) com = com_shinkuu ;
	else com = com_dshinkuu ;
	if( com_nml_ck( pl_w, com, &pl_w->com_w0 ) && *pl_w->gauge_level ){
		pl_w->delay_sp_type = com[2] ;
#if BUILD_DATE >= 20000412
		pl_w->atck_level = 1 ;
#endif
		return 1 ;
	}
	if( com_nml_ck( pl_w, com_haru, &pl_w->com_w1 ) && *pl_w->gauge_level ){
		if( 1 ) pl_w->delay_sp_type = pl22_sp_haru ;
		else pl_w->delay_sp_type = pl22_sp_haru ;
#if BUILD_DATE >= 20000412
		pl_w->atck_level = 1 ;
#endif
		return 1 ;
	}
	if( pl_w->obj_flag && com_seq_ck( pl_w, com_shungoku, &pl_w->com_w7 ) && *pl_w->gauge_level >= 3 ){
		pl_w->delay_sp_type = pl22_sp_shungoku ;
#if BUILD_DATE >= 20000412
		pl_w->atck_level = 1 ;
#endif
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl22_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( pl_w->obj_flag ){
		if( shungoku_ck( pl_w ) ) return ;
		if( ashura_f_ck( pl_w ) ) return ;
		if( ashura_b_ck( pl_w ) ) return ;
		if( dark_ck( pl_w ) ) return ;
		if( midare_ck( pl_w ) ) return ;
		if( shinkuu_ck( pl_w ) ) return ;
		if( haru_ck( pl_w ) ) return ;
		if( shouou_ck( pl_w ) ) return ;
		if( hadou_ck( pl_w ) ) return ;
		if( shunpu_ck( pl_w ) ) return ;
	} else {
		if( dark_ck( pl_w ) ) return ;
		if( midare_ck( pl_w ) ) return ;
		if( shinkuu_ck( pl_w ) ) return ;
		if( haru_ck( pl_w ) ) return ;
		if( shouou_ck( pl_w ) ) return ;
		if( hadou_ck( pl_w ) ) return ;
		if( shunpu_ck( pl_w ) ) return ;
	}
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/****************************************************************/
/*			The armour lasts only while the move that gave		*/
/*			it does												*/
/****************************************************************/
void pl22_main( PLAYER_WORK *pl_w )
{
	if( !PL22_LW( pl_w )->armor_flag ) return ;
	if( pl_w->obj_flag ) return ;
	if( pl_w->routine_1 != PL_NORM || pl_w->act_st != N_SPECIAL ){
		PL22_LW( pl_w )->armor_flag = 0 ;
		return ;
	}
	if( !pl_w->pl_sp_type ) return ;
	if( pl_w->pl_sp_type == pl22_sp_jshouou ) return ;
	PL22_LW( pl_w )->armor_flag = 0 ;
}

/****************************************************************/
/*			Whether a hit is taken through the armour			*/
/****************************************************************/
int pl22_armor_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	Float spd ;
	int n ;

	if( pl_w->pl_type != PL_SAKURA || !( em->atck_flag & HIT_KIND_MASK ) ) return 0 ;
	if( pl_w->obj_flag ) return 0 ;
	if( !PL22_LW( pl_w )->armor_flag ) return 0 ;
	if( PL22_LW( pl_w )->armor_flag > 0 ){
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

static void jump_atck( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A standing or crouching attack, from the tables;	*/
/*			the heavy kick with the stick forward is its own	*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	if( ( pl_w->atck_lever & KEY_FORWARD ) && pl_w->atck_pk == 1 && pl_w->nml_no == 2 && !pl_w->pl_stance ){
		pl_w->chain_tbl = 0 ;
		pl_w->nml_level = 1 ;
		pl_w->atck_no = pl22_at_fwd_hk ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Stand_K, 4 ) ;
	} else {
		pl_w->chain_tbl = &nml_tbl[pl_w->atck_pk * 12 + pl_w->nml_no * 4] ;
		pl_w->nml_level = pl_w->nml_no ;
		pl_w->atck_no = pl_w->pl_stance * 6 + pl_w->atck_pk * 3 + pl_w->nml_no ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, ( pl_w->pl_stance << 1 ) + pl_w->atck_pk + Char_Stand_P, pl_w->nml_no ) ;
	}
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	Sint8 *tbl ;
	register Sint16 no = pl_w->atck_pk * 12 + pl_w->nml_no * 4 ;

	if( !pl_w->sjump_flag ) tbl = &jump_tbl[no] ;
	else tbl = &sjump_tbl[no] ;
	pl_w->chain_tbl = tbl ;
	pl_w->nml_level = pl_w->nml_no ;
	pl_w->atck_no = pl_w->atck_pk * 3 + pl_w->nml_no + 12 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->atck_pk + Char_Jump_P, pl_w->nml_no ) ;
	pl_se_req( pl_w, pl_w->nml_no + 20 ) ;
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
	} else if( pl_w->jump_atk_cnt & JATK_K_MASK ) jump_atck( pl_w ) ;
}

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl22_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
void pl22_atck( PLAYER_WORK *pl_w )
{
	switch( pl_w->atck_type ){
	case 3:
		pl_smoke_set( pl_w ) ;
	case 0:
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_brake_stop( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		else if( pl_w->cg_flag1 ){
			pl_w->atck_no = pl_w->cg_flag1 ;
			HIT_RESET( pl_w ) ;
			pl_w->cg_flag1 = 0 ;
		}
		break ;
	case 2:
		pl_jump_hit_ck( pl_w ) ;
		pl_sjump_ctrl( pl_w ) ;
	case 1:
		pl_jump_move( pl_w ) ;
		pl_sjump_top_ck( pl_w ) ;
		if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
		if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
		break ;
	}
}

/****************************************************************/
/*			The crouching heavy punch or kick that launches		*/
/****************************************************************/
void pl22_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		if( pl_w->atck_pk == 0 ){
			pl_w->atck_no = pl22_at_upper_p ;
			pl_w->pl_stance = STANCE_CROUCH ;
			pl_se_req( pl_w, SE_SWING_H ) ;
			pl_char_set( pl_w, Char_Extra, 3 ) ;
		} else {
			pl_w->atck_no = pl22_at_upper_k ;
			pl_w->pl_stance = STANCE_STAND ;
			pl_se_req( pl_w, SE_SWING_M ) ;
			pl_char_set( pl_w, Char_Extra, 4 ) ;
		}
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
static void dash_stop( PLAYER_WORK *pl_w );
static void dash_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash; with obj_flag it glides instead			*/
/****************************************************************/
void pl22_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_start, dash_move, dash_stop, dash_end,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
	if( pl_w->obj_flag ) pl_smoke_set( pl_w ) ;
}

/*==============================================================*/
static void dash_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void dash_move( PLAYER_WORK *pl_w )
{
	if( !pl_w->obj_flag ){
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( !pl_w->cg_type ) return ;
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
			pl_w->routine_2 += 2 ;
			pl_char_set( pl_w, Char_Dash, 6 ) ;
		} else pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_stop( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
	}
	pl_smoke_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *pl_w )
{
	if( !pl_w->obj_flag ){
		if( !pl_w->cg_type ) pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_start( PLAYER_WORK *pl_w );
static void noki_move( PLAYER_WORK *pl_w );
static void noki_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash										*/
/****************************************************************/
void pl22_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_start, noki_move, noki_end,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
	if( pl_w->obj_flag ) pl_smoke_set( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( !pl_w->obj_flag ) noki_start( pl_w ) ;
	else {
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 10.0f ) : DOT_X( 10.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? DOT_X( 0.25f ) : -DOT_X( 0.25f ) ;
		pl_char_set( pl_w, Char_Dash, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ) return ;
	pl_w->routine_2 ++ ;
	pl_w->cg_type = 0 ;
	if( !pl_w->obj_flag ){
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? 0.36458331f : -0.36458331f ;
		pl_w->pos.x += pl_w->pl_dir ? -40 : 40 ;
	} else noki_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void noki_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->obj_flag ){
		pl_char_move( pl_w ) ;
		if( pl_w->cg_type ){
			pl_w->routine_2 ++ ;
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
		}
		if( pl_w->cg_flag1 ) pl_smoke_set2( pl_w, 3 ) ;
	} else {
		if( pl_w->acc.x * pl_w->spd.x > 0.0f ){
			pl_w->routine_2 ++ ;
			pl_char_set( pl_w, Char_Dash, 7 ) ;
		} else pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_wait( PLAYER_WORK *pl_w );
static void appear_run( PLAYER_WORK *pl_w );
static void appear_stop( PLAYER_WORK *pl_w );
static void appear_end( PLAYER_WORK *pl_w );
static void appear_late_wait( PLAYER_WORK *pl_w );
static void appear_late_init( PLAYER_WORK *pl_w );
static Float move_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The entrance; she runs in, or with her partner		*/
/*			on screen, slides in								*/
/****************************************************************/
void pl22_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_wait, appear_run, appear_stop, appear_end,
		appear_late_wait, appear_late_init, appear_run, appear_stop, appear_end,
	} ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		if( pl_w->target_w->pl_type == pl_w->pl_type ){
			if( !pl_w->target_w->routine_2 ){
				if( !( Rnd() & 1 ) ) pl_w->routine_3 = 5 ;
			} else if( pl_w->target_w->routine_3 < 5 ) pl_w->routine_3 = 5 ;
		}
	}
	appear_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	Float x, spd ;

	pl_w->routine_3 ++ ;
	pl_w->disp_flag = 1 ;
	PL22_LW( pl_w )->save_x = pl_w->pos.x ;
	x = 320.0f ;
	spd = 10.0f ;
	if( pl_w->disp_dir ){
		x = -320.0f ;
		spd = -10.0f ;
	}
	pl_w->spd.x = spd ;
	pl_w->pos.x -= x ;
	pl_w->acc.x = pl_w->spd.y = pl_w->acc.y = 0.0f ;
	ef_color_set( pl_w, 1, pl_w->pl_color * 2 + 8, 2 ) ;
	pls22_00_set( pl_w, 2 ) ;
	pl_char_set( pl_w, Char_Appear, 3 ) ;
}

/*--------------------------------------------------------------*/
static void appear_wait( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 18 ;
		pl_char_set( pl_w, Char_Appear, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_run( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->acc.x = pl_w->disp_dir ? 0.4296875f : -0.4296875f ;
		pl_char_set( pl_w, Char_Appear, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_stop( PLAYER_WORK *pl_w )
{
	if( move_ck( pl_w ) < 0.0f ){
		pl_w->routine_3 ++ ;
		pl_w->pos.x = PL22_LW( pl_w )->save_x ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
	} else {
		pl_char_move( pl_w ) ;
		pl_smoke_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
		pl_w->routine_3 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void appear_late_wait( PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st < VS_APPEAR ) pl_w->disp_flag = 0 ;
	else {
		pl_w->routine_3 ++ ;
		appear_late_init( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_late_init( PLAYER_WORK *pl_w )
{
	Float x, spd ;

	pl_w->disp_flag = 1 ;
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 32 ;
	PL22_LW( pl_w )->save_x = pl_w->pos.x ;
	x = 640.0f ;
	spd = DOT_X( 8.0f ) ;
	if( pl_w->disp_dir ){
		x = -640.0f ;
		spd = -DOT_X( 8.0f ) ;
	}
	pl_w->spd.x = spd ;
	pl_w->pos.x -= x ;
	pl_w->acc.x = pl_w->spd.y = pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*==============================================================*/
/*			A command during the win pose: steps through		*/
/*			four inputs											*/
/*==============================================================*/
static void win_cmd_ck( PLAYER_WORK *pl_w )
{
	switch( PL22_LW( pl_w )->cmd_cnt ){
	case 0:
		if( pl_w->pad_sw.on & KEY_UP ) PL22_LW( pl_w )->cmd_cnt ++ ;
		break ;
	case 1:
		if( pl_w->pad_sw.now & KEY_LEVER ) PL22_LW( pl_w )->cmd_cnt ++ ;
		break ;
	case 2:
		if( pl_w->pad_sw.on & KEY_DOWN ) PL22_LW( pl_w )->cmd_cnt ++ ;
		break ;
	case 3:
		if( pl_w->pad_sw.now & KEY_LEVER ){
			PL22_LW( pl_w )->cmd_cnt = 0 ;
			PL22_LW( pl_w )->cmd_flag = 1 ;
		}
		break ;
	}
}

static Uint8 win_tbl[] = { 0, 0, 4, 5, 5 } ;
static Uint8 win_rnd_tbl[] = { 0, 1, 2, 3, 4, 5, 5, 5, 0, 1, 2, 3, 4, 5, 5, 5 } ;

/* the win poses; buttons held pick one */

/****************************************************************/
void pl22_win( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	Uint8 no ;
	Float x ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		if( !pl_w->type ){
			if( vs_sys_work.vs_end_flag ){
				sw = pl_w->pad_sw.now & KEY_ATCK ;
				if( sw ){
					if( sw & KEY_LP ) no = 0 ;
					if( sw & KEY_HP ) no = 1 ;
					if( ( sw & KEY_PUNCH ) == 0x0300 ) no = 2 ;
					if( sw & KEY_LK ) no = 3 ;
					if( sw & KEY_HK ) no = 4 ;
					if( ( sw & KEY_KICK ) == 0x0060 ) no = 5 ;
					pl_w->routine_3 = no ;
				} else pl_w->routine_3 = win_rnd_tbl[Rnd() & 15] ;
				if( pl_w->routine_3 == 5 && vs_sys_work.vs_end_flag ){
					x = scr_work.scr_left + 320.0f ;
					if( pl_w->pos.x >= x ){
						pl_w->disp_dir = 0 ;
						pl_w->pl_dir = 0 ;
					} else {
						pl_w->disp_dir = 1 ;
						pl_w->pl_dir = 1 ;
					}
					if( Sys->game_timer0 & 1 ){
						pl_w->routine_3 = 6 ;
						pl_w->pl_dir = pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
					}
				}
			} else pl_w->routine_3 = 3 ;
			switch( pl_w->routine_3 ){
			case 0:
				pl_w->timer0 = 0 ;
				ef_color_set( pl_w, 1, 2, 1 ) ;
				pl_char_set( pl_w, Char_Win, pl_w->routine_3 ) ;
				pls22_00_set( pl_w, 0 ) ;
				break ;
			case 1:
			case 2:
			case 3:
			case 4:
				pl_w->timer0 = 0 ;
				PL22_LW( pl_w )->cmd_cnt = PL22_LW( pl_w )->unk_06 = PL22_LW( pl_w )->cmd_flag = 0 ;
				pl_char_set( pl_w, Char_Win, pl_w->routine_3 ) ;
				break ;
			case 5:
				PL22_LW( pl_w )->win5_flag = 0 ;
				pls22_00_set( pl_w, 4 ) ;
				ef_color_set( pl_w, 1, pl_w->pl_color * 2 + 8, 2 ) ;
				break ;
			case 6:
				pls22_00_set( pl_w, 5 ) ;
				PL22_LW( pl_w )->win6_flag = 0 ;
				ef_color_set( pl_w, 1, pl_w->pl_color * 2 + 8, 2 ) ;
				pl_char_set( pl_w, Char_Win, 12 ) ;
				break ;
			}
		} else pl_char_set( pl_w, Char_Win, win_tbl[pl_w->type] ) ;
		return ;
	} else if( !pl_w->type ){
		if( pl_w->routine_3 == 5 ){
			pl_w->win_wait = 0xff ;
			if( PL22_LW( pl_w )->win5_flag ){
				PL22_LW( pl_w )->win5_flag = 0 ;
				pl_char_set( pl_w, Char_Win, 3 ) ;
			}
		} else if( pl_w->routine_3 == 6 ){
			pl_w->win_wait = 0xff ;
			if( PL22_LW( pl_w )->win6_flag ){
				PL22_LW( pl_w )->win6_flag = 0 ;
				pl_char_set( pl_w, Char_Win, 13 ) ;
			}
			goto move ;
		} else {
			pl_char_move( pl_w ) ;
			if( pl_w->timer0 ) pl_w->win_wait = 0xff ;
			win_cmd_ck( pl_w ) ;
			if( pl_w->cg_type ){
				pl_w->timer0 = 1 ;
				pl_w->cg_type = 0 ;
				if( PL22_LW( pl_w )->cmd_flag ) shl22_00_set( pl_w, 3 ) ;
				else shl22_00_set( pl_w, 2 ) ;
			}
			return ;
		}
	}
move :
	if( 0 ) pl_w->cg_type = 0 ;
	pl_char_move( pl_w ) ;
}

static void sp_shouou( PLAYER_WORK *pl_w );
static void sp_shunpu( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_hadou( PLAYER_WORK *pl_w );
static void sp_midare( PLAYER_WORK *pl_w );
static void sp_jhadou( PLAYER_WORK *pl_w );
static void sp_shinkuu( PLAYER_WORK *pl_w );
static void sp_dshinkuu( PLAYER_WORK *pl_w );
static void sp_jshunpu( PLAYER_WORK *pl_w );
static void sp_jshouou( PLAYER_WORK *pl_w );
static void sp_haru( PLAYER_WORK *pl_w );
static void sp_ashura_f( PLAYER_WORK *pl_w );
static void sp_shungoku( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_dark( PLAYER_WORK *pl_w );

/****************************************************************/
void pl22_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_shouou, sp_shunpu, sp_chouhatsu, pl_stand_ret, pl_stand_ret,
		sp_hadou, sp_midare, sp_jhadou, sp_shinkuu, pl_stand_ret,
		pl_stand_ret, sp_dshinkuu, sp_jshunpu, sp_jshouou, sp_haru,
		pl_stand_ret, sp_ashura_f, sp_ashura_f, sp_shungoku, sp_snapback,
		sp_dark,					/* 20 the change to and from the dark form */
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

/*==============================================================*/
/*			Moves her; below zero once the speed has changed	*/
/*			sign												*/
/*==============================================================*/
static Float move_ck( PLAYER_WORK *pl_w )
{
	Float spd ;

	spd = pl_w->spd.x ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	return pl_w->spd.x * spd ;
}

/*==============================================================*/
/*			The taunt											*/
/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, 0 ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->atck_no = pl22_at_chouhatsu ;
		HIT_RESET( pl_w ) ;
		pl_char_set( pl_w, Char_Extra, 0 ) ;
	} else if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			Changes to the dark form or back (obj_flag)			*/
/*==============================================================*/
static void sp_dark( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( !pl_w->routine_2 ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->obj_flag = pl_w->obj_flag ^ 1 ;
		pl_color_set( pl_w, 0 ) ;
		pl_dir_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Super, 19 ) ;
	} else {
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->cg_type ){
			pl_w->cg_type = 0 ;
			ofs.x = -DOT_X( 10.0f ) ;
			ofs.y = DOT_Y( 72.0f ) ;
			pl_hyper_start( pl_w, &ofs, 3 ) ;
		}
	}
}

/*==============================================================*/
static void ashura_f_color( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 < 0 ){
		pl_w->timer0 = 2 ;
		pls22_00_set( pl_w, 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ashura_f_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ashura_f_move( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	ashura_f_color( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->acc.x * pl_w->spd.x < 0.0f ){
		if( !( pl_w->wall_flag & ( 1 << ( pl_w->pl_dir ^ 1 ) ) ) ){
			pl_char_move( pl_w ) ;
			return ;
		}
	}
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 44 ) ;
}

/*--------------------------------------------------------------*/
static void ashura_f_start( PLAYER_WORK *pl_w )
{
	Float x ;

	pl_dir_set( pl_w ) ;
	ashura_f_color( pl_w ) ;
	pl_w->push_ng_timer = 2 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		x = pl_w->cg_prm ;
		x *= 1.6666666f ;
		pl_w->cg_prm = 0 ;
		pl_w->pos.x += pl_w->pl_dir ? -x : x ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
	}
}

/*==============================================================*/
static void ashura_f_init( PLAYER_WORK *pl_w )
{
	Float spd, acc ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_gauge_add( pl_w, 2 ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 0 ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->pl_sp_type == pl22_sp_ashura_f ){
		spd = pl_w->atck_pk ? DOT_X( 10.0f ) : 25.0f ;
		acc = pl_w->atck_pk ? -0.67708331f : -0.78125f ;
	} else {
		spd = pl_w->atck_pk ? -DOT_X( 10.0f ) : -DOT_X( 14.0f ) ;
		acc = pl_w->atck_pk ? 0.67708331f : 0.78125f ;
	}
	pl_w->spd.x = pl_w->pl_dir ? spd : -spd ;
	pl_w->acc.x = pl_w->pl_dir ? acc : -acc ;
	pl_char_set( pl_w, Char_Special, 43 ) ;
}

/*==============================================================*/
/*			Ashura Senkuu, forward or (pl22_sp_ashura_b) back	*/
/*==============================================================*/
static void sp_ashura_f( PLAYER_WORK *pl_w )
{
	static void ( *ashura_f_jmp[])( PLAYER_WORK *pl_w ) = {
		ashura_f_init, ashura_f_start, ashura_f_move, ashura_f_end,
	} ;

	ashura_f_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shungoku_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_sp_ef_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = pl_w->pl_dir ? DOT_X( 3.5f ) : -DOT_X( 3.5f ) ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->timer0 = 40 ;
	pl_char_set( pl_w, Char_Super, 18 ) ;
}

/*--------------------------------------------------------------*/
static void shungoku_wait( PLAYER_WORK *pl_w )
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
		ofs.x = 0.0f ;
		ofs.y = DOT_Y( 76.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_start( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
}

/*==============================================================*/
/*			Runs in; catches the opponent if it can				*/
/*==============================================================*/
static void shungoku_dash( PLAYER_WORK *pl_w )
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
	if( ( pl_w->wall_flag & ( 1 << ( pl_w->pl_dir ^ 1 ) ) ) || -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( ( em = pl_catch_ck( pl_w ) ) ){
		pl_w->routine_2 ++ ;
		scr_excep_set( pl_w, 6, 6 ) ;
		pl_char_set( pl_w, Char_Catch, 6 ) ;
		ofs.x = -DOT_X( 50.0f ) ;
		ofs.y = DOT_Y( 74.0f ) ;
		nage_hit_set( pl_w, &ofs ) ;
		pl_w->timer0 = 16 ;
		pl_w->catch_no = 0xc3 ;
		pl_sp_catch_set( pl_w, em ) ;
		pl_w->hit_stop = 10 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		if( PL_CK( pl_w->catch_w, PL_SERVBOT ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 60.0f ) ;
		if( PL_CK( pl_w->catch_w, PL_ROLL ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 27.0f ) ;
		if( PL_CK( pl_w->catch_w, PL_MEGAMAN ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 23.0f ) ;
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
		pl_w->timer1 = 4 ;
		pl_char_set( pl_w, Char_Catch, 7 ) ;
		flash_dark_long_set() ;
		flash_work.timer = 2 ;
	}
}

/*==============================================================*/
/*			The flurry of hits, with sparks around her			*/
/*==============================================================*/
static void shungoku_hit( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	VEC3 pos ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->catch_flag = 1 ;
	flash_work.timer = 2 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	if( -- pl_w->timer1 <= 0 ){
		pl_w->timer1 = 4 ;
		if( Rnd() & 1 ) pl_se_req( pl_w, 0 ) ;
		else pl_se_req( pl_w, 1 ) ;
		pos.x = ( Rnd() & 127 ) * 1.6666666f ;
		pos.y = ( Rnd() & 63 ) * DOT_Y( 1.0f ) ;
		if( Rnd() & 1 ) pos.x = -pos.x ;
		if( Rnd() & 1 ) pos.y = -pos.y ;
		pos.x += pl_w->pos.x ;
		pos.y += pl_w->pos.y + DOT_Y( 96.0f ) ;
		pos.z = pl_w->pos.z ;
		hitmark_set1( &pos, pl_w->disp_dir, 1, 0 ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		em = pl_w->catch_w ;
		combo_set( em, pl_w, 16 ) ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 8 ;
		em->pl_stance = STANCE_STAND ;
		scr_excep_set( pl_w, 0, 0 ) ;
		em->atck_no = pl22_at_shungoku ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->timer0 = 64 ;
		flash_work.timer = 0 ;
		if( pl_w->disp_dir ) pl_w->pos.x += DOT_X( 16.0f ) ;
		else pl_w->pos.x -= DOT_X( 16.0f ) ;
		pl_catch_dir_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shungoku_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( -- pl_w->timer0 <= 0 ){
		pl_w->target_w->catch_ng_timer = 48 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			The throwing hyper combo							*/
/*==============================================================*/
static void sp_shungoku( PLAYER_WORK *pl_w )
{
	static void ( *shungoku_jmp[])( PLAYER_WORK *pl_w ) = {
		shungoku_init, shungoku_wait, shungoku_start, shungoku_dash, shungoku_hold, shungoku_hit, shungoku_end,
	} ;

	shungoku_jmp[pl_w->routine_2]( pl_w ) ;
}

static void shouou_init( PLAYER_WORK *pl_w );
static void shouou_wait( PLAYER_WORK *pl_w );
static void shouou_run( PLAYER_WORK *pl_w );
static void shouou_guard( PLAYER_WORK *pl_w );
static void shouou_end( PLAYER_WORK *pl_w );
static void shouou_upper( PLAYER_WORK *pl_w );
static void shouou_up( PLAYER_WORK *pl_w );
static void shouou_fall( PLAYER_WORK *pl_w );
static void shouou_stop( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rising uppercut									*/
/*==============================================================*/
static void sp_shouou( PLAYER_WORK *pl_w )
{
	static void ( *shouou_jmp[])( PLAYER_WORK *pl_w ) = {
		shouou_init, shouou_wait, shouou_run, shouou_guard, shouou_end,
		shouou_upper, shouou_up, shouou_fall, shouou_stop,
	} ;

	shouou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shouou_init( PLAYER_WORK *pl_w )
{
	static Sint16 shouou_atck_tbl[] = { 62, 64 } ;

	pl_w->routine_2 ++ ;
	PL22_LW( pl_w )->armor_end = 0 ;
	PL22_LW( pl_w )->armor_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = shouou_atck_tbl[pl_w->atck_level] ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = shouou_time[pl_w->atck_level] ;
	pl_w->spd.x = DOT_X( 7.0f ) ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_sp_ef_set( pl_w ) ;
	pl_voice_req( pl_w, 20 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level ) ;
}

/*--------------------------------------------------------------*/
static void shouou_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( !pl_w->obj_flag ){
			pls22_00_set( pl_w, 6 ) ;
			PL22_LW( pl_w )->armor_flag = 1 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shouou_run( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag & HR_GUARD ){
		pl_w->routine_2 ++ ;
		PL22_LW( pl_w )->armor_end = 2 ;
		PL22_LW( pl_w )->armor_flag = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		if( pl_w->atck_level ) pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_air_set( pl_w ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	} else if( pl_w->hit_flag ){
rise :
		pl_w->routine_2 = 5 ;
		PL22_LW( pl_w )->armor_end = 1 ;
		PL22_LW( pl_w )->armor_flag = 0 ;
		pl_w->spd.y = shouou_spd_y[pl_w->atck_level] ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		if( pl_w->obj_flag ){
			pl_w->acc.x = DOT_X( 0.25f ) ;
			if( pl_w->disp_dir ) pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	} else {
		pl_char_move( pl_w ) ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( -- pl_w->timer0 <= 0 ){
			if( pl_w->obj_flag ) goto rise ;
			pl_w->routine_2 = 8 ;
			PL22_LW( pl_w )->armor_end = 2 ;
			PL22_LW( pl_w )->armor_flag = 0 ;
			pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.5f ) : DOT_X( 0.5f ) ;
			pl_char_set( pl_w, Char_Special, 2 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shouou_guard( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
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
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shouou_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shouou_upper( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = DOT_X( 0.25f ) ;
		if( pl_w->disp_dir ) pl_w->acc.x = -pl_w->acc.x ;
		pl_air_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shouou_up( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( move_ck( pl_w ) < 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.y = -DOT_Y( 1.0f ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shouou_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 = 4 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 10 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shouou_stop( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( move_ck( pl_w ) < 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->routine_2 = 4 ;
		pl_char_set( pl_w, Char_Special, 3 ) ;
	} else pl_smoke_set2( pl_w, 3 ) ;
}

static void jshouou_init( PLAYER_WORK *pl_w );
static void jshouou_wait( PLAYER_WORK *pl_w );
static void jshouou_run( PLAYER_WORK *pl_w );
static void jshouou_guard( PLAYER_WORK *pl_w );
static void jshouou_end( PLAYER_WORK *pl_w );
static void jshouou_up( PLAYER_WORK *pl_w );
static void jshouou_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The uppercut with a step in							*/
/*==============================================================*/
static void sp_jshouou( PLAYER_WORK *pl_w )
{
	static void ( *jshouou_jmp[])( PLAYER_WORK *pl_w ) = {
		jshouou_init, jshouou_wait, jshouou_run, jshouou_guard, jshouou_end, jshouou_up, jshouou_fall,
	} ;

	jshouou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jshouou_init( PLAYER_WORK *pl_w )
{
	static Sint16 jshouou_atck_tbl[] = { 94, 96 } ;

	pl_w->routine_2 ++ ;
	PL22_LW( pl_w )->armor_end = 0 ;
	PL22_LW( pl_w )->armor_flag = 0 ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->atck_no = jshouou_atck_tbl[pl_w->atck_level] ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = jshouou_time[pl_w->atck_level] ;
	pl_w->spd.x = jshouou_spd_x[pl_w->atck_level] ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_voice_req( pl_w, 20 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 29 ) ;
	if( pl_w->obj_flag ){
		PL22_LW( pl_w )->armor_flag = 0 ;
		pls22_00_set( pl_w, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jshouou_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->pos.x += pl_w->pl_dir ? 32 : -32 ;
		if( !pl_w->obj_flag ){
			PL22_LW( pl_w )->armor_flag = 1 ;
			pls22_00_set( pl_w, 6 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void jshouou_run( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->hit_flag & HR_GUARD ){
		pl_w->routine_2 ++ ;
		PL22_LW( pl_w )->armor_end = 2 ;
		PL22_LW( pl_w )->armor_flag = 0 ;
		pl_w->spd.x = pl_w->pl_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		if( pl_w->atck_level ) pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Special, 4 ) ;
	} else if( pl_w->hit_flag ){
		pl_w->routine_2 = 5 ;
		PL22_LW( pl_w )->armor_end = 1 ;
		PL22_LW( pl_w )->armor_flag = 0 ;
		pl_w->spd.y = DOT_Y( 1.0f ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 32 ) ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		if( !pl_w->timer0 -- ){
			pl_w->routine_2 = 6 ;
			PL22_LW( pl_w )->armor_end = 2 ;
			PL22_LW( pl_w )->armor_flag = 0 ;
			pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.125f ) : DOT_X( 0.125f ) ;
			pl_w->spd.y = DOT_Y( 1.0f ) ;
			pl_w->acc.y = -DOT_Y( 0.375f ) ;
			pl_char_set( pl_w, Char_Special, 31 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void jshouou_guard( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Special, 5 ) ;
		pl_land_ef_set( pl_w ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jshouou_end( PLAYER_WORK *pl_w )
{
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.x += pl_w->spd.x ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void jshouou_up( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.y = jshouou_spd_y[pl_w->atck_level] ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x /= 2.0f ;
		pl_w->pos.y += 10.0f ;
	}
	if( move_ck( pl_w ) < 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 1.0f ) ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jshouou_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( move_ck( pl_w ) < 0.0f ){
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 = 4 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 10 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

static void hadou_init( PLAYER_WORK *pl_w );
static void hadou_shot( PLAYER_WORK *pl_w );
static void hadou_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The fireball										*/
/*==============================================================*/
static void sp_hadou( PLAYER_WORK *pl_w )
{
	static void ( *hadou_jmp[])( PLAYER_WORK *pl_w ) = {
		hadou_init, hadou_shot, hadou_end,
	} ;

	hadou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hadou_init( PLAYER_WORK *pl_w )
{
	static Uint8 hadou_atck_tbl[] = { 0x44, 0x46, 0x6d, 0x6e } ;
	static Uint8 hadou_char_tbl[] = { 17, 18, 45, 46 } ;
	int no ;

	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	if( pl_w->obj_flag ){ }
	pl_gauge_add( pl_w, 8 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	no = pl_w->obj_flag & 1 ;
	no <<= 1 ;
	no += pl_w->atck_level ;
	pl_w->timer0 = hadou_time[pl_w->obj_flag][pl_w->atck_level] ;
	pl_w->atck_no = hadou_atck_tbl[no] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, hadou_char_tbl[no] ) ;
}

/*--------------------------------------------------------------*/
static void hadou_shot( PLAYER_WORK *pl_w )
{
	static Uint8 hadou_end_tbl[] = { 19, 20, 47, 48 } ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( !pl_w->obj_flag ) shl22_00_set( pl_w, 0 ) ;
		else shl22_00_set( pl_w, 6 ) ;
		pl_voice_req( pl_w, 30 ) ;
		pl_w->unk_27b = 0 ;
		pl_w->cancel_timer = 16 ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, hadou_end_tbl[pl_w->obj_flag * 2 + pl_w->atck_level] ) ;
	}
}

/*--------------------------------------------------------------*/
static void hadou_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jhadou_init( PLAYER_WORK *pl_w );
static void jhadou_shot( PLAYER_WORK *pl_w );
static void jhadou_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The fireball in the air								*/
/*==============================================================*/
static void sp_jhadou( PLAYER_WORK *pl_w )
{
	static void ( *jhadou_jmp[])( PLAYER_WORK *pl_w ) = {
		jhadou_init, jhadou_shot, jhadou_end,
	} ;

	jhadou_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jhadou_init( PLAYER_WORK *pl_w )
{
	static Uint8 jhadou_atck_tbl[][2] = { { 0x4e, 0x50 }, { 0x6f, 0x70 } } ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl_w->atck_level + 68 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x /= 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_w->timer0 = jhadou_time[pl_w->obj_flag][pl_w->atck_level] ;
	pl_w->atck_no = jhadou_atck_tbl[pl_w->obj_flag][pl_w->atck_level] ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 21 ) ;
}

/*--------------------------------------------------------------*/
static void jhadou_shot( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ) pl_w->pos.y = pl_w->ground_y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		if( !pl_w->obj_flag ) shl22_00_set( pl_w, 1 ) ;
		else shl22_00_set( pl_w, 7 ) ;
		pl_voice_req( pl_w, 30 ) ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 23 ) ;
	}
}

/*--------------------------------------------------------------*/
static void jhadou_end( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y > pl_w->pos.y ) pl_w->pos.y = pl_w->ground_y ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void shunpu_init( PLAYER_WORK *pl_w );
static void shunpu_start( PLAYER_WORK *pl_w );
static void shunpu_jump( PLAYER_WORK *pl_w );
static void shunpu_land( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hopping kick									*/
/*==============================================================*/
static void sp_shunpu( PLAYER_WORK *pl_w )
{
	static void ( *shunpu_jmp[])( PLAYER_WORK *pl_w ) = {
		shunpu_init, shunpu_start, shunpu_jump, shunpu_land,
	} ;

	shunpu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shunpu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_dir_set( pl_w ) ;
	if( pl_w->team_type == 8 ){
		pl_w->spd.x = shunpu_spd[4] ;
		pl_w->spd.y = shunpu_spd[5] ;
		pl_w->atck_no = pl22_at_shunpu_assist ;
		HIT_RESET( pl_w ) ;
	} else {
		pl_w->spd.x = shunpu_spd[pl_w->atck_level * 2] ;
		pl_w->spd.y = shunpu_spd[pl_w->atck_level * 2 + 1] ;
		pl_w->atck_no = ( pl_w->atck_level << 1 ) + 75 ;
		HIT_RESET( pl_w ) ;
	}
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->spd.x = pl_w->pl_dir ? pl_w->spd.x : -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_gauge_add( pl_w, 8 ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 12 ) ;
}

/*--------------------------------------------------------------*/
static void shunpu_start( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_air_set( pl_w ) ;
		pl_sp_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shunpu_jump( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 14 ) ;
		pl_dir_set( pl_w ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shunpu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jshunpu_init( PLAYER_WORK *pl_w );
static void jshunpu_air( PLAYER_WORK *pl_w );
static void jshunpu_land( PLAYER_WORK *pl_w );
static void jshunpu_fall( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rolling kick that turns her round				*/
/*==============================================================*/
static void sp_jshunpu( PLAYER_WORK *pl_w )
{
	static void ( *jshunpu_jmp[])( PLAYER_WORK *pl_w ) = {
		jshunpu_init, jshunpu_air, jshunpu_land, jshunpu_fall,
	} ;

	jshunpu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jshunpu_init( PLAYER_WORK *pl_w )
{
	Float spd ;

	pl_w->routine_2 ++ ;
	pl_w->atck_no = ( pl_w->atck_level << 1 ) + 90 ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 4 ) ;
	pl_dir_set( pl_w ) ;
	pl_w->timer0 = jshunpu_time[pl_w->atck_level] ;
	spd = jshunpu_spd[pl_w->atck_level] ;
	if( pl_w->pl_dir ){
		spd = -spd ;
		if( pl_w->spd.x < 0.0f ) spd = -spd ;
	} else if( pl_w->spd.x > 0.0f ) spd = -spd ;
	pl_w->spd.x += spd ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 25 ) ;
}

/*--------------------------------------------------------------*/
static void jshunpu_air( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 && -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 = 3 ;
		pl_char_set( pl_w, Char_Special, 34 ) ;
		return ;
	}
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 = 2 ;
		pl_w->atck_no = pl22_at_jshunpu_land ;
		HIT_RESET( pl_w ) ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 34 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jshunpu_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->pl_dir ^= 1 ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jshunpu_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ) pl_stand_ret( pl_w ) ;
	else if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

static void midare_init( PLAYER_WORK *pl_w );
static void midare_wait( PLAYER_WORK *pl_w );
static void midare_run( PLAYER_WORK *pl_w );
static void midare_hit( PLAYER_WORK *pl_w );
static void midare_upper( PLAYER_WORK *pl_w );
static void midare_up( PLAYER_WORK *pl_w );
static void midare_fall( PLAYER_WORK *pl_w );
static void midare_stop( PLAYER_WORK *pl_w );
static void midare_guard( PLAYER_WORK *pl_w );
static void midare_end( PLAYER_WORK *pl_w );
static void midare_catch( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The rush that carries the opponent along			*/
/*==============================================================*/
static void sp_midare( PLAYER_WORK *pl_w )
{
	static void ( *midare_jmp[])( PLAYER_WORK *pl_w ) = {
		midare_init, midare_wait, midare_run, midare_hit, midare_upper, midare_up, midare_fall, midare_stop,
		midare_guard, midare_end, midare_catch,
	} ;

	midare_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void midare_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	pl_w->atck_no = pl22_at_midare ;
	HIT_RESET( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->timer0 = 30 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = pl_w->pl_dir ? 10.0f : -10.0f ;
	pl_char_set( pl_w, Char_Super, 1 ) ;
	pl_sp_ef_set( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void midare_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 68.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void midare_run( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !pl_w->hit_flag ){
		pl_char_move( pl_w ) ;
		if( !pl_w->cg_type ){
			pl_w->pos.x += pl_w->spd.x ;
			pl_w->spd.x += pl_w->acc.x ;
			pl_w->pos.y += pl_w->spd.y ;
			pl_w->spd.y += pl_w->acc.y ;
		}
		if( !pl_w->timer0 -- ){
			pl_w->routine_2 = 7 ;
			pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
			pl_char_set( pl_w, Char_Super, 2 ) ;
		}
	} else if( ( pl_w->hit_flag & HR_GUARD ) || pl_w->em_w->work_kind ){
		pl_w->routine_2 = 8 ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->spd.x = DOT_X( 2.0f ) ;
		if( pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_X( 4.0f ) ;
		if( pl_w->atck_level ) pl_w->spd.y = DOT_X( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
	} else {
		pl_w->routine_2 = 10 ;
		pl_w->spd.x = -DOT_X( 8.0f ) ;
		pl_w->acc.x = 0.0f ;
		if( !ABYSS_CK( pl_w ) ){
			pl_w->em_w->pl_stance = STANCE_STAND ;
			pl_w->em_w->pos.y = pl_w->ground_y ;
			pl_w->em_w->pos.x = pl_w->pos.x ;
		}
		PL22_LW( pl_w )->save_x = DOT_X( 80.0f ) ;
		if( pl_w->pl_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			PL22_LW( pl_w )->save_x = -PL22_LW( pl_w )->save_x ;
		}
		if( !ABYSS_CK( pl_w ) && pl_w->em_w->vital_new )
			pl_w->em_w->pos.x = pl_w->pos.x - PL22_LW( pl_w )->save_x ;
		pl_char_set( pl_w, Char_Super, 17 ) ;
		pl_w->em_w->hit_stop = 20 ;
		PL22_LW( pl_w )->carry_w = pl_w->em_w ;
	}
}

/*--------------------------------------------------------------*/
static void midare_hit( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
	} else if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		if( !ABYSS_CK( pl_w ) ){
			PLAYER_WORK *em = PL22_LW( pl_w )->carry_w ;

			if( em->vital_new ) em->pos.x = pl_w->pos.x - PL22_LW( pl_w )->save_x ;
		}
	}
}

/*--------------------------------------------------------------*/
static void midare_upper( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->acc.x = DOT_X( 0.25f ) ;
		if( pl_w->disp_dir ) pl_w->acc.x = -pl_w->acc.x ;
		pl_w->spd.y = DOT_Y( 12.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
}

/*--------------------------------------------------------------*/
static void midare_up( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	if( move_ck( pl_w ) < 0.0f ) pl_w->acc.x = pl_w->spd.x = 0.0f ;
	if( pl_w->spd.y < 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->acc.x = pl_w->spd.x = 0.0f ;
		pl_char_set( pl_w, Char_Super, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void midare_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->routine_2 = 9 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void midare_stop( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( move_ck( pl_w ) < 0.0f ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 = 9 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	} else pl_smoke_set2( pl_w, 3 ) ;
}

/*--------------------------------------------------------------*/
static void midare_guard( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y < pl_w->ground_y ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Super, 5 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void midare_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void midare_catch( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 = 3 ;
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_char_set( pl_w, Char_Super, 0 ) ;
		if( !ABYSS_CK( pl_w ) ){
			PLAYER_WORK *em = PL22_LW( pl_w )->carry_w ;

			if( em->vital_new ) em->pos.x = pl_w->pos.x - PL22_LW( pl_w )->save_x ;
		}
	}
}

static void shinkuu_init( PLAYER_WORK *pl_w );
static void shinkuu_wait( PLAYER_WORK *pl_w );
static void shinkuu_keep( PLAYER_WORK *pl_w );
static void shinkuu_end_init( PLAYER_WORK *pl_w );
static void shinkuu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_shinkuu( PLAYER_WORK *pl_w )
{
	static void ( *shinkuu_jmp[])( PLAYER_WORK *pl_w ) = {
		shinkuu_init, shinkuu_wait, shinkuu_keep, shinkuu_end_init, shinkuu_end,
	} ;

	shinkuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void shinkuu_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 0 ;
	pl_w->atck_no = pl22_at_shinkuu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 9 ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 2 ){
		pl_w->cg_type &= 0xfd ;
		shl22_00_set2( pl_w, 4 ) ;
	}
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 57.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shinkuu_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->timer0 ++ ;
	if( !( pl_w->timer0 &= 15 ) ) pl_sp_ef_set( pl_w ) ;
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_end_init( PLAYER_WORK *pl_w )
{
	pl_w->delay_flag = 0 ;
	pl_w->delay_timer = 0 ;
	pl_w->hyper_bg = 0 ;
	pl_w->hyper_bg_tm = 0 ;
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Super, 10 ) ;
}

/*--------------------------------------------------------------*/
static void shinkuu_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void haru_init( PLAYER_WORK *pl_w );
static void haru_wait( PLAYER_WORK *pl_w );
static void haru_up( PLAYER_WORK *pl_w );
static void haru_hit( PLAYER_WORK *pl_w );
static void haru_fall( PLAYER_WORK *pl_w );
static void haru_land( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_haru( PLAYER_WORK *pl_w )
{
	static void ( *haru_jmp[])( PLAYER_WORK *pl_w ) = {
		haru_init, haru_wait, haru_up, haru_hit, haru_fall, haru_land,
	} ;

	haru_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void haru_init( PLAYER_WORK *pl_w )
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
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl22_at_haru ;
	HIT_RESET( pl_w ) ;
	pl_w->spd.y = DOT_Y( 6.0f ) ;
	pl_char_set( pl_w, Char_Super, 15 ) ;
}

/*--------------------------------------------------------------*/
static void haru_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 60 ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = -DOT_X( 28.0f ) ;
		ofs.y = DOT_Y( 65.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void haru_up( PLAYER_WORK *pl_w )
{
	Float ofs_x ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) return ;
	pl_w->pl_stance = STANCE_AIR ;
	if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( -- pl_w->timer0 ) return ;
	if( pl_w->hit_flag ){
		pl_w->routine_2 ++ ;
		pl_w->em_w->pos.y = pl_w->pos.y ;
		ofs_x = 80.0f ;
		if( !pl_w->disp_dir ) ofs_x = -80.0f ;
		pl_w->em_w->pos.x = pl_w->pos.x + ofs_x ;
		pl_char_set( pl_w, Char_Super, 16 ) ;
	} else {
		pl_w->routine_2 += 2 ;
		pl_w->spd.y = -DOT_Y( 1.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Jump, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void haru_hit( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_sjump_ret( pl_w ) ;
	} else if( pl_w->cg_flag1 ){
		pl_w->atck_no = pl_w->cg_flag1 ;
		HIT_RESET( pl_w ) ;
		pl_w->cg_flag1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void haru_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
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
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Special, 10 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void haru_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dshinkuu_init( PLAYER_WORK *pl_w );
static void dshinkuu_wait( PLAYER_WORK *pl_w );
static void dshinkuu_shot( PLAYER_WORK *pl_w );
static void dshinkuu_keep( PLAYER_WORK *pl_w );
static void dshinkuu_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_dshinkuu( PLAYER_WORK *pl_w )
{
	static void ( *dshinkuu_jmp[])( PLAYER_WORK *pl_w ) = {
		dshinkuu_init, dshinkuu_wait, dshinkuu_shot, dshinkuu_keep, dshinkuu_end,
	} ;

	dshinkuu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dshinkuu_init( PLAYER_WORK *pl_w )
{
	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->timer0 = 48 ;
	if( pl_w->cpu_flag ) PL22_LW( pl_w )->renda_cnt = 24 ;
	else PL22_LW( pl_w )->renda_cnt = 16 ;
	pl_w->atck_no = pl22_at_dshinkuu ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Super, 12 ) ;
	pls22_00_set( pl_w, 10 ) ;
}

/*--------------------------------------------------------------*/
static void dshinkuu_wait( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = ( pl_w->team_type == 6 ) ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type &= 0xfe ;
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		ofs.x = DOT_X( 8.0f ) ;
		ofs.y = DOT_Y( 57.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dshinkuu_shot( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type & 0x80 ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		pls22_00_set3( pl_w, 9 ) ;
		shl22_01_set( pl_w, -195.0f, DOT_Y( 53.0f ) ) ;
	}
}

/*--------------------------------------------------------------*/
static void dshinkuu_keep( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( PL22_LW( pl_w )->renda_cnt && sw_renda_ck( pl_w ) ) PL22_LW( pl_w )->renda_cnt -- ;
	else if( -- pl_w->timer0 <= 0 ){
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Super, 14 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void dshinkuu_end( PLAYER_WORK *pl_w )
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
	pl_w->atck_no = pl22_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 42 ) ;
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
		ofs.x = -45.0f ;
		ofs.y = DOT_Y( 64.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl22_t_kick( PLAYER_WORK *pl_w )
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
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = DOT_X( 16.0f ) ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -0.4017857f ;
	pl_w->atck_no = pl22_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 1 ) ;
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
		pl_land_ef_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_stand_ret( pl_w ) ;
	}
}

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl22_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = 0 ;

	if( ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) && pl_w->atck_level && pl_w->pl_stance != STANCE_CROUCH ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			if( !pl_w->atck_pk ){
				if( pl_w->pl_stance == STANCE_AIR ) pl_w->catch_no = 2 ;
				else pl_w->catch_no = 0 ;
			} else if( pl_w->pl_stance == STANCE_AIR ) em = 0 ;
			else pl_w->catch_no = 1 ;
		}
	}
	return em ;
}

/****************************************************************/
/*			Start the throw that pl22_catch_ck chose			*/
/****************************************************************/
void	pl22_catch_set( PLAYER_WORK *pl_w )
{
	static Uint8 catch_char_tbl[] = { 0, 3, 5 } ;
	VEC3 ofs ;

	if( pl_w->atck_lever & KEY_BACK ) pl_w->pl_dir = pl_w->disp_dir = pl_w->disp_dir ^ 1 ;
	pl_w->catch_w->pl_dir = pl_w->catch_w->disp_dir = pl_w->disp_dir ^ 1 ;
	catch_gauge_add( pl_w ) ;
	pl_w->hit_stop = 10 ;
	if( !pl_w->catch_no ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->catch_w->pos.y = pl_w->catch_w->ground_y ;
		if( PL_CK( pl_w->catch_w, PL_SERVBOT ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 60.0f ) ;
		if( PL_CK( pl_w->catch_w, PL_ROLL ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 27.0f ) ;
		if( PL_CK( pl_w->catch_w, PL_MEGAMAN ) ) pl_w->catch_w->pos.y = pl_w->ground_y + DOT_Y( 23.0f ) ;
		pl_w->timer0 = 96 ;
		hold_renda_set( pl_w->catch_w, 40 ) ;
		catch_renda_set( pl_w, 1 ) ;
		ofs.x = -DOT_X( 16.0f ) ;
		ofs.y = DOT_Y( 82.0f ) ;
	} else {
		scr_excep_set( pl_w, 5, 5 ) ;
		ofs.x = -DOT_X( 37.0f ) ;
		ofs.y = DOT_Y( 51.0f ) ;
	}
	nage_hit_set( pl_w, &ofs ) ;
	pl_char_set( pl_w, Char_Catch, catch_char_tbl[pl_w->catch_no] ) ;
}

static void nml_p_catch( PLAYER_WORK *pl_w );
static void nml_p_throw( PLAYER_WORK *pl_w );
static void catch_fall( PLAYER_WORK *pl_w );
static void catch_land( PLAYER_WORK *pl_w );
static void kick_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl22_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_p_catch, nml_p_throw, catch_fall, catch_land,
		kick_catch, catch_fall, catch_land, pl_stand_ret,
		kick_catch, catch_land, pl_stand_ret, pl_stand_ret,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no * 4 + pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_p_catch( PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Uint8 throw_ok ;

	pl_char_move( pl_w ) ;
	if( catch_renda_ck( pl_w ) ) pl_w->cg_ctr = 1 ;
	if( -- pl_w->timer0 < 0 || ( pl_w->catch_w->renda_timer && hold_renda_ck( pl_w->catch_w ) ) )
		pl_w->catch_w->renda_timer = 0 ;
	throw_ok = 0 ;
	if( pl_w->cg_flag1 ){
		if( pl_w->cg_flag1 == 1 ) throw_ok = 1 ;
		else if( pl_w->cg_flag1 & 0x80 ){
			if( !pl_w->catch_w->renda_timer ) throw_ok = 1 ;
		} else throw_ok = 0 ;
	}
	if( throw_ok ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Catch, 1 ) ;
	} else if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl22_at_throw_p_hit ;
		catch_damage( pl_w ) ;
		if( pl_w->disp_dir ) pos.x = 10.0f ;
		else pos.x = -10.0f ;
		pos.x += pl_w->pos.x ;
		pos.y = pl_w->pos.y + DOT_Y( 78.0f ) ;
		hitmark_set( &pos, pl_w->disp_dir, 1, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_p_throw( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		SCR_EXCEP_SET ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->catch_type = 3 ;
		pl_w->catch_w->atck_no = pl22_at_throw_p ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = -DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		ofs.x = 45.0f ;
		ofs.y = DOT_Y( 68.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_fall( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y < 0.0f && pl_w->ground_y > pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		if( !pl_w->catch_no ){
			pl_dir_set( pl_w ) ;
			pl_char_set( pl_w, Char_Catch, 2 ) ;
		} else pl_char_set( pl_w, Char_Catch, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void catch_land( PLAYER_WORK *pl_w )
{
	if( pl_w->catch_no == 2 ){
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		if( pl_w->catch_no ){ }
		if( pl_w->catch_no == 2 || pl_w->pl_stance == STANCE_AIR ) pl_jump_ret( pl_w ) ;
		else pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void kick_catch( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		SCR_EXCEP_SET ;
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl22_at_throw_k_1 ;
		catch_damage( pl_w ) ;
		pl_w->catch_w->catch_type = 1 ;
		pl_w->catch_w->atck_no = pl22_at_throw_k ;
		ofs.x = -DOT_X( 64.0f ) ;
		ofs.y = DOT_Y( 64.0f ) ;
		catch_hit_mark_set( pl_w, &ofs ) ;
		hit_se_req( pl_w->catch_w, 1 ) ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->spd.x = DOT_X( 4.0f ) ;
		pl_w->acc.x = 0.0f ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	}
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl22_hold( PLAYER_WORK *pl_w )
{
	Float x ;

	if( pl_w->catch_w->cg_flag1 > 0 ) catch_pos_set( pl_w->catch_w, pl_w ) ;
	else catch_em_pos_set1( pl_w->catch_w, pl_w ) ;
	if( !pl_w->catch_type && pl_w->catch_w->cg_prm ){
		pl_w->catch_w->cg_prm = 0 ;
		x = 0.0f ;
		if( HI_BYTE( pl_w->catch_w->cg_rival ) == 1 ) x = 1.0f ;
		if( HI_BYTE( pl_w->catch_w->cg_rival ) == 2 ) x = -1.0f ;
		if( !pl_w->catch_w->disp_dir ) x = -x ;
		pl_w->pos.x += x ;
	}
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl22_sp_unison( PLAYER_WORK *pl_w )
{
	static Uint8 unison_tbl[] = {
		pl22_sp_haru, pl22_sp_shinkuu, pl22_sp_midare,
		pl22_sp_haru, pl22_sp_dshinkuu, pl22_sp_midare,
	} ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = unison_tbl[pl_w->pl_personal + pl_w->obj_flag * 3] ;
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl22_sp_gattai( PLAYER_WORK *pl_w )
{
	static Uint8 gattai_tbl[] = {
		pl22_sp_haru, pl22_sp_shinkuu, pl22_sp_midare,
		pl22_sp_haru, pl22_sp_dshinkuu, pl22_sp_midare,
	} ;

	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	pl_w->pl_sp_type = gattai_tbl[pl_w->pl_personal + pl_w->obj_flag * 3] ;
#if BUILD_DATE >= 20000412	/* the level, as in pl22_delay_ck */
	pl_w->atck_level = 1 ;
#endif
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl22_sp_assist( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl22_sp_shouou ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl22_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl22_sp_shunpu ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl22_sp_zero( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl22_sp_shouou ;
		pl_w->atck_level = 1 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl22_sp_hadou ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl22_sp_shunpu ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
/*			The dash, faster when obj_flag is set				*/
/****************************************************************/
void	pl22_dash_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( !pl_w->obj_flag ){
		pl_char_set( pl_w, Char_Dash, 0 ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	} else {
		pl_char_set( pl_w, Char_Dash, 4 ) ;
		pl_w->spd.x = pl_w->pl_dir ? DOT_X( 10.0f ) : -DOT_X( 10.0f ) ;
		pl_w->acc.x = pl_w->pl_dir ? -DOT_X( 0.25f ) : DOT_X( 0.25f ) ;
	}
}

/****************************************************************/
void	pl22_noki_set( PLAYER_WORK *pl_w )
{
	if( !pl_w->obj_flag ) pl_char_set( pl_w, Char_Dash, 1 ) ;
	else pl_char_set( pl_w, Char_Dash, 5 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl22.c" End of File	*/
