/* M. Bison's effect 02 (character 26's shot 02): a work that stays beside him for one of his
   animations and follows it, until he leaves it. The Psycho Crusher sets type 0, on the ground and
   in the air; nothing sets types 1 and 2. */
#include "player.h"
#include "pl_char.h"
#include "pl26_def.h"

/* the effect's own work */
typedef struct SHL26_02_LOCAL_W {
	Uint16	char_no;	/* 000: M. Bison's animation when it was set */
} SHL26_02_LOCAL_W;
#define SHL26_02_LW( w )	( ( SHL26_02_LOCAL_W * )&( w )->obj_flag )

static SHL26_02_LOCAL_W *lw;	/* the running work's */
static PL26_LOCAL_W *pl_lw;	/* M. Bison's, unused */

static void shl26_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl26_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl26_02_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2602 ;
		pl_w->ef_w = ew ;
	}
	return ew ;
}

static void shl26_02_init( PLAYER_WORK *ew );
static void shl26_02_move( PLAYER_WORK *ew );
static void shl26_02_end( PLAYER_WORK *ew );
static void shl26_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_02_main( PLAYER_WORK *ew )
{
	static void ( *shl26_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_02_init, shl26_02_move, shl26_02_end, shl26_02_free,
	} ;

	pl_lw = ( PL26_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = SHL26_02_LW( ew ) ;
	ew->hit_box_no = 0 ;
	shl26_02_jmp[ew->routine_0]( ew ) ;
}

static void shl26_02_crusher_init( PLAYER_WORK *ew );
static void shl26_02_1_init( PLAYER_WORK *ew );
static void shl26_02_2_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_02_init( PLAYER_WORK *ew )
{
	static void ( *shl26_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_02_crusher_init, shl26_02_1_init, shl26_02_2_init,
	} ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	shl26_02_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			Type 0: in front of M. Bison						*/
/*==============================================================*/
static void shl26_02_crusher_init( PLAYER_WORK *ew )
{
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	lw->char_no = ew->master_w->char_no.w ;
	ew->prio = 0 ;
	ew->pos.x += ew->master_w->pl_dir ? 120.0f : -120.0f ;
	pl_char_set( ew, Char_Effect, 5 ) ;
}

/*==============================================================*/
/*			Types 1 and 2: on him, drawn in the animation he	*/
/*			is in												*/
/*==============================================================*/
static void shl26_02_1_init( PLAYER_WORK *ew )
{
	lw->char_no = ew->master_w->char_no.w ;
	ew->prio = 0 ;
	pl_char_set( ew, 21, 32 ) ;
}

/*==============================================================*/
static void shl26_02_2_init( PLAYER_WORK *ew )
{
	lw->char_no = ew->master_w->char_no.w ;
	ew->prio = 0 ;
	pl_char_set( ew, 21, 36 ) ;
}

static void shl26_02_crusher_move( PLAYER_WORK *ew );
static void shl26_02_1_move( PLAYER_WORK *ew );
static void shl26_02_2_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl26_02_move( PLAYER_WORK *ew )
{
	static void ( *shl26_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_02_crusher_move, shl26_02_1_move, shl26_02_2_move,
	} ;

	shl26_02_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl26_02_crusher_move( PLAYER_WORK *ew )
{
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->master_w->pl_dir ? 120.0f : -120.0f ;
	if( lw->char_no != ew->master_w->char_no.w ){
		shl26_02_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Follows M. Bison's animation frame by frame			*/
/*==============================================================*/
static void shl26_02_1_move( PLAYER_WORK *ew )
{
	static Uint8 shl26_02_1_char[] = { 32, 33, 34, 35 } ;

	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( lw->char_no != ew->master_w->char_no.w ){
		shl26_02_end( ew ) ;
		return ;
	}
	if( ew->cg_prm != ew->master_w->cg_prm )
		pl_char_set( ew, 21, shl26_02_1_char[ew->master_w->cg_prm >> 1] ) ;
}

/*==============================================================*/
static void shl26_02_2_move( PLAYER_WORK *ew )
{
	static Uint8 shl26_02_2_char[] = { 36, 37, 38, 39 } ;

	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( lw->char_no != ew->master_w->char_no.w ){
		shl26_02_end( ew ) ;
		return ;
	}
	if( ew->cg_prm != ew->master_w->cg_prm )
		pl_char_set( ew, 21, shl26_02_2_char[ew->master_w->cg_prm >> 1] ) ;
}

/*--------------------------------------------------------------*/
static void shl26_02_end( PLAYER_WORK *ew )
{
	ew->routine_2 ++ ;
	ew->disp_flag = 0 ;
	shl26_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl26_02.c" End of File	*/
