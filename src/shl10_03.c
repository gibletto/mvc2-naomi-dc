/* Tron Bonne's effect 03 (character 10's shot 03): a Servbot who charges in from off the screen,
   one of a line of them, and runs on across it. */
#include "player.h"
#include "pl_char.h"
#include "pl10_def.h"

/* the Servbot's two animations, by kind: running in, and on the screen */
static Uint8 shl10_03_char_tbl[][2] = {
	{ 0x22, 0x23 }, { 0x24, 0x25 }, { 0x26, 0x27 }, { 0x28, 0x29 }, { 0x2a, 0x2b },
	{ 0x2c, 0x2d }, { 0x2e, 0x2f }, { 0x30, 0x31 }, { 0x32, 0x33 }, { 0x34, 0x35 },
	{ 0x36, 0x37 }, { 0x38, 0x39 }, { 0x3a, 0x3b }, { 0x3c, 0x3d }, { 0x3e, 0x3f },
} ;

/* the attack of each Servbot in the line, and whether he shouts */
static struct { Uint8 atck_no ; Uint8 voice ; } shl10_03_atck_tbl[] = {
	{ 0x35, 0 }, { 0x34, 0 }, { 0x33, 0 }, { 0x34, 0 }, { 0x35, 1 }, { 0x34, 0 }, { 0x33, 0 }, { 0x34, 0 },
	{ 0x35, 0 }, { 0x34, 0 }, { 0x33, 0 }, { 0x34, 0 }, { 0x35, 0 }, { 0x34, 1 }, { 0x33, 0 }, { 0x34, 0 },
	{ 0x35, 0 }, { 0x34, 0 }, { 0x33, 1 }, { 0x34, 0 }, { 0x35, 0 }, { 0x34, 0 }, { 0x33, 0 }, { 0x34, 1 },
	{ 0x35, 0 }, { 0x34, 0 }, { 0x33, 0 }, { 0x34, 0 }, { 0x35, 0 }, { 0x34, 0 }, { 0x33, 1 }, { 0x34, 0 },
	{ 0x35, 0 }, { 0x34, 0 }, { 0x33, 0 }, { 0x34, 0 }, { 0x35, 0 }, { 0x34, 0 }, { 0x33, 0 }, { 0x3e, 1 },
} ;

static void shl10_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl10_03_set( PLAYER_WORK *pl_w, Sint8 no, Uint8 cnt )
{
	PLAYER_WORK *ew ;
	PL10_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl10_03_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_no = cnt ;
		lw = PL10_LW( pl_w ) ;
		lw->run_cnt ++ ;
	}
	return ew ;
}

static void shl10_03_init( PLAYER_WORK *ew );
static void shl10_03_move( PLAYER_WORK *ew );
static void shl10_03_end( PLAYER_WORK *ew );
static void shl10_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl10_03_main( PLAYER_WORK *ew )
{
	static void ( *shl10_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl10_03_init, shl10_03_move, shl10_03_end, shl10_03_free,
	} ;

	shl10_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl10_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1003 ;
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
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 24 ;
	ew->size_back = 24 ;
	ew->prio = 10 ;
	ew->timer0 = 300 ;
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ){
		ew->pos.x += DOT_X( 368.0f ) ;
		ew->spd.x = -DOT_X( 8.0f ) ;
	} else {
		ew->pos.x -= DOT_X( 368.0f ) ;
		ew->spd.x = DOT_X( 8.0f ) ;
	}
	ew->pos.y = pl_w->ground_y ;
	ew->acc.x = 0.0f ;
	if( shl10_03_atck_tbl[ew->sub_no].voice ) pl_voice_req( ew, 26 ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = shl10_03_atck_tbl[ew->sub_no].atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, shl10_03_char_tbl[ew->type][0] ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	shl10_03_move( ew ) ;
}

static void shl10_03_run_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl10_03_move( PLAYER_WORK *ew )
{
	static void ( *shl10_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_03_run_main, shl10_03_run_main, shl10_03_run_main, shl10_03_run_main,
		shl10_03_run_main, shl10_03_run_main, shl10_03_run_main, shl10_03_run_main,
		shl10_03_run_main, shl10_03_run_main, shl10_03_run_main, shl10_03_run_main,
		shl10_03_run_main, shl10_03_run_main, shl10_03_run_main,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 || -- ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl10_03_move_jmp[ew->type]( ew, pl_w ) ;
}

static void shl10_03_run_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl10_03_run_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_03_run_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_03_run_in, shl10_03_run,
	} ;

	shl10_03_run_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Until he reaches the screen, or the Servbot			*/
/*			ahead of him										*/
/*==============================================================*/
static void shl10_03_run_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw = PL10_LW( pl_w ) ;
	PLAYER_WORK *em = lw->servbot_w ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( lw->servbot_flag && em->work_kind == W_PLS_ST && em->ef_no == 0x1003 ){
		if( ( ew->disp_dir == 0 && ew->pos.x <= em->pos.x ) || ( ew->disp_dir != 0 && ew->pos.x >= em->pos.x ) ){
			ew->routine_1 ++ ;
			em->timer1 ++ ;
			pl_char_set( ew, Char_Effect, shl10_03_char_tbl[ew->type][1] ) ;
		}
	} else if( scr_in_x_ck( ew ) ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, shl10_03_char_tbl[ew->type][1] ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_03_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer0 <= 210 && !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl10_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl10_03.c" End of File	*/
