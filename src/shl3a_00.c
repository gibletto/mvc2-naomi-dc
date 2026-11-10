/* The effect of Servbot's shot, the wave that follows his fist: a hit that rides on him, of a
   few kinds by type. */
#include "player.h"
#include "pl_char.h"

/* the effect's own work, from obj_flag */
typedef struct SHL3A_00_LOCAL_W {
	Uint32	char_no;	/* 000: the master's animation when it was put out */
} SHL3A_00_LOCAL_W;
#define SHL3A_00_LW( w )	( ( SHL3A_00_LOCAL_W * )&( w )->obj_flag )

static void shl3a_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl3a_00_set( PLAYER_WORK *pl_w, int type, int time )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl3a_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->timer0 = time ;
		ew->type = type ;
		ew->ef_no = 0x3a00 ;
		SHL3A_00_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl3a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_00_main( PLAYER_WORK *ew )
{
	static void ( *shl3a_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_00_init, shl3a_00_move, shl3a_00_end, shl3a_00_free,
	} ;

	shl3a_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static Float shl3a_00_scale_tbl[] = { 0.121f, 0.246f, 0.372f, 0.491f, 0.741f, 1.0f } ;
/* where it is beside the master, x and y, by type */
static Float shl3a_00_pos_tbl[] = { -30.0f, 45.0f, -DOT_X( 16.0f ), DOT_Y( 27.0f ) } ;

/*==============================================================*/
static void shl3a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->be_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->scale.x = ew->scale.y = shl3a_00_scale_tbl[ew->timer0] ;
	{
		Uint32 n = ew->timer0 ;

		if( !( n > 0 ) ) n = 1 ;
		ew->timer1 = n ;
	}
	ew->dir = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->type + 126 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, ew->type ) ;
	shl3a_00_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float *p ;
	Float x, y ;

	if( SHL3A_00_LW( ew )->char_no == pl_w->char_no.w ){
		if( pl_w->routine_1 || ew->hit_box_no ) goto hit ;
		if( ew->hit_flag ){
			if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ) goto hit ;
			if( ew->em_w->work_kind != W_PL ) goto hit ;
		}
		if( pl_w->cg_type ) goto hit ;
		if( pl_char_move( ew ) < 0 && !ew->be_flag ){
			shl3a_00_free( ew, pl_w ) ;
			return ;
		}
		ew->prio = pl_w->prio ;
		ew->pos = pl_w->pos ;
		p = &shl3a_00_pos_tbl[ew->type * 2] ;
		x = p[0] ;
		y = p[1] ;
		if( ew->disp_dir ) x = -x ;
		ew->pos.x += x ;
		ew->pos.y += y ;
		if( -- ew->dir & 0x80 ){
			ew->dir = 8 ;
			ew->atck_no = ew->type + 126 ;
			HIT_RESET( ew ) ;
		}
		hit_work_set( ew ) ;
		return ;
hit:
		pl_w->cg_type = 0 ;
		ew->hit_box_no = 0 ;
		ew->be_flag = ew->timer0 = ew->hit_flag = 0 ;
		pl_char_set( ew, Char_Effect, ew->type + 32 ) ;
	} else shl3a_00_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void shl3a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl3a_00.c" End of File	*/
