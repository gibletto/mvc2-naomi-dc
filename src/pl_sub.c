/* The players' shared routines: setting up the six player works for a game, a round and a
   partner coming in, the players pushing each other apart, jumps, throw mashing, hyper combo
   flashes, landing effects and the return to standing. */
#define PL_HYPER_START_NEW
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl_act.h"

/* the routine tables pl_char_set() and the character modules fill, one for each work */
extern Uint8 weight_tbl[];
extern Uint8 pl_size_tbl[][4];	/* each character's body size */
extern Sint8 prio_ofs_tbl[];

/* each character's tbl_set, which fills its routine table */
extern void pl00_tbl_set( PLAYER_WORK *pl_w ), pl01_tbl_set( PLAYER_WORK *pl_w ), pl02_tbl_set( PLAYER_WORK *pl_w ), pl03_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl04_tbl_set( PLAYER_WORK *pl_w ), pl05_tbl_set( PLAYER_WORK *pl_w ), pl06_tbl_set( PLAYER_WORK *pl_w ), pl07_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl08_tbl_set( PLAYER_WORK *pl_w ), pl09_tbl_set( PLAYER_WORK *pl_w ), pl0a_tbl_set( PLAYER_WORK *pl_w ), pl0b_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl0c_tbl_set( PLAYER_WORK *pl_w ), pl0d_tbl_set( PLAYER_WORK *pl_w ), pl0e_tbl_set( PLAYER_WORK *pl_w ), pl0f_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl10_tbl_set( PLAYER_WORK *pl_w ), pl11_tbl_set( PLAYER_WORK *pl_w ), pl12_tbl_set( PLAYER_WORK *pl_w ), pl13_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl14_tbl_set( PLAYER_WORK *pl_w ), pl15_tbl_set( PLAYER_WORK *pl_w ), pl16_tbl_set( PLAYER_WORK *pl_w ), pl17_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl18_tbl_set( PLAYER_WORK *pl_w ), pl19_tbl_set( PLAYER_WORK *pl_w ), pl1a_tbl_set( PLAYER_WORK *pl_w ), pl1b_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl1c_tbl_set( PLAYER_WORK *pl_w ), pl1d_tbl_set( PLAYER_WORK *pl_w ), pl1e_tbl_set( PLAYER_WORK *pl_w ), pl1f_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl20_tbl_set( PLAYER_WORK *pl_w ), pl21_tbl_set( PLAYER_WORK *pl_w ), pl22_tbl_set( PLAYER_WORK *pl_w ), pl23_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl24_tbl_set( PLAYER_WORK *pl_w ), pl25_tbl_set( PLAYER_WORK *pl_w ), pl26_tbl_set( PLAYER_WORK *pl_w ), pl27_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl28_tbl_set( PLAYER_WORK *pl_w ), pl29_tbl_set( PLAYER_WORK *pl_w ), pl2a_tbl_set( PLAYER_WORK *pl_w ), pl2b_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl2c_tbl_set( PLAYER_WORK *pl_w ), pl2d_tbl_set( PLAYER_WORK *pl_w ), pl2e_tbl_set( PLAYER_WORK *pl_w ), pl2f_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl30_tbl_set( PLAYER_WORK *pl_w ), pl31_tbl_set( PLAYER_WORK *pl_w ), pl32_tbl_set( PLAYER_WORK *pl_w ), pl33_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl34_tbl_set( PLAYER_WORK *pl_w ), pl35_tbl_set( PLAYER_WORK *pl_w ), pl36_tbl_set( PLAYER_WORK *pl_w ), pl37_tbl_set( PLAYER_WORK *pl_w ) ;
extern void pl38_tbl_set( PLAYER_WORK *pl_w ), pl39_tbl_set( PLAYER_WORK *pl_w ), pl3a_tbl_set( PLAYER_WORK *pl_w ) ;

extern void pl_mark_init( void );
extern void team_w_set( void );
extern PLAYER_WORK *pls00_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls03_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl0b_00_set( PLAYER_WORK *pl_w );
extern void *pls0c_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls10_03_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *pls14_00_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *pls18_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls19_02_set( PLAYER_WORK *pl_w );
extern void pls1c_02_set( PLAYER_WORK *pl_w );
extern void pls1e_00_set( PLAYER_WORK *pl_w );
extern void pls27_00_set( PLAYER_WORK *pl_w );
extern void pls2a_02_set( PLAYER_WORK *pl_w );
extern void pls2a_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls2c_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern void pls38_01_set( PLAYER_WORK *pl_w );

/* the hyper combo flash's effects, screen and sound */
extern void hyper_wave_start( PLAYER_WORK *pl_w, VEC3 *ofs );
extern void pl_gauge_sub( PLAYER_WORK *pl_w, Sint8 no );
extern void hyper_flash_set( VEC3 *pos, Sint16 dir, int a, int b );
extern void vib_req( int pl_no, int no );
extern void hyper_se_req( PLAYER_WORK *pl_w, Uint8 no );
extern void pl_pic_set( PLAYER_WORK *pl_w, Uint8 no );
extern void snap_flash_set( VEC3 *pos, Sint16 dir, int a, int b );
extern void sp_ef_set( VEC3 *pos, int dir );
extern void slide_dust_set( PLAYER_WORK *pl_w );
extern void *memcpy( void *dst, const void *src, Uint32 n );

/* speeds in 1/256 dots, by character */
extern Sint16 jump_spd_tbl[];	/* a jump's speed and fall, up and sideways, by direction */
extern Sint16 sjump_spd_tbl[];	/* the same for a super jump */
extern Sint16 jump_limit_tbl[];	/* the slowest and fastest sideways speed in a jump */
extern Sint16 jump_limit_up_tbl[];	/* the same, sped up */
extern Sint16 sjump_limit_tbl[];	/* the same in a super jump */
extern Sint16 sjump_limit_up_tbl[];	/* and sped up */
extern Sint16 sjump_ctrl_tbl[][3][2];	/* steering a super jump, by direction and stick */
extern Sint16 sjump_fall_tbl[];	/* the fall from the top of a super jump */
extern Sint8 renda_rnd_tbl[16];	/* a random part of renda_cnt's mash */

void pl_work_init( PLAYER_WORK *pl_w );
void pl_tlocal_init( PLAYER_WORK *pl_w );
void pl_char_init( PLAYER_WORK *pl_w );

static void pl_work_clear_1p( void );
static void pl_work_clear_2p( void );
static void pl_restart_set( PLAYER_WORK *pl_w );
static void boss_change_set( PLAYER_WORK *pl_w, int no );
static void pl_work_set( PLAYER_WORK *pl_w );
static void tlocal_clear( PLAYER_WORK *pl_w );
static void pl_push( PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void push_both( PLAYER_WORK *pl_w, PLAYER_WORK *em, Float *x, Float ov );
static void push_one( PLAYER_WORK *pl_w, PLAYER_WORK *em, Float *x, Float ov );
static void stage_end_r( PLAYER_WORK *pl_w, PLAYER_WORK *em, SCR_WORK *sw );
static void stage_end_l( PLAYER_WORK *pl_w, PLAYER_WORK *em, SCR_WORK *sw );
static void lever_clear( PLAYER_WORK *pl_w );
static int renda_get( PLAYER_WORK *pl_w );
static void team_hyper_flash( PLAYER_WORK *pl_w, VEC3 *ofs );
static void hyper_stop_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Clear all six player works							*/
/****************************************************************/
void pl_work_clear( void )
{
	pl_work_clear_1p() ;
	pl_work_clear_2p() ;
}

/*==============================================================*/
static void pl_work_clear_1p( void )
{
	PLAYER_WORK *pl_w ;
	Uint8 i ;

	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = pl_work + i * 2 ;
		memset( pl_w, 0, 0x4bc ) ;
		pl_w->pl_no = 0 ;
		pl_w->work_no = i * 2 ;
		pl_w->team_no = i ;
		pl_w->team_no_org = i ;
	}
	pl_w = pl_work ;
	vs_sys_work.win_cnt[0] = pl_w->win_cnt ;
	if( pl_w->cpu_flag ) vs_sys_work.win_cnt[0] = 0 ;
}

/*==============================================================*/
static void pl_work_clear_2p( void )
{
	PLAYER_WORK *pl_w ;
	Uint8 i ;

	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = pl_work + i * 2 + 1 ;
		memset( pl_w, 0, 0x4bc ) ;
		pl_w->pl_no = 1 ;
		pl_w->work_no = i * 2 + 1 ;
		pl_w->team_no = i ;
		pl_w->team_no_org = i ;
	}
	pl_w = &pl_work[1] ;
	vs_sys_work.win_cnt[1] = pl_w->win_cnt ;
	if( pl_w->cpu_flag ) vs_sys_work.win_cnt[1] = 0 ;
}

/****************************************************************/
/*			Set up both teams for a game: the point				*/
/*			characters in, the partners out						*/
/****************************************************************/
void pl_init( void )
{
	PLAYER_WORK *pl_w ;
	Uint8 i, j ;

	for( i = 0 ; i < 2 ; i ++ ){
		for( j = 0 ; j < 3 ; j ++ ){
			pl_w = vs_sys_w->team_w[i][j] ;
			pl_w->be_flag = j ? 0 : 1 ;
#ifdef TARGET_DC	/* a home rule: one character a side */
			if( Sys->one_char && !j ){
				pl_w->one_char_flag = 1 ;
				vs_sys_w->unk_8a[i] = pl_w->work_no ;
			}
#endif
			pl_w->disp_flag = pl_w->be_flag ;
			pl_char_set( pl_w, Char_Stand, 0 ) ;
			pl_w->pl_tbl = ( void ( ** )( PLAYER_WORK *pl_w ) )&pl_tbl_buff[pl_w->work_no] ;
			pl_work_set( pl_w ) ;
			pl_w->target_w = vs_sys_w->team_w[i][0] ;
		}
	}
	pl_mark_init() ;
}

/****************************************************************/
/*			Every player back to its starting place and full	*/
/*			vitality											*/
/****************************************************************/
void pl_restart( void )
{
	Uint8 i ;

	for( i = 0 ; i < 6 ; i ++ ) pl_restart_set( &pl_work[i] ) ;
}

/*==============================================================*/
static void pl_restart_set( PLAYER_WORK *pl_w )
{
	pl_w->col_flag = -1 ;
	pl_w->col_no = col_no_tbl[pl_w->work_no] ;
	pl_w->col_cnt = col_cnt_tbl[pl_w->load_type] ;
	if( !vs_sys_work.round_cnt ){
		pl_w->pos.x = pl_start_pos_tbl[pl_w->pl_no].x ;
		pl_w->pos.y = pl_start_pos_tbl[pl_w->pl_no].y ;
		pl_w->pos.z = pl_start_pos_tbl[pl_w->pl_no].z ;
		pl_w->ground_y = pl_w->pos.y ;
	}
	pl_w->disp_zm_x = 1.6666666f ;
	pl_w->disp_zm_y = 2.142857f ;
	pl_w->alpha = 1.0f ;
	pl_w->vital_new = 144 ;
	pl_w->vital_red = 144 ;
	pl_w->ko_flag = 0 ;
	if( pl_w->team_no && ABYSS_CK( pl_w ) ){
		pl_w->vital_new = 0 ;
		pl_w->vital_red = 0 ;
	}
	vs_sys_w->atck_cnt[pl_w->pl_no] = 0 ;
	vs_sys_w->hit_cnt[pl_w->pl_no] = 0 ;
	pl_w->stun_cnt = 80 ;
	pl_w->renda_cnt = 0 ;
	pl_w->unk_212 = 0 ;
}

/****************************************************************/
/*			Abyss goes on to his next form (no 0 the second,	*/
/*			1 the third)										*/
/****************************************************************/
void pl_boss_change( int no )
{
	Uint8 i ;

	for( i = 0 ; i < 6 ; i ++ ) boss_change_set( &pl_work[i], no ) ;
	team_w_set() ;
}

/*==============================================================*/
static void boss_change_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *nw ;
	int change ;
	Uint8 t ;

	pl_w->col_flag = -1 ;
	if( pl_w->be_flag && FLAG_CK( pl_w->pl_bit, PLB0( PL_ABYSS1 ) | PLB0( PL_ABYSS2 ), 0 ) ){
		change = 0 ;
		if( !no ){
			if( pl_w->pl_type == PL_ABYSS1 ){
				change = 1 ;
				nw = vs_sys_work.team_w[pl_w->pl_no][( pl_w->team_no + 4 ) % 3] ;
				if( nw->pl_type != PL_ABYSS2 ){
					nw = vs_sys_work.team_w[pl_w->pl_no][( pl_w->team_no + 2 ) % 3] ;
					if( nw->pl_type != PL_ABYSS2 ) nw->load_type = 25 ;
				}
			}
		} else if( pl_w->pl_type == PL_ABYSS2 ){
			change = 1 ;
			nw = vs_sys_work.team_w[pl_w->pl_no][( pl_w->team_no + 4 ) % 3] ;
			if( nw->pl_type != PL_ABYSS3 ){
				nw = vs_sys_work.team_w[pl_w->pl_no][( pl_w->team_no + 2 ) % 3] ;
				if( nw->pl_type != PL_ABYSS3 ) nw->load_type = 26 ;
			}
		}
		if( change ){
			pl_w->disp_flag = 0 ;
			pl_w->be_flag = 0 ;
			pl_w->vital_new = 0 ;
			pl_w->vital_red = 0 ;
			nw->be_flag = 1 ;
			nw->disp_flag = 0 ;
			nw->vital_new = 144 ;
			nw->vital_red = 144 ;
			nw->pos.x = pl_w->pos.x ;
			t = pl_w->team_no ;
			pl_w->team_no = nw->team_no ;
			nw->team_no = t ;
			nw->routine_0 = nw->routine_1 = nw->routine_2 = nw->routine_3 = 0 ;
		}
	}
	vs_sys_w->atck_cnt[pl_w->pl_no] = 0 ;
	vs_sys_w->hit_cnt[pl_w->pl_no] = 0 ;
	pl_w->stun_cnt = 80 ;
	pl_w->renda_cnt = 0 ;
	pl_w->unk_212 = 0 ;
}

/*==============================================================*/
static void pl_work_set( PLAYER_WORK *pl_w )
{
	Sint32 *lw ;

	pl_w->routine_0 = pl_w->routine_1 = pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->timer0 = pl_w->timer1 = 0 ;
	pl_w->dir = pl_w->type = pl_w->sub_type = 0 ;
	pl_w->rot[0] = pl_w->rot[1] = pl_w->rot[2] = 0 ;
	pl_w->spd.x = pl_w->spd.y = pl_w->spd.z = 0.0f ;
	pl_w->acc.x = pl_w->acc.y = pl_w->acc.z = 0.0f ;
	lw = &pl_w->obj_flag ;
	lw[0] = lw[1] = lw[2] = lw[3] = 0 ;
	pl_w->prio = pl_w->work_no + 1 ;
	pl_w->col_flag = -1 ;
	pl_w->pos.x = pl_start_pos_tbl[pl_w->pl_no].x ;
	pl_w->pos.y = pl_start_pos_tbl[pl_w->pl_no].y ;
	pl_w->pos.z = pl_start_pos_tbl[pl_w->pl_no].z ;
	pl_w->ground_y = pl_w->pos.y ;
	pl_w->disp_zm_x = 1.6666666f ;
	pl_w->disp_zm_y = 2.142857f ;
	pl_w->alpha = 1.0f ;
	pl_w->disp_dir = pl_w->pl_dir = pl_w->pl_no ^ 1 ;
	pl_w->gauge = &vs_sys_w->gauge[pl_w->pl_no] ;
	pl_w->gauge_level = &vs_sys_w->gauge_level[pl_w->pl_no] ;
	if( !pl_w->team_no ){
		*pl_w->gauge = 0 ;
		*pl_w->gauge_level = 1 ;
	}
	pl_w->gauge_no = 0 ;
	em_round_init( pl_w ) ;
	pl_work_init( pl_w ) ;
}

/****************************************************************/
/*			Set a player up as its character, for a round		*/
/****************************************************************/
void pl_work_init( PLAYER_WORK *pl_w )
{
#ifdef TARGET_DC	/* the characters' modules load at six places, one for each player */
	static void ( *tbl_set_jmp[6] )( PLAYER_WORK *pl_w ) = {
		( void ( * )( PLAYER_WORK *pl_w ) )0x0ce30000, ( void ( * )( PLAYER_WORK *pl_w ) )0x0ce38000,
		( void ( * )( PLAYER_WORK *pl_w ) )0x0ce40000, ( void ( * )( PLAYER_WORK *pl_w ) )0x0ce48000,
		( void ( * )( PLAYER_WORK *pl_w ) )0x0ce50000, ( void ( * )( PLAYER_WORK *pl_w ) )0x0ce58000,
	} ;
	void ( *tbl_set )( PLAYER_WORK *pl_w ) ;
#else
	static void ( *tbl_set_jmp[59] )( PLAYER_WORK *pl_w ) = {
		pl00_tbl_set, pl01_tbl_set, pl02_tbl_set, pl03_tbl_set, pl04_tbl_set, pl05_tbl_set,
		pl06_tbl_set, pl07_tbl_set, pl08_tbl_set, pl09_tbl_set, pl0a_tbl_set, pl0b_tbl_set,
		pl0c_tbl_set, pl0d_tbl_set, pl0e_tbl_set, pl0f_tbl_set, pl10_tbl_set, pl11_tbl_set,
		pl12_tbl_set, pl13_tbl_set, pl14_tbl_set, pl15_tbl_set, pl16_tbl_set, pl17_tbl_set,
		pl18_tbl_set, pl19_tbl_set, pl1a_tbl_set, pl1b_tbl_set, pl1c_tbl_set, pl1d_tbl_set,
		pl1e_tbl_set, pl1f_tbl_set, pl20_tbl_set, pl21_tbl_set, pl22_tbl_set, pl23_tbl_set,
		pl24_tbl_set, pl25_tbl_set, pl26_tbl_set, pl27_tbl_set, pl28_tbl_set, pl29_tbl_set,
		pl2a_tbl_set, pl2b_tbl_set, pl2c_tbl_set, pl2d_tbl_set, pl2e_tbl_set, pl2f_tbl_set,
		pl30_tbl_set, pl31_tbl_set, pl32_tbl_set, pl33_tbl_set, pl34_tbl_set, pl35_tbl_set,
		pl36_tbl_set, pl37_tbl_set, pl38_tbl_set, pl39_tbl_set, pl3a_tbl_set
	} ;
#endif
	Uint8 n ;

	memset( &pl_w->act_st, 0, 0x238 ) ;
	pl_w->atck_no = 0 ;
	pl_w->hit_flag = 0 ;
	pl_w->hit_stop = 0 ;
	pl_w->atck_attr = 0 ;
	pl_w->dm_attr = 0 ;
	pl_w->atck_flag = -128 ;
	pl_w->dm_flag = -128 ;
	pl_w->catch_ng_timer = 0 ;
	pl_w->pl_dir = pl_w->disp_dir ;
	pl_w->target_w = vs_sys_w->team_w[pl_w->pl_no ^ 1][0] ;
	pl_w->pl_type = pl_w->load_type ;
	pl_w->pl_color = pl_w->load_color ;
#ifdef TARGET_DC
	tbl_set = tbl_set_jmp[pl_w->work_no] ;
	tbl_set( pl_w ) ;
#else
	tbl_set_jmp[pl_w->pl_type]( pl_w ) ;
#endif
	pl_shadow_set( pl_w ) ;
	if( pl_w->team_no ){ }
	pl_w->weight = weight_tbl[pl_w->pl_type] ;
	armor_init( pl_w ) ;
	pl_w->size_top = pl_size_tbl[pl_w->pl_type][0] ;
	pl_w->size_bottom = pl_size_tbl[pl_w->pl_type][1] ;
	pl_w->size_front = pl_size_tbl[pl_w->pl_type][2] ;
	pl_w->size_back = pl_size_tbl[pl_w->pl_type][3] ;
	pl_w->prio_ofs = prio_ofs_tbl[pl_w->pl_type] * 2 ;
	pl_w->stun_cnt = 80 ;
	n = ( pl_w->pl_type >= PL_FELICIA ) ? 1 : 0 ;
	pl_w->pl_bit[n ^ 1] = 0 ;
	pl_w->pl_bit[n] = 1 << ( pl_w->pl_type - n * 32 ) ;
	pl_w->scale.x = 1.0f ;
	pl_w->scale.y = 1.0f ;
	pl_w->scale.z = 1.0f ;
	pl_w->armor_flag &= ~ARMOR_HEAVY ;
	if( pl_w->pl_type >= PL_ABYSS1 && pl_w->pl_type <= PL_ABYSS3 ){
		pl_w->scale.x /= 1.6666666f ;
		pl_w->scale.y /= 2.142857f ;
		if( pl_w->pl_type == PL_ABYSS3 ){
			pl_w->scale.x *= 4.0f ;
			pl_w->scale.y *= 4.0f ;
		} else {
			pl_w->scale.x *= 2.0f ;
			pl_w->scale.y *= 2.0f ;
		}
	}
	pl_w->alpha = 1.0f ;
	pl_w->pl_scale = pl_w->scale ;
	pl_w->fade.trnsl = pl_w->fade.r = pl_w->fade.g = pl_w->fade.b = 1.0f ;
	if( FLOAT_CK( pl_w ) ){
		pl_w->float_flag = 1 ;
		pl_w->float_y = pl_w->ground_y ;
	} else pl_w->float_flag = 0 ;
	tlocal_clear( pl_w ) ;
	pl_char_init( pl_w ) ;
}

/****************************************************************/
/*			A partner comes in (nw) for the player going out	*/
/****************************************************************/
void pl_change_set( PLAYER_WORK *pl_w, PLAYER_WORK *nw )
{
	nw->vital_red_cnt = 300 ;
	combo_reset( nw ) ;
	pl_voice_req( nw, 43 ) ;
	pl_tlocal_init( nw ) ;
	memset( &nw->pl_new_lvbt, 0, 0xc8 ) ;
	pl_sw_set( pl_w ) ;
	pl_w->vital_red = pl_w->vital_new ;
	pl_w->hit_flag = -1 ;
	pl_w->vital_red_wait = 0 ;
#ifdef TARGET_DC	/* the wait of the home rule that drains the vital */
	pl_w->drain_timer = 100 ;
#endif
	em_round_init( pl_w ) ;
}

/*==============================================================*/
static void tlocal_clear( PLAYER_WORK *pl_w )
{
	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
}

/****************************************************************/
/*			The character's own work back to its start			*/
/****************************************************************/
void pl_tlocal_init( PLAYER_WORK *pl_w )
{
	if( !pl_w->pl_tbl[TBL_TLOCAL_INIT] ){
		memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	} else {
init :
		pl_w->pl_tbl[TBL_TLOCAL_INIT]( pl_w ) ;
	}
}

/****************************************************************/
/*			A player leaves the fight, off the edge of the		*/
/*			stage												*/
/****************************************************************/
void pl_change_out( PLAYER_WORK *pl_w )
{
	pl_w->be_flag = 0 ;
	pl_w->disp_flag = 0 ;
	pl_w->routine_0 = PL_FIGHT ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->fly_screen_flag = 0 ;
	combo_reset( pl_w ) ;
	pl_w->act_st = pl_w->old_act_st = N_STAND ;
	pl_w->sp_ng_timer = 0 ;
	pl_w->atck_ng_timer = 0 ;
	pl_w->cg_atck_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	memset( &pl_w->com_w, 0, sizeof( COM_WORK ) * 13 ) ;
	pl_w->pl_sw_old = 0 ;
	pl_w->pl_sw_now = 0 ;
	pl_w->pl_sw_new = 0 ;
	pl_w->pl_last_lvbt = 0 ;
	pl_w->pl_now_lvbt = 0 ;
	pl_w->pl_new_lvbt = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->pos.x = scr_work.stage_right + DOT_X( 64.0f ) ;
	if( pl_w->disp_dir ) pl_w->pos.x = scr_work.stage_left - DOT_X( 64.0f ) ;
	pl_w->hyper_bg_tm = pl_w->hyper_bg = 0 ;
	pl_w->speed_up_timer = pl_w->speed_up_flag = 0 ;
	pl_w->atck_plus_timer = pl_w->atck_plus = 0 ;
	pl_w->def_plus_timer = pl_w->def_plus = 0 ;
	pl_w->delay_timer = pl_w->delay_flag = 0 ;
	pl_w->stop_move_flag = 0 ;
	pl_w->stop_move_timer = 0 ;
}

/****************************************************************/
/*			The characters that start the round with effects	*/
/*			or work of their own								*/
/****************************************************************/
void pl_char_init( PLAYER_WORK *pl_w )
{
	switch( pl_w->pl_type ){
	case PL_RYU :
		pls00_00_set( pl_w ) ;
		break ;
	case PL_MORRIGAN :
		pls03_02_set( pl_w ) ;
		break ;
	case PL_ANAKARIS :
		pls04_00_set( pl_w, 3, 0 ) ;
		pls04_00_set( pl_w, 3, 1 ) ;
		shl04_02_set( pl_w, 0 ) ;
		break ;
	case PL_CYCLOPS :
		pls06_00_set( pl_w, 0 ) ;
		break ;
	case PL_ROGUE :
		pls0a_01_set( pl_w ) ;
		break ;
	case PL_CAPTAIN_AMERICA :
		shl0b_00_set( pl_w ) ;
		break ;
	case PL_SPIDERMAN :
		pls0c_01_set( pl_w ) ;
		break ;
	case PL_TRON :
		pls10_03_set( pl_w, 0 ) ;
		break ;
	case PL_SONSON :
		pls14_00_set( pl_w, 0 ) ;
		break ;
	case PL_AMINGO :
		pls15_03_set( pl_w ) ;
		break ;
	case PL_ABYSS1 :
		pls18_02_set( pl_w ) ;
		break ;
	case PL_ABYSS2 :
		pls19_02_set( pl_w ) ;
		break ;
	case PL_MEGAMAN :
		pls1c_02_set( pl_w ) ;
		break ;
	case PL_AKUMA :
		pls1e_00_set( pl_w ) ;
		break ;
	case PL_BB_HOOD :
		pls1f_00_set( pl_w, 0, 0 ) ;
		pls1f_01_set( pl_w ) ;
		pls1f_02_set( pl_w ) ;
		break ;
	case PL_KEN :
		pls27_00_set( pl_w ) ;
		break ;
	case PL_STORM :
		pls2a_02_set( pl_w ) ;
		pls2a_03_set( pl_w ) ;
		break ;
	case PL_MAGNETO :
		pls2c_00_set( pl_w, 0 ) ;
		break ;
	case PL_CAPTAIN_COMMANDO :
		pls38_01_set( pl_w ) ;
		break ;
	}
}

/****************************************************************/
/*			Each player on one side against each on the			*/
/*			other: bodies that overlap push apart				*/
/****************************************************************/
void pl_push_ck( void )
{
	PLAYER_WORK *pl_w ;
	Uint8 i ;

	if( vs_sys_work.game_stop ) return ;
	vs_sys_work.push_bit = 0 ;
	pl_push( &pl_work[0], &pl_work[1] ) ;
	pl_push( &pl_work[2], &pl_work[1] ) ;
	pl_push( &pl_work[4], &pl_work[1] ) ;
	pl_push( &pl_work[0], &pl_work[3] ) ;
	pl_push( &pl_work[2], &pl_work[3] ) ;
	pl_push( &pl_work[4], &pl_work[3] ) ;
	pl_push( &pl_work[0], &pl_work[5] ) ;
	pl_push( &pl_work[2], &pl_work[5] ) ;
	pl_push( &pl_work[4], &pl_work[5] ) ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		if( pl_w->push_ng_timer && !pl_w->hit_stop ) pl_w->push_ng_timer -- ;
	}
}

/*==============================================================*/
static void pl_push( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	HIT_BOX *pb, *eb ;
	Float x[4] ;
	Float ov ;

	if( !( pl_w->be_flag & em->be_flag ) ) return ;
	if( pl_w->routine_1 == PL_CATCH ) return ;
	if( em->routine_1 == PL_CATCH ) return ;
	if( pl_w->hit_stop && em->hit_stop ) return ;
	if( pl_w->push_ng_timer ) return ;
	if( em->push_ng_timer ) return ;
	pb = pl_w->hit_box_tbl + pl_w->pl_stance + PUSH_BOX ;
	eb = em->hit_box_tbl + em->pl_stance + PUSH_BOX ;
	x[0] = pb->x * pl_w->scale.x * 1.6666666f ;
	if( pl_w->pl_dir ) x[0] = -x[0] ;
	x[0] += pl_w->pos.x ;
	x[1] = eb->x * em->scale.x * 1.6666666f ;
	if( em->pl_dir ) x[1] = -x[1] ;
	x[1] += em->pos.x ;
	if( ( x[2] = x[0] - x[1] ) < 0.0f ) x[2] = -x[2] ;
	x[3] = DOT_X( pb->w ) + DOT_X( eb->w ) ;
	if( ( ov = x[3] - x[2] ) < 0.0f ) return ;
	x[2] = pb->y * pl_w->scale.y * 2.142857f + pl_w->pos.y ;
	x[2] -= ( DOT_Y( eb->y ) + em->pos.y ) * em->scale.y ;
	if( x[2] < 0.0f ) x[2] = -x[2] ;
	x[3] = pb->h * pl_w->scale.y * 2.142857f + eb->h * em->scale.y * 2.142857f ;
	if( x[3] < x[2] ) return ;
	if( pl_w->pl_stance == STANCE_AIR && em->pl_stance == STANCE_AIR ){
		push_both( pl_w, em, &x[0], ov ) ;
	} else if( ( pl_w->wall_flag & 1 << pl_w->pl_dir ) && !em->wall_wide_timer ){
		push_one( pl_w, em, &x[0], ov ) ;
	} else if( em->wall_flag & 1 << em->pl_dir ){
		x[2] = x[0] ;
		push_one( em, pl_w, &x[1], ov ) ;
	} else if( pl_w->pl_stance == STANCE_AIR ){
		x[2] = x[0] ;
		push_one( em, pl_w, &x[1], ov ) ;
	} else if( em->pl_stance == STANCE_AIR ){
		push_one( pl_w, em, &x[0], ov ) ;
	} else if( ABYSS_CK( pl_w ) && !ABYSS_CK( em ) ){
		push_one( pl_w, em, &x[0], ov ) ;
	} else if( !ABYSS_CK( pl_w ) && ABYSS_CK( em ) ){
		x[2] = x[0] ;
		push_one( em, pl_w, &x[1], ov ) ;
	} else {
		push_both( pl_w, em, &x[0], ov ) ;
	}
}

/*==============================================================*/
/*			Both move apart by half the overlap					*/
/*==============================================================*/
static void push_both( PLAYER_WORK *pl_w, PLAYER_WORK *em, Float *x, Float ov )
{
	if( ov > DOT_X( 32.0f ) ) ov = DOT_X( 32.0f ) ;
	ov /= 2.0f ;
	if( x[0] < x[1] ) ov = -ov ;
	pl_w->pos.x += ov ;
	em->pos.x -= ov ;
	vs_sys_work.push_bit |= ( 1 << pl_w->work_no ) | ( 1 << em->work_no ) ;
}

/*==============================================================*/
/*			Em alone is pushed away from pl_w					*/
/*==============================================================*/
static void push_one( PLAYER_WORK *pl_w, PLAYER_WORK *em, Float *x, Float ov )
{
	if( ov > DOT_X( 16.0f ) ) ov = DOT_X( 16.0f ) ;
	if( pl_w->wall_flag & 1 << pl_w->pl_dir ){
		if( pl_w->wall_flag & WALL_LEFT ) ov = -ov ;
	} else if( x[0] < x[1] ) ov = -ov ;
	em->pos.x -= ov ;
	vs_sys_work.push_bit |= ( 1 << pl_w->work_no ) | ( 1 << em->work_no ) ;
}

/* the draw priority of the players (pl_l) and of their effects (ef_l), from front to back */
/****************************************************************/
/*			The sort reads the priorities unsigned				*/
/****************************************************************/
void pl_prio_set( PLAYER_WORK *pl_l, PLAYER_WORK *ef_l )
{
	PLAYER_WORK *w[6] ;
	PLAYER_WORK *ew, *em, *t ;
	Sint8 old[8] ;
	Sint8 d = 0 ;
	int n, i, j ;

	n = 0 ;
	for( ew = pl_l ; ew ; ew = ew->behind_w ){
		w[n] = ew ;
		old[ew->work_no] = ew->prio ;
		n ++ ;
	}
	for( ew = ef_l ; ew ; ew = ew->behind_w ){
		w[n] = ew ;
		old[ew->work_no] = ew->prio ;
		n ++ ;
	}
	if( !n ) return ;
	for( i = 0 ; i < n ; i ++ ){
		ew = w[i] ;
		d = 0 ;
		switch( ew->act_st ){
		case N_NML_ATCK :
			d = 16 ;
			break ;
		case N_SPECIAL :
		case N_SUPER :
			d = 32 ;
			break ;
		case N_DAMAGE :
		case N_GUARD :
		case N_DOWN :
		case N_AIR_GUARD :
			d = -32 ;
			break ;
		default :
			if( ew->routine_1 != 2 && ew->pl_stance == STANCE_AIR ) d = 8 ;
			else if( ew->routine_1 == 1 ) d = -8 ;
			else if( ew->routine_1 == 2 ) d = -16 ;
			break ;
		}
		ew->prio = d + ew->cg_prio - ew->pl_no + ew->team_no * 5 - ew->prio_ofs ;
	}
	for( i = 0 ; i < n - 1 ; i ++ ){
		for( j = 0 ; j < n - i - 1 ; j ++ ){
			if( ( Uint8 )w[j]->prio > ( Uint8 )w[j + 1]->prio || ( Uint8 )w[j]->prio == ( Uint8 )w[j + 1]->prio && w[j]->work_no < w[j + 1]->work_no ){
				t = w[j] ;
				w[j] = w[j + 1] ;
				w[j + 1] = t ;
			}
		}
	}
	for( i = 0 ; i < n ; i ++ ) w[i]->prio = 6 - i ;
	for( ew = ef_l ; ew ; ew = ew->behind_w ){
		em = ew->catch_w ;
		if( old[ew->work_no] < old[em->work_no] ){
			if( ew->prio > em->prio ){
				d = em->prio ;
				em->prio = ew->prio ;
				ew->prio = d ;
			}
		} else if( ew->prio < em->prio ){
			d = em->prio ;
			em->prio = ew->prio ;
			ew->prio = d ;
		}
	}
}

/****************************************************************/
/*			Read the stick and buttons for this frame, back		*/
/*			and forward by the facing							*/
/****************************************************************/
void pl_sw_set( PLAYER_WORK *pl_w )
{
	pl_w->pl_old2_lvbt = pl_w->pl_old_lvbt ;
	pl_w->pl_old_lvbt = pl_w->pl_last_lvbt ;
	pl_w->pl_last_lvbt = pl_w->pl_sw_old = pl_w->pl_new_lvbt ;
	pl_w->pl_new_lvbt = pl_w->pl_sw_new = pl_w->pad_sw.now & ( KEY_START | KEY_LEVER | KEY_BUTTON ) ;
	pl_w->pl_now_lvbt = pl_w->pl_sw_now = pl_w->pl_new_lvbt & ~pl_w->pl_last_lvbt ;
	if( pl_w->pl_dir ){
		if( pl_w->pl_sw_new & ( KEY_LEFT | KEY_RIGHT ) ) pl_w->pl_sw_new = pl_w->pl_sw_new ^ ( KEY_LEFT | KEY_RIGHT ) ;
		if( pl_w->pl_sw_now & ( KEY_LEFT | KEY_RIGHT ) ) pl_w->pl_sw_now = pl_w->pl_sw_now ^ ( KEY_LEFT | KEY_RIGHT ) ;
		if( pl_w->pl_sw_old & ( KEY_LEFT | KEY_RIGHT ) ) pl_w->pl_sw_old = pl_w->pl_sw_old ^ ( KEY_LEFT | KEY_RIGHT ) ;
	}
}

/****************************************************************/
/*			A jump's first speeds								*/
/****************************************************************/
void pl_jump_spd_set( PLAYER_WORK *pl_w )
{
	Sint16 *tbl ;
	Sint16 *p ;
	Float x, ax ;

	if( !pl_w->sjump_flag ) tbl = jump_spd_tbl ;
	else tbl = sjump_spd_tbl ;
	p = &tbl[pl_w->pl_type * 12 + ( pl_w->jump_dir + 1 ) * 4] ;
	pl_w->spd.y = p[0] * 2.142857f / 256.0f ;
	pl_w->acc.y = p[1] * 2.142857f / 256.0f ;
	x = p[2] * 1.6666666f / 256.0f ;
	ax = p[3] * 1.6666666f / 256.0f ;
	if( pl_w->pl_dir ){
		x = -x ;
		ax = -ax ;
	}
	if( pl_w->speed_up_flag && !pl_w->sjump_flag ){
		pl_w->spd.y *= 1.25f ;
		pl_w->acc.y *= 1.5f ;
		x *= 1.1f ;
		ax *= 1.1f ;
	}
	if( pl_w->old_act_st == N_DASH && !pl_w->jump_dir ) pl_w->spd.x += x ;
	else pl_w->spd.x = x ;
	pl_w->acc.x = ax ;
}

/****************************************************************/
/*			The walking speed									*/
/****************************************************************/
void pl_walk_spd_set( PLAYER_WORK *pl_w )
{
	Sint16 *tbl ;
	register Sint16 *p ;

	tbl = walk_spd_tbl[0] ;
	if( pl_w->armor_flag & ARMOR_HEAVY ) tbl = walk_spd_armor_tbl[0] ;
	p = &tbl[pl_w->pl_type * 2 + ( ( pl_w->pl_sw_new & BACK ) >> 10 )] ;
	if( pl_w->pl_dir ) pl_w->spd.x = *p * 1.6666666f / 256.0f ;
	else pl_w->spd.x = -( *p * 1.6666666f / 256.0f ) ;
	if( pl_w->speed_up_flag ) pl_w->spd.x *= 1.25f ;
	if( ( pl_w->armor_flag & ARMOR_HEAVY ) || pl_w->pl_type == PL_SENTINEL || pl_w->pl_type == PL_ABYSS1 ){
		if( pl_w->cg_flag1 ){
			pl_w->cg_flag1 = 0 ;
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
		}
		if( pl_w->cg_type ) pl_w->spd.x = 0.0f ;
	}
}

/****************************************************************/
/*			A short step forward or back						*/
/****************************************************************/
void pl_step_spd_set( PLAYER_WORK *pl_w )
{
	Float spd ;

	if( ( pl_w->pl_sw_new & BACK ) >> 10 ) spd = -2.5f ;
	else spd = DOT_X( 1.75f ) ;
	if( !pl_w->pl_dir ) spd = -spd ;
	pl_w->spd.x = spd ;
}

/****************************************************************/
/*			Move through the air, keeping the sideways speed	*/
/*			inside the character's limits						*/
/****************************************************************/
void pl_jump_move( PLAYER_WORK *pl_w )
{
	Sint16 *tbl ;
	Float lo, hi, spd, t ;

	if( pl_w->jump_move_flag ) return ;
	pl_w->jump_move_flag = 1 ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( !pl_w->sjump_flag ){
		if( !pl_w->speed_up_flag ) tbl = jump_limit_tbl ;
		else tbl = jump_limit_up_tbl ;
	} else {
		if( !pl_w->speed_up_flag ) tbl = sjump_limit_tbl ;
		else tbl = sjump_limit_up_tbl ;
	}
	tbl = &tbl[pl_w->pl_type * 2] ;
	lo = tbl[0] * 1.6666666f / 256.0f ;
	hi = tbl[1] * 1.6666666f / 256.0f ;
	spd = pl_w->spd.x ;
	if( pl_w->pl_dir ){
		t = lo ;
		lo = -hi ;
		hi = -t ;
	}
	if( lo > spd ) spd = lo ;
	if( spd > hi ) spd = hi ;
	pl_w->spd.x = spd ;
	pl_w->pos.x += spd ;
}

/****************************************************************/
/*			Steer a super jump with the stick					*/
/****************************************************************/
void pl_sjump_ctrl( PLAYER_WORK *pl_w )
{
	Float d ;

	if( !pl_w->sjump_flag ) return ;
	if( pl_w->sjump_cnt < 7 ){
		pl_w->sjump_cnt ++ ;
		return ;
	}
	if( !( pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ) ) return ;
	d = sjump_ctrl_tbl[pl_w->pl_type][pl_w->jump_dir + 1][( pl_w->pl_sw_new & BACK ) >> 10] * 1.6666666f / 256.0f ;
	if( pl_w->pl_dir ) pl_w->spd.x += -d ;
	else pl_w->spd.x += d ;
}

/****************************************************************/
/*			A fall no faster than the limit						*/
/****************************************************************/
void pl_fall_limit( PLAYER_WORK *pl_w )
{
	if( pl_w->spd.y >= 0.0f ) return ;
	if( pl_w->spd.y < -DOT_Y( 10.0f ) ) pl_w->spd.y = -DOT_Y( 10.0f ) ;
}

/****************************************************************/
/*			Past the top of a super jump, fall at the			*/
/*			character's own rate								*/
/****************************************************************/
void pl_sjump_top_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->sjump_flag == 1 && !( pl_w->spd.y > 0.0f ) ){
		pl_w->sjump_flag = 2 ;
		pl_w->acc.y = sjump_fall_tbl[pl_w->pl_type] * 2.142857f / 256.0f ;
	}
}

/****************************************************************/
/*			A hit in a jump slows its rise, once				*/
/****************************************************************/
void pl_jump_hit_ck( PLAYER_WORK *pl_w )
{
	if( pl_w->hit_flag && !pl_w->jump_hit_flag ){
		pl_w->jump_hit_flag ++ ;
		pl_w->spd.y *= 0.75f ;
	}
}

/****************************************************************/
/*			Keep a player inside the screen's edges, and a		*/
/*			flyer near the ground and the other team			*/
/****************************************************************/
void pl_wall_ck( PLAYER_WORK *pl_w )
{
	SCR_WORK *sw ;
	Float d, x ;

	sw = &scr_work ;
	if( pl_w->wall_ng_timer ) return ;
	if( pl_w->wall_wide_timer ) return ;
	if( pl_w->fly_flag && pl_w->routine_1 != PL_CAUGHT ){
		d = pl_w->target_w->pos.y - pl_w->pos.y + DOT_Y( 128.0f ) ;
		if( d < 0.0f ){
			pl_w->pos.y += d / 8.0f ;
		} else {
			d = pl_w->pos.y - pl_w->ground_y - DOT_Y( 16.0f ) ;
			if( d < 0.0f ) pl_w->pos.y -= d / 2.0f ;
		}
		x = scr_work.stage_top - DOT_Y( 96.0f ) ;
		if( pl_w->pos.y > x ) pl_w->pos.y = x ;
	}
	pl_w->wall_flag = 0 ;
	x = sw->eye[0].x - 285.0f ;
	if( ( Sint32 )pl_w->pos.x <= ( Sint32 )x ){
		pl_w->pos.x = x ;
		pl_w->wall_flag = WALL_LEFT ;
	} else {
		x = sw->eye[0].x + 285.0f ;
		if( ( Sint32 )pl_w->pos.x >= ( Sint32 )x ){
			pl_w->pos.x = x ;
			pl_w->wall_flag = WALL_RIGHT ;
		}
	}
}

/****************************************************************/
/*			The same for a player kept on screen by wall_wide_timer,	*/
/*			with a little more room								*/
/****************************************************************/
void pl_wall_wide_ck( PLAYER_WORK *pl_w )
{
	SCR_WORK *sw ;
	Float d, x ;

	sw = &scr_work ;
	if( !pl_w->wall_wide_timer ) return ;
	if( pl_w->fly_flag && pl_w->routine_1 != PL_CAUGHT ){
		d = pl_w->target_w->pos.y - pl_w->pos.y + DOT_Y( 128.0f ) ;
		if( d < 0.0f ){
			pl_w->pos.y += d / 8.0f ;
		} else {
			d = pl_w->pos.y - pl_w->ground_y - DOT_Y( 16.0f ) ;
			if( d < 0.0f ) pl_w->pos.y -= d / 2.0f ;
		}
		x = scr_work.stage_top - DOT_Y( 96.0f ) ;
		if( pl_w->pos.y > x ) pl_w->pos.y = x ;
	}
	pl_w->wall_flag = 0 ;
	x = sw->eye[0].x - 293.33334f ;
	if( ( Sint32 )pl_w->pos.x <= ( Sint32 )x ){
		pl_w->pos.x = x ;
		pl_w->wall_flag = WALL_LEFT ;
	} else {
		x = sw->eye[0].x + 293.33334f ;
		if( ( Sint32 )pl_w->pos.x >= ( Sint32 )x ){
			pl_w->pos.x = x ;
			pl_w->wall_flag = WALL_RIGHT ;
		}
	}
}

/****************************************************************/
/*			A player left off screen is held at the stage's		*/
/*			ends, or brought back standing						*/
/****************************************************************/
void pl_stage_end_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	SCR_WORK *sw ;
	Float x ;

	if( !pl_w->vital_new ) return ;
	if( pl_w->act_st == N_CHANGE ) return ;
	if( pl_w->act_st == N_DOWN ) return ;
	sw = &scr_work ;
	em = pl_w->target_w ;
	x = sw->eye[0].x - 285.0f ;
	if( ( Sint32 )pl_w->pos.x < ( Sint32 )x ){
		if( pl_w->pos.x < sw->stage_left + 35.0f ){
			pl_w->pos.x = sw->stage_left + 35.0f ;
			return ;
		}
		lever_clear( pl_w ) ;
		stage_end_l( pl_w, em, sw ) ;
	} else {
		x = sw->eye[0].x + 285.0f ;
		if( ( Sint32 )pl_w->pos.x > ( Sint32 )x ){
			if( pl_w->pos.x > sw->stage_right - 35.0f ){
				pl_w->pos.x = sw->stage_right - 35.0f ;
				return ;
			}
			lever_clear( pl_w ) ;
			stage_end_r( pl_w, em, sw ) ;
		} else if( em->routine_1 == PL_NORM ) em->fly_screen_flag = 0 ;
	}
}

/*==============================================================*/
static void stage_end_r( PLAYER_WORK *pl_w, PLAYER_WORK *em, SCR_WORK *sw )
{
	Float x ;

	x = sw->eye[0].x + 445.0f ;
	if( pl_w->pl_type == PL_JUGGERNAUT ) x += DOT_X( 32.0f ) ;
	if( pl_w->pos.x >= x ){
		if( !em->routine_1 && pl_w->act_st == N_DASH ) return ;
		pl_w->pos.x = x ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->fly_flag = 0 ;
		pl_stand_ret( pl_w ) ;
		pl_color_set( pl_w, 1 ) ;
		if( em->routine_1 ) return ;
	} else {
		if( em->routine_1 ) return ;
		if( pl_w->act_st == N_DASH ) return ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->fly_flag = 0 ;
		pl_stand_ret( pl_w ) ;
		pl_color_set( pl_w, 1 ) ;
	}
	pl_act_set( pl_w, N_DASH ) ;
}

/*==============================================================*/
static void stage_end_l( PLAYER_WORK *pl_w, PLAYER_WORK *em, SCR_WORK *sw )
{
	Float x ;

	x = sw->eye[0].x - 445.0f ;
	if( pl_w->pl_type == PL_JUGGERNAUT ) x -= DOT_X( 32.0f ) ;
	if( pl_w->pos.x <= x ){
		if( !em->routine_1 && pl_w->act_st == N_DASH ) return ;
		pl_w->pos.x = x ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->fly_flag = 0 ;
		pl_stand_ret( pl_w ) ;
		pl_color_set( pl_w, 1 ) ;
		if( em->routine_1 ) return ;
	} else {
		if( em->routine_1 ) return ;
		if( pl_w->act_st == N_DASH ) return ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->fly_flag = 0 ;
		pl_stand_ret( pl_w ) ;
		pl_color_set( pl_w, 1 ) ;
	}
	pl_act_set( pl_w, N_DASH ) ;
}

/*==============================================================*/
/*			Drop the stick from the input, leaving the			*/
/*			buttons												*/
/*==============================================================*/
static void lever_clear( PLAYER_WORK *pl_w )
{
	pl_w->atck_ng_timer = 2 ;
	pl_w->pl_new_lvbt &= KEY_BUTTON ;
	pl_w->pl_now_lvbt &= KEY_BUTTON ;
	pl_w->pl_last_lvbt &= KEY_BUTTON ;
	pl_w->pl_sw_new &= KEY_BUTTON ;
	pl_w->pl_sw_now &= KEY_BUTTON ;
	pl_w->pl_sw_old &= KEY_BUTTON ;
}

/****************************************************************/
/*			A thrown player's mash count to break free			*/
/****************************************************************/
void hold_renda_set( PLAYER_WORK *pl_w, Sint16 no )
{
	static Sint8 renda_rnd[32] = {
		3, -2, -1, 0, -3, -2, -1, -1, -1, 4, 3, -2, 4, 2, -4, 1,
		-2, 1, -3, 1, -4, 0, 2, 0, 1, 3, 0, -2, 1, 2, 0, -3
	} ;

	if( ( pl_w->renda_timer = no + renda_rnd[Rnd() & 31] ) <= 0 ) pl_w->renda_timer = 1 ;
}

/****************************************************************/
/*			A held player mashing, its animation sped up by		*/
/*			each press: 1 once it breaks free					*/
/****************************************************************/
int hold_renda_move_ck( PLAYER_WORK *pl_w )
{
	Sint16 n ;

	if( vs_sys_work.vs_st >= VS_END_WAIT ) return 1 ;
	if( !pl_w->vital_new ) return 1 ;
	if( !( n = renda_get( pl_w ) ) ) return 0 ;
	pl_w->cg_ctr = 1 ;
	pl_char_move( pl_w ) ;
	if( ( pl_w->renda_timer -= n ) > 0 ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			A held player mashing: 1 once it breaks free		*/
/****************************************************************/
int hold_renda_ck( PLAYER_WORK *pl_w )
{
	int n ;

	if( vs_sys_work.vs_st >= VS_END_WAIT ) return 1 ;
	if( !pl_w->vital_new ) return 1 ;
	n = renda_get( pl_w ) ;
	if( ( pl_w->renda_timer -= n ) > 0 ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			The thrower's mash count to hold on					*/
/****************************************************************/
void catch_renda_set( PLAYER_WORK *pl_w, Sint16 no )
{
	static Sint8 renda_rnd[32] = {
		3, -2, -1, 0, 0, 1, 2, -1, 1, 0, -3, 2, 0, 0, -2, -3,
		0, 2, 0, 0, 0, -1, 0, 1, -1, 0, -2, 1, 0, 0, 0, 3
	} ;

	if( ( pl_w->renda_timer = no + renda_rnd[Rnd() & 31] ) <= 0 ) pl_w->renda_timer = 1 ;
	pl_w->renda_max = pl_w->renda_timer ;
}

/****************************************************************/
/*			The thrower mashing: 1 each time its count runs		*/
/*			out, which starts it over							*/
/****************************************************************/
int catch_renda_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->renda_timer = pl_w->renda_timer - renda_get( pl_w ) ) > 0 ) return 0 ;
	pl_w->renda_timer = pl_w->renda_max ;
	return 1 ;
}

/*==============================================================*/
/*			This frame's mash: 2 for a new stick direction,		*/
/*			1 more for a button									*/
/*==============================================================*/
static int renda_get( PLAYER_WORK *pl_w )
{
	int n ;

	n = 0 ;
	if( !( pl_w->pl_last_lvbt & KEY_LEVER ) && pl_w->pl_now_lvbt & KEY_LEVER ) n = 2 ;
	if( pl_w->pl_now_lvbt & KEY_BUTTON ) n ++ ;
	return n ;
}

/****************************************************************/
/*			Mash down renda_cnt, and a hard enough mash moves		*/
/*			the animation on									*/
/****************************************************************/
void pl_renda_ck( PLAYER_WORK *pl_w )
{
	Uint16 sw ;
	Sint8 cnt[2] ;	/* the mash, and the count's low bits before it */

	if( !pl_w->renda_cnt || !( sw = pl_w->pl_now_lvbt ) ) return ;
	cnt[0] = 0 ;
	if( sw & KEY_LEVER ) cnt[0] += 2 ;
	if( sw & KEY_BUTTON ) cnt[0] ++ ;
	cnt[0] += renda_rnd_tbl[Rnd() & 15] ;
	cnt[1] = pl_w->renda_cnt & 3 ;
	if( ( pl_w->renda_cnt -= cnt[0] ) < 0 ) pl_w->renda_cnt = 0 ;
	if( cnt[1] >= cnt[0] ) return ;
	if( pl_w->routine_1 == PL_DAMAGE && pl_w->dm_no == 9 && pl_w->dm_no == 23 ){
		pl_w->cg_ctr = 1 ;
		pl_frame_jump( pl_w ) ;
	} else if( !pl_w->routine_1 && pl_w->act_st == N_DOWN ){
		pl_w->cg_ctr = 1 ;
	} else if( !pl_w->routine_1 && pl_w->act_st == N_DIZZY ){
		pl_w->cg_ctr = 1 ;
		pl_char_move( pl_w ) ;
		pl_w->cg_ctr = 1 ;
	}
}

/****************************************************************/
/*			Drift sideways with the stick while recover_ng_flag is		*/
/*			set													*/
/****************************************************************/
void pl_lever_drift( PLAYER_WORK *pl_w )
{
	Uint16 lv ;
	Float d ;

	if( !pl_w->vital_new ) return ;
	if( !pl_w->recover_ng_flag ) return ;
	lv = pl_w->pl_sw_new & ( KEY_FORWARD | KEY_BACK ) ;
	if( !lv ) return ;
	d = 0.052083332f ;
	if( pl_w->pl_dir ) d = -0.052083332f ;
	if( lv & KEY_FORWARD ) d = -d ;
	pl_w->spd.x += d ;
}

/****************************************************************/
/*			Start a hyper combo: its flash, or a team			*/
/*			hyper's or a delayed one's							*/
/****************************************************************/
void pl_hyper_start( PLAYER_WORK *pl_w, VEC3 *ofs, Sint8 no )
{
	if( pl_w->team_type == 4 ){
		pl_w->hyper_bg = 4 ;
		vs_sys_w->hyper_bg_req = ( pl_w->pl_no << 4 ) | 4 ;
		pl_w->hyper_bg_tm = 16 ;
		team_hyper_flash( pl_w, ofs ) ;
		pl_se_req( pl_w, 30 ) ;
		pl_voice_req( pl_w, 43 ) ;
		return ;
	}
	if( pl_w->team_type == 6 ){
		hyper_wave_start( pl_w, ofs ) ;
		pl_gauge_sub( pl_w, no ) ;
		pl_w->delay_flag = 0xff ;
		pl_w->delay_timer = 16 ;
		vib_req( pl_w->pl_no, 5 ) ;
		hyper_se_req( pl_w, vs_sys_w->hyper_cnt[pl_w->pl_no] - 1 ) ;
		return ;
	} else if( pl_w->team_type > 2 ) return ;
	else {
		pl_w->hyper_bg = vs_sys_w->hyper_cnt[pl_w->pl_no] ;
		vs_sys_w->hyper_bg_req = vs_sys_w->hyper_cnt[pl_w->pl_no] | ( pl_w->pl_no << 4 ) ;
		pl_w->hyper_bg_tm = 16 ;
		pl_hyper_flash( pl_w, ofs, no ) ;
		pl_w->delay_flag = 0xff ;
		pl_w->delay_timer = 16 ;
	}
}

/****************************************************************/
/*			A hyper combo's flash at the player, ofs from		*/
/*			its position										*/
/****************************************************************/
void pl_hyper_flash( PLAYER_WORK *pl_w, VEC3 *ofs, Sint8 no )
{
	VEC3 pos ;

	pl_w->hyper_bg = 1 ;
	vs_sys_w->hyper_bg_req = ( pl_w->pl_no << 4 ) | 1 ;
	pl_w->hyper_bg_tm = 16 ;
	pl_gauge_sub( pl_w, no ) ;
	if( !hyper_ef_set( pl_w, 0 ) ) return ;
	pos = pl_w->pos ;
	pos.y += ofs->y ;
	if( !pl_w->disp_dir ) pos.x += ofs->x ;
	else pos.x -= ofs->x ;
	hyper_flash_set( &pos, pl_w->disp_dir, 0, 0 ) ;
	vib_req( pl_w->pl_no, 5 ) ;
	hyper_se_req( pl_w, 0 ) ;
	pl_se_req( pl_w, 30 ) ;
	pl_voice_req( pl_w, 43 ) ;
	hyper_stop_set( pl_w ) ;
	pl_pic_set( pl_w, 0 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 2 ) ;
}

/****************************************************************/
/*			A hyper combo's flash without the effect at the		*/
/*			player												*/
/****************************************************************/
void pl_hyper_scr_flash( PLAYER_WORK *pl_w )
{
	pl_gauge_sub( pl_w, 1 ) ;
	if( !hyper_ef_set( pl_w, 4 ) ) return ;
	hyper_stop_set( pl_w ) ;
	scr_work.stop_x_mode = scr_work.stop_y_mode = 1 ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 2 ) ;
}

/*==============================================================*/
/*			A team hyper combo's flash							*/
/*==============================================================*/
static void team_hyper_flash( PLAYER_WORK *pl_w, VEC3 *ofs )
{
	VEC3 pos ;

	if( !hyper_ef_set( pl_w, 5 ) ) return ;
	pos = pl_w->pos ;
	pos.y += ofs->y ;
	if( !pl_w->disp_dir ) pos.x += ofs->x ;
	else pos.x -= ofs->x ;
	hyper_flash_set( &pos, pl_w->disp_dir, 0, 0 ) ;
	vib_req( pl_w->pl_no, 7 ) ;
	hyper_se_req( pl_w, pl_w->v_combi_cnt - 1 ) ;
	pl_se_req( pl_w, 30 ) ;
	pl_voice_req( pl_w, 43 ) ;
	hyper_stop_set( pl_w ) ;
	pl_pic_set( pl_w, 1 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 2 ) ;
	pl_w->change_req = 5 ;
}

/****************************************************************/
/*			The flash of a hyper combo one level up				*/
/****************************************************************/
void pl_hyper_lvup_flash( PLAYER_WORK *pl_w )
{
	pl_w->hyper_bg = vs_sys_w->hyper_cnt[pl_w->pl_no] + 1 ;
	vs_sys_w->hyper_bg_req = ( vs_sys_w->hyper_cnt[pl_w->pl_no] + 1 ) | ( pl_w->pl_no << 4 ) ;
	pl_w->hyper_bg_tm = 16 ;
	if( !hyper_ef_set( pl_w, 1 ) ) return ;
	hyper_stop_set( pl_w ) ;
	pl_se_req( pl_w, 30 ) ;
	pl_voice_req( pl_w, 43 ) ;
	pl_pic_set( pl_w, 0 ) ;
	scr_work.stop_x_mode = 1 ;
	scr_work.stop_y_mode = 7 ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 2 ) ;
}

/****************************************************************/
/*			A hyper combo's flash, with the effect at the		*/
/*			player and the screen's own kind					*/
/****************************************************************/
void pl_hyper_flash3( PLAYER_WORK *pl_w, VEC3 *ofs )
{
	VEC3 pos ;

	pl_w->hyper_bg = 1 ;
	vs_sys_w->hyper_bg_req = ( pl_w->pl_no << 4 ) | 1 ;
	pl_w->hyper_bg_tm = 16 ;
	if( !hyper_ef_set( pl_w, 2 ) ) return ;
	pos = pl_w->pos ;
	pos.y += ofs->y ;
	if( !pl_w->disp_dir ) pos.x += ofs->x ;
	else pos.x -= ofs->x ;
	hyper_flash_set( &pos, pl_w->disp_dir, 0, 0 ) ;
	vib_req( pl_w->pl_no, 5 ) ;
	hyper_se_req( pl_w, 0 ) ;
	pl_se_req( pl_w, 30 ) ;
	pl_voice_req( pl_w, 43 ) ;
	hyper_stop_set( pl_w ) ;
	pl_pic_set( pl_w, 0 ) ;
	scr_work.stop_x_mode = scr_work.stop_y_mode = 9 ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 2 ) ;
}

/****************************************************************/
/*			The flash of a snap back							*/
/****************************************************************/
void pl_snap_flash( PLAYER_WORK *pl_w, VEC3 *ofs )
{
	VEC3 pos ;

	pl_gauge_sub( pl_w, 1 ) ;
	if( !hyper_ef_set( pl_w, 6 ) ) return ;
	pos = pl_w->pos ;
	pos.y += ofs->y ;
	if( !pl_w->disp_dir ) pos.x += ofs->x ;
	else pos.x -= ofs->x ;
	snap_flash_set( &pos, pl_w->disp_dir, 0, 0 ) ;
	pl_se_req( pl_w, 64 ) ;
	hyper_stop_set( pl_w ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 2 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 2 ) ;
}

/*==============================================================*/
/*			Stop the fight for this side's flash				*/
/*==============================================================*/
static void hyper_stop_set( PLAYER_WORK *pl_w )
{
	pl_w->hyper_stop_flag |= 1 << pl_w->pl_no ;
	vs_sys_work.game_stop = pl_w->hyper_stop_flag ;
	pl_w->hyper_stop_timer = 0 ;
	scr_work.quake_level = 0 ;
	scr_work.quake_flag = 1 ;
	scr_work.stop_x_mode = scr_work.stop_y_mode = 10 ;
}

/****************************************************************/
/*			This side's flash is over							*/
/****************************************************************/
void pl_hyper_stop_clr( PLAYER_WORK *pl_w )
{
	pl_w->hyper_stop_flag &= ~( 1 << pl_w->pl_no ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][0], 10 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][1], 10 ) ;
	pl_color_set( vs_sys_work.team_w[pl_w->pl_no ^ 1][2], 10 ) ;
}

/****************************************************************/
/*			Count down the stop, and put every player's			*/
/*			colours back at its end								*/
/****************************************************************/
void pl_hyper_stop_ck( PLAYER_WORK *pl_w )
{
	int i ;

	if( --pl_w->hyper_stop_timer && !pl_w->routine_1 && ( pl_w->act_st == N_SUPER || pl_w->act_st == N_CHANGE ) ) return ;
	pl_w->hyper_stop_flag = 0 ;
	for( i = 0 ; i < 6 ; i ++ ) pl_color_set( &pl_work[i], 1 ) ;
}

/****************************************************************/
/*			Step the animation back or on by the frame's		*/
/*			jump code											*/
/****************************************************************/
void pl_frame_jump( register PLAYER_WORK *pl_w )
{
	Sint8 *p ;
	Uint8 c ;

	c = pl_w->cg_prm ;
	if( ( Sint8 )c >= 0 ) return ;
	p = pl_w->cg_adrs ;
	pl_char_move( pl_w ) ;
	if( c == pl_w->cg_prm ) return ;
	if( !( c & 0x40 ) ){
		c &= 15 ;
		if( !c ) return ;
		p -= ( c - 1 ) * 20 ;
	} else {
		c = ( c & 0x3f ) * 4 ;
		p += c ;
	}
	pl_w->cg_adrs = p ;
	*( CHAR_DATA * )&pl_w->cg_prm = *( CHAR_DATA * )p ;
	pl_w->cg_ctr ++ ;
	pl_char_move( pl_w ) ;
}

/****************************************************************/
/*			A special move's effect on the ground under the		*/
/*			player												*/
/****************************************************************/
void pl_sp_ef_set( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pos.x = pl_w->pos.x ;
	pos.y = pl_w->ground_y ;
	pos.z = pl_w->pos.z ;
	sp_ef_set( &pos, pl_w->disp_dir ) ;
}

/****************************************************************/
/*			A landing's effect on the ground under the			*/
/*			player												*/
/****************************************************************/
void pl_land_ef_set( PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pos.x = pl_w->pos.x ;
	pos.y = pl_w->ground_y ;
	pos.z = pl_w->pos.z ;
	fall_se_req( pl_w ) ;
	land_ef_set( &pos, 0 ) ;
}

/****************************************************************/
/*			Dust at the feet of a player moving on the			*/
/*			ground, every other frame							*/
/****************************************************************/
void pl_smoke_set( PLAYER_WORK *pl_w )
{
	if( Sys->game_timer0 & 1 ) return ;
	if( !( Sint32 )pl_w->spd.x ) return ;
	if( pl_w->pos.y > pl_w->ground_y ) return ;
	slide_dust_set( pl_w ) ;
}

/****************************************************************/
/*			Dust at the feet, every no frames					*/
/****************************************************************/
void pl_smoke_set2( PLAYER_WORK *pl_w, int no )
{
	if( Sys->game_timer0 % no ) return ;
	slide_dust_set( pl_w ) ;
}

/* em_dist: how far the player is from the opponent's nearest box */
/****************************************************************/
/*			The box numbers are read unsigned here (a Uint16	*/
/*			field would cost s_pl11 a routine)					*/
/****************************************************************/
void pl_em_dist_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	FRAME_BOX *bx ;
	HIT_BOX *p ;
	Float x[4] ;
	Float w, dist ;
	int back ;

	em = pl_w->target_w ;
	bx = em->frame_box ;
	if( !bx->body_box0 && !bx->body_box1 && !bx->body_box2 && !( bx->body_box3 & BOX_MASK ) ){
		pl_w->em_dist = 0.0f ;
		return ;
	}
	back = 0 ;
	x[0] = x[1] = x[2] = x[3] = -1.0f ;
	if( pl_w->pos.x > em->pos.x && !em->disp_dir || !( pl_w->pos.x > em->pos.x ) && em->disp_dir ){
		back = -1 ;
		x[0] = x[1] = x[2] = x[3] = 0.0f ;
	}
	if( em->frame_box->body_box0 ){
		p = &em->hit_box_tbl[( Uint16 )em->frame_box->body_box0] ;
		x[0] = DOT_X( p->x ) ;
		w = DOT_X( p->w ) ;
		if( back ) w = -w ;
		x[0] -= w ;
	}
	if( em->frame_box->body_box1 ){
		p = &em->hit_box_tbl[( Uint16 )em->frame_box->body_box1] ;
		x[1] = DOT_X( p->x ) ;
		w = DOT_X( p->w ) ;
		if( back ) w = -w ;
		x[1] -= w ;
	}
	if( em->frame_box->body_box2 ){
		p = &em->hit_box_tbl[( Uint16 )em->frame_box->body_box2] ;
		x[2] = DOT_X( p->x ) ;
		w = DOT_X( p->w ) ;
		if( back ) w = -w ;
		x[2] -= w ;
	}
	if( ( Sint16 )em->frame_box->body_box3 > 0 ){
		p = &em->hit_box_tbl[em->frame_box->body_box3 & BOX_MASK] ;
		x[3] = DOT_X( p->x ) ;
		w = DOT_X( p->w ) ;
		if( back ) w = -w ;
		x[3] -= w ;
	}
	if( !back ){
		if( x[0] >= x[1] ) x[0] = x[1] ;
		if( x[0] >= x[2] ) x[0] = x[2] ;
		if( x[0] >= x[3] ) x[0] = x[3] ;
	} else {
		if( x[0] <= x[1] ) x[0] = x[1] ;
		if( x[0] <= x[2] ) x[0] = x[2] ;
		if( x[0] <= x[3] ) x[0] = x[3] ;
	}
	if( em->disp_dir ) x[0] = -x[0] ;
	x[0] += em->pos.x ;
	if( pl_w->pos.x < em->pos.x ) dist = x[0] - pl_w->pos.x ;
	else dist = pl_w->pos.x - x[0] ;
	pl_w->em_dist = dist ;
}

/****************************************************************/
/*			1 if the opponent is more than range away			*/
/*			sideways											*/
/****************************************************************/
int pl_far_ck( PLAYER_WORK *pl_w, Float range )
{
	Float d ;

	d = pl_w->target_w->pos.x - pl_w->pos.x ;
	if( d > range || -range > d ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			1 if the opponent is more than range_x away			*/
/*			sideways, or more than range_y below				*/
/****************************************************************/
int pl_far_xy_ck( PLAYER_WORK *pl_w, Float range_x, Float range_y )
{
	Float d ;

	if( pl_far_ck( pl_w, range_x ) ) return 1 ;
	d = pl_w->pos.y - pl_w->target_w->pos.y ;
	if( d > range_y ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			The number of players on pl_w's side still in		*/
/*			the fight											*/
/****************************************************************/
int pl_be_cnt( PLAYER_WORK *pl_w )
{
	return vs_sys_w->be_cnt[pl_w->pl_no] ;
}

/****************************************************************/
/*			The winning team stands in a row on the ground,		*/
/*			and Abyss's last form falls to the losers' side		*/
/****************************************************************/
void pl_win_team_set( void )
{
	static Sint16 win_pos[3] = { 0, -0x7f00, 0x7f00 } ;	/* where the winners stand, from the screen's centre */
	PLAYER_WORK *pl_w ;
	int i ;

	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = vs_sys_w->team_w[vs_sys_w->winner_id][i] ;
		if( !pl_w->be_flag ){
			pl_w->be_flag = 1 ;
			pl_w->routine_0 = PL_FIGHT ;
			pl_w->routine_1 = PL_NORM ;
			pl_w->pos.y = pl_w->ground_y ;
			pl_act_set( pl_w, N_STAND ) ;
		} else {
			pl_w->spd.x = 0.0f ;
			pl_w->spd.y = 0.0f ;
			pl_w->acc.x = 0.0f ;
			pl_w->acc.y = 0.0f ;
			pl_jump_ret( pl_w ) ;
		}
		if( !pl_w->vital_new ){
			pl_w->vital_new = pl_w->vital_red = 1 ;
			pl_w->ko_flag = 0 ;
		}
		pl_w->pos.x = scr_work.eye[0].x + win_pos[i] * 1.6666666f / 256.0f ;
		if( FLAG_CK( pl_w->pl_bit, PLB0( PL_SONSON ), PLB1( PL_SERVBOT ) ) ) pl_w->armor_flag = 0 ;
		pl_color_set( pl_w, 1 ) ;
	}
	pl_w = vs_sys_w->team_w[vs_sys_w->winner_id ^ 1][0] ;
	if( pl_w->pl_type == PL_ABYSS3 ){
		pl_w->routine_0 = PL_FIGHT ;
		pl_w->routine_1 = PL_NORM ;
		pl_w->disp_flag = 0 ;
		pl_act_set( pl_w, N_DOWN ) ;
		pl_w->pos.x = scr_work.eye[0].x ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->catch_ng_timer = 16 ;
		pl_w->dm_flag = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_sub.c" End of File	*/
