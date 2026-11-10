/* Silver Samurai's effect 03 (character 2f's shot 03): a chain of five after-images. Each one
   shows the animation of the work it copies and, when its first frames are gone, sets the next. */
#include "player.h"
#include "pl_char.h"
#include "pl2f_def.h"

/* the effect's own work */
typedef struct SHL2F_03_LOCAL_W {
	PLAYER_WORK	*src_w;	/* 000: the work whose picture it copies */
} SHL2F_03_LOCAL_W;
#define SHL2F_03_LW( w )	( ( SHL2F_03_LOCAL_W * )&( w )->obj_flag )

static void shl2f_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2f_03_set( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_03_main ;
		ew->master_w = pl_w ;
		SHL2F_03_LW( ew )->src_w = em ;
		ew->ef_no = 0x2f03 ;
		if( !pl_w->work_kind ) ew->type = 0 ;
		else ew->type = pl_w->type + 1 ;
	}
	return ew ;
}

static void shl2f_03_init( PLAYER_WORK *ew );
static void shl2f_03_move( PLAYER_WORK *ew );
static void shl2f_03_end( PLAYER_WORK *ew );
static void shl2f_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl2f_03_main( PLAYER_WORK *ew )
{
	static void ( *shl2f_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl2f_03_init, shl2f_03_move, shl2f_03_end, shl2f_03_free,
	} ;

	shl2f_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl2f_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *src_w = SHL2F_03_LW( ew )->src_w ;
	Sint16 ofs ;

	ew->routine_0 ++ ;
	ew->timer0 = 120 ;
	ew->timer1 = 8 ;
	CHAR_PART_COPY( ew, src_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = src_w->pl_no ;
	ew->pl_type = src_w->pl_type ;
	ew->scale.x = src_w->scale.x ;
	ew->scale.y = src_w->scale.y ;
	ew->atck_level = src_w->atck_level ;
	ew->work_no = src_w->work_no ;
	ew->col_cnt = src_w->col_cnt ;
	ew->scale = src_w->scale ;
	ew->prio = src_w->prio ;
	ew->sub_prio = -1 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	ofs = 32 ;
	if( ew->disp_dir ) ofs = -32 ;
	ew->pos.x = ew->master_w->pos.x + DOT_X( ofs ) ;
	ew->pos.y = src_w->ground_y ;
	pl_char_set( ew, 20, 4 ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_03_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	if( ew->timer1 ){
		if( !-- ew->timer1 && ew->type < 4 ) shl2f_03_set( ew, pl_w ) ;
	}
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		pl_char_set( ew, 20, 3 ) ;
	}
	else pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_03_end( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl2f_03_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2f_03.c" End of File	*/
