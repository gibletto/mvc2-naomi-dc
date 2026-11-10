/* Shuma-Gorath's effect 00 (character 2d's shot 00): the six eyes that circle him while he gathers
   his Mystic Stare. They keep to a ring of twelve places, then fly at the opponent and stick to him. */
#include "player.h"
#include "pl_char.h"
#include "pl2d_def.h"

/* the twelve places of the ring: x and y */
static Sint16 shl2d_00_ofs[12 * 2] = {
	0, 36, -18, 32, -32, 18, -40, 0, -32, -18, -18, -32,
	0, -36, 18, -32, 32, -18, 40, 0, 32, 18, 18, 32,
} ;

/* the eyes' flying speed by the player's strength: 16.16 */
static Sint32 shl2d_00_spd[2] = { -311296, -442368 } ;

/* where each eye sticks to the opponent, by his stance: x, y, in front and the animation */
static Sint16 shl2d_00_stick_0[6][4] = {
	{ 11, 36, 0, 20 }, { 21, 58, 0, 32 }, { 18, 52, 1, 16 },
	{ -8, 64, 0, 27 }, { -16, 53, 1, 14 }, { -16, 40, 0, 7 },
} ;
static Sint16 shl2d_00_stick_1[6][4] = {
	{ 11, 12, 0, 20 }, { 21, 34, 0, 32 }, { 18, 28, 1, 16 },
	{ -8, 40, 0, 27 }, { -24, 29, 1, 14 }, { -16, 16, 0, 7 },
} ;
static Sint16 shl2d_00_stick_2[6][4] = {
	{ 16, 56, 0, 20 }, { 16, 86, 0, 32 }, { 16, 64, 1, 16 },
	{ -12, 80, 0, 27 }, { -20, 64, 1, 14 }, { -10, 58, 0, 28 },
} ;
static Sint16 shl2d_00_stick_3[6][4] = {
	{ -48, 16, 0, 3 }, { -32, 16, 1, 28 }, { -16, 14, 0, 5 },
	{ 0, 18, 1, 19 }, { -16, 16, 0, 13 }, { -32, 20, 1, 15 },
} ;
static Sint16 ( *shl2d_00_stick_tbl[4] )[4] = {
	shl2d_00_stick_0, shl2d_00_stick_1, shl2d_00_stick_2, shl2d_00_stick_3,
} ;

static void shl2d_00_main( PLAYER_WORK *ew );

/****************************************************************/
int shl2d_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int type ;

	if( work_rest_cnt <= 6 ) return 0 ;
	for( type = 0 ; type < 6 ; type ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_END ) ) != 0 ){
			ew->ef_no = 0x2d00 ;
			ew->type = type ;
			ew->work_func = shl2d_00_main ;
			ew->master_w = pl_w ;
		}
	}
	return type ;
}

static void shl2d_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void shl2d_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void shl2d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );

/*==============================================================*/
static void shl2d_00_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	static void ( *shl2d_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw ) = {
		shl2d_00_init, shl2d_00_move, shl2d_00_free,
	} ;

	PL2D_LOCAL_W *lw ;
	pl_w = ew->master_w ;
	lw = PL2D_LW( pl_w ) ;
	shl2d_00_jmp[ew->routine_0]( ew, pl_w, lw ) ;
}

static void shl2d_00_acc_set( PLAYER_WORK *ew );
static void shl2d_00_stick( PLAYER_WORK *ew );

/*==============================================================*/
static void shl2d_00_init( register PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	Sint16 *ofs ;
	Float ofs_x ;
	Float ofs_y ;

	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->prio = 0 ;
	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 0 || pl_w->hit_box_no ) lw->orb_flag = -1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl2d_at_stare ;
	HIT_RESET( ew ) ;
	ew->timer1 = ew->type * 2 ;
	ofs = &shl2d_00_ofs[ew->timer1 << 1] ;
	ofs_x = -90.0f ;
	ofs_y = DOT_Y( 69.0f ) ;
	ew->timer0 = 1 ;
	if( ew->disp_dir ){
		ofs_x = 90.0f ;
		ew->timer0 = -1 ;
	}
	ew->pos.x = pl_w->pos.x + ofs_x + *ofs ++ * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + ofs_y + *ofs * DOT_Y( 1.0f ) ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

static void shl2d_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void shl2d_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void shl2d_00_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void shl2d_00_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );
static void shl2d_00_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void shl2d_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	static void ( *shl2d_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw ) = {
		shl2d_00_appear, shl2d_00_fly, shl2d_00_keep, shl2d_00_atck, shl2d_00_stop,
	} ;

	shl2d_00_jmp[ew->routine_1]( ew, pl_w, lw ) ;
}

/*==============================================================*/
/*			The eyes come out of the flash and start to			*/
/*			circle												*/
/*==============================================================*/
static void shl2d_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->obj_flag = ew->atck_level ? 30 : 27 ;
		ew->size_top = 16 ;
		ew->size_bottom = 16 ;
		ew->size_front = 16 ;
		ew->size_back = 16 ;
		lw->orb_fire_flag |= 1 << ew->type ;
		lw->orb_cnt -- ;
		ew->spd.x = shl2d_00_spd[ew->atck_level] * 1.6666666f / 65536.0f ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		shl2d_00_acc_set( ew ) ;
	}
	if( pl_w->routine_1 != PL_NORM || !pl_w->vital_new || lw->orb_flag == -1 || vs_sys_work.vs_st == VS_END_WAIT ){
		ew->routine_1 = 4 ;
		lw->orb_flag = -1 ;
		lw->orb_fire_flag |= 1 << ew->type ;
		pl_char_set2( ew, Char_Effect, 5, ew->cg_type ) ;
	}
}

/*==============================================================*/
/*			The eyes circle; a hit, the end of the move or		*/
/*			the time makes them fly or stop						*/
/*==============================================================*/
static void shl2d_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	PLAYER_WORK *em_w ;

	if( lw->orb_cnt == 0 ){
		ew->pos.x += ew->spd.x + ew->acc.x ;
		ew->pos.y += ew->spd.y + ew->acc.y ;
		if( -- ew->sub_no == 0 ){
			ew->timer1 = ( ew->timer1 + ew->timer0 ) % 12 ;
			if( ew->timer1 < 0 ) ew->timer1 = 11 ;
			shl2d_00_acc_set( ew ) ;
		}
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_box_no || -- ew->obj_flag == 0 || !pl_w->vital_new || pl_w->routine_1
		|| lw->orb_flag == -1 || vs_sys_work.vs_st == VS_END_WAIT ){
		ew->routine_1 = 4 ;
		lw->orb_flag = -1 ;
		pl_char_set2( ew, Char_Effect, 5, ew->cg_type ) ;
		return ;
	}
	if( ew->hit_flag ){
		if( ew->hit_flag & HR_GUARD ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			burst_set( ew, &ew->pos, 1, 0x88 ) ;
		} else {
			em_w = ew->em_w ;
			if( em_w->work_kind != W_PL ){
				ew->routine_0 = 2 ;
				ew->disp_flag = 0 ;
				burst_set( ew, &ew->pos, 1, 0x88 ) ;
			} else {
				ew->routine_1 ++ ;
				ew->col_flag = 1 ;
				ew->sub_no = ew->col_no ;
				ew->timer0 = 0x1e0 ;
				ew->timer1 = 8 ;
				ew->sub_type = 0 ;
				shl2d_00_stick( ew ) ;
				if( em_w->team_no || pl_w->team_no ){
					ew->timer0 = 1 ;
					ew->timer1 = 1 ;
				}
			}
		}
		return ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Stuck to the opponent: it follows him until the		*/
/*			move ends or he is knocked away						*/
/*==============================================================*/
static void shl2d_00_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	PLAYER_WORK *em_w = ew->em_w ;

	shl2d_00_stick( ew ) ;
	if( ew->hit_box_no || !pl_w->vital_new || !em_w->vital_new || pl_w->routine_1 != PL_NORM || vs_sys_work.vs_st == VS_END_WAIT ){
stop :
		ew->routine_1 = 4 ;
		lw->orb_flag = -1 ;
		pl_char_set2( ew, Char_Effect, 5, ew->cg_type ) ;
		return ;
	}
	if( pl_w->act_st == N_CHANGE ){
		ew->timer0 = 1 ;
		ew->timer1 = 1 ;
	}
	ew->col_flag = 1 ;
	ew->col_no = ew->sub_no ;
	if( -- ew->timer1 == 0 ){
		ew->timer1 = ew->timer0 / 8 * 4 ;
		ew->sub_type = 3 ;
	}
	if( ew->sub_type ){
		ew->col_no = ew->sub_no + 1 ;
		ew->sub_type -- ;
	}
	if( pl_w->routine_1 == PL_CAUGHT || lw->power_step ) goto stop ;
	if( pl_w->routine_1 != PL_CATCH && -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 8 ;
		ew->atck_no = pl2d_at_stare_2 ;
		HIT_RESET( ew ) ;
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2d_00_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( ew->hit_flag ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		burst_set( ew, &ew->pos, 1, 0x88 ) ;
		return ;
	}
	if( -- ew->timer0 == 0 ){
		ew->routine_1 = 4 ;
		pl_char_set2( ew, Char_Effect, 5, ew->cg_type ) ;
		return ;
	}
	shl2d_00_stick( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2d_00_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	if( pl_char_move( ew ) < 0 || ew->cg_type ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl2d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL2D_LOCAL_W *lw )
{
	lw->orb_fire_flag &= ~( 1 << ew->type ) ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			Aims the eye at the next place of the ring, four	*/
/*			frames away											*/
/*==============================================================*/
static void shl2d_00_acc_set( PLAYER_WORK *ew )
{
	int i = ew->timer1 ;
	Sint16 *p = &shl2d_00_ofs[i * 2] ;
	Sint16 *q ;
	Float x = *p ++ * 1.6666666f ;
	Float y = *p * DOT_Y( 1.0f ) ;
	Float nx ;
	Float ny ;
	int next ;

	next = ( i + ew->timer0 ) % 12 ;
	if( next < 0 ) next = 11 ;
	q = &shl2d_00_ofs[next * 2] ;
	nx = *q ++ * 1.6666666f ;
	ny = *q * DOT_Y( 1.0f ) ;
	ew->acc.x = ( nx - x ) / 4.0f ;
	ew->acc.y = ( ny - y ) / 4.0f ;
	ew->sub_no = 4 ;
}

/*==============================================================*/
/*			Puts the eye on the opponent, where his stance		*/
/*			says												*/
/*==============================================================*/
static void shl2d_00_stick( PLAYER_WORK *ew )
{
	PLAYER_WORK *em_w = ew->em_w ;
	Uint8 stance = em_w->pl_stance ;
	Sint16 *rec ;
	Float x ;
	Float y ;

	if( SMALL_CK( em_w ) ){
		if( stance == 0 ){
			/* nothing is left of what stood here */
		}
	}
	rec = shl2d_00_stick_tbl[em_w->pl_stance][0] ;
	rec += ew->type * 4 ;
	x = *rec ++ * 1.6666666f ;
	y = *rec ++ * DOT_Y( 1.0f ) ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x = em_w->pos.x + x ;
	ew->pos.y = em_w->pos.y + y ;
	ew->prio = *rec ++ ? 7 : 0 ;
	pl_char_set2( ew, Char_Effect, 4, *rec ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2d_00.c" End of File	*/
