/* Marrow's effect 02 (character 16's shot 02): four pictures that keep step with her own
   animation, by the code in the frames' cg_rival (types 0 and 1 beside her, 2 and 3 above). */
#include "player.h"
#include "pl_char.h"
#include "pl16_def.h"

static void shl16_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl16_02_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl16_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1602 ;
		ew->type = type ;
	}
	return ew ;
}

static void shl16_02_start( PLAYER_WORK *ew );
static void shl16_02_move( PLAYER_WORK *ew );
static void shl16_02_end( PLAYER_WORK *ew );
static void shl16_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl16_02_main( PLAYER_WORK *ew )
{
	static void ( *shl16_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl16_02_start, shl16_02_move, shl16_02_end, shl16_02_free,
	} ;

	shl16_02_jmp[ew->routine_0]( ew ) ;
}

static void shl16_02_init_0( PLAYER_WORK *ew );
static void shl16_02_init_1( PLAYER_WORK *ew );
static void shl16_02_init_2( PLAYER_WORK *ew );
static void shl16_02_init_3( PLAYER_WORK *ew );

/*==============================================================*/
static void shl16_02_start( PLAYER_WORK *ew )
{
	static void ( *shl16_02_start_jmp[] )( PLAYER_WORK *ew ) = {
		shl16_02_init_0, shl16_02_init_1, shl16_02_init_2, shl16_02_init_3,
	} ;

	shl16_02_start_jmp[ew->type]( ew ) ;
}

static void shl16_02_move_0( PLAYER_WORK *ew );
static void shl16_02_move_1( PLAYER_WORK *ew );
static void shl16_02_move_2( PLAYER_WORK *ew );
static void shl16_02_move_3( PLAYER_WORK *ew );

/*==============================================================*/
static void shl16_02_init_0( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	shl16_02_move_0( ew ) ;
}

/*==============================================================*/
static void shl16_02_init_1( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	shl16_02_move_1( ew ) ;
}

/*==============================================================*/
static void shl16_02_init_2( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 256.0f ) ;
	shl16_02_move_2( ew ) ;
}

/*==============================================================*/
static void shl16_02_init_3( PLAYER_WORK *ew )
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
	ew->prio = 10 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 256.0f ) ;
	shl16_02_move_3( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl16_02_move( PLAYER_WORK *ew )
{
	static void ( *shl16_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl16_02_move_0, shl16_02_move_1, shl16_02_move_2, shl16_02_move_3,
	} ;

	shl16_02_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl16_02_move_0( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pl_char_set( ew, 21, 5 ) ;
	for( ;; ){
		if( HI_BYTE( ew->master_w->cg_rival ) == HI_BYTE( ew->cg_rival ) ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( pl_char_move( ew ) < 0 ){
			shl16_02_end( ew ) ;
			break ;
		}
	}
}

/*==============================================================*/
static void shl16_02_move_1( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pl_char_set( ew, 21, 6 ) ;
	for( ;; ){
		if( HI_BYTE( ew->master_w->cg_rival ) == HI_BYTE( ew->cg_rival ) ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( pl_char_move( ew ) < 0 ){
			shl16_02_end( ew ) ;
			break ;
		}
	}
}

/*==============================================================*/
static void shl16_02_move_2( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 256.0f ) ;
	pl_char_set( ew, 21, 7 ) ;
	for( ;; ){
		if( HI_BYTE( ew->master_w->cg_rival ) == HI_BYTE( ew->cg_rival ) ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( pl_char_move( ew ) < 0 ){
			shl16_02_end( ew ) ;
			break ;
		}
	}
}

/*==============================================================*/
static void shl16_02_move_3( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 256.0f ) ;
	pl_char_set( ew, 21, 8 ) ;
	for( ;; ){
		if( HI_BYTE( ew->master_w->cg_rival ) == HI_BYTE( ew->cg_rival ) ){
			ew->disp_flag = 1 ;
			break ;
		}
		if( pl_char_move( ew ) < 0 ){
			shl16_02_end( ew ) ;
			break ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl16_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	shl16_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl16_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl16_02.c" End of File	*/
