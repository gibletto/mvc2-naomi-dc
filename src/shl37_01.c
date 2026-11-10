/* Jin's effect 01 (character 37's shot 01): the pieces of his Blodia Vulcan, ten kinds by type: a
   body that slides in from the screen's edge with its three parts, pieces that fall or rise at the
   edges and flames that follow his opponent. Each kind has its own state routines. */
#include "player.h"
#include "pl_char.h"
#include "pl37_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL37_01_LOCAL_W {
	Sint16	x;		/* 000: the effect's place across from its start x */
	Sint16	y;		/* 002: and the start x, from the screen's edge */
	Uint8	fire_flag;		/* 004 */
} SHL37_01_LOCAL_W;
#define SHL37_01_LW( w )	( ( SHL37_01_LOCAL_W * )&( w )->obj_flag )

/* the three parts of the 1 kind: when each starts, and where it is from its piece */
static Sint8 shl37_01_1_time_tbl[] = { 0, 2, 4 } ;
static Sint16 shl37_01_1_spd_tbl[] = { 18, 116, 21, 105, 30, 99 } ;

static void shl37_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl37_01_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl37_01_main ;
		ew->ef_no = 0x3701 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl37_01_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl37_01_main ;
		ew->ef_no = 0x3701 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part of an effect: it rides on ef_w and has		*/
/*			the same master										*/
/*==============================================================*/
static PLAYER_WORK *shl37_01_part_set( PLAYER_WORK *ef_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl37_01_main ;
		ew->ef_no = 0x3701 ;
		ew->master_w = ef_w->master_w ;
		ew->ef_w = ef_w ;
		ew->pl_type = ef_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl37_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_4( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_8( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_type_9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_main( PLAYER_WORK *ew )
{
	static void ( *shl37_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_type_0, shl37_01_type_1, shl37_01_type_2, shl37_01_type_3, shl37_01_type_4,
		shl37_01_type_5, shl37_01_type_6, shl37_01_type_7, shl37_01_type_8, shl37_01_type_9,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Super ){
		shl37_01_free( ew, pl_w ) ;
		return ;
	}
	shl37_01_jmp[ew->type]( ew, pl_w ) ;
}

static void shl37_01_0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_0_init, shl37_01_0_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/* the screen's edge, on the side it faces */
static void shl37_01_edge_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_x_add( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->sub_prio = 2 ;
	lw->fire_flag = 0 ;
	lw->x = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ew->spd.x = DOT_X( 4.0f ) ;
	ew->pos.y = pl_w->ground_y + DOT_Y( 32.0f ) ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, 22, 2 ) ;
	shl37_01_part_set( ew, 1, 0 ) ;
	shl37_01_part_set( ew, 1, 1 ) ;
	shl37_01_part_set( ew, 1, 2 ) ;
	shl37_01_0_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_edge_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	if( !ew->disp_dir ) lw->y = scr_work.scr_right ;
	else lw->y = scr_work.scr_left ;
	if( lw->x > DOT_X( 128.0f ) ) lw->x = 213 ;
}

/*==============================================================*/
static void shl37_01_x_add( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	lw->x += ew->spd.x ;
}

/*==============================================================*/
static void shl37_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;
	Sint16 x = lw->x ;

	if( !ew->disp_dir ) x = -x ;
	ew->pos.x = x + lw->y ;
}

static void shl37_01_0_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_0_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_0_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_0_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_0_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_0_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_0_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_0_in, shl37_01_0_wait, shl37_01_0_ride,
		shl37_01_0_fire, shl37_01_0_stop, shl37_01_0_out,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	if( pl_lw->burn_flag ) ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	shl37_01_0_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_0_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	shl37_01_edge_set( ew, pl_w ) ;
	shl37_01_x_add( ew, pl_w ) ;
	shl37_01_pos_set( ew, pl_w ) ;
	if( lw->x >= 200.0f ){
		lw->x = 213 ;
		shl37_01_pos_set( ew, pl_w ) ;
		ew->timer0 = 18 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_0_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl37_01_edge_set( ew, pl_w ) ;
	shl37_01_pos_set( ew, pl_w ) ;
	pl_char_move( ew ) ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 23 ) ;
		shl37_01_part_set( ew, 9, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_0_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	shl37_01_edge_set( ew, pl_w ) ;
	shl37_01_pos_set( ew, pl_w ) ;
	pl_char_move( ew ) ;
	if( !pl_w->disp_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 2 ) ;
		lw->fire_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_0_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	shl37_01_edge_set( ew, pl_w ) ;
	shl37_01_pos_set( ew, pl_w ) ;
	pl_char_move( ew ) ;
	if( !pl_lw->beam_flag ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		lw->fire_flag = 0 ;
		ew->timer0 = 24 ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_0_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl37_01_edge_set( ew, pl_w ) ;
	shl37_01_pos_set( ew, pl_w ) ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->spd.x = -DOT_X( 8.0f ) ;
		ew->timer0 = 14 ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_0_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl37_01_edge_set( ew, pl_w ) ;
	shl37_01_x_add( ew, pl_w ) ;
	shl37_01_pos_set( ew, pl_w ) ;
	if( ! -- ew->timer0 ) ew->routine_0 ++ ;
}

static void shl37_01_1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_1_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_1_init, shl37_01_1_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_1_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -1 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = DOT_X( shl37_01_1_spd_tbl[ew->sub_type * 2] ) ;
	ew->spd.y = DOT_Y( shl37_01_1_spd_tbl[ew->sub_type * 2 + 1] ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, 22, 3 ) ;
	ew->timer0 = shl37_01_1_time_tbl[ew->sub_type] ;
	shl37_01_1_move( ew, pl_w ) ;
}

static void shl37_01_1_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_1_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_1_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_1_wait, shl37_01_1_follow,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_1_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_1_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer0 -- ) return ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	shl37_01_1_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl37_01_1_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->pos.x = ef_w->pos.x + ew->spd.x ;
	ew->pos.y = ef_w->pos.y + ew->spd.y ;
	pl_char_move( ew ) ;
	if( pl_lw->catch_flag ) ew->disp_flag = 0 ;
}

static void shl37_01_2_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_2_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_2_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_2_init, shl37_01_2_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_2_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_2_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	x = ( Uint8 )Rnd() ;
	x = DOT_X( x ) ;
	x += scr_work.scr_left ;
	x += DOT_X( 64.0f ) ;
	ew->pos.x = x ;
	ew->pos.y = pl_w->ground_y ;
	ew->pos.z = pl_w->pos.z ;
	if( !( Rnd() & 3 ) ){
		scr_work.quake_level = 1 ;
		scr_work.quake_flag = 1 ;
	}
	pl_char_set( ew, 22, 4 ) ;
	shl37_01_2_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl37_01_2_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

static void shl37_01_3_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_3_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_3_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_3_init, shl37_01_3_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_3_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_3_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;
	Sint16 y ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -1 ;
	ew->atck_flag = HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( !ew->sub_type ){
		ew->atck_no = pl37_at_vulcan ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl37_at_vulcan_spark_2 ;
		HIT_RESET( ew ) ;
	}
	ew->atck_attr |= ATR_DOWN_LIFT ;
	ew->pos.z = pl_w->pos.z ;
	y = ( Uint8 )Rnd() ;
	ew->pos.y = scr_work.scr_bottom + ( y + 64 ) * 2.142857f ;
	if( !ew->disp_dir ){
		ew->pos.x = scr_work.scr_right ;
		lw->x = scr_work.scr_left ;
	}
	else {
		ew->pos.x = scr_work.scr_left ;
		lw->x = scr_work.scr_right ;
	}
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = -DOT_X( 16.0f ) ;
	ew->spd.y = -DOT_Y( 4.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, 22, 5 ) ;
	shl37_01_3_move( ew, pl_w ) ;
}

static void shl37_01_3_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_3_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_3_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_3_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_3_fly, shl37_01_3_burst,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_3_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static int shl37_01_3_far_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It flies across until it hits, or lands on the		*/
/*			ground, or has gone far enough						*/
/*==============================================================*/
static void shl37_01_3_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( pl_lw->catch_flag ) ew->disp_flag = 0 ;
	if( shl37_01_3_far_ck( ew, pl_w ) ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->disp_flag = 0 ;
		pl_char_set( ew, 22, 6 ) ;
		burst_none( &ew->pos, 1 ) ;
		return ;
	}
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->disp_flag = 0 ;
		pl_char_set( ew, 22, 6 ) ;
		burst_none( &ew->pos, 1 ) ;
		return ;
	}
	if( pl_w->ground_y <= ew->pos.y ) return ;
	ew->pos.y = pl_w->ground_y ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->disp_flag = 0 ;
	pl_char_set( ew, 22, 7 ) ;
	burst_none( &ew->pos, 1 ) ;
}

/*--------------------------------------------------------------*/
static void shl37_01_3_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

/*==============================================================*/
/*			It is within 26 of where it started across			*/
/*==============================================================*/
static int shl37_01_3_far_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;
	Sint16 d = ew->pos.x - lw->x ;

	if( d < 0 ) d = -d ;
	if( d >= DOT_X( 16.0f ) ) return 0 ;
	return 1 ;
}

static void shl37_01_4_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_4_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_4( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_4_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_4_init, shl37_01_4_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_4_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_4_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -4 ;
	ew->disp_dir = 0 ;
	lw->x = 0 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	pl_char_set( ew, 22, 14 ) ;
	shl37_01_4_move( ew, pl_w ) ;
}

static void shl37_01_4_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_4_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_4_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_4_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_4_in, shl37_01_4_keep,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_4_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It falls from above the screen's top until it is	*/
/*			down												*/
/*==============================================================*/
static void shl37_01_4_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->pos.x = scr_work.scr_left + 320.0f ;
	lw->y = scr_work.scr_bottom + -DOT_Y( 128.0f ) ;
	lw->x += ew->spd.y ;
	ew->pos.y = lw->x + lw->y ;
	if( lw->x >= DOT_Y( 120.0f ) ){
		lw->x = 257 ;
		ew->pos.y = lw->x + lw->y ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_4_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->pos.x = scr_work.scr_left + 320.0f ;
	lw->y = scr_work.scr_bottom + -DOT_Y( 128.0f ) ;
	ew->pos.y = lw->x + lw->y ;
}

static void shl37_01_5_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_5_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_5_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_5_init, shl37_01_5_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_5_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_5_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -4 ;
	ew->disp_dir = 0 ;
	lw->x = 0 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = 2.142857f ;
	pl_char_set( ew, 22, 15 ) ;
	shl37_01_5_move( ew, pl_w ) ;
}

static void shl37_01_5_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_5_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_5_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_5_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_5_in, shl37_01_5_keep,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_5_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It rises from below the screen's top until it is	*/
/*			up													*/
/*==============================================================*/
static void shl37_01_5_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->pos.x = scr_work.scr_left + 320.0f ;
	lw->y = scr_work.scr_top + DOT_Y( 32.0f ) ;
	lw->x += ew->spd.y ;
	ew->pos.y = -lw->x + lw->y ;
	if( lw->x >= DOT_Y( 24.0f ) ){
		lw->x = 51 ;
		ew->pos.y = -lw->x + lw->y ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_5_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->pos.x = scr_work.scr_left + 320.0f ;
	lw->y = scr_work.scr_top + DOT_Y( 32.0f ) ;
	ew->pos.y = -lw->x + lw->y ;
}

static void shl37_01_6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_6_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_6_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_6_init, shl37_01_6_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_6_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_6_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -5 ;
	ew->disp_dir = 0 ;
	lw->x = 0 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	if( !ew->sub_type ) ew->char_no.b.no = 16 ;
	else ew->char_no.b.no = 17 ;
	pl_char_set( ew, 22, ew->char_no.b.no ) ;
	shl37_01_6_move( ew, pl_w ) ;
}

static void shl37_01_6_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_6_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_6_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_6_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_6_in, shl37_01_6_keep,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_6_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It drops down one side of the screen: the first		*/
/*			on its left, the second on its right				*/
/*==============================================================*/
static void shl37_01_6_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->pos.x = scr_work.scr_left + 320.0f ;
	if( !ew->sub_type ) ew->pos.x += DOT_X( 128.0f ) ;
	else ew->pos.x -= DOT_X( 128.0f ) ;
	lw->y = scr_work.scr_bottom + -DOT_Y( 72.0f ) ;
	lw->x += ew->spd.y ;
	ew->pos.y = lw->x + lw->y ;
	if( lw->x < DOT_Y( 64.0f ) ) return ;
	lw->x = 154 ;
	ew->pos.y = lw->x + lw->y ;
	ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl37_01_6_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_01_LOCAL_W *lw = SHL37_01_LW( ew ) ;

	ew->pos.x = scr_work.scr_left + 320.0f ;
	if( !ew->sub_type ) ew->pos.x += DOT_X( 128.0f ) ;
	else ew->pos.x -= DOT_X( 128.0f ) ;
	lw->y = scr_work.scr_bottom + -DOT_Y( 72.0f ) ;
	ew->pos.y = lw->x + lw->y ;
}

static void shl37_01_7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_7_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_7_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_7_init, shl37_01_7_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_7_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tg_w = pl_w->target_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( !ew->sub_type ){
		ew->atck_no = pl37_at_vulcan_flame_1 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	} else {
		ew->atck_no = pl37_at_vulcan_flame_2 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	}
	ew->pos.x = tg_w->pos.x ;
	ew->pos.y = tg_w->pos.y ;
	ew->pos.y += DOT_Y( 32.0f ) ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, 22, 28 ) ;
	ew->timer0 = 4 ;
	shl37_01_7_move( ew, pl_w ) ;
}

static void shl37_01_7_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_7_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_7_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_7_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_7_wait, shl37_01_7_hit,
	} ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_7_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_7_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->timer0 ){
		ew->routine_0 ++ ;
		return ;
	}
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->disp_flag = 0 ;
		pl_char_set( ew, 22, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_7_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

static void shl37_01_8_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_8_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_8( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_8_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_8_init, shl37_01_8_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_8_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_8_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->sub_prio = -2 ;
	ew->disp_dir = 0 ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->sub_type ) ew->char_no.b.no = 19 ;
	else ew->char_no.b.no = 20 ;
	pl_char_set( ew, 22, ew->char_no.b.no ) ;
	shl37_01_8_move( ew, pl_w ) ;
}

static void shl37_01_8_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_8_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_8_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_8_keep,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_8_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It keeps to the edge of the screen: the first on	*/
/*			its right, the second on its left					*/
/*==============================================================*/
static void shl37_01_8_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !ew->sub_type ) ew->pos.x = scr_work.scr_right ;
	else ew->pos.x = scr_work.scr_left ;
	ew->pos.y = scr_work.scr_top + -DOT_Y( 48.0f ) ;
}

static void shl37_01_9_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_9_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_01_type_9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_type_9_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_9_init, shl37_01_9_move, shl37_01_end, shl37_01_free,
	} ;

	shl37_01_type_9_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_9_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->routine_0 ++ ;
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
	ew->disp_flag = 0 ;
	ew->sub_prio = -3 ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
	ew->pos.z = ef_w->pos.z ;
	pl_char_set( ew, 22, 24 ) ;
	ew->timer0 = 24 ;
	shl37_01_9_move( ew, pl_w ) ;
}

static void shl37_01_9_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_9_show( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_01_9_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_01_9_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_01_9_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_01_9_wait, shl37_01_9_show, shl37_01_9_fire,
	} ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super || !pl_lw->beam_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_01_9_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_01_9_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer0 -- ) return ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void shl37_01_9_show( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	SHL37_01_LOCAL_W *ef_lw = SHL37_01_LW( ef_w ) ;

	pl_char_move( ew ) ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
	if( ef_lw->fire_flag ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 27 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_01_9_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	SHL37_01_LOCAL_W *ef_lw = SHL37_01_LW( ef_w ) ;
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	pl_char_move( ew ) ;
	ew->pos.x = ef_w->pos.x ;
	ew->pos.y = ef_w->pos.y ;
	if( pl_lw->catch_flag ) ew->disp_flag = 0 ;
	if( !ef_lw->fire_flag ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl37_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl37_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl37_01.c" End of File	*/
