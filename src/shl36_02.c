/* Thanos's effect 02 (character 36's shot 02): the pieces of rubble that fly to him (type 0), the
   ones that tumble and bounce off the floor (type 1) and the one that comes with them (type 2). */
#include "player.h"
#include "pl_char.h"
#include "pl36_def.h"

static void shl36_02_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A pair, one flying each way: type is the high		*/
/*			byte of no, sub_type the low						*/
/****************************************************************/
PLAYER_WORK *shl36_02_set( PLAYER_WORK *pl_w, Uint16 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl36_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3602 ;
		ew->type = no >> 8 ;
		ew->sub_type = no ;
		ew->dir = 0 ;
	}
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl36_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3602 ;
		ew->type = no >> 8 ;
		ew->sub_type = no ;
		ew->dir = 4 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl36_02_set1( PLAYER_WORK *pl_w, Uint16 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl36_02_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3602 ;
		ew->type = no >> 8 ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl36_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_02_main( PLAYER_WORK *ew )
{
	static void ( *shl36_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_02_init, shl36_02_move, shl36_02_end, shl36_02_free,
	} ;

	shl36_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl36_02_fly_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_fall_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_init_2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_02_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_02_fly_init, shl36_02_fall_init, shl36_02_init_2,
	} ;
	static Sint16 shl36_02_atck_no[4] = {
		62, 63, 64, 65,
	} ;

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
	ew->obj_flag = pl_w->master_w->ground_y ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = shl36_02_atck_no[ew->sub_type] ;
	HIT_RESET( ew ) ;
	shl36_02_init_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			A piece that flies to him from a place about		*/
/*			him, on the floor of the one who made it			*/
/*==============================================================*/
static void shl36_02_fly_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;

	ew->prio = 0 ;
	x = ( Rnd() & 255 ) * 3 / 4 ;
	x -= 96 ;
	ew->pos.x = pl_w->pos.x + DOT_X( x ) ;
	ew->pos.y = pl_w->master_w->ground_y ;
	ew->spd.x = ( pl_w->pos.x - ew->pos.x ) / 24.0f ;
	ew->spd.y = ( pl_w->pos.y - ew->pos.y ) / 24.0f ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->timer0 = 24 ;
	pl_char_set( ew, 22, ew->dir + 59 + ( Rnd() & 3 ) ) ;
	shl36_02_set1( ew, 0x200 ) ;
}

/*==============================================================*/
static void shl36_02_fall_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float px, py ;
	Sint16 x ;

	ew->disp_flag = 2 ;
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	x = ( Rnd() & 255 ) * 3 / 4 ;
	x -= 96 ;
	px = pl_w->pos.x + DOT_X( x ) ;
	py = pl_w->master_w->ground_y ;
	ew->spd.x = ( px - ew->pos.x ) / 12.0f ;
	ew->spd.y = ( py - ew->pos.y ) / 12.0f ;
	ew->acc.x = ew->acc.y = 0.0f ;
	pl_char_set( ew, 22, ew->dir + 59 + ( Rnd() & 3 ) ) ;
}

/*==============================================================*/
static void shl36_02_init_2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	pl_char_set( ew, 22, 58 ) ;
}

static void shl36_02_fly_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_fall_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_02_move_2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl36_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_02_fly_move, shl36_02_fall_move, shl36_02_move_2,
	} ;

	shl36_02_move_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It flies to him until its time is up, or he has		*/
/*			done with the move									*/
/*==============================================================*/
static void shl36_02_fly_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ) ew->routine_0 ++ ;
	if( pl_w->routine_1 || !pl_w->be_flag ){
		ew->type = 1 ;
		ew->routine_3 = 0 ;
		ew->spd.x = ew->acc.x = ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			It falls, and bounces once off the floor before		*/
/*			it is gone											*/
/*==============================================================*/
static void shl36_02_fall_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->obj_flag > ew->pos.y ){
		if( !ew->routine_3 ++ ){
			ew->disp_flag &= 1 ;
			ew->pos.y = ew->obj_flag ;
			x = -( Rnd() & 48 ) - 16 ;
			ew->spd.y = DOT_Y( 1.5f ) ;
			ew->acc.y = x * 2.142857f / 256.0f ;
		}
		else {
			ew->routine_0 ++ ;
		}
	}
	ew->disp_flag ^= 1 ;
	if( !ew->routine_3 ) hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl36_02_move_2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl36_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl36_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl36_02.c" End of File	*/
