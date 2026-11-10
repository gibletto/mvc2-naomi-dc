/* Abyss's second form's row of twelve marks (effect 01): a controller (type 0) lets them show one after
   another along the floor, and each mark (type 1) lights up as the count reaches it. */
#include "player.h"
#include "ef_char.h"
#include "pl19_def.h"

/* each work's own work, from obj_flag */
typedef struct PLS19_01_LOCAL_W {
	Uint8	cnt;		/* 000: the controller's count of the marks shown */
} PLS19_01_LOCAL_W;
#define PLS19_01_LW( w )	( ( PLS19_01_LOCAL_W * )&( w )->obj_flag )

/* the work being run, and its controller's */
static PLS19_01_LOCAL_W *pls19_01_lw ;
static PLS19_01_LOCAL_W *pls19_01_master_lw ;

static void pls19_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls19_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls19_01_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1901 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The twelve marks, each told its place in the row	*/
/*==============================================================*/
static void pls19_01_child_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint8 i ;
	Uint8 n = 0 ;

	for( i = 0 ; i < 12 ; i ++ ){
		if( ( ew = work_get( pl_w, W_PLS, LIST_BEHIND ) ) ){
			ew->work_func = pls19_01_main ;
			ew->type = 1 ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x1901 ;
			ew->sub_no = n ;
			n ++ ;
		}
	}
}

static void pls19_01_init( PLAYER_WORK *ew );
static void pls19_01_move( PLAYER_WORK *ew );
static void pls19_01_child_init( PLAYER_WORK *ew );
static void pls19_01_child_move( PLAYER_WORK *ew );
static void pls19_01_end( PLAYER_WORK *ew );
static void pls19_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls19_01_main( PLAYER_WORK *ew )
{
	static void ( *pls19_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls19_01_init, pls19_01_move, pls19_01_end, pls19_01_free,
	} ;
	static void ( *pls19_01_child_jmp[] )( PLAYER_WORK *ew ) = {
		pls19_01_child_init, pls19_01_child_move, pls19_01_end, pls19_01_free,
	} ;

	if( !ew->type ){
		pls19_01_lw = PLS19_01_LW( ew ) ;
		pls19_01_jmp[ew->routine_0]( ew ) ;
	} else {
		pls19_01_lw = PLS19_01_LW( ew ) ;
		pls19_01_master_lw = PLS19_01_LW( ew->master_w ) ;
		pls19_01_child_jmp[ew->routine_0]( ew ) ;
	}
}

/*==============================================================*/
/*			A mark's place on the floor: its master's, and		*/
/*			its number of steps of 64 pixels behind				*/
/*==============================================================*/
static void pls19_01_pos_set( PLAYER_WORK *ew )
{
	Float ofs ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ofs = ( ew->sub_no << 6 ) * 1.6666666f ;
	if( !ew->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
}

/*==============================================================*/
/*			The controller steps along, one more mark each		*/
/*			frame until the eleventh							*/
/*==============================================================*/
static void pls19_01_count( PLAYER_WORK *ew )
{
	if( pls19_01_lw->cnt < 11 ){
		if( --ew->timer1 <= 0 ){
			ew->timer1 = 1 ;
			pls19_01_lw->cnt ++ ;
			ew->pos.x += DOT_X( 32.0f ) ;
		}
	}
}

/*==============================================================*/
/*			Whether a mark is on the screen: 0 once it is		*/
/*			past the edge it faces								*/
/*==============================================================*/
static int pls19_01_check( PLAYER_WORK *ew )
{
	int ret = 1 ;

	if( !ew->disp_dir && ew->pos.x < scr_work.scr_left - ew->size_back * 1.6666666f ) ret = 0 ;
	else if( ew->disp_dir && ew->pos.x > scr_work.scr_right + DOT_X( ew->size_back ) ) ret = 0 ;
	return ret ;
}

/*==============================================================*/
static void pls19_01_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	ew->disp_dir = 0 ;
	ew->prio = 11 ;
	ew->scale.x = 1.01f ;
	ew->scale.y = 1.0f ;
	ew->pos = ew->master_w->pos ;
	ew->pos.y += DOT_Y( 64.0f ) ;
	pls19_01_lw->cnt = 0 ;
	ew->timer1 = 1 ;
	pls19_01_child_set( ew ) ;
	pls19_01_move( ew ) ;
}

static void pls19_01_wait( PLAYER_WORK *ew );
static void pls19_01_fade( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls19_01_move( PLAYER_WORK *ew )
{
	static void ( *pls19_01_step_jmp[] )( PLAYER_WORK *ew ) = {
		pls19_01_wait, pls19_01_fade,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls19_01_step_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Waits for the master's timer, then counts the		*/
/*			marks												*/
/*==============================================================*/
static void pls19_01_wait( PLAYER_WORK *ew )
{
	if( ew->master_w->timer0 <= 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 10 ;
		ew->scale.y = 1.0f ;
	}
	pls19_01_count( ew ) ;
}

/*==============================================================*/
/*			Shrinks away										*/
/*==============================================================*/
static void pls19_01_fade( PLAYER_WORK *ew )
{
	ew->scale.y += ( 0.0f - ew->scale.y ) / ew->timer0 ;
	if( --ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls19_01_child_init( PLAYER_WORK *ew )
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
	ew->size_top = 64 ;
	ew->size_bottom = 64 ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	pls19_01_child_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls19_01_child_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !ew->sub_no ) no = 8 ;
	else{
		if( ew->sub_no > pls19_01_master_lw->cnt ){
			ew->disp_flag = 0 ;
			return ;
		}
		if( ew->sub_no == pls19_01_master_lw->cnt ) no = 10 ;
		else no = 9 ;
	}
	ew->disp_flag = pls19_01_check( ew ) ;
	if( ew->disp_flag ) ef_char_set( ew, 27, no ) ;
	pls19_01_pos_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls19_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls19_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls19_01.c" End of File	*/
