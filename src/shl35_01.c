/* Blackheart's effect 01 (character 35's shot 01): the fiend that follows his attack. It stays
   beside him for as long as his animation lasts, strikes, and hands the same on to a copy of
   itself each time its animation passes a frame. */
#include "player.h"
#include "pl_char_old.h"
#include "pl35_def.h"

static Float shl35_01_x ;
static Uint8 shl35_01_unused[4] ;

static void shl35_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl35_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl35_01_main ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3501 ;
		ew->type = 0 ;
		ew->timer0 = pl_w->timer0 - 16 ;
		ew->timer1 = pl_w->char_no.b.no ;
	}
	return ew ;
}

/*==============================================================*/
/*			A second one, from the first: it is a piece of		*/
/*			the same attack										*/
/*==============================================================*/
static PLAYER_WORK *shl35_01_add( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl35_01_main ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3501 ;
		ew->type = 0xff ;
		ew->timer0 = pl_w->timer0 ;
		ew->timer1 = pl_w->timer1 ;
	}
	return ew ;
}

static void shl35_01_dummy( PLAYER_WORK *ew );
static void shl35_01_init( PLAYER_WORK *ew );
static void shl35_01_move( PLAYER_WORK *ew );
static void shl35_01_end( PLAYER_WORK *ew );
static void shl35_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_01_main( PLAYER_WORK *ew )
{
	static void ( *shl35_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_01_init, shl35_01_move, shl35_01_end, shl35_01_free,
	} ;

	shl35_01_dummy( ew ) ;
	shl35_01_jmp[ew->routine_0]( ew ) ;
}

static void shl35_01_copy( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_01_init( PLAYER_WORK *ew )
{
	static Float shl35_01_pos_tbl[][2] = {
		{ 480.0f, -195.0f }, { DOT_X( 185.0f ), DOT_Y( 137.0f ) },
	} ;
	PLAYER_WORK *pl_w ;
	Float *xp = &shl35_01_x ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	shl35_01_copy( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( !ew->type ){
		if( pl_w->team_type == 3 ){
			ew->atck_no = pl35_at_thunder_counter ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl_w->atck_level * 2 + 52 ;
			HIT_RESET( ew ) ;
		}
		*xp = -DOT_X( 49.0f ) ;
		if( !pl_w->pl_dir ) *xp = -*xp ;
		ew->pos.x += *xp ;
		ew->pos.y += DOT_Y( 102.0f ) ;
		pl_char_set( ew, 21, pl_w->atck_level * 2 + 3 ) ;
	}
	else{
		if( pl_w->team_type == 3 ){
			ew->atck_no = pl35_at_thunder_bolt_counter ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl35_at_thunder_bolt ;
			HIT_RESET( ew ) ;
		}
		*xp = shl35_01_pos_tbl[pl_w->atck_level][0] ;
		ew->pos.y += shl35_01_pos_tbl[pl_w->atck_level][1] ;
		pl_char_set( ew, 21, 9 ) ;
	}
}

static void shl35_01_move_keep( PLAYER_WORK *ew );
static void shl35_01_move_end( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl35_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	static void ( *shl35_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_01_move_keep, shl35_01_move_end,
	} ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_w = ew->master_w ;
	if( ew->timer1 != pl_w->char_no.b.no ){
		shl35_01_end( ew ) ;
		return ;
	}
	shl35_01_move_jmp[ew->routine_1]( ew ) ;
}

static void shl35_01_pos_set( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_01_move_keep( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Uint8 no ;

	shl35_01_pos_set( ew ) ;
	pl_char_move( ew ) ;
	pl_w = ew->master_w ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		if( !ew->type ) no = pl_w->atck_level * 2 + 6 ;
		else no = 7 ;
		pl_char_set( ew, 21, no ) ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		shl35_01_add( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl35_01_move_end( PLAYER_WORK *ew )
{
	shl35_01_pos_set( ew ) ;
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl35_01_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl35_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl35_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl35_01_copy( PLAYER_WORK *ew )
{
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
	ew->size_top = ew->size_bottom = 32 ;
	ew->size_front = ew->size_back = 36 ;
	ew->prio = 0 ;
}

/*==============================================================*/
static void shl35_01_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*			The first beside its master, its copies on the		*/
/*			one before them										*/
/*==============================================================*/
static void shl35_01_pos_set( PLAYER_WORK *ew )
{
	static Float shl35_01_ofs_tbl[][2] = {
		{ 480.0f, -195.0f }, { DOT_X( 185.0f ), DOT_Y( 137.0f ) },
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	if( !ew->type ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 102.0f ) ;
		x = DOT_X( 49.0f ) ;
		if( !pl_w->pl_dir ) x = -DOT_X( 49.0f ) ;
	}
	else{
		PLAYER_WORK *bw = ew->ef_w ;

		ew->pos.x = bw->pos.x ;
		ew->pos.y = bw->pos.y + shl35_01_ofs_tbl[pl_w->atck_level][1] ;
		x = shl35_01_ofs_tbl[pl_w->atck_level][0] ;
		if( !pl_w->pl_dir ) x = -x ;
	}
	ew->pos.x += x ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl35_01.c" End of File	*/
