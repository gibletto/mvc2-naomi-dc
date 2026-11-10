/* Servbot's effect 03: the bits of his attacks, seven kinds by type: those that stay with
   him through an animation, and those that fly up and bounce on the ground. */
#include "player.h"
#include "ef_char.h"

/* the effect's own work, from obj_flag */
typedef struct SHL3A_03_LOCAL_W {
	Uint32	char_no;	/* 000: the master's animation when it was put out */
} SHL3A_03_LOCAL_W;
#define SHL3A_03_LW( w )	( ( SHL3A_03_LOCAL_W * )&( w )->obj_flag )

static void shl3a_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl3a_03_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl3a_03_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->ef_no = 0x3a03 ;
		SHL3A_03_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl3a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl3a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl3a_03_main( PLAYER_WORK *ew )
{
	static void ( *shl3a_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl3a_03_init, shl3a_03_move, shl3a_03_end, shl3a_03_free,
	} ;

	shl3a_03_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* the animation of each type, and its draw order */
static Uint8 shl3a_03_no_tbl[] = { 2, 3, 4, 5, 6, 7, 8 } ;
static Sint8 shl3a_03_order_tbl[] = { 0, 0, 0, -1, -1, -1, -1, 0, 0 } ;

/*==============================================================*/
static void shl3a_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = shl3a_03_order_tbl[ew->type] ;
	ew->prio = 8 ;
	ew->pos = pl_w->pos ;
	if( ew->type >= 5 ){
		x = -DOT_X( 8.0f ) ;
		if( ew->disp_dir ) x = DOT_X( 8.0f ) ;
		ew->pos.x += x ;
		ew->pos.y += DOT_Y( 16.0f ) ;
	}
	if( ew->type >= 5 ){
		x = ( ( Rnd() & 63 ) << 4 ) * 1.6666666f / 256.0f ;
		if( !ew->disp_dir ) x = -x ;
		ew->spd.x = x ;
		ew->spd.y = ( ( Rnd() & 63 ) << 4 ) * 2.142857f / 256.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
	ef_char_set( ew, 27, shl3a_03_no_tbl[ew->type] ) ;
	shl3a_03_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->act_st == N_SUPER ) goto kill ;
	else if( pl_w->routine_1 ) goto kill ;
	else if( pl_w->hit_stop ) return ;
	if( ew->sub_prio ) ew->prio = pl_w->prio ;
	switch( ew->type ){
	case 3:
	case 4:
		if( SHL3A_03_LW( ew )->char_no != pl_w->char_no.w ) goto kill ;
		ew->pos.x = pl_w->pos.x ;
	case 1:
	case 2:
		ew->cg_ctr = pl_w->cg_ctr ;
	case 0:
		if( ef_char_move( ew ) < 0 ) goto kill ;
		else if( ew->cg_type ){
			ew->alpha -= 0.125f ;
			if( ew->alpha <= 0.0f ) ew->alpha = 0.0f ;
		}
		break ;
	case 5:
	case 6:
		ef_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.y = -ew->spd.y ;
			ew->spd.y /= 2.0f ;
			ew->timer0 = 1 ;
			if( !( ew->dir ^= 1 ) ) goto kill ;
		}
	}
	return ;
kill :
	shl3a_03_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl3a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl3a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl3a_03.c" End of File	*/
