/* Shuma-Gorath's player effects: eight kinds of copy of his sprite that go with his moves, some
   standing at the opponent he holds (kind 2), some in a string that follows his rise (kinds 3, 4). */
#include "player.h"
#include "pl_char.h"
#include "pl2d_def.h"

/* where kind 2 stands from the held opponent, by his character: x and y in pixels */
static Sint16 pls2d_00_pos_tbl[] = {
	19, 70, 41, 100, 9, 55, 19, 47, 3, 72, 27, 72, 18, 48, 19, 56,
	19, 56, 19, 56, 19, 63, 16, 74, 25, 61, 29, 86, 25, 100, 24, 81,
	49, 52, 19, 56, 19, 56, 28, 63, 16, 63, 48, 63, 15, 56, 15, 73,
	0, 0, 0, 0, 0, 0, 21, 37, 21, 37, 11, 22, 19, 70, 15, 52,
	28, 51, 13, 71, 13, 49, 19, 70, 16, 58, 18, 67, 33, 76, 19, 70,
	25, 61, 48, 93, 14, 63, 17, 86, 27, 79, 28, 60, 21, 57, 36, 76,
	31, 67, 26, 57, 18, 79, 21, 57, 37, 61, 27, 86, 27, 86, 25, 64,
	22, 56, 19, 56, 12, 19, 0, 0,
} ;

/* kind 4: the animation's frame, by the work's sub_type */
static Uint8 pls2d_00_frame_tbl[] = {
	0, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0,
} ;

/* kind 7: the height above his place and the animation, by sub_type */
static Sint16 pls2d_00_y_tbl[16] = {
	96, 96, -72, -72, -8, -8, 56, 56, 96, 96, -72, -72, -8, -8, 56, 56,
} ;

static Uint8 pls2d_00_no_tbl[16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
} ;

/* kind 7: the draw priority by the animation's code */
static Float pls2d_00_prio_tbl[8] = {
	11.0f, 11.0f, 11.0f, 11.0f, 12.0f, 0.0f, 0.0f, 0.0f,
} ;

/* what every kind takes from his work when it starts */
#define PLS2D_00_COPY( ew, pl_w ) \
	CHAR_PART_COPY( ew, pl_w ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( pl_w )->pl_no ; \
	( ew )->pl_type = ( pl_w )->pl_type ; \
	( ew )->scale.x = ( pl_w )->scale.x ; \
	( ew )->scale.y = ( pl_w )->scale.y ; \
	( ew )->atck_level = ( pl_w )->atck_level ; \
	( ew )->work_no = ( pl_w )->work_no ; \
	( ew )->col_cnt = ( pl_w )->col_cnt ; \
	( ew )->scale = ( pl_w )->scale ; \
	( ew )->prio = ( pl_w )->prio

static void pls2d_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2d_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0x2d00 ;
		ew->type = no ;
		ew->pos = pl_w->pos ;
		ew->work_func = pls2d_00_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls2d_00_t0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_main( PLAYER_WORK *ew )
{
	static void ( *pls2d_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t0, pls2d_00_t1, pls2d_00_t2, pls2d_00_t3,
		pls2d_00_t4, pls2d_00_t5, pls2d_00_t6, pls2d_00_t7,
	} ;

	pls2d_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls2d_00_t0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t0_init, pls2d_00_t0_move, pls2d_00_free,
	} ;

	pls2d_00_t0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = 0 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( !ew->routine_1 ){
		if( pl_w->act_st == N_BACK_DASH ){
			pl_char_move( ew ) ;
			if( ew->cg_type ){
				ew->routine_1 ++ ;
				ew->cg_type = 0 ;
			}
		}
		else {
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
		}
	}
	else {
		ew->prio = 7 ;
		ew->pos = pl_w->pos ;
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
		}
		else if( ew->cg_type ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			lw->noki_flag = 1 ;
		}
	}
}

static void pls2d_00_t1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t1_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t1_init, pls2d_00_t1_move, pls2d_00_free,
	} ;

	pls2d_00_t1_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	pl_char_set( ew, Char_Effect, 1 ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

static void pls2d_00_t2_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t2_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t2_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t2_init, pls2d_00_t2_move, pls2d_00_free,
	} ;

	pls2d_00_t2_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls2d_00_ofs( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t2_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -2 ;
	ew->disp_flag = 0 ;
	ew->disp_dir = pl_w->disp_dir ;
	pls2d_00_ofs( ew, pl_w ) ;
	pl_char_set( ew, Char_Effect, 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t2_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *dm_w ;

	ew->prio = pl_w->prio ;
	ew->sub_prio = -2 ;
	ew->disp_dir = pl_w->disp_dir ;
	pl_char_move( ew ) ;
	if( !ew->routine_1 ){
		if( pl_w->cg_type ){
			ew->routine_1 ++ ;
			ew->disp_flag = 1 ;
		}
		pls2d_00_ofs( ew, pl_w ) ;
		return ;
	}
	pls2d_00_ofs( ew, pl_w ) ;
	if( pl_w->routine_1 == PL_CATCH && pl_w->char_no.b.grp == Char_Catch && pl_w->char_no.b.no == 2 ) return ;
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
	dm_w = pl_w->catch_w ;
	dm_w->disp_dir ^= 1 ;
	dm_w->pl_dir ^= 1 ;
}

/*==============================================================*/
static void pls2d_00_ofs( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *dm_w = pl_w->catch_w ;
	Sint16 *ofs = pls2d_00_pos_tbl + ( dm_w->pl_type << 1 ) ;
	Float x = *ofs ++ * 1.6666666f ;
	Float y = *ofs * 2.142857f ;

	if( !pl_w->pl_dir ) x = -x ;
	ew->pos.x = dm_w->pos.x + x ;
	ew->pos.y = dm_w->pos.y + y ;
}

static void pls2d_00_t3_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t3_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t3_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t3_init, pls2d_00_t3_move, pls2d_00_free,
	} ;
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( pl_w->char_no.b.grp != Char_Special || lw->rise_flag < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	pls2d_00_t3_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t3_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	PLAYER_WORK *sw ;
	int i ;

	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	ew->sub_prio = 127 ;
	for( i = 1 ; i < 17 ; i ++ ){
		if( ( sw = work_get( ew, W_PLS, LIST_BEHIND ) ) ){
			sw->ef_no = 0x2d00 ;
			sw->type = 4 ;
			sw->sub_type = i ;
			sw->pos = ew->pos ;
			sw->sub_prio = i - ew->sub_prio ;
			sw->work_func = pls2d_00_main ;
			sw->master_w = pl_w ;
			sw->ef_w = ew ;
		}
		else {
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			lw->rise_flag = -1 ;
			return ;
		}
	}
	pl_char_set( ew, Char_Effect, 6 ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t3_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;
	Float dy ;

	pl_char_move( ew ) ;
	dy = pl_w->pos.y - ew->pos.y ;
	if( !lw->rise_flag ){
		if( dy < DOT_Y( 144.0f ) ){
			ew->acc.y = dy / 16.0f ;
			return ;
		}
		lw->rise_flag = 1 ;
	}
	if( dy < DOT_Y( 176.0f ) ) ew->acc.y = dy / 16.0f ;
	else pl_w->pos.y = ew->pos.y + DOT_Y( 176.0f ) ;
}

static void pls2d_00_t4_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t4_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t4_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t4_init, pls2d_00_t4_move, pls2d_00_free,
	} ;
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( pl_w->char_no.b.grp != Char_Special || lw->rise_flag < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	pls2d_00_t4_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t4_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int f ;

	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = 7 ;
	ew->pos.x = pl_w->pos.x ;
	f = pls2d_00_frame_tbl[ew->sub_type] ;
	pl_char_set2( ew, Char_Effect, 7, f ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t4_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w ;
	PLAYER_WORK *bw ;

	ew->pos.x = pl_w->pos.x ;
	ef_w = ew->ef_w ;
	bw = ew->before_w ;
	ew->pos.y = bw->pos.y + ef_w->acc.y ;
}

static void pls2d_00_t5_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t5_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t5_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t5_init, pls2d_00_t5_move, pls2d_00_free,
	} ;

	pls2d_00_t5_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t5_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	ew->timer0 = 8 ;
	pl_char_set( ew, Char_Effect, 8 ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t5_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || pl_w->act_st != N_SUPER || !-- ew->timer0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	else {
		pl_char_move( pl_w ) ;	/* his own work */
	}
}

static void pls2d_00_t6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t6_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t6_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t6_init, pls2d_00_t6_move, pls2d_00_free,
	} ;

	pls2d_00_t6_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->prio = 12 ;
	ew->pos = pl_w->pos ;
	pl_char_set( ew, Char_Effect, 9 ) ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t6_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos = pl_w->pos ;
	if( ew->cg_type ){
		ofs = -DOT_X( 8.0f ) ;
		if( ew->disp_dir ) ofs = DOT_X( 8.0f ) ;
		ew->pos.x += ofs ;
	}
}

static void pls2d_00_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t7_step( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2d_00_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t7_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t7_init, pls2d_00_t7_step, pls2d_00_free,
	} ;

	pls2d_00_t7_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2D_00_COPY( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( pls2d_00_y_tbl[ew->sub_type] ) ;
	pl_char_set( ew, Char_Effect, pls2d_00_no_tbl[ew->sub_type] + 10 ) ;
	ew->prio = pls2d_00_prio_tbl[ew->cg_type >> 1] ;
}

static void pls2d_00_t7_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t7_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2d_00_t7_blink( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls2d_00_t7_step( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2d_00_t7_step_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2d_00_t7_wait, pls2d_00_t7_keep, pls2d_00_t7_blink,
	} ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls2d_00_t7_step_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2d_00_t7_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	pl_char_move( ew ) ;
	ew->prio = pls2d_00_prio_tbl[ew->cg_type >> 1] ;
	if( lw->release_flag ){
		ew->routine_1 ++ ;
		ew->timer0 = 10 ;
	}
	else {
		ew->disp_flag = ew->cg_flag1 ? 0 : 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls2d_00_t7_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->prio = pls2d_00_prio_tbl[ew->cg_type >> 1] ;
	if( !-- ew->timer0 ) ew->routine_1 ++ ;
	ew->disp_flag = ew->cg_flag1 ? 0 : 1 ;
}

/*--------------------------------------------------------------*/
static void pls2d_00_t7_blink( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_prm ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	else if( Sys->game_timer0 & 1 ){
		ew->disp_flag = 0 ;
	}
	else {
		ew->disp_flag = ew->cg_flag1 ? 0 : 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls2d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2d_00.c" End of File	*/
