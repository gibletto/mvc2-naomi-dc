/* Hulk's Gamma Slam and Gamma Wave (character 0d's shot 01): the wave that runs along the ground
   (type 0 for Gamma Slam, type 2 for Gamma Wave) and throws up a column of rock every few frames
   (types 1 and 3). */
#include "player.h"
#include "pl_char.h"
#include "pl0d_def.h"

static void shl0d_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0d_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0d_01_main ;
		ew->ef_no = 0x0d01 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			A column, which rides on the wave that threw it		*/
/*			up													*/
/*==============================================================*/
static void *shl0d_01_part_set( PLAYER_WORK *wave_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( wave_w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl0d_01_main ;
		ew->ef_no = 0x0d01 ;
		ew->master_w = wave_w->master_w ;
		ew->ef_w = wave_w ;
		ew->pl_type = wave_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl0d_01_slam_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_slam_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_wave_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_wave_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_01_main( PLAYER_WORK *ew )
{
	static void ( *shl0d_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_slam_main, shl0d_01_slam_rock_main, shl0d_01_wave_main, shl0d_01_wave_rock_main,
	} ;

	shl0d_01_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0d_01_slam_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_slam_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_01_slam_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_slam_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_slam_init, shl0d_01_slam_move, shl0d_01_end, shl0d_01_free,
	} ;

	shl0d_01_slam_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_slam_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( !ew->disp_dir ){
		ew->pos.x -= 80.0f ;
		ew->spd.x = -10.0f ;
	} else {
		ew->pos.x += 80.0f ;
		ew->spd.x = 10.0f ;
	}
	ew->timer0 = 1 ;
	ew->timer1 = !ew->atck_level ? 24 : 48 ;
	shl0d_01_slam_move( ew, pl_w ) ;
}

static void shl0d_01_slam_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_01_slam_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_slam_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_slam_run,
	} ;

	shl0d_01_slam_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_slam_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !-- ew->timer0 ){
		ew->timer0 = 12 ;
		shl0d_01_part_set( ew, 1, 0 ) ;
	}
	if( !-- ew->timer1 ){
		ew->routine_0 ++ ;
		shl0d_01_end( ew, pl_w ) ;
	}
}

static void shl0d_01_slam_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_slam_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_01_slam_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_slam_rock_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_slam_rock_init, shl0d_01_slam_rock_move, shl0d_01_end, shl0d_01_free,
	} ;

	shl0d_01_slam_rock_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_slam_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wave_w = ew->ef_w ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->atck_no = pl0d_at_slam ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = wave_w->pos.x ;
	ew->pos.y = wave_w->pos.y ;
	ew->pos.z = wave_w->pos.z ;
	ew->spd.y = DOT_Y( 5.0f ) ;
	ew->acc.y = -0.6696428f ;
	pl_char_set( ew, 21, 28 ) ;
	shl0d_01_slam_rock_move( ew, pl_w ) ;
}

static void shl0d_01_slam_rock_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_slam_rock_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_01_slam_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_slam_rock_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_slam_rock_rise, shl0d_01_slam_rock_fall,
	} ;

	ew->prio = pl_w->prio ;
	shl0d_01_slam_rock_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_slam_rock_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		ew->spd.y = -DOT_Y( 0.25f ) ;
		ew->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void shl0d_01_slam_rock_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl0d_01_end( ew, pl_w ) ;
	}
}

static void shl0d_01_wave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_wave_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_01_wave_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_wave_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_wave_init, shl0d_01_wave_move, shl0d_01_end, shl0d_01_free,
	} ;

	shl0d_01_wave_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_wave_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->size_front = ew->size_back = 53 ;
	if( !ew->disp_dir ){
		ew->pos.x -= 80.0f ;
		ew->spd.x = -15.0f ;
	} else {
		ew->pos.x += 80.0f ;
		ew->spd.x = 15.0f ;
	}
	ew->timer0 = 1 ;
	shl0d_01_wave_move( ew, pl_w ) ;
}

static void shl0d_01_wave_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_01_wave_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_wave_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_wave_run,
	} ;

	shl0d_01_wave_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_wave_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !-- ew->timer0 ){
		ew->timer0 = 10 ;
		shl0d_01_part_set( ew, 3, 0 ) ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		shl0d_01_end( ew, pl_w ) ;
	}
}

static void shl0d_01_wave_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_wave_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_01_wave_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_wave_rock_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_wave_rock_init, shl0d_01_wave_rock_move, shl0d_01_end, shl0d_01_free,
	} ;

	shl0d_01_wave_rock_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_wave_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wave_w = ew->ef_w ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->atck_no = pl0d_at_wave ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = wave_w->pos.x ;
	ew->pos.y = wave_w->pos.y ;
	ew->pos.z = wave_w->pos.z ;
	ew->spd.y = DOT_Y( 12.0f ) ;
	ew->acc.y = -DOT_Y( 0.625f ) ;
	pl_char_set( ew, 22, 11 ) ;
	shl0d_01_wave_rock_move( ew, pl_w ) ;
}

static void shl0d_01_wave_rock_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_01_wave_rock_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_01_wave_rock_move( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_01_wave_rock_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_01_wave_rock_rise, shl0d_01_wave_rock_fall,
	} ;

	shl0d_01_wave_rock_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_01_wave_rock_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		ew->spd.y = -2.142857f ;
		ew->acc.y = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void shl0d_01_wave_rock_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl0d_01_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl0d_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0d_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0d_01.c" End of File	*/
