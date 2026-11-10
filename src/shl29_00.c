/* Juggernaut's effect 00 (character 29's shot 00): the two kinds of piece that come with his
   rocks: type 0 an animation that rides on him, type 1 a row of hits placed out in front of him
   one after another. */
#include "player.h"
#include "pl_char.h"

/* how many frames each hit waits, and how far in front of him it falls, by his level and which hit */
static Sint8 shl29_00_time_tbl[2 * 8] = {
	0, 16, 32, 0, 0, 0, 0, 0,
	0, 10, 20, 30, 40, 0, 0, 0,
} ;
static Sint16 shl29_00_x_tbl[2 * 5] = {
	-128, -160, -192, 0, 0,
	-128, -176, -224, -272, -320,
} ;

/* the effect's own work, from obj_flag */
typedef struct SHL29_00_LOCAL_W {
	Sint32	se_no;	/* 000: which hit of the row makes the sound */
} SHL29_00_LOCAL_W;
#define SHL29_00_LW( w )	( ( SHL29_00_LOCAL_W * )&( w )->obj_flag )

static void shl29_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl29_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl29_00_main ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->ef_no = 0x2900 ;
	}
	return ew ;
}

static void shl29_00_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl29_00_row( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl29_00_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ) work_free( ew ) ;
	else if( !ew->type ) shl29_00_ride( ew, pl_w ) ;
	else shl29_00_row( ew, pl_w ) ;
}

/*==============================================================*/
static void shl29_00_ride( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		pl_char_set( ew, Char_Effect, 0 ) ;
		ew->sub_prio = -8 ;
	}
	ew->prio = pl_w->prio ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
static void shl29_00_row( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( !ew->routine_0 ){
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
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->sub_prio = -8 ;
		SHL29_00_LW( ew )->se_no = pl_w->atck_level ? 4 : 2 ;
		ew->timer1 = ew->timer0 = shl29_00_time_tbl[pl_w->atck_level * 8 + ew->sub_type] ;
		ew->timer1 -= 4 ;
		x = DOT_X( shl29_00_x_tbl[pl_w->atck_level * 5 + ew->sub_type] ) ;
		if( ew->disp_dir ) x = -x ;
		ew->pos.x += x ;
		ew->atck_no = ( pl_w->atck_level << 1 ) + 53 ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 22, 2 ) ;
		ew->disp_flag = 0 ;
	} else {
		ew->prio = pl_w->prio ;
		if( ew->timer1 > 0 ) ew->timer1 -- ;
		else if( !ew->timer1 ){
			ew->timer1 = -1 ;
			if( ew->sub_type == SHL29_00_LW( ew )->se_no ) pl_se_req( ew, 59 ) ;
		}
		if( ew->timer0 ) ew->timer0 -- ;
		else {
			ew->disp_flag = 1 ;
			if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
			else hit_work_set( ew ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl29_00.c" End of File	*/
