/* Abyss's third form's player effect 00, by type: a piece at his place as he leaps for the stomp, which
   plays to its mark and fades (type 0), the spark where a hit lands on him (type 1), the two pieces on
   the ground where he comes down (type 2), the pieces that grow and fade over him while he is sunk
   away (type 3), and one that widens and fades, which nothing sets (type 4). */
#include "player.h"
#include "ef_char.h"
#include "pl1a_def.h"

/* how far the mark's size changes each frame, in the effect's own work */
typedef struct PLS1A_00_LOCAL_W {
	Float	scale_x ;
	Float	scale_y ;
} PLS1A_00_LOCAL_W ;
#define PLS1A_00_LW( w )	( ( PLS1A_00_LOCAL_W * )&( w )->obj_mtx )

static void pls1a_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1a_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1a_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x1a00 ;
		ew->timer1 = pl_w->char_no.w ;
	}
	return ew ;
}

/****************************************************************/
/*			A spark where a hit lands, at most six at a time	*/
/****************************************************************/
void pls1a_00_set2( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( PL1A_LW( pl_w )->spark_cnt > 5 ) return ;
	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls1a_00_main ;
		ew->master_w = pl_w ;
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
		ew->alpha = 1.0f ;
		ew->col_flag = -1 ;
		ew->col_no = col_no_tbl[pl_w->work_no] ;
		ew->disp_flag = 0 ;
		ew->type = 1 ;
		ew->sub_type = type ;
		ew->pos = *pos ;
		ew->ef_no = 0x1a00 ;
		PL1A_LW( pl_w )->spark_cnt ++ ;
	}
}

static void pls1a_00_t0( PLAYER_WORK *ew );
static void pls1a_00_spark( PLAYER_WORK *ew );
static void pls1a_00_land( PLAYER_WORK *ew );
static void pls1a_00_sink( PLAYER_WORK *ew );
static void pls1a_00_t4( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1a_00_main( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_t0, pls1a_00_spark, pls1a_00_land, pls1a_00_sink, pls1a_00_t4,
	} ;

	pls1a_00_jmp[ew->type]( ew ) ;
}

static void pls1a_00_t0_init( PLAYER_WORK *ew );
static void pls1a_00_t0_move( PLAYER_WORK *ew );
static void pls1a_00_end( PLAYER_WORK *ew );
static void pls1a_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1a_00_t0( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_t0_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_t0_init, pls1a_00_t0_move, pls1a_00_end, pls1a_00_free,
	} ;

	pls1a_00_t0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1a_00_t0_init( PLAYER_WORK *ew )
{
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
	ew->prio = 7 ;
	ew->pos = ew->master_w->pos ;
	pls1a_00_t0_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1a_00_t0_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	ew->alpha = 1.0f ;
	if( ( Uint16 )ew->timer1 != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->disp_flag = 1 ;
	switch( ew->routine_1 ){
	case 0 :
		ew->routine_1 ++ ;
		ef_char_set( ew, 27, 0 ) ;
		break ;
	case 1 :
		ef_char_move( ew ) ;
		if( ew->cg_type ){
			ew->routine_1 ++ ;
			ew->spd.x = 1.0f ;
			ew->acc.x = 0.0625f ;
		}
		break ;
	case 2 :
		ew->spd.x -= ew->acc.x ;
		if( ew->spd.x < 0 ){
			ew->routine_0 ++ ;
			ew->alpha = 1.0f ;
			ew->disp_flag = 0 ;
		}
		ew->alpha = ew->spd.x ;
		break ;
	}
}

static void pls1a_00_spark_init( PLAYER_WORK *ew );
static void pls1a_00_spark_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1a_00_spark( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_spark_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_spark_init, pls1a_00_spark_move, pls1a_00_end, pls1a_00_free,
	} ;

	pls1a_00_spark_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1a_00_spark_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->routine_0 ++ ;
	if( !ew->sub_type ){
		ew->scale.x *= 0.8f ;
		ew->scale.y *= 0.8f ;
	} else if( ew->sub_type == 1 ){
		ew->scale.x *= 1.0f ;
		ew->scale.y *= 1.0f ;
	} else {
		ew->scale.x *= 1.2f ;
		ew->scale.y *= 1.2f ;
	}
	ew->prio = 8 ;
	ef_char_set( ew, 27, 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls1a_00_spark_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls1a_00_land_init( PLAYER_WORK *ew );
static void pls1a_00_land_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1a_00_land( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_land_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_land_init, pls1a_00_land_move, pls1a_00_end, pls1a_00_free,
	} ;

	pls1a_00_land_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1a_00_land_init( PLAYER_WORK *ew )
{
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
	ew->prio = 0 ;
	ef_char_set( ew, 27, 5 ) ;
	if( ew->sub_type ) ew->disp_dir ^= 1 ;
	ew->scale.x *= 0.8f ;
	ew->scale.y *= 0.8f ;
	pls1a_00_land_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1a_00_land_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	} else {
		ew->pos = ew->master_w->pos ;
		ew->pos.y = ew->master_w->ground_y ;
	}
}

static void pls1a_00_sink_init( PLAYER_WORK *ew );
static void pls1a_00_sink_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1a_00_sink( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_sink_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_sink_init, pls1a_00_sink_move, pls1a_00_end, pls1a_00_free,
	} ;

	pls1a_00_sink_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1a_00_sink_init( PLAYER_WORK *ew )
{
	Float x ;

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
	ew->prio = 0 ;
	ef_char_set( ew, 27, 4 ) ;
	ew->pos = ew->master_w->pos ;
	x = -40.0f ;
	if( ew->disp_dir ) x = 40.0f ;
	ew->pos.x += x * ew->scale.x ;
	ew->pos.y += DOT_Y( 96.0f ) * ew->scale.y ;
	ew->acc = ew->scale ;
	ew->scale.x *= 0.4f ;
	ew->scale.y *= 0.4f ;
	PLS1A_00_LW( ew )->scale_x = ( ew->acc.x * 1.5f - ew->scale.x ) / 20.0f ;
	PLS1A_00_LW( ew )->scale_y = ( ew->acc.y * 1.5f - ew->scale.y ) / 20.0f ;
	ew->timer1 = 20 ;
	ew->timer0 = 30 ;
	ew->rot[2] = 0 ;
	ew->timer0 = 240 ;
	pls1a_00_sink_move( ew ) ;
}

static void pls1a_00_sink_grow( PLAYER_WORK *ew );
static void pls1a_00_sink_flat( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls1a_00_sink_move( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_sink_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_sink_grow, pls1a_00_sink_flat,
	} ;

	pls1a_00_sink_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls1a_00_sink_grow( PLAYER_WORK *ew )
{
	ew->scale.x += PLS1A_00_LW( ew )->scale_x ;
	ew->scale.y += PLS1A_00_LW( ew )->scale_y ;
	ew->alpha -= 0.04f ;
	if( -- ew->timer1 <= 0 ){
		ew->routine_0 ++ ;
		ew->timer1 = 19 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1a_00_sink_flat( PLAYER_WORK *ew )
{
	ew->scale.x *= 1.5f ;
	ew->scale.y *= 0.9f ;
	ew->alpha -= 0.032f ;
	if( -- ew->timer1 <= 0 ) ew->routine_0 ++ ;
}

static void pls1a_00_t4_init( PLAYER_WORK *ew );
static void pls1a_00_t4_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1a_00_t4( PLAYER_WORK *ew )
{
	static void ( *pls1a_00_t4_jmp[] )( PLAYER_WORK *ew ) = {
		pls1a_00_t4_init, pls1a_00_t4_move, pls1a_00_end, pls1a_00_free,
	} ;

	pls1a_00_t4_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1a_00_t4_init( PLAYER_WORK *ew )
{
	Float x ;

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
	ew->prio = 0 ;
	ef_char_set( ew, 27, 4 ) ;
	ew->pos = ew->master_w->pos ;
	x = -40.0f ;
	if( ew->disp_dir ) x = 40.0f ;
	ew->pos.x += x * ew->scale.x ;
	ew->pos.y += DOT_Y( 96.0f ) * ew->scale.y ;
	ew->acc = ew->scale ;
	ew->scale.x *= 0.015f ;
	ew->scale.y *= 0.015f ;
	ew->timer0 = 30 ;
	PLS1A_00_LW( ew )->scale_x = ( ew->acc.x * 10.0f - ew->scale.x ) / 30.0f ;
	pls1a_00_t4_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1a_00_t4_move( PLAYER_WORK *ew )
{
	ew->scale.x += PLS1A_00_LW( ew )->scale_x ;
	ew->scale.y *= 0.75f ;
	ew->alpha -= 0.033333335f ;
	if( -- ew->timer0 < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls1a_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	ew->alpha = 1.0f ;
}

/*--------------------------------------------------------------*/
static void pls1a_00_free( PLAYER_WORK *ew )
{
	if( ew->type == 1 ){
		PLAYER_WORK *pl_w = ew->master_w ;

		PL1A_LW( pl_w )->spark_cnt -- ;
	}
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1a_00.c" End of File	*/
