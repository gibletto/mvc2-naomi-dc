/* Servbot's effect 02 (the module writes 0x3a01 as its number, as shl3a_01 does): a piece
   that flies off him, drifts with the stick, then falls and fades. */
#include "player.h"
#include "pl_char.h"
#include "pl3a_def.h"

static void shl3a_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl3a_02_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl3a_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->ef_no = 0x3a01 ;
		ew->obj_flag = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl3a_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_02_main( PLAYER_WORK *ew )
{
	static void ( *shl3a_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_02_init, shl3a_02_move, shl3a_02_end, shl3a_02_free,
	} ;

	shl3a_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* the speed it flies off at, and where it starts beside the master, by type */
static Float shl3a_02_spd_tbl[] = { -10.0f, -10.0f, -2.5f, -5.0f } ;
static Float shl3a_02_x_tbl[] = { -DOT_X( 56.0f ), DOT_X( 40.0f ), -DOT_X( 40.0f ), -DOT_X( 40.0f ) } ;

static void shl3a_02_shake( PLAYER_WORK *ew, Sint16 n ) ;

/*==============================================================*/
static void shl3a_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

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
	ew->prio = 8 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	x = shl3a_02_x_tbl[ew->type] ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + 0.0f ;
	ew->timer0 = 80 ;
	ew->timer1 = 30 ;
	if( ew->type > 1 ) ew->scale.x = ew->scale.y = 0.5f ;
	x = shl3a_02_spd_tbl[ew->type] ;
	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->dir = 0 ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	ew->atck_no = pl3a_at_plane ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 34 ) ;
	shl3a_02_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->timer1 ){
		if( !scr_in_x_ck( ew ) ) goto kill ;
	} else ew->timer1 -- ;
	if( ew->hit_flag ) goto kill ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( Sys->game_timer0 & 4 ){
		shl3a_02_shake( ew, 50 ) ;
		ew->dir ++ ;
	}
	if( vs_sys_work.vs_st == VS_FIGHT ){
		if( !pl_w->cpu_flag && !( pl_w->pl_sw_new & KEY_START ) && ew->timer0 > 0 ) ew->timer0 = 0 ;
		if( -- ew->timer0 < 0 ){
			ew->disp_flag = ( Sys->game_timer0 & 4 ) >> 2 ;
			ew->spd.x /= 2.0f ;
			if( ew->timer0 < -40 ){
				ew->routine_0 = 3 ;
				ew->disp_flag = 0 ;
				ew->pos.y += DOT_Y( 8.0f ) ;
				burst_set( ew, &ew->pos, 1, 8 ) ;
				pl_char_set( ew, Char_Effect, ew->char_no.b.no + 1 ) ;
			}
		}
		if( pl_w->pl_sw_new & KEY_UP ) ew->pos.y += DOT_Y( 0.75f ) ;
		if( pl_w->pl_sw_new & KEY_DOWN ) ew->pos.y -= DOT_Y( 0.75f ) ;
		if( ew->hit_box_no || pl_w->ground_y + -DOT_Y( 8.0f ) > ew->pos.y ){
			ew->routine_0 ++ ;
			ew->spd.y = DOT_Y( 2.0f ) ;
			ew->acc.y = -DOT_Y( 0.375f ) ;
		}
		hit_work_set( ew ) ;
	}
	return ;
kill:
	shl3a_02_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ( ew->alpha -= 0.05f ) < 0.0f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl3a_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			It moves up and down along a sine, by the frames	*/
/*			it has lasted										*/
/*==============================================================*/
static void shl3a_02_shake( PLAYER_WORK *ew, Sint16 n )
{
	Float s ;
	Sint16 rad ;

	s = n * 256.0f ;
	rad = ( ( 40 - ew->dir ) & 31 ) << 11 ;
	ew->pos.y = ew->pos.y + s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl3a_02.c" End of File	*/
