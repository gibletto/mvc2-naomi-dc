/* Rogue's effect 01 (her pls0a): a work that shows the animation her own animation frames
   ask for (the high byte of cg_rival) at her place, while the frame asks. */
#include "player.h"
#include "ef_char.h"

/* the effect's own work, from obj_flag */
typedef struct PLS0A_01_LOCAL_W {
	Uint32	unk_00;		/* 000 */
	Uint32	no;		/* 004: the animation it shows */
} PLS0A_01_LOCAL_W;
#define PLS0A_01_LW( w )	( ( PLS0A_01_LOCAL_W * )&( w )->obj_flag )

static void pls0a_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0a_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls0a_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0xa01 ;
		ew->pl_type = pl_w->pl_type ;
	}
	return ew ;
}

static void pls0a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void pls0a_01_main( PLAYER_WORK *ew )
{
	static void ( *pls0a_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0a_01_init, pls0a_01_move, pls0a_01_end, pls0a_01_free,
	} ;

	pls0a_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls0a_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	PLS0A_01_LW( ew )->no = 0 ;
	pls0a_01_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0a_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint32 no ;

	ew->disp_flag = 0 ;
	if( !pl_w->disp_flag ) return ;
	no = HI_BYTE( pl_w->cg_rival ) ;
	if( no ){
		if( no < 32 ){
			ew->disp_flag = 1 ;
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->disp_dir = pl_w->disp_dir ;
			ew->prio = pl_w->prio ;
			if( PLS0A_01_LW( ew )->no != no ){
				PLS0A_01_LW( ew )->no = no ;
				ef_char_set( ew, 27, no ) ;
			} else if( !vs_sys_work.unk_44 ) ef_char_move( ew ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls0a_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls0a_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0a_01.c" End of File	*/
