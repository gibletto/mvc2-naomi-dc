/* Storm's effect 00 (character 2a's shot 00): eight pieces set out in a row in front of her,
   each waking a little after the one before and then striking. */
#include "player.h"
#include "pl_char.h"

/* the effect's own work, from obj_flag */
typedef struct SHL2A_00_LOCAL_W {
	Sint32	char_no;	/* 000: the animation Storm was in when it was made */
} SHL2A_00_LOCAL_W;
#define SHL2A_00_LW( w )	( ( SHL2A_00_LOCAL_W * )&( w )->obj_flag )

static void shl2a_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2a_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 i ;

	for( i = 7 ; i >= 0 ; i -- ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2a_00_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->sub_type = i ;
			ew->ef_no = 0x2a00 ;
			SHL2A_00_LW( ew )->char_no = pl_w->char_no.w ;
		}
	}
	return ew ;
}

static void shl2a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2a_00_main( PLAYER_WORK *ew )
{
	static void ( *shl2a_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2a_00_init, shl2a_00_move, shl2a_00_end, shl2a_00_free,
	} ;

	shl2a_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 ofs ;

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
	ofs = ew->sub_type * 64 + 88 ;
	if( pl_w->pl_dir ) ofs = -ofs ;
	ew->pos.x = pl_w->pos.x - ofs * 1.6666666f ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = 8 ;
	ew->timer0 = ew->sub_type * 8 + 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->atck_level + 48 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, 26 ) ;
	shl2a_00_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != ew->master_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !ew->routine_2 ){
		if( ! -- ew->timer0 ){
			ew->routine_2 ++ ;
			ew->timer0 = 28 ;
		}
	} else {
		ew->disp_flag = 1 ;
		if( ew->hit_box_no || pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2a_00.c" End of File	*/
