/* Tron Bonne's effect 01 (character 10's shot 01): a Servbot thrown forward, who lands, turns
   round and runs off the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl10_def.h"

/* a throw's speed and start from Tron, its priority, flight, animation and attack, by strength */
typedef struct SHL10_01_DATA {
	Sint32	spd_x, spd_y;	/* 16.16 dots */
	Sint16	ofs_x, ofs_y;
	Sint8	prio;
	Sint8	time;		/* frames before he starts to fall */
	Uint8	char_no;
	Uint8	atck_no;
} SHL10_01_DATA;

static SHL10_01_DATA shl10_01_tbl[] = {
	{ -0x40000, 0x38000, -84, 97, 11, 18, 28, 49 },
	{ -0x40000, 0x38000, -84, 97, 11, 30, 28, 49 },
} ;

static void shl10_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl10_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;
	PL10_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl10_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		lw = PL10_LW( pl_w ) ;
		lw->throw_cnt ++ ;
	}
	return ew ;
}

static void shl10_01_init( PLAYER_WORK *ew );
static void shl10_01_move( PLAYER_WORK *ew );
static void shl10_01_end( PLAYER_WORK *ew );
static void shl10_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl10_01_main( PLAYER_WORK *ew )
{
	static void ( *shl10_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl10_01_init, shl10_01_move, shl10_01_end, shl10_01_free,
	} ;

	shl10_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl10_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL10_01_DATA *dt ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1001 ;
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
	ew->size_top = 32 ;
	ew->size_bottom = 32 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	dt = &shl10_01_tbl[ew->type] ;
	ew->prio = dt->prio ;
	ew->timer0 = dt->time ;
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ){
		ew->pos.x += 1.6666666f * dt->ofs_x ;
		ew->spd.x = 1.6666666f * dt->spd_x / 65536.0f ;
	} else {
		ew->pos.x -= 1.6666666f * dt->ofs_x ;
		ew->spd.x = -( 1.6666666f * dt->spd_x / 65536.0f ) ;
	}
	ew->pos.y += 2.142857f * dt->ofs_y ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 2.142857f * dt->spd_y / 65536.0f ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = dt->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dt->char_no ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
	shl10_01_move( ew ) ;
}

static void shl10_01_atck_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl10_01_move( PLAYER_WORK *ew )
{
	static void ( *shl10_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_01_atck_main, shl10_01_safe_main,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl10_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl10_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_look( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_run_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			While he can still hit								*/
/*==============================================================*/
static void shl10_01_atck_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_01_atck_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_01_fly, shl10_01_stop, shl10_01_fall, shl10_01_land,
		shl10_01_look, shl10_01_turn, shl10_01_run_set, shl10_01_run,
	} ;

	shl10_01_atck_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl10_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_2 ++ ;
		ew->spd.x = ew->spd.x / 2.0f ;
		ew->spd.y = ew->spd.y / 2.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 29 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_01_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.y <= -DOT_Y( 1.5f ) ){
		ew->routine_2 ++ ;
		ew->spd.x = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_01_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + DOT_Y( 80.0f ) ){
		ew->routine_2 ++ ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 30 ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_01_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_2 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_set( ew, Char_Effect, 31 ) ;
		lw = PL10_LW( pl_w ) ;
		if( lw->throw_cnt ) lw->throw_cnt -- ;
	}
}

/*==============================================================*/
/*			Turns to face Tron, if he isn't already				*/
/*==============================================================*/
static void shl10_01_look( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 dir = 0 ;

	if( pl_char_move( ew ) < 0 ){
		if( ew->pos.x - pl_w->pos.x < 0.0f ) dir = 1 ;
		if( ew->disp_dir == dir ){
			ew->routine_2 = 6 ;
			shl10_01_run_set( ew, pl_w ) ;
			return ;
		}
		ew->routine_2 ++ ;
		ew->disp_dir = dir ;
		pl_char_set( ew, Char_Effect, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_2 ++ ;
		shl10_01_run_set( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_run_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_2 ++ ;
	if( !ew->disp_dir ) ew->spd.x = -DOT_X( 4.75f ) ;
	else ew->spd.x = DOT_X( 4.75f ) ;
	ew->acc.x = 0.0f ;
	pl_char_set( ew, Char_Effect, 13 ) ;
	shl10_01_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl10_01_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void shl10_01_safe_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_look( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_run_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl10_01_safe_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The same, without hits								*/
/*==============================================================*/
static void shl10_01_safe_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl10_01_safe_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_01_safe_start, shl10_01_safe_fly, shl10_01_safe_fall, shl10_01_safe_land,
		shl10_01_safe_look, shl10_01_safe_turn, shl10_01_safe_run_set, shl10_01_safe_run,
	} ;

	shl10_01_safe_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl10_01_safe_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_2 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.y = 0.0f ;
	shl10_01_safe_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_2 ++ ;
		ew->spd.x = ew->spd.x / 2.0f ;
		ew->spd.y = ew->spd.y / 2.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 29 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + DOT_Y( 80.0f ) ){
		ew->routine_2 ++ ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL10_LOCAL_W *lw ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_2 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_set( ew, Char_Effect, 31 ) ;
		lw = PL10_LW( pl_w ) ;
		if( lw->throw_cnt ) lw->throw_cnt -- ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_look( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 dir = 0 ;

	if( pl_char_move( ew ) < 0 ){
		if( ew->pos.x - pl_w->pos.x < 0.0f ) dir = 1 ;
		if( ew->disp_dir == dir ){
			ew->routine_2 = 6 ;
			shl10_01_run_set( ew, pl_w ) ;
			return ;
		}
		ew->routine_2 ++ ;
		ew->disp_dir = dir ;
		pl_char_set( ew, Char_Effect, 14 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_2 ++ ;
		shl10_01_run_set( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_run_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_2 ++ ;
	if( !ew->disp_dir ) ew->spd.x = -DOT_X( 4.75f ) ;
	else ew->spd.x = DOT_X( 4.75f ) ;
	ew->acc.x = 0.0f ;
	pl_char_set( ew, Char_Effect, 13 ) ;
	shl10_01_run( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl10_01_safe_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl10_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl10_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl10_01.c" End of File	*/
