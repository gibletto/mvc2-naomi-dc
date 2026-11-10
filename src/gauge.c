/* The fight's gauges: each player's vital gauge, the two sides' marks, the timed power-up's gauge
   and the team mark. Each piece is a work of list 11. */
#include "player.h"

/* the colour of each of a bar's four corners, for each character colour */
static Uint32 gauge_vtx_col_tbl[3][4] = {
	{ 0xFFFF40FF, 0xFFFFFF00, 0xFFFF40FF, 0xFFFFFF00 },
	{ 0xFF00FF00, 0xFFFFFF00, 0xFF00FF00, 0xFFFFFF00 },
	{ 0xFF00C0FF, 0xFFFFFF00, 0xFF00C0FF, 0xFFFFFF00 },
} ;

static VEC3 gauge_pos_tbl[24] = {
	{ -20.5f, 82.2f, -130.0f },
	{ 20.5f, 82.2f, -130.0f },
	{ -71.0f, 103.5f, -190.5f },
	{ 71.0f, 103.5f, -190.5f },
	{ -71.0f, 90.7f, -190.5f },
	{ 71.0f, 90.7f, -190.5f },
	{ -220.5f, 82.2f, -130.0f },
	{ 220.5f, 82.2f, -130.0f },
	{ -271.0f, 103.5f, -190.5f },
	{ 271.0f, 103.5f, -190.5f },
	{ -271.0f, 90.7f, -190.5f },
	{ 271.0f, 90.7f, -190.5f },
	{ -20.5f, 182.2f, -130.0f },
	{ 20.5f, 182.2f, -130.0f },
	{ -71.0f, 203.5f, -190.5f },
	{ 71.0f, 203.5f, -190.5f },
	{ -71.0f, 190.7f, -190.5f },
	{ 71.0f, 190.7f, -190.5f },
	{ -20.5f, 82.2f, -130.0f },
	{ 20.5f, 82.2f, -130.0f },
	{ -20.5f, 82.2f, -130.0f },
	{ 20.5f, 82.2f, -130.0f },
	{ -20.5f, 82.2f, -130.0f },
	{ 20.5f, 82.2f, -130.0f },
} ;

static VEC3 gauge_mark_pos_tbl[4] = {
	{ -7.2f, -49.2f, -71.1f },
	{ -7.2f, -49.25f, -71.225f },
	{ 7.2f, -49.2f, -71.1f },
	{ 7.2f, -49.25f, -71.225f },
} ;

static Uint32 gauge_icon_obj_tbl[6] = { 0x40, 0x49, 0x41, 0x4A, 0x42, 0x4B } ;

static VEC3 gauge_icon_pos_tbl[2] = {
	{ -100.5f, -3.300003f, 1.0f },
	{ 100.5f, -3.300003f, 1.0f },
} ;

static VEC3 gauge_wait_pos_tbl[2][3] = {
	{ { -33.0f, -13.0f, 21.0f }, { -33.0f, -11.75f, 21.0f }, { -33.0f, -10.25f, 21.0f } },
	{ { 33.0f, -13.0f, 21.0f }, { 33.0f, -11.75f, 21.0f }, { 33.0f, -10.25f, 21.0f } },
} ;

static VEC3 gauge_zero_vec = { 0.0f, 0.0f, 0.0f } ;
static VEC3 gauge_unit_vec = { 1.0f, 1.0f, 1.0f } ;

static VEC3 gauge_flash_pos_tbl[4] = {
	{ 25.5f, -38.0f, 70.5f },
	{ 25.5f, -33.199997f, 70.5f },
	{ -25.5f, -38.0f, 70.5f },
	{ -25.5f, -33.199997f, 70.5f },
} ;

static VEC3 gauge_timer_icon_pos_tbl[2] = {
	{ -52.0f, -43.7f, -70.0f },
	{ 52.0f, -43.7f, -70.0f },
} ;

static VEC3 gauge_timer_pos_tbl[2] = {
	{ -53.9f, -44.4f, -69.99f },
	{ 53.9f, -44.4f, -69.99f },
} ;

static FADE_COL gauge_col_tbl[10] = {
	{ 1.0f, 1.0f, 0.0f, 0.875f },
	{ 1.0f, 1.0f, 0.25f, 0.25f },
	{ 1.0f, 0.125f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.6f, 0.266f },
	{ 1.0f, 0.375f, 0.375f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 0.133f },
	{ 1.0f, 0.75f, 0.25f, 1.0f },
	{ 1.0f, 0.375f, 1.0f, 0.25f },
	{ 1.0f, 1.0f, 0.25f, 1.0f },
	{ 1.0f, 0.25f, 1.0f, 0.875f },
} ;

static FADE_COL gauge_mark_col_tbl[10] = {
	{ 1.0f, 0.85f, 0.0f, 0.74375004f },
	{ 1.0f, 0.85f, 0.2125f, 0.2125f },
	{ 1.0f, 0.10625f, 0.85f, 0.85f },
	{ 1.0f, 0.933f, 0.4f, 0.133f },
	{ 1.0f, 0.31875002f, 0.31875002f, 0.85f },
	{ 1.0f, 0.866f, 0.8f, 0.0f },
	{ 1.0f, 0.63750005f, 0.2125f, 0.85f },
	{ 1.0f, 0.31875002f, 0.85f, 0.2125f },
	{ 1.0f, 0.85f, 0.2125f, 0.85f },
	{ 1.0f, 0.2125f, 0.85f, 0.74375004f },
} ;

static FADE_COL gauge_mark_off_col = { 1.0f, 0.5f, 0.5f, 0.5f } ;

static FADE_COL gauge_white_col = { 1.0f, 1.0f, 1.0f, 1.0f } ;

static FADE_COL gauge_dim_col = { 1.0f, 0.5f, 0.5f, 0.5f } ;

static FADE_COL gauge_timer_col_tbl[15] = {
	{ 1.0f, 1.0f, 0.25f, 0.25f },
	{ 1.0f, 1.0f, 1.0f, 0.133f },
	{ 1.0f, 0.375f, 0.375f, 1.0f },
	{ 1.0f, 1.0f, 0.6f, 0.266f },
	{ 1.0f, 0.125f, 1.0f, 0.125f },
	{ 1.0f, 0.75f, 0.25f, 1.0f },
	{ 1.0f, 0.375f, 1.0f, 0.25f },
	{ 1.0f, 0.75f, 0.75f, 0.75f },
	{ 1.0f, 0.5f, 0.5f, 0.5f },
	{ 1.0f, 1.0f, 1.0f, DOT_X( 0.375f ) },
	{ 1.0f, 0.125f, 1.0f, 1.0f },
	{ 1.0f, 0.125f, 0.8f, 1.0f },
	{ 1.0f, 0.125f, 0.6f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 0.875f },
	{ 1.0f, 1.0f, 0.0f, 0.875f },
} ;

static VEC3 gauge_team_pos_tbl[6] = {
	{ -95.0f, 89.5f, -169.0f },
	{ 95.0f, 89.5f, -169.0f },
	{ -95.0f, 89.5f, -169.0f },
	{ 95.0f, 89.5f, -169.0f },
	{ -95.0f, 78.0f, -169.0f },
	{ 95.0f, 78.0f, -169.0f },
} ;

/* the object numbers the gauges use, by place */
static Uint8 gauge_team_idx_tbl[6] = { 0, 2, 4, 1, 3, 5 } ;
static Uint8 gauge_bar_obj_tbl[2] = { 62, 71 } ;
static Uint8 gauge_unk_85C[4] = { 0x74, 0x76, 0x75, 0x77 } ;
static Uint8 gauge_timer_icon_obj_tbl[2] = { 0x70, 0x71 } ;
static Uint8 gauge_timer_obj_tbl[2] = { 0x72, 0x73 } ;
static Uint8 gauge_team_obj_tbl[2] = { 0x78, 0x79 } ;

/* the effect's own work, from obj_flag: the timer gauge keeps the timer it shows */
typedef struct GAUGE_LOCAL_W {
	Sint32	flag ;
	Sint16	*timer ;
} GAUGE_LOCAL_W ;
#define GAUGE_LW( w )	( ( GAUGE_LOCAL_W * )&( w )->obj_flag )

static PLAYER_WORK *timer_gauge_w[6] ;
static Uint8 unk_0C2FB488[16] ;
extern void obj_uv_begin( void *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_col_put( Uint32 *col ) ;

static void gauge_set( int no ) ;
static void gauge_mark_set( int side, int no ) ;
static void vital_bar_set( PLAYER_WORK *gw, int no ) ;
static void gauge_icon_set( PLAYER_WORK *gw, int no ) ;
static void vital_wait_set( PLAYER_WORK *gw, int no ) ;
static void vital_red_set( PLAYER_WORK *gw, int no ) ;
static void vital_flash_set( PLAYER_WORK *gw, PLAYER_WORK *pl_w ) ;
static void timer_gauge_icon_set( PLAYER_WORK *gw ) ;
static void timer_gauge_set( PLAYER_WORK *pl_w, Sint16 *timer, int no ) ;
static void gauge_main( PLAYER_WORK *ew ) ;
static void gauge_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void gauge_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void gauge_col_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void gauge_disp( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void vital_bar_main( PLAYER_WORK *ew ) ;
static void gauge_mark_main( PLAYER_WORK *ew ) ;
static void gauge_icon_main( PLAYER_WORK *ew ) ;
static void vital_wait_main( PLAYER_WORK *ew ) ;
static void vital_flash_main( PLAYER_WORK *ew ) ;
static void vital_red_main( PLAYER_WORK *ew ) ;
static void timer_gauge_icon_main( PLAYER_WORK *ew ) ;
static void timer_gauge_main( PLAYER_WORK *ew ) ;
static void team_mark_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			A gauge piece that does nothing of its own			*/
/*==============================================================*/
static void gauge_null( PLAYER_WORK *ew )
{
}

/****************************************************************/
/*			The timed power-up's gauge, unless the game is		*/
/*			in mode 64											*/
/****************************************************************/
void Timer_Gauge_Set( PLAYER_WORK *pl_w, Sint16 *timer, int no )
{
	if( Sys->play_mode != 64 ) timer_gauge_set( pl_w, timer, no ) ;
}

/****************************************************************/
/*			The six players' vital gauges and the two sides'	*/
/*			marks, set up with the stage						*/
/****************************************************************/
void gauge_init( void )
{
	int i ;

	for( i = 0 ; i < 6 ; i ++ ) gauge_set( i ) ;
	for( i = 0 ; i < 2 ; i ++ ){
		gauge_mark_set( i, 0 ) ;
		gauge_mark_set( i, 1 ) ;
	}
}

/*==============================================================*/
/*			A player's vital gauge: its frame, which the		*/
/*			bar, icon, wait mark and red part ride on			*/
/*==============================================================*/
static void gauge_set( int no )
{
	PLAYER_WORK *ew, *pl_w ;
	Uint32 *p ;
	int i, obj_no ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = gauge_main ;
		obj_no = 61 ;
		if( no & 1 ) obj_no = 70 ;
		ew->obj = obj_dm00->obj[obj_no] ;
		pl_w = &pl_work[no] ;
		i = gauge_team_idx_tbl[pl_w->pl_no * 3 + pl_w->team_no] ;
		if( ABYSS_CK( pl_w ) ) i += 18 ;
		ew->pos = gauge_pos_tbl[i] ;
		ew->obj_flag = 0x10c21 ;
		ew->pl_no = no & 1 ;
		ew->type = no ;
		ew->sub_type = pl_w->team_no ;
		ew->master_w = pl_w ;
		ew->fade = gauge_white_col ;
		ew->timer0 = 0 ;
		p = ( Uint32 * )&timer_gauge_w[0] ;
		*p++ = 0 ;
		*p++ = 0 ;
		*p++ = 0 ;
		*p++ = 0 ;
		*p++ = 0 ;
		*p++ = 0 ;
		vital_bar_set( ew, no ) ;
		gauge_icon_set( ew, no ) ;
		vital_wait_set( ew, no ) ;
		vital_red_set( ew, no ) ;
	}
}

/*==============================================================*/
/*			The yellow bar of the gauge, as long as the			*/
/*			player's vital										*/
/*==============================================================*/
static void vital_bar_set( PLAYER_WORK *gw, int no )
{
	PLAYER_WORK *ew ;
	Uint32 *col ;
	int i ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = vital_bar_main ;
		ew->obj = obj_dm00->obj[gauge_bar_obj_tbl[no]] ;
		ew->scale = gauge_unit_vec ;
		ew->obj_flag = 0x10c10 ;
		ew->base_mtx = &gw->obj_mtx ;
		ew->pl_no = gw->pl_no ;
		ew->type = no ;
		ew->sub_type = pl_work[no].team_no ;
		ew->timer1 = pl_work[no].vital_new ;
		ew->master_w = &pl_work[no] ;
		ew->ef_w = gw ;
		obj_uv_begin( ew->obj, ew->obj ) ;
		col = gauge_vtx_col_tbl[pl_work[no].pl_personal] ;
		for( i = 0 ; i < 4 ; i ++ ){
			obj_uv_next() ;
			obj_uv_col_put( col ++ ) ;
		}
	}
}

/*==============================================================*/
/*			A mark by a side's gauge, side 0 or 1, no 0 or 1	*/
/*==============================================================*/
static void gauge_mark_set( int side, int no )
{
	PLAYER_WORK *ew ;
	int i ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = gauge_mark_main ;
		i = 63 ;
		if( side ) i = 72 ;
		ew->obj = obj_dm00->obj[i] ;
		ew->pos = gauge_mark_pos_tbl[side * 2 + no] ;
		ew->scale = gauge_unit_vec ;
		ew->obj_flag = 0x10c11 ;
		ew->type = side ;
		ew->sub_type = no ;
		ew->dir = 0 ;
		ew->master_w = &pl_work[side] ;
		ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 21845.0f ;
		ew->acc.z = 43690.0f ;
		ew->timer1 = ew->timer0 = 0 ;
		if( ABYSS_CK( ew->master_w ) ){
			ew->disp_flag = 0 ;
			ew->work_func = gauge_null ;
			ew->fade = gauge_mark_off_col ;
			if( no ) work_free( ew ) ;
		}
	}
}

/*==============================================================*/
/*			The icon in the gauge's frame						*/
/*==============================================================*/
static void gauge_icon_set( PLAYER_WORK *gw, int no )
{
	PLAYER_WORK *ew, *pl_w ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = gauge_icon_main ;
		ew->obj = obj_dm00->obj[gauge_icon_obj_tbl[no]] ;
		ew->pos = gauge_icon_pos_tbl[no & 1] ;
		ew->obj_flag = 0x10c21 ;
		ew->type = no ;
		pl_w = &pl_work[no] ;
		ew->sub_type = pl_w->team_no ;
		ew->master_w = pl_w ;
		ew->ef_w = gw ;
		ew->base_mtx = &gw->obj_mtx ;
	}
}

/*==============================================================*/
/*			The mark that shows while the red part of the		*/
/*			vital waits											*/
/*==============================================================*/
static void vital_wait_set( PLAYER_WORK *gw, int no )
{
	PLAYER_WORK *ew, *pl_w ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = vital_wait_main ;
		ew->obj = obj_dm00->obj[79] ;
		ew->pos = gauge_wait_pos_tbl[no & 1][no >> 1] ;
		ew->obj_flag = 0x10811 ;
		ew->scale.x = 5.0f ;
		ew->scale.y = ew->scale.z = 1.0f ;
		ew->type = no ;
		pl_w = &pl_work[no] ;
		ew->sub_type = pl_w->team_no ;
		ew->master_w = pl_w ;
		ew->base_mtx = &gw->obj_mtx ;
	}
}

/*==============================================================*/
/*			The flash by the bar when the vital grows			*/
/*==============================================================*/
static void vital_flash_set( PLAYER_WORK *gw, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = vital_flash_main ;
		ew->obj = obj_dm00->obj[80] ;
		ew->acc = ew->pos = gauge_flash_pos_tbl[( pl_w->pl_no << 1 ) + pl_w->team_no - 1] ;
		ew->obj_flag = 0x10801 ;
		ew->type = gw->type ;
		ew->sub_type = pl_w->team_no ;
		ew->master_w = pl_w ;
		ew->ef_w = gw ;
		ew->base_mtx = gw->base_mtx ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
/*			The red part of the bar, as long as the player's	*/
/*			vital_red											*/
/*==============================================================*/
static void vital_red_set( PLAYER_WORK *gw, int no )
{
	PLAYER_WORK *ew, *pl_w ;
	int i ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = vital_red_main ;
		i = 106 ;
		if( no & 1 ) i = 107 ;
		ew->obj = obj_dm00->obj[i] ;
		ew->scale = gauge_unit_vec ;
		ew->obj_flag = 0x10c11 ;
		ew->pos.x = 0.0f ;
		ew->pos.y = 0.0f ;
		ew->pos.z = -0.125f ;
		ew->base_mtx = &gw->obj_mtx ;
		ew->type = no ;
		pl_w = &pl_work[no] ;
		ew->sub_type = pl_w->team_no ;
		ew->master_w = pl_w ;
		ew->ef_w = gw ;
	}
}

/*==============================================================*/
/*			The timer gauge's icon, which follows the gauge		*/
/*==============================================================*/
static void timer_gauge_icon_set( PLAYER_WORK *gw )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( gw, W_SYS, LIST_BEHIND ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = timer_gauge_icon_main ;
		ew->obj = obj_dm00->obj[gauge_timer_icon_obj_tbl[gw->pl_no]] ;
		ew->pos = gauge_timer_icon_pos_tbl[gw->pl_no] ;
		ew->obj_flag = 0x10801 ;
		ew->master_w = gw ;
	}
}

/*==============================================================*/
/*			The gauge that shows a timer, with its colour no	*/
/*==============================================================*/
static void timer_gauge_set( PLAYER_WORK *pl_w, Sint16 *timer, int no )
{
	PLAYER_WORK *ew ;

	if( *timer ){
		if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
			ew->disp_flag = 1 ;
			ew->work_func = timer_gauge_main ;
			ew->obj = obj_dm00->obj[gauge_timer_obj_tbl[pl_w->pl_no]] ;
			ew->pos = gauge_timer_pos_tbl[pl_w->pl_no] ;
			ew->scale = gauge_unit_vec ;
			ew->pl_no = pl_w->pl_no ;
			ew->master_w = pl_w ;
			ew->obj_flag = 0x10c11 ;
			GAUGE_LW( ew )->timer = timer ;
			ew->timer0 = *timer ;
			ew->fade = gauge_timer_col_tbl[no] ;
			timer_gauge_icon_set( ew ) ;
			timer_gauge_w[pl_w->work_no] = ew ;
		}
	}
}

/****************************************************************/
/*			The mark of a player's place in the team, shown		*/
/*			for 120 frames										*/
/****************************************************************/
void team_mark_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( Sys->play_mode == 64 ) return ;
	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = team_mark_main ;
		ew->obj = obj_dm00->obj[gauge_team_obj_tbl[pl_w->pl_no]] ;
		ew->timer1 = 120 ;
		ew->obj_flag = 0x10801 ;
		ew->master_w = pl_w ;
		ew->pos = gauge_team_pos_tbl[pl_w->team_no * 2 + pl_w->pl_no] ;
		ew->routine_3 = pl_w->team_no ;
	}
}

/*==============================================================*/
static void gauge_main( PLAYER_WORK *ew )
{
	static void ( *gauge_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		gauge_in, gauge_move,
	} ;

	if( !ABYSS_CK( ew->master_w ) ) gauge_jmp[ew->routine_0]( ew, ew->master_w ) ;
	gauge_col_set( ew, ew->master_w ) ;
	gauge_disp( ew, ew->master_w ) ;
}

/*==============================================================*/
/*			When the player's place in the team changes, the	*/
/*			frame starts to move to its new place				*/
/*==============================================================*/
static void gauge_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 *p ;
	int team_no = pl_w->team_no ;

	if( ew->sub_type != team_no ){
		ew->routine_0 ++ ;
		ew->sub_no = 32 ;
		ew->timer1 = 0 ;
		if( ( ew->sub_type = team_no ) ) ew->timer1 |= 0x8000 ;
		p = &gauge_pos_tbl[team_no * 2 + pl_w->pl_no] ;
		ew->spd.x = ( p->x - ew->pos.x ) / 32.0f ;
		ew->spd.y = ( p->y - ew->pos.y ) / 32.0f ;
		ew->spd.z = ( p->z - ew->pos.z ) / 32.0f ;
		ew->acc = ew->pos ;
	}
}

/*==============================================================*/
/*			It moves, with a hop, and stops at its place		*/
/*==============================================================*/
static void gauge_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 *p ;
	Float y ;

	ew->pos.x += ew->spd.x ;
	ew->acc.y += ew->spd.y ;
	ew->pos.z += ew->spd.z ;
	y = 0.0f ;
	if( ew->timer1 >= 0 ){
		y = nlSin( ew->timer1 ) * 20.0f ;
		ew->timer1 += 0x400 ;
	}
	ew->pos.y = ew->acc.y + y ;
	if( -- ew->sub_no & 0x80 ){
		ew->routine_0 = 0 ;
		p = &gauge_pos_tbl[pl_w->team_no * 2 + pl_w->pl_no] ;
		ew->pos = *p ;
	}
}

/*==============================================================*/
/*			The frame's colour: white, or dimmed while the		*/
/*			red part of the vital waits							*/
/*==============================================================*/
static void gauge_col_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	FADE_COL *col ;

	col = &gauge_white_col ;
	ew->fade.r = col->r ;
	ew->fade.g = col->g ;
	ew->fade.b = col->b ;
	if( pl_w->vital_red_wait ){
		col = &gauge_dim_col ;
		ew->fade.r = col->r ;
		ew->fade.g = col->g ;
		ew->fade.b = col->b ;
	}
	if( ew->timer0 != pl_w->vital_red_wait ){
		if( !pl_w->vital_red_wait && pl_w->team_no ) team_mark_set( pl_w ) ;
	}
	ew->timer0 = pl_w->vital_red_wait ;
}

/*==============================================================*/
/*			The frame is hidden for a knocked-out player and	*/
/*			fades away after the fight							*/
/*==============================================================*/
static void gauge_disp( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->ko_flag && pl_w->vital_new <= 0 ) ew->disp_flag = 0 ;
	if( ew->disp_flag ){
		if( pl_w->ko_flag && !pl_w->be_flag ){
			if( vs_sys_work.ko_cnt[pl_w->pl_no] >= 3 && !pl_w->team_no ){
				ew->fade.trnsl = 1.0f ;
			}
			else {
				ew->fade.trnsl -= 0.03125f ;
				if( ew->fade.trnsl < 0.0f ){
					ew->disp_flag = 0 ;
					ew->fade.trnsl = 0.0f ;
				}
			}
		}
	}
	else if( !pl_w->ko_flag && pl_w->vital_new > 0 ){
		ew->disp_flag = 1 ;
		ew->fade.trnsl = 1.0f ;
	}
}

/*==============================================================*/
static void vital_bar_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->fade = ew->ef_w->fade ;
	pl_w = ew->master_w ;
	ew->scale.x = ( Float )pl_w->vital_new / 144.0f ;
	if( ew->timer1 < pl_w->vital_new && pl_w->team_no ) vital_flash_set( ew, pl_w ) ;
	ew->timer1 = pl_w->vital_new ;
}

/*==============================================================*/
/*			The mark blinks, or runs through colours when		*/
/*			its state is 5										*/
/*==============================================================*/
static void gauge_mark_main( PLAYER_WORK *ew )
{
	Uint32 no = vs_sys_work.gauge_level[ew->type] ;

	if( !ew->sub_type ){
		ew->fade = gauge_col_tbl[no + 1] ;
		ew->fade.r *= nlSin( bg_ang1 ) / 8.0f + 1.0f ;
		ew->fade.g *= nlSin( bg_ang1 ) / 8.0f + 1.0f ;
		ew->fade.b *= nlSin( bg_ang1 ) / 8.0f + 1.0f ;
		ew->scale.x = ( Float )vs_sys_work.gauge[ew->type] / 144.0f ;
	}
	else {
		ew->disp_flag = no ;
		if( ew->dir != no ) ew->fade = gauge_mark_col_tbl[no] ;
		if( no == 5 ){
			ew->fade.r = nlSin( bg_ang2 + ( int )ew->acc.x ) / 2.0f + 0.5f ;
			ew->fade.g = nlSin( bg_ang2 + ( int )ew->acc.y ) / 2.0f + 0.5f ;
			ew->fade.b = nlSin( bg_ang2 + ( int )ew->acc.z ) / 2.0f + 0.5f ;
		}
	}
	ew->dir = no ;
}

/*==============================================================*/
static void gauge_icon_main( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->ef_w->disp_flag ;
	ew->fade = ew->ef_w->fade ;
}

/*==============================================================*/
static void vital_wait_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->disp_flag = 0 ;
	pl_w = ew->master_w ;
	if( pl_w->vital_red_wait ){
		ew->disp_flag = 1 ;
		ew->pos = gauge_wait_pos_tbl[pl_w->pl_no][pl_w->team_no] ;
	}
}

/*==============================================================*/
/*			The flash grows from the bar's end through its		*/
/*			frames, and goes									*/
/*==============================================================*/
static void vital_flash_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w, *gw ;
	Float x ;

	ew->disp_flag = 1 ;
	pl_w = ew->master_w ;
	ew->pos = gauge_flash_pos_tbl[( pl_w->pl_no << 1 ) + pl_w->team_no - 1] ;
	gw = ew->ef_w ;
	x = -( gw->scale.x * 56.0f ) ;
	if( ew->type & 1 ) x = -x ;
	ew->pos.x = x + ew->acc.x ;
	ew->obj = obj_dm00->obj[80 + ( ew->timer0 >> 3 )] ;
	ew->timer0 ++ ;
	if( ew->timer0 > 40 || !pl_w->team_no || vs_sys_work.vs_st != VS_FIGHT ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
static void vital_red_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->fade = ew->ef_w->fade ;
	pl_w = ew->master_w ;
	ew->scale.x = ( Float )pl_w->vital_red / 144.0f ;
}

/*==============================================================*/
static void timer_gauge_icon_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = pl_w->disp_flag ;
	if( !pl_w->timer0 ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*			The bar shrinks with the timer; a newer gauge of	*/
/*			the player's takes the old one's place				*/
/*==============================================================*/
static void timer_gauge_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *old ;
	Sint16 *timer ;

	ew->disp_flag = 1 ;
	if( pl_w->team_no ) ew->disp_flag = 0 ;
	if( pl_w->vital_new > 0 && ew->timer0 ){
		timer = GAUGE_LW( ew )->timer ;
		ew->scale.x = ( Float )*timer / ( Float )ew->timer0 ;
		if( ( old = timer_gauge_w[pl_w->work_no] ) && ew != old ){
			if( timer == GAUGE_LW( old )->timer ){
				ew->timer0 = 0 ;
				ew->disp_flag = 0 ;
				timer_gauge_w[pl_w->work_no] = 0 ;
				work_free( ew ) ;
				return ;
			}
			old->disp_flag = 0 ;
		}
		timer_gauge_w[pl_w->work_no] = ew ;
		if( *timer > 0 ) return ;
	}
	ew->timer0 = 0 ;
	ew->disp_flag = 0 ;
	timer_gauge_w[pl_w->work_no] = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			The team mark blinks while the other side has		*/
/*			nobody who cannot be hit; it goes with the			*/
/*			player's place in the team							*/
/*==============================================================*/
static void team_mark_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VS_SYS_WORK *vs_w ;

	if( ew->routine_3 == pl_w->team_no ){
		if( !( ew->routine_3 = pl_w->team_no ) ) goto end ;
		if( pl_w->be_flag ) goto end ;
		ew->disp_flag = 0 ;
		vs_w = vs_sys_w ;
		if( vs_w->team_w[pl_w->pl_no ^ 1][0]->assist_dm_cnt ) return ;
		if( vs_w->team_w[pl_w->pl_no ^ 1][1]->assist_dm_cnt ) return ;
		if( vs_w->team_w[pl_w->pl_no ^ 1][2]->assist_dm_cnt ) return ;
		if( bg_ang1 >= 0 ) ew->disp_flag = 1 ;
		if( -- ew->timer1 > 0 ) return ;
	}
end:
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "gauge.c" End of File	*/
