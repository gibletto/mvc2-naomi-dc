/* B.B. Hood's player effect 01: its setter, its main, and the speed tables its other file reads. */
#include "player.h"
#include "pl1f_def.h"

/* which way it turns, picked at random: 0, 0, +1 and -1 */
Sint8 pls1f_01_dir_tbl[4] = { 0, 0, 1, -1 } ;

Float pls1f_01_spd_tbl0[200] = {
	-DOT_X( 28.0f ), 165.0f, -DOT_X( 68.0f ), 150.0f, 5.0f, DOT_Y( 16.0f ), -100.0f, DOT_Y( 16.0f ),
	-125.0f, DOT_Y( 73.0f ), -125.0f, DOT_Y( 73.0f ), -125.0f, DOT_Y( 73.0f ), -125.0f, DOT_Y( 73.0f ),
	-140.0f, DOT_Y( 36.0f ), -140.0f, DOT_Y( 36.0f ), -140.0f, DOT_Y( 36.0f ), -140.0f, DOT_Y( 36.0f ),
	DOT_X( 40.0f ), 120.0f, 80.0f, DOT_Y( 64.0f ), DOT_X( 56.0f ), DOT_Y( 72.0f ), DOT_X( 64.0f ), DOT_Y( 80.0f ),
	DOT_X( 40.0f ), DOT_Y( 36.0f ), 80.0f, DOT_Y( 36.0f ), DOT_X( 56.0f ), DOT_Y( 36.0f ), DOT_X( 64.0f ), DOT_Y( 36.0f ),
	DOT_X( 32.0f ), DOT_Y( 88.0f ), DOT_X( 40.0f ), DOT_Y( 96.0f ), 80.0f, DOT_Y( 104.0f ), DOT_X( 56.0f ), 240.0f,
	DOT_X( 32.0f ), 120.0f, DOT_X( 40.0f ), DOT_Y( 64.0f ), 80.0f, DOT_Y( 72.0f ), DOT_X( 56.0f ), DOT_Y( 80.0f ),
	-DOT_X( 64.0f ), DOT_Y( 80.0f ), -DOT_X( 68.0f ), DOT_Y( 88.0f ), -120.0f, DOT_Y( 96.0f ), -DOT_X( 76.0f ), DOT_Y( 104.0f ),
	-40.0f, DOT_Y( 34.0f ), -DOT_X( 32.0f ), DOT_Y( 36.0f ), -DOT_X( 40.0f ), DOT_Y( 38.0f ), -80.0f, DOT_Y( 40.0f ),
	DOT_X( 64.0f ), DOT_Y( 32.0f ), 120.0f, DOT_Y( 40.0f ), DOT_X( 80.0f ), DOT_Y( 48.0f ), DOT_X( 88.0f ), 120.0f,
	DOT_X( 40.0f ), DOT_Y( 36.0f ), 80.0f, DOT_Y( 36.0f ), DOT_X( 56.0f ), DOT_Y( 36.0f ), DOT_X( 64.0f ), DOT_Y( 36.0f ),
	-DOT_X( 16.0f ), DOT_Y( 32.0f ), -DOT_X( 16.0f ), DOT_Y( 32.0f ), -DOT_X( 16.0f ), DOT_Y( 32.0f ), -DOT_X( 16.0f ), DOT_Y( 32.0f ),
	-DOT_X( 7.0f ), DOT_Y( 48.0f ), -DOT_X( 7.0f ), DOT_Y( 48.0f ), -DOT_X( 7.0f ), DOT_Y( 48.0f ), -DOT_X( 7.0f ), DOT_Y( 48.0f ),
	-DOT_X( 16.0f ), DOT_Y( 8.0f ), -40.0f, DOT_Y( 16.0f ), -DOT_X( 32.0f ), DOT_Y( 24.0f ), -DOT_X( 40.0f ), DOT_Y( 32.0f ),
	-10.0f, DOT_Y( 4.0f ), -10.0f, DOT_Y( 8.0f ), -10.0f, DOT_Y( 12.0f ), -10.0f, DOT_Y( 16.0f ),
	-DOT_X( 8.0f ), DOT_Y( 64.0f ), -120.0f, DOT_Y( 118.0f ), -DOT_X( 58.0f ), DOT_Y( 73.0f ), -DOT_X( 16.0f ), DOT_Y( 137.0f ),
	120.0f, DOT_Y( 8.0f ), DOT_X( 80.0f ), DOT_Y( 16.0f ), DOT_X( 88.0f ), DOT_Y( 24.0f ), 160.0f, DOT_Y( 32.0f ),
	-DOT_X( 32.0f ), DOT_Y( 88.0f ), -DOT_X( 40.0f ), DOT_Y( 96.0f ), -80.0f, DOT_Y( 104.0f ), -DOT_X( 56.0f ), 240.0f,
	DOT_X( 32.0f ), DOT_Y( 116.0f ), 80.0f, 240.0f, DOT_X( 40.0f ), DOT_Y( 106.0f ), DOT_X( 56.0f ), 210.0f,
	-DOT_X( 8.0f ), DOT_Y( 72.0f ), -DOT_X( 16.0f ), DOT_Y( 80.0f ), 0.0f, DOT_Y( 88.0f ), -40.0f, DOT_Y( 96.0f ),
	-DOT_X( 28.0f ), 165.0f, -DOT_X( 68.0f ), 150.0f, 5.0f, DOT_Y( 16.0f ), -100.0f, DOT_Y( 16.0f ),
	-DOT_X( 16.0f ), DOT_Y( 62.0f ), -DOT_X( 13.0f ), DOT_Y( 62.0f ), -DOT_X( 11.0f ), DOT_Y( 62.0f ), -DOT_X( 13.0f ), DOT_Y( 62.0f ),
	-80.0f, DOT_Y( 48.0f ), -80.0f, DOT_Y( 48.0f ), -80.0f, DOT_Y( 48.0f ), -80.0f, DOT_Y( 48.0f ),
	-DOT_X( 20.0f ), DOT_Y( 16.0f ), -40.0f, DOT_Y( 37.0f ), -DOT_X( 10.0f ), DOT_Y( 32.0f ), -DOT_X( 16.0f ), DOT_Y( 44.0f ),
	DOT_X( 4.0f ), DOT_Y( 26.0f ), DOT_X( 8.0f ), DOT_Y( 37.0f ), 10.0f, DOT_Y( 32.0f ), DOT_X( 11.0f ), DOT_Y( 44.0f ),
} ;

Float pls1f_01_spd_tbl1[200] = {
	DOT_X( 8.0f ), DOT_Y( 16.0f ), 120.0f, 150.0f, DOT_X( 58.0f ), DOT_Y( 25.0f ), DOT_X( 16.0f ), DOT_Y( 89.0f ),
	DOT_X( 8.0f ), DOT_Y( 16.0f ), 120.0f, 150.0f, DOT_X( 58.0f ), DOT_Y( 25.0f ), DOT_X( 16.0f ), DOT_Y( 89.0f ),
	DOT_X( 8.0f ), DOT_Y( 16.0f ), 120.0f, 150.0f, DOT_X( 58.0f ), DOT_Y( 25.0f ), DOT_X( 16.0f ), DOT_Y( 89.0f ),
	-DOT_X( 32.0f ), DOT_Y( 48.0f ), -DOT_X( 40.0f ), DOT_Y( 52.0f ), -80.0f, 120.0f, -DOT_X( 56.0f ), DOT_Y( 60.0f ),
	-DOT_X( 32.0f ), DOT_Y( 32.0f ), -DOT_X( 40.0f ), DOT_Y( 36.0f ), -80.0f, DOT_Y( 40.0f ), -DOT_X( 56.0f ), DOT_Y( 44.0f ),
	DOT_X( 32.0f ), DOT_Y( 88.0f ), DOT_X( 40.0f ), DOT_Y( 96.0f ), 80.0f, DOT_Y( 104.0f ), DOT_X( 56.0f ), 240.0f,
	DOT_X( 32.0f ), 120.0f, DOT_X( 40.0f ), DOT_Y( 64.0f ), 80.0f, DOT_Y( 72.0f ), DOT_X( 56.0f ), DOT_Y( 80.0f ),
	DOT_X( 8.0f ), DOT_Y( 16.0f ), 120.0f, 150.0f, DOT_X( 58.0f ), DOT_Y( 25.0f ), DOT_X( 16.0f ), DOT_Y( 89.0f ),
	DOT_X( 8.0f ), DOT_Y( 16.0f ), 120.0f, 150.0f, DOT_X( 58.0f ), DOT_Y( 25.0f ), DOT_X( 16.0f ), DOT_Y( 89.0f ),
	DOT_X( 64.0f ), DOT_Y( 32.0f ), 120.0f, DOT_Y( 40.0f ), DOT_X( 80.0f ), DOT_Y( 48.0f ), DOT_X( 88.0f ), 120.0f,
	DOT_X( 8.0f ), DOT_Y( 16.0f ), 120.0f, 150.0f, DOT_X( 58.0f ), DOT_Y( 25.0f ), DOT_X( 16.0f ), DOT_Y( 89.0f ),
	-DOT_X( 16.0f ), DOT_Y( 32.0f ), -40.0f, DOT_Y( 40.0f ), -DOT_X( 32.0f ), DOT_Y( 48.0f ), -DOT_X( 40.0f ), 120.0f,
	-DOT_X( 8.0f ), DOT_Y( 48.0f ), -DOT_X( 8.0f ), DOT_Y( 48.0f ), -DOT_X( 8.0f ), DOT_Y( 48.0f ), -DOT_X( 8.0f ), DOT_Y( 48.0f ),
	-DOT_X( 16.0f ), DOT_Y( 8.0f ), -40.0f, DOT_Y( 16.0f ), -DOT_X( 32.0f ), DOT_Y( 24.0f ), -DOT_X( 40.0f ), DOT_Y( 32.0f ),
	-DOT_X( 8.0f ), DOT_Y( 4.0f ), -DOT_X( 8.0f ), DOT_Y( 8.0f ), -DOT_X( 8.0f ), DOT_Y( 12.0f ), -DOT_X( 8.0f ), DOT_Y( 16.0f ),
	-DOT_X( 8.0f ), DOT_Y( 64.0f ), -120.0f, DOT_Y( 118.0f ), -DOT_X( 58.0f ), DOT_Y( 73.0f ), -DOT_X( 16.0f ), DOT_Y( 137.0f ),
	120.0f, DOT_Y( 8.0f ), DOT_X( 80.0f ), DOT_Y( 16.0f ), DOT_X( 88.0f ), DOT_Y( 24.0f ), 160.0f, DOT_Y( 32.0f ),
	-80.0f, DOT_Y( 72.0f ), -DOT_X( 56.0f ), DOT_Y( 80.0f ), -DOT_X( 64.0f ), DOT_Y( 88.0f ), -120.0f, DOT_Y( 96.0f ),
	80.0f, DOT_Y( 100.0f ), DOT_X( 64.0f ), DOT_Y( 96.0f ), DOT_X( 56.0f ), DOT_Y( 90.0f ), 120.0f, DOT_Y( 82.0f ),
	-DOT_X( 8.0f ), DOT_Y( 72.0f ), -DOT_X( 16.0f ), DOT_Y( 80.0f ), 0.0f, DOT_Y( 88.0f ), -40.0f, DOT_Y( 96.0f ),
	DOT_X( 40.0f ), DOT_Y( 96.0f ), 80.0f, DOT_Y( 92.0f ), DOT_X( 32.0f ), DOT_Y( 108.0f ), DOT_X( 56.0f ), DOT_Y( 104.0f ),
	-30.0f, DOT_Y( 61.0f ), -DOT_X( 20.0f ), DOT_Y( 61.0f ), -30.0f, DOT_Y( 61.0f ), -DOT_X( 16.0f ), DOT_Y( 61.0f ),
	DOT_X( 32.0f ), DOT_Y( 48.0f ), DOT_X( 32.0f ), DOT_Y( 48.0f ), DOT_X( 32.0f ), DOT_Y( 48.0f ), DOT_X( 32.0f ), DOT_Y( 48.0f ),
	-DOT_X( 20.0f ), DOT_Y( 16.0f ), -40.0f, DOT_Y( 37.0f ), -DOT_X( 10.0f ), DOT_Y( 32.0f ), -DOT_X( 16.0f ), DOT_Y( 44.0f ),
	5.0f, DOT_Y( 27.0f ), DOT_X( 7.0f ), DOT_Y( 39.0f ), DOT_X( 5.0f ), DOT_Y( 34.0f ), 0.0f, DOT_Y( 46.0f ),
} ;

Float pls1f_01_spd_tbl2[200] = {
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	DOT_X( 32.0f ), DOT_Y( 88.0f ), DOT_X( 40.0f ), DOT_Y( 96.0f ), 80.0f, DOT_Y( 104.0f ), DOT_X( 56.0f ), 240.0f,
	DOT_X( 32.0f ), 120.0f, DOT_X( 40.0f ), DOT_Y( 64.0f ), 80.0f, DOT_Y( 72.0f ), DOT_X( 56.0f ), DOT_Y( 80.0f ),
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	-DOT_X( 17.0f ), DOT_Y( 78.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	DOT_X( 64.0f ), DOT_Y( 32.0f ), 120.0f, DOT_Y( 40.0f ), DOT_X( 80.0f ), DOT_Y( 48.0f ), DOT_X( 88.0f ), 120.0f,
	-DOT_X( 17.0f ), DOT_Y( 62.0f ), 50.0f, DOT_Y( 37.0f ), DOT_X( 40.0f ), DOT_Y( 103.0f ), DOT_X( 7.0f ), DOT_Y( 115.0f ),
	-DOT_X( 16.0f ), DOT_Y( 32.0f ), -40.0f, DOT_Y( 40.0f ), -DOT_X( 32.0f ), DOT_Y( 48.0f ), -DOT_X( 40.0f ), 120.0f,
	-15.0f, DOT_Y( 48.0f ), -15.0f, DOT_Y( 48.0f ), -15.0f, DOT_Y( 48.0f ), -15.0f, DOT_Y( 48.0f ),
	-DOT_X( 16.0f ), DOT_Y( 8.0f ), -40.0f, DOT_Y( 16.0f ), -DOT_X( 32.0f ), DOT_Y( 24.0f ), -DOT_X( 40.0f ), DOT_Y( 32.0f ),
	-DOT_X( 10.0f ), DOT_Y( 4.0f ), -DOT_X( 10.0f ), DOT_Y( 8.0f ), -DOT_X( 10.0f ), DOT_Y( 12.0f ), -DOT_X( 10.0f ), DOT_Y( 16.0f ),
	-DOT_X( 8.0f ), DOT_Y( 48.0f ), -120.0f, DOT_Y( 118.0f ), -DOT_X( 58.0f ), DOT_Y( 57.0f ), -DOT_X( 16.0f ), DOT_Y( 137.0f ),
	120.0f, DOT_Y( 8.0f ), DOT_X( 80.0f ), DOT_Y( 16.0f ), DOT_X( 88.0f ), DOT_Y( 24.0f ), 160.0f, DOT_Y( 32.0f ),
	-DOT_X( 16.0f ), DOT_Y( 72.0f ), -40.0f, DOT_Y( 80.0f ), -DOT_X( 32.0f ), DOT_Y( 88.0f ), -DOT_X( 40.0f ), DOT_Y( 96.0f ),
	DOT_X( 32.0f ), 180.0f, 80.0f, DOT_Y( 80.0f ), DOT_X( 40.0f ), DOT_Y( 74.0f ), DOT_X( 56.0f ), DOT_Y( 66.0f ),
	-DOT_X( 8.0f ), DOT_Y( 72.0f ), -DOT_X( 16.0f ), DOT_Y( 80.0f ), 0.0f, DOT_Y( 88.0f ), -40.0f, DOT_Y( 96.0f ),
	-DOT_X( 56.0f ), DOT_Y( 148.0f ), -DOT_X( 32.0f ), DOT_Y( 144.0f ), -80.0f, DOT_Y( 156.0f ), -DOT_X( 40.0f ), DOT_Y( 152.0f ),
	-DOT_X( 16.0f ), DOT_Y( 66.0f ), -30.0f, DOT_Y( 66.0f ), -DOT_X( 16.0f ), DOT_Y( 66.0f ), -DOT_X( 20.0f ), DOT_Y( 66.0f ),
	-DOT_X( 16.0f ), DOT_Y( 96.0f ), -DOT_X( 16.0f ), DOT_Y( 96.0f ), -DOT_X( 16.0f ), DOT_Y( 96.0f ), -DOT_X( 16.0f ), DOT_Y( 96.0f ),
	-DOT_X( 22.0f ), DOT_Y( 16.0f ), -DOT_X( 26.0f ), DOT_Y( 37.0f ), -20.0f, DOT_Y( 32.0f ), -30.0f, DOT_Y( 44.0f ),
	DOT_X( 2.0f ), DOT_Y( 26.0f ), DOT_X( 8.0f ), DOT_Y( 37.0f ), DOT_X( 4.0f ), DOT_Y( 32.0f ), 10.0f, DOT_Y( 44.0f ),
} ;
static void pls1f_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1f_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 0 ;
		ew->ef_no = 0x1f01 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 1 ;
		ew->ef_no = 0x1f01 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_01_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = 2 ;
		ew->ef_no = 0x1f01 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	return ew ;
}

extern void pls1f_01_init( PLAYER_WORK *ew );
extern void pls1f_01_move( PLAYER_WORK *ew );
extern void pls1f_01_next( PLAYER_WORK *ew );
extern void pls1f_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1f_01_main( PLAYER_WORK *ew )
{
	static void ( *pls1f_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls1f_01_init, pls1f_01_move, pls1f_01_next, pls1f_01_free,
	} ;

	PLAYER_WORK *w = ew ;
	pls1f_01_jmp[w->routine_0]( w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1f_01.c" End of File	*/
