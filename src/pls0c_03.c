/* Spider-Man's player effect 03: the web's four other pieces, by type: the line that drops
   from the ground (0), the pieces that fly off sideways and bounce back (1), the one that trails
   behind (2) and the ball that falls to the ground (3). */
#include "player.h"
#include "pl_char.h"
#include "pl0c_def.h"

/* the work takes the master's look */
#define PLS0C_03_LOOK( ew, pl_w ) \
	CHAR_PART_COPY( ew, pl_w ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( pl_w )->pl_no ; \
	( ew )->pl_type = ( pl_w )->pl_type ; \
	( ew )->scale.x = ( pl_w )->scale.x ; \
	( ew )->scale.y = ( pl_w )->scale.y ; \
	( ew )->atck_level = ( pl_w )->atck_level ; \
	( ew )->work_no = ( pl_w )->work_no ; \
	( ew )->col_cnt = ( pl_w )->col_cnt ; \
	( ew )->scale = ( pl_w )->scale

static void pls0c_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *pls0c_03_set( PLAYER_WORK *pl_w, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0xc03 ;
		ew->type = type ;
		ew->work_func = pls0c_03_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls0c_03_a_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_b_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_c_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_d_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_03_main( PLAYER_WORK *ew )
{
	static void ( *pls0c_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_03_a_main, pls0c_03_b_main, pls0c_03_c_main, pls0c_03_d_main,
	} ;

	pls0c_03_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls0c_03_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_03_a_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0c_03_a_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_03_a_init, pls0c_03_a_move, pls0c_03_free,
	} ;

	pls0c_03_a_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0c_03_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS0C_03_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	ew->prio = 7 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_03_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( pl_w->char_no.b.grp != Char_Appear || pl_w->char_no.b.no > 2 || ( pl_w->cg_type & 2 ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

static void pls0c_03_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_03_b_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0c_03_b_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_03_b_init, pls0c_03_b_move, pls0c_03_free,
	} ;
	PL0C_LOCAL_W *lw = ( PL0C_LOCAL_W * )pl_w->tlocal ;

	if( lw->win_cut_flag || pl_w->char_no.b.grp != Char_Win || pl_w->char_no.b.no != 1 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	pls0c_03_b_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0c_03_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS0C_03_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	ew->spd.x = DOT_X( 0.125f ) ;
	ew->acc.x = -0.013020833f ;
	pl_char_set( ew, Char_Effect, 22 ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_03_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	float x ;

	if( !ew->routine_1 ){
		if( pl_w->cg_flag1 ){
			ew->routine_1 ++ ;
			pl_char_move( ew ) ;
		}
		return ;
	}
	else pl_char_move( ew ) ;
	x = ew->pos.x ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	x -= ew->pos.x ;
	if( x < 0.0f ) x = -x ;
	if( x >= 0.44921875f ){
		ew->acc.x = -ew->acc.x ;
		ew->spd.x = ew->acc.x * 2.0f ;
	}
}

static void pls0c_03_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_c_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_03_c_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0c_03_c_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_03_c_init, pls0c_03_c_move, pls0c_03_free,
	} ;

	pls0c_03_c_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0c_03_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int no ;

	PLS0C_03_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ew->sub_type = ew->disp_dir ;
	no = pl_w->type ;
	if( ew->disp_dir ){
		ew->pos.x += DOT_X( 160.0f ) ;
		no += 3 ;
		ew->disp_dir = 0 ;
	}
	pl_char_set( ew, Char_Effect, no + 26 ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_03_c_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_1 && pl_w->act_st == N_SPECIAL ){
		ew->pos = pl_w->pos ;
		if( ew->sub_type ) ew->pos.x += DOT_X( 160.0f ) ;
		if( !( pl_w->cg_type & 2 ) ) return ;
	}
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
}

static void pls0c_03_d_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_d_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0c_03_d_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0c_03_d_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0c_03_d_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0c_03_d_init, pls0c_03_d_move, pls0c_03_free,
	} ;

	pls0c_03_d_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0c_03_d_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	float dx ;

	PLS0C_03_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->prio = 0 ;
	ew->spd.x = 20.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	dx = DOT_X( 64.0f ) ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
		dx = -DOT_X( 64.0f ) ;
	}
	ew->pos.x = pl_w->pos.x + dx ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 50.0f ) ;
	pl_char_set( ew, Char_Effect, 21 ) ;
}

/*--------------------------------------------------------------*/
static void pls0c_03_d_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls0c_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0c_03.c" End of File	*/
