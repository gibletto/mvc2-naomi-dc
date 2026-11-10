/* Blackheart's effect 00 (character 35's shot 00): a puff of his Heart of Darkness that appears
   beside him at a random place, drifts off with a random speed and ends when it reaches the ground. */
#include "player.h"
#include "pl_char.h"
#include "pl35_def.h"

static Sint32 shl35_00_no ;		/* the table entry picked */
static Float shl35_00_spd_x ;
static Float shl35_00_acc_x ;

static void shl35_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl35_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl35_00_main ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3500 ;
	}
	return ew ;
}

static void shl35_00_dummy( PLAYER_WORK *ew );
static void shl35_00_init( PLAYER_WORK *ew );
static void shl35_00_move( PLAYER_WORK *ew );
static void shl35_00_end( PLAYER_WORK *ew );
static void shl35_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_00_main( PLAYER_WORK *ew )
{
	static void ( *shl35_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_00_init, shl35_00_move, shl35_00_end, shl35_00_free,
	} ;

	shl35_00_dummy( ew ) ;
	shl35_00_jmp[ew->routine_0]( ew ) ;
}

static void shl35_00_copy( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_00_init( PLAYER_WORK *ew )
{
	static Float shl35_00_x_tbl[16] = {
		-320.0f, -DOT_X( 160.0f ), -DOT_X( 128.0f ), -160.0f,
		-DOT_X( 64.0f ), -80.0f, -DOT_X( 32.0f ), -DOT_X( 16.0f ),
		80.0f, DOT_X( 32.0f ), 40.0f, DOT_X( 16.0f ),
		DOT_X( 112.0f ), 160.0f, DOT_X( 80.0f ), DOT_X( 64.0f ),
	} ;
	static Float shl35_00_spd_tbl[8][4] = {
		{ 15.0f, 0.0f, -DOT_Y( 12.0f ), -DOT_Y( 0.75f ) },
		{ 5.0f, 0.0f, -DOT_Y( 4.5f ), -DOT_Y( 0.25f ) },
		{ DOT_X( 2.5f ), 0.0f, -DOT_Y( 4.0f ), -DOT_Y( 0.25f ) },
		{ DOT_X( 2.5f ), 0.0f, -DOT_Y( 4.5f ), -DOT_Y( 0.25f ) },
		{ DOT_X( 2.0f ), 0.0f, -DOT_Y( 3.0f ), -DOT_Y( 0.25f ) },
		{ DOT_X( 2.0f ), 0.0f, -DOT_Y( 2.5f ), -DOT_Y( 0.25f ) },
		{ 2.5f, 0.0f, -DOT_Y( 2.0f ), -DOT_Y( 0.25f ) },
		{ 10.0f, 0.0f, -DOT_Y( 9.0f ), -1.2053571f },
	} ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	shl35_00_copy( ew ) ;
	pl_w->sub_type = 1 ;
	shl35_00_no = Rnd() & 15 ;
	shl35_00_acc_x = shl35_00_x_tbl[shl35_00_no] ;
	if( !pl_w->pl_dir ) shl35_00_acc_x = -shl35_00_acc_x ;
	ew->pos.x += shl35_00_acc_x ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 32.0f ) ;
	shl35_00_no &= 7 ;
	shl35_00_spd_x = shl35_00_spd_tbl[shl35_00_no][0] ;
	shl35_00_acc_x = shl35_00_spd_tbl[shl35_00_no][1] ;
	ew->spd.y = shl35_00_spd_tbl[shl35_00_no][2] ;
	ew->acc.y = shl35_00_spd_tbl[shl35_00_no][3] ;
	if( !pl_w->pl_dir ){
		shl35_00_spd_x = -shl35_00_spd_x ;
		shl35_00_acc_x = -shl35_00_acc_x ;
	}
	ew->spd.x = shl35_00_spd_x ;
	ew->acc.x = shl35_00_acc_x ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl35_at_heart ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 22, 3 ) ;
}

/*--------------------------------------------------------------*/
static void shl35_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_w = ew->master_w ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_0 ++ ;
		ew->pos.y = pl_w->ground_y ;
		burst_set( ew, &ew->pos, 1, 8 ) ;
	}
	else{
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl35_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl35_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl35_00_copy( PLAYER_WORK *ew )
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
static void shl35_00_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl35_00.c" End of File	*/
