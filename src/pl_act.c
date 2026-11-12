/* The player's shared acts: the returns to standing and jumping that every character's moves end
   in, the checks that start a walk, a jump or an attack from them, throws, landing and act_st_set. */
#include "player.h"
#include "pl_char.h"
#include "pl_color_old.h"
#include "pl_act.h"

static void jump_ret_set( PLAYER_WORK *pl_w );
static void atck_type_set( PLAYER_WORK *pl_w );
static Uint8 jump_atck_ck( PLAYER_WORK *pl_w );
static void nml_atck_set( PLAYER_WORK *pl_w );
static Uint8 pl_air_guard_req_ck( PLAYER_WORK *pl_w );
static Uint8 em_atck_out_ck( Uint8 pl_no );
Uint8 pl_guard_req_ck( PLAYER_WORK *pl_w );
Uint8 pl_guard_req_now_ck( PLAYER_WORK *pl_w );
Uint8 pl_guard_ok( PLAYER_WORK *pl_w );
Uint8 pl_nml_atck_ck( PLAYER_WORK *pl_w );
void pl_sp_catch_no_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w, Uint8 no );
Uint8 pl_guard_dir_ck( PLAYER_WORK *pl_w );
Uint8 pl_crouch_now_ck( PLAYER_WORK *pl_w );
int pl_crouch_end_ck( PLAYER_WORK *pl_w );
Uint8 pl_fall_ck( PLAYER_WORK *pl_w );
int pl_stand_ck( PLAYER_WORK *pl_w );
void pl_change_in_set( PLAYER_WORK *pl_w );
void pl_change_in_set2( PLAYER_WORK *pl_w );
void pl_assist_in_set( PLAYER_WORK *pl_w );

/* tables by character, kept with the character data */
extern Uint16 cmd_atck_sw_tbl[];		/* the buttons of its down-forward attack */
extern Sint16 change_in_ofs_tbl[][3];	/* how far in a partner lands, by pl_personal */
extern Sint16 brake_acc_tbl[];		/* the ground brake on acc.x */

extern void pl_tlocal_init( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Back to standing, or on into whatever the stick		*/
/*			and buttons start									*/
/****************************************************************/
void pl_stand_ret( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 1 ;
	if( pl_w->pl_stance != STANCE_AIR && pl_w->pos.y > pl_w->ground_y ){
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	}
	if( pl_w->fly_flag ){
		pl_act_set( pl_w, N_FLY ) ;
		return ;
	}
	pl_w->disp_dir = pl_w->pl_dir ;
	if( pl_turn_ck( pl_w ) ) return ;
	if( pl_guard_req_ck( pl_w ) ) return ;
	if( pl_w->pl_stance != STANCE_AIR && pl_w->pl_sw_new & KEY_UP ){
		pl_act_set( pl_w, N_JUMP ) ;
		return ;
	}
	if( pl_w->pl_stance == STANCE_CROUCH ){
		if( pl_w->pl_sw_new & KEY_DOWN ) pl_act_set( pl_w, N_CROUCHING ) ;
		else pl_act_set( pl_w, N_STAND_UP ) ;
		return ;
	}
	if( pl_w->pl_stance == STANCE_STAND && pl_w->pl_sw_new & KEY_DOWN ){
		pl_act_set( pl_w, N_CROUCH ) ;
		return ;
	}
	if( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ){
		pl_act_set( pl_w, N_WALK ) ;
		return ;
	}
	if( pl_w->pl_sw_new & ( KEY_HP | KEY_HK ) ){
		if( pl_cmd_atck_ck( pl_w ) ) return ;
	}
	if( pl_w->pl_sw_new & KEY_ATCK ){
		if( pl_nml_atck_ck( pl_w ) ) return ;
	}
	pl_act_set( pl_w, N_STAND ) ;
}

/****************************************************************/
/*			Back into a jump's fall								*/
/****************************************************************/
void pl_jump_ret( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	pl_jump_ret2( pl_w ) ;
}

/****************************************************************/
/*			The same, keeping the way the player faces			*/
/****************************************************************/
void pl_jump_ret2( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 1 ;
	if( pl_w->fly_flag ) pl_act_set( pl_w, N_FLY ) ;
	else if( pl_w->sjump_flag ) pl_sjump_ret( pl_w ) ;
	else {
		act_st_set( pl_w, N_JUMP_AIR ) ;
		pl_w->routine_2 = 1 ;
		jump_ret_set( pl_w ) ;
	}
}

/****************************************************************/
/*			Back into a super jump's fall						*/
/****************************************************************/
void pl_sjump_ret( PLAYER_WORK *pl_w )
{
	act_st_set( pl_w, N_SJUMP_AIR ) ;
	pl_w->routine_2 = 1 ;
	pl_w->sjump_flag = pl_w->spd.y > 0.0f ? 1 : 2 ;
	jump_ret_set( pl_w ) ;
}

/*==============================================================*/
/*			In the air, falling, in the jump pose for			*/
/*			jump_dir											*/
/*==============================================================*/
static void jump_ret_set( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->routine_1 = PL_NORM ;
	if( pl_w->jump_dir < 0 ) pl_w->char_no.b.no = 9 ;
	else if( pl_w->jump_dir == 1 ) pl_w->char_no.b.no = 11 ;
	else pl_w->char_no.b.no = 10 ;
	pl_w->jump_dir = -1 ;
	pl_char_set( pl_w, Char_Jump, pl_w->char_no.b.no ) ;
}

/****************************************************************/
/*			Stop dead, and back to standing						*/
/****************************************************************/
void pl_stop_stand_ret( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			A walk, if the stick is held back or forward		*/
/****************************************************************/
int pl_walk_ck( PLAYER_WORK *pl_w, int now )
{
	Uint16 sw ;

	sw = now ? pl_w->pl_now_lvbt : pl_w->pl_new_lvbt ;
	if( !( sw & ( KEY_FORWARD | KEY_BACK ) ) ) return 0 ;
	pl_act_set( pl_w, N_WALK ) ;
	return 1 ;
}

/****************************************************************/
/*			A normal attack, if a button was pressed			*/
/****************************************************************/
Uint8 pl_nml_atck_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw, bit ;
	Sint8 no ;
	Sint8 *tbl ;
	PLAYER_WORK *em ;

	if( pl_w->auto_guard_timer || pl_w->atck_ng_timer ) return 0 ;
	combo_reset( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR && pl_w->pos.y < pl_w->ground_y ) return 0 ;
	sw = pl_w->pl_now_lvbt ;
	if( pl_w->act_st == N_NML_ATCK ) sw |= pl_w->pl_stop_now ;
	if( !( sw &= KEY_ATCK ) ) return 0 ;
	if( pl_w->act_st == N_NML_ATCK ){
		if( !pl_w->chain_tbl ) return 0 ;
		if( pl_w->cg_atck_flag & CG_ATCK_NG || !pl_w->hit_flag ) return 0 ;
		tbl = pl_w->chain_tbl ;
		if( sw & KEY_LP ){ no = tbl[0] ; bit = KEY_LP ; }
		if( sw & KEY_LK ){ no = tbl[2] ; bit = KEY_LK ; }
		if( sw & KEY_HP ){ no = tbl[1] ; bit = KEY_HP ; }
		if( sw & KEY_HK ){ no = tbl[3] ; bit = KEY_HK ; }
		if( no == -1 ) return 0 ;
		if( pl_w->hit_flag ){ }
		pl_w->jump_atk_cnt |= JATK_ONE ;
	} else {
		if( sw & KEY_LP ){ no = 0 ; bit = KEY_LP ; }
		if( sw & KEY_LK ){ no = 0 ; bit = KEY_LK ; }
		if( sw & KEY_HP ){ no = 2 ; bit = KEY_HP ; }
		if( sw & KEY_HK ){ no = 2 ; bit = KEY_HK ; }
	}
	if( !jump_atck_ck( pl_w ) ) return 0 ;
	pl_w->nml_no = no ;
	pl_w->atck_lever = pl_w->pl_sw_new & KEY_LEVER ;
	if( bit & KEY_PUNCH ) pl_w->atck_pk = 0 ;
	else pl_w->atck_pk = 1 ;
	if( bit & KEY_LIGHT ) pl_w->atck_level = 0 ;
	else pl_w->atck_level = 1 ;
	if( pl_w->act_st != N_DASH && pl_w->act_st != N_BACK_DASH && !pl_w->speed_up_flag ){
		if( em = pl_tbl_catch_ck( pl_w ) ){
			pl_catch_set( pl_w, em ) ;
			return 1 ;
		}
	}
	atck_type_set( pl_w ) ;
	if( pl_w->pl_stance != STANCE_AIR || pl_w->pl_stance == STANCE_AIR && pl_w->sjump_flag ) pl_dir_set( pl_w ) ;
	nml_atck_set( pl_w ) ;
	return 1 ;
}

/*==============================================================*/
/*			The attack's kind, from where it starts				*/
/*==============================================================*/
static void atck_type_set( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		if( !pl_w->sjump_flag ) pl_w->atck_type = 1 ;
		else pl_w->atck_type = 2 ;
	} else {
		pl_w->pos.y = pl_w->ground_y ;
		if( pl_w->pl_new_lvbt & KEY_DOWN ) pl_w->pl_stance = STANCE_CROUCH ;
		else pl_w->pl_stance = STANCE_STAND ;
		if( pl_w->act_st == N_DASH || pl_w->act_st == N_BACK_DASH ) pl_w->atck_type = 3 ;
		else pl_w->atck_type = 0 ;
	}
}

/****************************************************************/
/*			The attack done with down-forward and the			*/
/*			character's buttons									*/
/****************************************************************/
int pl_cmd_atck_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;

	if( pl_w->auto_guard_timer ) return 0 ;
	if( pl_w->atck_ng_timer ) return 0 ;
	combo_reset( pl_w ) ;
	if( pl_w->pl_stance == STANCE_AIR || pl_w->pl_stance == STANCE_DOWN ) return 0 ;
	if( pl_w->cg_atck_flag & CG_ATCK_ALL ) return 0 ;
	if( !( sw = pl_w->pl_now_lvbt & cmd_atck_sw_tbl[pl_w->pl_type] ) ) return 0 ;
	if( ( pl_w->pl_sw_new & KEY_LEVER ) != ( KEY_DOWN | KEY_FORWARD ) ) return 0 ;
	if( !jump_atck_ck( pl_w ) ) return 0 ;
	if( sw & KEY_HP ) pl_w->atck_pk = 0 ;
	else pl_w->atck_pk = 1 ;
	atck_type_set( pl_w ) ;
	pl_dir_set( pl_w ) ;
	pl_act_set( pl_w, N_CMD_ATCK ) ;
	return 1 ;
}

/****************************************************************/
/*			A jump, if the stick is held up						*/
/****************************************************************/
Uint8 pl_jump_ck( PLAYER_WORK *pl_w )
{
	if( !( pl_w->pl_new_lvbt & KEY_UP ) ) return 0 ;
	pl_act_set( pl_w, N_JUMP ) ;
	return 1 ;
}

/****************************************************************/
/*			A jump in the air, for the characters that have		*/
/*			one													*/
/****************************************************************/
int pl_air_jump_ck( PLAYER_WORK *pl_w )
{
	Uint8 max, i ;

	if( AIR_JUMP2_CK( pl_w ) ) max = 2 ;
	else max = 1 ;
	if( ( pl_w->air_jump_cnt & 0x7f ) >= max ) return 0 ;
	if( pl_w->cpu_flag ){
		if( !( pl_w->cpu_air_jump_flag & 2 ) ) return 0 ;
		pl_w->cpu_air_jump_flag &= ~2 ;
	} else {
		if( !( pl_w->air_jump_cnt & ~0x7f ) ){
			if( !( pl_w->pl_new_lvbt & KEY_UP ) ) pl_w->air_jump_cnt |= 0x80 ;
			return 0 ;
		}
		if( !( pl_w->pl_sw_new & KEY_UP ) ) return 0 ;
		pl_w->jump_dir = pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ? ( pl_w->pl_sw_new & KEY_BACK ) >> 10 : -1 ;
	}
	if( !pl_w->jump_dir ) pl_w->char_no.b.no = 1 ;
	else if( pl_w->jump_dir == 1 ) pl_w->char_no.b.no = 2 ;
	else pl_w->char_no.b.no = 0 ;
	if( pl_w->pl_type == PL_STRIDER ) pl_w->char_no.b.no += 6 ;
	i = 1 ;
	do {
		pl_char_set2( pl_w, Char_Jump, pl_w->char_no.b.no, i ) ;
		i ++ ;
	} while( pl_w->cg_type & 15 ) ;
	pl_w->air_jump_cnt = ( pl_w->air_jump_cnt + 1 ) & 0x7f ;
	pl_w->jump_atk_cnt = JATK_ONE ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->sjump_flag ){
		pl_w->sjump_flag = 0 ;
		pl_jump_spd_set( pl_w ) ;
		pl_w->sjump_flag = 1 ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
	} else pl_jump_spd_set( pl_w ) ;
	return 1 ;
}

/****************************************************************/
/*			A jump off the screen edge the player is against	*/
/****************************************************************/
Uint8 pl_wall_jump_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->wall_flag ) return 0 ;
	if( pl_w->pl_type == PL_FELICIA ){
		if( pl_w->wall_flag ^ ( pl_w->pl_now_lvbt & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) return 0 ;
	} else {
		if( ( pl_w->wall_flag | ( pl_w->pl_new_lvbt & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) != 3 ) return 0 ;
	}
	if( pl_w->jump_dash_cnt ) return 0 ;
	if( pl_w->pos.y < pl_w->ground_y + 240.0f ) return 0 ;
	if( pl_w->pos.y >= pl_w->ground_y + DOT_Y( 480.0f ) ) return 0 ;
	if( pl_w->pl_new_lvbt & KEY_UP ){
		pl_w->spd.x = 5.0f ;
		pl_w->spd.y = DOT_Y( 10.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.5f ) ;
	} else {
		pl_w->spd.x = DOT_X( 5.0f ) ;
		pl_w->spd.y = DOT_Y( 6.0f ) ;
		pl_w->acc.y = -0.9375f ;
	}
	pl_w->pl_dir = ( pl_w->wall_flag >> 1 ) & 1 ;
	pl_w->disp_dir = pl_w->pl_dir ^ 1 ;
	if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
	pl_w->acc.x = 0.0f ;
	act_st_set( pl_w, N_WALL_JUMP ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	return 1 ;
}

static Uint8 escape_init( PLAYER_WORK *pl_w ) ;
static Uint8 escape_first_ck( PLAYER_WORK *pl_w ) ;
static Uint8 escape_wait( PLAYER_WORK *pl_w ) ;
static Uint8 escape_second_ck( PLAYER_WORK *pl_w ) ;
static Uint8 escape_delay( PLAYER_WORK *pl_w ) ;
static Uint8 escape_first( PLAYER_WORK *pl_w ) ;
static Uint8 escape_second( PLAYER_WORK *pl_w ) ;

/****************************************************************/
/*			Breaking free of a throw							*/
/****************************************************************/
Uint8 pl_catch_escape( PLAYER_WORK *pl_w )
{
	static Uint8 ( *escape_jmp[] )( PLAYER_WORK *pl_w ) = {
		escape_init, escape_first_ck, escape_wait, escape_second_ck, escape_delay, escape_first, escape_second,
	} ;

	if( pl_w->catch_w->hit_stop ) return 0 ;
	return escape_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static Uint8 escape_init( PLAYER_WORK *pl_w )
{
	pl_w->escape_timer = 4 ;
	pl_w->routine_3 ++ ;
	return escape_first_ck( pl_w ) ;
}

/*--------------------------------------------------------------*/
static Uint8 escape_first_ck( PLAYER_WORK *pl_w )
{
	if( -- pl_w->escape_timer <= 0 ){
		pl_w->escape_timer = 0 ;
		pl_w->routine_3 = 2 ;
	}
	if( !( pl_w->catch_no & CATCH_ESC_NG ) ){
		if( pl_w->cpu_flag ){
			if( em_rate_ck( pl_w, 30, 2 ) ){
				pl_w->routine_3 = 4 ;
				pl_w->escape_timer = 10 ;
				return 0 ;
			} else {
				pl_w->escape_timer = 0 ;
				pl_w->routine_3 = 2 ;
			}
		}
		if( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) && pl_w->pl_sw_now & ( KEY_HP | KEY_HK ) ){
			pl_w->routine_3 = 4 ;
			pl_w->escape_timer = 10 ;
		}
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 escape_wait( PLAYER_WORK *pl_w )
{
	if( pl_w->escape_timer ) pl_w->routine_3 ++ ;
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 escape_second_ck( PLAYER_WORK *pl_w )
{
	if( -- pl_w->escape_timer > 0 ){
		if( pl_w->catch_no & CATCH_ESC2_NG ) return 0 ;
		if( pl_w->cpu_flag ){
			if( em_rate_ck( pl_w, 31, 2 ) ){
				pl_w->routine_3 = 6 ;
				return escape_second( pl_w ) ;
			}
			else pl_w->escape_timer = 0 ;
		}
		if( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) && pl_w->pl_sw_now & ( KEY_HP | KEY_HK ) ){
			pl_w->routine_3 = 6 ;
			return escape_second( pl_w ) ;
		}
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 escape_delay( PLAYER_WORK *pl_w )
{
	if( -- pl_w->escape_timer < 0 ) pl_w->routine_3 ++ ;
	return 0 ;
}

/*==============================================================*/
/*			Both players thrown clear							*/
/*==============================================================*/
static Uint8 escape_first( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	int ret ;

	nage_nuke( &pl_w->pos ) ;
	score_add( pl_w, 0, 0x700000 ) ;
	em = pl_w->catch_w ;
	pl_w->pl_dir = 0 ;
	if( em->pos.x > pl_w->pos.x ) pl_w->pl_dir = 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
	em->pl_dir = em->disp_dir = pl_w->pl_dir ^ 1 ;
	em->routine_1 = PL_NORM ;
	em->old_act_st = em->act_st ;
	em->routine_2 = 0 ;
	em->spd.x = 0.0f ;
	em->spd.y = 0.0f ;
	em->acc.x = 0.0f ;
	em->acc.y = 0.0f ;
	em->acc.y = -DOT_Y( 0.375f ) ;
	em->jump_atk_cnt = 0 ;
	pl_color_set( em, 1 ) ;
	pl_jump_ret( em ) ;
	scr_excep_clr() ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->catch_ng_timer = 4 ;
	pl_w->type = 0 ;
	ret = pl_act_set( pl_w, N_ESCAPE ) ;
	return ret ;
}

/*--------------------------------------------------------------*/
static Uint8 escape_second( PLAYER_WORK *pl_w )
{
	int ret ;

	score_add( pl_w, 0, 0x700000 ) ;
	scr_excep_clr() ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->catch_ng_timer = 4 ;
	pl_w->type = 1 ;
	ret = pl_act_set( pl_w, N_ESCAPE ) ;
	return ret ;
}

/****************************************************************/
/*			Turn to face the opponent, if it has gone past		*/
/****************************************************************/
Uint8 pl_turn_ck( PLAYER_WORK *pl_w )
{
	Float ofs ;

	ofs = -DOT_X( 14.0f ) ;
	pl_w->disp_dir = pl_w->pl_dir ;
	if( pl_w->pl_dir ) ofs = DOT_X( 14.0f ) ;
	if( pl_w->pos.x <= pl_w->target_w->pos.x + ofs && pl_w->pl_dir ) return 0 ;
	if( pl_w->pos.x > pl_w->target_w->pos.x + ofs && !pl_w->pl_dir ) return 0 ;
	if( !pl_w->pl_stance ) pl_act_set( pl_w, N_TURN ) ;
	else pl_act_set( pl_w, N_CROUCH_TURN ) ;
	return 1 ;
}

/****************************************************************/
/*			Face the opponent									*/
/****************************************************************/
void pl_dir_set( PLAYER_WORK *pl_w )
{
	Float ofs ;

	ofs = -DOT_X( 14.0f ) ;
	if( pl_w->pl_dir ) ofs = DOT_X( 14.0f ) ;
	if( pl_w->target_w->pos.x + ofs < pl_w->pos.x && pl_w->pl_dir
	 || pl_w->target_w->pos.x + ofs >= pl_w->pos.x && !pl_w->pl_dir ) pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
}

/****************************************************************/
/*			Face the player being thrown						*/
/****************************************************************/
void pl_catch_dir_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( pl_w->pos.x > em->pos.x ) pl_w->pl_dir = pl_w->disp_dir = 0 ;
	else pl_w->pl_dir = pl_w->disp_dir = 1 ;
}

/****************************************************************/
/*			Face dm_at_w										*/
/****************************************************************/
void pl_at_dir_set( PLAYER_WORK *pl_w )
{
	Float ofs ;

	ofs = -DOT_X( 14.0f ) ;
	if( pl_w->pl_dir ) ofs = DOT_X( 14.0f ) ;
	if( pl_w->dm_at_w->pos.x + ofs < pl_w->pos.x && pl_w->pl_dir
	 || pl_w->dm_at_w->pos.x + ofs >= pl_w->pos.x && !pl_w->pl_dir ) pl_w->pl_dir ^= 1 ;
	pl_w->disp_dir = pl_w->pl_dir ;
}

/* the gauge a normal attack earns, by its strength */
static Sint16 atck_gauge_tbl[4] = { 2, 6, 8, 8 } ;

/****************************************************************/
void pl_atck_gauge_add( PLAYER_WORK *pl_w )
{
	Sint16 add ;

	add = atck_gauge_tbl[pl_w->nml_level] ;
	if( pl_w->speed_up_flag ){
		if( SPEED_UP_CK( pl_w ) ) add = add >> 1 ;
	}
	pl_w->gauge_no = add ;
	pl_gauge_add( pl_w, add ) ;
}

/****************************************************************/
/*			The character's own throw check						*/
/****************************************************************/
PLAYER_WORK *pl_tbl_catch_ck( PLAYER_WORK *pl_w )
{
	return ( PLAYER_WORK * )( *( int (**)( PLAYER_WORK * ) )( pl_w->pl_tbl + 9 ) )( pl_w ) ;
}

/****************************************************************/
/*			A throw starts: the thrower holds em				*/
/****************************************************************/
void pl_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_w->catch_w = em ;
	pl_w->catch_pair.w = em ;
	em->catch_w = pl_w ;
	em->catch_pair.w = pl_w ;
	( ( void (*)( PLAYER_WORK *, PLAYER_WORK * ) )pl_w->pl_tbl[TBL_CATCH_SET] )( pl_w, em ) ;
	pl_w->routine_1 = PL_CATCH ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->catch_flag = 1 ;
	em->catch_no = pl_w->catch_no ;
	em->routine_1 = PL_CAUGHT ;
	act_st_set( em, N_CAUGHT ) ;
	em->catch_type = 0 ;
	em->routine_2 = em->routine_3 = 0 ;
	em->catch_grip_old = -1 ;
	em->hit_stop = 0 ;
	em->renda_cnt = 0 ;
	em->fly_flag = 0 ;
	em->dm_elem_old = em->dm_elem ;
	em->dm_elem = 0 ;
	vs_sys_work.move_bit |= 1 << em->work_no ;
	memset( &em->sjump_w, 0, sizeof( em->sjump_w ) ) ;
	em->ukemi_flag = 120 ;
	act_st_set( pl_w, N_CATCH ) ;
	pl_color_set( em, 1 ) ;
	combo_hit( em, pl_w ) ;
	if( !( em->catch_dm_cnt += 2 ) ) em->catch_dm_cnt = 0xff ;
}

/****************************************************************/
/*			A special move's throw starts						*/
/****************************************************************/
void pl_sp_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w )
{
	pl_w->catch_w = em_w ;
	pl_w->catch_pair.w = em_w ;
	em_w->catch_w = pl_w ;
	em_w->catch_pair.w = pl_w ;
	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	pl_w->push_ng_timer = 2 ;
	em_w->catch_no = pl_w->catch_no ;
	em_w->routine_1 = PL_CAUGHT ;
	act_st_set( em_w, N_CAUGHT ) ;
	em_w->catch_type = 0 ;
	em_w->routine_2 = em_w->routine_3 = 0 ;
	em_w->catch_grip_old = -1 ;
	em_w->hit_stop = 0 ;
	em_w->renda_cnt = 0 ;
	em_w->fly_flag = 0 ;
	em_w->dm_elem_old = em_w->dm_elem ;
	em_w->dm_elem = 0 ;
	memset( &em_w->sjump_w, 0, sizeof( em_w->sjump_w ) ) ;
	em_w->ukemi_flag = 120 ;
	vs_sys_work.move_bit |= 1 << em_w->work_no ;
	pl_color_set( em_w, 1 ) ;
	combo_hit( em_w, pl_w ) ;
	if( !( em_w->catch_dm_cnt += 2 ) ) em_w->catch_dm_cnt = 0xff ;
}

/****************************************************************/
/*			A throw made by an effect: its player holds em_w	*/
/****************************************************************/
void pl_ef_catch_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w )
{
	PLAYER_WORK *ew = &pl_work[pl_w->work_no] ;

	ew->catch_pair.w = em_w ;
	ew->catch_w = em_w ;
	pl_w->catch_pair.w = em_w ;
	pl_w->catch_w = em_w ;
	em_w->catch_pair.w = pl_w ;
	em_w->catch_w = ew ;
	ew->catch_flag = 1 ;
	em_w->catch_no = ew->catch_no ;
	em_w->routine_1 = PL_CAUGHT ;
	em_w->dm_flag = 0 ;
	em_w->catch_ng_timer = 2 ;
	act_st_set( em_w, N_CAUGHT ) ;
	em_w->catch_type = 0 ;
	em_w->routine_2 = em_w->routine_3 = 0 ;
	em_w->catch_grip_old = -1 ;
	em_w->hit_stop = 0 ;
	em_w->renda_cnt = 0 ;
	em_w->fly_flag = 0 ;
	em_w->dm_elem_old = em_w->dm_elem ;
	em_w->dm_elem = 0 ;
	memset( &em_w->sjump_w, 0, sizeof( em_w->sjump_w ) ) ;
	em_w->ukemi_flag = 120 ;
	pl_color_set( em_w, 1 ) ;
	if( !( em_w->catch_dm_cnt += 2 ) ) em_w->catch_dm_cnt = 0xff ;
}

/****************************************************************/
/*			The same, with the throw given						*/
/****************************************************************/
void pl_sp_catch_no_set( PLAYER_WORK *pl_w, PLAYER_WORK *em_w, Uint8 no )
{
	PLAYER_WORK *ew = &pl_work[pl_w->work_no] ;

	ew->catch_pair.w = em_w ;
	ew->catch_w = em_w ;
	pl_w->catch_pair.w = em_w ;
	pl_w->catch_w = em_w ;
	em_w->catch_pair.w = pl_w ;
	em_w->catch_w = ew ;
	ew->catch_flag = 1 ;
	em_w->catch_no = no ;
	em_w->routine_1 = PL_CAUGHT ;
	em_w->dm_flag = 0 ;
	em_w->catch_ng_timer = 2 ;
	act_st_set( em_w, N_CAUGHT ) ;
	em_w->catch_type = 0 ;
	em_w->routine_2 = em_w->routine_3 = 0 ;
	em_w->catch_grip_old = -1 ;
	em_w->hit_stop = 0 ;
	em_w->renda_cnt = 0 ;
	em_w->fly_flag = 0 ;
	em_w->dm_elem_old = em_w->dm_elem ;
	em_w->dm_elem = 0 ;
	memset( &em_w->sjump_w, 0, sizeof( em_w->sjump_w ) ) ;
	em_w->ukemi_flag = 120 ;
	pl_color_set( em_w, 1 ) ;
	if( !( em_w->catch_dm_cnt += 2 ) ) em_w->catch_dm_cnt = 0xff ;
}

/****************************************************************/
/*			The thrown player passes to ew						*/
/****************************************************************/
void pl_catch_pass( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	em->catch_w = pl_w ;
	em->catch_pair.w = ew ;
	ew->catch_w = em ;
	ew->catch_pair.w = em ;
}

/****************************************************************/
/*			Whether a special move's hit can turn into a		*/
/*			throw												*/
/****************************************************************/
int pl_sp_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em, *ew ;

	if( !( pl_w->hit_flag & HR_HIT ) ) return 0 ;
	if( pl_w->em_w->work_kind ) return 0 ;
	if( pl_w->hit_flag & ( HR_GUARD | HR_ARMOR ) ) return 0 ;
	if( pl_w != pl_w->em_w->dm_hit_w ) return 0 ;
	em = pl_w->em_w ;
	if( em->team_no ) return 0 ;
	if( em->pl_stance == STANCE_DOWN ) return 0 ;
	if( !em->vital_new ) return 0 ;
	if( ABYSS_CK( em ) ) return 0 ;
	if( em->routine_1 != PL_DAMAGE ) return 0 ;
	ew = &pl_work[pl_w->work_no] ;
	if( ew->routine_1 == 2 || ew->routine_1 == 1 ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			A guard, if the stick is held away from the			*/
/*			opponent while he has an attack out					*/
/****************************************************************/
Uint8 pl_guard_req_ck( PLAYER_WORK *pl_w )
{
	Float ofs ;
	Uint8 dir ;

	if( pl_w->pl_stance == STANCE_AIR ) return pl_air_guard_req_ck( pl_w ) ;
	combo_reset( pl_w ) ;
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->guard_ng_timer || pl_w->team_no ) return 0 ;
	if( !pl_w->pl_dir ) ofs = -DOT_X( 14.0f ) ;
	else ofs = DOT_X( 14.0f ) ;
	dir = pl_w->pos.x > pl_w->target_w->pos.x + ofs ? 0 : 1 ;
	if( !( pl_w->pl_new_lvbt & 1 << ( dir + 10 ) ) ) return 0 ;
	if( pl_w->pl_new_lvbt & KEY_DOWN ) pl_w->pl_stance = STANCE_CROUCH ;
	if( !em_atck_out_ck( pl_w->pl_no ) ) return 0 ;
	return pl_act_set( pl_w, N_GUARD ) ;
}

/*==============================================================*/
/*			The same in the air									*/
/*==============================================================*/
static Uint8 pl_air_guard_req_ck( PLAYER_WORK *pl_w )
{
	Uint8 dir ;

	combo_reset( pl_w ) ;
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->guard_ng_timer || pl_w->fly_flag || pl_w->team_no ) return 0 ;
	if( !pl_guard_ok( pl_w ) ) return 0 ;
	dir = pl_w->pos.x > pl_w->target_w->pos.x ? 0 : 1 ;
	if( !( pl_w->pl_new_lvbt & 1 << ( dir + 10 ) ) ) return 0 ;
	if( !em_atck_out_ck( pl_w->pl_no ) ) return 0 ;
	return pl_act_set( pl_w, N_AIR_GUARD ) ;
}

/****************************************************************/
/*			Pl_guard_req_ck() read from pl_now_lvbt				*/
/****************************************************************/
Uint8 pl_guard_req_now_ck( PLAYER_WORK *pl_w )
{
	Float ofs ;
	Uint8 dir ;

	combo_reset( pl_w ) ;
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->guard_ng_timer || pl_w->team_no ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ) return pl_air_guard_req_ck( pl_w ) ;
	if( !pl_w->pl_dir ) ofs = -DOT_X( 14.0f ) ;
	else ofs = DOT_X( 14.0f ) ;
	dir = pl_w->pos.x > pl_w->target_w->pos.x + ofs ? 0 : 1 ;
	if( !( pl_w->pl_now_lvbt & 1 << ( dir + 10 ) ) ) return 0 ;
	if( pl_w->pl_new_lvbt & KEY_DOWN ) pl_w->pl_stance = STANCE_CROUCH ;
	if( !em_atck_out_ck( pl_w->pl_no ) ) return 0 ;
	return pl_act_set( pl_w, N_GUARD ) ;
}

/****************************************************************/
/*			Whether a guard can start: always in an air			*/
/*			guard, otherwise where an attack could				*/
/****************************************************************/
Uint8 pl_guard_ok( PLAYER_WORK *pl_w )
{
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->guard_ng_timer || pl_w->team_no ) return 0 ;
	if( pl_w->act_st == N_AIR_GUARD ) return 1 ;
	return jump_atck_ck( pl_w ) ;
}

/*==============================================================*/
/*			Whether an attack can start: in a jump, only		*/
/*			while it has attacks left							*/
/*==============================================================*/
static Uint8 jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance != STANCE_AIR || pl_w->sjump_flag || pl_w->fly_flag ) return 1 ;
	if( pl_w->jump_com_cnt || pl_w->air_guard_cnt ) return 0 ;
	if( !( pl_w->jump_atk_cnt & JATK_P_MASK ) || !( pl_w->jump_atk_cnt & JATK_K_MASK ) ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Whether the stick is held away from the opponent	*/
/*			while he has an attack out							*/
/****************************************************************/
Uint8 pl_guard_dir_ck( PLAYER_WORK *pl_w )
{
	Float ofs ;
	Uint8 dir ;

	if( pl_w->pl_stance == STANCE_AIR ){
		dir = pl_w->pos.x >= pl_w->target_w->pos.x ? 0 : 1 ;
	} else {
		if( !pl_w->pl_dir ) ofs = -DOT_X( 14.0f ) ;
		else ofs = DOT_X( 14.0f ) ;
		dir = pl_w->pos.x > pl_w->target_w->pos.x + ofs ? 0 : 1 ;
	}
	if( !( pl_w->pl_new_lvbt & 1 << ( dir + 10 ) ) ) return 0 ;
	if( !em_atck_out_ck( pl_w->pl_no ) ) return 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Whether the other side has an attack out			*/
/*==============================================================*/
static Uint8 em_atck_out_ck( Uint8 pl_no )
{
	Sint8 i ;
	PLAYER_WORK *ew ;

	for( i = 0 ; i < hit_work_old[pl_no ^ 1] ; i ++ ){
		ew = hit_work_w[pl_no ^ 1][i] ;
		if( ew->work_kind <= 4 ){
			if( ( ew->cg_atck_flag & CG_ATCK_ALL ) >= 0x80 ) return 1 ;
		}
	}
	return 0 ;
}

/****************************************************************/
/*			A crouch, if the stick is held down					*/
/****************************************************************/
Uint8 pl_crouch_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	if( !( pl_w->pl_new_lvbt & KEY_DOWN ) ) return 0 ;
	pl_act_set( pl_w, N_CROUCH ) ;
	return 1 ;
}

/****************************************************************/
/*			The same, on the stick just pushed down				*/
/****************************************************************/
Uint8 pl_crouch_now_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
	if( !( pl_w->pl_now_lvbt & KEY_DOWN ) ) return 0 ;
	pl_act_set( pl_w, N_CROUCH ) ;
	return 1 ;
}

/****************************************************************/
/*			Up from a crouch, once the stick leaves down		*/
/****************************************************************/
int pl_crouch_end_ck( PLAYER_WORK *pl_w )
{
	if( !( vs_sys_work.ctrl_flag & CTRL_NO_INPUT ) && pl_w->pl_new_lvbt & KEY_DOWN ) return 0 ;
	pl_act_set( pl_w, N_STAND_UP ) ;
	return 1 ;
}

/****************************************************************/
/*			A fall, if the player is off the ground				*/
/****************************************************************/
Uint8 pl_fall_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y == pl_w->ground_y ) return pl_w->air_guard_cnt = 0 ;
	pl_jump_ret( pl_w ) ;
	return 1 ;
}

/****************************************************************/
/*			Back to standing, while unk_29d is set				*/
/****************************************************************/
int pl_stand_ck( PLAYER_WORK *pl_w )
{
	if( !pl_w->walk_ng_flag ) return 0 ;
	pl_act_set( pl_w, N_STAND ) ;
	return 1 ;
}

/****************************************************************/
/*			Slow down to a stop when the player is moving		*/
/*			forward, and cancel the fall						*/
/****************************************************************/
void pl_brake_set( PLAYER_WORK *pl_w )
{
	Float spd[2] ;

	if( pl_w->pl_dir && pl_w->spd.x > 0.0f || !pl_w->pl_dir && pl_w->spd.x < 0.0f ){
		if( pl_w->speed_up_flag ) spd[0] = walk_spd_up_tbl[pl_w->pl_type][0] * 1.6666666f / 256.0f ;
		else if( pl_w->armor_flag & ARMOR_HEAVY ) spd[0] = walk_spd_armor_tbl[pl_w->pl_type][0] * 1.6666666f / 256.0f ;
		else spd[0] = walk_spd_tbl[pl_w->pl_type][0] * 1.6666666f / 256.0f ;
		if( !pl_w->pl_dir ) spd[0] = -spd[0] ;
		if( ( spd[1] = pl_w->spd.x ) < 0.0f ){
			spd[0] = -spd[0] ;
			spd[1] = -spd[1] ;
		}
		if( spd[0] >= spd[1] ) pl_w->spd.x = 0.0f ;
	}
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	spd[0] = brake_acc_tbl[pl_w->pl_type] * 1.6666666f / 256.0f ;
	spd[1] = pl_w->acc.x ;
	if( pl_w->spd.x ){
		if( pl_w->spd.x < 0.0f ){
			spd[0] = -spd[0] ;
			if( spd[0] > spd[1] ) pl_w->acc.x = spd[0] ;
		} else {
			if( spd[0] < spd[1] ) pl_w->acc.x = spd[0] ;
		}
	}
}

/****************************************************************/
/*			Stop the slide once the brake has turned it			*/
/*			round												*/
/****************************************************************/
void pl_brake_stop( PLAYER_WORK *pl_w )
{
	if( !pl_w->acc.x ) return ;
	if( pl_w->acc.x * pl_w->spd.x > 0.0f ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
}

/*==============================================================*/
/*			A normal attack starts								*/
/*==============================================================*/
static void nml_atck_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	act_st_set( pl_w, N_NML_ATCK ) ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_stop_new = 0 ;
	pl_w->pl_stop_now = 0 ;
	pl_w->pl_tbl[TBL_ATCK_SET]( pl_w ) ;
	pl_atck_gauge_add( pl_w ) ;
}

/****************************************************************/
/*			Whether the player has landed						*/
/****************************************************************/
Uint8 pl_land_ck( PLAYER_WORK *pl_w )
{
	Float ofs ;
	PLAYER_WORK *em ;

	ofs = 0.0f ;
	if( pl_w->act_st == N_NML_ATCK && !pl_w->hit_flag ){
		em = pl_w->target_w ;
		if( !em->guard_cnt ) ofs = -DOT_Y( 20.0f ) ;
	}
	ofs += pl_w->ground_y ;
	if( pl_w->pos.y > ofs ){
		pl_w->pl_stance = STANCE_AIR ;
		return 0 ;
	}
	pl_w->pos.y = pl_w->ground_y ;
	if( pl_w->fly_flag ) return 0 ;
	pl_w->sjump_flag = 0 ;
	pl_w->air_guard_cnt = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	return 1 ;
}

/****************************************************************/
/*			The same, from height y								*/
/****************************************************************/
Uint8 pl_land_y_ck( PLAYER_WORK *pl_w, Float y )
{
	if( y > pl_w->ground_y ){
		pl_w->pl_stance = STANCE_AIR ;
		return 0 ;
	}
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->air_guard_cnt = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	return 1 ;
}

/****************************************************************/
/*			The same, leaving pl_stance as it is				*/
/****************************************************************/
Uint8 pl_land_ck3( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y > pl_w->ground_y ) return 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->air_guard_cnt = 0 ;
	return 1 ;
}

/****************************************************************/
/*			The landing from a jump								*/
/****************************************************************/
void pl_land_set( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	act_st_set( pl_w, N_LAND ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = 2 ;
	land_ef_set( &pl_w->pos, pl_w->sjump_flag ? 1 : 0 ) ;
	fall_se_req( pl_w ) ;
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->weight > 5 ){
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
	if( pl_w->jump_dir == -1 ) pl_w->char_no.b.no = 3 ;
	if( pl_w->jump_dir == 0 ) pl_w->char_no.b.no = 4 ;
	if( pl_w->jump_dir == 1 ) pl_w->char_no.b.no = 5 ;
	pl_char_set( pl_w, Char_Jump, pl_w->char_no.b.no ) ;
}

/****************************************************************/
/*			Empty in the arcade build, and called from			*/
/*			nowhere												*/
/****************************************************************/
void pl_land_dummy( void )
{
}

/****************************************************************/
/*			A partner jumps in from off the screen, placed		*/
/*			by its place in the team							*/
/****************************************************************/
void pl_change_in_set( PLAYER_WORK *pl_w )
{
	Float ofs ;

	if( !( pl_w->wall_flag & 1 << pl_w->disp_dir ) ){
		ofs = DOT_X( 16.0f ) ;
		if( !pl_w->disp_dir ) pl_w->pos.x += pl_w->team_no * ofs ;
		else pl_w->pos.x -= pl_w->team_no * ofs ;
	}
	ofs = 640.0f ;
	if( pl_w->disp_dir ) ofs = -640.0f ;
	pl_w->pos.x += ofs ;
	pl_w->spd.x = -( ofs / 21.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->pos.y = pl_w->ground_y + DOT_Y( 64.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.75f ) ;
	pl_w->spd.y = ( pl_w->ground_y - pl_w->pos.y ) / 20.0f - pl_w->acc.y * 20.0f / 2.0f ;
}

/****************************************************************/
/*			The same, one step in								*/
/****************************************************************/
void pl_change_in_set2( PLAYER_WORK *pl_w )
{
	Float ofs ;

	if( !( pl_w->wall_flag & 1 << pl_w->disp_dir ) ){
		ofs = DOT_X( 16.0f ) ;
		if( !pl_w->disp_dir ) pl_w->pos.x += ofs ;
		else pl_w->pos.x -= ofs ;
	}
	ofs = 640.0f ;
	if( pl_w->disp_dir ) ofs = -640.0f ;
	pl_w->pos.x += ofs ;
	pl_w->spd.x = -( ofs / 21.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->pos.y = pl_w->ground_y + DOT_Y( 64.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.75f ) ;
	pl_w->spd.y = ( pl_w->ground_y - pl_w->pos.y ) / 20.0f - pl_w->acc.y * 20.0f / 2.0f ;
}

/****************************************************************/
/*			The same, placed by the character's own distance	*/
/****************************************************************/
void pl_assist_in_set( PLAYER_WORK *pl_w )
{
	Float ofs ;

	if( !( pl_w->wall_flag & 1 << pl_w->disp_dir ) ){
		ofs = DOT_X( change_in_ofs_tbl[pl_w->pl_type][pl_w->pl_personal] ) ;
		if( pl_w->disp_dir ) ofs = -ofs ;
		pl_w->pos.x += ofs ;
	}
	ofs = 640.0f ;
	if( pl_w->disp_dir ) ofs = -640.0f ;
	pl_w->pos.x += ofs ;
	pl_w->spd.x = -( ofs / 21.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_w->pos.y = pl_w->ground_y + DOT_Y( 64.0f ) ;
	pl_w->acc.y = -DOT_Y( 0.75f ) ;
	pl_w->spd.y = ( pl_w->ground_y - pl_w->pos.y ) / 20.0f - pl_w->acc.y * 20.0f / 2.0f ;
}

/****************************************************************/
/*			Into the air, keeping the act						*/
/****************************************************************/
void pl_air_set( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ) return ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->jump_atk_cnt = JATK_ONE ;
	pl_w->air_jump_cnt = 0 ;
}

/****************************************************************/
/*			The player's act: keeps the one before, and a		*/
/*			special move or hyper combo counts for the team		*/
/****************************************************************/
void act_st_set( PLAYER_WORK *pl_w, Uint8 act )
{
	PLAYER_WORK *w ;

	pl_w->old_act_st = pl_w->act_st ;
	pl_w->act_st = act ;
	if( act != N_SPECIAL && act != N_SUPER ){
		if( ( pl_w->old_act_st == N_SPECIAL || pl_w->old_act_st == N_SUPER || pl_w->old_act_st == N_CATCH ) && act != 30
		 && pl_w->team_type != 8 ) pl_w->team_type = 0 ;
		if( act == 32 || act == 31 ) pl_w->team_type = 0 ;
	} else if( act == N_SPECIAL || act == N_SUPER ){
		if( act == N_SPECIAL ) pl_w->cancel_timer = 0xff ;
		if( act == N_SUPER ){
			if( pl_w->old_act_st == N_SPECIAL ) pl_w->team_type = 0 ;
#ifdef TARGET_DC	/* a home rule: a player may join the chain of hyper combos again */
			if( pl_w->team_type != 6 ){
				if( !Sys->delay_hyper_free ) vs_sys_work.hyper_bit[pl_w->pl_no] = ( 1 << pl_w->work_no / 2 ) + 0x10 ;
				else vs_sys_work.hyper_bit[pl_w->pl_no] = 0x10 ;
				vs_sys_work.hyper_cnt[pl_w->pl_no] = 1 ;
				vs_sys_work.hyper_chain_cnt[pl_w->pl_no] = 1 ;
			} else {
				if( !Sys->delay_hyper_free ) vs_sys_work.hyper_bit[pl_w->pl_no] |= 1 << pl_w->work_no / 2 ;
				vs_sys_work.hyper_cnt[pl_w->pl_no] ++ ;
				if( Sys->delay_hyper_free && ( Sint8 )vs_sys_work.hyper_cnt[pl_w->pl_no] > 3 ) vs_sys_work.hyper_cnt[pl_w->pl_no] = 1 ;
				if( ++ vs_sys_work.hyper_chain_cnt[pl_w->pl_no] > 3 ) vs_sys_work.hyper_chain_cnt[pl_w->pl_no] = 3 ;
			}
#else
			if( pl_w->team_type != 6 ){
				vs_sys_work.hyper_bit[pl_w->pl_no] = ( 1 << pl_w->work_no / 2 ) + 0x10 ;
				vs_sys_work.hyper_cnt[pl_w->pl_no] = 1 ;
				vs_sys_work.hyper_chain_cnt[pl_w->pl_no] = 1 ;
			} else {
				vs_sys_work.hyper_bit[pl_w->pl_no] |= 1 << pl_w->work_no / 2 ;
				vs_sys_work.hyper_cnt[pl_w->pl_no] ++ ;
				if( ++ vs_sys_work.hyper_chain_cnt[pl_w->pl_no] > 3 ) vs_sys_work.hyper_chain_cnt[pl_w->pl_no] = 3 ;
			}
#endif
			w = vs_sys_w->team_w[pl_w->pl_no][1] ;
			memset( &w->com_w, 0, 104 ) ;
		}
	}
}

static void act_stand( PLAYER_WORK *pl_w ) ;
static void act_walk( PLAYER_WORK *pl_w ) ;
static void act_jump( PLAYER_WORK *pl_w ) ;
static void act_crouch( PLAYER_WORK *pl_w ) ;
static void act_crouching( PLAYER_WORK *pl_w ) ;
static void act_stand_up( PLAYER_WORK *pl_w ) ;
static void act_turn( PLAYER_WORK *pl_w ) ;
static void act_crouch_turn( PLAYER_WORK *pl_w ) ;
static void act_dash( PLAYER_WORK *pl_w ) ;
static void act_fly( PLAYER_WORK *pl_w ) ;
static void act_wall_jump( PLAYER_WORK *pl_w ) ;
static void act_air_dash( PLAYER_WORK *pl_w ) ;
static void act_back_dash( PLAYER_WORK *pl_w ) ;
static void act_dizzy( PLAYER_WORK *pl_w ) ;
static void act_sjump( PLAYER_WORK *pl_w ) ;
static void act_air_recover( PLAYER_WORK *pl_w ) ;
static void act_fall( PLAYER_WORK *pl_w ) ;
static void act_get_up( PLAYER_WORK *pl_w ) ;
static void act_guard( PLAYER_WORK *pl_w ) ;
static void act_air_guard( PLAYER_WORK *pl_w ) ;
static void act_nml_atck( PLAYER_WORK *pl_w ) ;
static void act_special( PLAYER_WORK *pl_w ) ;
static void act_win( PLAYER_WORK *pl_w ) ;
static void act_down( PLAYER_WORK *pl_w ) ;
static void act_escape( PLAYER_WORK *pl_w ) ;
static void act_change( PLAYER_WORK *pl_w ) ;
static void act_cmd_atck( PLAYER_WORK *pl_w ) ;
static Uint8 sjump_ck( PLAYER_WORK *pl_w ) ;

/****************************************************************/
/*			A new act: its routine sets the player up			*/
/****************************************************************/
int pl_act_set( PLAYER_WORK *pl_w, Uint8 act )
{
	static void ( *act_jmp[] )( PLAYER_WORK *pl_w ) = {
		act_stand, act_walk, act_jump, act_jump, act_jump, act_crouch, act_crouching, act_stand_up,
		act_turn, act_crouch_turn, act_dash, act_back_dash, act_dizzy, act_sjump, act_sjump, act_air_recover,
		act_fall, act_get_up, act_guard, act_air_guard, act_nml_atck, act_special, act_win, act_down,
		act_fly, act_wall_jump, act_air_dash, act_escape, act_change, act_special, act_stand, act_stand,
		act_stand, act_stand, act_cmd_atck,
	} ;

	pl_w->disp_flag = 1 ;
	act_st_set( pl_w, act ) ;
	pl_w->routine_1 = pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->rot[2] = 0 ;
	pl_w->scale = pl_w->pl_scale ;
	if( act != 23 ) pl_w->down_hit_flag = pl_w->down_lift_flag = 0 ;
	vs_sys_work.move_again_flag = 1 ;
	act_jmp[act]( pl_w ) ;
	if( pl_w->act_st != N_DOWN ) combo_reset( pl_w ) ;
	return 1 ;
}

/*==============================================================*/
static void act_stand( PLAYER_WORK *pl_w )
{
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( pl_w->float_flag ){
		pl_w->float_y = pl_w->pos.y ;
		pl_w->float_st = 0 ;
		pl_w->float_wait = 0 ;
	}
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->jump_dir = -1 ;
	switch( pl_w->pl_type ){
	case PL_CABLE:
		if( pl_w->char_no.w == 0x702 || pl_w->char_no.w == 0x703 || pl_w->char_no.w == 0xf01 ) break ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
		break ;
	case PL_CAPTAIN_COMMANDO:
		pl_w->pl_tbl[TBL_EX2]( pl_w ) ;
		break ;
	case PL_SERVBOT:
		pl_w->pl_tbl[TBL_EX0]( pl_w ) ;
		break ;
	default:
		if( pl_w->armor_flag & ARMOR_HEAVY ) pl_w->char_no.b.no = 1 ;
		else pl_w->char_no.b.no = 0 ;
		pl_char_set( pl_w, Char_Stand, pl_w->char_no.b.no ) ;
	}
}

/*==============================================================*/
static void act_walk( PLAYER_WORK *pl_w )
{
	int no ;

	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	no = ( pl_w->pl_sw_new & KEY_BACK ) >> 10 ;
	pl_w->jump_dir = no ;
	if( pl_w->armor_flag & ARMOR_HEAVY ) no += 6 ;
	pl_char_set( pl_w, Char_Stand, no + 2 ) ;
}

/*==============================================================*/
static void act_jump( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( pl_w->cpu_flag ) pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->sjump_cnt = 0 ;
	pl_w->sjump_flag = 0 ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->jump_atk_cnt = JATK_ONE ;
	pl_w->air_jump_cnt = 0 ;
	if( sjump_ck( pl_w ) ) return ;
	if( !pl_w->cpu_flag ){
		if( !( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ) ) pl_w->jump_dir = -1 ;
		else if( pl_w->pl_sw_new & KEY_FORWARD ) pl_w->jump_dir = 0 ;
		else pl_w->jump_dir = 1 ;
	}
	switch( pl_w->jump_dir ){
	case 0:
		no = 1 ;
		break ;
	case 1:
		no = 2 ;
		break ;
	default:
		no = 0 ;
		break ;
	}
	pl_char_set( pl_w, Char_Jump, no ) ;
	pl_jump_spd_set( pl_w ) ;
}

/*==============================================================*/
static void act_crouch( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_brake_set( pl_w ) ;
	if( sjump_ck( pl_w ) ) return ;
	if( !ABYSS_CK( pl_w ) ) pl_char_set( pl_w, Char_Crouch, 0 ) ;
}

/*==============================================================*/
static void act_crouching( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_brake_set( pl_w ) ;
	if( sjump_ck( pl_w ) ) return ;
	switch( pl_w->pl_type ){
	}
	pl_char_set( pl_w, Char_Crouch, 1 ) ;
}

/*==============================================================*/
static void act_stand_up( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Crouch, 2 ) ;
}

/*==============================================================*/
static void act_turn( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Turn, 0 ) ;
}

/*==============================================================*/
static void act_crouch_turn( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Turn, 1 ) ;
}

/*==============================================================*/
static void act_dash( PLAYER_WORK *pl_w )
{
	if( pl_w->cpu_flag ) pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->jump_dir = 0 ;
	if( sjump_ck( pl_w ) ) return ;
	dash_ef_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( PL_CK( pl_w, PL_SAKURA ) ) pl_w->pl_tbl[TBL_EX1]( pl_w ) ;
	else pl_char_set( pl_w, Char_Dash, 0 ) ;
	pl_se_req( pl_w, 51 ) ;
}

/*==============================================================*/
static void act_fly( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->jump_dir = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_char_set( pl_w, Char_Fly, 1 ) ;
}

/*==============================================================*/
static void act_wall_jump( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void act_air_dash( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->atck_ng_timer = 2 ;
	pl_w->jump_dir = 0 ;
}

/*==============================================================*/
static void act_back_dash( PLAYER_WORK *pl_w )
{
	if( pl_w->cpu_flag ) pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->jump_dir = 1 ;
	if( sjump_ck( pl_w ) ) return ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( PL_CK( pl_w, PL_SAKURA ) ) pl_w->pl_tbl[TBL_EX2]( pl_w ) ;
	else pl_char_set( pl_w, Char_Dash, 1 ) ;
}

/*==============================================================*/
static void act_dizzy( PLAYER_WORK *pl_w )
{
	pl_w->stun_ng_flag = 0xff ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	if( pl_w->vital_new ){ }
	pl_w->dizzy_timer = 150 ;
	pl_w->renda_cnt = 90 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Dizzy, 0 ) ;
}

/*==============================================================*/
static void act_sjump( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->pl_stance = STANCE_STAND ;
	pl_w->sjump_flag = 1 ;
	pl_w->sjump_cnt = 0 ;
	pl_w->jump_hit_flag = 0 ;
	pl_w->jump_com_cnt = 0 ;
	pl_w->jump_dash_cnt = 0 ;
	pl_w->air_jump_cnt = 0 ;
	if( sjump_ck( pl_w ) ) return ;
	if( !pl_w->cpu_flag ){
		if( !( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ) ){
			pl_w->jump_dir = -1 ;
			pl_w->char_no.b.no = 6 ;
		} else if( pl_w->pl_sw_new & KEY_FORWARD ){
			pl_w->jump_dir = 0 ;
			pl_w->char_no.b.no = 7 ;
		} else {
			pl_w->jump_dir = 1 ;
			pl_w->char_no.b.no = 8 ;
		}
	}
	switch( pl_w->jump_dir ){
	case 0:
		no = 7 ;
		break ;
	case 1:
		no = 8 ;
		break ;
	default:
		no = 6 ;
		break ;
	}
	pl_char_set( pl_w, Char_Jump, no ) ;
	pl_jump_spd_set( pl_w ) ;
}

/*==============================================================*/
static void act_air_recover( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->jump_atk_cnt = -1 ;
	pl_w->routine_2 = 2 ;
	pl_w->sjump_flag = 1 ;
	pl_char_set( pl_w, Char_Damage, 30 ) ;
	combo_reset( pl_w ) ;
}

/*==============================================================*/
static void act_fall( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_char_set( pl_w, Char_Jump, 9 ) ;
}

/*==============================================================*/
static void act_get_up( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_w->pl_stance = STANCE_CROUCH ;
	pl_w->catch_ng_timer = 1 ;
	pl_w->dm_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->char_no.b.no = 0 ;
	if( pl_w->jump_dir == 0 ){
		if( pl_w->cpu_flag ) pl_w->char_no.b.no = ( Sys->game_timer0 & 1 ) + 1 ;
		if( pl_w->cpu_flag ) no = Rnd() & 1 ;
		else no = pl_w->atck_level ;
		pl_w->char_no.b.no = no + 1 ;
		pl_w->char_no.b.no = 2 ;
	} else if( pl_w->jump_dir == 1 ) pl_w->char_no.b.no = 3 ;
	pl_char_set( pl_w, Char_Get_Up, pl_w->char_no.b.no ) ;
}

Uint16 guard_cmd_data[2] = { 0xffff, 0xffff } ;	/* the command data a guard starts with: none */

/*==============================================================*/
static void act_guard( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	pl_brake_set( pl_w ) ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = 0.0f ;
	if( sjump_ck( pl_w ) ) return ;
	pl_w->guard_back_flag = 0 ;
	if( pl_w->old_act_st != N_GUARD && pl_w->old_act_st != N_AIR_GUARD ){
		pl_w->guard_timer = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
	pl_w->hit_data_adrs = 0 ;
	pl_w->step_adrs = guard_cmd_data ;
	if( pl_w->pl_stance != STANCE_CROUCH ){
		pl_w->char_no.b.no = 0 ;
		pl_w->pl_stance = STANCE_STAND ;
	} else {
		pl_w->char_no.b.no = 4 ;
		pl_w->pl_stance = STANCE_CROUCH ;
	}
	pl_char_set( pl_w, Char_Guard, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void act_air_guard( PLAYER_WORK *pl_w )
{
	pl_dir_set( pl_w ) ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_w->guard_back_flag = 0 ;
	if( !pl_w->fly_flag && pl_w->acc.y > -0.066964284f ) pl_w->acc.y = -DOT_Y( 0.375f ) ;
	if( pl_w->old_act_st != N_GUARD && pl_w->old_act_st != N_AIR_GUARD ) pl_w->guard_timer = 0 ;
	pl_char_set( pl_w, Char_Air_Guard, 0 ) ;
}

/*==============================================================*/
static void act_nml_atck( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void act_special( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void act_win( PLAYER_WORK *pl_w )
{
	Uint8 type ;
	Sint8 win ;

	pl_tlocal_init( pl_w ) ;
	win = vs_sys_work.winner_id ;
	if( win < 0 ) type = 4 ;
	else {
		type = 0 ;
		if( win != pl_w->pl_no ) type ++ ;
		if( vs_sys_work.time_over ) type += 2 ;
	}
	pl_w->type = type ;
	pl_w->pl_stance = STANCE_STAND ;
}

/*==============================================================*/
static void act_down( PLAYER_WORK *pl_w )
{
	pl_w->pl_stance = STANCE_DOWN ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	if( vs_sys_work.vs_st < VS_END_WAIT && !pl_w->ko_flag ){
		pl_w->catch_ng_timer = 2 ;
		pl_w->dm_flag = 0 ;
	}
	if( !pl_w->down_timer ){
		pl_w->down_timer = pl_w->dm_down_time ;
		if( !pl_w->renda_cnt ) pl_w->renda_cnt = 10 ;
	}
	if( ( Sint8 )pl_w->down_hit_flag < 0 ) pl_w->down_timer = 0 ;
	if( !pl_w->vital_new ) combo_reset( pl_w ) ;
	pl_char_set( pl_w, Char_Damage, 31 ) ;
}

/*==============================================================*/
static void act_escape( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, 1 ) ;
	pl_dir_set( pl_w ) ;
	if( !pl_w->type ){
		if( pl_w->pos.y < pl_w->ground_y + DOT_Y( 32.0f ) ) pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_AIR ;
		pl_w->sjump_cnt = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_dash_cnt = 0 ;
		pl_w->jump_atk_cnt = JATK_ONE ;
		pl_w->air_jump_cnt = 0 ;
		pl_w->spd.x = DOT_X( 7.0f ) ;
		if( pl_w->disp_dir ) pl_w->spd.x = -pl_w->spd.x ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 5.5f ) ;
		pl_w->acc.y = -DOT_Y( 0.25f ) ;
		pl_w->jump_dir = 1 ;
		pl_char_set( pl_w, Char_Damage, 32 ) ;
	} else {
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->pl_stance = STANCE_CROUCH ;
		pl_w->sjump_cnt = 0 ;
		pl_w->sjump_flag = 0 ;
		pl_w->jump_com_cnt = 0 ;
		pl_w->jump_dash_cnt = 0 ;
		pl_w->spd.x = DOT_X( 8.0f ) ;
		pl_w->acc.x = -1.25f ;
		if( pl_w->disp_dir ){
			pl_w->spd.x = -pl_w->spd.x ;
			pl_w->acc.x = -pl_w->acc.x ;
		}
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_char_set( pl_w, Char_Damage, 33 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*==============================================================*/
static void act_change( PLAYER_WORK *pl_w )
{
	switch( pl_w->change_type ){
	case 0:
		break ;
	}
}

/*==============================================================*/
static void act_cmd_atck( PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
/*			A jump started high enough off the ground goes		*/
/*			on as a super jump's fall							*/
/*==============================================================*/
static Uint8 sjump_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y - pl_w->ground_y < DOT_Y( 32.0f ) ){
		pl_w->pos.y = pl_w->ground_y ;
		return 0 ;
	} else {
		pl_sjump_ret( pl_w ) ;
		return 1 ;
	}
}

/****************************************************************/
/*			The partner's command check, while it waits off		*/
/*			the screen											*/
/****************************************************************/
void pl_wait_com_ck( PLAYER_WORK *pl_w )
{
	pl_w->change_req = 0 ;
	if( !( vs_sys_work.ctrl_flag & CTRL_WALK ) && !pl_w->team_no ) pl_w->pl_tbl[TBL_COM_CK]( pl_w ) ;
}

/****************************************************************/
/*			During a hyper combo, the next partner may join		*/
/*			in with a delayed hyper combo of its own			*/
/****************************************************************/
void pl_delay_hyper_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( pl_w->act_st != N_SUPER ) return ;
	if( pl_w->change_req ) return ;
	if( vs_sys_work.ctrl_flag & CTRL_WALK ) return ;
	ew = vs_sys_work.team_w[pl_w->pl_no][1] ;
	if( !( vs_sys_w->hyper_bit[ew->pl_no] & 1 << ew->work_no / 2 ) && !ew->be_flag && ew->vital_new > 0
	 && !ew->vital_red_wait ){
		ew->pl_new_lvbt = pl_w->pl_new_lvbt ;
		ew->pl_now_lvbt = pl_w->pl_now_lvbt ;
		ew->pl_last_lvbt = pl_w->pl_last_lvbt ;
		ew->pl_old_lvbt = pl_w->pl_old_lvbt ;
		ew->pl_old2_lvbt = pl_w->pl_old2_lvbt ;
		ew->pl_sw_new = pl_w->pl_sw_new ;
		ew->pl_sw_now = pl_w->pl_sw_now ;
		ew->pl_sw_old = pl_w->pl_sw_old ;
		if( ( ( int (*)( PLAYER_WORK * ) )ew->pl_tbl[TBL_DELAY_CK] )( ew ) ){
			pl_w->change_req = 6 ;
			pl_w->partner_no = 1 ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_act.c" End of File	*/
