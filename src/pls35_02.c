/* Blackheart's player effect 02: the pieces that come up out of the ground round him in his
   entrance and fly up off the screen, showing a short animation by their type. */
#include "player.h"
#include "pl35_def.h"

/* where each type ends, from the ground */
static Float pls35_02_y_tbl[9] = {
	DOT_Y( 32.0f ), DOT_Y( 32.0f ), DOT_Y( 32.0f ), DOT_Y( 32.0f ), DOT_Y( 48.0f ), DOT_Y( 48.0f ), DOT_Y( 32.0f ), DOT_Y( 32.0f ), DOT_Y( 32.0f ),
} ;

static void pls35_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls35_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls35_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3502 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->type = ( Rnd() & 3 ) + 4 ;
		ew->sub_type = 0 ;
	}
	return ew ;
}

static void pls35_02_init( PLAYER_WORK *ew );
static void pls35_02_move( PLAYER_WORK *ew );
static void pls35_02_end( PLAYER_WORK *ew );
static void pls35_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_02_main( PLAYER_WORK *ew )
{
	static void ( *pls35_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_02_init, pls35_02_move, pls35_02_end, pls35_02_free,
	} ;
	PLAYER_WORK *w = ew ;

	pls35_02_jmp[w->routine_0]( w ) ;
}

static void pls35_02_init0( PLAYER_WORK *ew );
static void pls35_02_rise_init( PLAYER_WORK *ew );
static void pls35_02_jump_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_02_init( PLAYER_WORK *ew )
{
	static void ( *pls35_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_02_init0, pls35_02_init0, pls35_02_rise_init, pls35_02_rise_init,
		pls35_02_jump_init, pls35_02_jump_init, pls35_02_rise_init, pls35_02_rise_init,
	} ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	pls35_02_init_jmp[ew->type]( ew ) ;
	ew->routine_1 = ew->routine_2 = 0 ;
}

static void pls35_02_pos_ofs( PLAYER_WORK *ew );
static void pls35_02_acc_set( PLAYER_WORK *ew );
static void pls35_02_char_set( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_02_init0( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	ew->prio = 10 ;
	pls35_02_pos_ofs( ew ) ;
	ew->timer0 = 0 ;
	ew->cg_type = 0 ;
	ew->timer1 = 1 ;
	pls35_02_char_set( ew ) ;
}

/*==============================================================*/
static void pls35_02_rise_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	ew->prio = 12 ;
	pls35_02_pos_ofs( ew ) ;
	pls35_02_acc_set( ew ) ;
	ew->timer0 = 0 ;
	ew->cg_type = 0 ;
	ew->timer1 = 1 ;
	pls35_02_char_set( ew ) ;
}

/*==============================================================*/
static void pls35_02_jump_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	ew->prio = 0 ;
	pls35_02_pos_ofs( ew ) ;
	ew->timer0 = 0 ;
	ew->cg_type = 0 ;
	ew->timer1 = 1 ;
	pls35_02_char_set( ew ) ;
}

static void pls35_02_move0( PLAYER_WORK *ew );
static void pls35_02_rise_move( PLAYER_WORK *ew );
static void pls35_02_jump_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls35_02_move( PLAYER_WORK *ew )
{
	static void ( *pls35_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_02_move0, pls35_02_move0, pls35_02_rise_move, pls35_02_rise_move,
		pls35_02_jump_move, pls35_02_jump_move, pls35_02_rise_move, pls35_02_rise_move,
	} ;

	pls35_02_move_jmp[ew->type]( ew ) ;
}

static void pls35_02_mv( PLAYER_WORK *ew );
static void pls35_02_time8( PLAYER_WORK *ew );
static void pls35_02_time16( PLAYER_WORK *ew );
static void pls35_02_anim( PLAYER_WORK *ew );

/*==============================================================*/
static void pls35_02_move0( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		pls35_02_mv( ew ) ;
		pls35_02_time8( ew ) ;
	} else {
		if( !scr_in_ck( ew ) ) ew->routine_0 ++ ;
		pls35_02_mv( ew ) ;
		pls35_02_anim( ew ) ;
	}
}

/*==============================================================*/
static void pls35_02_rise_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pls35_02_time16( ew ) ;
	} else {
		if( !scr_in_ck( ew ) ) ew->routine_0 ++ ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pls35_02_anim( ew ) ;
	}
}

/*==============================================================*/
static void pls35_02_jump_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		pls35_02_mv( ew ) ;
		pls35_02_time16( ew ) ;
	} else {
		if( !scr_in_ck( ew ) ) ew->routine_0 ++ ;
		pls35_02_mv( ew ) ;
		pls35_02_anim( ew ) ;
	}
}

/*==============================================================*/
/*			The animation's frame by how far the work is		*/
/*			from its end height: an eighth or a sixteenth		*/
/*==============================================================*/
static void pls35_02_time8( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float d ;
	int n ;

	d = pl_w->ground_y + pls35_02_y_tbl[ew->type] - ew->pos.y + DOT_Y( 8.0f ) ;
	if( d < 0.0f ) d = -d ;
	n = d / 2.142857f ;
	n /= 8 ;
	if( ew->timer0 > n ){
		pls35_02_anim( ew ) ;
		return ;
	}
	ew->timer0 = n ;
	pls35_02_char_set( ew ) ;
}

/*==============================================================*/
static void pls35_02_time16( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float d ;
	int n ;

	d = pl_w->ground_y + pls35_02_y_tbl[ew->type] - ew->pos.y + DOT_Y( 8.0f ) ;
	if( d < 0.0f ) d = -d ;
	n = d / 2.142857f ;
	n /= 16 ;
	if( ew->timer0 > n ){
		pls35_02_anim( ew ) ;
		return ;
	}
	ew->timer0 = n ;
	pls35_02_char_set( ew ) ;
}

/*==============================================================*/
static void pls35_02_anim( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->timer1 = ew->cg_ctr ;
	if( ew->cg_type < 0 ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
}

/* the frames each type shows, and the animation of each */
static Sint16 pls35_02_end_tbl[9] = { 7, 7, 4, 3, 6, 6, 3, 3, 4 } ;
static Uint8 pls35_02_char_tbl[9][8] = {
	{ 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16 },
	{ 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e },
	{ 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35 },
	{ 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 },
	{ 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25 },
	{ 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c, 0x2c },
	{ 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42 },
	{ 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39 },
	{ 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e },
} ;

/*==============================================================*/
static void pls35_02_char_set( PLAYER_WORK *ew )
{
	if( ew->timer0 >= pls35_02_end_tbl[ew->type] ){
		ew->timer0 = pls35_02_end_tbl[ew->type] ;
		ew->routine_1 ++ ;
	}
	pl_char_set2( ew, 18, pls35_02_char_tbl[ew->type][ew->timer0], ew->cg_type & 127 ) ;
	ew->cg_ctr = ew->timer1 ;
}

/* where it starts, beside the player */
static Float pls35_02_x_tbl[9][4] = {
	{ -DOT_X( 44.0f ), -60.0f, -DOT_X( 28.0f ), DOT_X( 20.0f ) },
	{ DOT_X( 44.0f ), 60.0f, DOT_X( 28.0f ), -DOT_X( 20.0f ) },
	{ 0.0f, -DOT_X( 8.0f ), 0.0f, DOT_X( 8.0f ) },
	{ 0.0f, DOT_X( 8.0f ), -DOT_X( 8.0f ), DOT_X( 16.0f ) },
	{ 80.0f, DOT_X( 40.0f ), DOT_X( 32.0f ), 40.0f },
	{ -80.0f, -DOT_X( 40.0f ), -DOT_X( 32.0f ), -40.0f },
	{ -DOT_X( 8.0f ), DOT_X( 8.0f ), -DOT_X( 40.0f ), DOT_X( 20.0f ) },
	{ DOT_X( 16.0f ), DOT_X( 8.0f ), -DOT_X( 8.0f ), -30.0f },
	{ DOT_X( 16.0f ), DOT_X( 8.0f ), -DOT_X( 16.0f ), -40.0f },
} ;

static Float pls35_02_y2_tbl[9] = {
	DOT_Y( 32.0f ), DOT_Y( 32.0f ), DOT_Y( 40.0f ), DOT_Y( 32.0f ), DOT_Y( 48.0f ), DOT_Y( 48.0f ), DOT_Y( 40.0f ), DOT_Y( 24.0f ), 120.0f,
} ;

/*==============================================================*/
static void pls35_02_pos_ofs( PLAYER_WORK *ew )
{
	Float x = pls35_02_x_tbl[ew->type][Rnd() & 3] ;

	if( ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y -= pls35_02_y2_tbl[ew->type] ;
}

/* its first speeds: the rise, by the roll */
static Float pls35_02_up_tbl[4][2] = {
	{ DOT_Y( 3.0f ), 0.13392857f },
	{ DOT_Y( 3.0f ), 0.4017857f },
	{ DOT_Y( 5.0f ), DOT_Y( 0.125f ) },
	{ DOT_Y( 6.0f ), 0.4017857f },
} ;

/*==============================================================*/
static void pls35_02_acc_set( PLAYER_WORK *ew )
{
	Uint8 no ;

	ew->spd.x = ew->acc.x = 0.0f ;
	no = Rnd() & 3 ;
	ew->spd.y = pls35_02_up_tbl[no][0] ;
	ew->acc.y = pls35_02_up_tbl[no][1] ;
}

static void pls35_02_mv_init( PLAYER_WORK *ew );
static void pls35_02_mv_rise( PLAYER_WORK *ew );
static void pls35_02_mv_wait( PLAYER_WORK *ew );
static void pls35_02_mv_fly( PLAYER_WORK *ew );

/*==============================================================*/
/*			Its flight: the first speeds, the rise, a wait at	*/
/*			the top, then up and off the screen					*/
/*==============================================================*/
static void pls35_02_mv( PLAYER_WORK *ew )
{
	static void ( *pls35_02_mv_jmp[] )( PLAYER_WORK *ew ) = {
		pls35_02_mv_init, pls35_02_mv_rise, pls35_02_mv_wait, pls35_02_mv_fly,
	} ;

	pls35_02_mv_jmp[ew->routine_2]( ew ) ;
}

static Float pls35_02_spd_x_tbl[8][2] = {
	{ 1.6666666f, 0.0f },
	{ DOT_X( 0.5f ), 0.0f },
	{ DOT_X( 0.25f ), 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ -DOT_X( 0.25f ), 0.0f },
	{ -DOT_X( 0.5f ), 0.0f },
	{ -1.6666666f, 0.0f },
} ;
static Float pls35_02_spd_y_tbl[8][2] = {
	{ DOT_Y( 8.0f ), -DOT_Y( 0.125f ) },
	{ 15.0f, -DOT_Y( 0.125f ) },
	{ DOT_Y( 6.5f ), -DOT_Y( 0.125f ) },
	{ DOT_Y( 5.0f ), -DOT_Y( 0.125f ) },
	{ DOT_Y( 8.0f ), -DOT_Y( 0.125f ) },
	{ DOT_Y( 6.0f ), -DOT_Y( 0.125f ) },
	{ DOT_Y( 8.0f ), -DOT_Y( 0.125f ) },
	{ DOT_Y( 10.0f ), -DOT_Y( 0.125f ) },
} ;

/*==============================================================*/
static void pls35_02_mv_init( PLAYER_WORK *ew )
{
	Uint8 no ;

	ew->routine_2 ++ ;
	no = Rnd() & 7 ;
	ew->spd.x = pls35_02_spd_x_tbl[no][0] ;
	ew->acc.x = pls35_02_spd_x_tbl[no][1] ;
	no = Rnd() & 7 ;
	ew->spd.y = pls35_02_spd_y_tbl[no][0] ;
	ew->acc.y = pls35_02_spd_y_tbl[no][1] ;
}

static Uint8 pls35_02_wait_tbl[4] = { 5, 6, 8, 10 } ;

/*--------------------------------------------------------------*/
static void pls35_02_mv_rise( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.y < 0.0f ){
		ew->routine_2 ++ ;
		ew->routine_3 = pls35_02_wait_tbl[Rnd() & 3] ;
	}
}

static Float pls35_02_fly_x_tbl[8][2] = {
	{ 5.0f, 0.0f },
	{ DOT_X( 2.5f ), 0.0f },
	{ DOT_X( 2.0f ), 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ -DOT_X( 2.0f ), 0.0f },
	{ -DOT_X( 2.5f ), 0.0f },
	{ -5.0f, 0.0f },
} ;
static Float pls35_02_fly_y_tbl[8][2] = {
	{ DOT_Y( 2.0f ), DOT_Y( 0.25f ) },
	{ DOT_Y( 2.0f ), DOT_Y( 0.25f ) },
	{ DOT_Y( 2.0f ), DOT_Y( 0.5f ) },
	{ DOT_Y( 2.0f ), DOT_Y( 0.625f ) },
	{ DOT_Y( 4.0f ), DOT_Y( 0.75f ) },
	{ DOT_Y( 4.0f ), 1.875f },
	{ DOT_Y( 4.0f ), 2.142857f },
	{ DOT_Y( 4.0f ), DOT_Y( 1.125f ) },
} ;

/*--------------------------------------------------------------*/
static void pls35_02_mv_wait( PLAYER_WORK *ew )
{
	Uint8 no ;

	if( !-- ew->routine_3 ){
		ew->routine_2 ++ ;
		no = Rnd() & 7 ;
		ew->spd.x = pls35_02_fly_x_tbl[no][0] ;
		ew->acc.x = pls35_02_fly_x_tbl[no][1] ;
		no = Rnd() & 7 ;
		ew->spd.y = pls35_02_fly_y_tbl[no][0] ;
		ew->acc.y = pls35_02_fly_y_tbl[no][1] ;
	}
}

/*--------------------------------------------------------------*/
static void pls35_02_mv_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void pls35_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls35_02_free( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls35_02.c" End of File	*/
