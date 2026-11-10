/* Jin's effect 03 (character 37's shot 03): the lights of his Saotome Cyclone's spin. Type 0 is
   three of them that bob beside him and burst when the spin is over; type 1 is the glow that
   follows him. */
#include "player.h"
#include "pl_char.h"
#include "pl37_def.h"

/* the animation frame each of the three starts at, and where it starts above Jin, and how long
   into its bob */
static Sint8 shl37_03_frame_tbl[] = { 0, 2, 4, 0 } ;
static Sint16 shl37_03_y_tbl[] = { 192, 256, 320, 0 } ;
static Sint8 shl37_03_time_tbl[] = { 0, 32, 64, 0 } ;

/* the bob: up and down in 128 frames */
static Sint8 shl37_03_bob_tbl[] = {
	0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15,
	-16, -17, -18, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29, -30, -31,
	-31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16,
	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
} ;

static void shl37_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl37_03_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl37_03_main ;
		ew->ef_no = 0x3703 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl37_03_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_03_type_1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_03_main( PLAYER_WORK *ew )
{
	static void ( *shl37_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_03_type_0, shl37_03_type_1,
	} ;

	shl37_03_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl37_03_0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_03_0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_03_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_03_type_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_03_0_init, shl37_03_0_move, shl37_03_end, shl37_03_free,
	} ;

	shl37_03_type_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_03_0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	pl_char_set2( ew, Char_Effect, 5, shl37_03_frame_tbl[ew->sub_type] ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.y += DOT_Y( shl37_03_y_tbl[ew->sub_type] ) ;
	ew->timer0 = shl37_03_time_tbl[ew->sub_type] ;
	shl37_03_0_move( ew, pl_w ) ;
}

static void shl37_03_0_bob( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_03_0_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_03_0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_03_0_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_03_0_bob, shl37_03_0_burst,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	shl37_03_0_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It bobs above Jin until the spin is over			*/
/*==============================================================*/
static void shl37_03_0_bob( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	pl_char_move( ew ) ;
	ew->timer0 -- ;
	ew->timer0 &= 127 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.x += DOT_X( shl37_03_bob_tbl[ew->timer0] ) ;
	if( pl_lw->spin_end_flag ){
		pl_char_set( ew, Char_Effect, 6 ) ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_03_0_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

static void shl37_03_1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_03_1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_03_type_1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_03_type_1_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_03_1_init, shl37_03_1_move, shl37_03_end, shl37_03_free,
	} ;

	shl37_03_type_1_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_03_1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	shl37_03_1_move( ew, pl_w ) ;
}

static void shl37_03_1_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_03_1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_03_1_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_03_1_follow,
	} ;

	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl37_03_1_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It follows Jin, and in the spin's first step		*/
/*			sits on his opponent								*/
/*==============================================================*/
static void shl37_03_1_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *pl_lw = ( PL37_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_w->routine_2 == 1 ){
		PLAYER_WORK *em_w = pl_w->target_w ;

		ew->pos.y = em_w->pos.y ;
		ew->pos.y -= DOT_Y( 8.0f ) ;
	}
	if( pl_lw->spin_end_flag ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl37_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl37_03.c" End of File	*/
