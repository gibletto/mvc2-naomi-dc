/* Silver Samurai's effect 01 (character 2f's shot 01): a piece that stays on his place and
   shows the animation he started it with, until he changes it. */
#include "player.h"
#include "pl_char.h"
#include "pl2f_def.h"

/* the effect's own work */
typedef struct SHL2F_01_LOCAL_W {
	Sint32	char_no;	/* 000: the master's animation when it was set */
} SHL2F_01_LOCAL_W;
#define SHL2F_01_LW( w )	( ( SHL2F_01_LOCAL_W * )&( w )->obj_flag )

static void shl2f_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2f_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f01 ;
		SHL2F_01_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl2f_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2f_01_main( PLAYER_WORK *ew )
{
	static void ( *shl2f_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2f_01_init, shl2f_01_move, shl2f_01_end, shl2f_01_free,
	} ;

	shl2f_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2f_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->sub_prio = -1 ;
	pl_char_set( ew, 21, 13 ) ;
	shl2f_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->prio = pl_w->prio ;
	ew->pos = pl_w->pos ;
	if( SHL2F_01_LW( ew )->char_no != pl_w->char_no.w ) shl2f_01_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void shl2f_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2f_01.c" End of File	*/
