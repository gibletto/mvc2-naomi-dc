/* Silver Samurai's effect 05 (character 2f's shot 05): a spark that flies up and round in a
   circle beside him, growing and shrinking, and the spark it leaves behind it each frame. */
#include "player.h"
#include "pl_char.h"
#include "pl2f_def.h"

static void shl2f_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2f_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_05_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f05 ;
		ew->type = 0 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The spark left behind: a copy of the work that		*/
/*			keeps its place beside Silver Samurai				*/
/*==============================================================*/
static void shl2f_05_trail_set( PLAYER_WORK *src_w, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_05_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f04 ;
		ew->type = 1 ;
		ew->dir = src_w->dir ;
		ew->pos = src_w->pos ;
		ew->spd.z = ew->pos.x - pl_w->pos.x ;
		ew->timer1 = src_w->timer1 ;
	}
}

static void shl2f_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2f_05_main( PLAYER_WORK *ew )
{
	static void ( *shl2f_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2f_05_init, shl2f_05_move, shl2f_05_end, shl2f_05_free,
	} ;

	shl2f_05_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* the trail's size, by the leader's turn */
static Float shl2f_05_scale_tbl[] = {
	0.52f, 0.58000004f, 0.64f, 0.70000005f, 0.76f, 0.82f, 0.88f, 0.94f,
	1.0f, 1.06f, 1.12f, 1.18f, 1.24f, 1.3f, 1.36f, 1.42f,
	1.48f, 1.42f, 1.36f, 1.3f, 1.24f, 1.18f, 1.12f, 1.06f,
	1.0f, 0.94f, 0.88f, 0.82f, 0.76f, 0.70000005f, 0.64f, 0.58000004f,
} ;

/*==============================================================*/
static void shl2f_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	if( !ew->type ){
		ew->disp_flag = 0 ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = 15.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.z = ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->dir = 0 ;
		ew->timer0 = 0 ;
	}
	else {
		ew->sub_prio = ew->timer1 ;
		ew->scale.x = ew->scale.y = shl2f_05_scale_tbl[ew->dir] ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl2f_at_homura ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 22, 6 ) ;
	}
}

static void shl2f_05_xmove( PLAYER_WORK *ew, Sint16 spd );

/*--------------------------------------------------------------*/
static void shl2f_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->type ){
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		ew->spd.z = pl_w->pos.x ;
		ew->dir += 3 ;
		ew->dir &= 31 ;
		shl2f_05_xmove( ew, 8000 ) ;
		ew->timer1 = 1 ;
		if( ( Uint8 )( ew->dir + 248 ) <= 16 ) ew->timer1 = -1 ;
		shl2f_05_trail_set( ew, pl_w ) ;
		if( !ground_ck( ew ) ) shl2f_05_free( ew, pl_w ) ;
	} else {
		ew->prio = pl_w->prio ;
		ew->pos.x = pl_w->pos.x + ew->spd.z ;
		if( pl_char_move( ew ) >= 0 ){
			hit_work_set( ew ) ;
			if( ew->cg_prm ){
				ew->cg_prm = 0 ;
				ew->scale.x *= 0.75f ;
				ew->scale.y *= 0.75f ;
			}
		} else shl2f_05_free( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2f_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void shl2f_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			The leader's place round the circle, from the		*/
/*			player												*/
/*==============================================================*/
static void shl2f_05_xmove( PLAYER_WORK *ew, Sint16 spd )
{
	Float s, x ;
	Sint16 rad ;

	s = spd * 256.0f ;
	rad = ( ( 40 - ew->dir ) & 31 ) << 11 ;
	x = s * nlCos( rad ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x = x + ew->spd.z ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2f_05.c" End of File	*/
