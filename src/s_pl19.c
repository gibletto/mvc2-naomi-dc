/* Abyss's second form (character 19): command checks, normal attacks, appearance, win and lose
   scenes, special moves, the partner jump and the change on a KO. He has no throws. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl_act.h"
#include "pl19_def.h"

void pl19_main( PLAYER_WORK *pl_w );
void pl19_com_ck( PLAYER_WORK *pl_w );
void pl19_atck_set( PLAYER_WORK *pl_w );
void pl19_atck( PLAYER_WORK *pl_w );
void pl19_dash( PLAYER_WORK *pl_w );
void pl19_noki( PLAYER_WORK *pl_w );
void pl19_appear( PLAYER_WORK *pl_w );
void pl19_win( PLAYER_WORK *pl_w );
void pl19_sp_atck( PLAYER_WORK *pl_w );
int pl19_catch_ck( PLAYER_WORK *pl_w );
void pl19_catch_set( PLAYER_WORK *pl_w );
void pl19_catch( PLAYER_WORK *pl_w );
void pl19_hold( PLAYER_WORK *pl_w );
int pl19_delay_ck( PLAYER_WORK *pl_w );
void pl19_t_kick( PLAYER_WORK *pl_w );
void pl19_sp_unison( PLAYER_WORK *pl_w );
void pl19_sp_gattai( PLAYER_WORK *pl_w );
void pl19_sp_assist( PLAYER_WORK *pl_w );
void pl19_sp_zero( PLAYER_WORK *pl_w );
void pl19_ko_set( PLAYER_WORK *pl_w );

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_bubble[] = { 3, COM_GROUND, pl19_sp_bubble, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* bubbles that hold whoever they touch */
static Uint16 com_storm[] = { 3, COM_GROUND | COM_HYPER, pl19_sp_storm, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ; /* shots in every direction */
static Uint16 com_beam[] = { 3, COM_GROUND, pl19_sp_beam, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* a beam, longer for pressed buttons */
static Uint16 com_blast[] = { 3, COM_GROUND, pl19_sp_blast, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* a blast at his side */

static void ( *pl19_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl19_main,
	pl19_com_ck,
	pl19_atck_set,
	pl19_atck,
	pl19_dash,
	pl19_noki,
	pl19_appear,
	pl19_win,
	pl19_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl19_catch_ck,	/* returns a value */
	pl19_catch_set,
	pl19_catch,
	pl19_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl19_delay_ck,
	pl19_t_kick,
	0,
	pl19_sp_unison,
	pl19_sp_gattai,
	pl19_sp_assist,
	pl19_sp_zero,
	0,
	0,
	0,
	0,
	pl19_ko_set,
} ;

static Uint8 bubble_ck( PLAYER_WORK *pl_w );
static Uint8 storm_ck( PLAYER_WORK *pl_w );
static Uint8 beam_ck( PLAYER_WORK *pl_w );
static Uint8 blast_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl19_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl19_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl19_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( bubble_ck( pl_w ) ) return ;
	if( storm_ck( pl_w ) ) return ;
	if( beam_ck( pl_w ) ) return ;
	if( blast_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
static Uint8 bubble_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_bubble, &pl_w->com_w ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl19_sp_bubble ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 storm_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_storm, &pl_w->com_w0 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl19_sp_storm ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 beam_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_beam, &pl_w->com_w1 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w1 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl19_sp_beam ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 blast_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_blast, &pl_w->com_w2 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w2 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl19_sp_blast ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
int pl19_delay_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/****************************************************************/
/*			Cycles the body's colours; when the round is		*/
/*			over he changes										*/
/****************************************************************/
void pl19_main( PLAYER_WORK *pl_w )
{
	COL_WORK *ow = &col_work[pl_w->col_no] ;
	PL19_LOCAL_W *lw = ( PL19_LOCAL_W * )pl_w->tlocal ;

	switch( lw->col_step ){
	case 0:
		lw->col_step ++ ;
		ow->trans_flag = 1 ;
		ow->col[10] |= 0xc000 ;
		ow->col[11] |= 0xc000 ;
		ow->col[12] |= 0xc000 ;
		ow->col[13] |= 0xc000 ;
		ow->col[14] |= 0xc000 ;
		lw->col_timer = 4 ;
	case 1:
		if( !lw->col_timer -- ){
			lw->col_timer = 4 ;
			ow->trans_flag = 1 ;
			ow->col[10] -= 0x1000 ;
			ow->col[11] -= 0x1000 ;
			ow->col[12] -= 0x1000 ;
			ow->col[13] -= 0x1000 ;
			ow->col[14] -= 0x1000 ;
			if( ( ow->col[10] & COL_ALPHA ) == 0x7000 ) lw->col_step ++ ;
		}
		break ;
	case 2:
		if( !lw->col_timer -- ){
			lw->col_timer = 4 ;
			ow->trans_flag = 1 ;
			ow->col[10] += 0x1000 ;
			ow->col[11] += 0x1000 ;
			ow->col[12] += 0x1000 ;
			ow->col[13] += 0x1000 ;
			ow->col[14] += 0x1000 ;
			if( ( ow->col[10] & COL_ALPHA ) == 0xf000 ) lw->col_step = 1 ;
		}
		break ;
	}
	if( vs_sys_w->vs_st >= VS_END_WAIT
	 && ( vs_sys_w->winner_id < 0 || vs_sys_w->winner_id != pl_w->pl_no )
	 && pl_w->routine_1 == PL_NORM && pl_w->act_st == N_STAND ){
		pl_act_set( pl_w, N_SPECIAL ) ;
		pl_w->pl_sp_type = pl19_sp_change ;
	}
	if( !pl_w->vital_new ) pl_w->push_ng_timer = 1 ;
}

/*==============================================================*/
/*			Normal attacks: nml_no is the button, three			*/
/*			crouching punches and kicks, then standing ones		*/
/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	pl_brake_set( pl_w ) ;
	switch( pl_w->nml_no ){
	case 0:  pl_w->char_no.b.grp = 9 ;  pl_w->char_no.b.no = 0 ; no = 6 ;  break ;
	case 1:  pl_w->char_no.b.grp = 9 ;  pl_w->char_no.b.no = 1 ; no = 7 ;  break ;
	case 2:  pl_w->char_no.b.grp = 9 ;  pl_w->char_no.b.no = 2 ; no = 8 ;  break ;
	case 3:  pl_w->char_no.b.grp = 10 ; pl_w->char_no.b.no = 0 ; no = 9 ;  break ;
	case 4:  pl_w->char_no.b.grp = 10 ; pl_w->char_no.b.no = 1 ; no = 10 ; break ;
	case 5:  pl_w->char_no.b.grp = 10 ; pl_w->char_no.b.no = 2 ; no = 11 ; break ;
	case 6:  pl_w->char_no.b.grp = 7 ;  pl_w->char_no.b.no = 0 ; no = 0 ;  break ;
	case 7:  pl_w->char_no.b.grp = 7 ;  pl_w->char_no.b.no = 1 ; no = 1 ;  break ;
	case 8:  pl_w->char_no.b.grp = 7 ;  pl_w->char_no.b.no = 2 ; no = 2 ;  break ;
	case 9:  pl_w->char_no.b.grp = 8 ;  pl_w->char_no.b.no = 0 ; no = 3 ;  break ;
	case 10: pl_w->char_no.b.grp = 8 ;  pl_w->char_no.b.no = 1 ; no = 4 ;  break ;
	case 11: pl_w->char_no.b.grp = 8 ;  pl_w->char_no.b.no = 2 ; no = 5 ;  break ;
	}
	pl_w->atck_no = no ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	Uint8 no ;

	switch( pl_w->nml_no ){
	case 0: case 6:  pl_w->char_no.b.grp = 11 ; pl_w->char_no.b.no = 0 ; no = 12 ; break ;
	case 1: case 7:  pl_w->char_no.b.grp = 11 ; pl_w->char_no.b.no = 1 ; no = 13 ; break ;
	case 2: case 8:  pl_w->char_no.b.grp = 11 ; pl_w->char_no.b.no = 2 ; no = 14 ; break ;
	case 3: case 9:  pl_w->char_no.b.grp = 12 ; pl_w->char_no.b.no = 0 ; no = 15 ; break ;
	case 4: case 10: pl_w->char_no.b.grp = 12 ; pl_w->char_no.b.no = 1 ; no = 16 ; break ;
	case 5: case 11: pl_w->char_no.b.grp = 12 ; pl_w->char_no.b.no = 2 ; no = 17 ; break ;
	}
	pl_w->atck_no = no ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pl_char_set( pl_w, pl_w->char_no.b.grp, pl_w->char_no.b.no ) ;
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
void pl19_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_jump_ret( pl_w ) ;
		return ;
	}
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
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
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl19_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/****************************************************************/
void pl19_dash( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
void pl19_noki( PLAYER_WORK *pl_w )
{
	pl_stand_ret( pl_w ) ;
}

/****************************************************************/
/*			Not used											*/
/****************************************************************/
void pl19_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) pl_w->routine_2 ++ ;
}

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl19_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	pl_w->disp_flag = 0 ;
	if( vs_sys_work.vs_st < VS_APPEAR ) return ;
	pl_w->routine_2 ++ ;
	pl_w->disp_flag = 1 ;
	pl_dir_set( pl_w ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_1 ++ ;
}

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_pose( PLAYER_WORK *pl_w );
static void win_pose_init( PLAYER_WORK *pl_w );
static void win_pose_move( PLAYER_WORK *pl_w );
static void win_fly( PLAYER_WORK *pl_w );
static void fly_init( PLAYER_WORK *pl_w );
static void fly_land( PLAYER_WORK *pl_w );
static void fly_wait( PLAYER_WORK *pl_w );
static void fly_effect( PLAYER_WORK *pl_w );
static void fly_start( PLAYER_WORK *pl_w );
static void fly_up( PLAYER_WORK *pl_w );
static void fly_move( PLAYER_WORK *pl_w );
static void fly_wave( PLAYER_WORK *pl_w );
static void lose_set( PLAYER_WORK *pl_w );
static void win_type_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl19_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( !pl_w->type ) win_type_ck( pl_w ) ;
	win_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_move_jmp[])( PLAYER_WORK *pl_w ) = {
		win_pose,
		lose_set, lose_set, lose_set, lose_set,
		win_pose, win_pose, win_pose,
		win_fly, win_fly, win_fly,
	} ;

	win_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
static void win_pose( PLAYER_WORK *pl_w )
{
	static void ( *win_pose_jmp[])( PLAYER_WORK *pl_w ) = {
		win_pose_init, win_pose_move,
	} ;

	win_pose_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_pose_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 0 ) ;
	win_pose_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_pose_move( PLAYER_WORK *pl_w )
{
	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			He rises and floats									*/
/*==============================================================*/
static void win_fly( PLAYER_WORK *pl_w )
{
	static void ( *win_fly_jmp[])( PLAYER_WORK *pl_w ) = {
		fly_init, fly_land, fly_wait, fly_effect, fly_start, fly_up, fly_move,
	} ;

	win_fly_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void fly_init( PLAYER_WORK *pl_w )
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
	pl_char_set( pl_w, Char_Win, 1 ) ;
	fly_land( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fly_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 0 ;
		pl_w->timer0 = 10 ;
		pls19_01_set( pl_w, 0 ) ;
		fly_wait( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void fly_wait( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer1 = 30 ;
	}
}

/*--------------------------------------------------------------*/
static void fly_effect( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer1 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer1 = 30 ;
		pls19_00_set( pl_w, 14 ) ;
		pls19_00_set( pl_w, 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void fly_start( PLAYER_WORK *pl_w )
{
	if( -- pl_w->timer1 <= 0 ){
		pl_w->routine_3 ++ ;
		pl_w->disp_flag = 1 ;
		pl_w->timer0 = 10 ;
		pl_w->timer1 = 0 ;
		pl_w->spd.y = pl_w->pos.y ;
		pl_w->alpha = 0.0f ;
		pl_char_set( pl_w, Char_Win, 3 ) ;
	}
}

/*==============================================================*/
/*			He fades in											*/
/*==============================================================*/
static void fly_up( PLAYER_WORK *pl_w )
{
	pl_w->alpha += ( 0.75f - pl_w->alpha ) / pl_w->timer0 ;
	if( -- pl_w->timer0 <= 0 ) pl_w->routine_3 ++ ;
	fly_wave( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void fly_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	fly_wave( pl_w ) ;
}

/*==============================================================*/
/*			Bobs up and down about the height kept in spd.y		*/
/*==============================================================*/
static void fly_wave( PLAYER_WORK *pl_w )
{
	Sint16 ang ;
	Float amp ;

	pl_w->pos.y = pl_w->spd.y ;
	amp = 256000.0f ;
	ang = ( ( 72 - pl_w->timer1 ) & 0x3f ) << 10 ;
	pl_w->pos.y = pl_w->pos.y + 2.142857f * ( amp * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f ) ;
	pl_w->timer1 ++ ;
	pl_w->timer1 &= 0x3f ;
}

/*==============================================================*/
static void lose_set( PLAYER_WORK *pl_w )
{
	pl_act_set( pl_w, N_SPECIAL ) ;
	pl_w->pl_sp_type = pl19_sp_change ;
	pl19_sp_atck( pl_w ) ;
}

/* the scene for a win picked by the button held, or at random */
static Sint16 win_type_tbl[16] = { 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 10, 10, 10 } ;

/*==============================================================*/
static void win_type_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_new_lvbt & KEY_BUTTON ){
		if( pl_w->pl_new_lvbt & KEY_LP ) pl_w->type = 5 ;
		else if( pl_w->pl_new_lvbt & KEY_HP ) pl_w->type = 6 ;
		else if( pl_w->pl_new_lvbt & KEY_A1 ) pl_w->type = 7 ;
		else if( pl_w->pl_new_lvbt & KEY_LK ) pl_w->type = 8 ;
		else if( pl_w->pl_new_lvbt & KEY_HK ) pl_w->type = 9 ;
		else pl_w->type = 10 ;
	} else pl_w->type = win_type_tbl[Rnd() & 0x0f] ;
	if( !vs_sys_work.vs_end_flag ){
		if( pl_w->type == 8 || pl_w->type == 9 || pl_w->type == 10 ) pl_w->type = 5 ;
	}
}

static void sp_bubble( PLAYER_WORK *pl_w );
static void sp_storm( PLAYER_WORK *pl_w );
static void sp_beam( PLAYER_WORK *pl_w );
static void sp_blast( PLAYER_WORK *pl_w );
static void sp_change( PLAYER_WORK *pl_w );

/****************************************************************/
void pl19_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *pl_sp_tbl[])( PLAYER_WORK *pl_w ) = {
		sp_bubble, sp_storm, sp_beam, sp_blast, sp_change,
	} ;

	pl_sp_tbl[pl_w->pl_sp_type]( pl_w ) ;
}

static void bubble_init( PLAYER_WORK *pl_w );
static void bubble_move( PLAYER_WORK *pl_w );
static void bubble_fade( PLAYER_WORK *pl_w );
static void bubble_shot0( PLAYER_WORK *pl_w );
static void bubble_shot1( PLAYER_WORK *pl_w );
static void bubble_shot2( PLAYER_WORK *pl_w );
static void bubble_back( PLAYER_WORK *pl_w );
static void bubble_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			He sinks into the floor and fires three shots		*/
/*==============================================================*/
static void sp_bubble( PLAYER_WORK *pl_w )
{
	static void ( *bubble_jmp[])( PLAYER_WORK *pl_w ) = {
		bubble_init, bubble_move, bubble_fade, bubble_shot0, bubble_shot1, bubble_shot2, bubble_back, bubble_end,
	} ;

	bubble_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void bubble_init( PLAYER_WORK *pl_w )
{
	COL_WORK *ow ;
	int i ;

	pl_w->routine_2 ++ ;
	( ( PL19_LOCAL_W * )pl_w->tlocal )->bubble_catch_cnt = 0 ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->alpha = 1.0f ;
	ow = &col_work[pl_w->col_no] + 3 ;
	ow->trans_flag = 1 ;
	for( i = 0 ; i <= 11 ; i++ ) ow->col[i + 4] = ( ow->col[i + 4] & COL_RGB ) | 0x8000 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Crouch, 0 ) ;
	pl_voice_req( pl_w, 10 ) ;
	pl_sp_ef_set( pl_w ) ;
	bubble_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void bubble_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
	if( pl_w->cg_type ) pl_w->push_ng_timer = 1 ;
}

/*--------------------------------------------------------------*/
static void bubble_fade( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 1 ;
	if( ( pl_w->alpha -= 0.025f ) <= 0.0f ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 0 ;
		pl_w->alpha = 1.0f ;
		pl_w->timer0 = 20 ;
	}
}

/*--------------------------------------------------------------*/
static void bubble_shot0( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 1 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		shl19_00_set( pl_w, 0, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bubble_shot1( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 1 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		shl19_00_set( pl_w, 0, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bubble_shot2( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 1 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 20 ;
		shl19_00_set( pl_w, 0, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bubble_back( PLAYER_WORK *pl_w )
{
	pl_w->push_ng_timer = 1 ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_w->disp_flag = 1 ;
		pl_char_set( pl_w, Char_Crouch, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void bubble_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ) pl_w->push_ng_timer = 1 ;
}

static void storm_init( PLAYER_WORK *pl_w );
static void storm_shot( PLAYER_WORK *pl_w );
static void storm_set( PLAYER_WORK *pl_w );
static void storm_start( PLAYER_WORK *pl_w );
static void storm_move( PLAYER_WORK *pl_w );
static void storm_end( PLAYER_WORK *pl_w );
static int storm_effect( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hyper combo										*/
/*==============================================================*/
static void sp_storm( PLAYER_WORK *pl_w )
{
	static void ( *storm_jmp[])( PLAYER_WORK *pl_w ) = {
		storm_init, storm_shot, storm_set, storm_start, storm_move, storm_end,
	} ;

	storm_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void storm_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->atck_no = pl19_at_storm ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	pl_sp_ef_set( pl_w ) ;
	storm_shot( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void storm_shot( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		ofs.x = DOT_X( 4.0f ) ;
		ofs.y = DOT_Y( 111.0f ) ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_char_set( pl_w, Char_Special, 4 ) ;
	storm_start( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void storm_start( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = 0 ;
		pl_w->timer1 = 1 ;
		pls19_00_set( pl_w, 7 ) ;
		pls19_00_set( pl_w, 8 ) ;
		pls19_00_set( pl_w, 9 ) ;
		pls19_00_set( pl_w, 10 ) ;
		pls19_00_set( pl_w, 11 ) ;
		pls19_00_set( pl_w, 12 ) ;
		pls19_00_set( pl_w, 13 ) ;
		pl_w->dir = 50 ;
	}
}

/*--------------------------------------------------------------*/
static void storm_move( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->dir && !-- pl_w->dir ) pl_voice_req( pl_w, 10 ) ;
	if( storm_effect( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set2( pl_w, Char_Special, 5, pl_w->cg_prm ) ;
	}
	pl_w->routine_3 ++ ;
	if( pl_w->routine_3 == 2 ) ef_color_set( pl_w, 5, 0, 1 ) ;
	if( pl_w->routine_3 == 4 ){
		pl_w->routine_3 = 0 ;
		pl_color_trans( pl_w, 5, 5, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void storm_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	if( pl_w->routine_3 == 2 ) ef_color_set( pl_w, 5, 0, 1 ) ;
	if( pl_w->routine_3 == 4 ){
		pl_w->routine_3 = 0 ;
		pl_color_trans( pl_w, 5, 5, 1 ) ;
	}
}

/* the shots, each with the frames to wait after it, in pairs, to the -1 that ends them */
static Sint8 storm_shot_tbl[] = {
	0x18, 1, 0x1a, 1, 0x1c, 1, 0x1e, 8,
	0x00, 1, 0x02, 1, 0x04, 1, 0x06, 1, 0x08, 1, 0x0a, 1, 0x0c, 1,
	0x0e, 1, 0x10, 1, 0x12, 1, 0x14, 1, 0x16, 1, 0x18, 1, 0x1a, 1,
	0x1c, 1, 0x1e, 8,
	0x00, 1, 0x02, 1, 0x04, 1, 0x06, 1, 0x08, 1, 0x0a, 1, 0x0c, 1,
	0x0e, 1, 0x10, 1, 0x12, 1, 0x14, 1, 0x16, 1, 0x18, 1, 0x1a, 1,
	0x1c, 1, 0x1e, 8,
	0x00, 1, 0x02, 1, 0x04, 1, 0x06, 1, 0x08, 1, 0x0a, 1, 0x0c, 1,
	0x0e, 1, 0x10, 1, 0x12, 1, 0x14, 1, 0x16, 1, 0x18, 1, 0x1a, 1,
	0x1c, 1, 0x1e, 8,
	0x00, 1, 0x02, 1, 0x04, 1, 0x06, 1, 0x08, 1, 0x0a, 1, 0x0c, 1,
	0x0e, 1, 0x10, 1, 0x12, 1, 0x14, 1, 0x16, 1, 0x18, 1, 0x1a, 1,
	0x1c, 1, 0x1e, 8,
	0x00, 1, 0x02, 1, 0x04, 1, 0x06, 1, 0x08, 1, 0x0a, 1, 0x0c, 1,
	0x0e, 1, 0x10, 1, 0x12, 1, 0x14, 1, 0x16, 1,
	-1, 0,
} ;

/*==============================================================*/
/*			Fires the shots due; returns -1 when they are		*/
/*			all done (the table is read as bytes, a shot and	*/
/*			its wait to each pair)								*/
/*==============================================================*/
static int storm_effect( PLAYER_WORK *pl_w )
{
	Sint8 no = 0 ;

	if( !-- pl_w->timer1 ){
		do {
			if( ( no = storm_shot_tbl[pl_w->timer0 * 2] ) == -1 ) break ;
			shl19_02_set( pl_w, 0, no ) ;
			pl_w->timer1 = storm_shot_tbl[pl_w->timer0 * 2 + 1] ;
			pl_w->timer0 ++ ;
		} while( !pl_w->timer1 ) ;
	}
	return no ;
}

static void beam_init( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw );
static void beam_wait( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw );
static void beam_start( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw );
static void beam_shot( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw );
static void beam_hold( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw );
static void beam_end( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw );

/*==============================================================*/
/*			A beam, made to last longer by pressing buttons		*/
/*==============================================================*/
static void sp_beam( PLAYER_WORK *pl_w )
{
	static void ( *beam_jmp[])( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw ) = {
		beam_init, beam_wait, beam_start, beam_shot, beam_hold, beam_end,
	} ;

	beam_jmp[pl_w->routine_2]( pl_w, ( PL19_LOCAL_W * )pl_w->tlocal ) ;
}

/*==============================================================*/
static void beam_init( register PLAYER_WORK *pl_w, PL19_LOCAL_W *lw )
{
	COL_WORK *ow ;
	int i ;

	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ow = &col_work[pl_w->col_no] + 5 ;
	ow->trans_flag = 1 ;
	for( i = 9 ; i <= 11 ; i++ ) ow->col[i + 4] = ( ow->col[i + 4] & COL_RGB ) | 0x8000 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	lw->renda_cnt = pl_w->cpu_flag ? 12 : 24 ;
	pl_w->timer0 = !pl_w->atck_level ? 60 : 90 ;
	pl_gauge_add( pl_w, pl_w->atck_level + 5 ) ;
	pl_w->atck_no = pl_w->atck_level + 51 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, pl_w->atck_level + 6 ) ;
	pl_sp_ef_set( pl_w ) ;
	beam_wait( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void beam_wait( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_prm ){
		pl_w->routine_2 ++ ;
		if( pl_w->atck_level >= 1 ) pls19_00_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beam_start( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		pls19_00_set( pl_w, pl_w->atck_level + 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beam_shot( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->routine_2 ++ ;
		shl19_03_set( pl_w, 0 ) ;
		pls19_00_set( pl_w, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beam_hold( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw )
{
	pl_char_move( pl_w ) ;
	if( lw->renda_cnt >= 0 && sw_renda_ck( pl_w ) ){
		lw->renda_cnt -- ;
		pl_w->timer0 ++ ;
	}
	if( -- pl_w->timer0 <= 0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, pl_w->atck_level + 8 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beam_end( PLAYER_WORK *pl_w, PL19_LOCAL_W *lw )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void blast_init( PLAYER_WORK *pl_w );
static void blast_shot( PLAYER_WORK *pl_w );
static void blast_wait( PLAYER_WORK *pl_w );
static void blast_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_blast( PLAYER_WORK *pl_w )
{
	static void ( *blast_jmp[])( PLAYER_WORK *pl_w ) = {
		blast_init, blast_shot, blast_wait, blast_end,
	} ;

	blast_jmp[pl_w->routine_2]( pl_w ) ;
}

/* the animations for each strength */
static Sint8 blast_char_tbl[2][2] = { { 1, 2 }, { 10, 11 } } ;

/*==============================================================*/
static void blast_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl_w->atck_level + 55 ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, blast_char_tbl[pl_w->atck_level][0] ) ;
	pl_sp_ef_set( pl_w ) ;
	blast_shot( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void blast_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->timer0 = !pl_w->atck_level ? 18 : 30 ;
		shl19_01_set( pl_w, pl_w->atck_level ) ;
	}
}

/*--------------------------------------------------------------*/
static void blast_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !-- pl_w->timer0 ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Special, blast_char_tbl[pl_w->atck_level][1] ) ;
	}
}

/*--------------------------------------------------------------*/
static void blast_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void change_init( PLAYER_WORK *pl_w );
static void change_move( PLAYER_WORK *pl_w );
static void change_fade( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			He fades away, to change into the next form			*/
/*==============================================================*/
static void sp_change( PLAYER_WORK *pl_w )
{
	static void ( *change_jmp[])( PLAYER_WORK *pl_w ) = {
		change_init, change_move, change_fade,
	} ;

	pl_w->push_ng_timer = 1 ;
	change_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void change_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->alpha = 1.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	change_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void change_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_w->routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void change_fade( PLAYER_WORK *pl_w )
{
	if( ( pl_w->alpha -= 0.025f ) <= 0.0f ){
		act_st_set( pl_w, N_DOWN ) ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		pl_w->pl_stance = STANCE_DOWN ;
		pl_w->disp_flag = 0 ;
	}
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_jump( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The partner's jump in								*/
/****************************************************************/
void pl19_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_jump, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( !pl_w->disp_dir ) pl_w->spd.x = -30.0f ;
	else pl_w->spd.x = 30.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl19_at_t_kick ;
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
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

/****************************************************************/
int pl19_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/****************************************************************/
void pl19_catch_set( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl19_catch( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl19_hold( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl19_sp_unison( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl19_sp_gattai( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl19_sp_assist( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
void pl19_sp_zero( PLAYER_WORK *pl_w )
{
}

/****************************************************************/
/*			Called on a KO: he fades away to change into the	*/
/*			next form											*/
/****************************************************************/
void pl19_ko_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl19_sp_change ;
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl19.c" End of File	*/
