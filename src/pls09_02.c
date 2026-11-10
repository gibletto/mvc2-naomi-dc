/* Iceman's player effect 02: the shadow that follows his hand, by the frame of his animation,
   as long as his own work asks for it. */
#include "player.h"
#include "pl_char.h"
#include "pl09_def.h"

/* where it sits from him, in 1/256: x and y for each cg_flag1 of his animation */
static Sint16 pls09_02_ofs_tbl[330] = {
	0, 0, -60, -61, -83, -65, -81, -65,
	-70, -60, -54, -51, -91, -68, -90, -68,
	16, -96, -66, -74, -35, -53, -66, -65,
	-70, -66, -74, -52, -77, -87, -76, -83,
	-66, -65, -58, -69, -58, -69, -80, -12,
	-68, -51, -67, -51, -66, -51, -47, -43,
	-43, -37, -41, -34, -12, -38, 0, -37,
	17, -36, 15, -41, 17, -41, 1, -39,
	-10, -39, -41, -44, -50, -49, -27, -51,
	-26, -47, -11, -45, -1, -45, 10, -46,
	9, -46, -9, -47, -21, -51, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54,
	-48, -37, -58, -58, -55, -50, -49, -21,
	15, -58, 15, -63, 27, -49, 31, -57,
	-19, -43, -40, -43, -47, -51, -41, -57,
	59, -78, 21, -72, -46, -47, 38, -63,
	-42, -46, 1, -44, 26, -49, -31, -54,
	-67, -51, 0, 0, -15, -31, -38, -24,
	-29, -22, 0, 0, 0, 0, 0, 0,
	-27, -28, -11, -129, -42, -109, -59, -74,
	-26, -55, -45, -60, -45, -107, -12, -131,
	-32, -25, -60, -54, -56, -38, -43, -30,
	-21, -14, -53, -15, -47, -11, -25, -17,
	19, -18, 66, -77, 43, -118, -44, -118,
	-26, -27, -15, -128, 27, -122, 68, -75,
	8, -19, -48, -36, -62, -21, -62, -19,
	-24, -15, -44, -27, -54, -34, -62, -54,
	-30, -16, -15, -128, -15, -129, -15, -128,
	-15, -129, -12, -129, -11, -130, -12, -129,
	-12, -130, -16, -127, -16, -128, -16, -127,
	-16, -128, -13, -128, -13, -130, -13, -128,
	-13, -130, -15, -126, -15, -128, -15, -126,
	-15, -126, -15, -126, -15, -128, -15, -126,
	-15, -128, 50, -101, -97, -38, -97, -38,
	-97, -38, -97, -38, 6, -56, 51, -99,
	-87, -43, -86, -44, 10, -56, 7, -144,
	-113, -21, -113, -21, 71, -137, 71, -137,
	7, -56,
} ;

static void pls09_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls09_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls09_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x902 ;
	}
	return ew ;
}

static void pls09_02_init( PLAYER_WORK *ew );
static void pls09_02_move( PLAYER_WORK *ew );
static void pls09_02_end( PLAYER_WORK *ew );
static void pls09_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls09_02_main( PLAYER_WORK *ew )
{
	static void ( *pls09_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls09_02_init, pls09_02_move, pls09_02_end, pls09_02_free,
	} ;

	pls09_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls09_02_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	pl_char_set( ew, 20, 3 ) ;
	pls09_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )pl_w->tlocal ;
	Float x ;

	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( !lw->arm_flag ){
		ew->routine_0 ++ ;
		pls09_02_end( ew ) ;
		return ;
	}
	ew->disp_flag = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->cg_flag1 ){
		ew->disp_flag = 1 ;
		x = DOT_X( pls09_02_ofs_tbl[ew->master_w->cg_flag1 * 2] ) ;
		if( ew->master_w->disp_dir ) x = -x ;
		ew->pos.x += x ;
		ew->pos.y -= DOT_Y( pls09_02_ofs_tbl[ew->master_w->cg_flag1 * 2 + 1] ) ;
	}
	if( ew->master_w->char_no.b.grp >= Char_Stand_P ) ew->disp_flag = 0 ;
	if( !ew->master_w->disp_flag ) ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls09_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls09_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls09_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls09_02.c" End of File	*/
