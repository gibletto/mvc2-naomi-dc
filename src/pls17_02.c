/* Cable's player effect 02: three copies of the effect he made, each at its own height, that show an animation of his. */
#include "player.h"
#include "ef_char.h"
#include "pl17_def.h"

/* the effect's own work, from obj_flag: the animation of his that it follows, read through one pointer
   set by the setter */
typedef struct PLS17_02_LOCAL_W {
	Uint16	char_no ;
} PLS17_02_LOCAL_W;
#define PLS17_02_LW( w )	( ( PLS17_02_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls17_02_char_no_p ;

static void pls17_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_02_set( PLAYER_WORK *pl_w, PLAYER_WORK *ef_w )
{
	PLAYER_WORK *ew ;
	Sint32 i = 0 ;

	do {
		if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
			ew->work_func = pls17_02_main ;
			ew->master_w = pl_w ;
			ew->ef_w = ef_w ;
			ew->disp_dir = ew->master_w->disp_dir ;
			ew->ef_no = 0x1700 ;
			ew->sub_no = i ;
			pls17_02_char_no_p = &PLS17_02_LW( ew )->char_no ;
			*pls17_02_char_no_p = ew->master_w->char_no.w ;
		}
		else return ;
		i ++ ;
	} while( i < 3 ) ;
	return ew ;
}

static void pls17_02_init( PLAYER_WORK *ew );
static void pls17_02_move( PLAYER_WORK *ew );
static void pls17_02_end( PLAYER_WORK *ew );
static void pls17_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls17_02_main( PLAYER_WORK *ew )
{
	static void ( *pls17_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls17_02_init, pls17_02_move, pls17_02_end, pls17_02_free,
	} ;

	pls17_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls17_02_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	CHAR_PART_COPY( ew, ew->ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->ef_w->pl_no ;
	ew->pl_type = ew->ef_w->pl_type ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->atck_level = ew->ef_w->atck_level ;
	ew->work_no = ew->ef_w->work_no ;
	ew->col_cnt = ew->ef_w->col_cnt ;
	ew->scale = ew->ef_w->scale ;
	ew->prio = ew->ef_w->prio ;
	switch( ew->sub_no ){
	case 0:
		ew->prio = 0 ;
		break ;
	case 1:
		ew->prio = 0 ;
		break ;
	case 2:
		ew->prio = 7 ;
		break ;
	}
	ew->pos = ew->ef_w->pos ;
	ef_char_set( ew, 27, ew->sub_no ) ;
}

/*--------------------------------------------------------------*/
static void pls17_02_move( PLAYER_WORK *ew )
{
	ew->pos = ew->ef_w->pos ;
	if( ew->pl_type != ew->master_w->pl_type ){
		pls17_02_free( ew ) ;
		return ;
	}
	if( ew->ef_w->routine_0 > 3 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls17_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_02.c" End of File	*/
