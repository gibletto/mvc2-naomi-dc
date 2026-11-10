/* Marrow's player effect 08: the big piece that flies in from the side of the screen to her place,
   growing and turning, and shakes the screen where it lands. */
#include "player.h"
#include "ef_char.h"
#include "pl16_def.h"

static void pls16_08_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls16_08_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls16_08_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1608 ;
	}
	return ew ;
}

static void pls16_08_init( PLAYER_WORK *ew );
static void pls16_08_move( PLAYER_WORK *ew );
static void pls16_08_end( PLAYER_WORK *ew );
static void pls16_08_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls16_08_main( PLAYER_WORK *ew )
{
	static void ( *pls16_08_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_08_init, pls16_08_move, pls16_08_end, pls16_08_free,
	} ;

	pls16_08_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls16_08_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 80.0f ) ;
	ew->scale.x = 0.05f ;
	ew->scale.y = 0.1f ;
	ew->spd.x = ( scr_work.eye[0].x - ew->pos.x ) / 16.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 0 ;
	ew->rot[2] = 0 ;
	ef_char_set( ew, 27, 16 ) ;
}

static void pls16_08_fly( PLAYER_WORK *ew );
static void pls16_08_stay( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls16_08_move( PLAYER_WORK *ew )
{
	static void ( *pls16_08_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls16_08_fly, pls16_08_stay,
	} ;

	pls16_08_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls16_08_fly( PLAYER_WORK *ew )
{
	Float t ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->rot[2] += 0xc0 ;
	ew->scale.x += 0.032f ;
	t = ew->timer0 * 0.16f ;
	t = t * t ;
	ew->scale.y += t ;
	ew->timer0 ++ ;
	if( ew->timer0 > 16 ){
		ew->routine_1 ++ ;
		ew->scale.x = 1.5f ;
		ew->scale.y = 1.5f ;
		ew->rot[2] = 0 ;
		ew->pos.x = ew->disp_dir ? scr_work.eye[0].x + 600.0f : scr_work.eye[0].x + -600.0f ;
		ew->pos.y = scr_work.center_y + 360.0f ;
		ef_char_set( ew, 27, 17 ) ;
		pl_voice_req( ew, 45 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls16_08_stay( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) ;
}

/*--------------------------------------------------------------*/
static void pls16_08_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls16_08_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls16_08_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls16_08.c" End of File	*/
